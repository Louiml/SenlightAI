// Write a C++ function `countValidSequences(int L, int K)` that returns the number of binary sequences of length `L` (where each position is either 0 or 1) such that every maximal contiguous block of consecutive 1s has length exactly `K` or `0`, i.e., no block of 1s can have length other than `K` (so blocks of 1s of length 1..K-1 and >K are forbidden). Also, the sequence may end with any character. The function should return the count as an `unsigned long long`. Handle the edge case `K > L` correctly (still allow sequences with no 1s at all). For example, for `L=3, K=2`, valid sequences are: `000`, `001`, `010`, `100`, `011`, `110` (blocks of 1s are either length 0 or exactly 2). `101` has blocks of length 1 (invalid), `111` has block length 3 (invalid). So count = 6. Assume `L >= 0` and `K >= 1`.
// The problem is a combinatorial counting problem. We can solve it with dynamic programming. Let `dp[l][0]` = number of valid sequences of length `l` that end with a 0, and `dp[l][1]` = number of valid sequences of length `l` that end with a 1. Base case: an empty sequence of length 0 can be considered as ending with a 1? In the given code, they set `dp[0][1]=1` and `dp[0][0]=0` as a trick, but we can also set both to 1 for empty? Let’s derive properly.
//
// We want to count sequences where every run of consecutive 1s has length exactly `K` (or zero). A standard approach: treat a valid sequence as a concatenation of blocks of the form `0` or `1^K` (i.e., K ones) but with the caveat that the sequence may start or end with `1^K` or `0`. Actually, any valid sequence can be described as a sequence of "chunks": either a single `0`, or exactly `K` ones. However, you cannot have two chunks of `1^K` adjacent without a separator, because that would create a block of length `2K` which is invalid (unless `K=0` but not). So a valid sequence is a sequence of chunks from {`0`, `1^K`} such that no two `1^K` chunks are adjacent (because they'd merge). But we can also think of it as: the sequence is composed of blocks of 0s (each of any length) separated by exactly-K runs of 1s. So the structure is: `0^a1 (1^K) 0^b (1^K) 0^c ...` with the constraint that between two `1^K` runs there is at least one `0`. Also, the sequence may start or end with `0^a` or `1^K`.
//
// The DP given in the snippet is a clever way: `dp[l][0]` = number of valid sequences of length `l` ending with 0, and `dp[l][1]` = number ending with 1. The recurrence:
// - For ending with 0: you can put a 0 after any valid sequence of length `l-1` (ending with either 0 or 1), so `dp[l][0] = dp[l-1][1] + (l >= K ? dp[l-K][1] : 0)`? Wait, that recurrence is odd. The given snippet uses:
// ```
// dp[l][0] = dp[l - 1][1] + (l >= K ? dp[l - K][1] : 0);
// dp[l][1] = dp[l - 1][0];
// ```
// But that seems to be a different problem? Let's test: For `L=3, K=2`, let's compute with that recurrence. Initialize `dp[0][1]=1, dp[0][0]=0`. For l=1: `dp[1][0] = dp[0][1] + (1>=2?0) = 1`, `dp[1][1] = dp[0][0] = 0`. For l=2: `dp[2][0] = dp[1][1] + (2>=2?dp[0][1]) = 0 + 1 = 1`, `dp[2][1] = dp[1][0] = 1`. For l=3: `dp[3][0] = dp[2][1] + (3>=2?dp[1][1]) = 1 + 0 = 1`, `dp[3][1] = dp[2][0] = 1`. Sum of `dp[l][0]` for l=0..3 = dp[0][0]=0 + dp[1][0]=1 + dp[2][0]=1 + dp[3][0]=1 = 3, but expected 6. So that snippet is actually counting something else? Let's re-read snippet: It uses `std::accumulate(dp.begin(), dp.end(), 0ULL, [](acc, a){ return acc + a[0];})` which sums all `dp[l][0]` over l=0..L. That gives 3 for L=3,K=2, not 6. So the original snippet is likely from a different problem (maybe counting sequences where runs of 1s are exactly of length K or 0 but with some other constraint? Actually maybe it counts sequences that end with 0? Or maybe it's counting something like “number of ways to place tiles of size 1x1 (0) and 1xK (1s) on a board of length L with no overlap” but that would be a different count.
//
// Let’s derive the correct DP for our task. Let `dp[l][0]` = number of valid sequences of length `l` that end with 0. Let `dp[l][1]` = number of valid sequences of length `l` that end with exactly one 1 (i.e., the last character is 1, but the run of 1s at the end may not be complete yet). Actually we need to track the length of the trailing run of 1s. A simpler way: Use a state `dp[l][0]` for ending with 0, and `dp[l][1]` for ending with a run of exactly `K` ones? But that doesn't work because you could have a run of length less than K. Better: Use two states: `end0[l]` = number of valid sequences of length `l` that end with 0 (and thus the last run of 1s, if any, is already complete). `end1[l]` = number of valid sequences of length `l` that end with a run of 1s of length exactly `1..K-1`? Actually we need to know if the last run is incomplete. A common approach: Let `dp[l]` be total number of valid sequences of length `l`. Then we can form a recurrence by considering the last block: either the sequence ends with a 0, or it ends with a block of exactly K ones. But to avoid double counting, we can think of generating sequences by appending either a single 0 or a block of K ones, but ensure we don't append a block of K ones immediately after another block of K ones without a 0 in between. So define `a[l]` = number of valid sequences of length `l` that end with 0 (or empty? we can treat empty as ending with 0). `b[l]` = number of valid sequences of length `l` that end with a block of exactly K ones (and the preceding character, if any, is a 0 or the sequence is just the block). Then:
// - To get a sequence ending with 0: you can append a 0 to any valid sequence of length `l-1` (either ending with 0 or with a block of ones). So `a[l] = a[l-1] + b[l-1]`? But careful: if the previous sequence ended with a block of K ones, appending a 0 is fine. If it ended with 0, appending 0 is fine. So `a[l] = a[l-1] + b[l-1]` (total valid sequences of length l-1). Actually total valid sequences of length l-1 is `a[l-1] + b[l-1]`. So `a[l] = total[l-1]`.
// - To get a sequence ending with a block of exactly K ones: you must take a valid sequence of length `l-K` that ends with 0 (so that the block of ones is preceded by a 0 or the sequence starts), and append K ones. So `b[l] = a[l-K]` (with `a[0] = 1`? For l=K, you can have a sequence of just K ones, which corresponds to `a[0]`? If we define empty sequence as ending with 0, then `a[0]=1`). Also for l<K, `b[l]=0`.
// Then total valid sequences of length `L` is `a[L] + b[L]`. But the problem asks to sum over all sequences of length `L`? The task says "countValidSequences(int L, int K)" returns number of binary sequences of length `L` satisfying the condition. So we just need `total[L] = a[L] + b[L]`. Let's test with L=3,K=2: We have `a[0]=1`, `b[0]=0`. For l=1: `a[1]=total[0]=1`, `b[1]=a[-1]?` no, l<K so 0. total[1]=1 (sequence "0"). For l=2: `a[2]=total[1]=1` (sequence "00"? wait total[1]=1, so a[2]=1, meaning ending with 0: sequence "00" only, but also "10" would end with 0? Actually total[1] includes only "0", so appending 0 gives "00". But "10" is not valid because "1" alone is invalid. Correct. `b[2]=a[0]=1` (sequence "11"). total[2]=2. For l=3: `a[3]=total[2]=2` (appending 0 to "00" and "11" gives "000" and "110"). `b[3]=a[1]=1` (appending "11" to "0" gives "011"). total[3]=3. But expected 6 from earlier? Wait, we listed 6 sequences: 000,001,010,100,011,110. But "001" and "010" and "100" are not counted. Let's see: Our recurrence only allows sequences that either end with 0 or end with exactly K ones, but it forces that a block of ones must be exactly K and must be preceded by a 0 or start. However, "001" ends with a single 1, which is a run of length 1 (invalid). So "001" is not valid. So the expected 6 I gave earlier was wrong. Let's re-evaluate: For L=3,K=2, the valid sequences: Blocks of 1s must have length exactly 2. So possible patterns: 
// - no 1s: 000
// - one block of length 2: positions (1,2) -> 110, (2,3) -> 011, also can have leading or trailing zeros: 001? that has a single 1, invalid. 100? has single 1, invalid. 010? single 1, invalid. Also 101? two blocks of length 1 each, invalid. So valid ones are: 000, 110, 011. That's 3. Also what about 1100? That's length 4, but for length 3 only. So the correct count for L=3,K=2 is 3. Good, my recurrence gives 3. So the original snippet's recurrence? It gave 3 as well (sum of dp[l][0] for l=0..3 = 3). But that sum is not total[L] but sum over all lengths? Actually the snippet returns sum over all l of dp[l][0]. That sum equals 3 for L=3,K=2, which matches our total[3]=3. Interesting coincidence? Let's test L=4,K=2 with our recurrence: a[0]=1,b[0]=0; a[1]=1,total[1]=1; a[2]=1,b[2]=1,total[2]=2; a[3]=2,b[3]=a[1]=1,total[3]=3; a[4]=total[3]=3,b[4]=a[2]=1,total[4]=4. Valid sequences length 4: Let's enumerate: 0000, 1100 (block at 1-2), 0110 (block at 2-3), 0011 (block at 3-4), also 1111? invalid (block length 4). Also 1010? invalid. So 4. Good. Now compute the snippet's sum for L=4,K=2: Using their recurrence:
// dp[0][1]=1,dp[0][0]=0.
// l=1: dp[1][0]=dp[0][1]+0=1, dp[1][1]=dp[0][0]=0.
// l=2: dp[2][0]=dp[1][1]+dp[0][1]=0+1=1, dp[2][1]=dp[1][0]=1.
// l=3: dp[3][0]=dp[2][1]+dp[1][1]=1+0=1, dp[3][1]=dp[2][0]=1.
// l=4: dp[4][0]=dp[3][1]+dp[2][1]=1+1=2, dp[4][1]=dp[3][0]=1.
// Sum of dp[l][0] for l=0..4 = 0+1+1+1+2=5. That's not 4. So the snippet is not counting our problem. The snippet is maybe counting something else like number of sequences where every run of 1s has length at most 1? No. So we need to write our own solution.
//
// Thus, the correct DP as derived: Let `end0[l]` = number of valid sequences of length `l` that end with 0 (or empty). Let `endK[l]` = number of valid sequences of length `l` that end with a block of exactly K ones (i.e., the last K characters are 1, and either l==K or the character before the block is 0). Then:
// - Base: `end0[0] = 1` (empty sequence treated as ending with 0), `endK[0] = 0`.
// - For l from 1 to L:
//   - `end0[l] = end0[l-1] + endK[l-1]` (append a 0 to any valid sequence of length l-1).
//   - `endK[l] = (l >= K ? end0[l-K] : 0)` (append a block of K ones after a sequence ending with 0, or the empty sequence for l==K).
// - The answer is `end0[L] + endK[L]`.
//
// Edge cases: If K > L, then `endK[l]` is always 0 for l<=L, so only sequences with all zeros are valid? Actually if K>L, you cannot have any block of ones because any run would be length <K, so only all-zero sequence is valid. So answer = 1. Our DP gives: end0[0]=1, end0[1]=1, ..., end0[L]=1, endK[L]=0, total=1. Good.
//
// For L=0: The empty sequence has no ones, so it's valid (all zeros). Answer = 1. Our DP: end0[0]=1, endK[0]=0, total=1.
//
// Time complexity: O(L) time, O(1) auxiliary space if we only keep previous two values, or O(L) with arrays. Space O(1) is fine. We'll implement an O(1) space version.
//
// Now we need to produce the required sections.
#include <cstddef>

// Counts binary sequences of length L where every maximal run of 1s has length exactly K (or 0).
unsigned long long countValidSequences(int L, int K) {
    if (L < 0 || K <= 0) return 0; // invalid input, but task assumes valid
    // dp0: number of valid sequences ending with 0 (or empty for length 0)
    // dpK: number of valid sequences ending with a block of exactly K ones
    unsigned long long end0 = 1ULL; // for length 0, empty sequence treated as ending with 0
    unsigned long long endK = 0ULL; // no block of ones for length 0

    // We will compute iteratively for lengths 1..L
    // We need to keep a history of end0 for lengths i-K to handle the block append.
    // Since K can be large, we use a small array (or deque) of last K+1 end0 values.
    // But to keep O(L) time and O(K) space? Actually we can do O(L) time and O(K) space,
    // but simpler is to store all end0 in a vector of size L+1. However, we can also do O(1) space
    // by using a circular buffer of size K+1. Let's do a vector for clarity.
    // Since L can be up to maybe 10^5, O(L) space is fine.
    // But the task says "self-contained" and no main, so we can use vector. Let's use O(L) space for simplicity.
    // However, to demonstrate good practice, we'll use O(L) space.
    std::vector<unsigned long long> end0_arr(L + 1, 0);
    end0_arr[0] = 1; // empty sequence
    unsigned long long endK_prev = 0;

    for (int l = 1; l <= L; ++l) {
        // end0[l] = end0[l-1] + endK[l-1]
        unsigned long long end0_l = end0_arr[l - 1] + endK_prev;
        end0_arr[l] = end0_l;

        // endK[l] = (l >= K ? end0_arr[l-K] : 0)
        unsigned long long endK_l = (l >= K) ? end0_arr[l - K] : 0ULL;
        endK_prev = endK_l;
    }

    return end0_arr[L] + endK_prev;
}
Wait, the above uses `endK_prev` to represent `endK[l-1]` when computing `end0[l]`. That works because we update it at the end of the loop. But careful: At iteration `l`, `endK_prev` currently holds `endK[l-1]` (from previous iteration). So `end0_arr[l] = end0_arr[l-1] + endK_prev` is correct. Then we compute `endK_l` for current `l` and set `endK_prev = endK_l` for next iteration. Good. Also note that `end0_arr[l-K]` for l>=K is already computed because l-K < l. So it's fine.

However, we need to include `<vector>` header. Also for `const` correctness, we can make parameters `const int` but not necessary. We'll use `int` but mark as `const int` maybe. The problem says "apply appropriate const correctness" – we can use `const int L, const int K` but they are passed by value, so const is fine.

But there is a subtle bug: In the loop, when we compute `endK_l` using `end0_arr[l-K]`, that array holds the correct value. But we also need to handle the case where `l == K` and `L` might be 0? Works.

Let's test with L=3,K=2 manually:
end0_arr[0]=1, endK_prev=0.
l=1: end0_l = 1+0=1, end0_arr[1]=1; endK_l = 0; endK_prev=0.
l=2: end0_l = 1+0=1, end0_arr[2]=1; endK_l = (2>=2? end0_arr[0]=1) => 1; endK_prev=1.
l=3: end0_l = end0_arr[2]+endK_prev = 1+1=2, end0_arr[3]=2; endK_l = (3>=2? end0_arr[1]=1) =>1; endK_prev=1.
Final: end0_arr[3]=2 + endK_prev=1 => 3. Correct.

For L=4,K=2:
l=1: end0_arr[1]=1, endK_prev=0
l=2: end0_arr[2]=1+0=1, endK_prev=end0_arr[0]=1
l=3: end0_arr[3]=1+1=2, endK_prev=end0_arr[1]=1
l=4: end0_arr[4]=2+1=3, endK_prev=end0_arr[2]=1
Total = 3+1=4. Correct.

So the solution is fine.

We need to output only code in section, no extra explanation. We'll include `<vector>`.
#include <cassert>

int main() {
    // L=0 => empty sequence only
    assert(countValidSequences(0, 1) == 1);
    assert(countValidSequences(0, 5) == 1);

    // L=1
    assert(countValidSequences(1, 1) == 2); // "0", "1"
    assert(countValidSequences(1, 2) == 1); // only "0"

    // L=2
    assert(countValidSequences(2, 1) == 3); // "00","01","10"
    assert(countValidSequences(2, 2) == 2); // "00","11"
    assert(countValidSequences(2, 3) == 1); // only "00"

    // L=3
    assert(countValidSequences(3, 1) == 5); // no adjacent ones
    assert(countValidSequences(3, 2) == 3); // "000","011","110"
    assert(countValidSequences(3, 3) == 2); // "000","111"
    assert(countValidSequences(3, 4) == 1); // only "000"

    // K > L
    assert(countValidSequences(5, 7) == 1);
    assert(countValidSequences(10, 100) == 1);

    // Larger sanity: L=4,K=2 => 4
    assert(countValidSequences(4, 2) == 4); // "0000","1100","0110","0011"
}
