Write a C++ function that implements binary search on a sorted array of integers and returns the index of a target element if found, or -1 if not present. The array is guaranteed to be sorted in ascending order. The function should work for arrays of any size, including empty arrays, and should handle duplicate values by returning any valid index where the target appears. Do not use any built-in binary search functions; implement the algorithm manually. The function signature must be `int binarySearch(const std::vector<int>& arr, int target)`. The caller will include a `main` function that creates test vectors, calls the function, and verifies results with assertions.
The solution uses the classic divide-and-conquer binary search. Maintain two indices, `left` and `right`, representing the current search interval. Initially, `left = 0` and `right = arr.size() - 1`. While `left <= right`, compute the mid index as `mid = left + (right - left) / 2` (this avoids integer overflow compared to `(left + right) / 2`). Compare `arr[mid]` with `target`: if equal, return `mid`; if less, discard the left half by setting `left = mid + 1`; if greater, discard the right half by setting `right = mid - 1`. If the loop ends without finding the target, return -1. Edge cases: an empty array immediately returns -1 (loop condition fails). Duplicates: any matching index is acceptable; the algorithm returns the first encountered mid, which is valid. Time complexity is O(log n) because each iteration halves the search space. Space complexity is O(1) as only a constant number of variables are used.
#include <vector>

// Performs binary search on a sorted (ascending) vector of integers.
// Returns the index of the target if found, otherwise -1.
int binarySearch(const std::vector<int>& arr, int target) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2; // avoids overflow
        if (arr[mid] == target) {
            return mid;
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    return -1;
}
#include <cassert>
#include <vector>

int main() {
    // Test with an empty array
    std::vector<int> empty;
    assert(binarySearch(empty, 5) == -1);

    // Test with a single element
    std::vector<int> single = {7};
    assert(binarySearch(single, 7) == 0);
    assert(binarySearch(single, 3) == -1);

    // Test with a normal sorted array, including odd and even lengths
    std::vector<int> odd = {1, 3, 5, 7, 9};
    assert(binarySearch(odd, 5) == 2);
    assert(binarySearch(odd, 1) == 0);
    assert(binarySearch(odd, 9) == 4);
    assert(binarySearch(odd, 8) == -1);

    std::vector<int> even = {2, 4, 6, 8};
    assert(binarySearch(even, 2) == 0);
    assert(binarySearch(even, 8) == 3);
    assert(binarySearch(even, 6) == 2);
    assert(binarySearch(even, 5) == -1);

    // Test with duplicate values - any valid index is acceptable
    std::vector<int> duplicates = {1, 2, 2, 2, 3};
    int idx = binarySearch(duplicates, 2);
    assert(idx >= 1 && idx <= 3);

    // Test with negative numbers and zeros
    std::vector<int> mixed = {-5, -3, 0, 2, 10};
    assert(binarySearch(mixed, -3) == 1);
    assert(binarySearch(mixed, 0) == 2);
    assert(binarySearch(mixed, 10) == 4);
    assert(binarySearch(mixed, -1) == -1);

    // Test with a large array (check for overflow in index calculation)
    std::vector<int> large(1000000, 0);
    assert(binarySearch(large, 0) != -1);
    assert(binarySearch(large, 1) == -1);

    return 0;
}
