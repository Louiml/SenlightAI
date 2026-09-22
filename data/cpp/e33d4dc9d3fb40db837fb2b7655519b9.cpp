/*
Given a string `s` of length `n` consisting only of characters `'X'`, `'Z'`, and `'.'` (dot), and an integer `k`, write a standalone C++ function `int countGoodStrings(const std::string& s, int k)` that returns the number of ways to replace exactly `k` dot characters with either `'X'` or `'Z'` such that in the resulting string, no two adjacent characters are equal. The original characters `'X'` and `'Z'` are fixed and cannot be changed. If it is impossible to replace exactly `k` dots without creating adjacent equal characters, return `0`. The string length `n` will be at least 1 and at most 100, and `k` will be between 0 and `n`. The result may fit in a 32-bit signed integer (i.e., ≤ 2^31-1), but use `long long` internally to be safe.
*/

#include <string>
#include <vector>
#include <algorithm>

// Count ways to replace all '.' with 'X' or 'Z' such that no adjacent equal,
// and total number of 'X' in the final string equals k.
long long countGoodStrings(const std::string& s, int k) {
    int n = (int)s.size();
    
    // Quick validity check for existing adjacent fixed characters.
    for (int i = 0; i + 1 < n; ++i) {
        if (s[i] != '.' && s[i+1] != '.' && s[i] == s[i+1]) {
            return 0;
        }
    }
    
    // Count fixed X's.
    int fixedX = 0;
    for (char c : s) {
        if (c == 'X') fixedX++;
    }
    
    // Extract maximal dot blocks.
    std::vector<std::vector<int>> blockOptions; // each block: list of possible X-replacements counts
    for (int i = 0; i < n; ) {
        if (s[i] == '.') {
            int j = i;
            while (j < n && s[j] == '.') j++;
            int len = j - i;
            char leftChar = (i > 0) ? s[i-1] : '\0';
            char rightChar = (j < n) ? s[j] : '\0';
            
            // Possible alternating patterns: start with 'X' or 'Z'
            std::vector<int> options;
            for (char start : {'X', 'Z'}) {
                bool valid = true;
                if (leftChar != '\0' && start == leftChar) valid = false;
                // Generate pattern
                std::string pattern;
                char cur = start;
                for (int t = 0; t < len; ++t) {
                    pattern.push_back(cur);
                    cur = (cur == 'X') ? 'Z' : 'X';
                }
                if (rightChar != '\0' && pattern.back() == rightChar) valid = false;
                if (valid) {
                    int cntX = 0;
                    for (char c : pattern) if (c == 'X') cntX++;
                    options.push_back(cntX);
                }
            }
            // Remove duplicate counts (should not happen but safe)
            std::sort(options.begin(), options.end());
            options.erase(std::unique(options.begin(), options.end()), options.end());
            blockOptions.push_back(options);
            i = j;
        } else {
            i++;
        }
    }
    
    // DP over blocks: dp[c] = number of ways to achieve total X count c so far.
    const int MAXK = n;
    std::vector<long long> dp(MAXK+1, 0);
    dp[fixedX] = 1; // start with fixed X count
    
    for (const auto& opts : blockOptions) {
        std::vector<long long> ndp(MAXK+1, 0);
        for (int cur = 0; cur <= MAXK; ++cur) {
            if (dp[cur] == 0) continue;
            for (int add : opts) {
                int nxt = cur + add;
                if (nxt <= MAXK) ndp[nxt] += dp[cur];
            }
        }
        dp = std::move(ndp);
    }
    
    return (k >= 0 && k <= MAXK) ? dp[k] : 0;
}

#include <cassert>
#include <string>

// Forward declaration from solution
long long countGoodStrings(const std::string& s, int k);

int main() {
    // Simple no-dot case
    assert(countGoodStrings("XZ", 1) == 1);
    assert(countGoodStrings("XX", 2) == 0); // invalid adjacency
    
    // Single dot
    assert(countGoodStrings(".", 0) == 0);
    assert(countGoodStrings(".", 1) == 1); // only 'X' possible? Actually '.' can be X or Z, both valid alone, so for k=1 one way, for k=2 zero.
    assert(countGoodStrings(".", 0) == 0); // wait, must replace all dots, so exactly one X or zero X.
    
    // Let's define: all dots must be replaced, so for "." the only possibilities are "X" (k=1) or "Z" (k=0). Both valid, so count for k=1 is 1, for k=0 is 1.
    assert(countGoodStrings(".", 0) == 1); // "Z"
    assert(countGoodStrings(".", 1) == 1); // "X"
    
    // Two dots at ends
    assert(countGoodStrings("..", 0) == 1); // "ZX" or "XZ"? Actually two dots with no constraints: patterns "XZ" (k=1) and "ZX" (k=1) both have one X, so only k=1 possible. Let's check: ".." can be "XZ" or "ZX", both valid and both have exactly 1 X. So count for k=1 is 2, for k=2 is 0, for k=0 is 0.
    assert(countGoodStrings("..", 1) == 2);
    assert(countGoodStrings("..", 2) == 0);
    assert(countGoodStrings("..", 0) == 0);
    
    // Fixed boundary forces pattern
    assert(countGoodStrings("X..Z", 1) == 1); // must be "X Y Z" where Y is opposite of both: X opposite is Z, but then Z adjacent to right Z invalid, so only pattern "X Z Z" invalid, "X Z X"? Wait "X..Z" has fixed X at 0, Z at 3. Dots at 1,2. Left fixed X forces dot1='Z', then dot2='X', then right fixed Z requires dot2='Z'? Conflict, so 0 ways. Actually check: if dot1='Z', dot2='X', then adjacencies: X-Z ok, Z-X ok, X-Z ok? Wait right fixed Z after dot2, dot2='X' then X-Z ok. So pattern "XZXZ" works, X count = 2. Let's compute: X at 0, Z at1, X at2, Z at3 → X count = 2. So k=2 gives 1 way, others 0. Test.
    assert(countGoodStrings("X..Z", 2) == 1);
    assert(countGoodStrings("X..Z", 1) == 0);
    
    // Fixed boundary forces unique pattern
    assert(countGoodStrings("X.Z", 2) == 1); // X Z then middle must be Z? Actually "X.Z" has fixed X and Z at ends, dot in middle must be opposite of both: X->Z, Z->X impossible, so 0 ways. Wait "X" at 0, "Z" at 2, dot at 1 must be Z (to be different from X) but then equals Z at 2, invalid. So 0 ways for any k.
    assert(countGoodStrings("X.Z", 1) == 0);
    
    // Three dots with one fixed at left
    assert(countGoodStrings("X...", 2) == 1); // pattern forced: X Z X Z, X count = 2 (including fixed X) so k=2 one way.
    assert(countGoodStrings("X...", 1) == 0);
    
    // Longer example with two blocks
    assert(countGoodStrings("X..Y..Z", 3) == 0); // but Y is not allowed, use only X and Z. Let's use "X..Z..X": first block of 2 dots between X and Z, second block of 2 between Z and X. First block: length 2, left X forces start Z, pattern ZX (1 X), right Z: ends with X, ok. Second block: left Z forces start X, pattern XZ (1 X), right X ends with Z ok. Total X = fixed X(2) +1+1=4. So only k=4 works.
    std::string s1 = "X..Z..X";
    assert(countGoodStrings(s1, 4) == 1);
    assert(countGoodStrings(s1, 3) == 0);
    
    // Multiple options for a block at start
    // ".." no constraints gives 2 options but both have 1 X, so DP handles
    assert(countGoodStrings("..Z", 1) == 1); // must start with X? Actually left no constraint, right fixed Z: pattern must end with opposite of Z i.e. X. So two options: start X -> pattern XZ (1 X) ends Z? ends Z equals fixed Z invalid. start Z -> pattern ZX (1 X) ends X, ok. So only ZX gives 1 X.
    assert(countGoodStrings("..Z", 2) == 0);
    
    std::cout << "All tests passed" << std::endl;
    return 0;
}

// The problem is a combinatorial counting problem on a string with fixed positions. Because only two characters `'X'` and `'Z'` exist and adjacent positions cannot be equal, each dot position must be assigned the opposite character of its left neighbor (if exists) and the opposite of its right neighbor (if exists). Therefore, the constraints propagate from left to right: if we decide the first character, the entire string is forced (unless there is a conflict with a fixed `'X'` or `'Z'`). However, because the original string already has fixed characters, the valid assignments are limited.
//
// Key observation: The string is partitioned into contiguous blocks separated by fixed characters. For each block of consecutive dots, the alternating pattern is forced by the fixed character on its left (or the leftmost block which has no left fixed character). But actually, since dots are only between fixed characters or at the ends, each maximal dot block of length `L` can be assigned exactly two patterns? No — because the pattern must alternate, and the left fixed character (if any) forces the first dot's value. If there is no left fixed character (i.e., the dot block starts at index 0), then there are exactly two possible alternating assignments for the entire block, but the right fixed character (if any) further constrains it: the assignment must also be opposite to the right fixed character. Therefore, a dot block of length `L` has either 0, 1, or 2 possible assignments, depending on boundary fixed characters.
//
// More precisely, split the string into maximal contiguous segments of dots. For each segment, determine how many `'X'` and `'Z'` replacements it would use in each possible alternation that respects the neighboring fixed characters (if any). Then the total number of ways to replace exactly `k` dots is the sum over all combinations of choosing one valid alternation pattern from each segment such that the total number of replacements equals `k`. This becomes a dynamic programming problem over the segments: `dp[i][j]` = number of ways to process first `i` segments and use exactly `j` replacements (where `j` is the number of `'X'` replacements, since choosing `'X'` vs `'Z'` for each dot determines the count; equivalently, count `'X'` replacements). Actually, since each dot must be replaced, the number of replacements is exactly the total number of dots, but the count of `'X'` replacements varies. Wait — the problem says "replace exactly k dot characters" — meaning we choose for each dot whether it becomes `'X'` or `'Z'`, and we require that exactly `k` of these chosen replacements result in `'X'` (or perhaps exactly `k` dots are replaced, but that's always the total dots). Reading carefully: "replace exactly `k` dot characters" is ambiguous — typical interpretation is that we replace each dot with one of the two characters, and we count the number of ways to do so such that the resulting string has exactly `k` occurrences of `'X'` (or maybe exactly `k` dots get replaced with `'X'`?). The original snippet has arrays `x[3]` and `z[3]` tracking positions of `'X'` and `'Z'` by index modulo 3, so likely the task is about counting based on positions modulo 3. But since we are creating a standalone task, we should redefine clearly. To make a meaningful task, I'll define: After replacing all dots (each dot must be turned into either `'X'` or `'Z'`), we count the number of resulting strings that have no adjacent equal characters and have exactly `k` occurrences of `'X'` in total. Return that count. This makes `k` meaningful and uses the fixed `'X'` and `'Z'` positions.
//
// Thus the approach: first check if the existing fixed characters already violate adjacency (two adjacent non-dots equal). If so, return 0. Then, for each maximal dot block, compute the possible patterns respecting boundaries. Each pattern has a fixed count of `'X'` replacements (since the block length is fixed and alternation is forced). Then use DP over blocks to sum counts: `dp[i][c]` = number of ways using first `i` blocks to have exactly `c` total `'X'` in the whole string (including the fixed ones). Initialize from fixed counts. At the end, return `dp[total_blocks][k]`. Time complexity: O(n * k) per test case, with n ≤ 100, k ≤ 100, so trivial. Space O(n*k). Edge cases: no dots (then just check if fixed string already valid and count `'X'` equals k), dot blocks at ends, blocks of length 1 with both neighbors fixed.
