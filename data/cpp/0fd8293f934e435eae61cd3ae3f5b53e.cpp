Write a C++ function `int maxDifferenceBounded(const std::string& s, int k)` that, given a string `s` consisting only of characters `'0'` through `'4'`, and a positive integer `k`, returns the maximum value of `(frequency of 'a' in a substring) - (frequency of 'b' in the same substring)` over all substrings of `s` that have length at least `k`, where `a` and `b` are distinct digits from `'0'` to `'4'`, and additionally the substring must contain the digit `b` an even number of times. In other words, for each choice of two different digits `a` and `b`, consider every contiguous substring of `s` of length `≥ k` where the count of `b` is even; compute `count_a - count_b` for that substring; the answer is the maximum such value over all choices of `a`, `b`, and valid substrings. If no valid substring exists, return a very small number (e.g., `INT_MIN`). The string length is between 1 and 100,000, and `k` is between 1 and the string length.

// The problem requires optimizing over two dimensions: choice of digit pair `(a, b)` and substring position. A naive O(n²) per pair would be too slow. Instead, for each fixed pair `(a, b)`, we process the string with a sliding window of minimum length `k`, while maintaining prefix counts of `a` and `b` up to the left boundary. The key insight is that the even‑count-of-`b` constraint can be handled using a parity state: for any possible left boundary, we only care about the parity of `b`'s count in the prefix before that boundary, because the full substring's `b` count is even iff the prefix's `b` parity equals the right boundary's `b` parity. For each possible parity status of the prefix (encoded as 2 bits: bit1 = parity of `a`, bit0 = parity of `b`), we keep the minimum value of `(prefix_a - prefix_b)` among all left boundaries that are at least `k` away from the current right boundary and satisfy the `b` count in the gap (`cnt_b - prev_b`) being at least 2 (this ensures the substring length is at least `k`? Actually the while condition `right - left >= k && cnt_b - prev_b >= 2` is from the snippet; but for our task, we must adapt. In the original snippet, the condition enforces both a minimum length and that the `b` count in the gap is at least 2, but that seems specific to their original problem. For our standalone task, the substring length must be at least `k`, and the total `b` count in the substring must be even. So we use a sliding window that maintains left pointer such that `right - left >= k` always holds when we attempt to update, but we also need to track the parity of `b` count in the prefix processed. The approach: for each right index, we consider all left indices `L` with `right - L + 1 >= k` (i.e., `L <= right - k + 1`). For each such `L`, the substring `s[L..right]` must have even `b` count. That is equivalent to `prefix_b[right] - prefix_b[L-1]` even, i.e., `prefix_b[right] % 2 == prefix_b[L-1] % 2`. Similarly, the value we want is `(prefix_a[right] - prefix_a[L-1]) - (prefix_b[right] - prefix_b[L-1])`. For a fixed right, this equals `(prefix_a[right] - prefix_b[right]) - (prefix_a[L-1] - prefix_b[L-1])`. So we need, for each possible parity of `prefix_a[L-1]` and `prefix_b[L-1]`, the minimum `prefix_a[L-1] - prefix_b[L-1]` among all `L-1` indices such that `L <= right - k + 1`. Then at right, we look up the parity status that makes the `b` parity even: if `right_b_parity` is `p`, then we need `left_b_parity == p`. Also `a` parity can be anything. So we match on the pair `(left_a_parity, left_b_parity)` where `left_b_parity == right_b_parity`. Among those, pick the minimum `left_value`, and update answer with `(prefix_a[right] - prefix_b[right]) - min_left`. To maintain the set of candidate left boundaries, we maintain a queue of indices that are eligible (i.e., their `L` satisfies `right - L + 1 >= k`). We slide `left` pointer as `right` increases, adding each new `left-1` position into a best array indexed by its parity status. We must also ensure that we only consider positions `L-1` that are at least `k` before `right`; so we add a new left boundary when `right - left >= k`? Actually, we iterate `right` from 0 to n-1. We need to have added all left boundaries `L` such that `L <= right - k + 1`. So as `right` increases, we add the left boundary `left = right - k + 1` when it becomes non-negative. But careful: we need to add `left-1` (the prefix index before `L`). For simplicity, we can precompute prefix arrays and then for each `right`, we only consider `L` values from 0 to `right-k+1`. We can maintain a pointer `addIdx` that goes from 0 upward; for each `right`, we add the prefix index `addIdx` if `addIdx + 1 <= right - k + 1` (since `L = addIdx+1`). But it's easier to use a two-pointer approach: We keep an array `best[4]` initialized to `INT_MAX`. We also keep a queue of candidate prefix indices. For each `right`, we first add the prefix index `right - k` if it's >=0 (because `L = right-k+1` gives `L-1 = right-k`). Actually let's define prefix index `i` meaning prefix up to position `i` inclusive (i from -1 to n-1). For a substring `L..right` with `L>=0`, the prefix before `L` is index `L-1`. So for a given `right`, eligible `L` are those with `L <= right-k+1`, so `L-1 <= right-k`. So we need to add all prefix indices `i` from -1 up to `right-k` into the best array. Start with `i=-1` (prefix before the first character) having counts 0,0, status (0,0). We can store best for index -1 initially. Then as `right` increments, we add prefix index `right-k` (if >= -1) into the best. But wait, `right` starts at 0; for `right` small, `right-k` might be negative, so we only add when `right >= k-1`? Let's think: For `right = k-1`, `right-k = -1`, so we can add index -1 (the empty prefix). For `right = k`, `right-k = 0`, add prefix index 0 (which includes s[0]). So we add each prefix index exactly once. That works. Then for each `right`, we have all prefix indices up to `right-k` available in `best`. Then we compute current prefix counts up to `right`, get its status, and look up `best` for the entry with same `b` parity (bit0) but any `a` parity? Actually because we need the substring's `b` count even, we need `left_b_parity == right_b_parity`. `a` parity can be anything, so we consider both possible `a` parities for the given `b` parity. So we look at statuses `(0, p)` and `(1, p)` where `p = right_b_parity`. Take the minimum of those two best values (if not INT_MAX). Then candidate = (current_a - current_b) - that min. Update answer. After processing all `right` for a fixed pair `(a,b)`, we move to next pair. We must also ensure the substring has length at least `k`; our addition of prefix indices up to `right-k` guarantees that any `L-1` used corresponds to `L = (i+1) <= right-k+1`, so substring length `right - L + 1 >= k`. However, we also need to ensure that the substring is valid: it must have at least one character, but that's automatic. Also note that we require the `b` count to be even; our parity matching ensures that. For pairs where `a` and `b` are distinct, we try all 5*4=20 combos. Complexity: O(20 * n) = O(n), with constant 20. Space O(1) for `best` and prefix counts, but we need to store prefix counts for the current right? Actually we can compute prefix counts on the fly for each right using cumulative counts, but we need the prefix counts for the index being added (which is `right-k`). So we can precompute two arrays `prefA` and `prefB` for a given `a,b`? But we don't know `a` and `b` in advance. However we can compute prefix counts for each digit independently? That would be O(5n) space and time, which is fine for n up to 100k. But we can also just iterate `right` and compute current counts using running totals, but we need prefix counts for index `right-k` later. Since we add `right-k` exactly once when `right` reaches that value, we can keep an array `prefA` and `prefB` for each digit? Actually we can precompute for each digit `d` an array `pref[d][n]` of counts? That would be 5*100k = 500k ints, fine. But to keep the solution simple, we can compute prefix counts for the specific `a` and `b` within the loop by precomputing cumulative counts for each digit once. Let's do that: We first create `vector<vector<int>> pref(5, vector<int>(n+1))` where `pref[d][i]` = count of digit `d` in first `i` characters (prefix length i). Then for each pair `(a,b)`, we can get `cnt_a` at index `right+1` as `pref[a][right+1]`, and for prefix index `i` (which is `right-k`), it's `pref[a][i+1]` (since prefix length i+1). This is O(1) per access. Then we can process all pairs. Edge case: if no valid substring exists (e.g., when `k` is large and for all pairs no even count of `b`), return INT_MIN. Also, the original snippet uses status encoding with `getStatus` as `(cnt_a&1)<<1 | (cnt_b&1)`, so bit1 is parity of a, bit0 parity of b. We follow that. Time: O(20 * n) = O(n) with constant 20. Space: O(5*(n+1)) for prefix arrays, which is O(n) but with small constant.

#include <vector>
#include <string>
#include <climits>
#include <algorithm>

// Returns the maximum (count_a - count_b) over all substrings of length >= k
// where a and b are distinct digits '0'..'4', and the substring contains an even number of b.
// If no such substring exists, returns INT_MIN.
int maxDifferenceBounded(const std::string& s, int k) {
    const int n = static_cast<int>(s.size());
    // prefix counts for each digit: pref[d][i] = count of digit d in first i characters (s[0..i-1])
    std::vector<std::vector<int>> pref(5, std::vector<int>(n + 1, 0));
    for (int i = 0; i < n; ++i) {
        for (int d = 0; d < 5; ++d) {
            pref[d][i + 1] = pref[d][i] + (s[i] == ('0' + d) ? 1 : 0);
        }
    }

    int ans = INT_MIN;

    // Try every ordered pair of distinct digits (a, b)
    for (int a = 0; a < 5; ++a) {
        for (int b = 0; b < 5; ++b) {
            if (a == b) continue;

            // best[status] = minimum (prefix_a - prefix_b) for a prefix index that
            // is at least k-1 before the current right position.
            // status encoding: bit1 = parity of prefix_a, bit0 = parity of prefix_b.
            int best[4] = {INT_MAX, INT_MAX, INT_MAX, INT_MAX};

            // Prefix index to add next: it is (right - k) when that becomes >= -1.
            // We add prefix index i (meaning prefix length i+1) when i+1 <= right-k+1,
            // i.e., i <= right - k. So we maintain a pointer addIdx from -1 upward.
            int addIdx = -1;  // the next prefix index to insert into best

            for (int right = 0; right < n; ++right) {
                // Add all prefix indices up to (right - k) into best.
                // The prefix index i is valid for substrings that end at right and have length >= k,
                // because then left = i+1 satisfies right - left + 1 >= k -> i <= right - k.
                while (addIdx <= right - k) {
                    int i = addIdx;
                    // For i == -1, prefix counts are zero.
                    int pa = (i == -1) ? 0 : pref[a][i + 1];
                    int pb = (i == -1) ? 0 : pref[b][i + 1];
                    int status = ((pa & 1) << 1) | (pb & 1);
                    best[status] = std::min(best[status], pa - pb);
                    ++addIdx;
                }

                // Current prefix counts up to right (inclusive), i.e., prefix length right+1.
                int ca = pref[a][right + 1];
                int cb = pref[b][right + 1];
                int right_status = ((ca & 1) << 1) | (cb & 1);
                int right_b_parity = cb & 1;

                // We need left_b_parity == right_b_parity to have even b count in substring.
                // a parity can be 0 or 1, so check both statuses.
                for (int pa_par = 0; pa_par < 2; ++pa_par) {
                    int need_status = (pa_par << 1) | right_b_parity;
                    if (best[need_status] != INT_MAX) {
                        ans = std::max(ans, ca - cb - best[need_status]);
                    }
                }
            }
        }
    }

    return ans;
}

#include <cassert>
#include <string>
#include <climits>

int main() {
    // Single character string, k=1, only one pair possible? For a='0', b='1' -> substring "0" has 1 '0', 0 '1' (even), length 1 >=1 -> value 1. Other pairs no substring with even b. So answer is 1.
    assert(maxDifferenceBounded("0", 1) == 1);
    // "1", k=1: for a='1', b='0' -> substring "1" has 1 '1', 0 '0' (even) -> value 1. So max 1.
    assert(maxDifferenceBounded("1", 1) == 1);
    // "01", k=1: consider a='0', b='1': substrings: "0" (1-0=1), "1" (0-1=-1), "01" (1-1=0) but b count in "01" is 1 (odd) invalid. So max from valid is 1. Also a='1', b='0': "1" gives 1, "0" gives -1, "01" gives -1? Actually "01" for a='1', b='0' gives count_a - count_b = 1 - 1 =0 but b count is 1 (odd) invalid. So max 1.
    assert(maxDifferenceBounded("01", 1) == 1);
    // "00", k=1: pairs (a='0',b='1') not present -> no b, so substrings of '0' only have b=0 even -> value = length. Substring "00" length2 has 2-0=2; length1 has 1. So max 2.
    assert(maxDifferenceBounded("00", 1) == 2);
    // "12", k=1: a='1',b='2': "1" gives 1, "12" gives 0 (b=1 odd) invalid, "2" gives -1. So max 1. Similarly a='2',b='1' gives max 1. So answer 1.
    assert(maxDifferenceBounded("12", 1) == 1);
    // "010", k=2: Check a='1', b='0': substrings length>=2: "01" (b count 1 odd) invalid; "10" (b count 1 odd) invalid; "010" (b count 2 even) -> count_a=1, count_b=2 => -1. Others? a='0',b='1': "01" (b=1 odd) invalid; "10" (b=1 odd) invalid; "010" (b=1 odd) invalid. So no valid substrings -> INT_MIN.
    assert(maxDifferenceBounded("010", 2) == INT_MIN);
    // "0001", k=2: a='0',b='1': substrings len>=2: "0001" (b=1 odd) invalid; "000" (valid) gives 3; "001" (b=1 odd) invalid; "00" (valid) gives 2; "01" (b=1 odd) invalid. So max 3. Also a='1',b='0': substring "0001" has a=1,b=3 odd invalid; "001" a=1,b=2 even -> -1; "01" a=1,b=1 odd invalid; so max maybe -1. So answer 3.
    assert(maxDifferenceBounded("0001", 2) == 3);
    // "1234", k=2: For a='1',b='2': "12" gives 0 (b=1 odd) invalid; "1234" gives 1-1=0 (b=1 odd) invalid; "23" for a='2',b='3' etc. But consider a='4',b='3': "34" gives 1-1=0 odd invalid; "1234" gives 1-1=0 odd invalid. Maybe no valid substrings? Let's check a='2',b='1': substring "23" has b=0 even, a=1 -> 1; length2. So answer 1.
    assert(maxDifferenceBounded("1234", 2) == 1);
    // Large test for performance: all '0's of length 1000, k=10, only pair (a='0',b='1') not present, so substrings of '0' have b=0 even, value = length. Max substring length >=10 is 1000, so answer 1000.
    std::string big(1000, '0');
    assert(maxDifferenceBounded(big, 10) == 1000);
    // Edge: k > n: no valid substring -> INT_MIN.
    assert(maxDifferenceBounded("0", 2) == INT_MIN);
    return 0;
}
