#include <Arduino.h>
#include <esp_mac.h>

#include <span>

#include "esp32_bluetooth.h"
#include "log.h"
#include "utils.h"
#include "wiipp.h"

#include <bitset>

constexpr int LED = 10;
wiipp::Bluetooth* bt;
wiipp::Wii* wii;

uint64_t last{0};
void setup() {
    Serial.begin(115200);
    pinMode(LED, OUTPUT);

    bt = new wiipp::Esp32Bluetooth();
    bt->onReady([](auto...) {
        log_d("Bluetooth initialized");
    });
    wii = new wiipp::Wii(bt, [](const wiipp::WiiEvent& event) {
        std::visit(overloaded{
            [](const wiipp::ScanStarted&) {
                digitalWrite(LED, LOW);
            },
            [](const wiipp::ScanStopped&) {
                digitalWrite(LED, HIGH);
            },
            [](const wiipp::BalanceBoardConnected& board) {
                log_i("Balance board connected %04X", board.handle);

            },
            [](const wiipp::BalanceBoardDisconnected& board) {
                log_i("Balance board disconnected %04X", board.handle);
            },
            [](const wiipp::BalanceBoardData& data) {
                float totalWeight = data.tr + data.br + data.tl + data.bl;
                float adjusted = (.999 * totalWeight * (1.0 - .0007 * (data.temperature - data.referenceTemperature)));
                auto now = millis();
                if (now -last > 2500) {
                    log_d("Weight: %.2f %.2f %d %d %d %d", totalWeight/1000, adjusted/1000, data.tr, data.br, data.tl, data.bl);
                    last = now;
                }
            },
        }, event);
    });

    bt->onReady([](const wiipp::Bluetooth* p) {
        log_d("Bluetooth device ready, starting scan");
        wii->sync();
    });
}

void loop() {
    wii->step();
}
