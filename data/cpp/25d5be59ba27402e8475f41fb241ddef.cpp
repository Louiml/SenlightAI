// Write a C++ function `std::string kthPermutation(int n, int k)` that, given a positive integer `n` and a positive integer `k` (1 ≤ k ≤ n!), returns the k-th permutation in lexicographic order of the string formed by concatenating the numbers 1 through n in order (e.g., for n=3, the base string is "123", and the permutations in lexicographic order are "123", "132", "213", "231", "312", "321"). The function must return the permutation as a string. If k is out of range (k < 1 or k > n!), return an empty string. You may assume n ≥ 1.
#include <cassert>
#include <string>

// The solution function is declared above; here we test it.

int main() {
    assert(kthPermutation(3, 1) == "123");
    assert(kthPermutation(3, 2) == "132");
    assert(kthPermutation(3, 3) == "213");
    assert(kthPermutation(3, 4) == "231");
    assert(kthPermutation(3, 5) == "312");
    assert(kthPermutation(3, 6) == "321");
    assert(kthPermutation(1, 1) == "1");
    assert(kthPermutation(4, 24) == "4321");
    assert(kthPermutation(4, 25) == ""); // out of range
    assert(kthPermutation(4, 0) == "");  // k must be positive
    return 0;
}
#include <string>
#include <vector>
#include <cstdint>

// Return the k-th lexicographic permutation of the string "1".."n".
// k is 1-indexed. Returns empty string if k is out of range.
std::string kthPermutation(int n, int k) {
    if (n <= 0 || k <= 0) return "";
    
    // Compute factorials up to n
    std::vector<long long> fact(n + 1, 1);
    for (int i = 1; i <= n; ++i) {
        fact[i] = fact[i - 1] * i;
    }
    
    if (k > fact[n]) return ""; // k out of range
    
    // Convert to 0-indexed
    long long k0 = static_cast<long long>(k) - 1;
    
    // Available digits
    std::vector<char> digits;
    for (int i = 1; i <= n; ++i) {
        digits.push_back(static_cast<char>('0' + i));
    }
    
    std::string result;
    result.reserve(n);
    
    for (int i = 0; i < n; ++i) {
        long long blockSize = fact[n - 1 - i];
        int index = static_cast<int>(k0 / blockSize);
        result.push_back(digits[index]);
        digits.erase(digits.begin() + index);
        k0 %= blockSize;
    }
    
    return result;
}
// The most straightforward approach is to generate all permutations recursively in lexicographic order and stop when the k-th is found. Instead of the provided solution’s brute-force backtracking that checks each partial permutation, a more efficient method is to use the factorial number system (also known as Lehmer code). Given k (1-indexed), we can compute the permutation directly without generating all permutations. 
//
// Algorithm:
// 1. Create a sorted list of available digits '1'..'n' (in a string or vector).
// 2. Decrement k by 1 to make it 0-indexed.
// 3. For each position from 0 to n-1:
//    - Compute factorial of (n - 1 - position) — the number of permutations that can start with each remaining digit.
//    - Let group = k / factorial. This gives the index of the digit to choose from the remaining list.
//    - Append that digit to the result, erase it from the remaining list.
//    - Update k = k % factorial.
// 4. If k was out of range (i.e., after decrementing it, if k < 0 or k >= n! before processing), return empty string.
//
// Edge cases: n=1 (only "1"), k=1 gives "1"; k=n! gives the last permutation. Factorial values grow quickly, so use `long long` to avoid overflow for n up to 20 (20! fits in 64-bit). Time complexity: O(n^2) because each position requires O(n) to erase from the list (or O(1) if using a balanced tree, but here simple vector erase is fine for typical n). Space complexity: O(n) for the result and remaining digits.
