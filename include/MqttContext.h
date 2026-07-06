#pragma once
#include <AsyncMqttClient.h>
#include <WiFi.h>
#include <functional>

namespace nspMiniCtrlBox {
struct MqttContext {
    AsyncMqttClient client;

    const char* wifiSsid = nullptr;
    const char* wifiPassword = nullptr;

    const char* brokerHost = nullptr;
    uint16_t brokerPort = 1883;

    bool wifiConnected = false;
    bool mqttConnected = false;

    // NEW: Callback für eingehende MQTT-Kommandos
    std::function<void(const String&, const String&)> onMessage;
};
} // namespace nspMiniCtrlBox
