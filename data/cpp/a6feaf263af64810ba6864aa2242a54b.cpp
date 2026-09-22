/*
Given an array of positive integers, a "split" is defined as dividing the array into contiguous segments where every segment after the first has the same sum as the first segment. For each possible first segment (starting at index 0 and ending at some index i), if the remaining array can be partitioned into contiguous segments each summing exactly to that first segment's sum, then the cost is the number of cuts needed (i.e., the number of segments minus 1). Write a C++ function `int minimumCuts(const std::vector<int>& v)` that returns the minimum number of cuts needed to partition the entire array into contiguous segments where all segments after the first have the same sum as the first segment. Every valid partition must include at least one segment (the whole array is always valid with 0 cuts). Return the cut count for the best (minimum) valid partition, or if no valid partition exists besides the whole array (0 cuts), return 0.
*/

#include <vector>
#include <algorithm>

// Returns the minimum number of cuts to partition the array into contiguous
// segments where all segments after the first have the same sum as the first segment.
// The whole array as one segment is always valid (0 cuts).
int minimumCuts(const std::vector<int>& v) {
    int n = static_cast<int>(v.size());
    if (n <= 1) return 0;

    // Compute total sum to know maximum possible prefix sums.
    int totalSum = 0;
    for (int x : v) totalSum += x;

    int best = n - 1; // worst case: cut after every element, but that's not valid usually.

    // Try each possible first segment ending at index i (inclusive).
    for (int i = 0; i < n; ++i) {
        int target = 0;
        for (int k = 0; k <= i; ++k) target += v[k];
        if (target > totalSum) continue; // impossible

        int running = 0;
        int segments = 1; // first segment already counted
        bool valid = true;
        for (int j = i + 1; j < n; ++j) {
            running += v[j];
            if (running == target) {
                segments++;
                running = 0;
            } else if (running > target) {
                valid = false;
                break;
            }
        }
        if (valid && running == 0) {
            best = std::min(best, segments - 1);
        }
        // Optimization: if target is 0? not possible because all positive.
    }
    // The whole array as one segment always gives 0 cuts, but that's covered
    // when i = n-1? Actually i must be at least 0, and if i = n-1 then j loop doesn't run and running=0, segments=1, cuts=0.
    return best;
}

#include <cassert>
#include <vector>

// Forward declaration
int minCutsForEqualSegments(const std::vector<int>& arr);

int main() {
    // [1,2,3,3] -> possible: [1,2] sum=3, then [3] and [3] -> segments=3 cuts=2; also [1] impossible because remaining sum doesn't match; so answer 2.
    assert(minCutsForEqualSegments({1,2,3,3}) == 2);
    // [2,2,2] -> [2] then [2] and [2] -> cuts=2; [2,2] sum=4 impossible; answer 2.
    assert(minCutsForEqualSegments({2,2,2}) == 2);
    // [1,1,1,1] -> [1] then three more -> cuts=3; [1,1] then [1,1] -> cuts=1; answer 1.
    assert(minCutsForEqualSegments({1,1,1,1}) == 1);
    // [1,2,3] -> possible [1]? remaining sum=5 not multiple of 1? Actually 5%1=0, but running through 2,3 gives running>1, invalid. [1,2] sum=3, remaining=3, but no remaining elements? Actually i=1, j=2, arr[2]=3 equals target -> segments=2, cuts=1, valid. So answer 1.
    assert(minCutsForEqualSegments({1,2,3}) == 1);
    // [1,2,4] -> no valid split because sums don't work out; whole array alone not allowed; -1.
    assert(minCutsForEqualSegments({1,2,4}) == -1);
    // [3,3,3,3] -> [3] then three more -> cuts=3; [3,3] then [3,3] -> cuts=1; answer 1.
    assert(minCutsForEqualSegments({3,3,3,3}) == 1);
    // Single element -> -1.
    assert(minCutsForEqualSegments({5}) == -1);
    // [1,1,2] -> [1]? remaining sum=3, not multiple of 1? 3%1=0, but running through 1,2 gives running>1 after 1+2=3 >1, invalid. [1,1] sum=2, remaining=2, j=2 arr[2]=2 equals target -> segments=2 cuts=1. Answer 1.
    assert(minCutsForEqualSegments({1,1,2}) == 1);
    // [1,2,3,1,2,3] -> [1]? no; [1,2] sum=3, then [3] and [1,2] and [3] -> segments=4 cuts=3; actually check: [1,2] then [3] then [1,2] then [3] -> valid, cuts=3; [1,2,3] sum=6, then [1,2,3] -> cuts=1. Answer 1.
    assert(minCutsForEqualSegments({1,2,3,1,2,3}) == 1);
}

// The solution iterates over each possible prefix length (from 0 to n-1) as the first segment. For each prefix, we compute its sum `target`. Then we scan the rest of the array, accumulating a running sum. When the running sum equals `target`, we reset it to 0 (a cut is made). If it exceeds `target`, this prefix is invalid. At the end, if the running sum is 0, the partition is valid, and the number of cuts is the number of times we reset plus the initial cut (between the first segment and the rest, but we count cuts, not segments). Actually, the number of cuts is `number_of_segments - 1`. For a prefix of length i, the number of segments is at least 1 (the first segment), and we add 1 for each reset. The total segments = 1 + (number of resets). So cuts = number of resets. However, the original code adds `cn` (which counts the number of elements not part of a completed segment? Actually in the snippet, `cn` increments for every element that is not a segment end, but it's messy). Let's design cleanly: For each prefix, we count how many additional segments we successfully form, say `segments` (starting at 1 for the first segment). For each time we reset `cur` to 0, we increment `segments`. At the end if remainder is 0, total cuts = segments - 1. Take the minimum over all valid prefixes. Since the whole array is always valid (prefix of length n, which we don't consider because we need at least one additional segment? Actually we can consider prefix of length n as the whole array, that gives 0 cuts). But we also want to consider prefixes that allow multiple segments. Edge cases: all elements positive, so sums grow; if prefix sum is greater than total sum (impossible). A prefix of length 0 is not allowed (must have at least one element). Complexity: O(n^2) time in worst case, O(1) extra space.
