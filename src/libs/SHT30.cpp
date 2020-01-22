#include "SHT30.h"

bool SHT30::setAddress(int a0)
{
    if (a0 == 1) {
        address = address | 1;
    }

    Wire.begin();
    Wire.beginTransmission(address);
    byte status = Wire.endTransmission();
    if (status != 0) {
        return false;
        Serial.println("Init failed");
    }
}

bool SHT30::update()
{
    Wire.beginTransmission(address);
    Wire.write(0x27);
    Wire.write(0x37);
    byte status = Wire.endTransmission();
    if (status != 0) {
        return false;
        Serial.println("Sensor did not respond to update call");
    }
    delay(10);
    Wire.requestFrom(address, 6);
    delay(50);
    buffer[0] = Wire.read();
    buffer[1] = Wire.read();
    buffer[2] = Wire.read();
    buffer[3] = Wire.read();
    buffer[4] = Wire.read();
    buffer[5] = Wire.read();
    temperature = (buffer[0] * 256 + buffer[1]) * 0.00267 - 45;
    humidity = (buffer[3] * 256 + buffer[4]) * 0.001526;
    return true;
}