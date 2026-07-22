
/***********************************************************************
 * TerraControl V3
 * Climate Manager
 ***********************************************************************/

#include "Climate.h"

ClimateManager Climate;

/***********************************************************************
 * Constructor
 ***********************************************************************/

ClimateManager::ClimateManager() :
    dht(DHT_PIN, DHT_TYPE)
{
}

/***********************************************************************
 * Begin
 ***********************************************************************/

void ClimateManager::begin()
{
    dht.begin();

    pinMode(HEATER_RELAY, OUTPUT);
    pinMode(HUMIDIFIER_RELAY, OUTPUT);
    pinMode(FAN_RELAY, OUTPUT);

    digitalWrite(HEATER_RELAY, RELAY_OFF);
    digitalWrite(HUMIDIFIER_RELAY, RELAY_OFF);
    digitalWrite(FAN_RELAY, RELAY_OFF);

    live.heater = false;
    live.humidifier = false;
    live.fan = false;
    live.sensorOK = false;
}

/***********************************************************************
 * Update
 ***********************************************************************/

void ClimateManager::update()
{
    if (millis() - lastRead < SENSOR_INTERVAL)
        return;

    lastRead = millis();

    readSensors();

    if (!live.sensorOK)
        return;

    controlTemperature();
    controlHumidity();
}

/***********************************************************************
 * Read Sensors
 ***********************************************************************/

void ClimateManager::readSensors()
{
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h))
    {
        live.sensorOK = false;
        return;
    }

    live.sensorOK = true;

    live.temperature = t;
    live.humidity = h;
    live.soil = readSoil();
}

/***********************************************************************
 * Soil Moisture
 ***********************************************************************/

float ClimateManager::readSoil()
{
    int raw = analogRead(SOIL_PIN);

    float moisture =
        ((float)(SOIL_DRY - raw) /
        (SOIL_DRY - SOIL_WET)) * 100.0f;

    moisture = constrain(moisture, 0.0f, 100.0f);

    return moisture;
}

/***********************************************************************
 * Temperature Control
 ***********************************************************************/

void ClimateManager::controlTemperature()
{
    switch (settings.heaterMode)
    {
        case FORCE_ON:
            setHeater(true);
            return;

        case FORCE_OFF:
            setHeater(false);
            return;

        case AUTO:
        default:
            break;
    }

    if (live.temperature <= settings.tempOn)
        setHeater(true);

    if (live.temperature >= settings.tempOff)
        setHeater(false);
}

/***********************************************************************
 * Humidity Control
 ***********************************************************************/

void ClimateManager::controlHumidity()
{
    switch (settings.humidifierMode)
    {
        case FORCE_ON:
            setHumidifier(true);
            return;

        case FORCE_OFF:
            setHumidifier(false);
            return;

        case AUTO:
        default:
            break;
    }

    if (live.humidity <= settings.humOn)
        setHumidifier(true);

    if (live.humidity >= settings.humOff)
        setHumidifier(false);
}

/***********************************************************************
 * Heater
 ***********************************************************************/

void ClimateManager::setHeater(bool state)
{
    if (live.heater == state)
        return;

    live.heater = state;

    digitalWrite(
        HEATER_RELAY,
        state ? RELAY_ON : RELAY_OFF
    );
}

/***********************************************************************
 * Humidifier
 ***********************************************************************/

void ClimateManager::setHumidifier(bool state)
{
    if (live.humidifier == state)
        return;

    live.humidifier = state;

    digitalWrite(
        HUMIDIFIER_RELAY,
        state ? RELAY_ON : RELAY_OFF
    );

    // Fan follows humidifier
    setFan(state);
}

/***********************************************************************
 * Fan
 ***********************************************************************/

void ClimateManager::setFan(bool state)
{
    switch (settings.fanMode)
    {
        case FORCE_ON:
            state = true;
            break;

        case FORCE_OFF:
            state = false;
            break;

        case AUTO:
        default:
            break;
    }

    if (live.fan == state)
        return;

    live.fan = state;

    digitalWrite(
        FAN_RELAY,
        state ? RELAY_ON : RELAY_OFF
    );
}

/***********************************************************************
 * Sensor Status
 ***********************************************************************/

bool ClimateManager::sensorOK() const
{
    return live.sensorOK;
}
