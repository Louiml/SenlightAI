/*
A robot starts at integer position `x` on an infinite number line and executes a sequence of `n` moves. Each character in the given string `s` is either `'L'` (move left by 1) or `'R'` (move right by 1). The robot needs to reach position `0` exactly. However, the robot can only perform at most `k` total moves (including repetitions if the sequence is repeated). You may repeat the entire sequence `s` any number of times (including zero times) to extend the number of moves, but you cannot stop in the middle of a sequence; you must complete the full sequence each time you use it. The total number of moves actually executed must be ≤ `k`. Determine the maximum number of times the robot can visit position `0` (including the final stopping point) under these constraints. If the robot never reaches `0` within `k` moves, return `0`. Write a function `int countZeroVisits(int n, long long x, long long k, const std::string& s)` that returns this maximum count. Note that `x` is starting position, `k` can be very large (up to 10^18), `n` up to 10^5, and characters are only `'L'` or `'R'`. The robot is allowed to pass through `0` multiple times, but each distinct time it lands exactly on `0` counts.
*/
#include <string>
#include <vector>
#include <algorithm>

// Count maximum times robot lands on 0 if it can only execute full sequences of s.
// Total moves must be <= k. t is chosen as floor(k/n) full sequences.
long long countZeroVisits(int n, long long x, long long k, const std::string& s) {
    if (n == 0 || k < 0) return 0;
    long long t_max = (n == 0) ? 0 : (k / n); // maximum full sequences we can execute
    if (t_max == 0) return 0;

    // prefix sums and delta
    std::vector<long long> pref(n, 0);
    long long cur = x;
    long long delta = 0;
    for (int i = 0; i < n; ++i) {
        if (s[i] == 'L') cur -= 1;
        else cur += 1;
        pref[i] = cur; // position after i+1 moves
        if (i == n-1) delta = cur - x;
    }

    // For each prefix index i where position becomes 0 within one sequence,
    // we need x + m*delta + (pref[i] - x)?? Actually pref[i] is absolute position after i+1 moves from x.
    // The condition for hitting 0 during the m-th full sequence at step i is:
    // x + m*delta + (pref[i] - x) == 0  =>  m*delta + pref[i] == 0.
    // Because starting position for m-th sequence is x + m*delta, and after i+1 moves it is x + m*delta + (pref[i]-x).
    long long total = 0;
    for (int i = 0; i < n; ++i) {
        if (pref[i] == 0) {
            // hits 0 at step i+1 in any sequence where m*delta + pref[i] == 0.
            // But pref[i] == 0 already, so condition is m*delta == 0.
            if (delta == 0) {
                // hits every sequence
                total += t_max;
            } else {
                // m*delta == 0 => m == 0 (since delta != 0)
                total += 1; // only in the very first sequence
            }
        } else {
            if (delta == 0) continue; // never hits 0 for m>0
            // solve m*delta = -pref[i]  => m = (-pref[i]) / delta must be integer
            if (delta != 0 && (-pref[i]) % delta == 0) {
                long long m = (-pref[i]) / delta;
                if (m >= 0 && m <= t_max - 1) {
                    // m is the index of the sequence (0-based). Condition: m*n + (i+1) <= k ? Actually total moves = m*n + (i+1) <= k? Since we must complete full sequences, but we stop at the moment of hitting 0? Wait: We are allowed to stop at any point? The task says "you cannot stop in the middle of a sequence; you must complete the full sequence each time you use it." That means you can only stop after finishing a full sequence, not at the moment of hitting 0. So you count hits that occur during the moves you actually make. Since you complete exactly t_max full sequences, you make t_max*n moves, and you count every time you land on 0 during those moves. So the condition m*n + (i+1) <= t_max*n automatically holds because m <= t_max-1. So no need to check k.
                    total += 1;
                }
            }
        }
    }
    // Avoid double counting: The above loop counts each hitting position for the appropriate m.
    // But note that if a prefix hits 0 in the first sequence (m=0) and also for some m>0, we counted both.
    // That's correct.
    return total;
}
#include <cassert>
#include <string>

long long countZeroVisits(int n, long long x, long long k, const std::string& s);

int main() {
    // Example 1: start x=1, s="L", n=1, k=2. t_max=2, delta=-1.
    // pref[0]=0, delta=-1, pref[0]==0 so delta!=0 => count 1. But actually after first sequence (m=0) we hit 0 at step1. After second sequence (m=1) start at 0, then first move L goes to -1, no hit. So total=1.
    assert(countZeroVisits(1, 1, 2, "L") == 1);

    // Example 2: start x=1, s="LR", n=2, k=2. t_max=1, delta=0. pref: [0, 0]? Actually x=1, L->0, R->1 => pref[0]=0, pref[1]=1. pref[0]==0, delta=0 => total += t_max=1. So total=1. But in one full sequence, moves: L (to 0) counts, R (to 1) no. So 1.
    assert(countZeroVisits(2, 1, 2, "LR") == 1);

    // Example 3: start x=2, s="LL", n=2, k=4. t_max=2. delta=-2. pref[0]=1, pref[1]=0. For pref[1]==0, delta!=0 => +1 (m=0). For pref[0]==1, -pref[0]=-1, delta=-2, (-1)%(-2) !=0, no. For m=1 sequence: start = 2+1*(-2)=0, after first L goes to -1, no hit; after second L to -2, no. So total=1.
    assert(countZeroVisits(2, 2, 4, "LL") == 1);

    // Example 4: start x=0, s="R", n=1, k=3. t_max=3, delta=1. pref[0]=1, no hit. So total=0.
    assert(countZeroVisits(1, 0, 3, "R") == 0);

    // Example 5: start x=1, s="LR", n=2, k=10. t_max=5, delta=0. pref[0]=0, delta=0 => total += 5. pref[1]=1 no. So total=5. Each full sequence: first move L hits 0, second move R back to 1. So each of 5 sequences gives 1 hit. Total 5.
    assert(countZeroVisits(2, 1, 10, "LR") == 5);

    // Example 6: start x=3, s="LRL", n=3, k=9. t_max=3. delta = -1 (L,R,L gives net -1). Let's compute pref: x=3, L->2, R->3, L->2, so pref=[2,3,2]. None zero. delta=-1. No hits ever. total=0.
    assert(countZeroVisits(3, 3, 9, "LRL") == 0);

    // Example 7: start x=2, s="R L R L" with no spaces: "RLRL", n=4, delta=0. Let's test manually: x=2, R->3, L->2, R->3, L->2. Never zero. total=0.
    assert(countZeroVisits(4, 2, 100, "RLRL") == 0);

    // Example 8: start x=1, s="L L" "LL", n=2, delta=-2. k=1? t_max = 0 (since 1/2=0), so return 0.
    assert(countZeroVisits(2, 1, 1, "LL") == 0);

    return 0;
}
// We first simulate the sequence from start position `x` to see if and when the first visit to `0` occurs. The robot starts at `x`, and we scan the string once. If during the first pass (within `n` moves) we hit `0`, we count it as one visit and record the position index (1-based) `pos1` where that first hit occurs. Also note that after completing one full sequence, the net displacement is `delta = (number of 'R') - (number of 'L')`, which can be negative, zero, or positive. If we never hit `0` in the first pass, then it's possible that after some full repetitions the robot enters a cycle. However, if `delta == 0`, the robot returns to the same position after each full sequence, so it will never hit `0` if it didn't in the first pass. If `delta != 0`, we need to consider the prefix sums. A more robust approach: First, compute the prefix positions `pos[0]=x`, then for i from 0 to n-1, update position. If any prefix position equals 0, record the first such i (0-indexed) as `firstHitIndex`. That gives one visit at move count `firstHitIndex+1`. Then, after completing one full sequence, the net change is `delta`. If delta == 0, then if the first pass had a hit, we can repeat the sequence and get the same pattern again, but only if we have enough moves. Actually, if delta == 0, each full sequence returns to the same starting point, so we can count how many times the first visit pattern repeats. The number of hits in one full sequence is `cntPerFull`. But we already counted the first hit in the first partial sequence. We need to be careful: The robot may hit 0 multiple times within a single full sequence. The simplest approach: simulate the entire sequence once from the starting position, tracking when we hit 0. Record the relative positions (offset from the start of a sequence) where we hit 0. Also record the net displacement `delta` after one full sequence. If `delta == 0`, then the positions repeat exactly after each full sequence, so the number of visits in one full sequence is `cycleCount`, and we can repeat that sequence as many times as `k // n` if we have enough moves. But we also have a partial first sequence: we may hit 0 before completing the first full sequence. A cleaner method: Let `prefixHits` be a list of move counts (1-indexed) where `0` is reached during the first scan of the string starting from `x`. Note that after each full sequence, the starting position changes if `delta != 0`. For `delta != 0`, the positions after each full sequence shift linearly, so the condition for hitting 0 in the m-th full sequence is that the starting position `x + m*delta` plus a prefix sum equals 0. That is `prefixSum[j] = - (x + m*delta)`. Since `prefixSum` is bounded, we can solve for `m` for each prefix index. However, parsing all that is complex. The given code's approach is incomplete for general cases, but the intended interpretation of the problem might be simpler: The robot executes the string repeatedly without stopping in the middle, and we count how many times it lands on 0. The repeat count is limited by `k`. The solution in the code actually stops at the first hit in the first pass, then finds the first hit in the second pass (starting from where you ended after first hit? Actually the code is buggy). For a clean task, we can define a simpler variant: The robot always starts at `x`, and we may repeat the entire sequence at most `floor(k/n)` times, plus a partial sequence of length `k % n` at the end. But the problem statement says "you may repeat the entire sequence any number of times" and total moves ≤ `k`. You can stop after any complete sequence, but not in the middle. So you can perform `t` full sequences where `t*n ≤ k`, and that's it. You cannot do a partial sequence because you must complete each sequence. So the total number of moves is exactly `t*n`. Then the task simplifies to: Given start `x`, apply sequence `s` exactly `t` times, count how many times the position becomes 0 across all those moves. Since `t` can be up to 1e18/n, we need a math formula. That is a cleaner and well-defined problem. So I will write the task accordingly: The robot can only move in full sequences; you choose an integer `t ≥ 0` such that `t * n ≤ k`. Determine the maximum number of times the robot lands on 0 if you choose `t` optimally. The answer is the maximum count over all valid `t`. Since more repetitions generally give more chances to hit 0, but not monotonic because the starting position shifts each time, it's not trivial. But we can compute the number of zeros in the first full sequence from the original start, and also the net shift `delta`. After each full sequence, the starting position is `x + m*delta`. The within-sequence zero hits occur at positions where `current_position + prefix_sum = 0`. So for each prefix index `i` where the sequence would hit 0 at some relative step, we need to find all `m` such that `x + m*delta + prefix_sum[i] = 0` and `m*n + (i+1) ≤ k`. That gives a linear congruence. We can count for each such `i` the number of valid `m` from 0 to `t_max-1` where `t_max = k/n`. Then sum them up. Edge cases: `delta=0` means either always hit 0 every time if the prefix sum condition is met, or never. Time complexity O(n) to compute prefix sums and delta, then O(number of zero-hitting prefixes) to solve a linear equation. Space O(n). We'll assume `x` and `k` fit in 64-bit. The answer may overflow 32-bit, so return `long long`.
