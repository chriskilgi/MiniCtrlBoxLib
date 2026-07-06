#pragma once
#include "MqttContext.h"

namespace nspMiniCtrlBox {
class MqttManager {
public:
    MqttManager(MqttContext& ctx);

    void begin();
    void loop();

    void publish(const char* topic, const char* payload);
    void publish(const char* topic, float value);
    void publish(const char* topic, int value);

    // NEW
    void subscribe(const char* topic);
    void setMessageHandler(std::function<void(const String&, const String&)> handler);

private:
    MqttContext& ctx;

    void setupWifi();
    void setupMqtt();

    void handleWifiConnect();
    void handleWifiDisconnect();
    void handleMqttConnect(bool sessionPresent);
    void handleMqttDisconnect(AsyncMqttClientDisconnectReason reason);
};
} // namespace nspMiniCtrlBox
