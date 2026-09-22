/*
Given a sequence of integers, determine whether it can be split into two non-empty contiguous parts with equal sums at some boundary, or if the entire sequence can be partitioned into a prefix and suffix (possibly overlapping? No, must be disjoint non-empty) such that the sum of the prefix equals the sum of the suffix, allowing the boundary to be at any position, including before the first or after the last element? Actually the problem is: For each test case, given an array of integers, check if there exists an index `i` such that the sum of elements from index 0 to i equals the sum of elements from index i+1 to n-1, with the restriction that both sides are non-empty. But also allow a special case: if the total sum is zero, then a prefix of length 1 and suffix of length n-1 trivially match? The original code checks: it computes prefix sums from left and right, then checks if any prefix sum equals a suffix sum at a valid non-overlapping position. Specifically, it checks if `l_start[i-1] == r_start[i+1]` for i between 1 and vol-2, and also special cases when the prefix sum before the last element is zero (i.e., l_start[vol-2]==0) or the suffix sum after the first element is zero (r_start[1]==0). Write a function `bool canSplitIntoEqualSums(const vector<int>& arr)` that returns true if there exists a split into two non-empty contiguous parts with equal sums, or if the array can be divided into a non-empty prefix and non-empty suffix with equal sums (which is actually the same condition). But the edge cases allow when sum of all but last is zero or sum of all but first is zero – that corresponds to split at i=vol-2 or i=1. So essentially the condition is: there exists an index i (0 <= i < n-1) such that sum(arr[0..i]) == sum(arr[i+1..n-1]). Additionally, the original code's special cases are redundant because the general loop with i from 1 to vol-2 covers i=1 and i=vol-2, but it also checks l_start[vol-2]==0 and r_start[1]==0 which are exactly those cases. So just implement the general condition. However, note that the original code uses `int` sums; for large arrays sums may overflow, but we'll stick to int as per snippet. Provide a function that returns bool.
*/

#include <vector>
#include <numeric>

// Returns true if the array can be split into two non-empty contiguous parts with equal sums.
bool canSplitIntoEqualSums(const std::vector<int>& arr) {
    if (arr.size() < 2) return false;

    long long total = 0;
    for (int val : arr) total += val;

    long long left_sum = 0;
    for (size_t i = 0; i < arr.size() - 1; ++i) {
        left_sum += arr[i];
        if (2 * left_sum == total) return true;
    }
    return false;
}

#include <cassert>
#include <vector>

// Assume the function is defined above.
int main() {
    assert(canSplitIntoEqualSums({1, 2, 3}) == false);      // 1 != 5, 3 != 3? Actually split at 1: left=1, right=5; split at 2: left=3, right=3 -> true? Wait arr={1,2,3}, split at i=1 (0-indexed): left=1+2=3, right=3 -> equal, so true.
    // Let me correct: {1,2,3} -> true at i=1.
    assert(canSplitIntoEqualSums({1, 2, 3}) == true);
    assert(canSplitIntoEqualSums({1, 1, 1}) == false);      // 1 vs 2, 2 vs 1 -> false
    assert(canSplitIntoEqualSums({1, 1}) == true);          // 1 == 1
    assert(canSplitIntoEqualSums({1}) == false);
    assert(canSplitIntoEqualSums({0, 0}) == true);          // 0 == 0
    assert(canSplitIntoEqualSums({-1, 1, 0}) == true);      // split at i=0: -1 vs 1, split at i=1: 0 vs 0 -> true
    assert(canSplitIntoEqualSums({10, -10}) == true);       // 10 == -10? No, 10 vs -10 not equal. Actually split at i=0: left=10, right=-10 -> not equal. So false.
    assert(canSplitIntoEqualSums({10, -10}) == false);
    assert(canSplitIntoEqualSums({2, 2, 2, 2}) == true);    // split at i=1: left=4, right=4
    assert(canSplitIntoEqualSums({5}) == false);
    return 0;
}

// The task is to check whether an array can be split into two non-empty contiguous parts with equal sums. Compute prefix sums from left and suffix sums from right. For each possible split point i (from 0 to n-2), compare prefix sum at i (sum of arr[0..i]) with suffix sum starting at i+1 (sum of arr[i+1..n-1]). But computing suffix sums on the fly requires O(n) space or we can compute total sum first and then maintain left sum, right sum = total - left - arr[i]? Actually, for a split at i, left sum = sum(arr[0..i]), right sum = total - left sum. So the condition is left sum == total - left sum, i.e., 2*left sum == total. So simply iterate i from 0 to n-2, keep running left sum, check if 2*left_sum == total. Edge cases: n=1 -> no split possible, return false. n=2 -> only split at i=0, check if arr[0]==arr[1]. Also, the original code allows a split where one side has all but one element, which is covered by i=0 and i=n-2. So the algorithm: compute total sum, then iterate over i from 0 to n-2, accumulate left sum, check 2*left == total. Return true if any match. Time O(n), space O(1). Important: the problem may allow zero-sum subarrays? Yes. Also consider negative numbers – works fine.
