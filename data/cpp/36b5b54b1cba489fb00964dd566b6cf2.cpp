Given an array of `n` integers (2 ≤ n ≤ 10^5), create a C++ function `long long maximizeProduct(const std::vector<long long>& nums)` that returns the maximum product that can be obtained by selecting exactly two distinct elements from the array. The array contains both positive and negative integers, and there can be duplicate values. The function must handle the case where the two largest positive numbers may not give the maximum product if there are two negative numbers with large absolute values, since the product of two negative numbers is positive. The result may be as large as 10^10, so use `long long` for all calculations.

// The maximum product of two distinct elements can be found by considering only the two largest positive numbers and the two smallest (most negative) numbers. Sort the array in ascending order. The maximum product is the maximum of `nums[0] * nums[1]` (product of two smallest negatives) and `nums[n-1] * nums[n-2]` (product of two largest positives). This covers all cases:
// - If all numbers are positive, the two largest give the maximum.
// - If all numbers are negative, the two smallest (least negative) give the maximum product (since negative × negative = positive), which is `nums[0] * nums[1]` after sorting.
// - If there's a mix, we still compare both pairs.
// Edge cases: duplicate values are allowed, and the two elements must be at distinct indices; sorting naturally handles this because we take indices 0,1 and n-1,n-2 which are always distinct when n≥2. Also handle n=2 specially – both pairs are the same pair, and the answer is just `nums[0] * nums[1]`.
// Time complexity: O(n log n) due to sorting. Space complexity: O(1) auxiliary, excluding input storage.

#include <bits/stdc++.h>
using namespace std;

// Returns the maximum product of two distinct elements from the vector.
// Assumes vector has at least 2 elements.
long long maximizeProduct(const std::vector<long long>& nums) {
    int n = nums.size();
    std::vector<long long> sorted = nums; // make a copy
    std::sort(sorted.begin(), sorted.end());

    if (n == 2) {
        return sorted[0] * sorted[1];
    }

    long long productSmallest = sorted[0] * sorted[1];
    long long productLargest = sorted[n-1] * sorted[n-2];

    return std::max(productSmallest, productLargest);
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: mixed positives and negatives
    std::vector<long long> v1 = {1, 2, 3, 4};
    assert(maximizeProduct(v1) == 12); // 3*4

    // Test 2: all negative
    std::vector<long long> v2 = {-5, -1, -3};
    assert(maximizeProduct(v2) == 3); // -1 * -3

    // Test 3: contains zeros and negatives
    std::vector<long long> v3 = {-10, -2, 0, 1};
    assert(maximizeProduct(v3) == 20); // -10 * -2

    // Test 4: two elements only
    std::vector<long long> v4 = {7, -3};
    assert(maximizeProduct(v4) == -21);

    // Test 5: duplicate values
    std::vector<long long> v5 = {5, 5, -5, -5};
    assert(maximizeProduct(v5) == 25); // either 5*5 or -5*-5

    // Test 6: large values
    std::vector<long long> v6 = {100000, 99999, -100000, -99999};
    assert(maximizeProduct(v6) == 10000000000LL); // 100000*100000 or -100000*-100000

    // Test 7: with one negative and large positives
    std::vector<long long> v7 = {10, 20, -30};
    assert(maximizeProduct(v7) == 200); // 10*20

    // Test 8: with all zeros and one positive
    std::vector<long long> v8 = {0, 0, 5};
    assert(maximizeProduct(v8) == 0); // 0*5 or 0*0

    // Test 9: with one pair negative giving large product
    std::vector<long long> v9 = {2, 3, 4, -100, -99};
    assert(maximizeProduct(v9) == 9900); // -100 * -99

    // Test 10: with single negative and single positive
    std::vector<long long> v10 = {-1, 10};
    assert(maximizeProduct(v10) == -10);

    return 0;
}
