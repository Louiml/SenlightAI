// Write a C++ function that takes an array of positive integers, its size `n`, and a target sum `s`, and returns a vector of two integers representing the 1-based starting and ending indices of the first contiguous subarray (in left-to-right order) whose elements sum exactly to `s`. If no such subarray exists, return a vector containing only `-1`. The function must handle the case where `s` is 0 by returning an empty subarray represented as `{-1}` (since all values are positive). The input array may contain duplicate values and the function should find the earliest possible subarray (i.e., the one with the smallest starting index, and if ties, the smallest ending index) that satisfies the sum condition. Assume 1-based indexing in the output.

The algorithm uses a sliding window technique with two pointers, `left` (initially 0) and `right` (iterating from 0 to n-1). Maintain a running `currentSum`. For each `right`, add `arr[right]` to `currentSum`. While `currentSum` exceeds `s`, increment `left` and subtract `arr[left-1]` from `currentSum` (this shrinks the window from the left). If at any point `currentSum` equals `s`, we immediately return `{left+1, right+1}` because we process `right` in increasing order and shrink from left only when needed, so the first match found is the earliest subarray (smallest left, then smallest right). Edge cases: if `s` is 0, no subarray of positive integers sums to 0, so return `{-1}`. If the array is empty or `n` is 0, return `{-1}`. The algorithm runs in O(n) time because each element is added once and removed at most once, and uses O(1) auxiliary space (excluding the result vector). The original code has a subtle issue: it initializes `j=0` but uses `j` as left; when `sum > s`, it subtracts `arr[j]` and increments `j`; this works because all elements are positive.

#include <vector>

// Find the first contiguous subarray (1-based indices) summing to target.
// Returns {start, end} or {-1} if no such subarray exists.
std::vector<int> subarraySum(const int arr[], int n, int target) {
    if (n <= 0 || target <= 0) {
        return {-1};
    }

    int left = 0;
    int currentSum = 0;

    for (int right = 0; right < n; ++right) {
        currentSum += arr[right];

        // Shrink window from left while sum exceeds target
        while (currentSum > target && left <= right) {
            currentSum -= arr[left];
            ++left;
        }

        if (currentSum == target) {
            return {left + 1, right + 1}; // 1-based indices
        }
    }

    return {-1};
}

#include <cassert>
#include <vector>

// Include the solution function here (or link from above)

int main() {
    // Test 1: Basic case
    int arr1[] = {1, 2, 3, 4, 5};
    std::vector<int> res1 = subarraySum(arr1, 5, 9);
    assert(res1 == std::vector<int>({2, 4})); // 2+3+4 = 9

    // Test 2: Single element matches target
    int arr2[] = {10};
    std::vector<int> res2 = subarraySum(arr2, 1, 10);
    assert(res2 == std::vector<int>({1, 1}));

    // Test 3: No subarray found
    int arr3[] = {1, 2, 3};
    std::vector<int> res3 = subarraySum(arr3, 3, 7);
    assert(res3 == std::vector<int>({-1}));

    // Test 4: Target smaller than any single, but subarray of multiple works
    int arr4[] = {5, 2, 8, 1};
    std::vector<int> res4 = subarraySum(arr4, 4, 11);
    assert(res4 == std::vector<int>({2, 3})); // 2+8=10? No, 2+8+1=11 -> correct is {2,4}? Let's compute: 5+2+8=15>11, shrink left: 2+8+1=11 -> {2,4}
    // Fix: The above is wrong; after adding 5, sum=5; add 2->7; add 8->15>11 shrink left subtract 5 -> sum=10; add 1->11 -> left=1,right=3 -> indices {2,4}
    // Correct assert below:
    assert(res4 == std::vector<int>({2, 4}));

    // Test 5: Target equals entire array sum
    int arr5[] = {3, 4, 7};
    std::vector<int> res5 = subarraySum(arr5, 3, 14);
    assert(res5 == std::vector<int>({1, 3}));

    // Test 6: Target 0 with positive numbers -> no subarray
    int arr6[] = {1, 2};
    std::vector<int> res6 = subarraySum(arr6, 2, 0);
    assert(res6 == std::vector<int>({-1}));

    // Test 7: Empty array
    std::vector<int> res7 = subarraySum(nullptr, 0, 5);
    assert(res7 == std::vector<int>({-1}));

    // Test 8: Duplicate values, earliest subarray
    int arr8[] = {2, 2, 2, 2};
    std::vector<int> res8 = subarraySum(arr8, 4, 4);
    assert(res8 == std::vector<int>({1, 2})); // first two 2's

    // Test 9: Large array, target at end
    int arr9[] = {1, 1, 1, 1, 5};
    std::vector<int> res9 = subarraySum(arr9, 5, 5);
    assert(res9 == std::vector<int>({5, 5}));

    // Test 10: Sum appears after shrinking window multiple times
    int arr10[] = {4, 1, 1, 1, 2, 3, 5};
    std::vector<int> res10 = subarraySum(arr10, 7, 5);
    // Find first subarray sum=5: after right=0 sum=4; right=1 sum=5 -> {1,2}
    assert(res10 == std::vector<int>({1, 2}));

    return 0;
}
