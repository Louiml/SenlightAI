// Write a C++ function `int piDifficulty(const std::string& s)` that computes the minimum total difficulty of partitioning a string of digits into contiguous segments of length 3, 4, or 5, where each segment's difficulty is determined by its pattern. The difficulty rules for a segment of digits are: (1) if all digits are identical, difficulty = 1; (2) if the digits form a strictly increasing or decreasing sequence by exactly 1 each step (e.g., 123, 987), difficulty = 2; (3) if the digits are an arithmetic progression with a common difference other than ±1 (e.g., 135, 8642), difficulty = 5; (4) if the digits alternate between two values (e.g., 12121, 3232), difficulty = 4; (5) otherwise, difficulty = 10. The function must return the minimal sum of difficulties over a valid partition covering the entire string, or -1 if no valid partition exists (i.e., the string length is less than 3, or no combination of segment lengths 3,4,5 sums to the total length). The input string will contain only digit characters ('0'–'9') and will be non-empty. Use dynamic programming or memoized recursion, and ensure the solution handles strings up to length 10,000 efficiently.

// The problem is a classic optimal partitioning with overlapping subproblems. Define `dp[pos]` as the minimum difficulty to partition the suffix starting at index `pos`. The base case is `dp[n] = 0` (empty suffix). If `pos > n - 3` (remaining length < 3), the suffix cannot be partitioned, so `dp[pos] = INF` (or -1). Otherwise, we try all valid segment lengths `len` from 3 to 5 (but not exceeding `n-pos`), compute the difficulty of the substring `s[pos..pos+len-1]` using the given rules, and add `dp[pos+len]`. The answer is `dp[0]` if finite, else -1. Important edge cases: (1) the alternating rule (two digits alternating) should be checked after the arithmetic progression rule, because a segment like "121" is both alternating and has a difference of -1? Actually "121" has differences -1, 2, so it's not arithmetic; but "1212" is alternating and also has differences -1,1,-1 so not arithmetic. But "123123" would be alternating and arithmetic? Differences 1,1,? Actually "123123" has differences 1,1,? The third digit to fourth is 1-3 = -2, so not arithmetic. The rules are mutually exclusive: identical (diff 1), adjacent ±1 (diff 2), arithmetic with other constant difference (diff 5), alternating two digits (diff 4), else 10. So check in that order. For alternating, we must check that length >= 3, and that for all i from a to b-3, `s[i] == s[i+2]`, and also ensure it's not already caught by earlier rules (e.g., "111" is identical -> diff 1, not alternating). Also note: a segment like "1212" satisfies alternating (s[0]==s[2], s[1]==s[3]) and each adjacent difference is not constant, so diff 4. A segment like "12321" does not satisfy alternating because s[0]!=s[2]? Actually 1 vs 3, no. So fine. Time complexity: O(n * 3) for DP transitions, and each difficulty calculation O(1) since segment length ≤5. Overall O(n) time and O(n) space for memoization. For n up to 10,000 this is trivial.

#include <string>
#include <vector>
#include <algorithm>
#include <climits>

// Compute the minimum total difficulty of partitioning a digit string into segments of length 3-5.
int piDifficulty(const std::string& s) {
    int n = static_cast<int>(s.size());
    if (n < 3) return -1;

    // dp[i] = minimum difficulty for suffix starting at i, or -1 if impossible.
    std::vector<int> dp(n + 1, -1);
    dp[n] = 0; // empty suffix

    // Helper lambda to compute difficulty of a segment s[a..b-1], length between 3 and 5.
    auto segmentDifficulty = [&](int a, int b) -> int {
        int len = b - a;
        // Rule 1: all identical
        bool allSame = true;
        for (int i = a + 1; i < b; ++i) {
            if (s[i] != s[a]) { allSame = false; break; }
        }
        if (allSame) return 1;

        // Check if it's an arithmetic progression
        bool arithmetic = true;
        int diffVal = s[a + 1] - s[a];
        for (int i = a + 1; i < b - 1; ++i) {
            if (s[i + 1] - s[i] != diffVal) { arithmetic = false; break; }
        }
        if (arithmetic) {
            // If diff is ±1, difficulty 2, else difficulty 5
            return (diffVal == 1 || diffVal == -1) ? 2 : 5;
        }

        // Rule 3: alternating two values (only for len >= 3)
        bool alternating = true;
        for (int i = a; i < b - 2; ++i) {
            if (s[i] != s[i + 2]) { alternating = false; break; }
        }
        if (alternating) return 4;

        // Default difficulty
        return 10;
    };

    // Fill dp from right to left
    for (int pos = n - 1; pos >= 0; --pos) {
        int best = INT_MAX;
        // Try segment lengths 3, 4, 5 but not exceeding remaining length
        for (int len = 3; len <= 5 && pos + len <= n; ++len) {
            int next = dp[pos + len];
            if (next == -1) continue; // suffix after this segment impossible
            int difficulty = segmentDifficulty(pos, pos + len);
            best = std::min(best, difficulty + next);
        }
        dp[pos] = (best == INT_MAX) ? -1 : best;
    }

    return dp[0];
}

#include <cassert>
#include <string>

// The solution function is defined above; here we test it.
int main() {
    // All identical: "111" -> difficulty 1
    assert(piDifficulty("111") == 1);
    // "1111" can be one segment of length 4 -> difficulty 1
    assert(piDifficulty("1111") == 1);
    // "11111" length 5 -> difficulty 1
    assert(piDifficulty("11111") == 1);

    // Monotonic ±1: "123" -> difficulty 2
    assert(piDifficulty("123") == 2);
    // "9876" -> difficulty 2
    assert(piDifficulty("9876") == 2);

    // Arithmetic with diff 2: "135" -> difficulty 5
    assert(piDifficulty("135") == 5);
    // "8642" length 4, diff -2 -> difficulty 5
    assert(piDifficulty("8642") == 5);

    // Alternating: "121" -> difficulty 4
    assert(piDifficulty("121") == 4);
    // "32323" length 5 -> difficulty 4
    assert(piDifficulty("32323") == 4);

    // Otherwise: "1234" length 4 -> difficulty 10
    assert(piDifficulty("1234") == 10);

    // Impossible: length 1 or 2
    assert(piDifficulty("1") == -1);
    assert(piDifficulty("12") == -1);

    // Case where no partition covers exactly: length 7 (e.g., 3+4 works, but 7 can be 3+4 or 4+3)
    // Let's test "1234567" -> segments "123" (diff 2) + "4567" (diff 10?) Actually "4567" is +1 each -> diff 2, total 4
    // "4567" is monotonic +1 -> diff 2, so total 2+2=4. Another partition: "1234" diff 10? "1234" is +1 each -> diff 2 actually! So "1234" diff 2, plus "567" diff 2 -> total 4. So expected 4.
    assert(piDifficulty("1234567") == 4);

    // Mixed case: "12111" length 5. Options: one segment "12111" -> not alternating (1,2,1,1,1?), not arithmetic, not same -> diff 10. Or "121" diff 4 + "11" impossible. So only one segment -> 10.
    assert(piDifficulty("12111") == 10);

    // A case with multiple segments: "123456" length 6. Could be "123" (2) + "456" (2) = 4; or "1234" (2) + "56" impossible; or "12345" (2) + "6" impossible; or "12" impossible. So answer 4.
    assert(piDifficulty("123456") == 4);

    // Edge: all same but length 8: "88888888" -> partition into 3+5 or 4+4, each segment diff 1 -> total 2.
    assert(piDifficulty("88888888") == 2);

    return 0;
}
