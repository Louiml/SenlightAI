// Write a C++ function `int countDistinctDuplicates(const std::vector<int>& numbers)` that, given a non-empty vector of integers, returns the total number of duplicate occurrences beyond the first occurrence of each distinct value. In other words, if a value appears `k` times, it contributes `k - 1` to the result. For example, `{1,2,2,3,3,3}` should return `3` because `2` appears once extra and `3` appears twice extra. The input may contain negative numbers, zeros, and large values. The function must not modify the input vector and should handle an empty vector by returning `0` (though the problem guarantees non-empty in practice).
#include <cassert>
#include <vector>

int main() {
    // Function declaration from the solution (assumed already included)
    int countDistinctDuplicates(const std::vector<int>&);
    
    assert(countDistinctDuplicates({1, 2, 3}) == 0);
    assert(countDistinctDuplicates({5, 5, 5, 5}) == 3);
    assert(countDistinctDuplicates({1, 1, 2, 2, 3, 3, 3}) == 4);
    assert(countDistinctDuplicates({-1, -1, 0, 0, 0, 2}) == 3);
    assert(countDistinctDuplicates({7}) == 0);
    assert(countDistinctDuplicates({}) == 0);
    assert(countDistinctDuplicates({100, 100, -100, -100, 0}) == 2);
    assert(countDistinctDuplicates({42, 42, 42, 42, 42, 42}) == 5);
    assert(countDistinctDuplicates({1, 2, 2, 3, 3, 3, 4, 4, 4, 4}) == 6);
    assert(countDistinctDuplicates({0, -0, 0}) == 2); // 0 and -0 are the same integer
}
#include <vector>
#include <unordered_map>

// Returns the total number of duplicate occurrences beyond the first occurrence
// for each distinct value in the input vector. For an empty vector, returns 0.
int countDistinctDuplicates(const std::vector<int>& numbers) {
    std::unordered_map<int, int> frequency;
    for (const int value : numbers) {
        ++frequency[value];
    }
    
    int duplicates = 0;
    for (const auto& entry : frequency) {
        const int count = entry.second;
        if (count > 1) {
            duplicates += count - 1;
        }
    }
    return duplicates;
}
// The solution uses a hash map (or unordered_map) to count frequencies of each integer in one pass. For each value, increment its count. After building the frequency map, iterate over it and for each distinct value with frequency `f`, add `f - 1` to the result. This counts every duplicate occurrence beyond the first. Edge cases: all values distinct → result is 0; all same value → result is size-1; empty vector → return 0. Time complexity is O(n) on average (hash map operations) with n being the number of elements. Space complexity is O(u) where u is the number of distinct values. Using `std::map` would yield O(n log u) time and O(u) space, but unordered_map is preferable for average O(n).
