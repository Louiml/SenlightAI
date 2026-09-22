/*
Write a C++ function `int binary_search(const std::vector<int>& a, int x)` that, given a strictly increasing (sorted) vector of distinct integers and a target value `x`, returns the index of `x` if found, or `-1` otherwise. The function must implement the binary search algorithm with O(log n) time complexity. The input vector is guaranteed to be sorted in ascending order and may contain any number of elements (possibly zero, in which case always return `-1`). Handle edge cases such as empty vectors, targets smaller than the first element, larger than the last element, and exact matches at the boundaries.
*/

#include <vector>

// Performs binary search on a sorted vector 'a' (ascending order) for target 'x'.
// Returns the index if found, otherwise -1. 
int binary_search(const std::vector<int>& a, int x) {
    int left = 0;
    int right = static_cast<int>(a.size()); // exclusive upper bound

    while (left < right) {
        int mid = left + (right - left) / 2; // avoid overflow
        if (a[mid] == x) {
            return mid;
        } else if (a[mid] < x) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return -1;
}

#include <cassert>
#include <vector>

// Assume binary_search is declared above (include the solution code before this main).

int main() {
    std::vector<int> empty = {};
    assert(binary_search(empty, 5) == -1);

    std::vector<int> single = {10};
    assert(binary_search(single, 10) == 0);
    assert(binary_search(single, 5) == -1);
    assert(binary_search(single, 15) == -1);

    std::vector<int> small = {1, 3, 5, 7};
    assert(binary_search(small, 1) == 0);
    assert(binary_search(small, 7) == 3);
    assert(binary_search(small, 3) == 1);
    assert(binary_search(small, 0) == -1);
    assert(binary_search(small, 4) == -1);
    assert(binary_search(small, 8) == -1);

    std::vector<int> large = {2, 4, 6, 8, 10, 12, 14};
    assert(binary_search(large, 2) == 0);
    assert(binary_search(large, 14) == 6);
    assert(binary_search(large, 10) == 4);
    assert(binary_search(large, 7) == -1);
    assert(binary_search(large, 100) == -1);

    // Negative numbers and zero
    std::vector<int> negatives = {-5, -3, 0, 2, 9};
    assert(binary_search(negatives, -5) == 0);
    assert(binary_search(negatives, 0) == 2);
    assert(binary_search(negatives, 9) == 4);
    assert(binary_search(negatives, -1) == -1);

    return 0;
}

// Binary search works by repeatedly dividing the search interval in half. Maintain two pointers: `left` starting at index 0 and `right` initially set to `a.size()` (exclusive upper bound). While `left < right`, compute the middle index as `mid = left + (right - left) / 2` (this avoids integer overflow). If `a[mid] == x`, return `mid`. If `a[mid] < x`, the target must be in the right half, so set `left = mid + 1`. Otherwise, set `right = mid`. If the loop exits without finding `x`, return `-1`. This approach works correctly for empty vectors because the initial condition `left < right` is false and the function returns `-1`. Edge cases like `x` being smaller than the first element or larger than the last element naturally cause the loop to shrink the interval to exhaustion and return `-1`. Time complexity is O(log n) and space complexity is O(1) since only a few integer variables are used.
