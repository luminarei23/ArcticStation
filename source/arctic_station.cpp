/**
 * Copyright (c) 2024 ArcticByte Development
 *
 * License-Identifier: GNU GENERAL PUBLIC LICENSE
 */

// Custom libraries
#include "arctic_station_init.hpp"
#include "aqi_sensor_serial.hpp"
#include "pms7003.hpp"
#include "wifi.hpp"

// Pico SDK libraries
#include "pico/stdlib.h"

AirSensorStream airSensorUart;
PMS7003 pms7003;
WiFiManager wifi;

int main()
{
    // Initialize all sensors
    if (!initializeAllModules(wifi, pms7003, airSensorUart))
    {
        printf("Failed to initialize modules\n");
        return 1;
    }

    // while (true)
    // {

    //     while (!airSensor.hasNewData() || airSensor.getRawGreaterThan_0_3() == 0)
    //     {
    //         airSensor.updateFrame();

    //         printf(("PM_1: %d\n"), airSensor.getPM_1_0());
    //         printf(("PM_2.5: %d\n"), airSensor.getPM_2_5());
    //         printf(("PM_10: %d\n"), airSensor.getPM_10_0());
    //         sleep_ms(1000);

    //         if (airSensor.getErrorCode() > 0)
    //         {
    //             printf(("Sensor: %d\n"), airSensor.getHWVersion());
    //             printf(("Error: %d\n"), airSensor.getHWVersion());
    //         }
    //     }
    // }
}