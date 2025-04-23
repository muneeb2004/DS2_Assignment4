#include "solution.hpp"
#include <iostream> // For cout, endl
#include <string>   // For string, to_string
#include <chrono>   // For duration_cast, milliseconds, system_clock
#include <utility>  // For make_pair

using namespace std;
using namespace chrono;

// Define global variables (without extern)
std::vector<SensorReading> allReadings;
std::vector<int> positions;
std::priority_queue<
    std::pair<long long, int>,
    std::vector<std::pair<long long, int>>,
    std::greater<std::pair<long long, int>>>
    expirationHeap;
double sum = 0.0;
int activeReadingCount = 0;
std::vector<double> latestTemps(16, 0.0);       // Assuming size 16, adjust as needed
std::vector<long long> latestTimestamps(16, 0); // Assuming size 16
std::vector<long long> entryTimes(16, 0);       // Assuming size 16
std::ofstream alertFile("alert_logging.txt");
MinMaxHeap minMaxHeap(allReadings, positions);

// Function to process each reading
void processReading(const SensorReading &reading)
{
    long long now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

    // Remove expired readings
    while (!expirationHeap.empty() && expirationHeap.top().first <= now)
    {
        auto [expTime, idx] = expirationHeap.top();
        expirationHeap.pop();
        if (positions[idx] != -1)
        {
            minMaxHeap.deleteAtIndex(positions[idx]);
            sum -= allReadings[idx].temperature;
            activeReadingCount--;
        }
    }

    // Add new reading
    allReadings.push_back(reading);
    int idx = allReadings.size() - 1;
    positions.push_back(-1);
    minMaxHeap.insert(idx);
    expirationHeap.push(make_pair(reading.timestamp + 60000, idx));
    sum += reading.temperature;
    activeReadingCount++;

    // Update latest temps
    int sensorID = reading.sensorID;
    latestTemps[sensorID] = reading.temperature;
    latestTimestamps[sensorID] = reading.timestamp;

    // Check for anomalies
    int k = 5; // Assume k=5
    vector<int> topKMax = minMaxHeap.getTopKMax(k);
    vector<int> bottomKMin = minMaxHeap.getTopKMin(k);
    double average = (activeReadingCount > 0) ? sum / activeReadingCount : 0.0;

    // Isolated anomalies
    for (int maxIdx : topKMax)
    {
        SensorReading &r = allReadings[maxIdx];
        if (r.temperature > 48.0)
        {
            int sid = r.sensorID;
            bool neighbor1_normal = (sid - 1 >= 1) ? (latestTemps[sid - 1] <= 48.0) : true;
            bool neighbor2_normal = (sid + 1 <= 15) ? (latestTemps[sid + 1] <= 48.0) : true;
            if (neighbor1_normal && neighbor2_normal)
            {
                string log = "[ALERT] Time: " + to_string(now) + " | Sensor: " + to_string(sid) + " | Type: Isolated High Spike | Temp: " + to_string(r.temperature) + " C [Note] Neighboring sensors are normal.";
                cout << log << endl;
                alertFile << log << endl;
            }
        }
    }
}