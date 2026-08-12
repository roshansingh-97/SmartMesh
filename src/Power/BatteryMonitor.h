#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <Arduino.h>

#define BATTERY_PIN 34
#define VOLTAGE_DIVIDER_RATIO 2.0
#define ADC_REF_VOLTAGE 3.3
#define ADC_RESOLUTION 4095.0

class BatteryMonitor {
private:
    float filteredVoltage = 3.7;
    float alpha = 0.15; // Moving average smoothing factor

public:
    void begin();
    float readVoltage();
    uint8_t getPercentage();
};

extern BatteryMonitor Battery;

#endif