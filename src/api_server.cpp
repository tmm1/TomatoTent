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

void metricsCmd(WebServer& server, WebServer::ConnectionType type, char* url_tail, bool tail_complete)
{
    server.httpSuccess("text/plain; version=0.0.4; charset=utf-8");

    server << "# HELP tent_temperature The temperature of the tent in Celsius\n";
    server << "# TYPE tent_temperature gauge\n";
    server << "tent_temperature " << tent.sensors.tentTemperatureC << "\n";

    server << "# HELP tent_humidity The relative humidity of the tent\n";
    server << "# TYPE tent_humidity gauge\n";
    server << "tent_humidity " << tent.sensors.tentHumidity << "\n";

    server << "# HELP soil_temperature The temperature of the soil in Celsius\n";
    server << "# TYPE soil_temperature gauge\n";
    server << "soil_temperature " << tent.sensors.soilTemperatureC << "\n";

    server << "# HELP soil_moisture The moisture of the soil in Celsius\n";
    server << "# TYPE soil_moisture gauge\n";
    server << "soil_moisture " << tent.sensors.soilMoisture << "\n";

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

void ApiServer::begin()
{
    WebServer::begin();

    setDefaultCommand(&defaultCmd);
    addCommand("metrics", &metricsCmd);
}
