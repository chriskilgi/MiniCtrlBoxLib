#pragma once
#include <driver/twai.h>
#include <driver/gpio.h>

#define PIN_SSR GPIO_NUM_2 // GPIO pin connected to the SSR (Solid State Relay)

class CCanBus {
public:
    struct Frame {
        uint32_t canIdentifier;
        uint8_t  dlc;
        uint8_t  data[8];
    };

    CCanBus(gpio_num_t txPin = GPIO_NUM_21, gpio_num_t rxPin = GPIO_NUM_20, uint32_t bitrate = 500000);
    ~CCanBus();

    bool busOffDetected(); // Check if the CAN bus is in a bus-off state
    void restart(); // Restart the CAN controller

    bool start();
    void stop();

    bool transmit(const Frame& frame, uint32_t timeoutMs = 100);
    bool receive(Frame& outFrame, uint32_t timeoutMs = 100);

    // Control CAN bus termination using the SSR
    void setCANTermination(bool boTermination);

private:
    gpio_num_t txPin;
    gpio_num_t rxPin;
    uint32_t   bitrate;
    bool       running;

    static twai_timing_config_t timingFromBitrate(uint32_t bitrate);
};




    // Example of sending a CAN message
    //CANBus::Frame tx = {0x123, 2, {0xAB, 0xCD}};
    //can.transmit(tx);


    // Empfangen
    // CANBus::Frame rx;
    // if (can.receive(rx, 50)) {
    //     Serial.print("RX ID: ");
    //     Serial.print(rx.id, HEX);
    //     Serial.print(" DLC: ");
    //     Serial.println(rx.dlc);
    // }
