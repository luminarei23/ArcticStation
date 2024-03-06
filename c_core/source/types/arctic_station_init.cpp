#include "arctic_station_init.hpp"

// Pico SDK libraries
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

bool initializeSdk()
{
    stdio_init_all();

    if (cyw43_arch_init())
    {
        printf("failed to initialise cyw43\n");
        return false;
    }
    else
    {
        printf("SDK Initialized.\n");
        return true;
    }
}

bool initializeWiFi(WiFiManager &wifi)
{
    printf("Initializing WiFi...\n");

    wifi.setSSID("SSID");
    wifi.setPassword("PASSWORD");

    if (wifi.init())
    {
        wifi.getIp();
        printf("WiFi Initialized.\n");
        return true;
    }
    else
    {
        printf("WiFi Initialization failed.\n");
        return false;
    }
}

bool initializeAqiSensors(PMS7003 &pms7003, AirSensorStream &uartStream)
{
    printf("Initializing Plantower PMS7003...\n");
    uartStream.init();
    pms7003.init(&uartStream);
    printf("Plantower PMS7003 Initialized.\n");
    return true; // Assuming initialization is always successful for demonstration purposes
}

// Initialize all sensors and return true if all are initialized successfully
bool initializeAllModules(WiFiManager &wifi, PMS7003 &pms7003, AirSensorStream &uartStream)
{
    bool success = true;
    success &= initializeSdk();
    success &= initializeWiFi(wifi);
    success &= initializeAqiSensors(pms7003, uartStream);
    return success;
}
