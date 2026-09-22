Write a C++ function that, given a vector of integers `nums` and an integer `k`, returns a pair of integers representing the longest contiguous subsequence (by value, not by index) where every distinct integer in that subsequence appears at least `k` times in the original array. The subsequence must consist of consecutive integer values (e.g., 4,5,6,7) and the pair should be the smallest starting value and the largest ending value of that longest sequence. If no such sequence exists (i.e., no single integer appears at least `k` times), return `{-1, -1}`. The function should handle negative numbers, duplicate values, large `k` values, and empty input. For ties in length, choose the sequence with the smaller starting value.
The solution first counts the frequency of each distinct integer using an unordered_map (or map if ordering is desired). Then collect all distinct values into a sorted container (e.g., set or sort a vector). Iterate through the sorted distinct values, maintaining a running count of how many consecutive values (by integer value) each have frequency >= k. When a value meets the threshold, extend the current run; if the current value breaks the consecutive pattern (i.e., previous value + 1 != current value), finalize the previous run and start a new one. If a value does not meet the threshold, finalize the current run and reset. Track the best run: longest length, and for ties, the smallest starting value. Edge cases: empty input (return {-1,-1}), all values meet threshold (the final run must be finalized after the loop), no value meets threshold (return {-1,-1}), and when the run ends because the set ends (finalize after loop). Complexity: O(n log n) due to sorting distinct values (or O(n) if using unordered_set and then sorting the distinct values). Space: O(n) for the frequency map and the set/vector of distinct values.
#include <vector>
#include <map>
#include <set>
#include <utility>

// Returns the longest contiguous integer sequence where each value appears at least k times,
// as a pair {start, end}. If none exists, returns {-1, -1}.
std::pair<long long, long long> longestFrequentConsecutive(const std::vector<long long>& nums, long long k) {
    if (nums.empty()) {
        return {-1, -1};
    }

    std::map<long long, long long> freq;
    for (long long x : nums) {
        ++freq[x];
    }

    std::set<long long> distinct;
    for (const auto& entry : freq) {
        distinct.insert(entry.first);
    }

    long long bestLen = 0;
    std::pair<long long, long long> bestRange = {-1, -1};

    long long curLen = 0;
    long long curStart = 0;
    long long prev = -1;
    bool inRun = false;

    for (long long val : distinct) {
        if (freq[val] >= k) {
            if (!inRun) {
                curStart = val;
                curLen = 1;
                inRun = true;
            } else if (val == prev + 1) {
                ++curLen;
            } else {
                // consecutive break but still meets threshold -> finalize previous run and start new
                if (curLen > bestLen || (curLen == bestLen && curStart < bestRange.first)) {
                    bestLen = curLen;
                    bestRange = {curStart, prev};
                }
                curStart = val;
                curLen = 1;
            }
            prev = val;
        } else {
            // value does not meet threshold, finalize any running sequence
            if (inRun) {
                if (curLen > bestLen || (curLen == bestLen && curStart < bestRange.first)) {
                    bestLen = curLen;
                    bestRange = {curStart, prev};
                }
                inRun = false;
                curLen = 0;
            }
            prev = -1;
        }
    }

    // finalize if the last value was part of a run
    if (inRun) {
        if (curLen > bestLen || (curLen == bestLen && curStart < bestRange.first)) {
            bestLen = curLen;
            bestRange = {curStart, prev};
        }
    }

    if (bestLen == 0) {
        return {-1, -1};
    }
    return bestRange;
}
#include <cassert>
#include <vector>
#include <utility>

// Solution function is assumed to be defined above.

int main() {
    // Basic case: all appear at least once
    assert(longestFrequentConsecutive({1,2,3,4}, 1) == std::make_pair(1,4));
    // Missing frequencies, longest is 2-3
    assert(longestFrequentConsecutive({1,2,2,3,3,5}, 2) == std::make_pair(2,3));
    // No value meets k
    assert(longestFrequentConsecutive({1,2,3}, 2) == std::make_pair(-1,-1));
    // Empty input
    assert(longestFrequentConsecutive({}, 1) == std::make_pair(-1,-1));
    // Negative numbers and duplicates
    assert(longestFrequentConsecutive({-2,-2,-1,-1,0,0}, 2) == std::make_pair(-2,0));
    // All same value
    assert(longestFrequentConsecutive({5,5,5,5}, 3) == std::make_pair(5,5));
    // Tie length: choose smaller start (1-2 vs 4-5)
    assert(longestFrequentConsecutive({1,1,2,2,4,4,5,5}, 2) == std::make_pair(1,2));
    // Large k exceeding all frequencies
    assert(longestFrequentConsecutive({10,11,12}, 5) == std::make_pair(-1,-1));
    // Non-consecutive values with enough frequency
    assert(longestFrequentConsecutive({1,1,3,3,5,5}, 2) == std::make_pair(1,1));
    // Sequence at the end of sorted values
    assert(longestFrequentConsecutive({1,2,2,3,3,4,4}, 2) == std::make_pair(2,4));
}
