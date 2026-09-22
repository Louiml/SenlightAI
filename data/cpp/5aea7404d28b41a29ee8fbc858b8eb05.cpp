Write a C++ function named `findOddOccurrence` that takes a non-empty array of integers and its size `n` as parameters, and returns the integer that appears an odd number of times in the array. It is guaranteed that exactly one integer appears an odd number of times, and every other integer appears an even number of times. The array may contain negative numbers, zero, and duplicate values. You must implement the solution without modifying the input array, and the function should work for any reasonable `n` (up to 10^5). The function signature must be `int findOddOccurrence(const int arr[], int n)` with appropriate `const` correctness.

#include <cassert>

int findOddOccurrence(const int arr[], std::size_t n);

int main() {
    // Basic case with positive numbers
    int arr1[] = {1, 2, 3, 2, 3, 1, 4};
    assert(findOddOccurrence(arr1, 7) == 4);

    // Single element
    int arr2[] = {5};
    assert(findOddOccurrence(arr2, 1) == 5);

    // Odd occurrence is zero
    int arr3[] = {7, 0, 7, 0, 0};
    assert(findOddOccurrence(arr3, 5) == 0);

    // Negative numbers included
    int arr4[] = {-3, -3, -7, -7, -9};
    assert(findOddOccurrence(arr4, 5) == -9);

    // Larger even counts and duplicates
    int arr5[] = {2, 2, 2, 2, 3, 3, 3, 3, 3};
    assert(findOddOccurrence(arr5, 9) == 3);

    // Unsorted array with mixed signs
    int arr6[] = {4, -1, 4, -1, 4, -1, 4, -1, 4};
    assert(findOddOccurrence(arr6, 9) == 4);

    // All elements same but odd count
    int arr7[] = {9, 9, 9};
    assert(findOddOccurrence(arr7, 3) == 9);

    // Edge: n = 2 (each appears once, but that's odd twice? Actually violates spec, but test with even pairs)
    // Here both appear once, but we stick to spec: one odd, other even -> so cannot have two odd counts.
    // Instead test with one odd and one even; even count must be 2,4,... so skip.
    // Additional test with size 4 where one value appears once
    int arr8[] = {1, 1, 2, 3};
    assert(findOddOccurrence(arr8, 4) == 3);

    // Case with large repetition of zeros and a single nonzero odd
    int arr9[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12};
    assert(findOddOccurrence(arr9, 21) == 12);

    return 0;
}

#include <cstddef>

// Given an array where exactly one integer appears an odd number of times
// and all others appear an even number of times, return that integer.
// The array is not modified; it is accessed in a read-only manner.
int findOddOccurrence(const int arr[], std::size_t n) {
    int result = 0;
    for (std::size_t i = 0; i < n; ++i) {
        result ^= arr[i];
    }
    return result;
}

// The main observation is that the XOR (exclusive OR) operation has the property that `x ^ x = 0` and `x ^ 0 = x`. XOR is also commutative and associative, so if we XOR all elements in the array together, every integer that appears an even number of times cancels out to 0, leaving only the integer that appears an odd number of times. This works regardless of the order of elements, and also correctly handles negative numbers because XOR operates on the bit representation of integers. Edge cases to consider: the array could have only one element, which is trivially the answer; the odd-occurring number could be zero or negative. We simply loop through the array once, accumulating the XOR result into a single integer initialized to 0. Time complexity is O(n) where n is the array size, since we perform exactly one traversal. Space complexity is O(1) because we use only a constant amount of extra memory.
