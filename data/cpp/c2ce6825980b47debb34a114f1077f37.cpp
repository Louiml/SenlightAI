Given two non-empty strings `s` (pattern) and `t` (text) consisting of lowercase English letters, and an integer `k`, write a C++ function `countApproximateMatches` that returns the number of starting indices `i` (0-indexed) in `s` such that the substring of `s` of length `|t|` starting at `i` matches `t` with at most `k` mismatches (i.e., Hamming distance ≤ `k`). The strings may have different lengths; if `|s| < |t|`, the substring length cannot fit, so return 0. The function must handle up to lengths of 200,000 characters efficiently. Note: the original snippet uses FFT-based convolution to count mismatches for all positions, and you must implement the same algorithmic approach (not brute force).

The problem is to find all positions in `s` where the substring of length `|t|` differs from `t` in at most `k` positions. A direct O(n*m) comparison is too slow for lengths up to 2e5. The efficient approach uses polynomial multiplication (convolution) via FFT. For each letter `ch` from 'a' to 'z', we create two polynomials:
- `A` of degree `|s|-1`: set `A[|s|-1 - j] = 1` if `s[j] != ch`, else 0. This reverses `s` so that convolution aligns substrings properly.
- `B` of degree `|t|-1`: set `B[j] = 1` if `t[j] == ch`, else 0.

The convolution `C = A * B` has the property that `C[|t|-1 + i]` equals the number of positions where `s[i+j] != ch` and `t[j] == ch` for `j=0..|t|-1`. Summing over all 26 letters gives the total number of mismatches for each starting index `i`. Since for each position, a mismatch occurs exactly when `s[i+j] != t[j]`, and the convolution for the specific `t[j]`'s character counts only those positions, summing over letters yields exactly the Hamming distance. Then we count indices `i` from 0 to `|s|-|t|` where the mismatches ≤ `k`. We use iterative FFT with complex numbers, handling padding to a power of two at least `|s|+|t|-1`. Complexity: O(26 * L log L) where L ≈ next power of 2 of (|s|+|t|), so roughly O(26 * n log n). Memory O(L). Edge cases: if `|s| < |t|`, return 0. If `k` is large or zero, handle accordingly. Floating-point rounding: add 0.4 before truncating.

#include <complex>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

// Perform iterative FFT in-place. If invert is true, performs inverse FFT.
void fft(std::vector<std::complex<double>>& a, bool invert) {
    int n = a.size();
    double pi = std::acos(-1.0);
    // Bit-reversal permutation
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) std::swap(a[i], a[j]);
    }
    // Iterative Cooley-Tukey
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * pi / len * (invert ? -1 : 1);
        std::complex<double> wlen(std::cos(ang), std::sin(ang));
        for (int i = 0; i < n; i += len) {
            std::complex<double> w(1.0, 0.0);
            for (int j = 0; j < len / 2; ++j) {
                std::complex<double> u = a[i + j];
                std::complex<double> v = a[i + j + len / 2] * w;
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) {
        for (auto& x : a) x /= n;
    }
}

// Count number of starting positions in s where t matches with at most k mismatches.
int countApproximateMatches(const std::string& s, const std::string& t, int k) {
    const int n = static_cast<int>(s.size());
    const int m = static_cast<int>(t.size());
    if (m > n) return 0;

    // Next power of two >= n + m - 1
    int sz = 1;
    while (sz < n + m - 1) sz <<= 1;

    std::vector<int> mismatchCount(n - m + 1, 0);

    for (int letter = 0; letter < 26; ++letter) {
        std::vector<std::complex<double>> a(sz, 0.0), b(sz, 0.0);
        // Build reversed s polynomial: a[n-1-j] = 1 if s[j] != letter
        for (int j = 0; j < n; ++j) {
            if (s[j] != static_cast<char>('a' + letter)) {
                a[n - 1 - j] = 1.0;
            }
        }
        // Build t polynomial: b[j] = 1 if t[j] == letter
        for (int j = 0; j < m; ++j) {
            if (t[j] == static_cast<char>('a' + letter)) {
                b[j] = 1.0;
            }
        }
        fft(a, false);
        fft(b, false);
        for (int i = 0; i < sz; ++i) a[i] *= b[i];
        fft(a, true);
        // For each start index i, add the number of positions where s[i+j] != letter and t[j] == letter
        // This is at index (n-1 - (i)) + (m-1) = n + m - 2 - i? Actually with reversed a, convolution index is:
        // Let a_index = n-1-j (for j=0..n-1) and b_index = p (for p=0..m-1)
        // Sum over j,p with a_index + b_index = R -> R = n-1-j + p
        // We want j = i + p for substring start i. So R = n-1 - (i+p) + p = n-1-i.
        // Thus the convolution value at index n-1-i counts mismatches for start i.
        // But we need to align with standard convolution indexing. The standard uses same indexing, so we read a[n-1-i].
        for (int i = 0; i < n - m + 1; ++i) {
            int convIdx = n - 1 - i;
            if (convIdx >= 0 && convIdx < sz) {
                mismatchCount[i] += static_cast<int>(a[convIdx].real() + 0.4);
            }
        }
    }

    int result = 0;
    for (int mismatches : mismatchCount) {
        if (mismatches <= k) ++result;
    }
    return result;
}

#include <cassert>
#include <string>

// The solution function is declared here (for test compilation)
int countApproximateMatches(const std::string& s, const std::string& t, int k);

int main() {
    // Exact match
    assert(countApproximateMatches("abc", "abc", 0) == 1);
    // One mismatch allowed
    assert(countApproximateMatches("abc", "abd", 1) == 1);
    // Many positions with different mismatch counts
    assert(countApproximateMatches("aaaa", "aa", 0) == 3);
    assert(countApproximateMatches("aaaa", "aa", 1) == 3);
    // Pattern longer than text
    assert(countApproximateMatches("abc", "abcd", 0) == 0);
    // All mismatches allowed
    assert(countApproximateMatches("abc", "xyz", 3) == 1);
    // Multiple positions with k=0 exact matches only
    assert(countApproximateMatches("abab", "ab", 0) == 2);
    // Check k larger than m
    assert(countApproximateMatches("ab", "cd", 5) == 1);
    // Edge case: single character strings
    assert(countApproximateMatches("a", "a", 0) == 1);
    assert(countApproximateMatches("a", "b", 0) == 0);
    // Random small test: "abcd" with "bce" at positions 0,1: compare "abc" vs "bce" = 2 mismatches, "bcd" vs "bce" = 1 mismatch
    assert(countApproximateMatches("abcd", "bce", 2) == 2);
    assert(countApproximateMatches("abcd", "bce", 1) == 1);
    assert(countApproximateMatches("abcd", "bce", 0) == 0);
}
