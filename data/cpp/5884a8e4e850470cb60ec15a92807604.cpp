// Write a C++ function that takes a vector of positive integers and returns a minimal subsequence (as a vector) such that the sum of the chosen elements is strictly greater than the sum of the remaining elements. If multiple such subsequences exist, return the one with the largest possible sum (which will also be the lexicographically largest among those with minimal length). The function should sort the result in descending order before returning. For example, given `[4,3,10,9,8]`, the output should be `[10,9]` because 10+9=19 > 4+3+8=15, and no smaller subsequence satisfies the condition. The input vector may be empty; in that case, return an empty vector.
// The solution approach: First, sort the entire array in ascending order. Compute the total sum of all elements. Then iterate from the largest element down to the smallest, greedily adding elements to the result while subtracting them from the remaining sum. After adding each element, check if the accumulated right-side sum is greater than the remaining left-side sum. If yes, stop. This greedy works because we always pick the largest available elements first, ensuring the subsequence has minimal length (since we take the fewest largest numbers needed) and maximum possible sum among minimal-length subsequences. Edge cases: empty input returns empty; if the sum of all elements is already greater than zero (but since all are positive, only empty input is trivial), and if the largest element alone already exceeds the sum of the rest, we stop immediately after one pick. Time complexity is O(n log n) due to sorting, plus O(n) for the loop, so overall O(n log n). Space complexity is O(n) for the output vector (excluding input storage) and O(1) auxiliary if we ignore the sort's internal stack usage.
#include <vector>
#include <algorithm>
#include <numeric>

// Returns a minimal subsequence of nums whose sum is strictly greater than the sum of the remaining elements.
// The result is sorted in descending order. If multiple minimal-length subsequences exist, picks the one with the largest sum.
std::vector<int> minSubsequence(std::vector<int> nums) {
    if (nums.empty()) return {};
    
    std::sort(nums.begin(), nums.end()); // ascending
    int totalSum = std::accumulate(nums.begin(), nums.end(), 0);
    int remainingSum = totalSum; // sum of elements not yet chosen
    int chosenSum = 0;
    std::vector<int> result;
    
    // Iterate from largest to smallest
    for (int i = static_cast<int>(nums.size()) - 1; i >= 0; --i) {
        chosenSum += nums[i];
        remainingSum -= nums[i];
        result.push_back(nums[i]);
        if (chosenSum > remainingSum) {
            break;
        }
    }
    return result; // already in descending order because we traversed sorted ascending backwards
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    std::vector<int> v1 = {4, 3, 10, 9, 8};
    assert((minSubsequence(v1) == std::vector<int>{10, 9}));
    
    std::vector<int> v2 = {6, 7, 1, 2, 3};
    assert((minSubsequence(v2) == std::vector<int>{7, 6}));
    
    std::vector<int> v3 = {1, 2, 3};
    assert((minSubsequence(v3) == std::vector<int>{3, 2}));
    
    // Single element
    std::vector<int> v4 = {5};
    assert((minSubsequence(v4) == std::vector<int>{5}));
    
    // Already large element dominates
    std::vector<int> v5 = {10, 1, 1, 1};
    assert((minSubsequence(v5) == std::vector<int>{10}));
    
    // All equal elements
    std::vector<int> v6 = {2, 2, 2};
    assert((minSubsequence(v6) == std::vector<int>{2, 2}));
    
    // Empty input
    std::vector<int> v7 = {};
    assert((minSubsequence(v7) == std::vector<int>{}));
    
    // Larger test: need multiple picks
    std::vector<int> v8 = {1, 2, 3, 4, 5, 6, 7, 8};
    // total=36, we need >18. Greedy picks 8,7,6,5 -> sum=26 > 10, but actually we can stop at 8+7+6=21 > 15? Check: 36-8=28, not >; 28-7=21, not > (21 > 21 false); 21-6=15, now chosen=21 > 15 true, so result {8,7,6}
    assert((minSubsequence(v8) == std::vector<int>{8, 7, 6}));
    
    // Duplicate values
    std::vector<int> v9 = {5, 5, 1, 1};
    // total=12, need >6. Greedy: 5+5=10 > 2, so {5,5}
    assert((minSubsequence(v9) == std::vector<int>{5, 5}));
    
    return 0;
}
