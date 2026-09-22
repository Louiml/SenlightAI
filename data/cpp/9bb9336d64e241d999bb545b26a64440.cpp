Given a string `s` of length `n` (1 ≤ n ≤ 100,000) consisting of digits only ('0'–'9'), write a C++ function `long long sumOfAllSubstringValues(const std::string& s)` that returns the sum, modulo \(10^9+7\), of the integer values represented by all non-empty substrings of `s`. For example, for `s = "123"`, substrings are: "1", "2", "3", "12", "23", "123" → values 1+2+3+12+23+123 = 164. The function must be efficient for large `n`, avoiding naive string-to-int conversion for each substring.
#include <cassert>
#include <string>

// Declaration of the function under test (assumed provided)
long long sumOfAllSubstringValues(const std::string& s);

int main() {
    // Basic examples
    assert(sumOfAllSubstringValues("1") == 1);
    assert(sumOfAllSubstringValues("12") == 1 + 2 + 12); // 15
    assert(sumOfAllSubstringValues("123") == 164);

    // All same digit
    assert(sumOfAllSubstringValues("11") == 1 + 1 + 11); // 13
    assert(sumOfAllSubstringValues("0") == 0);
    assert(sumOfAllSubstringValues("00") == 0);

    // Larger test: "1234" substrings sum: 1+2+3+4+12+23+34+123+234+1234 = 1670
    assert(sumOfAllSubstringValues("1234") == 1670);

    // Test modulo with large n: sum for a string of 100000 '9's is known from formula.
    // We'll just verify it's within modulo range and not crash.
    std::string s(100000, '9');
    long long result = sumOfAllSubstringValues(s);
    assert(result >= 0 && result < 1000000007);

    // Small edge: "0" repeated
    assert(sumOfAllSubstringValues("0001") == 1); // only substrings containing '1' contribute

    // More thorough check using brute force for small strings
    auto brute = [](const std::string& t) -> long long {
        long long sum = 0;
        int n = (int)t.size();
        for (int i = 0; i < n; ++i) {
            long long val = 0;
            for (int j = i; j < n; ++j) {
                val = (val * 10 + (t[j]-'0')) % MOD;
                sum = (sum + val) % MOD;
            }
        }
        return sum;
    };

    assert(sumOfAllSubstringValues("987") == brute("987"));
    assert(sumOfAllSubstringValues("101") == brute("101"));
    assert(sumOfAllSubstringValues("123456789") == brute("123456789"));

    return 0;
}
#include <string>
#include <cstdint>

const long long MOD = 1000000007LL;

// Returns the sum of integer values of all substrings modulo 1e9+7.
long long sumOfAllSubstringValues(const std::string& s) {
    const int n = static_cast<int>(s.size());
    if (n == 0) return 0;

    long long dp = static_cast<long long>(s[0] - '0') % MOD; // sum of substrings ending at index 0
    long long total = dp;

    for (int i = 1; i < n; ++i) {
        long long digit = static_cast<long long>(s[i] - '0');
        // dp[i] = (i+1)*digit + 10*dp[i-1]
        long long newDp = ((i + 1) * digit) % MOD;
        newDp = (newDp + (10 * dp) % MOD) % MOD;
        dp = newDp;
        total = (total + dp) % MOD;
    }

    return total;
}
// The classic solution iterates from right to left. Let `f[i]` be the integer value of the suffix starting at index `i` (0-based) modulo MOD. We can compute `f[i] = (f[i+1] + (s[i]-'0') * 10^(n-i-1)) % MOD`. Then we accumulate contributions for substrings ending at each position. Specifically, for each index `i` (from 1 to n-1), the substrings that start anywhere from 0 to i-1 and end at i contribute: sum over start `j` of value(s[j..i]) = sum_{j=0}^{i-1} (value(s[j..i])). Notice that value(s[j..i]) = (value(s[j..i-1])*10 + (s[i]-'0')). But a more direct formula: the contribution of all substrings ending at `i` equals `(i+1) * (s[i]-'0') + 10 * (sum of values of substrings ending at i-1)`. Let `dp[i]` = sum of values of all substrings ending at index `i`. Then `dp[0] = s[0]-'0'`, and for i>0: `dp[i] = ( (i+1)*(s[i]-'0') + 10*dp[i-1] ) % MOD`. The total answer is sum_{i=0}^{n-1} dp[i] mod MOD. This works because each substring ending at i is either the single character s[i] (contributes (i+1) times? Actually the single character appears once, but we derive: the substring s[i] itself contributes s[i]-'0', and for each substring ending at i-1, appending s[i] multiplies its value by 10 and adds s[i]-'0'. There are exactly (i) such substrings ending at i-1, but dp[i-1] is their sum. So dp[i] = (s[i]-'0') + 10*dp[i-1] + (s[i]-'0') * i? Let's re-derive: substrings ending at i are: s[i] alone (1 substring), and for each substring ending at i-1 (there are i such substrings, because indices 0..i-1 as starts), we extend it by s[i]. The sum of those extended values = 10 * (sum of values of substrings ending at i-1) + i * (s[i]-'0'). So dp[i] = (s[i]-'0') + 10*dp[i-1] + i*(s[i]-'0') = (i+1)*(s[i]-'0') + 10*dp[i-1]. Yes correct. Complexity: O(n) time, O(1) extra space (only need previous dp and running total). Edge cases: single character, zeros, large n. Modulo careful multiplication to avoid overflow using long long. The given snippet uses a different method (suffix sums) but the above DP is simpler and correct. Time O(n), space O(1) (or O(n) if we store all dp, but not needed).
