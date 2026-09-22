/*
Write a C++ function that takes a non-empty vector of integers containing only the values 0, 1, and 2 (possibly with duplicates) and returns a new vector with all the elements sorted in non-decreasing order (i.e., all 0s first, then all 1s, then all 2s). The function must preserve the relative order within each group, but since all identical values are indistinguishable, the output is simply all zeros, then all ones, then all twos. The input vector may be modified? No—the function must operate on a `const std::vector<int>&` and return a new vector. The order of the input values does not matter; only the counts of 0s, 1s, and 2s matter. The vector may be empty. Ensure the solution is efficient and does not use `std::sort`.
*/

#include <vector>

// Given a vector containing only 0s, 1s, and 2s, return a new vector sorted in non-decreasing order.
// Time: O(n), Space: O(n) for the returned vector.
std::vector<int> sortZeroOneTwo(const std::vector<int>& input) {
    int count0 = 0, count1 = 0, count2 = 0;
    for (int value : input) {
        if (value == 0) ++count0;
        else if (value == 1) ++count1;
        else ++count2;
    }

    std::vector<int> result;
    result.reserve(input.size());

    for (int i = 0; i < count0; ++i) result.push_back(0);
    for (int i = 0; i < count1; ++i) result.push_back(1);
    for (int i = 0; i < count2; ++i) result.push_back(2);

    return result;
}

#include <cassert>
#include <vector>

// Assume the solution function is declared above.
int main() {
    // Basic case
    std::vector<int> v1 = {0, 1, 2, 0, 1, 2};
    assert(sortZeroOneTwo(v1) == std::vector<int>({0, 0, 1, 1, 2, 2}));

    // All zeros
    std::vector<int> v2 = {0, 0, 0};
    assert(sortZeroOneTwo(v2) == std::vector<int>({0, 0, 0}));

    // All twos
    std::vector<int> v3 = {2, 2, 2};
    assert(sortZeroOneTwo(v3) == std::vector<int>({2, 2, 2}));

    // Mixed with duplicates
    std::vector<int> v4 = {2, 0, 2, 1, 1, 0, 2};
    assert(sortZeroOneTwo(v4) == std::vector<int>({0, 0, 1, 1, 2, 2, 2}));

    // Single element
    std::vector<int> v5 = {1};
    assert(sortZeroOneTwo(v5) == std::vector<int>({1}));

    // Empty vector
    std::vector<int> v6;
    assert(sortZeroOneTwo(v6).empty());

    // Already sorted
    std::vector<int> v7 = {0, 0, 1, 2};
    assert(sortZeroOneTwo(v7) == std::vector<int>({0, 0, 1, 2}));

    // Large sequence (but only three values)
    std::vector<int> v8 = {1, 2, 0, 2, 1, 0, 0, 2, 1};
    assert(sortZeroOneTwo(v8) == std::vector<int>({0, 0, 0, 1, 1, 1, 2, 2, 2}));

    return 0;
}

// The solution approach: Count the occurrences of each value (0, 1, 2) by iterating through the input vector once. Then construct the output vector by first appending `count0` zeros, then `count1` ones, then `count2` twos. This is a classic counting sort for a small fixed range of values. Edge cases: an empty input returns an empty vector; inputs with only one distinct value still produce the correct sorted output. Complexity: Time is O(n) where n is the size of the input (one pass to count, then one pass to fill the output). Space is O(n) for the output vector (plus O(1) auxiliary for the counters). The function should be `const`‑correct by taking `const std::vector<int>&` and returning `std::vector<int>` by value.
