#include "wifi.hpp"

// Pico SDK libraries
#include "pico/cyw43_arch.h"

bool WiFiManager::init()
{
    // Initialize Wi-Fi with power safe mode
    cyw43_arch_enable_sta_mode();
    cyw43_wifi_pm(&cyw43_state, cyw43_pm_value(CYW43_PM1_POWERSAVE_MODE, 20, 1, 1, 1));

    printf("Connecting to WiFi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(this->ssid, this->pass, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("Failed to connect.\n");
        return false;
    }
    else
    {
        printf("Connected.\n");
        return true;
    }
}

void WiFiManager::stop()
{
    // Stop Wi-Fi TODO
}

void WiFiManager::setSSID(const char *ssid)
{
    this->ssid = ssid;
}

void WiFiManager::setPassword(const char *pass)
{
    this->pass = pass;
}

std::array<int, 4> WiFiManager::getIp()
{

    std::array<int, 4> ipAddress;
    auto ip_addr = cyw43_state.netif[CYW43_ITF_STA].ip_addr.addr;
    printf("IP Address: %lu.%lu.%lu.%lu\n", ip_addr & 0xFF, (ip_addr >> 8) & 0xFF, (ip_addr >> 16) & 0xFF, ip_addr >> 24);
    // Extracting individual octets
    ipAddress[0] = (ip_addr & 0xFF);
    ipAddress[1] = ((ip_addr >> 8) & 0xFF);
    ipAddress[2] = ((ip_addr >> 16) & 0xFF);
    ipAddress[3] = (ip_addr >> 24);

    this->localNetworkAddr = ipAddress[3];

    return ipAddress;
}