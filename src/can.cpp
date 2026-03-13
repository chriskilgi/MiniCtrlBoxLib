#include "can.h"

CCANBus::CCANBus(gpio_num_t txPin, gpio_num_t rxPin, uint32_t bitrate)
    : txPin(txPin), rxPin(rxPin), bitrate(bitrate), running(false)
{
}

CCANBus::~CCANBus() {
    stop();
    twai_driver_uninstall();
}

bool CCANBus::start() {
    if (running)
        return true;

    twai_general_config_t g_config =
        TWAI_GENERAL_CONFIG_DEFAULT(txPin, rxPin, TWAI_MODE_NORMAL);

    twai_timing_config_t t_config = timingFromBitrate(bitrate);
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK)
        return false;

    if (twai_start() != ESP_OK)
        return false;

    running = true;
    return true;
}

void CCANBus::stop() {
    if (!running)
        return;

    twai_stop();
    running = false;
}

bool CCANBus::transmit(const Frame& frame, uint32_t timeoutMs) {
    twai_message_t msg = {};
    msg.identifier        = frame.canIdentifier;
    msg.data_length_code  = frame.dlc;
    msg.flags             = 0;

    for (int i = 0; i < frame.dlc; i++)
        msg.data[i] = frame.data[i];

    return twai_transmit(&msg, pdMS_TO_TICKS(timeoutMs)) == ESP_OK;
}

bool CCANBus::receive(Frame& outFrame, uint32_t timeoutMs) {
    twai_message_t msg;

    if (twai_receive(&msg, pdMS_TO_TICKS(timeoutMs)) != ESP_OK)
        return false;

    outFrame.canIdentifier = msg.identifier;
    outFrame.dlc           = msg.data_length_code;

    for (int i = 0; i < outFrame.dlc; i++)
        outFrame.data[i] = msg.data[i];

    return true;
}

twai_timing_config_t CCANBus::timingFromBitrate(uint32_t bitrate) {
    switch (bitrate) {
        case 250000:  return TWAI_TIMING_CONFIG_250KBITS();
        case 500000:  return TWAI_TIMING_CONFIG_500KBITS();
        case 1000000: return TWAI_TIMING_CONFIG_1MBITS();
        default:      return TWAI_TIMING_CONFIG_500KBITS();
    }
}
