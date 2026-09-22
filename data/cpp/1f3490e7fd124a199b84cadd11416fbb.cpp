// Write a C++ function `stableSortByParity` that takes a `std::vector<int>&` and reorders its elements in-place so that all even numbers appear first (in their original relative order) and all odd numbers appear after them (also in their original relative order). The function must be stable, meaning the relative order of elements within the even group and within the odd group must be preserved. The input vector may be empty, may contain duplicates, and may contain negative numbers. The function should return `void` and modify the vector directly.
The problem requires a stable partition of the vector into even and odd elements. A simple approach is to use the standard library algorithm `std::stable_partition` from `<algorithm>`, which performs exactly this task on a range while preserving the relative order of elements that satisfy the predicate (even numbers) and those that do not (odd numbers). The predicate can be a lambda `[](int x) { return x % 2 == 0; }`. However, for negative numbers, `x % 2` returns `-1` or `0` in C++ (since the sign of the remainder matches the dividend), so we must check `x % 2 == 0` (which works correctly: `-4 % 2 == 0`, `-3 % 2 == -1`). Alternatively, we could implement a manual stable partition using a temporary vector to copy evens and odds, but `std::stable_partition` is both simpler and optimal. The algorithm runs in O(n) time and uses O(n) auxiliary space (as a stable partition may allocate a temporary buffer). Edge cases: an empty vector requires no action; a vector with all evens or all odds remains unchanged; duplicates are preserved in their original order. If we were to implement a manual solution, we'd create two vectors for evens and odds, iterate through the input, push back accordingly, then clear the input and append the evens followed by odds—this also gives O(n) time and O(n) space.
#include <vector>
#include <algorithm>

// Stable reorder: all evens first, then all odds, preserving relative order within each group.
void stableSortByParity(std::vector<int>& arr) {
    // std::stable_partition preserves the relative order of elements.
    // The predicate returns true for even numbers (including negative evens).
    std::stable_partition(arr.begin(), arr.end(), [](int x) {
        return x % 2 == 0;
    });
}
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution function here (or link to it)

int main() {
    // Test 1: simple mix
    std::vector<int> v1 = {1, 2, 3, 4, 5, 6};
    stableSortByParity(v1);
    assert((v1 == std::vector<int>{2, 4, 6, 1, 3, 5}));

    // Test 2: negative numbers
    std::vector<int> v2 = {-3, -2, -1, 0, 1, 2};
    stableSortByParity(v2);
    assert((v2 == std::vector<int>{-2, 0, 2, -3, -1, 1}));

    // Test 3: all evens
    std::vector<int> v3 = {4, 8, 12};
    stableSortByParity(v3);
    assert((v3 == std::vector<int>{4, 8, 12}));

    // Test 4: all odds
    std::vector<int> v4 = {3, 5, 7};
    stableSortByParity(v4);
    assert((v4 == std::vector<int>{3, 5, 7}));

    // Test 5: empty vector
    std::vector<int> v5 = {};
    stableSortByParity(v5);
    assert(v5.empty());

    // Test 6: duplicates
    std::vector<int> v6 = {1, 2, 1, 2, 3};
    stableSortByParity(v6);
    assert((v6 == std::vector<int>{2, 2, 1, 1, 3}));

    // Test 7: single element even
    std::vector<int> v7 = {10};
    stableSortByParity(v7);
    assert((v7 == std::vector<int>{10}));

    // Test 8: single element odd
    std::vector<int> v8 = {7};
    stableSortByParity(v8);
    assert((v8 == std::vector<int>{7}));

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
