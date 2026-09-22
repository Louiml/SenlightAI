Given a non-negative integer `n`, write a C++ function that returns a vector of integers `ans` of length `n + 1` such that for each index `i` (0 ≤ i ≤ n), `ans[i]` is the number of 1-bits (population count) in the binary representation of `i`. For example, if `n = 5`, the output should be `{0, 1, 1, 2, 1, 2}` because the binary representations are 0 (0 bits), 1 (1 bit), 10 (1 bit), 11 (2 bits), 100 (1 bit), 101 (2 bits). The function must compute the result efficiently without using built‑in bit‑count functions or iterating over each bit individually for every number. Implement the solution using the recurrence relation based on clearing the lowest set bit.
// The key insight is the recurrence: for any positive integer `i`, the number of 1‑bits in `i` equals the number of 1‑bits in `i & (i - 1)` plus one. The expression `i & (i - 1)` clears the lowest set bit of `i`. For example, `i = 6` (binary `110`) → `i & 5` = `110 & 101` = `100` (4), which is `6` with its lowest set bit removed. Since `i & (i - 1) < i` for all `i > 0`, we can compute the answer for `i` after the answer for all smaller indices is already known. This allows a single forward loop from `1` to `n`, building the vector iteratively. 
//
// Base case: `ans[0] = 0` because zero has no set bits. For each `i` from 1 to `n`, set `ans[i] = ans[i & (i - 1)] + 1`. The loop runs in `O(n)` time because each iteration performs constant‑time arithmetic and array access. The space complexity is `O(n)` for the output vector. Edge cases: `n = 0` returns `{0}`; the function must handle this without accessing out‑of‑bounds indices. Since `i & (i - 1)` is always less than `i`, the recurrence is always well‑defined and the computed indices are always previously filled.
#include <vector>

// Given a non-negative integer n, return a vector of length n+1
// where ans[i] is the number of 1-bits in the binary representation of i.
std::vector<int> countBits(const int n) {
    std::vector<int> ans(n + 1, 0); // ans[0] = 0 is automatically set
    for (int i = 1; i <= n; ++i) {
        // Clear the lowest set bit of i, get the count for that smaller number, then add 1
        ans[i] = ans[i & (i - 1)] + 1;
    }
    return ans;
}
#include <cassert>
#include <vector>

// The solution function is declared above; here we test it.
int main() {
    // Test n = 0
    std::vector<int> result0 = countBits(0);
    assert(result0.size() == 1);
    assert(result0[0] == 0);

    // Test n = 1
    std::vector<int> result1 = countBits(1);
    assert(result1.size() == 2);
    assert(result1[0] == 0 && result1[1] == 1);

    // Test n = 2
    std::vector<int> result2 = countBits(2);
    assert(result2 == std::vector<int>({0, 1, 1}));

    // Test n = 5 (example from the description)
    std::vector<int> result5 = countBits(5);
    assert(result5 == std::vector<int>({0, 1, 1, 2, 1, 2}));

    // Test n = 8
    std::vector<int> result8 = countBits(8);
    assert(result8 == std::vector<int>({0, 1, 1, 2, 1, 2, 2, 3, 1}));

    // Test a larger value, e.g., n = 16
    std::vector<int> result16 = countBits(16);
    assert(result16 == std::vector<int>({0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1}));

    // Test that all values are non-negative (trivial) and sum to known value for n=3
    std::vector<int> result3 = countBits(3);
    assert(result3 == std::vector<int>({0, 1, 1, 2}));

    return 0;
}
