#ifndef MINMAXHEAP_HPP
#define MINMAXHEAP_HPP

#include <vector>
#include "SensorReading.hpp"

class MinMaxHeap
{
private:
    std::vector<int> heapIndices;
    std::vector<int> &readingPositions;
    const std::vector<SensorReading> &allReadingsData;

    void swapHeapNodes(int index1, int index2);
    bool isMinLevel(int nodeIndex);
    void bubbleUp(int startIndex);
    void bubbleDown(int startIndex);
    void bubbleDownMin(int nodeIndex);
    void bubbleDownMax(int nodeIndex);
    int findMinDescendantIndex(int nodeIndex);
    int findMaxDescendantIndex(int nodeIndex);

public:
    MinMaxHeap(const std::vector<SensorReading> &readingsRef, std::vector<int> &positionsRef);
    void insertReadingIndex(int readingIndex);
    int findMinReadingIndex();
    int findMaxReadingIndex();
    void deleteMinReading();
    void deleteMaxReading();
    void deleteElementAtHeapIndex(int heapIndex);
    std::vector<int> getTopKMinIndices(int k);
    std::vector<int> getTopKMaxIndices(int k);
    bool isEmpty() const;
    size_t size() const;
};

#endif