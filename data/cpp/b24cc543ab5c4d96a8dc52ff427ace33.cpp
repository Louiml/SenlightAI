/*
Given a binary string `s` containing only '0' and '1' characters (length between 1 and 1000), write a C++ function `int minimumCuts(const std::string& s)` that determines the minimum number of cuts needed to partition the entire string into substrings, where each substring, when interpreted as a binary number (with the leftmost character as the most significant bit), is a power of 5 (i.e., 1, 5, 25, 125, ...). The cuts are made between characters, and each resulting substring must represent a valid power of 5 without leading zeros (so a substring cannot start with '0'). If it is impossible to partition the string under these rules, return -1. For example, for `s = "101101"`, the binary representations of powers of 5 are "1" (1), "101" (5), "11001" (25), "1111101" (125), etc., so one optimal partition is "101" + "101" (two cuts: between positions 2-3 and after the last character? Actually substrings "101" and "101" requires one cut between them, so minimum cuts = 1). Note: A single-character substring "1" is valid (1 = 5^0), but "0" is invalid due to leading zero and value zero.
*/
#include <string>
#include <vector>
#include <unordered_set>
#include <climits>
#include <cmath>

// Precompute all powers of 5 that fit in a 64-bit integer (up to 5^27).
static const std::unordered_set<long long> powersOfFive = []() {
    std::unordered_set<long long> powers;
    long long value = 1;
    powers.insert(value);
    for (int i = 1; i <= 27; ++i) {
        value *= 5;
        powers.insert(value);
    }
    return powers;
}();

// Helper function for recursive segmentation with memoization.
long solveCuts(int pos, const std::string& s, std::vector<long>& memo) {
    if (pos == static_cast<int>(s.size())) {
        return 0;
    }
    if (memo[pos] != -1) {
        return memo[pos];
    }
    if (s[pos] == '0') {
        return memo[pos] = INT_MAX;
    }
    
    long long num = 0;
    long minCuts = INT_MAX;
    
    for (int j = pos; j < static_cast<int>(s.size()); ++j) {
        num = num * 2 + (s[j] - '0');
        // If the number is too large, further extensions will only grow, so break.
        if (num > 4000000000000000000LL) { // roughly > 4e18, larger than max power of 5 we store
            break;
        }
        if (powersOfFive.find(num) != powersOfFive.end()) {
            long rest = solveCuts(j + 1, s, memo);
            if (rest != INT_MAX) {
                minCuts = std::min(minCuts, 1 + rest);
            }
        }
    }
    return memo[pos] = minCuts;
}

// Returns the minimum number of cuts to partition the binary string into substrings
// that are powers of 5, or -1 if impossible.
int minimumCuts(const std::string& s) {
    if (s.empty()) {
        return 0; // Degenerate case: no cuts needed for empty string.
    }
    std::vector<long> memo(s.size(), -1);
    long result = solveCuts(0, s, memo);
    return (result >= INT_MAX) ? -1 : static_cast<int>(result);
}
#include <cassert>
#include <string>

int main() {
    // Single character "1" is a power of 5 (5^0), no cuts needed.
    assert(minimumCuts("1") == 0);
    // "101" is binary for 5, no cuts needed.
    assert(minimumCuts("101") == 0);
    // "101101" can be split as "101" + "101", one cut.
    assert(minimumCuts("101101") == 1);
    // "1011101" -> binary 93, not a power of 5; split as "1"+"0"+"11101"? "0" invalid.
    // Better: "101"+"1101"? 1101 is 13, invalid. Try "1"+"0" fails. So impossible.
    assert(minimumCuts("1011101") == -1);
    // "11001" is binary for 25, no cuts.
    assert(minimumCuts("11001") == 0);
    // "101001" -> possible as "101"+"001"? "001" invalid, but "10100"+"1"? 10100=20 invalid.
    // Actually "1"+"01001"? starts with 0 invalid. So likely impossible.
    assert(minimumCuts("101001") == -1);
    // "111" -> binary 7, not power of 5; try "1"+"11" (3 invalid), "11"+"1" (3 invalid), "1"+"1"+"1" (two cuts total)
    // that yields all "1" segments, each valid, so min cuts = 2.
    assert(minimumCuts("111") == 2);
    // "1000" -> "1"+"000"? leading zero invalid, "10"+"00" invalid, "100"+"0" invalid, "1000"=8 invalid. Impossible.
    assert(minimumCuts("1000") == -1);
    // "110111" -> possible: "1"+"101"+"11"? 11=3 invalid; "1101"+"11"? 13 invalid; "1"+"1"+"0111"? invalid.
    // "110"+"111"? 6 and 7 invalid; "11"+"0" invalid. Maybe "11011"+"1"? 27 invalid. So impossible.
    assert(minimumCuts("110111") == -1);
    // "10101" -> binary 21, not power of 5; split "1"+"0101" invalid, "10"+"101" invalid, "101"+"01" invalid,
    // "1"+"0" invalid, "1"+"1"+"01" invalid, "1"+"1"+"1"+"01"? invalid. So impossible.
    assert(minimumCuts("10101") == -1);
    // "1" repeated 10 times: all segments "1", need 9 cuts.
    assert(minimumCuts("1111111111") == 9);
    return 0;
}
// The problem is a classic recursion/dynamic programming segmentation problem. Define `solve(pos)` as the minimum number of cuts needed to partition the suffix `s[pos..end]` into valid substrings. The base case is when `pos == s.length()`, meaning we have successfully processed the entire string, returning 0 cuts (no further cuts needed). If the character at `pos` is '0', the substring starting here would have a leading zero, so it cannot be a valid segment; return a large sentinel (like `INT_MAX`) to indicate impossibility. For a valid starting position, iterate over all possible ending positions `j` from `pos` to `s.length()-1`, building the binary number `num` incrementally: `num = num * 2 + (s[j] - '0')`. Since the binary string can be long, we must be careful about overflow; the maximum power of 5 that fits in a 64-bit integer is 5^27 ≈ 7.45e18, but binary numbers with length up to 1000 can exceed that. However, any valid power of 5 must be a positive integer and cannot have more than 27 binary digits when using 64-bit; if `num` exceeds the largest power of 5 we care about, we can break early. Precompute all powers of 5 up to 5^27 (since 5^27 > 2^62) and store them in a set for fast lookup. For each `j`, if `num` equals a precomputed power of 5, then we can make a cut after position `j`, and the cost is `1 + solve(j+1)`, taking the minimum over all such choices. If no valid cut leads to a successful partition, return `INT_MAX`. The recursion can be memoized with a `vector<long>` of size `n+1` initialized to -1. Edge cases: empty string? The problem guarantees non-empty, but if empty, return 0 cuts (or handle gracefully). Strings with a leading '0' at the start cannot be partitioned because the first character is invalid; return -1. Time complexity: Each state tries up to `n` possible endings, and for each ending we do O(1) set lookup, so O(n^2) time and O(n) space for memoization and the set of powers. The recursion depth is at most `n`.
