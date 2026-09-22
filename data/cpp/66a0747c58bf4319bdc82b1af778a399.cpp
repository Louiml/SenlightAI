/*
Write a C++ function `std::vector<int> generateCombinations(int n, int m, bool isCombination)` that returns a vector representing the **first** element in lexicographic order of either: (1) all combinations of `n` distinct numbers chosen from `0` to `m-1` (inclusive) when `isCombination == true`, or (2) all sequences of length `n` where each element is in the range `[0, m-1]` (repetition allowed) when `isCombination == false`. The function should handle the following edge cases: for combinations, if `n > m`, it should return an empty vector (since no valid combination exists); for sequences, it should always return `{0, 0, ..., 0}` (length `n`) as the first element, even if `n` or `m` is zero (in which case the vector may be empty if `n == 0`). The first element is defined as the lexicographically smallest valid array: for combinations, it’s `[0, 1, 2, ..., n-1]`; for sequences, it’s all zeros. The function must not use any global state and must be self-contained.
*/

#include <vector>

// Return the first lexicographic element of either combinations (n distinct from 0..m-1)
// or sequences (length n with values from 0..m-1).
// For combinations, returns {} if n > m; for sequences, returns {} if m == 0 and n > 0.
// If n == 0, returns {}.
std::vector<int> generateCombinations(int n, int m, bool isCombination) {
    if (n < 0 || m < 0) return {};
    if (n == 0) return {};
    if (isCombination) {
        if (n > m) return {};
        std::vector<int> result(n);
        for (int i = 0; i < n; ++i) result[i] = i;
        return result;
    } else {
        if (m == 0) return {};
        return std::vector<int>(n, 0);
    }
}

#include <cassert>
#include <vector>

int main() {
    // Combinations valid
    std::vector<int> c1 = generateCombinations(3, 5, true);
    assert(c1 == std::vector<int>({0, 1, 2}));
    
    // Combinations invalid (n > m)
    std::vector<int> c2 = generateCombinations(5, 3, true);
    assert(c2.empty());
    
    // Sequences valid
    std::vector<int> s1 = generateCombinations(4, 3, false);
    assert(s1 == std::vector<int>({0, 0, 0, 0}));
    
    // Sequences with m = 0 and n > 0 → empty
    std::vector<int> s2 = generateCombinations(2, 0, false);
    assert(s2.empty());
    
    // n = 0 → empty for both
    std::vector<int> e1 = generateCombinations(0, 5, true);
    assert(e1.empty());
    std::vector<int> e2 = generateCombinations(0, 5, false);
    assert(e2.empty());
    
    // Sequences with n = 1
    std::vector<int> s3 = generateCombinations(1, 1, false);
    assert(s3 == std::vector<int>({0}));
    
    // Combinations with n = 1 and m = 1
    std::vector<int> c3 = generateCombinations(1, 1, true);
    assert(c3 == std::vector<int>({0}));
}

// The solution directly constructs the "first" enumeration state without iterating through all possibilities. For combinations, the lexicographically smallest array of `n` distinct numbers from `0` to `m-1` is simply `[0, 1, ..., n-1]`, but this is valid only if `n <= m`. If `n > m`, no such set exists, so return an empty vector. For sequences, the smallest array is all zeros `[0, 0, ..., 0]`, regardless of `m` (as long as `m >= 0`). However, if `n == 0`, the vector is naturally empty, and if `m == 0` but `n > 0`, there are no valid sequences (since no value can be chosen), so the function should also return an empty vector. Important edge cases: `n == 0` returns empty for both cases; `m == 0` and `n > 0` returns empty for sequences; combinations require `n <= m` and `n >= 0`. The time complexity is O(n) to build the vector, and space complexity is O(n) for the result. No iteration is needed, making it extremely efficient.
