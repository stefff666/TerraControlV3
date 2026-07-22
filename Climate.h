#pragma once

/***********************************************************************
 * TerraControl V3
 * Climate Manager
 ***********************************************************************/

#include <Arduino.h>
#include <DHT.h>

#include "Config.h"

class ClimateManager
{
public:

    ClimateManager();

    // Initialise hardware
    void begin();

    // Called every loop()
    void update();

    // Returns true if the DHT is reading correctly
    bool sensorOK() const;

private:

    DHT dht;

    unsigned long lastRead = 0;

    void readSensors();

    float readSoil();

    void controlTemperature();

    void controlHumidity();

    void setHeater(bool state);

    void setHumidifier(bool state);

    void setFan(bool state);
};

// Global object

extern ClimateManager Climate;
