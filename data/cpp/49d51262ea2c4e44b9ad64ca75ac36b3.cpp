Write a C++ function `numsSameConsecDiff(int n, int k)` that returns a vector of all integers of exactly `n` digits (with no leading zeros, except for the single-digit number 0 when `n == 1`) where the absolute difference between every two consecutive digits is exactly `k`. For example, when `n = 3` and `k = 7`, the valid numbers include `181` (|1-8| = 7, |8-1| = 7) and `292` but not `180` (since |1-8|=7 but |8-0|=8). The returned vector should be sorted in ascending order, and each integer must have exactly `n` digits (no padding zeros), except `0` itself is allowed if `n == 1`. Edge cases include `k = 0` (where each digit must equal the previous digit, e.g., `111` for `n = 3`), `n = 1` (return all single digits 0-9), and large `n` values up to 9 (since 10^9 - 1 is the maximum int range). Your function must handle `1 ≤ n ≤ 9` and `0 ≤ k ≤ 9`. Assume no input validation is needed beyond these constraints.
// The solution uses a depth-first search (DFS) that builds numbers digit by digit. Starting from each possible leading digit `1` through `9` (or `0` when `n == 1`), the DFS appends valid next digits based on the current last digit. For a current number ending with digit `d`, the next digit must be either `d - k` (if `d >= k`) or `d + k` (if `k > 0` and `d + k <= 9`). The DFS proceeds recursively until the desired length `n` is reached, at which point the fully constructed number is added to the result vector. Because the DFS always adds digits in increasing order (first the smaller candidate `d-k` if valid, then the larger candidate `d+k`), the resulting numbers are generated in ascending order across all starting digits and recursive branches, so no explicit sorting is needed. Important edge cases include: when `k = 0`, the two candidate conditions become identical (`d - 0 = d` and `d + 0 = d`), so special care is taken to avoid adding the same digit twice by only recursing on the `d-k` branch when `k == 0`; when `n == 1`, the result must include `0` separately because the loop starts at `1`. The recursion depth is at most `n`, and each state has at most two branches, so time complexity is `O(10 * 2^(n-1))`, which simplifies to `O(2^n)` in the worst case, and space complexity is `O(2^n)` for storing results plus `O(n)` recursion stack.
#include <vector>

// Return all integers with exactly n digits where the absolute difference
// between consecutive digits is exactly k, sorted in ascending order.
std::vector<int> numsSameConsecDiff(int n, int k) {
    std::vector<int> result;
    if (n == 1) {
        result.push_back(0);
    }
    for (int startDigit = 1; startDigit <= 9; ++startDigit) {
        // Depth-first search to build the number digit by digit.
        auto buildNumber = [&](int current, int remainingDigits, auto&& dfs) -> void {
            if (remainingDigits == 0) {
                result.push_back(current);
                return;
            }
            int lastDigit = current % 10;
            // Try the smaller valid next digit (lastDigit - k).
            if (lastDigit >= k) {
                dfs(current * 10 + (lastDigit - k), remainingDigits - 1, dfs);
            }
            // Try the larger valid next digit (lastDigit + k), but skip duplicates when k == 0.
            if (k > 0 && lastDigit + k <= 9) {
                dfs(current * 10 + (lastDigit + k), remainingDigits - 1, dfs);
            }
        };
        buildNumber(startDigit, n - 1, buildNumber);
    }
    return result;
}
#include <cassert>
#include <vector>

// The function declaration (in actual code, this would be in a header).
std::vector<int> numsSameConsecDiff(int n, int k);

int main() {
    // n = 1, any k -> digits 0-9
    assert(numsSameConsecDiff(1, 0) == std::vector<int>({0,1,2,3,4,5,6,7,8,9}));
    assert(numsSameConsecDiff(1, 5) == std::vector<int>({0,1,2,3,4,5,6,7,8,9}));
    
    // n = 2, k = 0 -> repeated digit numbers
    assert(numsSameConsecDiff(2, 0) == std::vector<int>({11,22,33,44,55,66,77,88,99}));
    
    // n = 3, k = 7 -> typical example
    std::vector<int> expected3_7 = {181,292,707,818,929};
    assert(numsSameConsecDiff(3, 7) == expected3_7);
    
    // n = 2, k = 1 -> consecutive pairs
    std::vector<int> expected2_1 = {10,12,21,23,32,34,43,45,54,56,65,67,76,78,87,89,98};
    assert(numsSameConsecDiff(2, 1) == expected2_1);
    
    // n = 4, k = 0 -> all repeated digits
    std::vector<int> expected4_0;
    for (int d = 1; d <= 9; ++d) {
        expected4_0.push_back(d * 1111);
    }
    assert(numsSameConsecDiff(4, 0) == expected4_0);
    
    // n = 2, k = 9 -> only pairs (9,0) valid
    assert(numsSameConsecDiff(2, 9) == std::vector<int>({90}));
    
    // n = 3, k = 1 -> specific check
    std::vector<int> result = numsSameConsecDiff(3, 1);
    // Verify that all results are sorted and have exactly 3 digits
    for (size_t i = 1; i < result.size(); ++i) {
        assert(result[i] > result[i-1]);
        assert(result[i] >= 100 && result[i] <= 999);
    }
    
    // n = 9, k = 0 -> one valid number: 999999999
    assert(numsSameConsecDiff(9, 0) == std::vector<int>({999999999}));
    
    // n = 2, k = 5 -> valid pairs
    std::vector<int> expected2_5 = {16,27,38,49,50,61,72,83,94};
    assert(numsSameConsecDiff(2, 5) == expected2_5);
    
    return 0;
}
