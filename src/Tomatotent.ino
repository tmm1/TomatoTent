#include "Particle.h"
#include <Arduino.h>
#include "softap_http.h"
#include "XPT2046_Touch.h"
#include "Adafruit_mfGFX.h"
#include "DFRobot_SHT20.h"
#include "systemStatus.h"
#include "button.h"
#include "tent.h"
#include "screen_manager.h"
#include "Adafruit_ILI9341.h"
#include "assets.h"

PRODUCT_ID(10167);
PRODUCT_VERSION(9);

#define CS_PIN D5
#define TIRQ_PIN A0
#define TFT_DC A1
#define TFT_CS A2

double temp;
double hum;
double waterLevel;
unsigned long dimmerBtnTime = 0;

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, D6);
DFRobot_SHT20 sht20;
Tent tent;
ScreenManager screenManager;
SystemStatus systemStatus;

Timer draw_temp_home(7013, &Tent::doCheckStats, tent);

//sets the timer for the GrowLight Photoperiod
Timer minuteCounter(60000, &SystemStatus::countMinute, systemStatus); //once per minute

Timer minuteCounterTent(60000, &Tent::minutelyTick, tent);

XPT2046_Touchscreen ts(SPI1, 320, 240, CS_PIN, TIRQ_PIN);

SYSTEM_MODE(SEMI_AUTOMATIC);

struct Page {
    const char* url;
    const char* mime_type;
    const char* data;
};

Page myPages[] = {
    { "/index.html", "text/html", index_html },
    { "/rsa-utils/rsa.js", "application/javascript", rsa_js },
    { "/style.css", "text/css", style_css },
    { "/rsa-utils/rng.js", "application/javascript", rng_js },
    { "/rsa-utils/jsbn_2.js", "application/javascript", jsbn_2_js },
    { "/rsa-utils/jsbn_1.js", "application/javascript", jsbn_1_js },
    { "/script.js", "application/javascript", script_js },
    { "/rsa-utils/prng4.js", "application/javascript", prng4_js },
    { nullptr }
};

void myPage(const char* url, ResponseCallback* cb, void* cbArg, Reader* body, Writer* result, void* reserved)
{
    Serial.printlnf("handling page %s", url);

    if (strcmp(url, "/index") == 0) {
        Serial.println("sending redirect");
        Header h("Location: /index.html\r\n");
        cb(cbArg, 0, 301, "text/plain", &h);
        return;
    }

    int8_t idx = 0;
    for (;; idx++) {
        Page& p = myPages[idx];
        if (!p.url) {
            idx = -1;
            break;
        } else if (strcmp(url, p.url) == 0) {
            break;
        }
    }

    if (idx == -1) {
        cb(cbArg, 0, 404, nullptr, nullptr);
    } else {
        cb(cbArg, 0, 200, myPages[idx].mime_type, nullptr);
        result->write(myPages[idx].data);
    }
}

// Press SETUP for 3 seconds to make the Photon enter Listening mode
// Navigate to http://192.168.0.1 to setup Wi-Fi

void setup_handler()
{
    screenManager.wifiSetupScreen();
    tent.tp->stop();
    tent.tp1->stop();
}

void setup_finished_handler()
{
    tent.tp->start();
    tent.tp1->start();
    screenManager.homeScreen();
}

STARTUP(
    softap_set_application_page_handler(myPage, nullptr);
    pinMode(FAN_SPEED_PIN, OUTPUT);
    analogWrite(FAN_SPEED_PIN, 255, 25000);)

void setup()
{
    System.set(SYSTEM_CONFIG_SOFTAP_PREFIX, "TomatoTent");

    System.on(setup_begin, setup_handler);
    System.on(setup_end, setup_finished_handler);

    /*SET WIFI 
        Can remember up to 5 Networks,
        Set through Particle phone app or here in-code
        Needs to be set only once then will remember
    */
    if (WiFi.hasCredentials()) {
        Particle.connect();
    } else {
        WiFi.off();
        //WiFi.setCredentials("WiFi-YY9V", "");
    }
    //END SET WIFI

    // Time.zone(+8);

    tft.begin();
    tft.setRotation(1);
    //DISPLAY BRIGHNESS
    pinMode(TFT_BRIGHTNESS_PIN, OUTPUT);
    pinMode(GROW_LIGHT_BRIGHTNESS_PIN, OUTPUT);
    pinMode(GROW_LIGHT_ON_OFF_PIN, OUTPUT);
    pinMode(DIM_PIN, INPUT_PULLUP);

    //TOUCH
    ts.begin();
    ts.setRotation(3); // 1 for 2.4 screen, 3 for 2.8
    //END TOUCH

    // Init SHT20 Sensor
    sht20.initSHT20();
    delay(255);
    sht20.checkSHT20();

    //REMOTE FUNCTIONS
    Particle.variable("temperature", temp);
    Particle.variable("humidity", hum);
    //Particle.variable("fanSpeed", fanSpeed);

    //END REMOTE FUNCTIONS

    screenManager.homeScreen();

    tent.begin();

    //was there a grow in process before (re)booting?
    if (systemStatus.getDayCount() > -1) {

        if (systemStatus.isDay()) { //was the light on when we restarted?
            tent.growLight("HIGH");
        }

        tent.doCheckStats(); //First time right away
        draw_temp_home.start();

        systemStatus.countMinute(); //after restart
        minuteCounter.start();
        minuteCounterTent.start();

        //for updates from earlier version that don't have temp units
        if (systemStatus.getTempUnit() != 'F' && systemStatus.getTempUnit() != 'C') {
            systemStatus.setTempUnit('F');
        }
        //

    } else {
        systemStatus.init();
    }
}

void touchHandler(void)
{
    tent.displayLightHigh(); // Switch on Displaylight on touch

    TS_Point p = ts.getPosition();
    p.x += 20; // calibration

    //WAS A BUTTON TOUCHED - And which one?
    uint8_t c { 0 };

    for (c = 0; c < (sizeof(screenManager.buttons) / sizeof(screenManager.buttons[0])); ++c) {
        Button& btn = screenManager.buttons[c];
        if (!btn.isPressed(p.x, p.y)) {
            continue;
        }
        if (btn.getStatus() != "none") {
            continue;
        }

        btn.setStatus("pressed");

        if (btn.getName() == "startGrowBtn") {
            tent.growLight("HIGH");
            systemStatus.setDayCount(1);

            screenManager.growStartedScreen();

            delay(3000);

            screenManager.homeScreen();

            tent.doCheckStats(); //First time right away
            draw_temp_home.start();

            systemStatus.countMinute(); // First time on new grow
            minuteCounter.start();
            minuteCounterTent.start();

            break;
        }

        if (btn.getName() == "timerBtn") {
            screenManager.timerScreen();
            break;
        }

        if (btn.getName() == "dayCounterBtn") {
            screenManager.cancelScreen();
            break;
        }

        if (btn.getName() == "tempBtn") {
            screenManager.tempUnitScreen();
            break;
        }

        //CANCEL SCREEN

        if (btn.getName() == "cancelScreenOkBtn") {
            screenManager.homeScreen();
            break;
        }

        if (btn.getName() == "terminateBtn") {
            screenManager.cancelConfirmationScreen();
            break;
        }

        if (btn.getName() == "terminateYesBtn") {
            tent.growLight("OFF");
            draw_temp_home.stop();
            tent.fan("OFF");
            minuteCounter.stop();
            minuteCounterTent.stop();

            systemStatus.init();
            screenManager.homeScreen();
            break;
        }

        if (btn.getName() == "terminateNoBtn") {
            screenManager.homeScreen();
            break;
        }

        //TIMER SCREEN
        if (btn.getName() == "timerUpBtn") {
            int dayDuration = systemStatus.getDayDuration() + 60;

            if (dayDuration > 1440) {
                dayDuration = 60;
            }

            systemStatus.setDayDuration(dayDuration);

            tft.fillRect(10, 70, 200, 22, ILI9341_BLACK);
            tft.setTextColor(ILI9341_WHITE);
            tft.setTextSize(2);
            tft.setCursor(10, 70);
            tft.print(String(dayDuration / 60) + " Hours ON");

            tft.setCursor(10, 140);
            tft.fillRect(10, 140, 215, 22, ILI9341_BLACK);
            int nightDuration = (24 * 60) - dayDuration;
            tft.print(String(nightDuration / 60) + " Hours OFF");

            break;
        }
        if (btn.getName() == "timerDownBtn") {
            int dayDuration = systemStatus.getDayDuration() - 60;

            if (dayDuration <= 0) {
                dayDuration = 1440;
            }

            systemStatus.setDayDuration(dayDuration);

            tft.fillRect(10, 70, 200, 22, ILI9341_BLACK);
            tft.setTextColor(ILI9341_WHITE);
            tft.setTextSize(2);
            tft.setCursor(10, 70);
            tft.print(String(dayDuration / 60) + " Hours ON");

            tft.setCursor(10, 140);
            tft.fillRect(10, 140, 215, 22, ILI9341_BLACK);
            int nightDuration = (24 * 60) - dayDuration;
            tft.print(String(nightDuration / 60) + " Hours OFF");
            break;
        }

        if (btn.getName() == "timerOkBtn") {
            screenManager.homeScreen();
            break;
        }

        if (btn.getName() == "wifiBtn") {
            screenManager.wifiScreen();
            break;
        }

        if (btn.getName() == "wifiOnBtn") {
            Particle.connect();
            screenManager.homeScreen();
            break;
        }

        if (btn.getName() == "wifiOffBtn") {
            Particle.disconnect();
            WiFi.off();
            screenManager.homeScreen();
            break;
        }

        if (btn.getName() == "wifiOkBtn") {
            screenManager.homeScreen();
            break;
        }

        if (btn.getName() == "fanBtn") {
            screenManager.fanScreen();
            break;
        }

        if (btn.getName() == "fanAutoBtn") {
            systemStatus.setFanAutoMode(1);
            screenManager.renderButtons(true);
            systemStatus.check_fan();
            break;
        }

        if (btn.getName() == "fanManualBtn") {
            systemStatus.setFanAutoMode(0);
            screenManager.renderButtons(true);
            systemStatus.check_fan();
            break;
        }

        if (btn.getName() == "fanUpBtn") {
            float fanSpeedPercent = systemStatus.getFanSpeed();

            //set to manual
            systemStatus.setFanAutoMode(0);

            screenManager.renderButtons(true);

            if (fanSpeedPercent < 100) {

                fanSpeedPercent += 5;

                systemStatus.setFanSpeed(fanSpeedPercent);
                systemStatus.check_fan();
            }
            break;
        }

        if (btn.getName() == "fanDownBtn") {
            float fanSpeedPercent = systemStatus.getFanSpeed();

            //set to manual
            systemStatus.setFanAutoMode(0);

            screenManager.renderButtons(true);

            if (fanSpeedPercent > 0) {

                fanSpeedPercent -= 5;

                systemStatus.setFanSpeed(fanSpeedPercent);
                systemStatus.check_fan();
            }
            break;
        }

        if (btn.getName() == "fanOkBtn") {
            screenManager.homeScreen();
            break;
        }

        //temp unit select screen
        if (btn.getName() == "tempCelsiusBtn") {
            systemStatus.setTempUnit('C');
            screenManager.renderButtons(true);
            screenManager.homeScreen();
            tent.check_temperature(systemStatus.getTempUnit());
            break;
        }

        if (btn.getName() == "tempFahrenheitBtn") {
            systemStatus.setTempUnit('F');
            screenManager.renderButtons(true);
            screenManager.homeScreen();
            tent.check_temperature(systemStatus.getTempUnit());
            break;
        }
    }
}

void loop(void)
{
    if (ts.touched()) {
        touchHandler();
        delay(10);

    } else {
        screenManager.renderButtons();
    }

    bool dimmerBtnVal = digitalRead(DIM_PIN);
    if (dimmerBtnVal == LOW) {
        unsigned long now = millis();
        if ((now - dimmerBtnTime) >= 1000 || dimmerBtnTime == 0) {
            dimmerBtnTime = now;
            tent.dimGrowLight();
        }
    }

    if (tent.getCheckStats()) {
        tent.check_temperature(systemStatus.getTempUnit());
        tent.check_humidity();
        // tent.check_waterlevel(); // removed for stand alone controller
        systemStatus.check_fan();
        tent.resetCheckStats();
    }
}
