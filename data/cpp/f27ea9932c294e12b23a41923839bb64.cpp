// Write a C++ function named `findSingleOccurrence` that takes a non-empty array of integers and its size, and returns the value that appears exactly once in the array, given that every other value appears exactly twice. The array may contain negative numbers and zeros, and the order of elements is arbitrary. The function must be `const`‑correct (accept a pointer to const data) and must not modify the input array. You may assume the input is always valid per the problem guarantee (at least one element, and exactly one element with an odd count).

// The core algorithm uses the bitwise XOR operator. XOR has the property that `a ^ a = 0` and `a ^ 0 = a`, and it is commutative and associative. Therefore, if we XOR all elements in the array, every pair of equal values cancels out to 0, leaving only the value that appears once. This works for any integer type, including negative numbers, because XOR operates on the binary representation of the value. Edge cases: the array may contain only one element (the answer is that element), or the unique element may be zero (`0`). The algorithm handles these naturally because starting `ans = 0` and XORing with the only element gives that element, and XORing with zero leaves the result unchanged. Time complexity is `O(n)` because we traverse the array once. Space complexity is `O(1)` since we use a single accumulator variable.

#include <cstddef>

// Returns the single element that appears once, all others appear twice.
// Assumes valid input: non-empty array, exactly one element with odd frequency.
int findSingleOccurrence(const int* arr, std::size_t size) {
    int result = 0;
    for (std::size_t i = 0; i < size; ++i) {
        result ^= arr[i];
    }
    return result;
}

#include <cassert>

// The function is declared above (assume available).

int main() {
    // Example from the snippet
    int arr1[7] = {2, 3, 1, 6, 3, 6, 2};
    assert(findSingleOccurrence(arr1, 7) == 1);

    // Single element
    int arr2[1] = {42};
    assert(findSingleOccurrence(arr2, 1) == 42);

    // Unique value is zero
    int arr3[5] = {1, 0, 1, 2, 2};
    assert(findSingleOccurrence(arr3, 5) == 0);

    // Negative numbers
    int arr4[5] = {-5, 1, -5, 2, 1};
    assert(findSingleOccurrence(arr4, 5) == 2);

    // Larger mix with duplicates
    int arr5[9] = {7, 3, 3, 9, 7, 9, 0, 4, 4};
    assert(findSingleOccurrence(arr5, 9) == 0);

    // Only pairs and no unique? Not allowed, but test an array with two equal pairs and one unique
    int arr6[5] = {100, 100, -1, 50, 50};
    assert(findSingleOccurrence(arr6, 5) == -1);

    // Duplicates at beginning and end
    int arr7[5] = {11, 22, 11, 22, 33};
    assert(findSingleOccurrence(arr7, 5) == 33);

    // All zeros except one
    int arr8[5] = {0, 0, 0, 5, 0};
    assert(findSingleOccurrence(arr8, 5) == 5);

    // Two identical pairs and unique large positive
    int arr9[5] = {123456, 789, 123456, 789, 55};
    assert(findSingleOccurrence(arr9, 5) == 55);
}
