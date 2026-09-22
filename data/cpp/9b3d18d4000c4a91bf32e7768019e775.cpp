// Write a C++ function that takes an array of integers, its size, and a target key, and returns the index of the **first** occurrence of the key in the array, or -1 if the key is not present. The function must be implemented **recursively** and must not use any loops or standard library search functions. Additionally, write a second recursive function that returns the index of the **last** occurrence of the key, or -1 if not found. The solution must be self-contained, include a single free function for each task (e.g., `firstOccurrence` and `lastOccurrence`), and handle edge cases like an empty array, a key that appears once, multiple times, or not at all.

// The task requires two recursive functions that traverse the array linearly. For the **first occurrence** (`firstOccurrence`), the recursion starts at index 0 and checks the current element; if it matches the key, return the current index immediately. If the index reaches the array size, return -1. This ensures that the first matching index (lowest) is returned because we check from the beginning and stop at the first match.
//
// For the **last occurrence** (`lastOccurrence`), the recursion first descends to the end of the array (by calling itself with `i+1` until `i == n`), then on the way back up, it checks the current element. The first match encountered during unwinding is the last occurrence in the original array order. If the recursive call returns a valid index (not -1), that value is propagated upward, overriding any later checks. If no match is found, it returns -1.
//
// Edge cases: Empty array (size 0): both functions return -1. Key present exactly once: both return the same index. Key absent: both return -1. Duplicates: first returns smallest index, last returns largest index. For recursion with `i==n`, we return -1 as the base case. Time complexity is O(n) for both because each element is visited once. Space complexity is O(n) due to recursion stack depth (not constant), but no extra heap memory.

#include <cstddef>

// Recursively find the first index of key in arr[0..n-1], or -1 if not found.
int firstOccurrence(const int arr[], std::size_t n, std::size_t i, int key) {
    if (i == n) {
        return -1;
    }
    if (arr[i] == key) {
        return static_cast<int>(i);
    }
    return firstOccurrence(arr, n, i + 1, key);
}

// Recursively find the last index of key in arr[0..n-1], or -1 if not found.
int lastOccurrence(const int arr[], std::size_t n, std::size_t i, int key) {
    if (i == n) {
        return -1;
    }
    int res = lastOccurrence(arr, n, i + 1, key);
    if (res != -1) {
        return res;
    }
    if (arr[i] == key) {
        return static_cast<int>(i);
    }
    return -1;
}

#include <cassert>
#include <cstddef>

// Declare the functions (they are defined elsewhere, but for a complete test we include the definitions here)
int firstOccurrence(const int arr[], std::size_t n, std::size_t i, int key);
int lastOccurrence(const int arr[], std::size_t n, std::size_t i, int key);

int main() {
    int arr1[] = {4, 2, 1, 2, 5, 2, 7};
    std::size_t n1 = 7;
    assert(firstOccurrence(arr1, n1, 0, 2) == 1);
    assert(lastOccurrence(arr1, n1, 0, 2) == 5);

    int arr2[] = {1, 1, 1};
    assert(firstOccurrence(arr2, 3, 0, 1) == 0);
    assert(lastOccurrence(arr2, 3, 0, 1) == 2);

    int arr3[] = {5, 6, 7};
    assert(firstOccurrence(arr3, 3, 0, 8) == -1);
    assert(lastOccurrence(arr3, 3, 0, 8) == -1);

    int arr4[] = {42};
    assert(firstOccurrence(arr4, 1, 0, 42) == 0);
    assert(lastOccurrence(arr4, 1, 0, 42) == 0);

    int arr5[] = {0, 0, 0, 0};
    assert(firstOccurrence(arr5, 4, 0, 0) == 0);
    assert(lastOccurrence(arr5, 4, 0, 0) == 3);

    // empty array (size 0) – using a pointer to any valid location; recursion stops immediately
    int arr6[] = {1}; // placeholder, but we pass n=0
    assert(firstOccurrence(arr6, 0, 0, 1) == -1);
    assert(lastOccurrence(arr6, 0, 0, 1) == -1);

    return 0;
}
