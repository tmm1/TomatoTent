#include "Particle.h"
#define WEBDUINO_FAVICON_DATA ""
#include <WebServer.h>
#include "api_server.h"
#include "tent.h"

extern Tent tent;

// no-cost stream operator as described at
// http://sundial.org/arduino/?page_id=119
template <class T>
inline Print& operator<<(Print& obj, T arg)
{
    obj.print(arg);
    return obj;
}

void defaultCmd(WebServer& server, WebServer::ConnectionType type, char* url_tail, bool tail_complete)
{
    server.httpSuccess();
    server << "welcome to tomatotent";
}

void apiCmd(WebServer& server, WebServer::ConnectionType type, char* url_tail, bool tail_complete)
{
    switch (type) {
    case WebServer::GET: {
        server.httpSuccess();
        server << "api help";
        break;
    }
    }
}

void metricsCmd(WebServer& server, WebServer::ConnectionType type, char* url_tail, bool tail_complete)
{
    server.httpSuccess("text/plain; version=0.0.4; charset=utf-8");

    if (tent.rawSensors.tentTemperature != -1) {
        server << "# HELP tomatotent_air_temperature The temperature of the air in Celsius\n";
        server << "# TYPE tomatotent_air_temperature gauge\n";
        server << "tomatotent_air_temperature " << tent.rawSensors.tentTemperature << "\n";
    }

    if (tent.rawSensors.tentHumidity != -1) {
        server << "# HELP tomatotent_air_humidity The relative humidity of the air\n";
        server << "# TYPE tomatotent_air_humidity gauge\n";
        server << "tomatotent_air_humidity " << tent.rawSensors.tentHumidity << "\n";
    }

    if (tent.rawSensors.soilTemperature != -1) {
        server << "# HELP tomatotent_soil_temperature The temperature of the soil in Celsius\n";
        server << "# TYPE tomatotent_soil_temperature gauge\n";
        server << "tomatotent_soil_temperature " << tent.rawSensors.soilTemperature << "\n";
    }

    if (tent.rawSensors.soilMoisture != -1) {
        server << "# HELP tomatotent_soil_moisture The moisture level in the soil\n";
        server << "# TYPE tomatotent_soil_moisture gauge\n";
        server << "tomatotent_soil_moisture " << tent.sensors.waterLevel << "\n";

        server << "# HELP tomatotent_soil_capacitance The capacitance level of the moisture sensor in the soil\n";
        server << "# TYPE tomatotent_soil_capacitance gauge\n";
        server << "tomatotent_soil_capacitance " << tent.rawSensors.soilMoisture << "\n";
    }

    if (tent.state.getDayCount() != -1) {
        server << "# HELP tomatotent_grow_days How many days since the grow started?\n";
        server << "# TYPE tomatotent_grow_days gauge\n";
        server << "tomatotent_grow_days " << tent.state.getDayCount() << "\n";

        server << "# HELP tomatotent_light_on Is it daylight in the tent?\n";
        server << "# TYPE tomatotent_light_on gauge\n";
        server << "tomatotent_light_on " << tent.rawSensors.lightBrightness << "\n";

        server << "# HELP tomatotent_light_period How many minutes of daylight in the tent?\n";
        server << "# TYPE tomatotent_light_period gauge\n";
        server << "tomatotent_light_period " << tent.state.getDayDuration() << "\n";

        server << "# HELP tomatotent_period_progress How many minutes into the current photoperiod?\n";
        server << "# TYPE tomatotent_period_progress gauge\n";
        server << "tomatotent_period_progress " << tent.state.getMinutesInPhotoperiod() << "\n";

        server << "# HELP tomatotent_fan_auto Is the fan running in automatic mode?\n";
        server << "# TYPE tomatotent_fan_auto gauge\n";
        server << "tomatotent_fan_auto " << (tent.state.getFanAutoMode() ? 1 : 0) << "\n";

        server << "# HELP tomatotent_fan_speed Speed of the fan\n";
        server << "# TYPE tomatotent_fan_speed gauge\n";
        server << "tomatotent_fan_speed " << (tent.state.getFanSpeed() + 5) << "\n";
    }
}

void ApiServer::begin()
{
    WebServer::begin();

    setDefaultCommand(&defaultCmd);
    addCommand("api", &apiCmd);
    addCommand("metrics", &metricsCmd);
}
