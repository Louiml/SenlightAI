Write a C++ function `int exponentialSearch(const std::vector<int>& vec, int target)` that performs an exponential search on a **sorted** vector of integers. The function must return the index of the target if it exists, or `-1` if it is not present. The vector is guaranteed to be sorted in ascending order, but the input may contain duplicates; in that case, any valid index of the target is acceptable. The function should handle vectors of any size (including empty vectors) and targets smaller than the first element or larger than the last element efficiently. Do not modify the input vector, and use binary search internally after determining a suitable range.

The exponential search algorithm works by first checking if the first element equals the target. If not, it doubles an index `right` starting from 1 while `right < n` and `vec[right] < target`. This quickly narrows the search to a range `[right/2, min(right, n-1)]` where the target may lie. Once the range is found, a standard binary search is performed within that range. Edge cases include an empty vector (return -1 immediately), a target less than the first element (the doubling loop never runs, and binary search on `[0, min(1, n-1)]` will correctly return -1), and a target greater than all elements (the loop terminates when `right >= n`, and binary search on `[right/2, n-1]` correctly returns -1). If duplicates exist, the first match found by binary search is acceptable. Time complexity is `O(log i)` where `i` is the index of the target (or the position where it would be), and space complexity is `O(1)` as no extra data structures are used.

#include <vector>
#include <algorithm>

// Perform exponential search on a sorted vector.
// Returns the index of target if found, otherwise -1.
int exponentialSearch(const std::vector<int>& vec, int target) {
    // Handle empty vector
    if (vec.empty()) {
        return -1;
    }

    // Check first element
    if (vec[0] == target) {
        return 0;
    }

    int n = static_cast<int>(vec.size());
    int right = 1;

    // Double the range until we exceed the target or vector bounds
    while (right < n && vec[right] < target) {
        right *= 2;
    }

    // Define the search bounds for binary search
    int left = right / 2;
    int boundedRight = std::min(right, n - 1);

    // Standard binary search within [left, boundedRight]
    while (left <= boundedRight) {
        int mid = left + (boundedRight - left) / 2;
        if (vec[mid] == target) {
            return mid;
        } else if (vec[mid] < target) {
            left = mid + 1;
        } else {
            boundedRight = mid - 1;
        }
    }

    return -1;
}

#include <cassert>
#include <vector>

int exponentialSearch(const std::vector<int>& vec, int target);

int main() {
    // Basic present target
    std::vector<int> v1 = {1, 3, 5, 7, 9};
    assert(exponentialSearch(v1, 5) == 2);
    assert(exponentialSearch(v1, 1) == 0);
    assert(exponentialSearch(v1, 9) == 4);

    // Absent target within range
    assert(exponentialSearch(v1, 4) == -1);
    // Target less than first
    assert(exponentialSearch(v1, 0) == -1);
    // Target greater than last
    assert(exponentialSearch(v1, 10) == -1);

    // Empty vector
    std::vector<int> v2;
    assert(exponentialSearch(v2, 0) == -1);

    // Single element vector
    std::vector<int> v3 = {42};
    assert(exponentialSearch(v3, 42) == 0);
    assert(exponentialSearch(v3, 41) == -1);

    // Duplicates - any valid index accepted
    std::vector<int> v4 = {1, 2, 2, 2, 3};
    int result = exponentialSearch(v4, 2);
    assert(result >= 1 && result <= 3);

    // Larger vector to verify doubling behavior
    std::vector<int> v5 = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120};
    assert(exponentialSearch(v5, 80) == 7);
    assert(exponentialSearch(v5, 75) == -1);
    assert(exponentialSearch(v5, 120) == 11);

    // Input remains unmodified (const correctness)
    std::vector<int> original = {1, 3, 5};
    exponentialSearch(original, 3);
    assert(original[0] == 1 && original[1] == 3 && original[2] == 5);

    return 0;
}
