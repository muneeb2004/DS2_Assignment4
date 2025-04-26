#include "MinMaxHeap.hpp"
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <limits>

MinMaxHeap::MinMaxHeap(const std::vector<SensorReading> &readingsRef, std::vector<int> &positionsRef)
    : allReadingsData(readingsRef), readingPositions(positionsRef) {}

void MinMaxHeap::swapHeapNodes(int index1, int index2)
{
    int readingIndex1 = heapIndices[index1];
    int readingIndex2 = heapIndices[index2];
    std::swap(heapIndices[index1], heapIndices[index2]);
    readingPositions[readingIndex1] = index2;
    readingPositions[readingIndex2] = index1;
}

bool MinMaxHeap::isMinLevel(int nodeIndex)
{
    if (nodeIndex < 0)
        return false;
    if (nodeIndex == 0)
        return true;
    int level = static_cast<int>(std::floor(std::log2(nodeIndex + 1)));
    return (level % 2 == 0);
}

void MinMaxHeap::bubbleUp(int startIndex)
{
    int currentIndex = startIndex;
    if (currentIndex == 0)
    {
        return;
    }

    int parentIndex = (currentIndex - 1) / 2;

    if (isMinLevel(currentIndex))
    {
        if (allReadingsData[heapIndices[currentIndex]].temperature > allReadingsData[heapIndices[parentIndex]].temperature)
        {
            swapHeapNodes(currentIndex, parentIndex);
            bubbleUp(parentIndex);
        }
        else
        {
            int grandparentIndex = (parentIndex - 1) / 2;
            if (parentIndex > 0 && allReadingsData[heapIndices[currentIndex]].temperature < allReadingsData[heapIndices[grandparentIndex]].temperature)
            {
                swapHeapNodes(currentIndex, grandparentIndex);
                bubbleUp(grandparentIndex);
            }
        }
    }
    else
    {
        if (allReadingsData[heapIndices[currentIndex]].temperature < allReadingsData[heapIndices[parentIndex]].temperature)
        {
            swapHeapNodes(currentIndex, parentIndex);
            bubbleUp(parentIndex);
        }
        else
        {
            int grandparentIndex = (parentIndex - 1) / 2;
            if (parentIndex > 0 && allReadingsData[heapIndices[currentIndex]].temperature > allReadingsData[heapIndices[grandparentIndex]].temperature)
            {
                swapHeapNodes(currentIndex, grandparentIndex);
                bubbleUp(grandparentIndex);
            }
        }
    }
}

int MinMaxHeap::findMinDescendantIndex(int nodeIndex)
{
    int minDescendantIndex = -1;
    double minDescendantTemp = std::numeric_limits<double>::max();
    int currentHeapSize = heapIndices.size();

    int leftChildIndex = 2 * nodeIndex + 1;
    int rightChildIndex = 2 * nodeIndex + 2;

    if (leftChildIndex < currentHeapSize)
    {
        minDescendantTemp = allReadingsData[heapIndices[leftChildIndex]].temperature;
        minDescendantIndex = leftChildIndex;
    }
    if (rightChildIndex < currentHeapSize && allReadingsData[heapIndices[rightChildIndex]].temperature < minDescendantTemp)
    {
        minDescendantTemp = allReadingsData[heapIndices[rightChildIndex]].temperature;
        minDescendantIndex = rightChildIndex;
    }

    for (int i = 0; i < 4; ++i)
    {
        int grandChildIndex = 4 * nodeIndex + 3 + i;
        if (grandChildIndex < currentHeapSize && allReadingsData[heapIndices[grandChildIndex]].temperature < minDescendantTemp)
        {
            minDescendantTemp = allReadingsData[heapIndices[grandChildIndex]].temperature;
            minDescendantIndex = grandChildIndex;
        }
    }

    return minDescendantIndex;
}

int MinMaxHeap::findMaxDescendantIndex(int nodeIndex)
{
    int maxDescendantIndex = -1;
    double maxDescendantTemp = -std::numeric_limits<double>::max();
    int currentHeapSize = heapIndices.size();

    int leftChildIndex = 2 * nodeIndex + 1;
    int rightChildIndex = 2 * nodeIndex + 2;

    if (leftChildIndex < currentHeapSize)
    {
        maxDescendantTemp = allReadingsData[heapIndices[leftChildIndex]].temperature;
        maxDescendantIndex = leftChildIndex;
    }
    if (rightChildIndex < currentHeapSize && allReadingsData[heapIndices[rightChildIndex]].temperature > maxDescendantTemp)
    {
        maxDescendantTemp = allReadingsData[heapIndices[rightChildIndex]].temperature;
        maxDescendantIndex = rightChildIndex;
    }

    for (int i = 0; i < 4; ++i)
    {
        int grandChildIndex = 4 * nodeIndex + 3 + i;
        if (grandChildIndex < currentHeapSize && allReadingsData[heapIndices[grandChildIndex]].temperature > maxDescendantTemp)
        {
            maxDescendantTemp = allReadingsData[heapIndices[grandChildIndex]].temperature;
            maxDescendantIndex = grandChildIndex;
        }
    }
    return maxDescendantIndex;
}

void MinMaxHeap::bubbleDown(int startIndex)
{
    if (isMinLevel(startIndex))
    {
        bubbleDownMin(startIndex);
    }
    else
    {
        bubbleDownMax(startIndex);
    }
}

void MinMaxHeap::bubbleDownMin(int nodeIndex)
{
    int currentHeapSize = heapIndices.size();
    if (2 * nodeIndex + 1 >= currentHeapSize)
    {
        return;
    }

    int minDescendantIndex = findMinDescendantIndex(nodeIndex);

    if (minDescendantIndex != -1)
    {
        int parentOfMinDescendant = (minDescendantIndex - 1) / 2;
        bool isGrandchild = parentOfMinDescendant != nodeIndex;

        if (isGrandchild)
        {
            if (allReadingsData[heapIndices[minDescendantIndex]].temperature < allReadingsData[heapIndices[nodeIndex]].temperature)
            {
                swapHeapNodes(nodeIndex, minDescendantIndex);
                if (allReadingsData[heapIndices[minDescendantIndex]].temperature > allReadingsData[heapIndices[parentOfMinDescendant]].temperature)
                {
                    swapHeapNodes(minDescendantIndex, parentOfMinDescendant);
                }
                bubbleDownMin(minDescendantIndex);
            }
        }
        else
        {
            if (allReadingsData[heapIndices[minDescendantIndex]].temperature < allReadingsData[heapIndices[nodeIndex]].temperature)
            {
                swapHeapNodes(nodeIndex, minDescendantIndex);
                bubbleDownMin(minDescendantIndex);
            }
        }
    }
}

void MinMaxHeap::bubbleDownMax(int nodeIndex)
{
    int currentHeapSize = heapIndices.size();
    if (2 * nodeIndex + 1 >= currentHeapSize)
    {
        return;
    }

    int maxDescendantIndex = findMaxDescendantIndex(nodeIndex);

    if (maxDescendantIndex != -1)
    {
        int parentOfMaxDescendant = (maxDescendantIndex - 1) / 2;
        bool isGrandchild = parentOfMaxDescendant != nodeIndex;

        if (isGrandchild)
        {
            if (allReadingsData[heapIndices[maxDescendantIndex]].temperature > allReadingsData[heapIndices[nodeIndex]].temperature)
            {
                swapHeapNodes(nodeIndex, maxDescendantIndex);
                if (allReadingsData[heapIndices[maxDescendantIndex]].temperature < allReadingsData[heapIndices[parentOfMaxDescendant]].temperature)
                {
                    swapHeapNodes(maxDescendantIndex, parentOfMaxDescendant);
                }
                bubbleDownMax(maxDescendantIndex);
            }
        }
        else
        {
            if (allReadingsData[heapIndices[maxDescendantIndex]].temperature > allReadingsData[heapIndices[nodeIndex]].temperature)
            {
                swapHeapNodes(nodeIndex, maxDescendantIndex);
                bubbleDownMax(maxDescendantIndex);
            }
        }
    }
}

void MinMaxHeap::insertReadingIndex(int readingIndex)
{
    if (readingIndex >= readingPositions.size())
    {
        throw std::out_of_range("readingIndex out of bounds for readingPositions vector during insert");
    }
    if (readingPositions[readingIndex] != -1)
    {
        std::cerr << "Warning: Attempting to insert reading index " << readingIndex << " which is already in the heap." << std::endl;
        return;
    }

    heapIndices.push_back(readingIndex);
    int newHeapIndex = heapIndices.size() - 1;
    readingPositions[readingIndex] = newHeapIndex;
    bubbleUp(newHeapIndex);
}

int MinMaxHeap::findMinReadingIndex()
{
    if (heapIndices.empty())
    {
        return -1;
    }
    return heapIndices[0];
}

int MinMaxHeap::findMaxReadingIndex()
{
    if (heapIndices.empty())
    {
        return -1;
    }
    if (heapIndices.size() == 1)
    {
        return heapIndices[0];
    }
    int index1 = 1;
    int index2 = 2;
    int maxIndex = index1;

    if (index2 < heapIndices.size() &&
        allReadingsData[heapIndices[index2]].temperature > allReadingsData[heapIndices[index1]].temperature)
    {
        maxIndex = index2;
    }
    return heapIndices[maxIndex];
}

void MinMaxHeap::deleteMinReading()
{
    if (!heapIndices.empty())
    {
        deleteElementAtHeapIndex(0);
    }
}

void MinMaxHeap::deleteMaxReading()
{
    if (heapIndices.empty())
    {
        return;
    }
    if (heapIndices.size() == 1)
    {
        deleteElementAtHeapIndex(0);
        return;
    }

    int index1 = 1;
    int index2 = 2;
    int maxHeapIndex = index1;

    if (index2 < heapIndices.size() &&
        allReadingsData[heapIndices[index2]].temperature > allReadingsData[heapIndices[index1]].temperature)
    {
        maxHeapIndex = index2;
    }

    deleteElementAtHeapIndex(maxHeapIndex);
}

void MinMaxHeap::deleteElementAtHeapIndex(int heapIndex)
{
    if (heapIndex < 0 || heapIndex >= heapIndices.size())
    {
        return;
    }

    int lastIndex = heapIndices.size() - 1;
    int readingIndexToRemove = heapIndices[heapIndex];

    swapHeapNodes(heapIndex, lastIndex);
    heapIndices.pop_back();
    readingPositions[readingIndexToRemove] = -1;

    if (heapIndex >= heapIndices.size())
    {
        return;
    }

    int movedElementReadingIndex = heapIndices[heapIndex];
    bubbleDown(heapIndex);
    if (readingPositions[movedElementReadingIndex] == heapIndex)
    {
        bubbleUp(heapIndex);
    }
}

void restoreHeap(MinMaxHeap &heap, const std::vector<int> &removedIndices)
{
    for (int readingIndex : removedIndices)
    {
        heap.insertReadingIndex(readingIndex);
    }
}

std::vector<int> MinMaxHeap::getTopKMinIndices(int k)
{
    std::vector<int> resultIndices;
    std::vector<int> temporarilyRemovedIndices;
    int count = 0;

    while (count < k && !heapIndices.empty())
    {
        int minReadingIndex = findMinReadingIndex();
        resultIndices.push_back(minReadingIndex);
        temporarilyRemovedIndices.push_back(minReadingIndex);
        deleteMinReading();
        count++;
    }

    restoreHeap(*this, temporarilyRemovedIndices);
    return resultIndices;
}

std::vector<int> MinMaxHeap::getTopKMaxIndices(int k)
{
    std::vector<int> resultIndices;
    std::vector<int> temporarilyRemovedIndices;
    int count = 0;

    while (count < k && !heapIndices.empty())
    {
        int maxReadingIndex = findMaxReadingIndex();
        resultIndices.push_back(maxReadingIndex);
        temporarilyRemovedIndices.push_back(maxReadingIndex);
        deleteMaxReading();
        count++;
    }

    restoreHeap(*this, temporarilyRemovedIndices);
    return resultIndices;
}

bool MinMaxHeap::isEmpty() const
{
    return heapIndices.empty();
}

size_t MinMaxHeap::size() const
{
    return heapIndices.size();
}