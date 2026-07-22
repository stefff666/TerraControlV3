#pragma once

/***********************************************************************
 * TerraControl V3
 * Configuration
 *
 * Hardware:
 *  - ESP32-WROOM-DA
 *  - DHT11
 *  - Capacitive Soil Sensor
 *  - 4 Active LOW Relays
 ***********************************************************************/

#include <Arduino.h>
#include <DHT.h>

/***********************************************************************
 * Firmware
 ***********************************************************************/

#define FIRMWARE_NAME      "TerraControl V3"
#define FIRMWARE_VERSION   "1.0.0"

/***********************************************************************
 * WiFi
 * Change these to your network
 ***********************************************************************/

const char* WIFI_SSID     = "YOUR_WIFI";
const char* WIFI_PASSWORD = "YOUR_PASSWORD";

/***********************************************************************
 * Time Zone
 * Adelaide (Automatic Daylight Saving)
 ***********************************************************************/

const char* TZ_INFO = "ACST-9:30ACDT,M10.1.0/2,M4.1.0/3";

/***********************************************************************
 * DHT11
 ***********************************************************************/

#define DHT_PIN     13
#define DHT_TYPE    DHT11

/***********************************************************************
 * Soil Sensor
 ***********************************************************************/

#define SOIL_PIN    34

// Calibration values
#define SOIL_DRY    2470
#define SOIL_WET    1550

/***********************************************************************
 * Relay Outputs
 * Active LOW
 ***********************************************************************/

#define LIGHT_RELAY       5
#define HEATER_RELAY      4
#define FAN_RELAY         12
#define HUMIDIFIER_RELAY  14

#define RELAY_ON   LOW
#define RELAY_OFF  HIGH

/***********************************************************************
 * Moonlight
 ***********************************************************************/

#define MOONLIGHT_PIN 27

/***********************************************************************
 * Timing
 ***********************************************************************/

#define SENSOR_INTERVAL    2000UL
#define HISTORY_INTERVAL   900000UL      // 15 minutes
#define STATUS_INTERVAL    30000UL
#define WIFI_INTERVAL      10000UL

/***********************************************************************
 * History
 ***********************************************************************/

#define HISTORY_POINTS 96

/***********************************************************************
 * Manual Modes
 ***********************************************************************/

enum ControlMode
{
    AUTO,
    FORCE_ON,
    FORCE_OFF
};

/***********************************************************************
 * User Settings
 ***********************************************************************/

struct Settings
{
    // Temperature

    float tempOn  = 24.0;
    float tempOff = 26.0;

    // Humidity

    float humOn  = 60.0;
    float humOff = 70.0;

    // Lighting

    uint8_t lightOnHour   = 8;
    uint8_t lightOnMinute = 0;

    uint8_t lightOffHour   = 20;
    uint8_t lightOffMinute = 0;

    // Moonlight

    uint8_t moonBrightness = 15;

    // Manual Modes

    ControlMode heaterMode      = AUTO;
    ControlMode humidifierMode  = AUTO;
    ControlMode fanMode         = AUTO;
    ControlMode lightMode       = AUTO;
};

/***********************************************************************
 * Live Runtime Data
 ***********************************************************************/

struct LiveData
{
    float temperature = 0.0;
    float humidity    = 0.0;
    float soil        = 0.0;

    bool heater      = false;
    bool humidifier  = false;
    bool fan         = false;

    bool lights      = false;
    bool moonlight   = false;

    bool sensorOK    = false;
};

/***********************************************************************
 * Global Variables
 ***********************************************************************/

extern Settings settings;
extern LiveData live;

extern float tempHistory[HISTORY_POINTS];
extern float humHistory[HISTORY_POINTS];
extern float soilHistory[HISTORY_POINTS];

extern uint8_t historyIndex;
