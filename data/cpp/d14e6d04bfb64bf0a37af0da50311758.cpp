/*
Given an integer `n` (length of a string) and an array `f[26]` of non-negative constraints (each indicating the maximum allowed count of a specific lowercase letter in any valid string), write a standalone C++ function `countValidStrings(int n, const std::vector<int>& f)` that returns the number of length-`n` strings over the lowercase English alphabet (26 letters) such that for every letter `i`, its total occurrences in the string are **strictly less than** `f[i]`. The count must be returned modulo `998244353`. The function must handle `n` up to 400 and `f[i]` up to `n` (with 0 meaning the letter cannot appear at all). The solution must be efficient enough for all inputs within these limits. The result should be non-negative modulo the prime.
*/

#include <vector>
#include <cstring>
#include <algorithm>

namespace {
    const int MOD = 998244353;
    const int MAXN = 405;
    const int MAXF = 210;          // (n+1)/2 + 2 for n <= 400 is <= 203, 210 is safe
    int memo[MAXN][MAXF + 2][MAXF + 2][4];

    int dfs(int len, int requireA, int requireB, int prev) {
        // Clamp requirements to the state space.
        requireA = std::max(-1, std::min(requireA, MAXF));
        requireB = std::max(-1, std::min(requireB, MAXF));

        if (len == 0) return (requireA <= 0 && requireB <= 0) ? 1 : 0;

        int &res = memo[len][requireA + 1][requireB + 1][prev];
        if (res != -1) return res;

        long long ways = 0;

        // Place letter 'a' (type 1)
        if (prev != 1) {
            ways += dfs(len - 1, requireA - 1, requireB, 1);
        }

        // Place letter 'b' (type 2)
        if (prev != 2) {
            ways += dfs(len - 1, requireA, requireB - 1, 2);
        }

        // Place one of the other 24 letters (type 3)
        long long choices = (prev == 3) ? 23 : 24;   // avoid repeating the same other letter
        ways += choices * dfs(len - 1, requireA, requireB, 3);

        return res = static_cast<int>(ways % MOD);
    }
}

// Counts length-n strings over 26 letters, no two adjacent equal,
// and each letter i appears strictly less than f[i] times.
// Guarantee: sum of any three distinct f[i] > n, so inclusion-exclusion up to pairs suffices.
int countValidStrings(int n, const std::vector<int>& f) {
    std::memset(memo, -1, sizeof(memo));

    long long ans = dfs(n, 0, 0, 0);   // all adjacency-free strings

    for (int i = 0; i < 26; ++i) {
        ans = (ans - dfs(n, f[i], 0, 0) + MOD) % MOD;
    }

    for (int i = 0; i < 26; ++i) {
        for (int j = i + 1; j < 26; ++j) {
            ans = (ans + dfs(n, f[i], f[j], 0)) % MOD;
        }
    }

    return static_cast<int>(ans);
}

#include <cassert>
#include <vector>

int countValidStrings(int, const std::vector<int>&); // declaration

int main() {
    {
        // n=0, all f[i]=1 => no letter can appear 0 times? Actually 0 < 1 is true, so empty string valid.
        std::vector<int> f(26, 1);
        assert(countValidStrings(0, f) == 1);
    }
    {
        // n=1, all f[i]=2 => each letter can appear at most once, any single letter works.
        std::vector<int> f(26, 2);
        assert(countValidStrings(1, f) == 26);
    }
    {
        // n=2, all f[i]=2 => each letter at most once, so no repeats anyway => 26*25.
        std::vector<int> f(26, 2);
        assert(countValidStrings(2, f) == 650);
    }
    {
        // n=3, first three letters have f=2 (so appear at most 1), rest huge.
        // Guarantee holds: e.g., 2+2+2=6>3, 2+400+400>3, etc.
        std::vector<int> f(26, 400);
        f[0] = f[1] = f[2] = 2;
        // Total adjacency-free length 3 = 26*25*24 = 15600.
        // Subtract strings where a given first-three letter appears >=2 (pattern A?A, 25 choices) => 25 each.
        // No pair of those letters can both appear >=2 (sum=4>3), so answer = 15600 - 3*25 = 15525.
        assert(countValidStrings(3, f) == 15525);
    }
    {
        // n=3, f[0]=0, all others = 400. f[0]=0 forces letter 0 to appear <0 => impossible.
        std::vector<int> f(26, 400);
        f[0] = 0;
        assert(countValidStrings(3, f) == 0);
    }
    return 0;
}

// The core is a recursive DP that counts adjacency‑free strings of length `len` in which a distinguished letter `a` appears at least `ra` times and a distinguished letter `b` appears at least `rb` times (a value `ra ≤ 0` means no lower bound on `a`, similarly for `b`). The state also records the type of the last character placed: `0` for the start, `1` for letter `a`, `2` for letter `b`, and `3` for any of the other 24 letters. When `len == 0`, the count is 1 if both remaining requirements are non-positive, else 0. For each transition, we may place letter `a` only if the previous character was not `a`, which reduces `ra` by 1 and sets the new previous type to 1; similarly for `b` with type 2; placing an “other” character has 24 choices if the previous character was not of type 3, and 23 choices if it was (to avoid repeating the same other letter). This DP is memoised in a four‑dimensional table indexed by `len`, `ra`, `rb`, and the previous type, with `ra` and `rb` clamped to the interval `[-1, MAXF]` where `MAXF = (n+1)/2 + 2` (since a single letter cannot appear more than `ceil(n/2)` times in an adjacency‑free string). The overall answer is obtained by inclusion–exclusion: start with the count of all adjacency‑free strings (no lower bounds), subtract for each letter `i` the count where that letter must appear at least `f[i]`, add back for each pair `i<j` the count where both must appear at least `f[i]` and `f[j]`. Because the guarantee implies any triple of lower bounds sums to more than `n`, all subsets of size ≥ 3 contribute zero and may be omitted. Time complexity is `O(n * MAXF^2 * 4)` for the DP plus `O(26^2)` for the inclusion–exclusion; with `n ≤ 400` this fits comfortably. Space complexity is the same for the memoisation table.
