/*
Write a C++ function that takes an array of integers and its size, and returns the smallest positive integer (greater than 0) that is missing from the array. The array may contain negative numbers, zeros, duplicates, and positive numbers up to a large range. If all positive integers from 1 up to the maximum positive element in the array are present, then the function should return the maximum positive element plus one. The function must handle edge cases like an empty array (size 0, though in practice it will be called with size >= 1), and arrays with only zeros or negative numbers, in which case the smallest missing positive integer is 1.
*/

#include <unordered_map>

/*
 * Returns the smallest positive integer (>0) that is not present in arr.
 * If all positive integers from 1 up to max(arr) are present, returns max(arr)+1.
 * Assumes n >= 0.
 */
int smallestMissingPositive(const int arr[], int n) {
    int max_positive = 0;
    std::unordered_map<int, bool> present;
    for (int i = 0; i < n; ++i) {
        if (arr[i] > max_positive) {
            max_positive = arr[i];
        }
        present[arr[i]] = true;
    }

    // The candidate missing number can be at most max_positive + 1,
    // but if there are no positive numbers, max_positive = 0, so answer is 1.
    int limit = max_positive + 1;
    for (int candidate = 1; candidate <= limit; ++candidate) {
        if (present.find(candidate) == present.end()) {
            return candidate;
        }
    }
    // Unreachable in practice, but keep the compiler happy.
    return limit;
}

#include <cassert>

int main() {
    int arr1[] = {1, 2, 3};
    assert(smallestMissingPositive(arr1, 3) == 4);

    int arr2[] = {0, -1, 2};
    assert(smallestMissingPositive(arr2, 3) == 1);

    int arr3[] = {2, 3, 1, 5};
    assert(smallestMissingPositive(arr3, 4) == 4);

    int arr4[] = {1, 1, 1};
    assert(smallestMissingPositive(arr4, 3) == 2);

    int arr5[] = {-5, -3, -1};
    assert(smallestMissingPositive(arr5, 3) == 1);

    int arr6[] = {100, 200, 300};
    assert(smallestMissingPositive(arr6, 3) == 1);

    int arr7[] = {1};
    assert(smallestMissingPositive(arr7, 1) == 2);

    int arr8[] = {0};
    assert(smallestMissingPositive(arr8, 1) == 1);

    int arr9[] = {1, 2, 3, 4, 5};
    assert(smallestMissingPositive(arr9, 5) == 6);

    int arr10[] = {5, 4, 3, 2, 1};
    assert(smallestMissingPositive(arr10, 5) == 6);
}

// The approach uses a hash map (or unordered_set) to record the presence of each element. First, track the maximum positive value encountered in the array while inserting elements into the map. Since the smallest positive missing number is at most max_positive + 1, we only need to check integers from 1 to max_positive + 1. Iterate from 1 upward, and for each integer check if it exists in the map. The first integer not found is the answer. Edge cases: if all numbers are non-positive, max_positive remains at its initial value (we initialize it to 0 or to 1? In the snippet they initialize to 1, but if all negative, maxi=1, then loop 1 to 2, if 1 not present, return 1). If the array contains a continuous sequence from 1 to n, the loop will check up to max_positive+1 and return that. Time complexity is O(n) for insertion and O(max_positive) for the search, which in the worst case when the array contains all numbers from 1 to n, max_positive = n, so overall O(n). Space complexity is O(n) for the hash map.
