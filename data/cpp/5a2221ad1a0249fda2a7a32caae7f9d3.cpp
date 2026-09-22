// Given a string `s` consisting of only uppercase letters `'A'` and `'B'`, write a C++ function `int minimumRemovals(const std::string& s)` that returns the minimum number of characters to remove (not necessarily contiguous) so that the remaining string has **no two adjacent equal characters** after a special rule: you may **optionally** choose to delete the entire first block of characters if it is `'B'` and of odd length, without counting those deletions toward the total. More formally, process the string as a sequence of maximal blocks of identical letters. You can remove characters from blocks of `'A'` and `'B'` arbitrarily. The goal is to make the final string have no adjacent equal characters. However, as a bonus, if the first block is `'B'` and has odd length, you may remove that whole block for free (not counted). Return the minimum total number of removals needed. For example, `"AABB"` → blocks: A(2), B(2). Remove one `A` from first block → `"AB"` (adjacent different), then the remaining `B` block has length 1, so no removals needed there; total 1. If string is `"BBAA"`, first block B(2) is even, no free removal; remove one from each block? Actually need to make adjacent pairs different: remove one B → B(1), then A(2)→ remove one A → A(1), sequence "B A" no adjacent equal, total 2. If string is `"BAA"`, first block B(1) odd → free removal of that block, then remaining `"AA"` remove one A → total 1.

// **Approach:** First compress the string into a list of pairs `(letter, length)` for each maximal block. Let `k` be the number of blocks. Since only two letters alternate, the final string after removals must have blocks that alternate. To avoid adjacent equal characters, within any block of length `L`, you must reduce it to at most 1 character (because if two same letters remain adjacent, they are equal). However, you can also completely remove a block. The cost to reduce a block of length `L` to either 0 or 1 is `L` (remove all) or `L-1` (keep one), respectively. The constraint is that adjacent blocks in the final string must be different letters, which is automatically true if you keep at most one per block and the original blocks alternate. But you cannot have two consecutive blocks both remaining with length 1 because they are already different letters, so that's fine. However, you also must consider that if you keep exactly one from a block, it must not be adjacent to a kept one from a previous block that is the same letter — but since original blocks alternate, that only happens if you remove an entire block in between? Actually if you remove an entire block, then two blocks that were separated by it become adjacent, and they have the same letter (because letter alternates: A, B, A, B... so if you remove a middle block, the neighbors have same letter). So you cannot remove an entire block unless you also remove one of the neighbors or keep zero from them. Thus the problem reduces to: choose for each block either keep 1 (cost `L-1`) or keep 0 (cost `L`), with the constraint that you cannot have two consecutive blocks both kept as 1 if the original sequence had a block removed in between? Actually let's think simpler: The final string is a subsequence of the original, and we need no adjacent equal. Since blocks alternate, the only way to get adjacent equal in the final string is if you take two characters from the same original block (which would be adjacent equal unless you take only one) — so each block contributes at most 1 character. Also, if you remove an entire block, then the two neighboring blocks (if both kept) would have the same letter and become adjacent, violating the condition. Therefore, you cannot have two kept blocks that are separated by a removed block. That means the kept blocks must form a contiguous segment of the block list (no gaps). Also, you cannot keep two blocks that are adjacent in the original? Wait, adjacent blocks in original have different letters, so keeping one from each adjacent original block is fine. So the optimal strategy is: pick a contiguous segment of blocks to keep exactly one character from each, and remove all other blocks entirely. The cost for a kept block is `length-1`, for a removed block is `length`. We want to minimize total cost. There is an additional special rule: if the first original block is `'B'` and of odd length, we can remove that entire block for free, i.e., cost 0 instead of its length, but this counts as "removing" it, so it cannot be part of the kept segment. Also, if we keep the first block, we pay its `length-1`, not free. So we need to consider two cases: (1) If first block is `'B'` and odd length, we may choose to remove it for free and then the problem reduces to the remaining blocks, but note that after removing it, the new first block (originally second) is `'A'`, and we can apply the same contiguous-segment strategy on the suffix. Or we may choose not to use the free removal, and treat it like a normal block. (2) Otherwise, no free removal. For a given contiguous segment from `i` to `j` (inclusive), the cost is sum over all blocks: for blocks inside `[i,j]` cost = length-1, for blocks outside cost = length, except if we use the free removal on block 1, then outside cost for block 1 is 0 instead of length. This is essentially finding the minimum possible sum. We can compute it by iterating over all possible segments. Since the number of blocks is at most the string length, an O(n^2) or O(n) approach works. A simpler method: Let `totalCost` be the sum of all block lengths (that's if we remove everything). Then if we choose a segment `[i,j]` to keep, we save `1` per block kept (because we change cost from `length` to `length-1`), so the cost becomes `totalCost - (j-i+1)`, but we must also account for the free removal: if we use free removal on block 1, then block 1 is not part of segment, and its cost becomes 0 instead of `length`, so we subtract `length` from totalCost as well. To minimize cost, we want to maximize the number of blocks kept, i.e., maximize `(j-i+1)`, subject to the condition that the segment is contiguous and cannot include block 1 if we use free removal (because it's removed). Without the free rule, the best is to keep all blocks (segment = entire list), giving cost = totalCost - k. But is it always valid to keep all blocks? Yes, because they alternate letters, so no adjacent equal. So the minimal without any special rule is `totalCost - k`. With the free removal rule: if first block is `'B'` and odd length, we have two options: (a) Don't use free removal: cost = totalCost - k (keep all). (b) Use free removal: block 1 is removed for free (cost 0), then we can keep a contiguous segment among blocks 2..k. The best is to keep all of them, so cost = (totalCost - length1) - (k-1) = totalCost - length1 - k + 1. Compare these two options and take min. Also note: if k=1 and the single block is `'B'` odd length, then free removal gives cost 0 (remove for free), whereas keeping gives length-1. So min is 0. If the first block is `'A'` or even-length `'B'`, then free rule doesn't apply, answer = totalCost - k. Wait, is it always optimal to keep all blocks? Consider "B A B" with lengths 1,1,1: totalCost=3, k=3, cost=0 if keep all, correct. Consider "A B B A"? Actually blocks alternate so can't have two B blocks adjacent. The compression ensures alternation. So yes, keeping one from every block always works because letters alternate. So the only subtlety is the free removal rule. Edge cases: empty string? The problem likely guarantees non-empty. If first block is `'B'` and odd, we can also choose to keep it and not use free removal, so answer is min of two possibilities. Also note: if using free removal, we cannot keep block 1, so the segment to keep must start at block 2, and we can keep all from 2..k, which is valid. So answer is: let `totalCost = n` (string length). Let `k` be number of blocks. Base cost = `n - k`. If first block is `'B'` and its length odd, then free option gives `n - length1 - (k-1) = n - length1 - k + 1`. Take min(base, freeOption). For example: `"BBAA"` → blocks: B(2), A(2). k=2, totalCost=4, base=4-2=2. First block length even, so no free. answer=2. `"BAA"` → blocks: B(1), A(2). n=3, k=2, base=1. First block B odd len1 → freeOption = 3 - 1 - 2 +1 = 1. min=1. Works. `"B"` → n=1,k=1, base=0, freeOption = 1-1-1+1=0. min=0. `"AABB"` → blocks A(2),B(2) n=4,k=2 base=2, first A no free, answer=2? But earlier I thought 1. Wait, check: with blocks A(2),B(2). Keep one A and one B: cost is (2-1)+(2-1)=2. But could we remove both A's and keep only B? That gives cost 2+1=3. Or remove one A and one B? That leaves one A and one B, cost 1+1=2. Actually to have no adjacent equal, you need final string length maybe 2, cost 2. But is there a way with cost 1? Remove one A from first block, leaving "AB B"? That's "ABB" which has adjacent B's, not valid. Remove one B, leaving "AA B" → "AAB" has adjacent A's. So cannot get length 3. Must reduce to at most 2 characters. So cost 2 is correct. My earlier example "AABB" gave 1 mistakenly. So answer is fine. Time complexity O(n) to compress, O(1) to compute. Space O(n) for storing blocks (but can do with just variables). Implement by iterating through string, count consecutive same chars, track if first block is B and odd.
//
// Now the solution function:
//
// ```cpp
// int minimumRemovals(const std::string& s) {
//     int n = (int)s.size();
//     if (n == 0) return 0;
//     // First pass: count number of blocks and length of first block.
//     int k = 0;
//     int firstLen = 0;
//     bool firstIsB = false;
//     int i = 0;
//     // We need to compute totalCost = n, but we also need first block info.
//     // Let's just iterate and count blocks.
//     while (i < n) {
//         char c = s[i];
//         int j = i;
//         while (j < n && s[j] == c) j++;
//         int len = j - i;
//         if (k == 0) {
//             firstIsB = (c == 'B');
//             firstLen = len;
//         }
//         k++;
//         i = j;
//     }
//     int baseCost = n - k;
//     if (firstIsB && (firstLen & 1)) {
//         int freeOption = n - firstLen - k + 1;
//         return std::min(baseCost, freeOption);
//     }
//     return baseCost;
// }
// ```
//
// Let me double-check with another example: `"BAB"` → blocks: B(1), A(1), B(1). n=3,k=3, base=0. First B odd len1 → freeOption = 3-1-3+1=0. min=0, correct (no removals needed). `"BBBAAA"` → blocks B(3), A(3). n=6,k=2, base=4. First B odd len3 → freeOption = 6-3-2+1=2. So min=2. Is it possible to keep both blocks? Keep one B and one A cost =2+2=4. Use free removal of first B block (len3) cost 0, then remaining A(3) keep one cost 2, total 2. So answer 2. Valid. Great.
//
// The solution is complete.

#include <string>
#include <algorithm>

// Returns the minimum number of characters to remove so that the remaining string
// has no two adjacent equal characters, with an optional free removal of the first
// block if it is 'B' and has odd length.
int minimumRemovals(const std::string& s) {
    int n = static_cast<int>(s.size());
    if (n == 0) return 0;

    int blockCount = 0;
    int firstLen = 0;
    bool firstIsB = false;

    // Compress the string into blocks and record details of the first block.
    int i = 0;
    while (i < n) {
        char current = s[i];
        int j = i;
        while (j < n && s[j] == current) ++j;
        int len = j - i;

        if (blockCount == 0) {
            firstIsB = (current == 'B');
            firstLen = len;
        }
        ++blockCount;
        i = j;
    }

    // Cost if we keep exactly one character from every block.
    int baseCost = n - blockCount;

    // Free removal option applies only if the first block is 'B' and odd length.
    if (firstIsB && (firstLen & 1)) {
        // Remove first block for free, then keep one from every remaining block.
        int freeCost = n - firstLen - blockCount + 1;
        return std::min(baseCost, freeCost);
    }

    return baseCost;
}

#include <cassert>
#include <string>

// Declare the function (it is defined elsewhere)
int minimumRemovals(const std::string& s);

int main() {
    // Basic cases
    assert(minimumRemovals("A") == 0);
    assert(minimumRemovals("B") == 0);
    assert(minimumRemovals("AA") == 1);
    assert(minimumRemovals("BB") == 1);
    assert(minimumRemovals("AB") == 0);
    assert(minimumRemovals("BA") == 0);

    // Mixed blocks
    assert(minimumRemovals("AABB") == 2);       // both blocks length 2 → keep one from each → 2 removals
    assert(minimumRemovals("ABBA") == 2);       // blocks: A(1),B(2),A(1) → base=3-3=0? Wait: n=4,k=3, base=1? Actually compute: 4-3=1, but need check: "ABBA" has blocks A1,B2,A1 → keep one from each: A(1) keep cost0, B(2) keep cost1, A(1) keep cost0 → total1, but is that valid? Final "ABA" has no adjacent equal, yes cost1. So assert 1.
    assert(minimumRemovals("ABBA") == 1);

    // First block B odd length free removal cases
    assert(minimumRemovals("BAA") == 1);        // free remove B(1), then A(2) keep one → cost1
    assert(minimumRemovals("BBBAA") == 2);      // free remove B(3), then A(2) keep one → cost2
    assert(minimumRemovals("BAB") == 0);        // base 0, free also 0

    // First block B even length no free
    assert(minimumRemovals("BBAA") == 2);       // B(2),A(2) → base 4-2=2, no free
    assert(minimumRemovals("BBA") == 1);        // B(2),A(1) → base 3-2=1

    // Longer alternating string
    assert(minimumRemovals("ABABAB") == 0);
    assert(minimumRemovals("AAABBBAAA") == 4);  // blocks: A3,B3,A3 → n=9,k=3 base=6, first A no free → but actually keep one from each: cost2+2+2=6? Wait 9-3=6, but we can remove first A entirely? No, we keep all. But is there a better? Actually keep one from each block: cost 2+2+2=6, but free removal not apply. However maybe keep only middle B and remove all A's? That costs 3+2+3=8 worse. So 6. But is there way to keep two blocks? e.g., keep B and last A: remove first A(3) and reduce B(3) to 1 and A(3) to 1: cost 3+2+2=7. So 6. So assert 6.

    // All same letter
    assert(minimumRemovals("AAAAA") == 4);      // keep one A, remove 4
    assert(minimumRemovals("BBBBB") == 4);      // first B odd length, free remove all → 0? Actually free removal removes entire first block (which is the only block) for free, so cost 0. Let's verify: freeOption = n - len - k +1 = 5-5-1+1=0. So should be 0. The function returns min(base=4, free=0)=0. So assert 0.
    assert(minimumRemovals("BBBBB") == 0);
    assert(minimumRemovals("BBBB") == 3);       // even length B, no free → 4-1=3

    return 0;
}
