Write a C++ function `int countDecodings(const std::string& s)` that returns the number of ways a non-empty string of digits can be decoded into letters using the mapping `'1' -> 'A'`, `'2' -> 'B'`, ..., `'26' -> 'Z'`. The decoding must cover the entire string, and each decoded letter corresponds to either a single digit (1–9) or a two-digit number (10–26). Leading zeros are not allowed as part of a decoded letter (e.g., "0" or "06" are invalid), and a '0' cannot be the first digit of a two-digit number unless the pair is 10 or 20. The result must be returned modulo 1,000,000. The input string may contain any digits, including zeros, and may have length up to 5000.

// The problem is a classic DP counting problem. Define `dp[i]` as the number of ways to decode the suffix starting at index `i`. The base cases: if `i == length`, return 1 (one valid way to decode an empty suffix); if `i == length - 1`, return 1 if the digit is not '0', else 0. For a general `i`, if `s[i] == '0'`, then no decoding is possible from here (since '0' cannot stand alone and a two-digit number starting with '0' is invalid), so return 0. Otherwise, we always can take one digit, so add `dp[i+1]`. If `s[i] == '1'`, we can also take two digits, so add `dp[i+2]`. If `s[i] == '2'`, we can take two digits only if `s[i+1]` is between '0' and '6', so add `dp[i+2]` only then; otherwise not. For digits '3'–'9', only a single-digit decoding is possible. Use memoization with an array initialized to -1. Time complexity is \(O(n)\) since each index is computed once, and space is \(O(n)\) for the memo array (though the code uses a fixed size 5000, we can allocate `n+1`). Edge cases: empty string (though task says non-empty, we can handle it), strings starting with '0', strings containing consecutive zeros like "100" (only one way: "10" and "0" → invalid, so total 0), and long strings near the modulo limit.

#include <string>
#include <vector>
#include <cstring>

// Count number of ways to decode string s into letters (1-26), modulo 1,000,000.
// Returns -1 for invalid input (but problem guarantees non-empty).
int countDecodings(const std::string& s) {
    const int MOD = 1000000;
    int n = static_cast<int>(s.size());
    if (n == 0) return 0; // no ways to decode empty (not specified, but safe)

    std::vector<int> dp(n + 1, -1);
    dp[n] = 1; // empty suffix has one way

    // Helper lambda for recursive memoized computation
    // We'll implement bottom-up to avoid recursion overhead.
    // Start from the end.
    for (int i = n - 1; i >= 0; --i) {
        if (s[i] == '0') {
            dp[i] = 0;
            continue;
        }
        long long ways = dp[i + 1]; // single digit (always possible if not '0')
        if (i + 1 < n) {
            int twoDigit = (s[i] - '0') * 10 + (s[i + 1] - '0');
            if (twoDigit >= 10 && twoDigit <= 26) {
                ways += dp[i + 2];
            }
        }
        dp[i] = static_cast<int>(ways % MOD);
    }
    return dp[0];
}

#include <cassert>
#include <string>

int countDecodings(const std::string& s); // declaration from solution

int main() {
    assert(countDecodings("12") == 2);      // "AB" or "L"
    assert(countDecodings("226") == 3);     // "BZ", "VF", "BBF"
    assert(countDecodings("0") == 0);       // no way
    assert(countDecodings("10") == 1);      // only "J"
    assert(countDecodings("101") == 1);     // "JA" only, "10"+"1" works
    assert(countDecodings("01") == 0);      // leading zero
    assert(countDecodings("27") == 1);      // "BG", not "AA"+"G" (2 and 7)
    assert(countDecodings("111") == 3);     // "AAA", "AK", "KA"
    assert(countDecodings("100") == 0);     // "10"+"0" invalid, "1"+"00" invalid
    assert(countDecodings("123456789") == 3); // manually verified possible ways
}
