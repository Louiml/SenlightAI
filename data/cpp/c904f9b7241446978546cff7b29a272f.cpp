// Write a C++ function that, given a vector of positive integers (subject marks) and an integer I (1-indexed), returns the I-th smallest sum among all possible non-empty contiguous subarrays of the marks vector. The function should handle duplicates correctly, meaning that equal sums from different subarrays count separately when determining the I-th smallest. The input vector may contain any positive integers, and I is guaranteed to be between 1 and the total number of subarrays (i.e., n*(n+1)/2). Return the integer value of that I-th smallest sum.
The solution uses binary search on the range of possible subarray sums, combined with a counting function that determines how many subarray sums are less than or equal to a given threshold. The possible sums range from the minimum element (since all elements are positive, the smallest subarray is a single element) to the total sum of all elements. For a candidate `mid`, we use a two-pointer sliding window technique to count subarrays whose sum is ≤ `mid`. The key observation is that if we maintain a window `[left, right]` where the sum is ≤ `mid`, then for each right endpoint, all subarrays ending at `right` and starting anywhere from `left` to `right` are valid, adding `right - left + 1` to the count. If the count of subarrays with sum ≤ `mid` is less than `I`, we need a larger threshold; otherwise, we can try a smaller one. Binary search converges to the smallest `mid` such that at least `I` subarrays have sum ≤ `mid`, which is exactly the I-th smallest sum. Edge cases include duplicate marks (where identical sums appear multiple times) and subarrays with identical sums being counted separately. Time complexity is O(n log S), where n is the array length and S is the total sum (since binary search runs in O(log S) with each counting pass O(n)). Space complexity is O(1) auxiliary.
#include <vector>
#include <algorithm>

// Counts the number of subarrays with sum <= limit using two-pointer sliding window.
int countSubarraysLEQ(const std::vector<int>& marks, int limit) {
    int count = 0;
    int sum = 0;
    int left = 0;
    int n = static_cast<int>(marks.size());
    for (int right = 0; right < n; ++right) {
        sum += marks[right];
        while (sum > limit) {
            sum -= marks[left];
            ++left;
        }
        count += right - left + 1;
    }
    return count;
}

// Returns the I-th smallest sum among all non-empty contiguous subarrays of marks.
// The marks vector contains positive integers, and I is 1-indexed.
int kthSmallestSubarraySum(const std::vector<int>& marks, int I) {
    int n = static_cast<int>(marks.size());
    if (n == 0) return 0;

    int low = *std::min_element(marks.begin(), marks.end());
    int high = 0;
    for (int value : marks) {
        high += value;
    }

    while (low < high) {
        int mid = low + (high - low) / 2;
        int count = countSubarraysLEQ(marks, mid);
        if (count < I) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    return low;
}
#include <cassert>
#include <vector>
#include <iostream>

// Include the solution functions here (copy from above)

int main() {
    // Sample test 1
    std::vector<int> marks1 = {3, 2, 4};
    assert(kthSmallestSubarraySum(marks1, 1) == 2);
    assert(kthSmallestSubarraySum(marks1, 2) == 3);
    assert(kthSmallestSubarraySum(marks1, 3) == 4);
    assert(kthSmallestSubarraySum(marks1, 4) == 5);
    assert(kthSmallestSubarraySum(marks1, 5) == 6);
    assert(kthSmallestSubarraySum(marks1, 6) == 9);

    // Sample test 2
    std::vector<int> marks2 = {2, 2, 4, 4};
    assert(kthSmallestSubarraySum(marks2, 1) == 2);
    assert(kthSmallestSubarraySum(marks2, 2) == 2);
    assert(kthSmallestSubarraySum(marks2, 3) == 4);
    assert(kthSmallestSubarraySum(marks2, 7) == 8);  // Provided sample
    assert(kthSmallestSubarraySum(marks2, 8) == 8);
    assert(kthSmallestSubarraySum(marks2, 10) == 8);

    // Single element
    std::vector<int> marks3 = {7};
    assert(kthSmallestSubarraySum(marks3, 1) == 7);

    // All identical elements
    std::vector<int> marks4 = {5, 5, 5};
    // Subarray sums: 5,5,5,10,10,15
    assert(kthSmallestSubarraySum(marks4, 1) == 5);
    assert(kthSmallestSubarraySum(marks4, 2) == 5);
    assert(kthSmallestSubarraySum(marks4, 3) == 5);
    assert(kthSmallestSubarraySum(marks4, 4) == 10);
    assert(kthSmallestSubarraySum(marks4, 5) == 10);
    assert(kthSmallestSubarraySum(marks4, 6) == 15);

    // Larger array
    std::vector<int> marks5 = {1, 3, 2, 4};
    // Subarray sums: 1,3,2,4,4,5,6,6,9,10
    assert(kthSmallestSubarraySum(marks5, 1) == 1);
    assert(kthSmallestSubarraySum(marks5, 2) == 2);
    assert(kthSmallestSubarraySum(marks5, 3) == 3);
    assert(kthSmallestSubarraySum(marks5, 10) == 10);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
