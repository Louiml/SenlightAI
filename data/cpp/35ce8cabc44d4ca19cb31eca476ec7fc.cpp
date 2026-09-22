Write a C++ function `vector<long long> maxScores(const string& s, int k)` that takes a binary string `s` of length `n` (1-indexed internally for convenience) and an integer `k` (0 ≤ k ≤ n), and returns a vector `ans` of length `n` where `ans[i]` is the maximum possible value of `bestLen * i + bestLen` (with `bestLen` defined as the length of a contiguous substring that can be obtained by flipping at most `k` zeros to ones, and the score is computed as `bestLen * i + bestLen` for each `i` from 1 to n, maximizing over all choices of that contiguous substring). In other words, for each possible "multiplier" `i` (1-indexed position in output), you may choose any contiguous segment of the string, flip at most `k` zeros in it to ones, and the value contributed is `(flippedSegmentLength) * i + (flippedSegmentLength)`. The output for index `i` is the maximum over all such segments. The string contains only characters '0' and '1'. The function must handle multiple test cases internally but as a single function call per test case; the input `s` and `k` are given, and the function returns the vector of `n` long long values. The algorithm must be efficient for `n` up to 3000, and the function must be self-contained (no external global state).

// The core idea is to precompute for every prefix and suffix the maximum length of a contiguous substring that can be made all ones using at most `j` flips (where `j` ranges from 0 to n). This is done using two dynamic programming tables:
// - `pdp[i][j]`: maximum length of a contiguous segment ending at or before position `i` that can be made all ones with at most `j` flips. This is computed by extending the best from `pdp[i-1][j]` and also considering the longest segment ending exactly at `i` that can be fixed with at most `j` flips. To find that longest segment, binary search the leftmost starting position such that the number of zeros in `[l, i]` ≤ `j`. The number of zeros is `(segment length) - (number of ones)` which equals `(i - l + 1) - (pref[i] - pref[l-1])` where `pref` is prefix sum of ones. The binary search finds the smallest `l` such that zeros ≤ `j`, and then the segment length is `i - l + 1`. 
// - `sdp[i][j]`: symmetric for suffixes, largest segment starting at or after position `i` that can be fixed with at most `j` flips.
// After building these tables, we iterate over all possible segment lengths `len` from 0 to n. For each `len`, we find the maximum achievable "base" length that can be combined with this segment when we put this segment somewhere in the string and use the remaining flips outside the segment. Specifically, for each possible starting position `pos` of a segment of length `len`, we consider the zeros inside that segment: `zerosInside = len - (pref[pos] - pref[pos-len])`. The flips used there are `zerosInside`, leaving `d = k - zerosInside` flips for the rest of the string outside this segment. Then the best we can do outside is the maximum of `pdp[pos - len][d]` (best segment entirely before the chosen segment) and `sdp[pos + 1][d]` (best segment entirely after). The "bestLen" for this configuration is `max(segLen, bestOutside)`. For each `len`, we compute the maximum `bestLen` over all positions and both options (including the case where we don't use a segment of that length and just take `pdp[n][k]` for `len=0`). Then for each output index `i` from 1 to n, we update `ans[i] = max(ans[i], bestLen * i + len)`. This works because for a fixed `len`, the contribution `bestLen * i + len` is linear in `i`, so we can update all `i` with the same bestLen. Edge cases include `k` larger than number of zeros (then `d` can exceed n, but we cap flips at n since no need), `len=0` meaning we skip the segment and just use the best overall segment, and ensuring `d` is non-negative. Time complexity: O(n^2) for building pdp and sdp (each cell does a binary search O(log n), but since n=3000, O(n^2 log n) ~ 9e6*12 ≈ 1e8, which is acceptable in C++ but can be optimized to O(n^2) by precomputing the leftmost position for each i and j incrementally, but the given code uses binary search and is fine). The final loop over `len` and positions is O(n^2), and updating ans is O(n^2) (since for each len we loop over positions and then over all i, but the code given does the inner update O(n) per len, making O(n^2) total). Space is O(n^2) for the two tables and O(n) for ans.

#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

// Returns ans vector where ans[i] is maximum over all contiguous segments of
// (bestLen * i + len) with at most k flips, for i = 1..n.
std::vector<long long> maxScores(const std::string& s, int k) {
    int n = static_cast<int>(s.size());
    std::vector<long long> ans(n + 1, 0);

    // Prefix sums of ones (1-indexed for convenience)
    std::vector<int> pref(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        pref[i] = pref[i - 1] + (s[i - 1] == '1' ? 1 : 0);
    }

    // pdp[i][j] = max segment length that can be made all ones using at most j flips,
    // considering only prefix of length i.
    std::vector<std::vector<int>> pdp(n + 2, std::vector<int>(n + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j <= n; ++j) {
            pdp[i][j] = pdp[i - 1][j];
            // Find leftmost l such that zeros in [l, i] <= j
            int l = 0;
            int r = i;
            while (l != r) {
                int mid = (l + r) / 2;
                // zeros in [mid+1, i]? Actually using 1-indexed pref where pref[t] = number of ones in [1,t]
                // Segment [pos, i] where pos = mid+1 (since l is 0-indexed offset)
                int ones = pref[i] - pref[mid];
                int length = i - mid;
                if (length - ones > j) l = mid + 1;
                else r = mid;
            }
            // l is the smallest offset such that length - ones <= j
            int length = i - l;
            pdp[i][j] = std::max(pdp[i][j], length);
        }
    }

    // sdp[i][j] = max segment length that can be made all ones using at most j flips,
    // considering only suffix starting at i.
    std::vector<std::vector<int>> sdp(n + 2, std::vector<int>(n + 1, 0));
    for (int i = n; i >= 1; --i) {
        for (int j = 0; j <= n; ++j) {
            sdp[i][j] = sdp[i + 1][j];
            // Find rightmost r such that zeros in [i, r] <= j
            // Use binary search on r
            int l = i - 1;
            int r = n;
            while (l != r) {
                int mid = (l + r + 1) / 2;
                int ones = pref[mid] - pref[i - 1];
                int length = mid - i + 1;
                if (length - ones > j) r = mid - 1;
                else l = mid;
            }
            int length = l - i + 1;
            sdp[i][j] = std::max(sdp[i][j], length);
        }
    }

    // For each possible length len of a "separator" segment that we reserve,
    // compute the best achievable base segment length outside it.
    for (int len = 0; len <= n; ++len) {
        long long bestBase = -1;
        if (len == 0) {
            // No separator, just best overall segment
            bestBase = pdp[n][k];
        } else {
            for (int start = 1; start + len - 1 <= n; ++start) {
                int end = start + len - 1;
                int onesInside = pref[end] - pref[start - 1];
                int zerosInside = len - onesInside;
                int remaining = k - zerosInside;
                if (remaining < 0) continue;
                // Best outside left or right
                int outside = 0;
                if (start > 1) outside = std::max(outside, pdp[start - 1][remaining]);
                if (end < n) outside = std::max(outside, sdp[end + 1][remaining]);
                bestBase = std::max(bestBase, static_cast<long long>(outside));
                // Also consider the separator itself can be the best segment
                bestBase = std::max(bestBase, static_cast<long long>(len));
            }
        }
        if (bestBase < 0) continue;
        // Update all ans[i] for i = 1..n
        for (int i = 1; i <= n; ++i) {
            ans[i] = std::max(ans[i], bestBase * i + len);
        }
    }
    // ans is 1-indexed; return a 1-indexed vector but typically the problem expects 1..n.
    ans.erase(ans.begin()); // remove unused index 0, now size n
    return ans;
}

#include <cassert>
#include <vector>
#include <string>
using namespace std;

// Include the solution function here (or link it)
// For brevity, assume the function is declared above.

int main() {
    // Test 1: single character "0", k=0
    auto res1 = maxScores("0", 0);
    // n=1, i=1: only segment "0" len=1, flips=0 => bestLen=0 (can't make it all ones), len=1 => 0*1+1=1
    assert(res1.size() == 1 && res1[0] == 1);

    // Test 2: all ones, k=0
    auto res2 = maxScores("111", 0);
    // For i=1: bestLen=3, len=3 => 3*1+3=6
    // For i=2: 3*2+3=9
    // For i=3: 3*3+3=12
    assert((res2 == vector<long long>{6, 9, 12}));

    // Test 3: "010", k=1
    // Possible segments: length 1: bestLen up to 1, len=1 => for i=1: 1*1+1=2
    // length 2: segment [1,2]="01" has 1 zero, flip it => bestLen=2, len=2 => i=1: 2*1+2=4
    // length 3: "010" has 2 zeros >1, not allowed. But we can pick [1,2] or [2,3]; bestLen=2.
    // For i=1: max(2, 4) =4; i=2: max(1*2+1=3, 2*2+2=6)=6; i=3: max(1*3+1=4, 2*3+2=8)=8
    auto res3 = maxScores("010", 1);
    assert((res3 == vector<long long>{4, 6, 8}));

    // Test 4: "10101", k=2
    // We can make the whole string all ones (5 zeros? actually zeros=2) => bestLen=5, len=5
    // For i=1: 5*1+5=10, i=2: 5*2+5=15, ..., i=5:30
    auto res4 = maxScores("10101", 2);
    assert((res4 == vector<long long>{10, 15, 20, 25, 30}));

    // Test 5: n=2 "00", k=0
    // No flips: bestLen=0 (can't make any ones), but we can take len=1 with bestLen=0 => value=0*i+1=1
    // Or len=0 with bestLen=0 => 0. So ans[1]=1, ans[2]= max(len=1 => 0*2+1=1, len=2 with bestLen=0 => 0*2+2=2) => 2
    auto res5 = maxScores("00", 0);
    assert((res5 == vector<long long>{1, 2}));

    // Test 6: n=1 "1", k=5 (k can exceed n)
    auto res6 = maxScores("1", 5);
    // bestLen=1, len=1 => for i=1: 1*1+1=2
    assert((res6 == vector<long long>{2}));

    // Test 7: "0011", k=1
    // We can flip one zero to make "1011" or "0111", best segment length=3, len=3
    // For i=1: 3*1+3=6; i=2: 3*2+3=9; i=3: 12; i=4:15
    // Also len=4? zeros=2>1 no. len=3 yields these values.
    auto res7 = maxScores("0011", 1);
    assert((res7 == vector<long long>{6, 9, 12, 15}));

    // Test 8: "01010", k=2
    // Whole string zeros=3 >2, not possible. Best segment length=4? e.g., [1,4]="0101" zeros=2, flip => 4, len=4
    // For i=1: 4*1+4=8; i=2: 12; i=3:16; i=4:20; i=5:24
    auto res8 = maxScores("01010", 2);
    assert((res8 == vector<long long>{8, 12, 16, 20, 24}));

    // Test 9: empty? Not allowed by constraints.
    // Test 10: larger n=5 "11111", k=0
    auto res10 = maxScores("11111", 0);
    assert((res10 == vector<long long>{10, 15, 20, 25, 30}));

    return 0;
}
