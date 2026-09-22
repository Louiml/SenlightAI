// Write a standalone C++ function `getKthPermutation(int n, int k)` that returns the `k`-th permutation of the numbers `1` through `n`, where permutations are generated in lexicographical order. The parameters satisfy `1 <= n <= 9` and `1 <= k <= n!`. For example, for `n = 3`, the permutations in order are `"123"`, `"132"`, `"213"`, `"231"`, `"312"`, `"321"`, so `getKthPermutation(3, 1)` should return `"123"` and `getKthPermutation(3, 4)` should return `"231"`. The function should return a `std::string` containing the digits with no separators. Do not generate all permutations (which would be factorial in time); instead, use a mathematical direct-construction method.

#include <cassert>

int main() {
    // Basic cases from the problem description
    assert(getKthPermutation(3, 1) == "123");
    assert(getKthPermutation(3, 2) == "132");
    assert(getKthPermutation(3, 3) == "213");
    assert(getKthPermutation(3, 4) == "231");
    assert(getKthPermutation(3, 5) == "312");
    assert(getKthPermutation(3, 6) == "321");

    // Edge cases: n = 1
    assert(getKthPermutation(1, 1) == "1");

    // n = 4, last permutation
    assert(getKthPermutation(4, 24) == "4321");

    // n = 4, first permutation
    assert(getKthPermutation(4, 1) == "1234");

    // n = 5, middle permutation (check lexicographic ordering)
    assert(getKthPermutation(5, 60) == "25431");

    // n = 2, both permutations
    assert(getKthPermutation(2, 1) == "12");
    assert(getKthPermutation(2, 2) == "21");

    // n = 4, k = 13 (known from manual calculation)
    assert(getKthPermutation(4, 13) == "3124");
}

#include <string>
#include <vector>

std::string getKthPermutation(int n, int k) {
    // Precompute factorials up to n
    std::vector<int> factorial(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
        factorial[i] = factorial[i - 1] * i;
    }

    // Available digits
    std::vector<int> available;
    for (int i = 1; i <= n; ++i) {
        available.push_back(i);
    }

    // Convert to zero-based index
    --k;

    std::string result;
    result.reserve(n);

    // Construct permutation digit by digit
    for (int remaining = n; remaining > 0; --remaining) {
        int blockSize = factorial[remaining - 1];
        int index = k / blockSize;
        result += static_cast<char>('0' + available[index]);
        available.erase(available.begin() + index);
        k %= blockSize;
    }

    return result;
}

// The key idea is to construct the permutation digit by digit without recursion or backtracking. First, precompute factorials from `0!` to `n!`, where `factorial[0] = 1` and `factorial[i] = factorial[i-1] * i`. Then maintain a vector `available` of numbers `{1, 2, ..., n}` representing the unused digits. Convert `k` to zero-based indexing by decrementing it: `k--`. For each position from left to right, the block size is `factorial[available.size() - 1]` (the number of permutations that can be formed by the remaining digits after fixing the current digit). The index of the digit to place is `index = k / blockSize`. Append `available[index]` to the result string, erase that element, and update `k = k % blockSize`. Continue until `available` is empty. Edge cases: when `n = 1`, the loop simply places the single digit; when `k = n!`, the adjusted zero-based `k` becomes `n! - 1`, which yields the last permutation. The time complexity is `O(n^2)` because erasing from a vector costs `O(n)` for each of the `n` positions, and the space complexity is `O(n)` for the vector and the output string.
