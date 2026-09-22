Write a C++ function `groupByFrequency` that takes a vector of integers `nums` where each value is between 1 and the size of the vector (inclusive), and returns a 2D vector of integers. The returned matrix must be constructed so that no row contains duplicate values, and each original value `x` appearing `f` times must be placed in exactly `f` different rows (i.e., distributed one copy per row across the first `f` rows). The rows must be filled in order such that all elements of the first row are determined first, then the second, etc., and within each row, values appear in increasing order. For example, for input `[1, 3, 4, 1, 2, 3, 1]`, the result should be `[[1, 2, 3, 4], [1, 3], [1]]` (because 1 appears three times, 2 once, 3 twice, 4 once). The function must be `const`-correct for the input and use only the necessary standard headers.

// The algorithm counts the frequency of each value using a frequency array of size `n+1` (since values are between 1 and `n`). For each value `x` from 1 to `n`, we need to place `cnt[x]` copies into rows 0 through `cnt[x]-1`. To achieve this, we iterate over values in increasing order and for each value, we ensure that the matrix has enough rows by appending empty rows as needed. For each copy `j` (0-indexed), we push `x` into row `j`. This automatically groups values by frequency and ensures rows are filled in order: the first row gets all values that appear at least once, the second row gets values that appear at least twice, and so on. Because we iterate `x` in increasing order, each row ends up sorted ascending. The key edge case is when `cnt[x]` is zero — we skip it. Also, if a value appears many times, we may need to create many rows, and the number of rows is exactly the maximum frequency (e.g., if all values appear once, only one row exists). Time complexity is O(n + total frequency) = O(n) because each element of `nums` contributes to exactly one push into the answer; space complexity is O(n) for the count array plus the answer matrix which holds exactly `nums.size()` elements in total.

#include <vector>

// Group numbers by their frequency into rows without duplicates.
// Each value x appears cnt[x] times, placed in the first cnt[x] rows.
std::vector<std::vector<int>> groupByFrequency(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    std::vector<int> count(n + 1, 0);
    for (int x : nums) {
        ++count[x];
    }

    std::vector<std::vector<int>> result;
    // Iterate values in increasing order to ensure rows are sorted.
    for (int x = 1; x <= n; ++x) {
        int freq = count[x];
        if (freq == 0) continue;
        // Ensure enough rows exist for the highest frequency seen so far.
        while (static_cast<int>(result.size()) < freq) {
            result.push_back({});
        }
        for (int row = 0; row < freq; ++row) {
            result[row].push_back(x);
        }
    }
    return result;
}

#include <cassert>
#include <vector>

// Assume groupByFrequency is defined above.

int main() {
    // Single element.
    assert(groupByFrequency({1}) == std::vector<std::vector<int>>{{1}});
    // All distinct values -> one row sorted.
    assert(groupByFrequency({2, 1, 3}) == std::vector<std::vector<int>>{{1, 2, 3}});
    // Example from description.
    assert(groupByFrequency({1, 3, 4, 1, 2, 3, 1}) == std::vector<std::vector<int>>{{1, 2, 3, 4}, {1, 3}, {1}});
    // All same value -> each row has one copy.
    assert(groupByFrequency({5, 5, 5}) == std::vector<std::vector<int>>{{5}, {5}, {5}});
    // Mixed frequencies with max 3.
    assert(groupByFrequency({2, 2, 1, 2, 3, 3, 4}) == std::vector<std::vector<int>>{{1, 2, 3, 4}, {2, 3}, {2}});
    // Large frequency of maximum value.
    assert(groupByFrequency({3, 1, 3, 1, 3}) == std::vector<std::vector<int>>{{1, 3}, {1, 3}, {3}});
    // Value 1 appears twice, value 2 once.
    assert(groupByFrequency({1, 2, 1}) == std::vector<std::vector<int>>{{1, 2}, {1}});
    // All numbers 1..n with n=5, each once.
    assert(groupByFrequency({3, 1, 4, 2, 5}) == std::vector<std::vector<int>>{{1, 2, 3, 4, 5}});
    // n=4, value 4 appears 4 times.
    assert(groupByFrequency({4, 4, 4, 4}) == std::vector<std::vector<int>>{{4}, {4}, {4}, {4}});
    // Random order with duplicates.
    assert(groupByFrequency({2, 2, 1, 1, 1, 3}) == std::vector<std::vector<int>>{{1, 2, 3}, {1, 2}, {1}});
    return 0;
}
