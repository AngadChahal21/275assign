#include "intSet.h"
#include <iostream>

void intSet::swap(intSet& otherSet) {
    // Swap internal storage/metadata
    int* oldData = data;
    data = otherSet.data;
    otherSet.data = oldData;

    int oldSize = size;
    size = otherSet.size;
    otherSet.size = oldSize;

    int oldCapacity = capacity;
    capacity = otherSet.capacity;
    otherSet.capacity = oldCapacity;
}

int intSet::indexOf(int val) const {
    // Linear search; returns -1 when not found.
    for (int idx = 0; idx < size; idx++) {
        if (data[idx] == val) {
            return idx;
        }
    }
    return -1;
}

intSet::intSet() {
    capacity = 4;
    size = 0;
    data = new int[capacity];
}

intSet::intSet(const intSet& otherSet) : data{new int[otherSet.capacity]}, size{otherSet.size}, capacity{otherSet.capacity} {
    for (int elementIdx = 0; elementIdx < size; elementIdx++) {
        data[elementIdx] = otherSet.data[elementIdx];
    }
}

intSet::intSet(intSet&& otherSet) : data{otherSet.data}, size{otherSet.size}, capacity{otherSet.capacity} {
    otherSet.data = nullptr;
    otherSet.size = 0;
    otherSet.capacity = 0;
}

intSet& intSet::operator=(const intSet& otherSet) {
    intSet copySet(otherSet);
    swap(copySet);
    return *this;
}

intSet& intSet::operator=(intSet&& otherSet) {
    swap(otherSet);
    return *this;
}

intSet::~intSet() {
    delete[] data;
}

bool intSet::contains(int val) const {
    return indexOf(val) != -1;
}

void intSet::add(int val) {
    if (contains(val)) {
        return;
    }

    if (size == capacity) {
        // Grow backing array when full.
        capacity *= 2;
        int* grownData = new int[capacity];
        for (int elementIdx = 0; elementIdx < size; elementIdx++) {
            grownData[elementIdx] = data[elementIdx];
        }
        delete[] data;
        data = grownData;
    }

    data[size++] = val;
}

void intSet::remove(int val) {
    int foundIndex = indexOf(val);
    if (foundIndex != -1) {
        // Fill the hole by moving the last element into its spot (order doesn't matter).
        data[foundIndex] = data[size - 1];
        size--;
    }
}

intSet intSet::operator|(const intSet& otherSet) const {
    // Union: start with a copy, then add anything from the other set.
    intSet unionResult(*this);
    for (int elementIdx = 0; elementIdx < otherSet.size; elementIdx++) {
        unionResult.add(otherSet.data[elementIdx]);
    }
    return unionResult;
}

intSet intSet::operator&(const intSet& otherSet) const {
    // Intersection: keep values that appear in both sets.
    intSet intersectionResult;
    for (int elementIdx = 0; elementIdx < size; elementIdx++) {
        if (otherSet.contains(data[elementIdx])) {
            intersectionResult.add(data[elementIdx]);
        }
    }
    return intersectionResult;
}

bool intSet::operator==(const intSet& otherSet) const {
    if (size != otherSet.size) {
        return false;
    }
    return isSubset(otherSet);
}

bool intSet::isSubset(const intSet& s) const {
    // True if every element of s is also in this set.
    for (int elementIdx = 0; elementIdx < s.size; elementIdx++) {
        if (!this->contains(s.data[elementIdx])) {
            return false;
        }
    }
    return true;
}

std::ostream& operator<<(std::ostream& out, const intSet& otherSet) {
    if (otherSet.size == 0) {
        out << "()";
        return out;
    }

    // Make a sorted copy 
    int* sortedValues = new int[otherSet.size];
    for (int elementIdx = 0; elementIdx < otherSet.size; elementIdx++) {
        sortedValues[elementIdx] = otherSet.data[elementIdx];
    }

    // Simple selection sort 
    for (int i = 0; i < otherSet.size - 1; i++) {
        int smallestPos = i;
        for (int j = i + 1; j < otherSet.size; j++) {
            if (sortedValues[j] < sortedValues[smallestPos]) {
                smallestPos = j;
            }
        }
        if (smallestPos != i) {
            int swapTemp = sortedValues[i];
            sortedValues[i] = sortedValues[smallestPos];
            sortedValues[smallestPos] = swapTemp;
        }
    }

    out << "(" << sortedValues[0];
    for (int i = 1; i < otherSet.size; i++) {
        out << ", " << sortedValues[i];
    }
    out << ")";

    delete[] sortedValues;
    return out;
}
