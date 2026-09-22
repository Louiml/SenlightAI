// Write a C++ function named `rangeSum` that takes a vector of integers `nums` and a vector of pairs `queries`, where each pair `(l, r)` represents a 1-indexed inclusive range, and returns a vector of integers containing the sum of elements in `nums` from index `l` to index `r` for each query. The function must handle arbitrary input sizes, including empty `nums` (in which case all query results should be 0) and must correctly process queries where `l` and `r` are within the bounds of the vector. Use a prefix sum array for efficiency: precompute cumulative sums once, then answer each query in constant time. Ensure the function is `const`-correct and does not modify its inputs.
The solution uses a prefix sum array `prefix` of length `n+1` where `prefix[0] = 0` and `prefix[i] = sum of nums[0..i-1]` for `1 <= i <= n`. For each query `(l, r)` (1-indexed), the range sum is `prefix[r] - prefix[l-1]`. This works because `prefix[r]` includes all elements up to index `r-1`, and subtracting `prefix[l-1]` removes elements before index `l-1`. Edge cases: if `nums` is empty, all sums are 0; if `l == 1`, then `prefix[l-1]` is `prefix[0] = 0`; if `l == r`, the sum is just that single element. Time complexity: building prefix array is O(n), answering each query is O(1), total O(n + q) where q is number of queries. Space complexity: O(n) for the prefix array (plus O(q) for the result vector, which is necessary output).
#include <vector>
#include <utility>

// Given a vector of integers and a list of 1-indexed inclusive range queries,
// return the sum of each range using prefix sums.
std::vector<int> rangeSum(const std::vector<int>& nums,
                          const std::vector<std::pair<int,int>>& queries) {
    int n = nums.size();
    std::vector<int> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        prefix[i+1] = prefix[i] + nums[i];
    }
    std::vector<int> result;
    result.reserve(queries.size());
    for (const auto& q : queries) {
        int l = q.first;
        int r = q.second;
        if (l < 1 || r > n || l > r) {
            result.push_back(0); // Invalid or empty range; treat as 0.
        } else {
            result.push_back(prefix[r] - prefix[l-1]);
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

// Assume rangeSum is declared above.

int main() {
    // Basic test with positive numbers
    std::vector<int> nums1 = {1, 2, 3, 4, 5};
    std::vector<std::pair<int,int>> q1 = {{1, 5}, {2, 4}, {3, 3}};
    std::vector<int> res1 = rangeSum(nums1, q1);
    assert(res1.size() == 3);
    assert(res1[0] == 15); // 1+2+3+4+5
    assert(res1[1] == 9);  // 2+3+4
    assert(res1[2] == 3);  // 3

    // Negative numbers
    std::vector<int> nums2 = {-5, 10, -3, 7};
    std::vector<std::pair<int,int>> q2 = {{1, 4}, {2, 2}, {1, 1}};
    std::vector<int> res2 = rangeSum(nums2, q2);
    assert(res2[0] == 9);  // -5+10-3+7
    assert(res2[1] == 10);
    assert(res2[2] == -5);

    // Single element
    std::vector<int> nums3 = {42};
    std::vector<std::pair<int,int>> q3 = {{1, 1}};
    assert(rangeSum(nums3, q3)[0] == 42);

    // Empty nums
    std::vector<int> nums4;
    std::vector<std::pair<int,int>> q4 = {{1, 1}, {0, 0}};
    std::vector<int> res4 = rangeSum(nums4, q4);
    assert(res4.size() == 2);
    assert(res4[0] == 0);
    assert(res4[1] == 0);

    // Out-of-bounds and invalid queries
    std::vector<int> nums5 = {2, 4, 6};
    std::vector<std::pair<int,int>> q5 = {{0, 2}, {2, 5}, {3, 2}, {1, 3}};
    std::vector<int> res5 = rangeSum(nums5, q5);
    assert(res5[0] == 0); // l<1
    assert(res5[1] == 0); // r>n
    assert(res5[2] == 0); // l>r
    assert(res5[3] == 12); // 2+4+6

    // Large repeated queries
    std::vector<int> nums6 = {1, 1, 1, 1};
    std::vector<std::pair<int,int>> q6 = {{1, 4}, {2, 3}, {2, 2}};
    std::vector<int> res6 = rangeSum(nums6, q6);
    assert(res6[0] == 4);
    assert(res6[1] == 2);
    assert(res6[2] == 1);

    return 0;
}
