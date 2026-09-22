/*
Write a C++ function `int minimumTotalOperations(const std::vector<std::string>& strings)` that, given a non-empty vector of strings, determines if all strings are "compatible" under the following operation rule: you may change a single character in a string to any other character, but every operation costs 1 unit, and you can apply operations to any of the strings any number of times. The goal is to make all strings have exactly the same sequence of distinct consecutive character groups (for example, "aabbbc" groups to "abc", and "xyyzz" groups to "xyz"). If the group sequences are not identical at all, the function should return -1. If they are identical, return the minimum total cost (sum over all strings) to make every string have exactly the same counts of each character in each group as some common target. The common target is chosen optimally to minimize total cost. The input strings consist only of lowercase English letters, and each string length is between 1 and 100. The vector size is between 1 and 100.
*/
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

// Returns minimum total operations to make all strings have identical group counts,
// or -1 if the character group sequences are not identical across all strings.
int minimumTotalOperations(const std::vector<std::string>& strings) {
    int n = static_cast<int>(strings.size());
    if (n == 0) return -1;

    // Store group characters and counts for each string
    std::vector<std::string> groupChars(n);
    std::vector<std::vector<int>> groupCounts(n);

    for (int i = 0; i < n; ++i) {
        const std::string& s = strings[i];
        if (s.empty()) return -1; // empty string not allowed by problem, but handle safety
        std::string chars;
        std::vector<int> counts;
        chars.push_back(s[0]);
        counts.push_back(1);
        for (size_t j = 1; j < s.size(); ++j) {
            if (s[j] == s[j-1]) {
                counts.back()++;
            } else {
                chars.push_back(s[j]);
                counts.push_back(1);
            }
        }
        groupChars[i] = chars;
        groupCounts[i] = counts;
    }

    // Check all group character sequences are identical
    for (int i = 1; i < n; ++i) {
        if (groupChars[i] != groupChars[0]) {
            return -1;
        }
    }

    int m = static_cast<int>(groupChars[0].size());
    // For each group index, collect counts across all strings
    std::vector<std::vector<int>> countsPerGroup(m);
    for (int g = 0; g < m; ++g) {
        for (int i = 0; i < n; ++i) {
            countsPerGroup[g].push_back(groupCounts[i][g]);
        }
    }

    int totalCost = 0;
    for (int g = 0; g < m; ++g) {
        std::vector<int>& counts = countsPerGroup[g];
        std::sort(counts.begin(), counts.end());
        int median = counts[n / 2]; // lower median works for both odd/even
        for (int c : counts) {
            totalCost += std::abs(c - median);
        }
    }

    return totalCost;
}
#include <cassert>
#include <string>
#include <vector>

// forward declaration of the solution function
int minimumTotalOperations(const std::vector<std::string>& strings);

int main() {
    // Identical strings: cost 0
    assert(minimumTotalOperations({"abc", "abc"}) == 0);

    // All strings already have same groups and counts
    assert(minimumTotalOperations({"aa", "aa"}) == 0);

    // Incompatible group sequences
    assert(minimumTotalOperations({"abc", "abd"}) == -1);
    assert(minimumTotalOperations({"a", "b"}) == -1);
    assert(minimumTotalOperations({"ab", "ba"}) == -1);

    // Single string: cost 0
    assert(minimumTotalOperations({"aabbcc"}) == 0);

    // Simple change: one string has extra count in a group
    // "aa" (group a count 2) and "a" (count 1) -> median 1 or 2? sorted {1,2}, lower median =1, cost |1-1|+|2-1|=1
    assert(minimumTotalOperations({"aa", "a"}) == 1);

    // More complex case
    // "aab" -> groups a:2, b:1 ; "ab" -> a:1,b:1 ; "aaab" -> a:3,b:1
    // Counts per group: a: {2,1,3} sorted {1,2,3} median=2 cost |2-2|+|1-2|+|3-2|=2 ; b: {1,1,1} cost 0 -> total 2
    assert(minimumTotalOperations({"aab", "ab", "aaab"}) == 2);

    // Even number of strings with different counts
    // "a","aa","aaa","aaaa" -> counts a: {1,2,3,4} sorted, median lower =2 (n/2=2) cost |1-2|+|2-2|+|3-2|+|4-2|=1+0+1+2=4
    assert(minimumTotalOperations({"a", "aa", "aaa", "aaaa"}) == 4);

    // All characters same but different counts, multiple groups? just one group
    // "b", "bb", "bbbb" -> {1,2,4} median=2 cost 1+0+2=3
    assert(minimumTotalOperations({"b", "bb", "bbbb"}) == 3);

    // Empty vector not allowed, but if happens, function returns -1
    assert(minimumTotalOperations({}) == -1);
}
// First, transform each string into its "group summary": a sequence of unique characters (preserving order) and a parallel sequence of counts for each group (run-length encoding). For example, "aabbbc" yields characters "abc" and counts {2,3,1}. If any two input strings have different character sequences (different lengths or different characters in order), they are incompatible, and the function returns -1. Otherwise, for each group index, we have a list of counts (one per input string). The optimal common count for that group is the median of these counts, because the sum of absolute deviations from a value is minimized at the median. Compute the total cost as the sum over all groups and all strings of `abs(count - median)`. For even number of strings, both the lower and upper median give the same minimal sum, so we can safely use the lower median (index `n/2` after sorting) or upper median; the example in the snippet uses `n/2` after sorting (works for both odd and even since both medians give identical sums). Edge cases: all strings already equal (cost 0), single string (cost 0), and when incompatible groups are found early. Time complexity: for `n` strings of average length `L`, run-length encoding is `O(L)` per string, and sorting each group's counts is `O(n log n)` per group, with at most `L` groups, so total `O(nL + L * n log n)`, which simplifies to `O(nL log n)` worst-case. Space complexity is `O(nL)` for storing counts, but we can process group by group if we store all summaries.
