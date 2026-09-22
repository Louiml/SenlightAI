Write a C++ function `int sumOfSubsetXORTotals(const std::vector<int>& nums)` that, given a non-empty vector of integers (`1 <= nums.size() <= 12`, `1 <= nums[i] <= 20`), returns the sum of the XOR totals of every subset of the vector. The XOR total of a subset is the bitwise XOR of all its elements, with the empty subset having XOR total 0. Subsets with the same elements but formed in different ways (which does not happen here since subsets are based on index positions, but duplicates in the input count as distinct elements) must be counted separately. For example, if the input is `{1,3}`, the subsets are `{}` (0), `{1}` (1), `{3}` (3), and `{1,3}` (2), giving a sum of 6. The function should be efficient and avoid generating all subsets explicitly (though a recursive or bitmask approach is acceptable); a bitwise optimization based on the fact that each bit’s contribution depends on the OR of all numbers is expected. The function must handle vectors of length up to 12, where the maximum possible sum fits within a 32-bit signed integer. Provide a clean, self-contained implementation with no external dependencies beyond the standard library.
// The key insight is that the XOR total of a subset is determined bit by bit. For any bit position `k` (0 to 30, since values are ≤ 20), consider how many subsets have that bit set in their XOR total. The contribution of bit `k` to the total sum is `(count of subsets with bit k set) * (2^k)`. To count subsets with bit `k` set, observe that in a subset, bit `k` of the XOR is set if and only if an odd number of selected elements have bit `k` set. If there are `c` numbers in the input where bit `k` is set, and `n` numbers where it is not, then: the number of subsets selecting an odd number from the `c` set-containing numbers is `2^(c-1)` (half of all subsets of those `c` numbers), and the selection from the `n` numbers not having bit `k` can be arbitrary (`2^n` ways). Thus, the total count for bit `k` is `2^(c-1) * 2^n = 2^(n-1)`, provided `c >= 1`; if `c == 0`, the count is 0. Therefore, bit `k` contributes `2^k * 2^(n-1)` for every bit that appears in at least one element. This means the total sum equals `(OR of all numbers) * 2^(n-1)`. For `n = 0` (not possible per constraints), the formula would give `OR * 2^(-1)` which is invalid, but `n >= 1` always. Edge case: if `n = 1`, the sum is just the single element (since subsets: empty and the element itself, sum = 0 + element = element), and `OR * 2^(0) = OR = element` holds. The algorithm computes the bitwise OR of all elements in `O(n)` time and left-shifts by `n-1` (which is equivalent to multiplying by `2^(n-1)`). Time complexity is `O(n)`, space complexity is `O(1)`. This solution is optimal and avoids enumeration of up to `2^12 = 4096` subsets.
#include <vector>
#include <cstddef>

// Computes the sum of XOR totals of all non-empty subsets of nums.
// The formula used: (bitwise OR of all elements) * 2^(n-1).
// This works because each bit contributes independently, and every bit present in any number
// appears in exactly half of all subsets (2^(n-1) subsets).
int sumOfSubsetXORTotals(const std::vector<int>& nums) {
    int bitwiseOr = 0;
    for (int num : nums) {
        bitwiseOr |= num;
    }
    // Left shift by (nums.size() - 1) multiplies by 2^(n-1).
    // nums.size() is guaranteed >= 1, so the shift is valid.
    return bitwiseOr << (nums.size() - 1);
}
#include <cassert>
#include <vector>

int sumOfSubsetXORTotals(const std::vector<int>& nums);

int main() {
    // Example 1 from the problem statement
    assert(sumOfSubsetXORTotals({1, 3}) == 6);
    // Example 2
    assert(sumOfSubsetXORTotals({5, 1, 6}) == 28);
    // Example 3
    assert(sumOfSubsetXORTotals({3, 4, 5, 6, 7, 8}) == 480);
    // Single element: sums to the element itself
    assert(sumOfSubsetXORTotals({7}) == 7);
    // Two identical elements: subsets are {}, {2}, {2}, {2,2}=0 -> sum 4
    assert(sumOfSubsetXORTotals({2, 2}) == 4);
    // All elements share no bits (1 and 2): OR = 3, n=2 -> 3 * 2 = 6
    assert(sumOfSubsetXORTotals({1, 2}) == 6);
    // Larger values within constraints
    assert(sumOfSubsetXORTotals({20, 19, 18}) == (20 | 19 | 18) << 2);
    // Edge: n=12, all ones (value 1) -> OR=1, sum = 2^11 = 2048
    assert(sumOfSubsetXORTotals(std::vector<int>(12, 1)) == 2048);
    // Test with zeros not allowed by constraints, but check logic: {0} -> sum 0
    // (not per constraints but for completeness)
    assert(sumOfSubsetXORTotals({0}) == 0);
    return 0;
}
