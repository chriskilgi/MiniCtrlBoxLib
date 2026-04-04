#include <Arduino.h>
#include <can.h>

CCanBus::CCanBus(gpio_num_t txPin, gpio_num_t rxPin, uint32_t bitrate)
    : txPin(txPin), rxPin(rxPin), bitrate(bitrate), running(false)
{
    setCANTermination(false); // Ensure CAN bus termination is disabled at startup

}

CCanBus::~CCanBus() {
    stop();
    twai_driver_uninstall();
}

// Check if the CAN controller is in bus-off state
bool busOffDetected() {
    uint32_t alerts;
    if (twai_read_alerts(&alerts, 0) == ESP_OK) {
        return (alerts & TWAI_ALERT_BUS_OFF) != 0;
    }
    return false;
}

// Restart the CAN controller by stopping and starting it again
void restart() {
    twai_stop();
    twai_driver_uninstall();
    start();
}

bool CCanBus::start() {
    if (running)
        return true;

    twai_general_config_t g_config =
        TWAI_GENERAL_CONFIG_DEFAULT(txPin, rxPin, TWAI_MODE_NORMAL);

    g_config.alerts_enabled =
        TWAI_ALERT_TX_FAILED
        | TWAI_ALERT_BUS_OFF
        | TWAI_ALERT_TX_SUCCESS;
  

    twai_timing_config_t t_config = timingFromBitrate(bitrate);
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK)
        return false;

    if (twai_start() != ESP_OK)
        return false;

    running = true;
    return true;
}

void CCanBus::stop() {
    if (!running)
        return;

    twai_stop();
    running = false;
}

bool CCanBus::transmit(const Frame& frame, uint32_t timeoutMs) {
    twai_message_t msg = {};
    msg.identifier        = frame.canIdentifier;
    msg.data_length_code  = frame.dlc;
    msg.flags             = 0;

    for (int i = 0; i < frame.dlc; i++)
        msg.data[i] = frame.data[i];

    return twai_transmit(&msg, pdMS_TO_TICKS(timeoutMs)) == ESP_OK;
}

bool CCanBus::receive(Frame& outFrame, uint32_t timeoutMs) {
    twai_message_t msg;

    if (twai_receive(&msg, pdMS_TO_TICKS(timeoutMs)) != ESP_OK)
        return false;

    outFrame.canIdentifier = msg.identifier;
    outFrame.dlc           = msg.data_length_code;

    for (int i = 0; i < outFrame.dlc; i++)
        outFrame.data[i] = msg.data[i];

    return true;
}

twai_timing_config_t CCanBus::timingFromBitrate(uint32_t bitrate) {
    switch (bitrate) {
        case 250000:  return TWAI_TIMING_CONFIG_250KBITS();
        case 500000:  return TWAI_TIMING_CONFIG_500KBITS();
        case 1000000: return TWAI_TIMING_CONFIG_1MBITS();
        default:      return TWAI_TIMING_CONFIG_500KBITS();
    }
}

void CCanBus::setCANTermination(bool boTermination) {
    digitalWrite(PIN_SSR, boTermination);
}

