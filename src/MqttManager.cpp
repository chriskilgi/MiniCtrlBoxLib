#include "MqttManager.h"

namespace nspMiniCtrlBox {
MqttManager::MqttManager(MqttContext& c) : ctx(c) {}

void MqttManager::begin() {
    setupWifi();
    setupMqtt();
}

void MqttManager::setupWifi() {
    WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t){
        if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
            handleWifiConnect();
        }
        if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
            handleWifiDisconnect();
        }
    });

    WiFi.begin(ctx.wifiSsid, ctx.wifiPassword);
}

void MqttManager::setupMqtt() {
    ctx.client.onConnect([this](bool sp){
        handleMqttConnect(sp);
    });

    ctx.client.onDisconnect([this](AsyncMqttClientDisconnectReason reason){
        handleMqttDisconnect(reason);
    });

    ctx.client.setServer(ctx.brokerHost, ctx.brokerPort);
}


void MqttManager::loop() {
    // optional reconnect logic
}

void MqttManager::publish(const char* topic, const char* payload) {
    if (ctx.mqttConnected) {
        ctx.client.publish(topic, 0, false, payload);
    }
}

void MqttManager::publish(const char* topic, float value) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%.3f", value);
    publish(topic, buf);
}

void MqttManager::handleWifiConnect() {
    ctx.wifiConnected = true;
    ctx.client.connect();
}

void MqttManager::handleWifiDisconnect() {
    ctx.wifiConnected = false;
}

void MqttManager::handleMqttConnect(bool) {
    ctx.mqttConnected = true;
}

void MqttManager::handleMqttDisconnect(AsyncMqttClientDisconnectReason) {
    ctx.mqttConnected = false;
}

} // namespace nspMiniCtrlBox
