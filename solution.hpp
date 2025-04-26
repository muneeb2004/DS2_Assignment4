#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <vector>
#include <queue>
#include <fstream>
#include <utility>
#include "SensorReading.hpp"
#include "MinMaxHeap.hpp"

extern std::vector<SensorReading> allReadings;
extern std::vector<int> readingPositionsInHeap;
extern std::priority_queue<
    std::pair<long long, int>,
    std::vector<std::pair<long long, int>>,
    std::greater<std::pair<long long, int>>>
    expirationQueue;
extern double activeTemperatureSum;
extern int activeReadingCounter;
extern std::vector<double> latestSensorTemperatures;
extern std::vector<long long> latestSensorTimestamps;
extern std::vector<long long> sensorEntryTimestamps;
extern std::ofstream alertLogFile;
extern MinMaxHeap minMaxHeap;

extern const int MAX_SENSORS_PLUS_ONE;
extern const long long READING_EXPIRATION_MS;
extern const int ANOMALY_CHECK_K;
extern const double HIGH_TEMP_THRESHOLD;
extern const int MAX_SENSOR_ID;
extern const int MIN_SENSOR_ID;

void processReading(const SensorReading &reading);

#endif