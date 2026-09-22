// Write a C++ function that takes a vector of numeric strings and a vector of queries, where each query specifies a `trim` length and a `k` value. For each query, you must trim each number to its last `trim` digits (by removing the leading characters, keeping only the suffix of that length), sort the resulting trimmed strings in ascending lexicographical order (which corresponds to numeric order since all trimmed strings will have equal length and contain only digits), and return the original index of the `k`-th smallest trimmed string (1-indexed, so k=1 means the smallest). All input strings consist only of digits and have length equal to or greater than the given trim length. If two trimmed strings are identical because the same suffix appears in different original numbers, they should be ordered by their original index (the `pair` containing the string and index handles this automatically in `std::sort` tie-breaking). Return a vector of integers containing the answer for each query in order. The function signature must be `std::vector<int> smallestTrimmedNumbers(const std::vector<std::string>& nums, const std::vector<std::vector<int>>& queries)`.

// The solution processes each query independently. For a given query `[k, trim]`, we need to find the original index of the `k`-th smallest number when considering only the last `trim` digits. The main algorithm:
// 1. For each query, create a vector of pairs where each pair consists of the trimmed suffix (obtained via `substr(nums[i].size() - trim)`) and the original index `i`.
// 2. Sort this vector of pairs. Sorting lexicographically on the string first is correct because all trimmed strings have the same length (equal to `trim`) and contain only digits, so lexicographic order equals numeric order. In case of equal strings, `std::pair` comparison falls back to the second element (the index), which gives a deterministic order based on original position.
// 3. The answer is the `index` from the `(k-1)`-th element (since k is 1-indexed).
// 4. Move to the next query.
//
// Edge cases: `trim` is guaranteed to be at most the length of each string, so `substr` never fails. `k` is guaranteed to be between 1 and `nums.size()` inclusive. Duplicate trimmed strings are handled by index-based tie-breaking. If there is only one element, the answer is just index 0. Time complexity: For `q` queries and `n` numbers each of average length `L`, each query does O(n) substring operations each O(trim) but since trim ≤ L, that's O(n·L) per query, plus O(n log n) for sorting, so total O(q·(n·L + n log n)). Space complexity per query is O(n) for the pairs vector.

#include <string>
#include <vector>
#include <algorithm>
#include <utility>

// For each query [k, trim], return the original index of the k-th smallest
// number when considering only the last 'trim' digits of each string.
std::vector<int> smallestTrimmedNumbers(const std::vector<std::string>& nums,
                                        const std::vector<std::vector<int>>& queries) {
    std::vector<int> results;
    results.reserve(queries.size());

    for (const auto& query : queries) {
        int k = query[0];      // 1-indexed rank
        int trim = query[1];   // number of trailing digits to consider

        // Build pairs of (trimmed suffix, original index)
        std::vector<std::pair<std::string, int>> pairs;
        pairs.reserve(nums.size());
        for (size_t i = 0; i < nums.size(); ++i) {
            // Take the last 'trim' characters
            std::string trimmed = nums[i].substr(nums[i].size() - trim);
            pairs.emplace_back(trimmed, static_cast<int>(i));
        }

        // Sort lexicographically; ties broken by index automatically
        std::sort(pairs.begin(), pairs.end());

        // k-th smallest is at index k-1 (since k is 1-indexed)
        results.push_back(pairs[k - 1].second);
    }

    return results;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    // Example 1: basic case
    std::vector<std::string> nums1 = {"102", "473", "251", "814"};
    std::vector<std::vector<int>> queries1 = {{1, 1}, {2, 1}, {3, 1}, {4, 1}};
    std::vector<int> result1 = smallestTrimmedNumbers(nums1, queries1);
    // Last digit: nums1[3]="814"->'4' (index 3), nums1[0]="102"->'2' (0),
    // nums1[2]="251"->'1' (2), nums1[1]="473"->'3' (1). Sorted: 1(idx2),2(idx0),3(idx1),4(idx3)
    std::vector<int> expected1 = {2, 0, 1, 3};
    assert(result1 == expected1);

    // Example 2: trim length equals whole string length
    std::vector<std::string> nums2 = {"24", "37", "15"};
    std::vector<std::vector<int>> queries2 = {{1, 2}, {3, 2}};
    // Full strings: "15"(idx2), "24"(idx0), "37"(idx1)
    std::vector<int> result2 = smallestTrimmedNumbers(nums2, queries2);
    assert((result2 == std::vector<int>{2, 1}));

    // Example 3: duplicate trimmed suffixes, tie broken by index
    std::vector<std::string> nums3 = {"111", "211", "311"};
    std::vector<std::vector<int>> queries3 = {{1, 1}, {2, 1}, {3, 1}};
    // All trimmed to '1', sorted by index: 0, 1, 2
    std::vector<int> result3 = smallestTrimmedNumbers(nums3, queries3);
    assert((result3 == std::vector<int>{0, 1, 2}));

    // Example 4: single element
    std::vector<std::string> nums4 = {"98765"};
    std::vector<std::vector<int>> queries4 = {{1, 3}};
    std::vector<int> result4 = smallestTrimmedNumbers(nums4, queries4);
    assert((result4 == std::vector<int>{0}));

    // Example 5: multiple queries with different trim lengths
    std::vector<std::string> nums5 = {"123456", "654321"};
    std::vector<std::vector<int>> queries5 = {{1, 1}, {2, 1}, {1, 3}, {2, 3}};
    // trim=1: last digits '6'(idx0), '1'(idx1) -> sorted: idx1, idx0
    // trim=3: "456"(idx0), "321"(idx1) -> sorted: idx1, idx0
    std::vector<int> result5 = smallestTrimmedNumbers(nums5, queries5);
    assert((result5 == std::vector<int>{1, 0, 1, 0}));

    return 0;
}
