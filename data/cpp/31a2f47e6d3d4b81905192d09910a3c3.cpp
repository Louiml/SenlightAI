// Write a C++ function `kSmallestPairs` that takes two sorted integer vectors `nums1` and `nums2` (both non-decreasing) and a positive integer `k`, and returns a vector of vectors containing the `k` smallest pairs `(nums1[i], nums2[j])` as `{nums1[i], nums2[j]}` with the sum `nums1[i] + nums2[j]` used as the sort key. The pairs should be returned in ascending order of their sums, and in case of equal sums, any relative order is acceptable. You must not use sorting the entire set of pairs; instead, use an efficient approach that produces only the required `k` pairs. The function should handle edge cases such as empty input vectors, `k` larger than the total number of possible pairs (in which case return all possible pairs), and duplicates in the input arrays (each duplicate element is treated as distinct). The output vector should be built efficiently without generating all pairs at once.
// The core idea is to use a min-heap (priority queue) to lazily generate pairs in increasing order of their sums. Since both arrays are sorted, for each element `nums2[j]` we consider the pair `(nums1[0], nums2[j])` as the smallest possible pair for that column. We initialize the heap with the first `min(k, nums2.size())` columns using `nums1[0]` paired with `nums2[j]`. Then, repeatedly extract the smallest sum from the heap, push the resulting pair into the result, and if there is a next element in `nums1` for that column (i.e., `i+1 < nums1.size()`), push `(nums1[i+1], nums2[j])` into the heap. This guarantees that we always pop the globally smallest pair among the candidates, and each pair is generated at most once. Edge cases: if either vector is empty or `k==0`, return an empty vector. Because we initialize at most `k` columns (but no more than `nums2.size()`), and each pop adds one pair, the total number of heap operations is `O(k log k)` (since the heap size is at most `k`). Space complexity is `O(k)` for the heap and `O(k)` for the result (excluding the result itself). The time complexity is `O(k log k)`. If `k` is larger than the total pairs, the loop naturally stops when the heap is empty, returning all pairs.
#include <vector>
#include <queue>
#include <tuple>

// Return the k smallest pairs (as {nums1[i], nums2[j]}) ordered by their sum.
// Input vectors must be sorted non-decreasing. If k exceeds the total number
// of pairs, all pairs are returned.
std::vector<std::vector<int>> kSmallestPairs(
    const std::vector<int>& nums1,
    const std::vector<int>& nums2,
    int k) {

    std::vector<std::vector<int>> result;
    if (nums1.empty() || nums2.empty() || k <= 0) {
        return result;
    }

    // Min-heap storing tuples (sum, index_in_nums1, index_in_nums2)
    auto comp = [](const std::tuple<int, int, int>& a,
                   const std::tuple<int, int, int>& b) {
        // Return true if a should come after b (i.e., a has larger sum)
        return std::get<0>(a) > std::get<0>(b);
    };
    std::priority_queue<
        std::tuple<int, int, int>,
        std::vector<std::tuple<int, int, int>>,
        decltype(comp)> minHeap(comp);

    // Initialize with first element of nums1 paired with first k elements of nums2
    // (or all of nums2 if smaller than k)
    for (int j = 0; j < static_cast<int>(nums2.size()) && j < k; ++j) {
        minHeap.emplace(nums1[0] + nums2[j], 0, j);
    }

    // Extract k smallest pairs
    while (k-- > 0 && !minHeap.empty()) {
        auto [sum, i, j] = minHeap.top();
        minHeap.pop();
        result.push_back({nums1[i], nums2[j]});

        // If there is a next element in nums1 for this column, push candidate
        if (i + 1 < static_cast<int>(nums1.size())) {
            minHeap.emplace(nums1[i + 1] + nums2[j], i + 1, j);
        }
    }

    return result;
}
#include <cassert>
#include <vector>

// The function under test is declared above (assumed available).
int main() {
    // Basic test with k smaller than total pairs
    std::vector<int> nums1 = {1, 7, 11};
    std::vector<int> nums2 = {2, 4, 6};
    auto result = kSmallestPairs(nums1, nums2, 3);
    assert(result.size() == 3);
    assert(result[0] == std::vector<int>{1, 2});
    assert(result[1] == std::vector<int>{1, 4});
    assert(result[2] == std::vector<int>{1, 6} || result[2] == std::vector<int>{7, 2});

    // Test k larger than total pairs returns all pairs
    nums1 = {1, 2};
    nums2 = {3};
    result = kSmallestPairs(nums1, nums2, 10);
    assert(result == std::vector<std::vector<int>>({{1,3},{2,3}}));

    // Test empty input
    result = kSmallestPairs({}, {1,2}, 5);
    assert(result.empty());
    result = kSmallestPairs({1,2}, {}, 5);
    assert(result.empty());

    // Test k = 0
    result = kSmallestPairs({1,2}, {3,4}, 0);
    assert(result.empty());

    // Test duplicate values
    nums1 = {1, 1, 2};
    nums2 = {1, 1};
    result = kSmallestPairs(nums1, nums2, 4);
    assert(result.size() == 4);
    // All sums should be 2 for first three, then 3
    int sum0 = result[0][0] + result[0][1];
    int sum1 = result[1][0] + result[1][1];
    int sum2 = result[2][0] + result[2][1];
    int sum3 = result[3][0] + result[3][1];
    assert(sum0 == 2 && sum1 == 2 && sum2 == 2 && sum3 == 3);

    // Test single element arrays
    nums1 = {5};
    nums2 = {10};
    result = kSmallestPairs(nums1, nums2, 1);
    assert(result == std::vector<std::vector<int>>({{5,10}}));

    // Test k = 1
    nums1 = {1, 100, 200};
    nums2 = {1, 2};
    result = kSmallestPairs(nums1, nums2, 1);
    assert(result == std::vector<std::vector<int>>({{1,1}}));

    // Test negative numbers
    nums1 = {-3, 0};
    nums2 = {-2, 1};
    result = kSmallestPairs(nums1, nums2, 3);
    assert(result.size() == 3);
    // Possible pairs sorted by sum: (-3,-2)= -5, (-3,1)= -2, (0,-2)= -2, (0,1)=1
    assert(result[0] == std::vector<int>{-3, -2});
    // Next two can be either order, but both sums must be -2
    int sum3a = result[1][0] + result[1][1];
    int sum3b = result[2][0] + result[2][1];
    assert(sum3a == -2 && sum3b == -2);

    return 0;
}
