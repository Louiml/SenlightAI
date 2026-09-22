Write a C++ function that takes an array of integers and its size (where every integer appears exactly three times except for one integer that appears exactly once) and returns the integer that occurs only once. The function must handle arrays of any non-negative size, including the edge case where the input contains only the single unique element (size 1). You may assume the input array is non-empty. The algorithm must not use extra data structures like hash maps or sets, and it must work for both positive and negative integers, as well as zero. The function signature should be `int findSingleOccurrence(const int arr[], int n);`.

#include <cassert>

int main() {
    // Basic case from the problem statement
    int arr1[] = {4, 3, 5, 5, 4, 4, 5};
    assert(findSingleOccurrence(arr1, 7) == 3);

    // Single element only
    int arr2[] = {42};
    assert(findSingleOccurrence(arr2, 1) == 42);

    // Unique element is zero
    int arr3[] = {7, 7, 7, 0, 2, 2, 2};
    assert(findSingleOccurrence(arr3, 7) == 0);

    // Unique element is negative
    int arr4[] = {-5, 1, 1, 1, -5, -5, 9, 9, 9};
    assert(findSingleOccurrence(arr4, 9) == -5);

    // Mixed large positive and negative values
    int arr5[] = {1000000, -1000000, 1000000, 1000000, -1000000, -1000000, 12345};
    assert(findSingleOccurrence(arr5, 7) == 12345);

    // All elements identical except unique appears once
    int arr6[] = {8, 8, 8, 8, 8, 8, 3};
    assert(findSingleOccurrence(arr6, 7) == 3);

    // Unique element is the maximum int
    int arr7[] = {1, 1, 1, 2147483647, 2, 2, 2};
    assert(findSingleOccurrence(arr7, 7) == 2147483647);

    // Unique element is the minimum int
    int arr8[] = {0, 0, 0, -2147483647 - 1, 5, 5, 5};
    assert(findSingleOccurrence(arr8, 7) == (-2147483647 - 1));

    // Random but valid input
    int arr9[] = {2, 2, 2, 4, 4, 4, 6, 6, 6, 8};
    assert(findSingleOccurrence(arr9, 10) == 8);

    // Large n with repeated pattern
    int arr10[] = {10, 20, 30, 10, 20, 30, 10, 20, 30, 99};
    assert(findSingleOccurrence(arr10, 10) == 99);
}

#include <cstddef>

// Returns the element that appears exactly once in an array where
// every other element appears exactly three times.
// Assumes n > 0 and that the input satisfies the triple-occurrence property.
int findSingleOccurrence(const int arr[], int n) {
    int result = 0;
    const int INT_BITS = 32; // sizeof(int) * 8 is also fine, but 32 is typical

    for (int bit = 0; bit < INT_BITS; ++bit) {
        int mask = 1 << bit;
        int sum = 0;
        for (int i = 0; i < n; ++i) {
            if (arr[i] & mask) {
                ++sum;
            }
        }
        if (sum % 3 != 0) {
            result |= mask;
        }
    }
    return result;
}

// The solution leverages bitwise counting across all 32 bits of an integer (assuming 32-bit `int`). For each bit position from 0 to 31, we count how many numbers in the array have that bit set (`sum`). Since every number except the unique one appears exactly three times, for each bit position, the count modulo 3 will be 1 if and only if the unique number has that bit set, and 0 otherwise. We accumulate these bits into a result integer using bitwise OR. This works for both positive and negative numbers because the bit representation of negative numbers (two's complement) is handled consistently: the sign bit (bit 31) will be counted as set for negative numbers, and the modulo 3 logic still applies. Edge cases: if the array has size 1, the loop counts each bit of that single element, and `sum % 3` will be 1 for every set bit, so the result equals that element. If the unique element is 0, its bits are all zero, so the result remains 0. If the unique element is negative, the sign bit contributes correctly. Time complexity is \(O(32 \cdot n) = O(n)\) since 32 is a constant. Space complexity is \(O(1)\) extra space (only a few integer variables). The algorithm does not modify the input array, so it is safe for `const` correctness.
