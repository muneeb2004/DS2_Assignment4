#include "solution.hpp"
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <queue>
#include <fstream>
#include <utility>

using namespace std;
using namespace chrono;

std::vector<SensorReading> allReadings;
std::vector<int> readingPositionsInHeap;
std::priority_queue<std::pair<long long, int>, std::vector<std::pair<long long, int>>,
                    std::greater<std::pair<long long, int>>>
    expirationQueue;
double activeTemperatureSum = 0.0;
int activeReadingCounter = 0;
const int MAX_SENSORS_PLUS_ONE = 16;
std::vector<double> latestSensorTemperatures(MAX_SENSORS_PLUS_ONE, 0.0);
std::vector<long long> latestSensorTimestamps(MAX_SENSORS_PLUS_ONE, 0);
std::vector<long long> sensorEntryTimestamps(MAX_SENSORS_PLUS_ONE, 0);
std::ofstream alertLogFile("alert_logging.txt");
MinMaxHeap minMaxHeap(allReadings, readingPositionsInHeap);

const long long READING_EXPIRATION_MS = 60000;
const int ANOMALY_CHECK_K = 5;
const double HIGH_TEMP_THRESHOLD = 48.0;
const int MAX_SENSOR_ID = 15;
const int MIN_SENSOR_ID = 1;

void processReading(const SensorReading &reading)
{
    long long currentTimeMs = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    while (!expirationQueue.empty() && expirationQueue.top().first <= currentTimeMs)
    {
        int expiredReadingIndex = expirationQueue.top().second;
        expirationQueue.pop();

        if (expiredReadingIndex < readingPositionsInHeap.size() && readingPositionsInHeap[expiredReadingIndex] != -1)
        {
            int heapIndexToRemove = readingPositionsInHeap[expiredReadingIndex];
            double expiredTemp = allReadings[expiredReadingIndex].temperature;
            minMaxHeap.deleteElementAtHeapIndex(heapIndexToRemove);
            activeTemperatureSum -= expiredTemp;
            activeReadingCounter--;
        }
    }

    allReadings.push_back(reading);
    int newReadingIndex = allReadings.size() - 1;

    if (newReadingIndex >= readingPositionsInHeap.size())
    {
        readingPositionsInHeap.resize(newReadingIndex + 1, -1);
    }
    readingPositionsInHeap[newReadingIndex] = -1;

    minMaxHeap.insertReadingIndex(newReadingIndex);

    long long expirationTime = reading.timestamp + READING_EXPIRATION_MS;
    expirationQueue.push(make_pair(expirationTime, newReadingIndex));

    activeTemperatureSum += reading.temperature;
    activeReadingCounter++;

    int sensorID = reading.sensorID;
    if (sensorID >= MIN_SENSOR_ID && sensorID <= MAX_SENSOR_ID)
    {
        latestSensorTemperatures[sensorID] = reading.temperature;
        latestSensorTimestamps[sensorID] = reading.timestamp;
    }
    else if (sensorID >= 0 && sensorID < MAX_SENSORS_PLUS_ONE)
    {
        latestSensorTemperatures[sensorID] = reading.temperature;
        latestSensorTimestamps[sensorID] = reading.timestamp;
    }
    else
    {
        cerr << "Warning: Sensor ID " << sensorID << " is out of expected range [0-" << MAX_SENSORS_PLUS_ONE - 1 << "]." << endl;
    }

    if (minMaxHeap.size() < ANOMALY_CHECK_K)
    {
        return;
    }

    vector<int> hottestKIndices = minMaxHeap.getTopKMaxIndices(ANOMALY_CHECK_K);

    for (const int &hotReadingIndex : hottestKIndices)
    {
        const SensorReading &hotReading = allReadings[hotReadingIndex];

        if (hotReading.temperature > HIGH_TEMP_THRESHOLD)
        {
            int hotSensorID = hotReading.sensorID;

            bool leftNeighborNormal = true;
            if (hotSensorID > MIN_SENSOR_ID)
            {
                if (hotSensorID - 1 < latestSensorTemperatures.size())
                {
                    leftNeighborNormal = (latestSensorTemperatures[hotSensorID - 1] <= HIGH_TEMP_THRESHOLD);
                }
            }

            bool rightNeighborNormal = true;
            if (hotSensorID < MAX_SENSOR_ID)
            {
                if (hotSensorID + 1 < latestSensorTemperatures.size())
                {
                    rightNeighborNormal = (latestSensorTemperatures[hotSensorID + 1] <= HIGH_TEMP_THRESHOLD);
                }
            }

            if (leftNeighborNormal && rightNeighborNormal)
            {
                string logMessage = "[ALERT] Time: " + to_string(currentTimeMs) +
                                    " | Sensor: " + to_string(hotSensorID) +
                                    " | Type: Isolated High Spike" +
                                    " | Temp: " + to_string(hotReading.temperature) + " C" +
                                    " [Note] Neighboring sensors are normal.";

                cout << logMessage << endl;
                if (alertLogFile.is_open())
                {
                    alertLogFile << logMessage << endl;
                }
            }
        }
    }
}