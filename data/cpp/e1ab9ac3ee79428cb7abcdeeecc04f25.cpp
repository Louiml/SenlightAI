// Write a C++ function named `countLongestIncreasingSubsequences` that takes a `std::vector<int>` as input and returns the number of longest strictly increasing subsequences (LIS) in that array. The input vector is non-empty, may contain up to 2000 integers, and each integer fits in a 32-bit signed int. The function must handle duplicate values correctly: only strictly increasing subsequences are considered (e.g., `[2,2,2]` has LIS length 1 and count 3, because each single element forms a valid LIS). The result fits in a 32-bit signed int. The function should be declared with `const` correctness where applicable (i.e., taking the vector by const reference).

// The problem asks for the count of the longest strictly increasing subsequence in an array. A direct dynamic programming approach would be O(n²), which is fine for n ≤ 2000 but we can also implement a more efficient Fenwick tree (BIT) based solution for practice. However, the reference solution uses a BIT to achieve O(n log n) time.
//
// The algorithm:
// 1. **Coordinate compression**: Sort a copy of the input, remove duplicates, and map each value to its rank (1-based index). This reduces the value range to at most n distinct values.
// 2. **Fenwick tree initialization**: We maintain a BIT where each node stores a pair `(max_length, count)`. For each prefix of the array (handled by rank), the BIT stores the maximum LIS length ending at that value (or lower) and the number of ways to achieve that length.
// 3. **Processing each number**: For the current number `v`, we query the BIT for the best `(length, count)` among all values strictly less than `v` (rank-1). If no such subsequence exists, the best length is 0, and we treat the count as 0 (so we start a new subsequence of length 1 with count 1). Then we set the new LIS length ending at `v` as `best_length + 1` and the count as `max(best_count, 1)` (if best_count is 0, use 1). We update the BIT at position `rank(v)` with this new `(length, count)`, merging with existing entries: if new length > existing length, replace; if equal, add counts.
// 4. **Answer**: After processing all numbers, query the BIT over the entire range (rank of maximum value) to get the global maximum length and the total count of subsequences achieving that length. Return the count.
//
// **Edge cases**:
// - All elements equal: every single element is an LIS of length 1, so the count is `n`.
// - Single element: answer is 1.
// - Strictly decreasing: same as all equal? No, if strictly decreasing, each element alone is an LIS length 1, count = n.
// - Duplicate values: must not combine elements with equal values in a subsequence (strictly increasing). The query uses rank-1 to exclude equal values.
//
// **Time complexity**: O(n log n) due to BIT operations per element (each update and query is O(log n)). Space: O(n) for the BIT and coordinate compression maps.
//
// **Alternative DP approach**: A standard O(n²) DP with `dp[i]` = length of LIS ending at i, and `cnt[i]` = number of ways, works but is O(n²). The BIT solution is more advanced and demonstrates efficient handling of prefix maxima with counts.
//
// The reference solution uses the BIT approach exactly as in the snippet, with appropriate modifications for clarity and const correctness.

#include <vector>
#include <algorithm>
#include <unordered_map>

// Helper: Fenwick tree storing (max_length, count) pairs for prefix maxima.
class Fenwick {
    std::vector<std::pair<int, int>> tree; // (max_len, count)
public:
    explicit Fenwick(int size) : tree(size + 1, {0, 0}) {}

    // Update: if new_len > current_len, replace; if equal, add counts.
    void update(int idx, int len, int cnt) {
        while (idx < static_cast<int>(tree.size())) {
            if (len > tree[idx].first) {
                tree[idx] = {len, cnt};
            } else if (len == tree[idx].first) {
                tree[idx].second += cnt;
            }
            idx += idx & -idx;
        }
    }

    // Query: return the best (max_len, count) among prefixes up to idx.
    std::pair<int, int> query(int idx) const {
        std::pair<int, int> res = {0, 0};
        while (idx > 0) {
            if (tree[idx].first > res.first) {
                res = tree[idx];
            } else if (tree[idx].first == res.first) {
                res.second += tree[idx].second;
            }
            idx -= idx & -idx;
        }
        return res;
    }
};

// Count the number of longest strictly increasing subsequences in nums.
int countLongestIncreasingSubsequences(const std::vector<int>& nums) {
    int n = static_cast<int>(nums.size());
    if (n == 0) return 0;

    // Coordinate compression to ranks 1..m (m = distinct count)
    std::vector<int> vals = nums;
    std::sort(vals.begin(), vals.end());
    vals.erase(std::unique(vals.begin(), vals.end()), vals.end());
    std::unordered_map<int, int> rank;
    for (int i = 0; i < static_cast<int>(vals.size()); ++i) {
        rank[vals[i]] = i + 1; // 1-based
    }

    Fenwick bit(static_cast<int>(vals.size()));

    for (int v : nums) {
        int r = rank[v];
        // Query strictly less than v: rank-1
        auto best = bit.query(r - 1);
        int new_len = best.first + 1;
        int new_cnt = std::max(best.second, 1); // if best.second==0, start new subsequence
        bit.update(r, new_len, new_cnt);
    }

    // Query over all values to get global max length and count
    auto result = bit.query(static_cast<int>(vals.size()));
    return result.second;
}

#include <cassert>
#include <vector>

// Function to test (already defined above, but we declare here for clarity)
int countLongestIncreasingSubsequences(const std::vector<int>& nums);

int main() {
    // Example from problem: [1,3,5,4,7] -> LIS length 3, two subsequences: [1,3,5,7] and [1,3,4,7]
    assert(countLongestIncreasingSubsequences({1,3,5,4,7}) == 2);

    // All equal: each single element is LIS length 1, count = 5
    assert(countLongestIncreasingSubsequences({2,2,2,2,2}) == 5);

    // Single element
    assert(countLongestIncreasingSubsequences({7}) == 1);

    // Strictly decreasing: each element alone is LIS length 1, count = 3
    assert(countLongestIncreasingSubsequences({5,4,3}) == 3);

    // Already strictly increasing
    assert(countLongestIncreasingSubsequences({1,2,3,4}) == 1);

    // With duplicates but still multiple LIS
    // LIS length 2 in [1,2,2,3]: subsequences [1,2,3] length 3? Actually 1,2,3 works, length 3 count 1.
    // So assert 1:
    assert(countLongestIncreasingSubsequences({1,2,2,3}) == 1);

    // Mixed duplicates producing multiple LIS: [1,3,2,4] -> LIS length 3: [1,3,4] and [1,2,4] -> count 2
    assert(countLongestIncreasingSubsequences({1,3,2,4}) == 2);

    // Another example: [1,1,1,2] -> LIS length 2, count? Subsequences [1,2] (each 1 with the 2) -> 3 ways
    assert(countLongestIncreasingSubsequences({1,1,1,2}) == 3);

    // Edge: empty? But problem says non-empty, we still assert 0 if called with empty
    assert(countLongestIncreasingSubsequences({}) == 0);

    return 0;
}
