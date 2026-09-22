// Design a C++ class called `PrefixSum` that maintains a prefix sum array over a fixed sequence of integers. The class must expose two public methods: a constructor `PrefixSum(const std::vector<int>& nums)` that precomputes cumulative sums from the given input vector, and a member function `int rangeSum(int i, int j) const` that returns the sum of elements from index `i` to index `j` inclusive (where `0 ≤ i ≤ j < nums.size()`). The constructor should handle an empty input vector gracefully. The class must be self-contained, properly manage any dynamically allocated memory, and support const-correct usage. The solution should avoid recalculating sums from scratch for each query.

The key insight is to precompute a prefix sum array `prefix` where `prefix[k]` stores the sum of the first `k` elements (`nums[0]` through `nums[k-1]`). This allows any range sum from `i` to `j` to be computed in O(1) time using the formula `prefix[j+1] - prefix[i]`. The construction requires O(n) time and O(n) space for the prefix array (plus the input vector storage if copied). Edge cases include an empty input vector (constructor should handle it, perhaps by setting prefix size to 1 with `prefix[0]=0`). For a single-element vector, range sum works correctly for `i=j=0`. Invalid indices (out-of-range or `i > j`) are assumed not to occur per the problem constraints, but the implementation should not rely on undefined behavior for valid inputs. Dynamic memory management is unnecessary if using `std::vector<int>` for the prefix array, which automatically handles allocation and cleanup. Time per query is O(1); total time is O(n) for construction and O(q) for `q` queries. Space is O(n).

#include <vector>
#include <cstddef>  // for size_t

class PrefixSum {
private:
    std::vector<int> prefix;  // prefix[i] = sum of nums[0..i-1]

public:
    // Constructor: precompute prefix sums from the input vector.
    explicit PrefixSum(const std::vector<int>& nums) {
        prefix.resize(nums.size() + 1);
        prefix[0] = 0;
        for (size_t i = 0; i < nums.size(); ++i) {
            prefix[i + 1] = prefix[i] + nums[i];
        }
    }

    // Return sum of elements from index i to j inclusive.
    int rangeSum(int i, int j) const {
        // Assume valid input: 0 <= i <= j < nums.size()
        return prefix[j + 1] - prefix[i];
    }
};

#include <cassert>
#include <vector>

// The PrefixSum class is assumed to be in scope (e.g., from the solution above).
// We include the necessary headers here for the test.

int main() {
    // Test 1: Example from the problem statement
    std::vector<int> nums1 = {-2, 0, 3, -5, 2, -1};
    PrefixSum ps1(nums1);
    assert(ps1.rangeSum(0, 2) == 1);   // (-2)+0+3 = 1
    assert(ps1.rangeSum(2, 5) == -1);  // 3+(-5)+2+(-1) = -1
    assert(ps1.rangeSum(0, 5) == -3);  // sum of all = -3

    // Test 2: Single element
    std::vector<int> nums2 = {7};
    PrefixSum ps2(nums2);
    assert(ps2.rangeSum(0, 0) == 7);

    // Test 3: All positive numbers
    std::vector<int> nums3 = {1, 2, 3, 4, 5};
    PrefixSum ps3(nums3);
    assert(ps3.rangeSum(1, 3) == 2+3+4);   // 9
    assert(ps3.rangeSum(0, 4) == 15);
    assert(ps3.rangeSum(3, 3) == 4);

    // Test 4: All negative numbers
    std::vector<int> nums4 = {-5, -1, -10, -3};
    PrefixSum ps4(nums4);
    assert(ps4.rangeSum(0, 0) == -5);
    assert(ps4.rangeSum(1, 2) == -1 + -10);  // -11
    assert(ps4.rangeSum(0, 3) == -19);

    // Test 5: Empty vector (should not throw, rangeSum not called on empty)
    std::vector<int> nums5;
    PrefixSum ps5(nums5);
    // Construction should be fine; no range sum available since no valid indices.

    // Test 6: Larger random check with multiple queries
    std::vector<int> nums6 = {3, -2, 7, 0, -1, 5, 8, -4, 2};
    PrefixSum ps6(nums6);
    assert(ps6.rangeSum(2, 6) == 7 + 0 + (-1) + 5 + 8); // 19
    assert(ps6.rangeSum(0, 8) == 3 + (-2) + 7 + 0 + (-1) + 5 + 8 + (-4) + 2); // 18
    assert(ps6.rangeSum(4, 4) == -1);
    assert(ps6.rangeSum(7, 8) == -4 + 2); // -2

    return 0;
}
