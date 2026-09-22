// Write a C++ function `string getPermutation(int n, int k)` that returns the `k`-th permutation sequence of the numbers from `1` to `n`, where permutations are ordered lexicographically (i.e., in the order they would appear in a dictionary). The function must accept `1 ≤ n ≤ 9` and `1 ≤ k ≤ n!` and return the permutation as a string with no separators. For example, for `n = 3`, the permutations in lexicographic order are `"123", "132", "213", "231", "312", "321"`, so `getPermutation(3, 3)` should return `"213"`. You must not generate all permutations; instead, compute the result directly using factorial-based positioning. Ensure the function handles edge cases like `n = 1` and `k = n!` correctly.
The approach is based on determining each digit of the result one at a time using factorial values. Start with a vector of available digits `['1', '2', ..., 'n']` and an index `k` (1‑based). For each position from left to right, there are `(remaining_digits - 1)!` permutations that start with any given digit. Compute the block size as `factorial(remaining_digits - 1)`. The index within the sorted available digits is `(k - 1) / block_size`. Append that digit to the result, remove it from the vector, and update `k` to `k - (block_size * index)`, which is the position within the remaining block. Recurse until no digits remain. Edge cases: when `n = 1`, the only digit is appended immediately; when `k = n!`, the algorithm picks the last digit first (since `(k-1)` is `n! - 1`, index becomes `n-1`). Time complexity is `O(n^2)` due to vector erase operations (each erase shifts elements) and recursive depth `O(n)`. Space complexity is `O(n)` for the vector and call stack, excluding the output string.
#include <string>
#include <vector>
#include <numeric>

// Compute factorial of a non-negative integer.
int factorial(int x) {
    int result = 1;
    for (int i = 2; i <= x; ++i) {
        result *= i;
    }
    return result;
}

// Return the k-th lexicographic permutation of numbers 1..n (1-indexed k).
std::string getPermutation(int n, int k) {
    std::vector<char> digits;
    digits.reserve(n);
    for (int i = 1; i <= n; ++i) {
        digits.push_back(static_cast<char>('0' + i));
    }
    
    std::string result;
    result.reserve(n);
    
    // k is 1-based; convert to 0-based index for easier math.
    --k;
    
    while (!digits.empty()) {
        int block_size = factorial(static_cast<int>(digits.size()) - 1);
        int index = k / block_size;
        result.push_back(digits[index]);
        digits.erase(digits.begin() + index);
        k %= block_size;
    }
    
    return result;
}
#include <cassert>
#include <string>

// Declare the function (assume it is defined above or in the same translation unit).
std::string getPermutation(int n, int k);

int main() {
    // Basic cases
    assert(getPermutation(1, 1) == "1");
    assert(getPermutation(2, 1) == "12");
    assert(getPermutation(2, 2) == "21");
    
    // n=3 full sequence
    assert(getPermutation(3, 1) == "123");
    assert(getPermutation(3, 2) == "132");
    assert(getPermutation(3, 3) == "213");
    assert(getPermutation(3, 4) == "231");
    assert(getPermutation(3, 5) == "312");
    assert(getPermutation(3, 6) == "321");
    
    // n=4 spot checks
    assert(getPermutation(4, 1) == "1234");
    assert(getPermutation(4, 6) == "1432");
    assert(getPermutation(4, 24) == "4321");
    
    // n=5, last permutation
    assert(getPermutation(5, 120) == "54321");
    
    // n=6, some middle value (k=400)
    // The 400th permutation of 1..6 (total 720)
    assert(getPermutation(6, 400) == "365241");
    
    return 0;
}
