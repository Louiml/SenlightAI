Given a sorted array of integers and a target key, write a C++ function that returns the 1-based index of the first element that is greater than or equal to the key, using binary search. If no such element exists (i.e., all elements are smaller than the key), return `-1`. The input array is guaranteed to be non-empty and sorted in non-decreasing order. The function must handle duplicate values and keys outside the array's range correctly.

// This is the classic "lower bound" (first element ≥ key) problem. The solution uses a standard binary search on the sorted array. Initialize `low = 0` and `high = n-1`. While `low <= high`, compute `mid = low + (high - low) / 2` to avoid integer overflow. If `a[mid] >= key`, this element is a candidate for the answer, so we record its index (or its 1-based position) and continue searching in the left half (`high = mid - 1`) to find an even earlier occurrence. If `a[mid] < key`, we discard the left half and search the right (`low = mid + 1`). After the loop, if we never found a candidate, return `-1`; otherwise return the recorded index + 1 (since the problem asks for 1-based indexing). Edge cases include the key being smaller than or equal to the first element (answer is 1), the key being larger than all elements (answer is -1), and duplicate values where the first occurrence of a value ≥ key must be returned. Time complexity is O(log n), and space complexity is O(1).

#include <vector>
#include <cstddef>

// Return the 1-based index of the first element >= key, or -1 if none exists.
int lowerBoundIndex(const std::vector<int>& a, int key) {
    int low = 0;
    int high = static_cast<int>(a.size()) - 1;
    int result = -1; // 1-based index of the answer, -1 if not found

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (a[mid] >= key) {
            // Candidate found; store its 1-based index and search left for earlier.
            result = mid + 1;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return result;
}

#include <cassert>
#include <vector>

int lowerBoundIndex(const std::vector<int>& a, int key);

int main() {
    std::vector<int> arr = {1, 2, 3, 3, 4};

    // Basic cases
    assert(lowerBoundIndex(arr, 1) == 1);
    assert(lowerBoundIndex(arr, 2) == 2);
    assert(lowerBoundIndex(arr, 3) == 3); // first 3
    assert(lowerBoundIndex(arr, 4) == 5);
    assert(lowerBoundIndex(arr, 5) == -1); // greater than all

    // Edge: key smaller than first element
    assert(lowerBoundIndex(arr, 0) == 1);

    // Single element array
    std::vector<int> single = {7};
    assert(lowerBoundIndex(single, 7) == 1);
    assert(lowerBoundIndex(single, 6) == 1);
    assert(lowerBoundIndex(single, 8) == -1);

    // All equal
    std::vector<int> allEqual = {5, 5, 5};
    assert(lowerBoundIndex(allEqual, 5) == 1);
    assert(lowerBoundIndex(allEqual, 4) == 1);
    assert(lowerBoundIndex(allEqual, 6) == -1);
}
