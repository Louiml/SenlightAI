// Write a C++ function named `sortColorsInPlace` that takes a `std::vector<int>& nums` containing only the integers 0, 1, and 2 (representing colors red, white, and blue respectively) in any order and sorts the vector in ascending order so that all 0s come first, then all 1s, then all 2s. The function must modify the vector in place without using any extra data structures or the standard library sort; use a counting approach. The vector may be empty, may contain duplicates, and may already be sorted. Your function should handle any size up to a few thousand elements efficiently and must not print anything.
The simplest and most direct solution leverages the fact that the input values are restricted to the small set {0, 1, 2}. This allows a counting sort: first count how many 0s, 1s, and 2s appear in the input by iterating through the vector once and storing the counts in a fixed-size array of three integers. Then, overwrite the original vector by filling in the correct number of 0s first, then the correct number of 1s, then the correct number of 2s. This avoids any comparisons between elements and runs in linear time. Edge cases: an empty vector requires no changes (the counts will all be zero, and the overwrite loop will do nothing); vectors with only one value work naturally; vectors already sorted will produce the same result. Time complexity is O(n) for counting plus O(n) for overwriting, so overall O(n) where n is the number of elements. Space complexity is O(1) because the counter array has a fixed size of three, regardless of input size. The implementation should use a fixed array (e.g., `int counts[3] = {0,0,0};`) initialized to zero, increment counts in a ranged-for loop, then use nested loops or pointer arithmetic to rewrite the vector. The function signature should be `void sortColorsInPlace(std::vector<int>& nums)` with no `const` because it modifies the input.
#include <vector>

// Sorts a vector containing only 0, 1, and 2 in ascending order in place.
// Uses counting sort with a fixed-size counter array because the values are limited.
void sortColorsInPlace(std::vector<int>& nums) {
    int counts[3] = {0, 0, 0};
    
    // Count occurrences of each color value
    for (int value : nums) {
        ++counts[value];
    }
    
    // Overwrite the vector with sorted values: 0s, then 1s, then 2s
    int index = 0;
    for (int color = 0; color < 3; ++color) {
        for (int i = 0; i < counts[color]; ++i) {
            nums[index++] = color;
        }
    }
}
#include <cassert>
#include <vector>

// Assume the solution function is declared above or in the same translation unit.
// Use the provided solution implementation.

int main() {
    // Test 1: mixed values
    std::vector<int> v1 = {2, 0, 1, 2, 1, 0};
    sortColorsInPlace(v1);
    assert(v1 == std::vector<int>({0, 0, 1, 1, 2, 2}));

    // Test 2: already sorted
    std::vector<int> v2 = {0, 0, 1, 2, 2};
    sortColorsInPlace(v2);
    assert(v2 == std::vector<int>({0, 0, 1, 2, 2}));

    // Test 3: single element
    std::vector<int> v3 = {1};
    sortColorsInPlace(v3);
    assert(v3 == std::vector<int>({1}));

    // Test 4: empty vector
    std::vector<int> v4;
    sortColorsInPlace(v4);
    assert(v4.empty());

    // Test 5: all zeros
    std::vector<int> v5 = {0, 0, 0};
    sortColorsInPlace(v5);
    assert(v5 == std::vector<int>({0, 0, 0}));

    // Test 6: all twos and ones
    std::vector<int> v6 = {2, 1, 2, 1};
    sortColorsInPlace(v6);
    assert(v6 == std::vector<int>({1, 1, 2, 2}));

    // Test 7: reverse order large case
    std::vector<int> v7 = {2, 2, 2, 1, 1, 0, 0};
    sortColorsInPlace(v7);
    assert(v7 == std::vector<int>({0, 0, 1, 1, 2, 2, 2}));

    // Test 8: values in alternating pattern
    std::vector<int> v8 = {0, 1, 2, 0, 1, 2, 0, 1, 2};
    sortColorsInPlace(v8);
    assert(v8 == std::vector<int>({0, 0, 0, 1, 1, 1, 2, 2, 2}));

    return 0;
}
