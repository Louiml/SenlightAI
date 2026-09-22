Write a C++ function `long long countRangeSum(vector<int>& nums, int lower, int upper)` that, given an integer array `nums` and two integer bounds `lower` and `upper` (with `lower <= upper`), returns the number of contiguous subarrays whose sum lies within the inclusive range `[lower, upper]`. A subarray is defined by two indices `i` and `j` (0 ≤ i ≤ j < n) with sum `nums[i] + ... + nums[j]`. The input array may contain negative numbers, zero, and large positive values, so the function must handle up to size 10^5 with prefix sums that can exceed 32-bit integers. The result may exceed the range of a 32-bit integer, so return a `long long`. You must not use any built-in range-sum query structure; instead, implement the count using a merge-sort-based divide-and-conquer approach on prefix sums.

The solution uses the classic technique of transforming subarray sum queries into prefix-sum differences. Let `prefix[0] = 0` and `prefix[i] = sum(nums[0..i-1])` for `i = 1..n`. Then the sum of subarray from index `i` to `j` (inclusive) equals `prefix[j+1] - prefix[i]`. So we need to count pairs `(i, j)` with `0 <= i < j <= n` such that `lower <= prefix[j] - prefix[i] <= upper`. Direct enumeration is O(n²) and too slow for `n = 10^5`. Instead, we adapt merge sort: the prefix-sum array is recursively split. In the merge step, when combining two sorted halves, we count how many pairs have one index from the left half and one from the right half such that their difference falls in the range. Since both halves are already sorted, for each left element `prefix[i]`, we find the first index `lo` in the right half where `prefix[lo] - prefix[i] >= lower` and the first index `hi` where `prefix[hi] - prefix[i] > upper` (note careful inclusive-exclusive boundaries). Then `hi - lo` gives the number of right elements that satisfy the range for that left element. We accumulate counts from the left, right, and cross pairs. After counting, we merge the two sorted halves to maintain order for the upper levels. Important edge cases: `nums` may be empty (return 0, since no subarray exists), `lower` and `upper` may be equal, negative numbers cause prefix sums to be non-monotonic, and the count may overflow 32-bit. Also, the initial call must include the prefix index 0 (representing empty prefix). Time complexity is O(n log n) from merge sort, and auxiliary space is O(n) for the temporary merge array and recursion stack. Space for the prefix array is O(n). All boundary checks must use `long long` to avoid overflow when subtracting prefix sums.

#include <vector>
#include <cstdint>

// Counts the number of subarrays of nums whose sum is within [lower, upper].
// Uses divide-and-conquer merge sort on prefix sums.
long long countRangeSum(std::vector<int>& nums, int lower, int upper) {
    const int n = static_cast<int>(nums.size());
    if (n == 0) return 0;

    // Build prefix sums (1-indexed: prefix[0] = 0, prefix[i] = sum of first i elements)
    std::vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + nums[i];
    }

    long long lowerBound = static_cast<long long>(lower);
    long long upperBound = static_cast<long long>(upper);

    // Helper merge function to merge two sorted halves of prefix in [l..r]
    auto merge = [&](int l, int mid, int r) {
        std::vector<long long> left(prefix.begin() + l, prefix.begin() + mid + 1);
        std::vector<long long> right(prefix.begin() + mid + 1, prefix.begin() + r + 1);
        
        int i = 0, j = 0, k = l;
        while (i < (int)left.size() && j < (int)right.size()) {
            if (left[i] <= right[j]) {
                prefix[k++] = left[i++];
            } else {
                prefix[k++] = right[j++];
            }
        }
        while (i < (int)left.size()) prefix[k++] = left[i++];
        while (j < (int)right.size()) prefix[k++] = right[j++];
    };

    // Recursive function that sorts prefix[l..r] and counts pairs (i, j) with l <= i < j <= r
    // whose prefix[j] - prefix[i] lies in [lower, upper]
    std::function<long long(int, int)> mergeSortCount = [&](int l, int r) -> long long {
        if (l >= r) return 0; // single element or empty range
        int mid = l + (r - l) / 2;
        long long count = mergeSortCount(l, mid) + mergeSortCount(mid + 1, r);

        // Count cross pairs: left indices in [l..mid], right indices in [mid+1..r]
        int lo = mid + 1, hi = mid + 1;
        for (int i = l; i <= mid; ++i) {
            // Find first right index where prefix[j] - prefix[i] >= lower
            while (lo <= r && prefix[lo] - prefix[i] < lowerBound) lo++;
            // Find first right index where prefix[j] - prefix[i] > upper
            while (hi <= r && prefix[hi] - prefix[i] <= upperBound) hi++;
            count += (hi - lo);
        }

        merge(l, mid, r);
        return count;
    };

    // The prefix array has indices 0..n. We need pairs (i, j) with 0 <= i < j <= n.
    return mergeSortCount(0, n);
}

#include <cassert>
#include <vector>

// The solution function is defined above (include its code here).

int main() {
    // Test 1: Basic positive range
    std::vector<int> nums1 = {1, 2, 3};
    assert(countRangeSum(nums1, 1, 3) == 3); // subarrays: [1], [2], [1,2] sums 1,2,3

    // Test 2: Negative bounds and negative numbers
    std::vector<int> nums2 = {-2, 5, -1};
    assert(countRangeSum(nums2, -2, 2) == 3); // [0,0]=-2, [2,2]=-1, [1,2]=4? no, range -2..2 so only -2 and -1 = 2? Let's compute: prefix: 0,-2,3,2. pairs: (0,2):3, (0,1):-2, (0,3):2, (1,2):5, (1,3):4, (2,3):-1 → valid: -2,2,-1 = 3

    // Test 3: Empty array
    std::vector<int> nums3;
    assert(countRangeSum(nums3, 0, 0) == 0);

    // Test 4: Single element inside range
    std::vector<int> nums4 = {5};
    assert(countRangeSum(nums4, 5, 5) == 1);

    // Test 5: Single element outside range
    std::vector<int> nums5 = {10};
    assert(countRangeSum(nums5, 1, 5) == 0);

    // Test 6: All zeros, large count
    std::vector<int> nums6(10, 0);
    assert(countRangeSum(nums6, 0, 0) == 55); // all subarrays sum 0, count = 10*11/2

    // Test 7: Large numbers to check overflow handling
    std::vector<int> nums7 = {1000000000, 1000000000, 1000000000};
    // sums: [1e9], [1e9], [1e9], [2e9], [2e9], [3e9]
    assert(countRangeSum(nums7, 2000000000LL, 3000000000LL) == 3); // three sums: 2e9,2e9,3e9

    // Test 8: Range with negative lower and positive upper mixed
    std::vector<int> nums8 = {-3, 1, 2, -1};
    // prefix: 0, -3, -2, 0, -1
    // pairs: (0,1): -3, (0,2):-2, (0,3):0, (0,4):-1, (1,2):1, (1,3):3, (1,4):2, (2,3):2, (2,4):1, (3,4):-1
    // range [-1,2] valid: -1,0,1,2,1,2,1,-1 → total 8? Wait let's count: -1 (0,4),0(0,3),1(1,2),2(1,4),2(2,3),1(2,4),-1(3,4) = 7? Need careful: list: -1,0,1,2,2,1,-1 → 7.
    assert(countRangeSum(nums8, -1, 2) == 7);

    // Test 9: Lower equals upper
    std::vector<int> nums9 = {1, -1, 2, -2};
    // prefix: 0,1,0,2,0
    // pairs with diff = 0: (0,2)=0, (0,4)=0, (1,3)=1? no, diff 0: prefix[2]-prefix[0]=0, prefix[4]-prefix[0]=0, prefix[3]-prefix[1]=1? no, prefix[4]-prefix[2]=0, prefix[3]-prefix[1]=1? Let's list all pairs: (0,1):1, (0,2):0, (0,3):2, (0,4):0, (1,2):-1, (1,3):1, (1,4):-1, (2,3):2, (2,4):0, (3,4):-2 → zeros at (0,2), (0,4), (2,4) = 3
    assert(countRangeSum(nums9, 0, 0) == 3);

    // Test 10: Mixed large positive/negative to stress
    std::vector<int> nums10 = {5, -4, 3, -2, 1};
    // Manually compute? Or use brute in test? We'll just pick known result from brute logic:
    // compute all subarray sums: 5,1,4,2,3, -4,-1,1,0, 3,1,2, -2,-1,1 → valid for range [-2,2]? Let's count: 1,2,1,0,0,1, -2,-1,1,1? Too many. Instead use a brute helper in test? For simplicity we just call and compare to a brute function.
    // Since we can write a brute in test, but the instruction says assert. We'll do a simple known count for range [-3,3].
    // Actually we'll trust our algorithm over a typical test. Let's just do a small brute using double loop in assert:
    long long expected = 0;
    for (size_t i = 0; i < nums10.size(); ++i) {
        long long sum = 0;
        for (size_t j = i; j < nums10.size(); ++j) {
            sum += nums10[j];
            if (sum >= -3 && sum <= 3) expected++;
        }
    }
    assert(countRangeSum(nums10, -3, 3) == expected);

    return 0;
}
