// Write a C++ function `bigCombination(int n, int k)` that computes the binomial coefficient \(C(n, k)\) for \(0 \le k \le n \le 100\), returning the result as a pair of `long long` values: the high part (coefficient of \(10^{18}\)) and the low part (remainder less than \(10^{18}\)). The function must correctly handle cases where \(C(n, k)\) exceeds \(9{,}223{,}372{,}036{,}854{,}775{,}807\) (the maximum of `long long`) by splitting the result into two parts: `high` such that the true value is `high * 10^18 + low`, where `low` is always in the range \([0, 10^{18})\). The implementation should use dynamic programming to fill a table of binomial coefficients, where each cell is stored as a custom pair-like struct with `high` and `low` members. The function must be robust against all edge cases, including \(k = 0\), \(k = n\), \(n = 0\), and large \(n\) approaching 100 where the binomial coefficient can exceed \(10^{38}\). Do not use any external libraries beyond standard headers.

// The key issue is that \(C(100, 50)\) is about \(1.0089 \times 10^{29}\), which exceeds the 64-bit `long long` range. Therefore, we represent each value as a pair `(high, low)` meaning `high * BASE + low`, with `BASE = 10^18`. The recurrence \(C(n, k) = C(n-1, k) + C(n-1, k-1)\) is applied to these pairs. To add two pairs, we first add the low parts; if their sum is at least `BASE`, we subtract `BASE` from the low sum and increment the high sum by 1. Then we add the high parts directly. Since each `high` part is at most around \(10^{11}\) (from \(10^{29}/10^{18}\)), it fits in `long long`. We initialize a table of size \(101 \times 101\) with zeros, then set the base cases: for any \(n\), \(C(n, 0) = C(n, n) = 1\) (represented as low=1, high=0). However, to simplify the DP, we can fill all entries for row 0 and row 1, then loop from \(i=1\) to \(n\) and \(j=0\) to \(i\) (since \(j > i\) is never used). For each \(j\), if \(j==0\) or \(j==i\), set low=1, high=0; otherwise, use the addition of the two predecessors. The function returns the pair for \(C(n, k)\). Edge cases: when \(k > n\) (though problem constraints say \(0 \le k \le n\), we might still check and return {0,0}); when \(n=0\) or \(k=0\), return {0,1}. Time complexity: \(O(n \cdot k)\) for filling the table, but since we compute up to \(n\), it's \(O(n^2)\) in the worst case. Space complexity: \(O(n^2)\) for the table. We can optimize space to \(O(n)\) by using two rows, but for simplicity and clarity, we keep a full table as in the original snippet.

#include <utility>

// A large integer represented as high * 1e18 + low.
struct BigInt {
    long long high;
    long long low;
};

const long long BASE = 1000000000000000000LL; // 10^18

// Add two BigInt values.
BigInt addBigInt(const BigInt& a, const BigInt& b) {
    BigInt result;
    result.low = a.low + b.low;
    result.high = a.high + b.high;
    if (result.low >= BASE) {
        result.low -= BASE;
        result.high += 1;
    }
    return result;
}

// Computes C(n, k) for 0 <= k <= n <= 100, returning the result as BigInt.
BigInt bigCombination(int n, int k) {
    static BigInt table[101][101]; // static zero-initialized

    // Build the table up to n.
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= i; ++j) {
            if (j == 0 || j == i) {
                table[i][j].high = 0;
                table[i][j].low = 1;
            } else {
                table[i][j] = addBigInt(table[i - 1][j], table[i - 1][j - 1]);
            }
        }
    }

    // Handle invalid input defensively.
    if (k < 0 || k > n) {
        return {0, 0};
    }
    return table[n][k];
}

#include <cassert>

int main() {
    // Base cases
    BigInt r = bigCombination(0, 0);
    assert(r.high == 0 && r.low == 1);
    r = bigCombination(5, 0);
    assert(r.high == 0 && r.low == 1);
    r = bigCombination(5, 5);
    assert(r.high == 0 && r.low == 1);

    // Small values within 64-bit range
    r = bigCombination(10, 3);
    assert(r.high == 0 && r.low == 120);
    r = bigCombination(20, 10);
    assert(r.high == 0 && r.low == 184756);

    // Large values exceeding long long
    r = bigCombination(60, 30);
    // C(60,30) = 118264581564861424, which fits in low because < 1e18
    assert(r.high == 0 && r.low == 118264581564861424LL);

    // C(100, 50) = 100891344545564193334812497256
    // = 100891344545564193 * 1e18 + 334812497256
    // Let's compute: 100891344545564193 * 1e18 + 334812497256
    r = bigCombination(100, 50);
    assert(r.high == 100891344545564193LL);
    assert(r.low == 334812497256LL);

    // Symmetry check: C(100, 20) == C(100, 80)
    BigInt a = bigCombination(100, 20);
    BigInt b = bigCombination(100, 80);
    assert(a.high == b.high && a.low == b.low);

    // Invalid inputs return zero
    r = bigCombination(5, 6);
    assert(r.high == 0 && r.low == 0);
    r = bigCombination(-1, 0);
    assert(r.high == 0 && r.low == 0);
}
