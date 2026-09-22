Write a C++ function named `firstOccurrence` that takes a sorted vector of integers and a target key, and returns the index of the first occurrence of that key in the vector (i.e., the smallest index `i` such that `vec[i] == key`). If the key is not present, return `-1`. You must implement the search using a binary-search–style algorithm (do not use linear search or `std::find`). The input vector is guaranteed to be sorted in non-decreasing order, but it may contain duplicate values. Your function should be `const`-correct: the vector must be accepted as a `const std::vector<int>&` parameter.

The core idea is an extended binary search that does not stop as soon as a match is found, but instead continues searching the left half to locate the leftmost index of the key. Maintain two pointers: `low = 0` and `high = vec.size() - 1`. While `low <= high`, compute `mid` using `low + (high - low) / 2` to avoid integer overflow. If `vec[mid] == key`, we record `mid` as a candidate answer and set `high = mid - 1` to search for an earlier occurrence. If `vec[mid] < key`, move `low = mid + 1`; otherwise move `high = mid - 1`. At the end, return the recorded candidate (or `-1` if none was found). Edge cases: empty vector should return `-1` immediately; if the key appears at the very first position, the algorithm will still find it and return `0`; if duplicates exist, the leftmost index is found. Time complexity is `O(log n)` where `n` is the size of the vector, and space complexity is `O(1)`.

#include <vector>

// Return the index of the first occurrence of key in a sorted vector.
// If key is not present, return -1.
int firstOccurrence(const std::vector<int>& vec, int key) {
    if (vec.empty()) {
        return -1;
    }

    int low = 0;
    int high = static_cast<int>(vec.size()) - 1;
    int result = -1;

    while (low <= high) {
        int mid = low + (high - low) / 2; // avoid overflow

        if (vec[mid] == key) {
            result = mid;      // possible candidate
            high = mid - 1;    // look for earlier occurrence
        } else if (vec[mid] < key) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return result;
}

#include <cassert>
#include <vector>

// The solution function is declared above; include it here if needed.
// For the test, we assume it is already declared.

int main() {
    std::vector<int> v1 = {1, 2, 2, 2, 3, 4, 5};
    assert(firstOccurrence(v1, 2) == 1);
    assert(firstOccurrence(v1, 1) == 0);
    assert(firstOccurrence(v1, 5) == 6);
    assert(firstOccurrence(v1, 6) == -1);

    std::vector<int> v2 = {7, 7, 7, 7};
    assert(firstOccurrence(v2, 7) == 0);
    assert(firstOccurrence(v2, 8) == -1);

    std::vector<int> v3 = {10};
    assert(firstOccurrence(v3, 10) == 0);
    assert(firstOccurrence(v3, 9) == -1);

    std::vector<int> v4 = {};
    assert(firstOccurrence(v4, 1) == -1);

    std::vector<int> v5 = {-5, -3, -3, 0, 2};
    assert(firstOccurrence(v5, -3) == 1);
    assert(firstOccurrence(v5, 0) == 3);

    return 0;
}
