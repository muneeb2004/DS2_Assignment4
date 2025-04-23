#include "MinMaxHeap.hpp"
#include <algorithm>

MinMaxHeap::MinMaxHeap(const std::vector<SensorReading> &r, std::vector<int> &p)
    : allReadings(r), positions(p) {}

void MinMaxHeap::swap(int i, int j)
{
    int a = heap[i];
    int b = heap[j];
    heap[i] = b;
    heap[j] = a;
    positions[a] = j;
    positions[b] = i;
}

bool MinMaxHeap::isMinLevel(int index)
{
    int level = 0;
    while (index > 0)
    {
        index = (index - 1) / 2;
        level++;
    }
    return (level % 2 == 0);
}

void MinMaxHeap::bubbleUp(int index)
{
    if (index == 0)
        return;
    int parent = (index - 1) / 2;
    if (isMinLevel(index))
    {
        if (allReadings[heap[index]].temperature < allReadings[heap[parent]].temperature)
        {
            swap(index, parent);
            bubbleUp(parent);
        }
        else if (parent > 0)
        {
            int grandparent = (parent - 1) / 2;
            if (allReadings[heap[index]].temperature > allReadings[heap[grandparent]].temperature)
            {
                swap(index, grandparent);
                bubbleUp(grandparent);
            }
        }
    }
    else
    {
        if (allReadings[heap[index]].temperature > allReadings[heap[parent]].temperature)
        {
            swap(index, parent);
            bubbleUp(parent);
        }
        else if (parent > 0)
        {
            int grandparent = (parent - 1) / 2;
            if (allReadings[heap[index]].temperature < allReadings[heap[grandparent]].temperature)
            {
                swap(index, grandparent);
                bubbleUp(grandparent);
            }
        }
    }
}

void MinMaxHeap::bubbleDown(int index)
{
    if (isMinLevel(index))
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        if (left >= heap.size())
            return;
        int smallest = left;
        if (right < heap.size() && allReadings[heap[right]].temperature < allReadings[heap[smallest]].temperature)
        {
            smallest = right;
        }
        for (int gc = 4 * index + 3; gc <= 4 * index + 6 && gc < heap.size(); gc++)
        {
            if (allReadings[heap[gc]].temperature < allReadings[heap[smallest]].temperature)
            {
                smallest = gc;
            }
        }
        if (allReadings[heap[smallest]].temperature < allReadings[heap[index]].temperature)
        {
            swap(index, smallest);
            int parent_of_smallest = (smallest - 1) / 2;
            if (parent_of_smallest != index && allReadings[heap[smallest]].temperature < allReadings[heap[parent_of_smallest]].temperature)
            {
                swap(smallest, parent_of_smallest);
            }
            bubbleDown(smallest);
        }
    }
    else
    {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        if (left >= heap.size())
            return;
        int largest = left;
        if (right < heap.size() && allReadings[heap[right]].temperature > allReadings[heap[largest]].temperature)
        {
            largest = right;
        }
        for (int gc = 4 * index + 3; gc <= 4 * index + 6 && gc < heap.size(); gc++)
        {
            if (allReadings[heap[gc]].temperature > allReadings[heap[largest]].temperature)
            {
                largest = gc;
            }
        }
        if (allReadings[heap[largest]].temperature > allReadings[heap[index]].temperature)
        {
            swap(index, largest);
            int parent_of_largest = (largest - 1) / 2;
            if (parent_of_largest != index && allReadings[heap[largest]].temperature > allReadings[heap[parent_of_largest]].temperature)
            {
                swap(largest, parent_of_largest);
            }
            bubbleDown(largest);
        }
    }
}

void MinMaxHeap::insert(int idx)
{
    heap.push_back(idx);
    positions[idx] = heap.size() - 1;
    bubbleUp(heap.size() - 1);
}

int MinMaxHeap::findMin()
{
    if (heap.empty())
        return -1;
    return heap[0];
}

int MinMaxHeap::findMax()
{
    if (heap.empty())
        return -1;
    if (heap.size() == 1)
        return heap[0];
    if (heap.size() == 2)
        return heap[1];
    return (allReadings[heap[1]].temperature > allReadings[heap[2]].temperature) ? heap[1] : heap[2];
}

void MinMaxHeap::deleteMin()
{
    deleteAtIndex(0);
}

void MinMaxHeap::deleteMax()
{
    if (heap.size() <= 1)
    {
        deleteAtIndex(0);
        return;
    }
    int maxIndex = (heap.size() > 2 && allReadings[heap[2]].temperature > allReadings[heap[1]].temperature) ? 2 : 1;
    deleteAtIndex(maxIndex);
}

void MinMaxHeap::deleteAtIndex(int heapIndex)
{
    if (heapIndex >= heap.size())
        return;
    int last = heap.size() - 1;
    swap(heapIndex, last);
    int removed_idx = heap[last];
    heap.pop_back();
    positions[removed_idx] = -1;
    if (heapIndex < heap.size())
    {
        bubbleUp(heapIndex);
        bubbleDown(heapIndex);
    }
}

std::vector<int> MinMaxHeap::getTopKMin(int k)
{
    std::vector<int> result;
    std::vector<int> temp;
    for (int i = 0; i < k && !heap.empty(); i++)
    {
        int minIdx = findMin();
        result.push_back(minIdx);
        temp.push_back(minIdx);
        deleteMin();
    }
    for (int idx : temp)
    {
        insert(idx);
    }
    return result;
}

std::vector<int> MinMaxHeap::getTopKMax(int k)
{
    std::vector<int> result;
    std::vector<int> temp;
    for (int i = 0; i < k && !heap.empty(); i++)
    {
        int maxIdx = findMax();
        result.push_back(maxIdx);
        temp.push_back(maxIdx);
        deleteMax();
    }
    for (int idx : temp)
    {
        insert(idx);
    }
    return result;
}