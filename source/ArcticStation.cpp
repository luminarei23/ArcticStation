/**
 * Copyright (c) 2024 ArcticByte Development
 *
 * License-Identifier: GNU GENERAL PUBLIC LICENSE
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "PMS7003.hpp"

Plantower_PMS7003 PMS7003;

int main()
{
    stdio_init_all();

    /*uncomment when the PIN 0 and 1 will be free for connecting PMS7003*/
    // PMS7003.init();

    while (true)
    {
        printf(">:( \n");
        sleep_ms(2000);
    }
}