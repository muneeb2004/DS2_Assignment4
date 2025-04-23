#ifndef SENSOR_READING_HPP
#define SENSOR_READING_HPP

#include <cstdint>
#include <iostream>

struct SensorReading
{
    int sensorID;
    int64_t timestamp;  // epoch ms
    double temperature; // °C

    bool operator<(const SensorReading &o) const { return temperature < o.temperature; }
    bool operator>(const SensorReading &o) const { return temperature > o.temperature; }

    // Optional: for debugging or logging
    void print() const
    {
        std::cout << "[Sensor " << sensorID << "] " << timestamp << " ms => Temp: " << temperature << " C" << std::endl;
    }
};

#endif // SENSOR_READING_HPP