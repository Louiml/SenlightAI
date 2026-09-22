/*
Write a C++ function `int minimumCuts(const std::string& S)` that, given a binary string `S` consisting only of characters `'0'` and `'1'`, returns the minimum number of pieces into which `S` must be cut so that every piece, when interpreted as a binary number (with no leading zeros allowed except for the single digit `"0"`), is a positive power of 5 (i.e., 1, 5, 25, 125, ...). If it is impossible to partition the entire string into such pieces, return `-1`. Each cut must produce non-empty contiguous substrings, and the order of the pieces must be the same as they appear in `S`.
*/
#include <string>
#include <vector>
#include <algorithm>
#include <climits>

// Checks if a positive integer x is a power of 5.
static bool isPowerOfFive(long long x) {
    while (x > 1 && x % 5 == 0) {
        x /= 5;
    }
    return x == 1;
}

// Returns the minimum number of pieces to partition S into binary substrings that are powers of 5.
int minimumCuts(const std::string& S) {
    const int n = static_cast<int>(S.size());
    const int INF = INT_MAX / 2;
    std::vector<int> dp(n + 1, INF);
    dp[0] = 0;

    for (int end = 0; end < n; ++end) {
        for (int start = 0; start <= end; ++start) {
            // Skip substrings with leading zero, because they are not positive powers of 5.
            if (S[start] == '0') continue;

            // Compute the binary value of S[start..end].
            long long value = 0;
            for (int k = start; k <= end; ++k) {
                value = (value << 1) | (S[k] - '0');
            }

            if (!isPowerOfFive(value)) continue;

            // Update DP if a better partition is found.
            if (dp[start] + 1 < dp[end + 1]) {
                dp[end + 1] = dp[start] + 1;
            }
        }
    }

    return (dp[n] >= INF) ? -1 : dp[n];
}
#include <cassert>

int main() {
    assert(minimumCuts("1") == 1);          // "1" = 1 = 5^0
    assert(minimumCuts("101") == 1);        // "101" = 5
    assert(minimumCuts("11001") == 1);      // "11001" = 25
    assert(minimumCuts("101101") == 2);     // "101" + "101" = 5 + 5
    assert(minimumCuts("111") == -1);       // 7 is not a power of 5, no split works
    assert(minimumCuts("000") == -1);       // leading zeros invalid, cannot partition
    assert(minimumCuts("10101") == 2);      // "101" + "01" invalid due to leading zero, but "1" + "0101"? No. Actually "10101" = 21, can split as "101" (5) + "01" invalid, or "1" (1) + "0101" invalid, or "10" (2) invalid, etc. Let's test: best is "1" + "0101"? no. Actually "10101" can be split as "101" + "01" (invalid), or "10" invalid, "1010" invalid, "10101" invalid. So answer is -1. But I'll assert -1.)
    assert(minimumCuts("10101") == -1);
    assert(minimumCuts("110011") == 2);     // "11001" (25) + "1" (1) = 2 pieces
    assert(minimumCuts("1010101") == -1);    // no valid partition (some might find "101"+"01" invalid)
    return 0;
}
// The solution uses dynamic programming over string positions. Define `dp[i]` as the minimum number of valid pieces to partition the prefix `S[0..i-1]` (length `i`). Initialize `dp[0] = 0` for the empty prefix, and all other `dp[i]` to a large sentinel value. For each end position `i` (from 0 to n-1), we try every possible start position `j` from 0 to `i` such that the substring `S[j..i]` forms a valid piece. A piece is valid if it does not start with `'0'` (because leading zeros would create numbers with extra zeros, which are never powers of 5 unless the number itself is 0, but 0 is not a power of 5; also note the problem states positive powers of 5, so the substring `"0"` is invalid), and the integer value of that binary substring is a power of 5. To check if a positive integer `x` is a power of 5, repeatedly divide by 5 while it is divisible and non-zero; the result must be exactly 1. To avoid overflow, note the string length can be up to 50, so the binary value can be up to 2^50, which fits in a 64-bit signed integer (2^50 ≈ 1.12e15, well below 9.22e18). For each valid `(j,i)`, update `dp[i+1] = min(dp[i+1], dp[j] + 1)`. Finally, if `dp[n]` remains the sentinel, return `-1`; otherwise return `dp[n]`. Time complexity is O(n^3) in the worst case if we recompute substring values naively (O(n^2) substrings, each costing O(n) to convert), but since n≤50, this is trivial. With precomputation of powers or early break, it can be O(n^2) per substring check but still O(n^3) overall. With n=50, this is at most 125,000 operations, which is fine. Space complexity is O(n) for the DP array.
