/**
 * @file arctic_station_init.hpp
 * @brief Header file for initializing Arctic Station sensors and peripherals.
 */

#ifndef ARCTIC_STATION_INIT_HPP
#define ARCTIC_STATION_INIT_HPP

#include "wifi.hpp"
#include "pms7003.hpp"

/**
 * @brief Initialize the SDK.
 *
 * @return true if SDK initialization is successful, false otherwise.
 */
bool initializeSdk();

/**
 * @brief Initialize the Wi-Fi module.
 *
 * @param wifi Reference to a WiFiManager object.
 * @return true if Wi-Fi initialization is successful, false otherwise.
 */
bool initializeWiFi(WiFiManager &wifi);

/**
 * @brief Initialize all modules.
 *
 * @param pms7003 Reference to a Plantower_PMS7003 object.
 * @param uartStream Reference to an AirSensorStream object.
 * @return true if all modules initialization is successful, false otherwise.
 */
bool initializeAllModules(WiFiManager &wifi, PMS7003 &pms7003, AirSensorStream &uartStream);

#endif // ARCTICSTATIONINIT_HPP