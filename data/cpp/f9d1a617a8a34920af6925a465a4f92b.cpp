/*
Write a C++ function `transformArray` that takes an array of integers and its size `n` as parameters, reads `n` integers from standard input into the array, reverses the array in place, and then swaps every pair of adjacent elements (indices 0 and 1, 2 and 3, etc.) in the already-reversed array. If `n` is odd, the last element remains unchanged after the swap step. The function should not return anything (void). The input is guaranteed to contain exactly `n` integers, each fitting in `int`. After the function completes, the modified array should be directly observable by the caller.
*/
#include <algorithm>
#include <cstddef>

// Reads n integers from standard input into arr, then transforms the array:
// first reverses it in place, then swaps adjacent pairs (indices 0-1, 2-3, ...).
void transformArray(int* arr, std::size_t n) {
    for (std::size_t i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    // Reverse the array in place.
    for (std::size_t i = 0; i < n / 2; ++i) {
        std::swap(arr[i], arr[n - 1 - i]);
    }

    // Swap adjacent pairs in the reversed array.
    for (std::size_t i = 0; i + 1 < n; i += 2) {
        std::swap(arr[i], arr[i + 1]);
    }
}
#include <cassert>
#include <sstream>
#include <iostream>

// Function declaration (from solution)
void transformArray(int* arr, std::size_t n);

int main() {
    // Test 1: even length, typical case
    {
        int arr[4];
        std::istringstream input("1 2 3 4");
        std::cin.rdbuf(input.rdbuf());
        transformArray(arr, 4);
        assert(arr[0] == 2 && arr[1] == 1 && arr[2] == 4 && arr[3] == 3);
    }
    // Test 2: odd length, last element stays
    {
        int arr[5];
        std::istringstream input("10 20 30 40 50");
        std::cin.rdbuf(input.rdbuf());
        transformArray(arr, 5);
        // reversed: 50 40 30 20 10, then swap pairs: 40 50 20 30 10
        assert(arr[0] == 40 && arr[1] == 50 && arr[2] == 20 && arr[3] == 30 && arr[4] == 10);
    }
    // Test 3: single element, no change
    {
        int arr[1] = {0};
        std::istringstream input("99");
        std::cin.rdbuf(input.rdbuf());
        transformArray(arr, 1);
        assert(arr[0] == 99);
    }
    // Test 4: two elements, simple swap after reversal
    {
        int arr[2];
        std::istringstream input("7 8");
        std::cin.rdbuf(input.rdbuf());
        transformArray(arr, 2);
        // reversed: 8 7, swap pairs: 7 8
        assert(arr[0] == 7 && arr[1] == 8);
    }
    // Test 5: three elements, middle becomes last
    {
        int arr[3];
        std::istringstream input("1 2 3");
        std::cin.rdbuf(input.rdbuf());
        transformArray(arr, 3);
        // reversed: 3 2 1, swap pairs (0,1): 2 3 1
        assert(arr[0] == 2 && arr[1] == 3 && arr[2] == 1);
    }
    // Test 6: negative numbers and duplicates
    {
        int arr[6];
        std::istringstream input("-5 0 -5 10 10 2");
        std::cin.rdbuf(input.rdbuf());
        transformArray(arr, 6);
        // reversed: 2 10 10 -5 0 -5; swap pairs: 10 2 -5 10 -5 0
        assert(arr[0] == 10 && arr[1] == 2 && arr[2] == -5 && arr[3] == 10 && arr[4] == -5 && arr[5] == 0);
    }
    return 0;
}
// The algorithm is a two-phase in-place transformation of the array. First, read all `n` integers from `std::cin` into the array using a simple loop. Then, reverse the array by swapping the first and last elements, moving inward until the middle is reached; this is done with a loop that runs for `i` from `0` to `n/2 - 1`, swapping `arr[i]` with `arr[n-1-i]`. After reversal, perform adjacent swapping by iterating `i` from `0` to `n-2` in steps of 2, swapping `arr[i]` and `arr[i+1]` using `std::swap`. Edge cases: for `n = 0` or `n = 1`, both phases are no-ops (the reversal loop condition `i < n/2` and the swap loop condition `i < n-1` fail immediately); for even `n`, all pairs are swapped perfectly; for odd `n` (e.g., 3), the last element (index `n-1`) is never part of a pair because the loop stops at `n-2` (for `n=3`, `i` only takes `0`, swapping indices 0 and 1, leaving index 2 unchanged). Time complexity: reading takes O(n), reversal takes O(n/2), adjacent swapping takes O(n/2), so total O(n). Space complexity: O(1) auxiliary (excluding input array storage), since only loop variables and a temporary for swap are used.
