#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <random>
#include <chrono>
#include <string>
#include <vector>
#include "SensorReading.hpp"
#include "solution.hpp"

using namespace std;
using namespace chrono;

// Configuration
const int numSensors = 15;
const int delayMs = 100;     // Interval between readings per sensor
const int anomalyEvery = 15; // Inject anomalies every 15 readings
const int seed = 42;         // GLOBAL SEED for reproducibility

// Shared queue and mutex
queue<SensorReading> readingQueue;
mutex queueMutex;

// Random engines and distributions
default_random_engine globalGen(seed);
uniform_real_distribution<float> normalDist(40.0, 45.0);
uniform_real_distribution<float> spikeDist(75.0, 85.0);
uniform_real_distribution<float> coldDist(10.0, 25.0);

// Get current timestamp
long long currentTimestamp()
{
    return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

// Sensor simulation
void sensorStream(int sensorID)
{
    int count = 0;
    while (true)
    {
        SensorReading reading;
        reading.sensorID = sensorID;
        reading.timestamp = currentTimestamp();
        reading.temperature = normalDist(globalGen);

        // Inject anomalies every N readings
        if (count % anomalyEvery == 0)
        {
            float r = uniform_real_distribution<float>(0.0, 1.0)(globalGen);
            if (r < 0.4)
            {
                reading.temperature = spikeDist(globalGen); // High spike
            }
            else if (r < 0.7)
            {
                reading.temperature = coldDist(globalGen); // Cold spot
            }
        }

        // Push to queue
        {
            lock_guard<mutex> lock(queueMutex);
            readingQueue.push(reading);
        }

        this_thread::sleep_for(milliseconds(delayMs));
        count++;
    }
}

// Monitor thread
void monitorReadings()
{
    while (true)
    {
        SensorReading reading;
        {
            lock_guard<mutex> lock(queueMutex);
            if (!readingQueue.empty())
            {
                reading = readingQueue.front();
                readingQueue.pop();
                processReading(reading); // Call the function from solution.cpp
            }
        }
        this_thread::sleep_for(milliseconds(50));
    }
}

int main()
{
    // Start sensor threads
    for (int i = 1; i <= numSensors; ++i)
    {
        thread(sensorStream, i).detach();
    }

    // Start monitor thread
    monitorReadings();
    return 0;
}