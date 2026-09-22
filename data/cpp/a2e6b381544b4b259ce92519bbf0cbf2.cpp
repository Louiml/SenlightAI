// Write a C++ function `findMissingPositive` that takes a non-empty vector of distinct positive integers containing every number from 1 to n except one, along with the value of n (where the vector size is n−1), and returns the missing number. The function must use only constant extra space and must not sort or use any auxiliary data structures (e.g., sets, maps, boolean arrays). The input vector may be unsorted. Ensure the function works correctly for n=1 (empty vector), n=2 (single-element vector), and for large values of n (up to 10^6).

// The classic approach uses XOR properties: XOR of a number with itself is 0, and XOR of a number with 0 is the number. We XOR all integers from 1 to n together, then XOR all elements in the array. Because every number from 1 to n appears exactly once in the full set, and the array contains all except one, the XOR of the two sets cancels out all present numbers, leaving only the missing one. Edge cases: if n=1, the array is empty, and the function should return 1 (since the only number is 1, and it's missing). For n=2, the array has one element; if that element is 1, the missing is 2, if it's 2, missing is 1. The algorithm runs in O(n) time and O(1) auxiliary space. No need to worry about overflow because XOR is bitwise and does not depend on magnitude beyond int range; however, for n up to 10^6, int is sufficient (max missing value is 10^6).

#include <vector>

// Finds the missing positive integer in a vector containing numbers 1..n except one.
// Input: arr = vector of n-1 distinct integers from 1..n, n = upper bound (size+1).
// Returns the missing integer.
int findMissingPositive(const std::vector<int>& arr, int n) {
    int xor_full = 0;  // XOR of 1..n
    int xor_arr = 0;   // XOR of array elements

    // XOR all numbers from 1 to n-1 and all array elements in one loop
    for (int i = 0; i < n - 1; ++i) {
        xor_arr ^= arr[i];
        xor_full ^= (i + 1);
    }
    xor_full ^= n;  // Include the last number n

    return xor_full ^ xor_arr;
}

#include <cassert>
#include <vector>
using namespace std;

int main() {
    // n=1, empty array, missing 1
    assert(findMissingPositive({}, 1) == 1);

    // n=2, array [1] -> missing 2
    assert(findMissingPositive({1}, 2) == 2);

    // n=2, array [2] -> missing 1
    assert(findMissingPositive({2}, 2) == 1);

    // n=3, array [1,3] -> missing 2
    assert(findMissingPositive({1,3}, 3) == 2);

    // Unsorted larger case
    vector<int> arr1 = {3, 7, 1, 2, 8, 4, 5}; // n=8, missing 6
    assert(findMissingPositive(arr1, 8) == 6);

    // Another unsorted case
    vector<int> arr2 = {9, 3, 5, 1, 4, 8, 2, 7}; // n=9, missing 6
    assert(findMissingPositive(arr2, 9) == 6);

    // Large n simple case
    vector<int> arr3;
    for (int i = 1; i <= 1000000; ++i) {
        if (i != 500000) arr3.push_back(i);
    }
    assert(findMissingPositive(arr3, 1000000) == 500000);

    return 0;
}
