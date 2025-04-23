#ifndef MINMAXHEAP_HPP
#define MINMAXHEAP_HPP

#include <vector>
#include "SensorReading.hpp"

class MinMaxHeap
{
private:
    std::vector<int> heap;                         // Indices into allReadings
    std::vector<int> &positions;                   // Reference to external positions vector
    const std::vector<SensorReading> &allReadings; // Reference to external readings vector

    void swap(int i, int j);
    bool isMinLevel(int index);
    void bubbleUp(int index);
    void bubbleDown(int index);

public:
    MinMaxHeap(const std::vector<SensorReading> &r, std::vector<int> &p);
    void insert(int idx);
    int findMin();
    int findMax();
    void deleteMin();
    void deleteMax();
    void deleteAtIndex(int heapIndex);
    std::vector<int> getTopKMin(int k);
    std::vector<int> getTopKMax(int k);
};

#endif