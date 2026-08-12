#include "BatteryMonitor.h"

BatteryMonitor Battery;

void BatteryMonitor::begin() {
    analogReadResolution(12);
    analogSetPinAttenuation(BATTERY_PIN, ADC_11db);
    readVoltage(); // Initial baseline read
}

float BatteryMonitor::readVoltage() {
    int rawADC = analogRead(BATTERY_PIN);
    float measured = (rawADC / ADC_RESOLUTION) * ADC_REF_VOLTAGE * VOLTAGE_DIVIDER_RATIO;
    
    // Exponential Moving Average (EMA) to prevent screen flicker
    filteredVoltage = (alpha * measured) + ((1.0 - alpha) * filteredVoltage);
    return filteredVoltage;
}

uint8_t BatteryMonitor::getPercentage() {
    float v = readVoltage();
    if (v >= 4.2) return 100;
    if (v <= 3.2) return 0;
    
    // Linear map between 3.2V (0%) and 4.2V (100%)
    return (uint8_t)(((v - 3.2) / 1.0) * 100.0);
}