/*
Given a list of up to 10,000,000 non-negative integers where each integer is between 1 and 10,000 (inclusive), write a C++ function `std::vector<int> countSortValues(const std::vector<int>& input)` that returns a new vector containing all input values sorted in ascending order. The function must use counting sort to achieve linear time complexity relative to the number of input elements, leveraging the fixed and small value range. The input vector may contain duplicates, and the returned vector must preserve duplicates in sorted order. The function should not modify the input vector.
*/

#include <vector>
#include <map> // not needed, but for completeness
#include <cstddef>

// Count sort for values in [1, 10000]. Returns sorted vector.
std::vector<int> countSortValues(const std::vector<int>& input) {
    constexpr int MAX_VALUE = 10000;
    // Using a fixed-size array for counting frequency.
    int count[MAX_VALUE + 1] = {0};
    
    // Count occurrences of each value.
    for (int value : input) {
        // Assuming value is between 1 and 10000 inclusive.
        count[value]++;
    }
    
    // Build the sorted output.
    std::vector<int> sorted;
    sorted.reserve(input.size());
    for (int i = 1; i <= MAX_VALUE; ++i) {
        for (int j = 0; j < count[i]; ++j) {
            sorted.push_back(i);
        }
    }
    return sorted;
}

#include <cassert>
#include <vector>

// Function declaration (from solution)
std::vector<int> countSortValues(const std::vector<int>& input);

int main() {
    // Test 1: basic sorted order
    std::vector<int> input1 = {5, 2, 8, 1, 3};
    std::vector<int> result1 = countSortValues(input1);
    std::vector<int> expected1 = {1, 2, 3, 5, 8};
    assert(result1 == expected1);

    // Test 2: duplicates
    std::vector<int> input2 = {10, 10, 10, 1, 1};
    std::vector<int> result2 = countSortValues(input2);
    std::vector<int> expected2 = {1, 1, 10, 10, 10};
    assert(result2 == expected2);

    // Test 3: single element
    std::vector<int> input3 = {7};
    std::vector<int> result3 = countSortValues(input3);
    std::vector<int> expected3 = {7};
    assert(result3 == expected3);

    // Test 4: already sorted
    std::vector<int> input4 = {1, 2, 3, 4, 5};
    std::vector<int> result4 = countSortValues(input4);
    std::vector<int> expected4 = {1, 2, 3, 4, 5};
    assert(result4 == expected4);

    // Test 5: all max values
    std::vector<int> input5 = {10000, 10000, 10000};
    std::vector<int> result5 = countSortValues(input5);
    std::vector<int> expected5 = {10000, 10000, 10000};
    assert(result5 == expected5);

    // Test 6: empty input
    std::vector<int> input6;
    std::vector<int> result6 = countSortValues(input6);
    assert(result6.empty());

    // Test 7: values at boundaries
    std::vector<int> input7 = {1, 10000, 5000, 1, 10000};
    std::vector<int> result7 = countSortValues(input7);
    std::vector<int> expected7 = {1, 1, 5000, 10000, 10000};
    assert(result7 == expected7);

    // Test 8: many elements with mixed values
    std::vector<int> input8 = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    std::vector<int> result8 = countSortValues(input8);
    std::vector<int> expected8 = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(result8 == expected8);

    // Test 9: original input unchanged
    std::vector<int> input9 = {3, 1, 2};
    std::vector<int> copy9 = input9;
    countSortValues(input9);
    assert(input9 == copy9);

    // Test 10: very large n (1e6) to ensure linear performance
    std::vector<int> input10(1000000);
    for (int i = 0; i < 1000000; ++i) {
        input10[i] = ((i * 37) % 10000) + 1; // deterministic pseudo-random in [1,10000]
    }
    std::vector<int> result10 = countSortValues(input10);
    // Verify sorted order
    for (size_t i = 1; i < result10.size(); ++i) {
        assert(result10[i - 1] <= result10[i]);
    }
    // Verify same size
    assert(result10.size() == input10.size());

    return 0;
}

// Counting sort is ideal because the value range (1 to 10,000) is known and fixed, and the number of elements can be very large (up to 10^7). The algorithm first determines the maximum possible value (10,000) to allocate a count array of size `MAX_VALUE + 1` (for indices 0..10000, though index 0 will remain zero since minimum value is 1). Then it makes a single pass over the input, incrementing the count at each value's index. Finally, it reconstructs the sorted output by iterating over the count array from 1 to 10000, appending each index `i` exactly `count[i]` times. Edge cases: empty input returns an empty vector; input values are guaranteed within [1, 10000], but if not, using an array of size 10001 is safe for values up to 10000; duplicates are handled naturally by counting. Time complexity is O(N + M) where N is the number of elements and M = 10,000 (the value range), which effectively is O(N) since M is constant. Space complexity is O(M) for the count array plus O(N) for the output vector (required by the return type), but auxiliary extra space beyond output is O(1) if we consider M as constant.
