// Write a C++ function named `findPivotIndex` that accepts a non-empty vector of integers (`std::vector<int>`) and returns the index of the first "pivot" element. A pivot index is defined as an index `i` such that the sum of all elements strictly to the left of `i` equals the sum of all elements strictly to the right of `i`. If no such index exists, the function must return `-1`. The function must handle vectors with negative numbers, zeros, duplicates, and any length from 1 to large sizes. You must implement an efficient solution using prefix sums (not brute force), and the function must be `const`‑correct (i.e., it should not modify the input vector).
#include <cassert>
#include <vector>

// Replace with the actual solution header if separate. This main tests the function.
int main() {
    // Example from typical LeetCode problem: [1,7,3,6,5,6] -> pivot at index 3
    std::vector<int> v1 = {1, 7, 3, 6, 5, 6};
    assert(findPivotIndex(v1) == 3);

    // Single element: [1] -> pivot at 0 (left 0, right 0)
    std::vector<int> v2 = {1};
    assert(findPivotIndex(v2) == 0);

    // No pivot: [1,2,3] -> -1
    std::vector<int> v3 = {1, 2, 3};
    assert(findPivotIndex(v3) == -1);

    // All zeros: [0,0,0] -> first pivot 0
    std::vector<int> v4 = {0, 0, 0};
    assert(findPivotIndex(v4) == 0);

    // Negative numbers: [-1,-1,0,1,1] -> pivot at index 2 (left=-2, right=2)?? Wait check: left -2, right -1+1=0? Actually compute: -1-1+0+1+1=0; at i=2: left=-2, right=1+1=2, no; i=3: left=-1, right=1, no; i=0: left=0, right=0? Wait right = total - left - nums[0] = 0-0-(-1)=1, no. Actually no pivot? Let's pick a known example: [2,1,-1] -> pivot at 0? left=0, right=1-1=0 → yes.
    std::vector<int> v5 = {2, 1, -1};
    assert(findPivotIndex(v5) == 0);

    // Pivot with negative numbers: [-1, -2, 3] -> pivot at 2? left=-3, right=0, no. Try [-2,1,1] pivot at 1? left=-2, right=1, no; index0 left=0 right=2 no; index2 left=-1 right=0 no. Try [0,-1,1] pivot at 1? left=0 right=1 no; at 2 left=-1 right=0 no; at 0 left0 right0? right=0-0-0=0 yes. Actually a simple negative test: [-1,1] pivot at 0? left0 right=1 no; at1 left=-1 right=0 no → -1.
    std::vector<int> v6 = {-1, 1};
    assert(findPivotIndex(v6) == -1);

    // Large negative and positive: [1,-1,1,-1] pivot at 1? left=1 right=1-1+(-1)? Wait compute total=0; at i=1: left=1, right=0-1-(-1)=0? Actually right = total-left-nums[1]=0-1-(-1)=0, left=1 ≠0; try i=0 left0 right=0-0-1=-1 no; i=2 left=0 right=0-0-1=-1 no; i=3 left=-1 right=0 no → -1. Test a valid one: [1,2,2,1] pivot at 1? left=1 right=2+1=3 no; at2 left=3 right=1 no; at0 left0 right=5 no; at3 left=5 right0 no → -1. Use simple: [1,2,3] no pivot already. 

    // Classic: [1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2,1] pivot at 9? Not needed.

    // Ensure first pivot is returned: [1,0,1] pivot at 0? left0 right=1, no; at1 left=1 right=1 → index 1.
    std::vector<int> v7 = {1, 0, 1};
    assert(findPivotIndex(v7) == 1);

    // Long vector with pivot at end: [1,2,3,0] pivot at 3? left=6 right=0 no; at2 left=3 right=0 no; at1 left=1 right=3 no; at0 left0 right=6 no → -1. Use [0,1,2,3] pivot 1? left0 right=5 no; actually no. 

    return 0;
}
#include <vector>

/**
 * @brief Finds the first pivot index in a vector of integers.
 * 
 * A pivot index is an index i such that the sum of elements strictly to the left
 * equals the sum of elements strictly to the right. Returns -1 if none exists.
 * 
 * @param nums A non-empty vector of integers.
 * @return int The smallest pivot index, or -1.
 */
int findPivotIndex(const std::vector<int>& nums) {
    int totalSum = 0;
    for (int x : nums) {
        totalSum += x;
    }
    
    int leftSum = 0;
    for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
        int rightSum = totalSum - leftSum - nums[i];
        if (leftSum == rightSum) {
            return i;
        }
        leftSum += nums[i];
    }
    return -1;
}
// The key idea is to precompute the left and right sums for every index without nested loops. The brute-force approach (for each index, sum everything left and right) is O(n²). Instead, we compute two auxiliary arrays: `leftSum` where `leftSum[i]` = sum of `nums[0]` … `nums[i-1]` (with `leftSum[0] = 0`), and `rightSum` where `rightSum[i]` = sum of `nums[i+1]` … `nums[n-1]` (with `rightSum[n-1] = 0`). Then a single pass checks if `leftSum[i] == rightSum[i]`. This works for edge cases: a single element (index 0 has left 0 and right 0 → pivot at 0), all zeros (first index 0 is a pivot), and negative numbers (sums can be negative). We can even optimize space to O(1) by first computing the total sum, then maintaining a running left sum while iterating: for each `i`, `rightSum = totalSum - leftSum - nums[i]`. The algorithm runs in O(n) time and O(1) auxiliary space (excluding the input vector). Returning the first match ensures the earliest pivot index is chosen. If no match is found after the loop, return `-1`.
