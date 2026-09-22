// Write a C++ function that takes two vectors of vectors, where each inner vector represents a key-value pair with a non-negative integer key (between 0 and 1000 inclusive) and an integer value, and returns a merged vector of key-value pairs. The result must contain all keys that appear in either input, with their values summed if the key appears in both. The output must be sorted in ascending order of keys, and only include keys whose total sum is non-zero.

// The solution uses a fixed-size array to accumulate sums because the key range is small and known (0–1000). First, iterate over every pair in `nums1` and add each value to the corresponding index in the accumulator array. Repeat for `nums2`, summing values for keys that appear in both inputs. Then iterate over the array from index 0 to 1000, and for any index with a non-zero sum, add the pair `{index, sum}` to the result vector. This automatically produces sorted output and handles duplicate keys, missing keys, and zero-sum keys by omitting them. Edge cases include empty inputs (returns empty result), keys with values that cancel out (omitted), and keys with negative values (handled correctly since the sum can be non-zero). Time complexity is O(n + m + 1001) where n and m are the sizes of the input vectors, effectively linear. Space complexity is O(1001) for the accumulator array plus O(k) for the output, where k is the number of distinct keys with non-zero sums.

#include <vector>

// Merge two key-value pair vectors, summing values for matching keys.
// Keys are non-negative integers in [0, 1000]. Returns pairs sorted by key,
// excluding keys whose total sum is zero.
std::vector<std::vector<int>> mergeArrays(
    const std::vector<std::vector<int>>& nums1,
    const std::vector<std::vector<int>>& nums2
) {
    constexpr int MAX_KEY = 1000;
    std::vector<int> sums(MAX_KEY + 1, 0);

    for (const auto& pair : nums1) {
        sums[pair[0]] += pair[1];
    }
    for (const auto& pair : nums2) {
        sums[pair[0]] += pair[1];
    }

    std::vector<std::vector<int>> result;
    result.reserve(MAX_KEY + 1);
    for (int key = 0; key <= MAX_KEY; ++key) {
        if (sums[key] != 0) {
            result.push_back({key, sums[key]});
        }
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    // Both inputs empty
    assert(mergeArrays({}, {}).empty());

    // Single input, keys preserved
    std::vector<std::vector<int>> n1 = {{1, 5}, {2, 3}};
    std::vector<std::vector<int>> n2 = {};
    assert(mergeArrays(n1, n2) == std::vector<std::vector<int>>({{1, 5}, {2, 3}}));

    // Disjoint keys, sorted output
    n1 = {{0, 1}, {3, 2}};
    n2 = {{2, 4}, {1000, -1}};
    std::vector<std::vector<int>> expected = {{0, 1}, {2, 4}, {3, 2}, {1000, -1}};
    assert(mergeArrays(n1, n2) == expected);

    // Overlapping keys sum values
    n1 = {{1, 10}, {2, -5}, {3, 0}};
    n2 = {{1, -3}, {2, 8}, {4, 7}};
    expected = {{1, 7}, {2, 3}, {4, 7}};
    assert(mergeArrays(n1, n2) == expected);

    // Values cancel to zero, key omitted
    n1 = {{5, 9}, {6, 1}};
    n2 = {{5, -9}, {6, 1}};
    expected = {{6, 2}};
    assert(mergeArrays(n1, n2) == expected);

    // Duplicate keys within a single input
    n1 = {{1, 2}, {1, 3}, {2, 4}};
    n2 = {{1, -1}};
    expected = {{1, 4}, {2, 4}};
    assert(mergeArrays(n1, n2) == expected);

    // Keys at boundary 0 and 1000
    n1 = {{0, -2}, {1000, 5}};
    n2 = {{0, 2}, {1000, -5}};
    assert(mergeArrays(n1, n2).empty());

    // Mixed negative and positive values
    n1 = {{10, -4}, {11, 3}};
    n2 = {{10, 6}, {11, -3}, {12, 0}};
    expected = {{10, 2}};
    assert(mergeArrays(n1, n2) == expected);

    // Large values (within int range)
    n1 = {{0, 2000000000}, {1, -1000000000}};
    n2 = {{0, -1999999999}, {1, 1000000000}};
    expected = {{0, 1}, {1, 0}}; // Note: second pair has sum zero, omitted
    // Correct expected: only key 0 with sum 1
    expected = {{0, 1}};
    assert(mergeArrays(n1, n2) == expected);

    return 0;
}
