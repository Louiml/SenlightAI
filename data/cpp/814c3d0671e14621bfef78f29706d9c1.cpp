// Write a C++ function `long long kthSmallestProduct(vector<int>& nums1, vector<int>& nums2, long long k)` that returns the k-th smallest product among all pairs `(nums1[i], nums2[j])`. Both input vectors are sorted in non-decreasing order and may contain negative, zero, and positive integers. The length of each vector is between 1 and 5·10⁴, and k is between 1 and the total number of pairs. The product values can be large, so use 64-bit integers. The function must not use any external libraries beyond standard ones and must handle edge cases such as zeros, all-negative arrays, and mixed signs.
We solve this using binary search over the product value. The answer lies in the range `[-1e10, 1e10]` because the maximum absolute value of any element is at most 10⁴, and the product of two such elements is at most 10⁸, but the given constraints allow up to 10⁴ each, so the product range is `[-1e8, 1e8]`. However, to be safe, we can use `-1e10` to `1e10` as bounds. For a candidate product `mid`, we count how many pairs have product ≤ `mid`. If that count is ≥ k, then `mid` is a candidate and we search the left half; otherwise, we search the right half. The counting function must handle both positive and negative elements of `nums1` separately because for a positive `c`, the numbers in `nums2` that satisfy `c * nums2[j] ≤ mid` are those up to `floor(mid / c)`, and we can use binary search in the sorted `nums2`. For negative `c`, the product inequality reverses: `c * nums2[j] ≤ mid` becomes `nums2[j] ≥ ceil(mid / c)` (since dividing by a negative flips the inequality), so the valid numbers are from some index to the end, and we can compute the count as `nums2.size() - lower_bound`. We must be careful with integer division and negative numbers: use `long long` for all computations, and prefer using `__int128` for multiplication to avoid overflow, but since `c` up to 10⁴ and `nums2[j]` up to 10⁴, `c * nums2[j]` fits in `long long` (max 1e8). However, in the binary search condition, we compare `1LL * nums2[m] * c ≤ mid`, which is safe. The counting function runs in O(n log m) per call where n = nums1.size() and m = nums2.size(). The binary search over product values runs in O(log(range)) ~ 35 iterations. Total time complexity O(n log m log range), which is acceptable for the constraints. Space complexity O(1) aside from input. Edge cases: zero in nums1 or nums2: if c = 0, then product is 0 for all j, so count is either all or none depending on mid. Also, when c is negative, we must correctly invert the inequality. The binary search on `nums2` for negative c uses a different condition: we need to find the first index where `nums2[index] * c > mid`, and then the count is `size - index`. We can use the standard `upper_bound` or manual binary search with adjusted logic.
#include <vector>
#include <algorithm>
#include <cstdint>

long long kthSmallestProduct(std::vector<int>& nums1, std::vector<int>& nums2, long long k) {
    auto count_leq = [&](long long x) -> long long {
        long long total = 0;
        for (int c : nums1) {
            if (c == 0) {
                // product is always 0, which is ≤ x only if x >= 0
                if (x >= 0) total += nums2.size();
            } else if (c > 0) {
                // Need nums2[j] <= floor(x / c)
                // Find last index with nums2[idx] * c <= x
                long long limit = x / c; // integer division truncates toward zero for positive c, but since x and c can be negative, careful. Use floor division.
                // In C++, integer division truncates toward zero, so for positive c and possibly negative x, this is floor division? No. We need floor(x/c) for correctness.
                // Simpler: binary search directly on product condition
                auto it = std::upper_bound(nums2.begin(), nums2.end(), x, [&](long long val, int elem) {
                    return 1LL * elem * c > val;
                });
                // This is tricky. Let's just do manual binary search.
                int l = 0, r = nums2.size() - 1, pos = -1;
                while (l <= r) {
                    int m = l + (r - l) / 2;
                    if (1LL * nums2[m] * c <= x) {
                        pos = m;
                        l = m + 1;
                    } else {
                        r = m - 1;
                    }
                }
                total += (pos + 1);
            } else { // c < 0
                // Need nums2[j] >= ceil(x / c)
                // Because multiplying by negative flips inequality: c * nums2[j] <= x  =>  nums2[j] >= x / c (but careful with floor/ceil)
                // Use binary search to find first index with nums2[idx] * c > x
                int l = 0, r = nums2.size() - 1, pos = nums2.size();
                while (l <= r) {
                    int m = l + (r - l) / 2;
                    if (1LL * nums2[m] * c > x) {
                        pos = m;
                        r = m - 1;
                    } else {
                        l = m + 1;
                    }
                }
                total += (nums2.size() - pos);
            }
        }
        return total;
    };

    long long low = -10000000000LL;
    long long high = 10000000000LL;
    long long answer = low;
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (count_leq(mid) >= k) {
            answer = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return answer;
}
#include <cassert>
#include <vector>

long long kthSmallestProduct(std::vector<int>& nums1, std::vector<int>& nums2, long long k);

int main() {
    std::vector<int> a1 = {2, 5};
    std::vector<int> b1 = {3, 4};
    assert(kthSmallestProduct(a1, b1, 1) == 6);  // 2*3
    assert(kthSmallestProduct(a1, b1, 2) == 8);  // 2*4
    assert(kthSmallestProduct(a1, b1, 3) == 15); // 5*3
    assert(kthSmallestProduct(a1, b1, 4) == 20); // 5*4

    std::vector<int> a2 = {-4, -2, 0, 3};
    std::vector<int> b2 = {2, 4};
    // All products: -8,-16,-4,-8,0,0,6,12, sorted: -16,-8,-8,-4,0,0,6,12
    assert(kthSmallestProduct(a2, b2, 1) == -16);
    assert(kthSmallestProduct(a2, b2, 3) == -8);
    assert(kthSmallestProduct(a2, b2, 5) == 0);
    assert(kthSmallestProduct(a2, b2, 8) == 12);

    std::vector<int> a3 = {-5, -1};
    std::vector<int> b3 = {2, 3};
    // Products: -10,-15,-2,-3 sorted: -15,-10,-3,-2
    assert(kthSmallestProduct(a3, b3, 1) == -15);
    assert(kthSmallestProduct(a3, b3, 4) == -2);

    std::vector<int> a4 = {1, 1};
    std::vector<int> b4 = {1, 1};
    // All products are 1
    assert(kthSmallestProduct(a4, b4, 1) == 1);
    assert(kthSmallestProduct(a4, b4, 4) == 1);

    std::vector<int> a5 = {0};
    std::vector<int> b5 = {-100, 100};
    // Both products are 0
    assert(kthSmallestProduct(a5, b5, 1) == 0);
    assert(kthSmallestProduct(a5, b5, 2) == 0);

    // Large values
    std::vector<int> a6 = {10000, 10000};
    std::vector<int> b6 = {10000, 10000};
    assert(kthSmallestProduct(a6, b6, 1) == 100000000);
    assert(kthSmallestProduct(a6, b6, 4) == 100000000);

    std::vector<int> a7 = {-10000};
    std::vector<int> b7 = {-10000, 10000};
    // Products: 100000000, -100000000
    assert(kthSmallestProduct(a7, b7, 1) == -100000000);
    assert(kthSmallestProduct(a7, b7, 2) == 100000000);

    return 0;
}
