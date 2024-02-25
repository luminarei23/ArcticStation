/**
 * Copyright (c) 2024 ArcticByte Development
 *
 * License-Identifier: GNU GENERAL PUBLIC LICENSE
 */

// Standard libraries
#include <iostream>
#include "unistd.h"
#include <string.h>
#include <stdlib.h>

// Custom libraries
#include "pms7003.hpp"
#include "wifi.hpp"

// Pico SDK libraries

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

Plantower_PMS7003 PMS7003;
WiFiManager wifi;

int main()
{
    stdio_init_all();

    if (cyw43_arch_init())
    {
        printf("failed to initialise\n");
        return 1;
    }

    wifi.setSSID("Irithyll");
    wifi.setPassword("Dekonspirowany12");
    if (wifi.init())
    {
        wifi.getIp();
    };

    // AirSensorStream uartStream;
    // uartStream.init();
    // PMS7003.init(&uartStream);

    // while (true)
    // {

    //     while (!PMS7003.hasNewData() || PMS7003.getRawGreaterThan_0_3() == 0)
    //     {
    //         PMS7003.updateFrame();

    //         printf(("PM_1: %d\n"), PMS7003.getPM_1_0());
    //         printf(("PM_2.5: %d\n"), PMS7003.getPM_2_5());
    //         printf(("PM_10: %d\n"), PMS7003.getPM_10_0());
    //         sleep_ms(1000);

    //         if (PMS7003.getErrorCode() > 0)
    //         {
    //             printf(("Sensor: %d\n"), PMS7003.getHWVersion());
    //             printf(("Error: %d\n"), PMS7003.getHWVersion());
    //         }
    //     }
    // }
}