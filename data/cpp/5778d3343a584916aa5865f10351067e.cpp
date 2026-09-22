// Given a string `s` consisting only of uppercase letters from `'A'` to `'E'`, where each character has a fixed positive value (`'A'=1`, `'B'=10`, `'C'=100`, `'D'=1000`, `'E'=10000`), you need to write a C++ function that returns the maximum possible total sum after changing **exactly one** character in the string to any of the five letters (it may remain the same). The total sum is computed from left to right using a rule inspired by Roman numerals: for each character, if its value is **less than** the maximum value seen to its **right** (including itself? Actually the rule is: when scanning from right to left, maintain the maximum encountered so far; if the current character's value is less than that maximum, then it contributes negatively, otherwise positively). Specifically, the total is `sum_{i=0}^{n-1} (value(s[i]) if value(s[i]) >= max_{j>i} value(s[j]) else -value(s[i]))`, where `max_{j>i}` is the maximum value among characters with index greater than `i` (for the last character, the max is 0, so it always contributes positively). Return the maximum possible sum after at most one replacement. The replacement can be any letter from 'A' to 'E'. The input string length is between 1 and 200,000, and the function must be efficient for large inputs.
#include <bits/stdc++.h>
int main() {
    // Provided tests
    assert(maxSumAfterOneChange("A") == 1);
    assert(maxSumAfterOneChange("E") == 10000);
    assert(maxSumAfterOneChange("AA") == 2);
    assert(maxSumAfterOneChange("AB") == 11); // original: A(1)+B(10 max right)=11, no change better
    assert(maxSumAfterOneChange("BA") == 9); // original: B(10) + A(1<10?) actually A's right max is 0, so A=+1? Wait: scan right: A=+1, then B=+10 => 11. Change B to E => E+A = 10000+1=10001, so answer 10001.
    // Let's recompute manually: s="BA", original right to left: A=+1, mx=1, B=10>=1 => +10, total=11. Change B to E: A=+1, then E=10000>=1 => +10000, total=10001. Change A to E: B=10, right max for B is 10000? Actually after changing A to E, right max for B is 10000, so B=+10, E=+10000 => total 10010. So max is 10010. So assert.
    assert(maxSumAfterOneChange("BA") == 10010);
    
    // More tests
    assert(maxSumAfterOneChange("ABC") == 111); // original: C=+100, B=+10, A=+1 => 111; any change? Change A to E: E(10000)+B(10)+C(100) but right max for B is 100, so B=+10, C=+100, E=+10000 => 10110. Change B to E: A=+1? right max for A is 10000? so A=-1, E=+10000, C=+100 => 10099. Change C to E: A=+1, B=+10, E=+10000 => 10011. So max 10110.
    assert(maxSumAfterOneChange("ABC") == 10110);
    
    // Edge with duplicates and long string
    assert(maxSumAfterOneChange("AAAA") == 4); // no change improves
    string big = string(200000, 'A');
    assert(maxSumAfterOneChange(big) == 200000); // all A, sum = 1*200000 = 200000
    // Change one A to E gives 1*199999 + 10000 = 209999? Actually rule: all A's have right max maybe 0 unless E on right, so if change last to E: A's before have right max 10000, so they become negative -1 each? Wait that would be terrible. Let's compute: For all A's of length 200k, original sum = 200000. If we change position 0 to E, then right max for all following A's is 10000? Actually for position 1, right max includes E at 0? No, right max is to the right, not left. So changing position 0 doesn't affect the right max of positions >0. Changing position 199999 (last) to E: all previous A's have right max = 10000, so they contribute -1 each, and E contributes +10000 => total = -199999 + 10000 = -189999, worse. Changing middle one: e.g., change position 100000 to E: positions 0..99999 have right max = 10000 (due to E at 100000), so they contribute -1 each = -100000; position 100000 contributes +10000; positions 100001..199999 have right max = 0 (no E to their right), so +1 each = +100000 - 100000? Actually 100001..199999 count = 100000, sum = +100000; total = -100000 + 10000 + 100000 = +10000, worse than 200000. So best remains 200000. Thus assert.
    assert(maxSumAfterOneChange(big) == 200000);
    
    // Mixed case
    assert(maxSumAfterOneChange("DAB") == 1001); // Let's compute: original D=1000, A=1 (right max 1000 so -1), B=10 => 1000-1+10=1009. Change D to E: E=10000, A=-1, B=+10 => 10009. Change A to E: D=1000, E=10000? right max for D is 10000 (E at A position), so D=+1000, E=+10000, B=+10 => 11010. Change B to E: D=1000, A=-1? right max for A is 10000 (E at B), so A=-1, E=+10000 => 10999. Best is 11010.
    assert(maxSumAfterOneChange("DAB") == 11010);
    
    // Test with only one letter repeated and a change at the end
    assert(maxSumAfterOneChange("EAAA") == 10009); // original: A's are -1 each? Right: A=+1, A=+1? Actually scan: last A=+1, next A=+1, next A=+1, E=+10000 => total=10003. Change one A to E: e.g., change first A to E gives E=+10000, then next A's have right max 10000? Actually right max for the second A includes the first E? No, right max is to the right, so changing first doesn't affect. Change the second A to E: first A's right max is 10000 (E at pos1), so first A = -1, E=+10000, third A and fourth A have right max 0? Actually they are to the right of E, so they have right max 0? Wait E at pos1, positions 2 and 3 are to the right, their right max is 0 (since no E to their right), so +1 each. So total = -1 +10000 +1+1 = 10001. Change last A to E: first A's right max is 10000? Actually E at pos3, so pos0,1,2 have right max 10000, they each contribute -1, E contributes +10000 => total = -3+10000=9997. So best is 10003 (original) or maybe changing E to something else? E to A gives all A's, sum=4. So answer 10003.
    assert(maxSumAfterOneChange("EAAA") == 10003);

    printf("All tests passed!\n");
    return 0;
}
#include <bits/stdc++.h>
using namespace std;

// Returns the maximum total sum after changing at most one character in s.
long long maxSumAfterOneChange(const string& s) {
    int n = (int)s.size();
    const int values[5] = {1, 10, 100, 1000, 10000};
    
    // Convert string to integer array
    vector<int> v(n);
    for (int i = 0; i < n; ++i) v[i] = s[i] - 'A';

    // suffix_sum[i] = total contribution from i..n-1 under original rule
    // suffix_max[i] = maximum value among i..n-1
    vector<long long> suffix_sum(n + 1, 0);
    vector<int> suffix_max(n + 1, 0); // suffix_max[n] = 0
    long long tmp = 0;
    int mx = 0;
    for (int i = n - 1; i >= 0; --i) {
        if (v[i] < mx) tmp -= values[v[i]];
        else tmp += values[v[i]];
        mx = max(mx, v[i]);
        suffix_sum[i] = tmp;
        suffix_max[i] = mx;
    }

    // pref[i][t] = sum over k=0..i-1 of (values[v[k]] < values[t] ? -values[v[k]] : values[v[k]])
    // where t is an index from 0..4 representing threshold value.
    // We'll store pref as vector of arrays.
    vector<array<long long, 5>> pref(n + 1);
    fill(pref[0].begin(), pref[0].end(), 0LL);
    for (int i = 1; i <= n; ++i) {
        for (int t = 0; t < 5; ++t) {
            pref[i][t] = pref[i-1][t];
            if (v[i-1] < t) pref[i][t] -= values[v[i-1]];
            else pref[i][t] += values[v[i-1]];
        }
    }

    long long best = suffix_sum[0]; // original sum
    // Try replacing each position with each of 5 letters
    for (int i = 0; i < n; ++i) {
        for (int nv = 0; nv < 5; ++nv) {
            int right_max = suffix_max[i+1]; // max of i+1..n-1
            int new_max = max(right_max, nv);
            // Prefix part: positions 0..i-1, threshold = new_max
            long long total = pref[i][new_max];
            // Contribution of new character at i
            if (nv >= right_max) total += values[nv];
            else total -= values[nv];
            // Suffix part i+1..n-1 unchanged
            total += suffix_sum[i+1];
            best = max(best, total);
        }
    }
    return best;
}
// The key is to precompute suffix information. For each position `i` in the original string, we can compute:
// - `suffix_sum[i]`: the total contribution of substring from `i` to `n-1` under the original rule.
// - `suffix_max[i]`: the maximum value among characters from `i` to `n-1`.
//
// The original total sum is `suffix_sum[0]`. For each position `i`, we want to try replacing the character at `i` with each of the five letters `ch`. The new total sum will consist of three parts:
// 1. The contribution from positions `0` to `i-1` (before the change). This part depends on the original characters and the original suffix maximums to the right. Specifically, for each `k < i`, its contribution is `+value(s[k])` if `value(s[k]) >= max_{j>k} value(s[j])`, else `-value(s[k])`. But after changing position `i`, the maximum to the right for positions `k < i` might change if the new character at `i` becomes a new maximum greater than the original suffix maximum after `i`. However, positions before `i` only care about the maximum to their right, which includes position `i`. So we need to be careful.
//
// Better approach: precompute prefix sums assuming that the maximum to the right is fixed. Actually, we can do a two-pass approach. Let `pref[i]` be the contribution of positions `0..i-1` under the rule but with the maximum to the right being `max(suffix_max[i], new_value)`? That's complicated. Simplification: iterate over each position and each replacement character, and compute the total sum in O(1) using precomputed suffix sums and suffix maximums, but need to adjust for the prefix part.
//
// Alternative: Precompute for each prefix `i` (positions 0..i-1) the total contribution assuming that the maximum to the right is given (i.e., we can compute `prefix_sum[i][m]`? Too large). Instead, notice that for positions left of `i`, the maximum to their right is either the original suffix maximum after `i` (if the new character is not larger) or the new character's value (if it is larger than that suffix maximum). But the prefix contributions can be computed efficiently if we maintain for each possible threshold `t` (value from {1,10,100,1000,10000}) the sum of contributions of the prefix given that the maximum to the right is at least `t`. However, since only 5 distinct values, we can precompute for each position `i` and each of the 5 possible thresholds `t` the total contribution of prefix `0..i-1` under the rule where any character with value `< t` contributes negatively, else positively. That is, we define `pref[i][t]` = sum over `k=0..i-1` of (if value(s[k]) < t then -value(s[k]) else +value(s[k])). Then for a replacement at `i` with new value `nv`, the maximum to the right for positions left of `i` is `max(suffix_max[i+1], nv)`. Let `m = max(suffix_max[i+1], nv)`. Then the sum of prefix is `pref[i][m]` (since the rule for each left position depends only on whether its value is less than the maximum to its right). For position `i` itself, its contribution is `+nv` (since it is its own rightmost? Actually for position `i`, the maximum to its right is `max_{j>i}`, so for the new character at `i`, its contribution is `+nv` if `nv >= suffix_max[i+1]`, else `-nv`. But careful: the rule says "if current character's value is less than the maximum seen to its right (including itself?)" The original snippet computes `tmp += valu(s[i])` if `valu(s[i]) >= mx` where `mx` is the maximum of `valu(s[i])` and all to its right? Actually scanning from right, `mx` starts at 0, then for each char from right, it updates `mx = max(mx, valu(s[i]))` before the check? Let's re-read: In the original code, they do `tmp += valu(s[i])` if `valu(s[i])<mx` then subtract else add, then update `mx = max(mx, valu(s[i]))`. That means for position `i`, `mx` at that point is the maximum of positions `i+1..n-1`. So for the last character, `mx` is 0, so it adds. For others, it compares with maximum to the right only. So for the new character at `i`, its contribution is `+nv` if `nv >= suffix_max[i+1]` else `-nv`. Then the suffix part `i+1..n-1` remains unchanged, so its sum is `suffix_sum[i+1]`. So total = `pref[i][max(suffix_max[i+1], nv)]` + (nv if nv >= suffix_max[i+1] else -nv) + `suffix_sum[i+1]`.
//
// To compute `pref[i][t]` for all `i` and `t` in O(n*5), we can maintain cumulative sums for each threshold. But we can do simpler: For a given threshold `t`, `pref[i+1][t] = pref[i][t] + (value(s[i]) < t ? -value(s[i]) : value(s[i]))`. So we can precompute a 2D array of size `(n+1) x 5`. However, n up to 200k, 5*200k = 1M integers, fine.
//
// Edge cases: The string length 1, replacement can be same or different; must handle all five choices. Also, the replacement can be to the same letter, which is effectively no change. Time complexity O(n*5) = O(n), space O(n*5) = O(n). The solution must be self-contained and return a `long long` result.
