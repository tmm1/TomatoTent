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
        server << "# HELP tent_temperature The temperature of the tent in Celsius\n";
        server << "# TYPE tent_temperature gauge\n";
        server << "tent_temperature " << tent.rawSensors.tentTemperature << "\n";
    }

    if (tent.rawSensors.tentHumidity != -1) {
        server << "# HELP tent_humidity The relative humidity of the tent\n";
        server << "# TYPE tent_humidity gauge\n";
        server << "tent_humidity " << tent.rawSensors.tentHumidity << "\n";
    }

    if (tent.rawSensors.soilTemperature != -1) {
        server << "# HELP soil_temperature The temperature of the soil in Celsius\n";
        server << "# TYPE soil_temperature gauge\n";
        server << "soil_temperature " << tent.rawSensors.soilTemperature << "\n";
    }

    if (tent.rawSensors.soilMoisture != -1) {
        server << "# HELP soil_moisture The moisture of the soil in Celsius\n";
        server << "# TYPE soil_moisture gauge\n";
        server << "soil_moisture " << tent.rawSensors.soilMoisture << "\n";
    }

    if (tent.state.getDayCount() != -1) {
        server << "# HELP is_day Is it daylight in the tent?\n";
        server << "# TYPE is_day gauge\n";
        server << "is_day " << (tent.state.isDay() ? 1 : 0) << "\n";

        server << "# HELP day_count How many days into the grow?\n";
        server << "# TYPE day_count gauge\n";
        server << "day_count " << tent.state.getDayCount() << "\n";

        server << "# HELP day_duration How many minutes in a day?\n";
        server << "# TYPE day_duration gauge\n";
        server << "day_duration " << tent.state.getDayDuration() << "\n";

        server << "# HELP minutes_in_photoperiod How many minutes into the current photoperiod?\n";
        server << "# TYPE minutes_in_photoperiod gauge\n";
        server << "minutes_in_photoperiod " << tent.state.getMinutesInPhotoperiod() << "\n";
    }
}

void ApiServer::begin()
{
    WebServer::begin();

    setDefaultCommand(&defaultCmd);
    addCommand("api", &apiCmd);
    addCommand("metrics", &metricsCmd);
}
