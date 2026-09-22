/*
Write a C++ function named `orderAgnosticSearch` that takes a sorted array (either strictly ascending or strictly descending), its size, and a target value, and returns the index of the target if present, otherwise returns `-1`. The function must detect the order of the array (ascending or descending) on the first call dynamically by comparing the first two elements, and then perform a standard binary search tailored to that order. Assume the array has at least two elements, all elements are distinct, and the array is already sorted in one of the two orders. The function must be `const`-correct: take the array as a `const int*` and the size as `std::size_t`, and operate without modifying the input.
*/

#include <cstddef>

// Perform order-agnostic binary search on a sorted array (ascending or descending).
// Returns the index of target if found, otherwise -1.
int orderAgnosticSearch(const int arr[], std::size_t size, int target) {
    std::size_t low = 0;
    std::size_t high = size - 1;
    
    // Determine order: ascending if arr[0] < arr[1], else descending.
    bool isAscending = arr[0] < arr[1];
    
    while (low <= high) {
        std::size_t mid = low + (high - low) / 2;
        
        if (arr[mid] == target) {
            return static_cast<int>(mid);
        }
        
        if (isAscending) {
            if (arr[mid] < target) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        } else {
            if (arr[mid] > target) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }
    
    return -1;
}

#include <cassert>
#include <cstddef>

int orderAgnosticSearch(const int arr[], std::size_t size, int target);

int main() {
    // Ascending order
    int asc[] = {1, 3, 5, 7, 9};
    assert(orderAgnosticSearch(asc, 5, 7) == 3);
    assert(orderAgnosticSearch(asc, 5, 1) == 0);
    assert(orderAgnosticSearch(asc, 5, 9) == 4);
    assert(orderAgnosticSearch(asc, 5, 4) == -1);
    
    // Descending order
    int desc[] = {40, 10, 5, 2, 1};
    assert(orderAgnosticSearch(desc, 5, 10) == 1);
    assert(orderAgnosticSearch(desc, 5, 40) == 0);
    assert(orderAgnosticSearch(desc, 5, 1) == 4);
    assert(orderAgnosticSearch(desc, 5, 3) == -1);
    
    // Edge case: size 2
    int asc2[] = {2, 4};
    int desc2[] = {4, 2};
    assert(orderAgnosticSearch(asc2, 2, 2) == 0);
    assert(orderAgnosticSearch(asc2, 2, 4) == 1);
    assert(orderAgnosticSearch(desc2, 2, 4) == 0);
    assert(orderAgnosticSearch(desc2, 2, 2) == 1);
    
    // Large case to verify no overflow in mid calculation
    constexpr std::size_t bigSize = 1000000;
    int* bigAsc = new int[bigSize];
    for (std::size_t i = 0; i < bigSize; ++i) bigAsc[i] = static_cast<int>(i) * 2;
    assert(orderAgnosticSearch(bigAsc, bigSize, bigSize*2 - 2) == bigSize - 1);
    assert(orderAgnosticSearch(bigAsc, bigSize, 0) == 0);
    assert(orderAgnosticSearch(bigAsc, bigSize, 1) == -1);
    delete[] bigAsc;
    
    return 0;
}

// The solution first determines whether the array is sorted in ascending or descending order by comparing `arr[0]` and `arr[1]`. If `arr[0] < arr[1]`, it is ascending; otherwise descending. Then a standard binary search is performed within a while loop (`low <= high`) using `mid = low + (high - low) / 2` to avoid overflow. For ascending order, if the middle element is less than the target, we move `low` to `mid+1`; otherwise, we move `high` to `mid-1`. For descending order, the logic is inverted: if the middle element is greater than the target, we move `low` to `mid+1`; otherwise, we move `high` to `mid-1`. If the loop exits without finding the target, return `-1`. Edge cases: since the array is guaranteed sorted and distinct, no duplicates or empty arrays need handling, but if the array size is less than 2, we could fall back to a linear check; the task assumes size ≥2. Time complexity is O(log n) and space complexity is O(1).
