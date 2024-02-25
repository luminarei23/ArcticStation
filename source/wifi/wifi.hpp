// WiFiManager.hpp

#ifndef WIFI_MANAGER_HPP
#define WIFI_MANAGER_HPP

// Standard libraries
#include <array>
#include <stdint.h>

/**
 * @brief Class for managing Wi-Fi functionality.
 */
class WiFiManager
{
public:
    /**
     * @brief Default constructor.
     */
    WiFiManager() noexcept = default;

    /**
     * @brief Default destructor.
     */
    ~WiFiManager() noexcept = default;

    /**
     * @brief Initialize Wi-Fi connection.
     *
     * @param ssid The SSID of the Wi-Fi network.
     * @param pass The password of the Wi-Fi network.
     * @return true if initialization is successful, false otherwise.
     */
    bool init() noexcept;

    /**
     * @brief Stop Wi-Fi connection.
     */
    void stop() noexcept;

    /**
     * @brief Set the SSID.
     *
     * @param ssid The SSID of the Wi-Fi network.
     */
    void setSSID(const char *ssid) noexcept;

    /**
     * @brief Set the password.
     *
     * @param pass The password of the Wi-Fi network.
     */
    void setPassword(const char *pass) noexcept;

    /**
     * @brief returns Ip in array.
     */
    std::array<int, 4> getIp();

private:
    const char *ssid = nullptr; /** private ssid The SSID of the Wi-Fi network.*/
    const char *pass = nullptr; /** private password of the Wi-Fi network*/
    uint8_t localNetworkAddr;   /**4th octet from IP address*/
};

#endif // WIFI_MANAGER_HPP
