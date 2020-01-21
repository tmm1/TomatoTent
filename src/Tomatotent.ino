#include "Particle.h"
#include <Arduino.h>
#include "softap_http.h"
#include "DFRobot_SHT20.h"
#include "tent_state.h"
#include "button.h"
#include "tent.h"
#include "screen_manager.h"
#include "assets.h"

PRODUCT_ID(10167);
PRODUCT_VERSION(9);

double temp;
double hum;
double waterLevel;
unsigned long dimmerBtnTime = 0;

DFRobot_SHT20 sht20;

Tent tent;
ScreenManager screenManager;

Timer draw_temp_home(7013, &Tent::doCheckStats, tent);
Timer minutelyTicker(60000, &minutelyTick);

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
    screenManager.wifiSplashScreen();
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

    screenManager.setup();

    //DISPLAY BRIGHNESS
    pinMode(TFT_BRIGHTNESS_PIN, OUTPUT);
    pinMode(GROW_LIGHT_BRIGHTNESS_PIN, OUTPUT);
    pinMode(GROW_LIGHT_ON_OFF_PIN, OUTPUT);
    pinMode(DIM_PIN, INPUT_PULLUP);
    analogWrite(TFT_BRIGHTNESS_PIN, 255);

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
    if (tent.state.getDayCount() > -1) {

        if (tent.state.isDay()) { //was the light on when we restarted?
            tent.growLight("HIGH");
        }

        tent.doCheckStats(); //First time right away
        draw_temp_home.start();

        tent.countMinute(); //after restart
        minutelyTicker.start();

        //for updates from earlier version that don't have temp units
        if (tent.state.getTempUnit() != 'F' && tent.state.getTempUnit() != 'C') {
            tent.state.setTempUnit('F');
        }

    } else {
        tent.state.init();
    }
}

void minutelyTick()
{
    tent.minutelyTick();
}

void loop(void)
{
    screenManager.tick();

    bool dimmerBtnVal = digitalRead(DIM_PIN);
    if (dimmerBtnVal == LOW) {
        unsigned long now = millis();
        if ((now - dimmerBtnTime) >= 1000 || dimmerBtnTime == 0) {
            dimmerBtnTime = now;
            tent.dimGrowLight();
        }
    }

    if (tent.getCheckStats()) {
        tent.check_temperature();
        tent.check_humidity();
        // tent.check_waterlevel(); // removed for stand alone controller
        tent.adjustFan();
        tent.resetCheckStats();
    }

    if (screenManager.current) {
        screenManager.current->update();
    }
}
