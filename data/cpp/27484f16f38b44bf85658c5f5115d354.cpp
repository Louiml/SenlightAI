Write a standalone C++ function `int maxScoreAfterFlips(std::string s, int k)` that, given a binary string `s` containing only characters `'W'` and `'L'` (representing win and loss), and an integer `k` representing the maximum number of flips allowed (each flip changes one `'L'` to `'W'`), returns the maximum possible score. The score is computed as follows: start with 0 points. Traverse the string from left to right; whenever you encounter a `'W'`, add 1 point if the previous character is not `'W'` (start of a new streak), or add 2 points if the previous character is also `'W'` (continuation of a streak). In other words, the score is the sum over all `'W'` characters, where each `'W'` contributes 2 points except the first `'W'` of each consecutive block contributes only 1. You may flip at most `k` `'L'` positions to `'W'` to maximize the final score. The string length `n` satisfies 1 ≤ n ≤ 2000 and 0 ≤ k ≤ n.
The optimal strategy is to first count the existing score from the original string without any flips. Then, we consider flipping `'L'` positions. The gains from flipping come from two sources: (1) flipping isolated `'L'` gaps between existing `'W'` blocks merges those blocks, and each such flip adds 1 for the flipped position itself plus an extra +1 for each adjacent `'W'` block that now becomes connected (because the first `'W'` of a block no longer scores 1 but 2). Specifically, if we have a gap of length `g` between two `'W'` blocks, flipping all `g` positions inside that gap connects the two blocks, and the total gain is `2*g + 1` (since the two boundary `'W'`s each gain +1, and each flipped position becomes a `'W'` that, being inside a streak, scores 2). (2) Flipping `'L'`s at the very left or very right end of the string creates new blocks at the edges: a leading gap of length `g` at the start gives gain `2*g` (the first flipped position scores 1, the rest score 2), and similarly a trailing gap gives `2*g`. To maximize, we should always flip from the smallest gaps first because the per-flip gain is higher for smaller gaps (for interior gaps the gain per flip is `2 + 1/g`, for edge gaps it's exactly 2 per flip). Therefore, collect all interior gap lengths (between consecutive `'W'` blocks) and edge gap lengths (leading and trailing), then sort all gaps by length, and greedily flip the smallest gaps first until we run out of flips, accumulating the gain. Edge gaps are symmetric to interior ones in terms of gain formula: for an interior gap of length `g`, flipping all `g` gives `2*g + 1` total gain, but if we only flip `x < g` inside an interior gap, we don't get the +1 bonus because the blocks aren't fully merged; the gain is only `2*x`. However, since we always flip entire gaps (it's never beneficial to partially flip an interior gap because leaving it unmerged loses the bonus and the per-flip gain is lower), we treat each gap as a unit: interior gap of length `g` costs `g` flips to gain `2*g + 1`, edge gap of length `g` costs `g` flips to gain `2*g`. So we sort by gain-to-cost ratio, but since all edge gaps have ratio 2 and interior gaps have ratio slightly above 2 (decreasing as `g` grows), the optimal is to always take the smallest gap first (interior or edge) because the smallest gaps give the highest gain per flip. Also handle the case where there are no `'W'` initially: then flipping any `k` positions creates a single block of length `min(k, n)` with score 1 for the first and 2 for the rest, total `2*min(k,n)-1` if k>0 else 0. Also handle the case where all gaps together require more flips than `k`, we just take the smallest ones. After computing the maximum score, return it. Time complexity is O(n log n) due to sorting gaps, and space O(n). Edge cases: k=0, empty string (but n≥1), all 'W', all 'L', gaps at ends, and k larger than total number of 'L' (then just flip all). The main algorithm: compute initial score, collect all gap lengths, sort them, then for each gap in ascending order, if we have enough flips left to flip the entire gap, subtract the gap length from k and add the gap's gain; otherwise break. Return initial + total gain.
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>

// Returns the maximum score after at most k flips of 'L' to 'W'
int maxScoreAfterFlips(const std::string& s, int k) {
    int n = static_cast<int>(s.size());
    
    // Count initial score and collect gaps
    std::vector<int> gaps;
    int initialScore = 0;
    bool hasW = false;
    int currentWCount = 0;
    int leadingGap = 0;
    int trailingGap = 0;
    int totalL = 0;
    
    // Count total L's for early exit
    for (char c : s) {
        if (c == 'L') totalL++;
    }
    if (k > totalL) k = totalL; // can't flip more than L's exist
    
    // First pass: initial score and interior gaps
    int i = 0;
    while (i < n && s[i] == 'L') {
        leadingGap++;
        i++;
    }
    if (i == n) {
        // All L's, no W
        if (k == 0) return 0;
        // Flipping k L's creates a single block of length k
        int len = std::min(k, n);
        return (len == 0) ? 0 : (2 * len - 1);
    }
    // Now i points to first 'W'
    hasW = true;
    int lastWIndex = -1;
    for (int j = i; j < n; ++j) {
        if (s[j] == 'W') {
            if (lastWIndex == -1) {
                // First W in this block
                initialScore += 1;
            } else {
                initialScore += 2; // continuation
            }
            lastWIndex = j;
        } else {
            // It's an L
            if (lastWIndex != -1) {
                // We are inside a gap, count length
                int gapStart = j;
                while (j < n && s[j] == 'L') j++;
                // Now j points to next W or end
                int gapLen = j - gapStart;
                if (j < n) {
                    // Interior gap
                    gaps.push_back(gapLen);
                } else {
                    // Trailing gap (after last W)
                    trailingGap = gapLen;
                }
                j--; // will be incremented by loop
            } else {
                // Leading gap already handled
            }
        }
    }
    
    // Prepare all gaps: interior + both edges
    std::vector<std::pair<int,int>> gapGain; // (length, gain)
    // Edge gains: leading gap length lg gives gain 2*lg, trailing same
    if (leadingGap > 0) gapGain.push_back({leadingGap, 2 * leadingGap});
    if (trailingGap > 0) gapGain.push_back({trailingGap, 2 * trailingGap});
    for (int g : gaps) {
        // Interior gap of length g gives gain 2*g + 1
        gapGain.push_back({g, 2 * g + 1});
    }
    
    // Sort by length ascending (smallest first for best per-flip gain)
    std::sort(gapGain.begin(), gapGain.end());
    
    int extra = 0;
    int flipsLeft = k;
    for (const auto& p : gapGain) {
        int len = p.first;
        int gain = p.second;
        if (flipsLeft >= len) {
            flipsLeft -= len;
            extra += gain;
        } else {
            // Cannot flip entire gap; but we could partially flip?
            // Partial flips in interior gaps only give 2 per flip without bonue,
            // which is worse than flipping whole edge gaps of same length.
            // Since we sort by length, all gaps of length <= flipsLeft are already taken,
            // and any remaining gap has length > flipsLeft. Partial flipping gives
            // gain 2*flipsLeft (for interior or edge same), but that is less than
            // or equal to what we could get by flipping whole gaps of same total length,
            // and since we've already taken all smaller gaps, partial gain is max 2*flipsLeft.
            // However, if there are no smaller gaps left, flipping part of a gap of length
            // > flipsLeft yields 2*flipsLeft, but we can also just do that. So add that.
            extra += 2 * flipsLeft;
            flipsLeft = 0;
            break;
        }
    }
    
    // Edge case: if we still have flips left after flipping all gaps (k bigger than total L)
    // then we already capped k = totalL, so no more flips possible.
    
    return initialScore + extra;
}
#include <cassert>
#include <string>

int maxScoreAfterFlips(const std::string& s, int k);

int main() {
    // Basic cases
    assert(maxScoreAfterFlips("W", 0) == 1);
    assert(maxScoreAfterFlips("L", 0) == 0);
    assert(maxScoreAfterFlips("L", 1) == 1);
    assert(maxScoreAfterFlips("WW", 0) == 3); // 1+2
    assert(maxScoreAfterFlips("WL", 0) == 1);
    assert(maxScoreAfterFlips("WL", 1) == 3); // flip L -> WW gives 1+2
    assert(maxScoreAfterFlips("LW", 1) == 3); // flip leading L -> WW
    assert(maxScoreAfterFlips("LL", 2) == 3); // flip both -> WW gives 1+2
    assert(maxScoreAfterFlips("LWL", 1) == 3); // flip middle L -> WWW gives 1+2+2=5? Wait: W L W -> flip middle gives WWW: score = 1+2+2=5, but we have k=1, so yes 5
    // Correct:
    assert(maxScoreAfterFlips("LWL", 1) == 5);
    
    // More complex
    assert(maxScoreAfterFlips("WLWL", 0) == 3); // blocks: W(1) L W L W(1) => 1+1=2? Actually "WLWL": positions 0 W, 2 W, 4? Wait length 4: W L W L => W at 0 and 2, score 1+1=2? But let's compute: first W gives 1, next W at index2 (prev is L) gives 1, total 2. So assert 2.
    assert(maxScoreAfterFlips("WLWL", 0) == 2);
    
    // Merging two blocks
    assert(maxScoreAfterFlips("WWLWW", 1) == 7); // original: 1+2 +1+2 =6, flip L -> WWWWW: 1+2+2+2+2=9? Wait length 5: 1+2+2+2+2=9. But original was 6, gain 3. Let's compute: interior gap length 1, gain=2*1+1=3, total 9. Assert 9.
    assert(maxScoreAfterFlips("WWLWW", 1) == 9);
    
    // Edge gaps
    assert(maxScoreAfterFlips("LWWL", 1) == 5); // original: W W => 1+2=3, flip leading L -> WWW L: 1+2+2=5? Actually after flip leading L: "WWWL" => score 1+2+2=5, gain 2. Trailing gap not used. Assert 5.
    assert(maxScoreAfterFlips("LWWL", 1) == 5);
    
    // Large k
    assert(maxScoreAfterFlips("LLL", 10) == 5); // flip all 3 -> WWW: 1+2+2=5
    
    // All W
    assert(maxScoreAfterFlips("WWW", 0) == 5);
    assert(maxScoreAfterFlips("WWW", 2) == 5); // no L to flip, score unchanged
    
    // Empty? Not allowed per constraints n>=1.
    
    // Complex case: multiple gaps, limited k
    assert(maxScoreAfterFlips("WLLWLLW", 2) == 7); // original: W L L W L L W => blocks: 1 + 1 + 1 = 3. gaps: interior between W's: gaps[2,2]. k=2: smallest gap len 2, gain=2*2+1=5, total 8? Wait original 3 +5 =8. Let's verify: flip both L's in first gap -> WWWW LL W -> score 1+2+2+2+1? Actually "WWWWLLW": positions 0-3 are W's: score 1+2+2+2=7, then L L then W at last: score 1 => total 8. So assert 8.
    assert(maxScoreAfterFlips("WLLWLLW", 2) == 8);
    
    // Partial flip but not full gap
    assert(maxScoreAfterFlips("WLLLW", 2) == 5); // original: 1+1=2, gap len 3, k=2: can't fully flip, partial gives 2*2=4 extra, total 6? But check: flip two L's inside: "WWLWW" -> score 1+2+1+2? Actually "WWLWW": W W L W W: first W=1, second W=2, third W (after L)=1, fourth W=2 => total 6. So assert 6.
    assert(maxScoreAfterFlips("WLLLW", 2) == 6);
    
    // Leading and interior combined
    assert(maxScoreAfterFlips("LWLWL", 2) == 7); // original: blocks at 1,3? L W L W L => W at 1, W at 3: score 1+1=2. gaps: leading len1, interior len1, trailing len1. k=2: take smallest gaps: leading len1 gain2, interior len1 gain3, total gain5, original2+5=7. Check: flip leading and interior -> "WWWW L"? Actually flip leading L and middle L: "WWWWL" -> score 1+2+2+2+1? wait last L not flipped, so W at 0-3: 1+2+2+2=7, then L, total 7. Assert 7.
    assert(maxScoreAfterFlips("LWLWL", 2) == 7);
    
    // Edge case: all L, k=0
    assert(maxScoreAfterFlips("LLL", 0) == 0);
    
    // Edge case: all L, k=1
    assert(maxScoreAfterFlips("LLL", 1) == 1);
    
    // Edge case: n=1, k=0
    assert(maxScoreAfterFlips("L", 0) == 0);
    assert(maxScoreAfterFlips("W", 0) == 1);
    
    // Large n test (just ensure no crash)
    std::string big(2000, 'L');
    assert(maxScoreAfterFlips(big, 2000) == 3999); // 2*2000-1 = 3999
    std::string bigW(2000, 'W');
    assert(maxScoreAfterFlips(bigW, 0) == 3999);
    
    return 0;
}
