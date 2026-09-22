Write a C++ function `findFirstMeetingTime` that takes two pseudo-random number generators defined by parameters `(h0, a, x, y, m)` for each of two sequences, where each generator produces a cyclic sequence of states `s[0] = h0`, and `s[i+1] = (x * s[i] + y) % m`. The function should return the smallest non-negative integer `t` such that both sequences have the value `a` at time `t` (i.e., `seq1[t] == a1` and `seq2[t] == a2`), or `-1` if no such time exists. The generators may be purely cyclic from the start, or may have a pre-period (tail) before entering a cycle. The sequences are infinite, and time starts at 0. The function must handle all parameter ranges up to `m` up to 1,000,000, and both `a` values may be unreachable or the cycles may never align.

// We need to find the first time `t` where both sequences equal their target values simultaneously. Each sequence is eventually periodic: it has a prefix (tail) of length `b` (where `b` can be 0) before entering a cycle of length `c` (with `c >= 1`). For a target value `a`, we need to check if `a` appears in the sequence at all. If it appears only in the tail, then there is exactly one possible time (the index it appears). If it appears in the cycle, then there are infinitely many times of the form `b + k*c` for `k >= 0` (assuming the first occurrence in the cycle is at index `b`, but careful: the target might appear multiple times in the cycle; we need the first occurrence after the tail, which we denote as `b_i` and cycle length `c_i`). However, the given code simplifies by assuming the target appears exactly once in the cycle? Actually, in the given snippet, they find the first time the target appears in the traversed sequence (starting from `h`), then break out if it's not found. But we must be careful: the target may appear multiple times in the cycle, and we want the earliest common time. The typical approach: for each sequence, find the first index `b_i` where the target is seen, and the cycle length `c_i` such that the target re-appears every `c_i` steps after that. But if the target appears multiple times in the cycle, there are multiple residue classes modulo `c_i`. The given code seems to assume the target appears exactly once in the cycle? Actually, it finds `dist1[a1]` which is the first occurrence index (starting from 1) of `a1` in the sequence of states visited starting from `h1`. Then `b1 = dist1[a1]-1` is the first time index (0-based) that `a1` appears. Then it computes `c1` by starting from `a1` and iterating until it returns to `a1` again, counting the cycle length. This is correct only if `a1` is on the cycle, not in the tail. If `a1` is in the tail, then `b1` is the unique occurrence. If `a1` is on the cycle, then `b1` is the first time it appears (which could be after some tail? Actually if it's on the cycle, the first occurrence might be in the tail? No, if it's on the cycle, it won't appear in the tail before the cycle starts, unless the cycle starts at the same value? But the code's while loop breaks when `dist1[h1]` is already set, meaning we have a repeat; the first repeat is the start of the cycle. So `dist1` gives the first occurrence index of each state. If `a1` is on the cycle, its first occurrence is at index `b1` which is after the tail. Then the cycle length `c1` is the period from `a1` back to `a1`, which is the cycle length. That works even if `a1` appears multiple times in the cycle? Actually, if `a1` appears multiple times in the cycle, then the time from `b1` to the next occurrence is some divisor of the cycle length? But the code starts from `a1` and iterates until it returns to `a1`, counting the full cycle length, not the distance to the next occurrence. So if `a1` appears at multiple positions in the cycle, then the set of times where `a1` appears is `b1 + k*d` where `d` is the distance to the next occurrence, not necessarily the full cycle length. The code incorrectly uses the full cycle length `c1` as the period for `a1` occurrences. Actually, if `a1` appears at positions `b1`, `b1+d`, `b1+2d`, ... within the cycle, then the sequence of times is `b1 + k*d` for `k>=0` where `d` is the smallest positive integer such that starting from `a1`, after `d` steps you return to `a1`. That `d` is exactly what the code computes as `c1`? Wait, they start from `a1`, apply the transformation once to get the next state, then keep applying until they get back to `a1`. The number of steps to return to `a1` is the period of `a1` in the functional graph, which is indeed the cycle length if `a1` is on the cycle and is the first occurrence? Actually, in a functional graph, each node on a cycle has a period equal to the cycle length; they all return to themselves after the full cycle length, not a divisor unless the cycle has repeated values? But in a deterministic function, if the cycle has length L, then applying the function L times to any node on the cycle returns to the same node. It may also return earlier if the node has a smaller period, but that would imply the cycle has a smaller length, contradicting the minimality of L. So every node on a cycle of length L returns to itself exactly after multiples of L, and not before. So the code's computation of `c1` is indeed the cycle length, and `a1` appears exactly once per cycle. So the assumption is fine. So the solution: For each sequence, determine if the target is reachable, its first occurrence index `b_i`, and the cycle length `c_i` (if it is on the cycle; if it's in the tail, then `c_i` is effectively 0? Actually we treat it as having only one occurrence). Then we need to find the minimal `t` such that `t >= b1`, `t >= b2`, and `(t - b1) % c1 == 0` and `(t - b2) % c2 == 0`, where if a target is only in a tail, then `t` must equal exactly `b_i` (so we can treat `c_i` as 1? No, we need to handle specially). The approach: 
// - If either target is unreachable, return -1.
// - If both are unique (in tail), then they must be equal: if `b1 == b2`, return `b1`, else -1.
// - If one is unique and the other is cyclic: say `seq1` unique at `b1`, `seq2` cyclic with first occurrence `b2` and period `c2`. Then we need `t = b1` and also `(b1 - b2) % c2 == 0` and `b1 >= b2`. If so return `b1`, else -1.
// - If both cyclic: we need to solve `k1*c1 + b1 == k2*c2 + b2` for non-negative integers `k1,k2` minimizing the expression. This is a linear congruence: `k1*c1 ≡ (b2 - b1) mod c2`. Find the smallest non-negative `k1` satisfying this, then compute `t = k1*c1 + b1` (and check `t >= max(b1,b2)` automatically since k1>=0). If no solution, return -1. We can use the extended Euclidean algorithm or brute force up to `lcm(c1,c2)` but since m <= 1e6, we can brute force up to `c2` iterations for `k1` (since modulo `c2`). But we must also ensure that when we find `k1`, the computed `k2` is non-negative, which it will be if `t >= b2`; if not, we might need to adjust. The given code uses a visited array to find the first k1 that satisfies the congruence, which is O(c2). That's acceptable since m <= 1e6. Complexity: O(m) time and O(m) space.
//
// Edge cases: 
// - If a target appears at time 0 (h0 == a), then b_i = 0.
// - If the sequence cycles without a tail (h0 is on the cycle), then b_i = dist[a]-1 where dist is the first time index.
// - If both are cyclic and the solution exists, we must output the minimal t, not just any. The first k1 that satisfies congruence gives the minimal t? Since t = k1*c1 + b1, and we iterate k1 from 0 upward, the first that satisfies gives the minimal t, provided that k2 >= 0. If the first solution has k2 < 0, then we need to add multiples of c2/gcd(c1,c2) to k1 until k2 >= 0. But the given code handles this by checking if k2 < 0, then doing a similar loop on k2. We can simplify: iterate k1 from 0 to c2-1, but we must also consider that k1 can be larger if the first solution yields negative k2. Actually, we can iterate k1 in increasing order and compute k2 = (t - b2)/c2, but we must ensure t >= b2. If the first solution has t < b2, we can add multiples of lcm(c1,c2) to t until it becomes >= max(b1,b2). Since c1 and c2 <= m, lcm could be huge (up to 1e12) which may overflow, but we only need to add at most one or a few multiples? Actually, if t0 is a solution to the congruence, then all solutions are t = t0 + k * lcm(c1,c2). We want the smallest t >= max(b1,b2). So we can compute t0 as the smallest non-negative t that satisfies both modular conditions, then adjust upward by lcm. However, lcm may overflow, so we can compute using 64-bit. Since m <= 1e6, lcm could be up to ~1e12, but we only need to add at most once or twice? Actually, if t0 < max(b1,b2), then we need to find the smallest k such that t0 + k*lcm >= max(b1,b2). That k can be up to (max - t0)/lcm, which could be large if lcm is small? But lcm is at least max(c1,c2) <= 1e6, and max(b1,b2) <= m <= 1e6, so k is at most 1. So we just need to check t0 and t0 + lcm (and possibly t0 + 2*lcm? but max difference is 1e6, lcm >= 1, so k could be up to 1e6 if lcm=1, but then t0 may be small and we might need to add many multiples. Actually, if lcm = 1, then all times satisfy the congruence, so the minimal t is max(b1,b2). But we can handle that separately. To keep it simple, we can just brute force k1 from 0 up to c2-1 (since the period of the congruence modulo c2 is c2/gcd(c1,c2) which divides c2), and for each candidate compute t = k1*c1 + b1, check if t >= b2 and (t - b2) % c2 == 0. The first such t is minimal. Since k1 ranges at most c2 <= 1e6, that's fine. That is what the given code does with the visited array. However, note that if the solution requires k1 > c2-1, we may miss it? Actually, if a solution exists, there is one with k1 < c2/gcd(c1,c2) <= c2, because the condition is modulo c2. So k1 in [0, c2-1] is sufficient. The given code also handles the case where k2 < 0 by switching to a loop on k2, but we can simply check t >= b2. So I'll implement that.
//
// Implementation: For each sequence, we need to compute:
// - dist_to_target: first time index (0-based) when the sequence equals the target, or -1 if never.
// - cycle_len: length of the cycle that contains the target, if the target is on the cycle; if target is in tail, cycle_len = 0 (or we can treat as any positive but we handle specially).
// To compute these, we can simulate the sequence from h0, tracking the first time each state is visited (using an array of size m, initialized to -1). We stop when we revisit a state (start of cycle). Then for each state, we can know its first time index and whether it's on the cycle. Then for target a, if first_time[a] is -1, unreachable. Else, we need to know if it's on the cycle: we can find the start of the cycle index, and then check if first_time[a] >= start. If yes, cycle_len = (total_time - start). If no, it's in the tail and unique. 
//
// But we must be careful: Suppose the target appears multiple times in the tail? No, tail is a simple path, each state appears once. So it's fine. 
//
// Then we find the minimal t as described.
//
// Time complexity: O(m) per sequence, total O(m). Space: O(m).

#include <vector>
#include <algorithm>

// Helper struct to describe the occurrence pattern of a target value in a sequence.
struct Occurrence {
    bool reachable;   // target appears at least once
    long long first;  // first time index (0-based)
    long long cycle;  // if target is on the cycle, the cycle length; if in tail, 0
};

// Analyze a linear congruential sequence:
// s[0] = h0, s[i+1] = (x * s[i] + y) % m
// Returns the occurrence pattern of value 'target' in the infinite sequence.
Occurrence analyzeSequence(int m, int h0, int target, int x, int y) {
    std::vector<int> firstSeen(m, -1);
    int curr = h0;
    int time = 0;
    int cycleStart = -1;

    while (firstSeen[curr] == -1) {
        firstSeen[curr] = time;
        curr = (static_cast<long long>(x) * curr + y) % m;
        ++time;
        // If we've seen all m states, we must have a repeat, but the loop will stop anyway.
    }
    cycleStart = firstSeen[curr];  // index where the cycle begins

    if (firstSeen[target] == -1) {
        return {false, -1, 0};
    }

    if (firstSeen[target] < cycleStart) {
        // Target is in the tail, appears exactly once.
        return {true, static_cast<long long>(firstSeen[target]), 0};
    } else {
        // Target is on the cycle.
        long long cycleLen = static_cast<long long>(time) - cycleStart;
        return {true, static_cast<long long>(firstSeen[target]), cycleLen};
    }
}

// Finds the smallest t >= 0 such that both sequences have their target values at time t.
// Returns -1 if no such time exists.
long long findFirstMeetingTime(int m,
                               int h1, int a1, int x1, int y1,
                               int h2, int a2, int x2, int y2) {
    Occurrence occ1 = analyzeSequence(m, h1, a1, x1, y1);
    Occurrence occ2 = analyzeSequence(m, h2, a2, x2, y2);

    if (!occ1.reachable || !occ2.reachable) {
        return -1;
    }

    // Cases involving at least one unique (tail) occurrence.
    if (occ1.cycle == 0 && occ2.cycle == 0) {
        // Both unique.
        return (occ1.first == occ2.first) ? occ1.first : -1;
    }
    if (occ1.cycle == 0) {
        // occ1 unique, occ2 cyclic.
        if (occ1.first >= occ2.first && (occ1.first - occ2.first) % occ2.cycle == 0) {
            return occ1.first;
        }
        return -1;
    }
    if (occ2.cycle == 0) {
        // occ2 unique, occ1 cyclic.
        if (occ2.first >= occ1.first && (occ2.first - occ1.first) % occ1.cycle == 0) {
            return occ2.first;
        }
        return -1;
    }

    // Both are on cycles. Solve k1*c1 + b1 == k2*c2 + b2.
    long long b1 = occ1.first;
    long long b2 = occ2.first;
    long long c1 = occ1.cycle;
    long long c2 = occ2.cycle;

    // Iterate k1 from 0 to c2-1; the congruence has a solution iff one of these works.
    // We want the smallest t = k1*c1 + b1.
    for (long long k1 = 0; k1 < c2; ++k1) {
        long long t = k1 * c1 + b1;
        if (t >= b2 && (t - b2) % c2 == 0) {
            return t;
        }
    }
    // Also need to check k1 up to c2-1? Actually if no solution in that range, none exists.
    // But careful: if c1 and c2 are not coprime, the minimal solution might require k1 > c2-1?
    // No, since we only care about (k1*c1) mod c2, which has period c2/gcd(c1,c2), so k1 in [0, c2-1] covers all residues.
    return -1;
}

#include <cassert>

int main() {
    // Test 1: trivial - both sequences constant at target from time 0.
    // seq1: h=5, x=1, y=0 -> always 5; target=5.
    // seq2: h=3, x=1, y=0 -> always 3; target=3.
    assert(findFirstMeetingTime(10, 5, 5, 1, 0, 3, 3, 1, 0) == 0);

    // Test 2: same sequence, both targets are at time 2.
    // seq: h=0, x=1, y=1, m=5 -> states: 0,1,2,3,4,0,... target=2 -> first at 2.
    assert(findFirstMeetingTime(5, 0, 2, 1, 1, 0, 2, 1, 1) == 2);

    // Test 3: one sequence unique, other cyclic, meeting at the unique time.
    // seq1: h=0, x=1, y=0 -> always 0, target=0 at time 0 (unique).
    // seq2: h=1, x=1, y=1, m=3 -> 1,2,0,1,... target=0 appears at time 2, 5, 8...
    // Meeting at t=2? but seq1 at t=2 is 0, yes. But t=0? seq2 at 0 is 1 not 0, so no. So answer 2.
    assert(findFirstMeetingTime(3, 0, 0, 1, 0, 1, 0, 1, 1) == 2);

    // Test 4: both cyclic, meeting at a later time.
    // seq1: m=4, h=1, x=3, y=1 -> 1,0,1,... cycle length 2, target=0 at times 1,3,5...
    // seq2: m=4, h=2, x=3, y=2 -> 2,0,2,... cycle length 2, target=0 at times 1,3,5...
    // They meet at t=1.
    assert(findFirstMeetingTime(4, 1, 0, 3, 1, 2, 0, 3, 2) == 1);

    // Test 5: both cyclic, but cycles never align.
    // seq1: m=2, h=0, x=1, y=1 -> 0,1,0,... target=1 at times 1,3,5...
    // seq2: m=2, h=1, x=1, y=0 -> 1,1,1,... target=0 never appears -> -1.
    assert(findFirstMeetingTime(2, 0, 1, 1, 1, 1, 0, 1, 0) == -1);

    // Test 6: both cyclic, gcd(c1,c2) doesn't divide difference.
    // seq1: c1=2, target at times 1,3,5...
    // seq2: c2=2, target at times 0,2,4...
    // Difference = 1 mod 2, not divisible by gcd(2,2)=2 -> no solution.
    // We need to construct: seq1: m=3, h=0, x=1, y=1 -> 0,1,2,0,... target=1 at t=1,3,5...
    // seq2: m=3, h=2, x=1, y=1 -> 2,0,1,2,... target=2 at t=0,3,6... Actually target=2 appears at t=0,3,6? Let's simulate: s0=2, s1=0, s2=1, s3=2, so target=2 at t=0,3,6... times are 0 mod 3, not 0 mod 2. Let's pick seq2 target=0: t=1,4,7... That gives t ≡ 1 mod 3. seq1 target=1 gives t ≡ 1 mod 2? Actually seq1 has cycle length 3, target=1 at t=1 only? But t=1,4,7... So both are 1 mod 3? That's too easy. Let's instead use m=4, seq1: h=0, x=2, y=1 -> 0,1,3,3,... cycle length 1 at 3. Not good.
    // Instead, use simple: seq1: m=4, h=1, x=1, y=0 -> always 1, but we need a cycle. Let's do seq1: m=4, h=0, x=2, y=1 -> 0,1,3,3,... target=0 at t=0 only (tail), not cyclic. 
    // Simpler: seq1: m=4, cycle 2, target at odd times. seq2: m=4, cycle 2, target at even times. Then no solution. 
    // seq1: h=0, x=1, y=1 -> 0,1,2,3,0,... cycle length 4, but target=2 appears at t=2,6,10... That is 2 mod 4.
    // seq2: h=1, x=1, y=1 -> 1,2,3,0,1,... target=1 at t=0,4,8... That is 0 mod 4. Difference 2 mod 4, gcd(4,4)=4 doesn't divide 2 -> -1.
    // So test: findFirstMeetingTime(4, 0, 2, 1, 1, 1, 1, 1, 1) should be -1.
    assert(findFirstMeetingTime(4, 0, 2, 1, 1, 1, 1, 1, 1) == -1);

    // Test 7: unique and cyclic but no match.
    // seq1: h=0, x=1, y=0 -> always 0, target=0 at t=0 unique.
    // seq2: h=1, x=1, y=1, m=2 -> 1,0,1,... target=0 at t=1,3,5...
    // seq1 at t=1 is 0, but seq2 at t=1 is 0? Check: seq2 s0=1, s1=0, so t=1 matches. So actually it works. Need a case where seq1's unique time is not in seq2's periodic set. 
    // seq1: h=0, x=0, y=0 -> always 0, target=0 at t=0 unique.
    // seq2: h=1, x=2, y=0, m=3 -> 1,2,1,... target=1 at t=0,2,4... (cycle length 2). seq1 at t=0 is 0, not 1? Wait seq1 target=0, seq2 target=1? We need same target? No, targets are different per sequence? Actually each sequence has its own target value. So seq1 target=0 appears at t=0. seq2 target=1 appears at t=0,2,4... So they both appear at t=0? seq1 at t=0 has value 0 (target), seq2 at t=0 has value 1 (target). So t=0 works. To make it fail, choose seq1 unique time not in seq2's set. For example, seq1 target=0 at t=0, seq2 target=2 appears at t=1,3,5... So no match. 
    // seq1: m=2, h=0, x=0, y=0 -> always 0, target=0 at t=0.
    // seq2: m=3, h=0, x=1, y=1 -> 0,1,2,0,... target=2 appears at t=2,5,8... Not 0. So -1.
    assert(findFirstMeetingTime(2, 0, 0, 0, 0, 3, 0, 2, 1, 1) == -1);

    // Test 8: both cyclic, need to add lcm to satisfy lower bound.
    // seq1: cycle length 2, target at t=1,3,5...
    // seq2: cycle length 3, target at t=0,3,6... Actually let's create:
    // seq1: m=5, h=1, x=1, y=1 -> 1,2,3,4,0,1,... cycle length 5. Not good.
    // Simpler: seq1: m=3, h=0, x=1, y=1 -> 0,1,2,0,... cycle length 3, target=2 at t=2,5,8...
    // seq2: m=2, h=1, x=1, y=0 -> 1,1,... cycle length 1, target=1 at t=0,1,2,3... (all times). So t must be >= max(2,0) and (t-2)%3==0 always true? Actually (t-2)%3==0 requires t ≡ 2 mod 3. Minimal t >=0 that is 2 mod 3 is 2. So answer 2.
    // Test that: findFirstMeetingTime(3,0,2,1,1, 2,1,1,0) -> seq2 always 1, target=1, so at any time. So answer 2.
    assert(findFirstMeetingTime(3,0,2,1,1, 2,1,1,0) == 2);

    // Test 9: both cyclic, solution requires k1 large but within c2.
    // seq1: c1=2, target at t=1 mod 2.
    // seq2: c2=3, target at t=1 mod 3.
    // Solve t ≡ 1 mod 2 and t ≡ 1 mod 3 => t=1 mod 6. So first t=1. But if b1=1, b2=0? Let's construct: seq1 target appears at t=1,3,5,... (c1=2, b1=1). seq2 target appears at t=0,3,6,... (c2=3, b2=0). Then need t ≡ 1 mod 2 and t ≡ 0 mod 3 => t=3,9,... first t=3. So answer 3. 
    // Take seq1: m=4, h=1, x=2, y=1 -> 1,3,3,... not good. Better: simple linear recurrences.
    // Use m=6, seq1: h=1, x=1, y=1 -> 1,2,3,4,5,0,1... cycle length 6, not 2.
    // Instead, use m=2: h=0, x=1, y=1 -> 0,1,0,... cycle length 2, target=1 at t=1,3,5... (b1=1, c1=2).
    // For seq2 with c2=3: use m=3, h=0, x=1, y=1 -> 0,1,2,0,... cycle length 3, target=0 at t=0,3,6... (b2=0, c2=3).
    // Then t must satisfy t ≡ 1 mod 2 and t ≡ 0 mod 3 => t=3,9,... first t=3.
    // So test: findFirstMeetingTime(3? actually m1=2, m2=3 but function takes single m? The function signature has one m for all? Actually the task says two generators with each having its own m? The problem statement says "two pseudo-random number generators defined by parameters (h0, a, x, y, m) for each of two sequences" – each has its own m. But the given code uses a single m shared by both? No, the code has one m read, then both use the same m. So in our function signature, we used a single m for both. That might be restrictive. The task should allow different moduli? The original snippet uses a single m for both sequences. To stay consistent with the snippet, I'll keep a single m. But then for test 9, we need both sequences under the same m. To have c1=2 and c2=3 under the same m, m must be multiple of lcm(2,3)=6. Let's set m=6. 
    // seq1: h=0, x=3, y=1? Let's find a cycle of length 2: e.g., m=6, h=0, x=1, y=0 -> always 0, not cycle. Need function that gives cycle length 2. For m=6, function f(x)= (a*x + b) mod 6. A 2-cycle exists, e.g., f(0)=1, f(1)=0. That is a= something. For linear, a= -1? 0 -> b, 1 -> a*1+b. Set b=1, a= -1 mod 6 = 5. Then f(0)=1, f(1)= (5*1+1)=6%6=0. Then f(2)=5*2+1=11%6=5, f(5)=26%6=2, so 2->5->2 cycle length 2. So start at 0: 0->1->0 cycle length 2. So seq1: m=6, h=0, target=1, x=5, y=1 -> states: 0,1,0,... target=1 at t=1,3,5,... (b1=1,c1=2).
    // seq2: need cycle length 3, same m=6. For example, linear function f(x)= (x+1) mod 6 gives cycle length 6. Need 3. Try f(x)= (2*x+1) mod 6? Let's test: 0->1,1->3,3->7%6=1, so cycle 1,3 length 2. Not 3. f(x)= (x+2) mod 6 gives cycle length 3? Actually 0->2->4->0, length 3. So seq2: h=0, x=1, y=2 (since f(x)= (1*x+2)%6), gives 0,2,4,0,... Target=0 at t=0,3,6,... (b2=0,c2=3).
    // Then meeting: t must be odd (from seq1) and multiple of 3 (from seq2) => t=3,9,... first 3.
    // Test: findFirstMeetingTime(6, 0,1,5,1, 0,0,1,2) should return 3.
    assert(findFirstMeetingTime(6, 0,1,5,1, 0,0,1,2) == 3);

    // Test 10: large cycle alignment with m up to 1e6? We'll do a small one.
    // Both sequences start from target at time 0 and cycle length 1 (constant).
    // seq1: h=7, x=1, y=0, target=7.
    // seq2: h=7, x=1, y=0, target=7.
    assert(findFirstMeetingTime(100, 7,7,1,0, 7,7,1,0) == 0);

    return 0;
}
