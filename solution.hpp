#ifndef SOLUTION_HPP
#define SOLUTION_HPP

#include <vector>
#include <queue>
#include <fstream>
#include "SensorReading.hpp" // For SensorReading
#include "MinMaxHeap.hpp"    // For MinMaxHeap

// Declare global variables with extern
extern std::vector<SensorReading> allReadings;
extern std::vector<int> positions;
extern std::priority_queue<
    std::pair<long long, int>,
    std::vector<std::pair<long long, int>>,
    std::greater<std::pair<long long, int>>>
    expirationHeap;
extern double sum;
extern int activeReadingCount;
extern std::vector<double> latestTemps;
extern std::vector<long long> latestTimestamps;
extern std::vector<long long> entryTimes;
extern std::ofstream alertFile;
extern MinMaxHeap minMaxHeap;

// Declare the function
void processReading(const SensorReading &reading);

#endif // SOLUTION_HPP