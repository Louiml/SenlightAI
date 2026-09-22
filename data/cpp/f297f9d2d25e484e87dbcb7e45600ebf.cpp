Write a C++ function named `findMissingNumbers` that takes an array of 28 unique integers, each in the range 1 to 30 inclusive, and returns a `std::vector<int>` containing the two missing integers (i.e., the numbers from 1 to 30 that are not present in the input) in ascending order. The input array may be unsorted and may not contain duplicates (since every integer from 1 to 30 appears at most once, and exactly 28 are provided). The function must handle cases where the missing numbers are consecutive or at the boundaries (1 and/or 30).
The simplest and most efficient approach is to use a fixed-size boolean array (or a `std::bitset`) of length 31 (indices 0–30, with index 0 unused) to mark which numbers have been seen. Iterate over the input array and set `present[n] = true` for each `n`. Then iterate from 1 to 30, collecting all indices `i` where `present[i]` is false into a result vector. Since the input is guaranteed to have exactly 28 unique numbers, the result will always contain exactly 2 elements. No sorting is needed because we collect in increasing order by index. Edge cases include missing numbers 1 and 2 (at the low end), 29 and 30 (at the high end), and any pair in between. Time complexity is O(30) ≈ O(1) for input size 28 (or O(n) if n is the range size), and space complexity is O(1) additional for the boolean array (fixed size).
#include <vector>
#include <array>

// Given 28 unique integers from 1 to 30, return the two missing numbers in ascending order.
std::vector<int> findMissingNumbers(const std::array<int, 28>& numbers) {
    bool present[31] = {false}; // index 0 unused, indices 1..30 valid
    for (int num : numbers) {
        present[num] = true;
    }

    std::vector<int> missing;
    for (int i = 1; i <= 30; ++i) {
        if (!present[i]) {
            missing.push_back(i);
        }
    }
    return missing; // always contains exactly 2 elements
}
#include <cassert>
#include <vector>
#include <array>

// (Function definition from Solution section goes here)

int main() {
    std::array<int, 28> test1 = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28};
    assert(findMissingNumbers(test1) == std::vector<int>({29,30}));

    std::array<int, 28> test2 = {3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
    assert(findMissingNumbers(test2) == std::vector<int>({1,2}));

    std::array<int, 28> test3 = {1,2,3,4,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29};
    assert(findMissingNumbers(test3) == std::vector<int>({5,30}));

    std::array<int, 28> test4 = {30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3};
    assert(findMissingNumbers(test4) == std::vector<int>({1,2}));

    std::array<int, 28> test5 = {1,30,2,29,3,28,4,27,5,26,6,25,7,24,8,23,9,22,10,21,11,20,12,19,13,18,14,17};
    assert(findMissingNumbers(test5) == std::vector<int>({15,16}));
}
