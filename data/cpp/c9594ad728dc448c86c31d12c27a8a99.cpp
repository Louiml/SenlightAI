/*
Given an array of positive integers, write a C++ function `long long countMinimalSets(const std::vector<long long>& a)` that returns the minimum number of non-empty sets into which the array can be partitioned, such that each set is either a singleton or consists of consecutive integers (each set's elements, when sorted, form an arithmetic progression with common difference 1, i.e., a consecutive block like `[x, x+1, ..., x+k]`). Each element of the original array must be placed into exactly one set, and sets may contain duplicate values only if those duplicates appear consecutively within that set’s range (i.e., a set cannot contain the same value more than once unless the value itself appears multiple times in the array and those duplicates are assigned to different positions in the same consecutive block, which is impossible, so duplicate values must end up in separate sets or be part of a range that has missing integers filled by other array elements). In other words, the goal is to cover all given numbers with the fewest number of "chains" where each chain is a strictly increasing sequence with step 1, and each chain can start at any number and may stop at any number. The order of elements in the original array does not matter; you may rearrange them arbitrarily when forming sets. For example, given `[1,2,2,3]`, the minimum number of sets is 2: one set is `{1,2,3}` and the other is `{2}`. Given `[1,1,1]`, the minimum is 3. Given `[5,6,7,8]`, the minimum is 1. The input vector may be empty, in which case return 0. The function should handle arbitrarily large positive integers (fits in `long long`). You must implement the function with optimal efficiency, not just brute force.
*/
#include <vector>
#include <map>
#include <algorithm>

// Returns the minimum number of sets needed to partition the numbers
// such that each set is a consecutive block (e.g., {x, x+1, ..., x+k}).
// Uses frequency counting and sums the excess of each value over its predecessor.
long long countMinimalSets(const std::vector<long long>& a) {
    if (a.empty()) return 0;

    std::map<long long, long long> freq;
    for (long long val : a) {
        ++freq[val];
    }

    long long result = 0;
    for (const auto& [key, count] : freq) {
        long long prevCount = 0;
        auto it = freq.find(key - 1);
        if (it != freq.end()) {
            prevCount = it->second;
        }
        result += std::max(0LL, count - prevCount);
    }

    return result;
}
#include <cassert>
#include <vector>

// Function under test
long long countMinimalSets(const std::vector<long long>& a);

int main() {
    // Basic examples
    assert(countMinimalSets({}) == 0);
    assert(countMinimalSets({1, 2, 3}) == 1);
    assert(countMinimalSets({1, 2, 2, 3}) == 2);
    assert(countMinimalSets({1, 1, 1}) == 3);
    assert(countMinimalSets({5, 6, 7, 8}) == 1);

    // Duplicates and gaps
    assert(countMinimalSets({1, 3, 5}) == 3);
    assert(countMinimalSets({1, 1, 2, 2, 3, 3}) == 2); // can form {1,2,3} and {1,2,3}
    assert(countMinimalSets({10, 11, 12, 11, 10}) == 2); // {10,11,12} and {10,11}

    // Unsorted input, large values
    assert(countMinimalSets({100, 101, 100, 102, 101}) == 2); // {100,101,102} and {100,101}
    assert(countMinimalSets({1, 2, 3, 3, 4, 5}) == 2); // {1,2,3,4,5} and {3}
    assert(countMinimalSets({0, 0, 0, 1}) == 3); // three zeros need 3 sets, one can absorb 1

    // More complex
    assert(countMinimalSets({1, 2, 3, 4, 5, 5, 4, 3, 2, 1}) == 2); // two full chains
    assert(countMinimalSets({1, 2, 4, 5, 7, 8}) == 3); // {1,2}, {4,5}, {7,8}

    return 0;
}
// The problem reduces to counting the minimum number of chains (consecutive sequences) needed to cover all given numbers, where each number can be used exactly once. A classic greedy approach works by sorting or, equivalently, by using a frequency map. The key observation is that whenever we have a number `x`, it can be appended to an existing chain whose current last value is `x - 1`. Therefore, the number of new chains we must start equals the number of times `x` appears more frequently than `x - 1` in the entire array. More precisely, if `cnt[x]` is the frequency of value `x`, then the number of chains that must begin at `x` is `max(0, cnt[x] - cnt[x-1])`. Summing this over all distinct values of `x` gives the total number of chains, which equals the minimum number of sets. This works because each chain is a strictly increasing sequence with step 1, and the count of chains starting at a particular value is the excess of that value's occurrences over the previous value's occurrences (since each occurrence of `x-1` can "feed" at most one occurrence of `x` in a chain). Edge cases: empty input returns 0; duplicates only force additional chains if they exceed the previous count; very large values are handled by `long long`; values that appear only once and have no predecessor start a new chain. Time complexity is O(n) to build the frequency map and then O(m) where m is number of distinct values (≤ n) to sum the excesses, giving O(n) overall. Space complexity is O(m) for the map.
