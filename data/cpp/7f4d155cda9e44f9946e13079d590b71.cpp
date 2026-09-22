Write a C++ function named `maxSubsetSizeUnderLimits` that takes a vector of positive integers `nums` and a vector of positive integers `queries`. For each query value `q`, the function must return the maximum number of elements from `nums` that can be selected such that the sum of the selected elements is less than or equal to `q`. Selection is unrestricted — any subset (not necessarily contiguous) can be chosen, and each element may be used at most once. The order of the results must match the order of `queries`. The function must be `const`-correct and not modify the input vectors (you may copy and sort internally).

The key insight is that to maximize the count of elements under a sum limit, we should always pick the smallest elements first. This is a greedy strategy that works because all values are positive — swapping a larger selected element with a smaller unselected one never increases the sum and never decreases the count. Therefore, after sorting `nums` in ascending order, for each query we can scan from the smallest element, accumulating the sum until adding the next element would exceed the query. The number of elements successfully added is the answer for that query. Important edge cases: if the query is smaller than the smallest element, the count is 0; if all elements fit, the count is the size of `nums`. Duplicates are handled naturally because the sorted scan includes them. Time complexity is O(n log n + m * n) where n = nums.size(), m = queries.size(), due to sorting and then a linear scan per query. Space complexity is O(n) for the copied sorted vector (or O(1) if we sort in-place, but we avoid modifying input by copying).

#include <vector>
#include <algorithm>

// For each query q, return the maximum number of smallest elements
// from a sorted copy of nums whose sum is <= q.
std::vector<int> maxSubsetSizeUnderLimits(const std::vector<int>& nums,
                                          const std::vector<int>& queries) {
    std::vector<int> sorted = nums;
    std::sort(sorted.begin(), sorted.end());

    std::vector<int> result;
    result.reserve(queries.size());

    for (int q : queries) {
        int sum = 0;
        int count = 0;
        while (count < static_cast<int>(sorted.size()) &&
               sum + sorted[count] <= q) {
            sum += sorted[count];
            ++count;
        }
        result.push_back(count);
    }
    return result;
}

#include <cassert>
#include <vector>

int main() {
    std::vector<int> nums1 = {1, 2, 3, 4};
    std::vector<int> queries1 = {1, 3, 5, 10};
    assert(maxSubsetSizeUnderLimits(nums1, queries1) == std::vector<int>({1, 2, 3, 4}));

    std::vector<int> nums2 = {5, 5, 5};
    std::vector<int> queries2 = {4, 5, 10, 15};
    assert(maxSubsetSizeUnderLimits(nums2, queries2) == std::vector<int>({0, 1, 2, 3}));

    std::vector<int> nums3 = {10, 1, 3, 2};
    std::vector<int> queries3 = {3, 6, 16};
    assert(maxSubsetSizeUnderLimits(nums3, queries3) == std::vector<int>({2, 3, 4}));

    std::vector<int> nums4 = {100};
    std::vector<int> queries4 = {99, 100, 101};
    assert(maxSubsetSizeUnderLimits(nums4, queries4) == std::vector<int>({0, 1, 1}));

    std::vector<int> nums5 = {1, 1, 1, 1};
    std::vector<int> queries5 = {0, 1, 2, 4};
    assert(maxSubsetSizeUnderLimits(nums5, queries5) == std::vector<int>({0, 1, 2, 4}));

    // Ensure const-correctness and input not modified
    std::vector<int> original = {3, 1, 2};
    maxSubsetSizeUnderLimits(original, {3});
    assert(original == std::vector<int>({3, 1, 2}));
}
