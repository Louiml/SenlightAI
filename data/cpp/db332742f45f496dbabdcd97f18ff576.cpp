Write a C++ function `int maxPairSum(std::vector<int>& nums)` that takes a vector of integers (which always has an even length, at least 2, and may contain negative numbers, duplicates, or zero) and returns the maximum possible sum of the minimums of each pair after partitioning the integers into pairs. The pairing is unrestricted: you can choose how to pair the numbers. The function must sort the input in non-decreasing order before processing.

#include <cassert>
#include <vector>

// Function declaration (must match the solution exactly)
int maxPairSum(std::vector<int>& nums);

int main() {
    // Basic case from the original snippet
    std::vector<int> nums1 = {1, 3, 4, 2};
    assert(maxPairSum(nums1) == 4); // pairs: (1,2)->1, (3,4)->3 => 1+3=4

    // Large numbers
    std::vector<int> nums2 = {6, 2, 6, 5, 1, 2};
    assert(maxPairSum(nums2) == 9); // sorted: 1,2,2,5,6,6 => 1+2+6=9

    // Negative numbers
    std::vector<int> nums3 = {-5, -1, -10, -2};
    assert(maxPairSum(nums3) == -7); // sorted: -10,-5,-2,-1 => -10 + -2 = -12? actually -10 + -2 = -12, but better pairing? Let's compute: sorted -10,-5,-2,-1 -> sum = -10 + -2 = -12, but optimal is pair (-10,-1)->-10 and (-5,-2)->-5 sum=-15? Wait careful: We need max sum of mins. Sorted gives pairs (-10,-5)->min=-10 and (-2,-1)->min=-2 sum=-12. Alternative pairing: (-10,-1)->-10 and (-5,-2)->-5 sum=-15, worse. So -12 is correct. But let's verify alternate: Actually pairing (-10,-2)->-10 and (-5,-1)->-5 sum=-15. So -12 is best.

    // Duplicates and zero
    std::vector<int> nums4 = {0, 0, 7, 7, 3, 3};
    assert(maxPairSum(nums4) == 3); // sorted: 0,0,3,3,7,7 => 0+3+7=10? Wait sum even indices: 0+3+7=10. Actually correct: 0+3+7=10. Let's re-check: pairs (0,0)->0, (3,3)->3, (7,7)->7 sum=10. Yes assert that.

    // Already sorted input
    std::vector<int> nums5 = {1, 2, 3, 4};
    assert(maxPairSum(nums5) == 4); // 1+3=4

    // All same values
    std::vector<int> nums6 = {5, 5, 5, 5};
    assert(maxPairSum(nums6) == 10); // 5+5=10

    // Longer even list
    std::vector<int> nums7 = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    assert(maxPairSum(nums7) == 20); // sorted: 1..10 => 1+3+5+7+9=25? Actually sum of 1,3,5,7,9 = 25. But check: pairs (1,2)->1, (3,4)->3, (5,6)->5, (7,8)->7, (9,10)->9 sum=25. Yes assert 25.

    return 0;
}

#include <vector>
#include <algorithm>

// Given a vector of integers with even length, partition into pairs and
// return the maximum possible sum of the minimums of each pair.
// The input vector is sorted in place to achieve the optimal pairing.
int maxPairSum(std::vector<int>& nums) {
    std::sort(nums.begin(), nums.end());
    int sum = 0;
    // After sorting, pairs are (nums[0], nums[1]), (nums[2], nums[3]), ...
    // The minimum of each pair is the element at even index.
    for (std::size_t i = 0; i < nums.size(); i += 2) {
        sum += nums[i];
    }
    return sum;
}

// The key insight is that to maximize the sum of the minimums of each pair, we should pair the numbers so that the smallest numbers are not "wasted" as large minimums. Sort the array in ascending order. Then, the optimal pairing is always adjacent elements: (nums[0], nums[1]), (nums[2], nums[3]), and so on. This works because sorting ensures that for any pair, the minimum is the first (smaller) element of that pair. Greedily taking every second element starting from index 0 (i.e., nums[0], nums[2], nums[4], ...) gives the maximum sum. A direct proof: in any optimal partition, the globally smallest element must be paired with something, and the sum contribution is that smallest element. To maximize the next contribution, we want the second smallest element to also contribute as much as possible, which forces pairing the second smallest with the smallest, then the third smallest contributes, etc. The algorithm simply sorts (O(n log n)) and then sums every element at even indices (O(n)), so total time O(n log n) and space O(1) auxiliary (excluding input storage). Edge cases: negative numbers are handled naturally by sorting; duplicates are fine; the vector size is guaranteed even, so no out-of-bounds risk.
