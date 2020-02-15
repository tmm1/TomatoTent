#include "Particle.h"
#define WEBDUINO_FAVICON_DATA ""
#include <WebServer.h>
#include "api_server.h"

// no-cost stream operator as described at
// http://sundial.org/arduino/?page_id=119
template<class T>
inline Print &operator <<(Print &obj, T arg)
{ obj.print(arg); return obj; }

void defaultCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
    server.httpSuccess();
    server << "welcome to tomatotent";
}

void ApiServer::begin()
{
    WebServer::begin();

    setDefaultCommand(&defaultCmd);
}
