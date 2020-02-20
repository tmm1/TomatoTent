#pragma once

#include "Particle.h"
#define WEBDUINO_NO_IMPLEMENTATION
#include <WebServer.h>

class ApiServer : public WebServer {
public:
    ApiServer()
        : WebServer("", 80) {};

    void begin();
};