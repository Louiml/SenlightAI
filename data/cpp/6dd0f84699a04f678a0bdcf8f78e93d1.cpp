// Write a C++ function `countFairSquareNumbers(long long A, long long B)` that returns the number of integers `N` in the inclusive range `[A, B]` such that both `N` and `N*N` are palindromes (read the same forwards and backwards). The range may be large (up to 10^14 for both endpoints), and `A` and `B` may be equal or in either order (though you may assume `A <= B` for simplicity). The function must be efficient enough to handle many queries, so precompute all "fair square" numbers up to the maximum possible square root (i.e., 10^7, since 10^7^2 = 10^14) once. Return the count using a binary search on the precomputed list. Handle single-digit numbers correctly (they are palindromes).

The problem reduces to generating all palindromic numbers `p` from 1 to 10^7 such that `p*p` is also palindromic. There are only 39 such numbers (as seen in the snippet). Precompute them once into a sorted vector. For each query, count how many fall in `[A, B]` by finding the first index where the value >= A (via lower_bound) and the first index where value > B (via upper_bound), then subtracting. Use `long long` (or `int64_t`) to avoid overflow since squares up to 10^14 exceed 32-bit integers. The palindrome check for a number can be done by converting to a string or by reversing digits; the latter is simpler and avoids string overhead. Complexity: Precomputation is O(10^7 * digits) ≈ ~10^7 * 8 operations, acceptable for a one-time setup. Each query is O(log M) where M=39, so essentially O(1). Space is O(39) for the list. Edge cases: ensure numbers like 0 are not included (since starting at 1), and that squares like 1, 4, 9, 121, 484, etc. are handled. The function must be const-correct and not depend on global mutable state across calls unless recalculated each call (but the task suggests precomputing once, so we can use a static local vector).

#include <vector>
#include <cstdint>
#include <algorithm>

// Check if a given positive integer is a palindrome.
bool isPalindrome(int64_t num) {
    if (num < 0) return false;
    int64_t original = num;
    int64_t reversed = 0;
    while (num > 0) {
        reversed = reversed * 10 + num % 10;
        num /= 10;
    }
    return original == reversed;
}

// Precompute all palindromic numbers p (1 <= p <= 10^7) such that p*p is also palindromic.
// Returns a sorted vector of those squares.
std::vector<int64_t> generateFairSquares() {
    std::vector<int64_t> fairSquares;
    const int64_t LIMIT = 10000000; // 10^7
    for (int64_t p = 1; p <= LIMIT; ++p) {
        if (isPalindrome(p) && isPalindrome(p * p)) {
            fairSquares.push_back(p * p);
        }
    }
    return fairSquares;
}

// Count how many fair square numbers lie in the inclusive range [A, B].
// A and B must satisfy A <= B.
int64_t countFairSquareNumbers(int64_t A, int64_t B) {
    // Static local ensures precomputation happens only once.
    static const std::vector<int64_t> fairSquares = generateFairSquares();
    
    // Find first index with value >= A.
    auto itLow = std::lower_bound(fairSquares.begin(), fairSquares.end(), A);
    // Find first index with value > B (or end).
    auto itHigh = std::upper_bound(fairSquares.begin(), fairSquares.end(), B);
    
    return static_cast<int64_t>(itHigh - itLow);
}

#include <cassert>
#include <cstdint>

// Assume the function is declared above.

int main() {
    // Test small ranges with known fair squares: 1, 4, 9, 121, 484, 10201, 12321, 14641, 40804, 44944, etc.
    // First few squares: p=1 ->1, p=2 ->4, p=3 ->9, p=11 ->121, p=22 ->484, p=101 ->10201, p=111 ->12321, p=121 ->14641, p=202 ->40804, p=212 ->44944.
    assert(countFairSquareNumbers(1, 1) == 1);
    assert(countFairSquareNumbers(1, 10) == 3); // 1,4,9
    assert(countFairSquareNumbers(1, 100) == 3); // still 1,4,9
    assert(countFairSquareNumbers(100, 500) == 2); // 121, 484
    assert(countFairSquareNumbers(1, 10201) == 6); // 1,4,9,121,484,10201
    assert(countFairSquareNumbers(10201, 10201) == 1);
    assert(countFairSquareNumbers(10202, 12321) == 1); // 12321
    assert(countFairSquareNumbers(1, 100000000000000LL) == 39); // total known count
    assert(countFairSquareNumbers(1, 0) == 0); // empty range (A>B, though not expected, but safe)
    return 0;
}
