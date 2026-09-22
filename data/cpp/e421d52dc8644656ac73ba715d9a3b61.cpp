/*
Write a C++ function that simulates the synchronized movement of `B` beasts through a zoo with `Z+1` positions (numbered 0 to Z). Each beast follows a deterministic transition table: for beast `i` and current position `p`, the next position is `next[i][p]`. All beasts start at position 0 at time 0 and move simultaneously every time step. The function must determine whether there exists a time `T` (non-negative integer) and a position `P` such that all beasts are at position `P` at time `T`. If such a meeting exists, return a pair `(P, T)` where `P` is the smallest position that can be the meeting point (if multiple positions meet at the same earliest time, choose the smallest position index), and `T` is the earliest time at which that meeting occurs. If no meeting is ever possible, return `(-1, -1)`. Input constraints: `1 ≤ B ≤ 10`, `1 ≤ Z ≤ 100`, and each transition value is between 0 and Z inclusive. The function signature is `pair<int,long long> findMeetingTime(const vector<vector<int>>& next)` where `next` is a `B x (Z+1)` matrix. Note that the meeting time `T` may exceed 32-bit integers, so return it as `long long`. All transitions are deterministic, so each beast eventually enters a cycle. The problem is to solve using the Chinese Remainder Theorem after preprocessing each beast’s trajectory to find its cycle and entry time.
*/

#include <vector>
#include <algorithm>
#include <numeric>
#include <cstdint>

using i128 = __int128_t;

static i128 gcd(i128 a, i128 b) {
    while (b) { a %= b; std::swap(a, b); }
    return a;
}

static i128 lcm(i128 a, i128 b) {
    return a / gcd(a, b) * b;
}

// Extended Euclid for i128. Returns gcd, and sets x,y such that a*x + b*y = gcd.
static i128 egcd(i128 a, i128 b, i128& x, i128& y) {
    if (b == 0) { x = 1; y = 0; return a; }
    i128 x1, y1;
    i128 g = egcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

// Solve CRT for a system of congruences x ≡ r[i] (mod m[i]).
// Returns true and sets ans to the smallest non-negative solution, or false if impossible.
static bool crt(const std::vector<i128>& mods, const std::vector<i128>& rems, i128& ans) {
    i128 cur_mod = mods[0];
    i128 cur_rem = rems[0] % cur_mod;
    for (size_t i = 1; i < mods.size(); ++i) {
        i128 m2 = mods[i];
        i128 r2 = rems[i] % m2;
        i128 g = gcd(cur_mod, m2);
        if ((cur_rem - r2) % g != 0) return false;
        i128 p, q;
        i128 m1 = cur_mod / g;
        i128 m2s = m2 / g;
        egcd(m1, m2s, p, q);
        // Solution: ans = cur_rem + cur_mod * p * ((r2 - cur_rem)/g) mod (lcm)
        i128 l = lcm(cur_mod, m2);
        i128 t = ((r2 - cur_rem) / g) % m2s;
        if (t < 0) t += m2s;
        i128 step = (cur_mod / g) * p % m2s;
        if (step < 0) step += m2s;
        cur_mod = l;
        cur_rem = (cur_rem + (cur_mod/g) * step % cur_mod) % cur_mod;
        // Simpler: use the standard CRT formula:
        // x = (r1 + m1 * p * ((r2-r1)/g)) mod l
        // We'll re-derive cleanly below.
        // Recompute using a robust method:
        i128 x1, y1;
        egcd(m1, m2s, x1, y1);
        i128 k = ((r2 - cur_rem) / g) % m2s;
        if (k < 0) k += m2s;
        i128 add = (cur_mod * x1 % cur_mod) * k % cur_mod;
        if (add < 0) add += cur_mod;
        cur_rem = (cur_rem + add) % cur_mod;
        cur_mod = l;
    }
    ans = cur_rem;
    return true;
}

std::pair<int, long long> findMeetingTime(const std::vector<std::vector<int>>& next) {
    int B = (int)next.size();
    int Z = (int)next[0].size() - 1; // positions 0..Z

    // First, quick check: simulate up to Z steps from start, if all meet.
    {
        std::vector<int> at(B, 0);
        for (int t = 0; t <= Z; ++t) {
            // check if all equal
            bool all_same = true;
            for (int i = 1; i < B; ++i) if (at[i] != at[0]) { all_same = false; break; }
            if (all_same) {
                // smallest position among at[0]? Actually they are all same, so position = at[0].
                // But we need smallest position that works at this earliest time? Since they are all equal now, only one position.
                return {at[0], t};
            }
            // move to next
            for (int i = 0; i < B; ++i) at[i] = next[i][at[i]];
        }
    }

    // For each beast, find cycle and entry time.
    std::vector<int> t_to_cycle(B);
    std::vector<std::vector<int>> cycle(B);
    for (int i = 0; i < B; ++i) {
        const auto& trans = next[i];
        std::vector<int> visited(Z+1, -1);
        std::vector<int> path;
        int pos = 0;
        int step = 0;
        while (visited[pos] == -1) {
            visited[pos] = step++;
            path.push_back(pos);
            pos = trans[pos];
        }
        // visited[pos] is the start of cycle
        t_to_cycle[i] = visited[pos];
        cycle[i].clear();
        for (int k = visited[pos]; k < (int)path.size(); ++k) {
            cycle[i].push_back(path[k]);
        }
    }

    int max_tc = 0;
    for (int i = 0; i < B; ++i) max_tc = std::max(max_tc, t_to_cycle[i]);

    // Rotate each cycle so that at time max_tc, the first element is the beast's position.
    std::vector<std::vector<int>> rotated(B);
    std::vector<std::vector<int>> zoo_pos(B, std::vector<int>(Z+1, -1));
    for (int i = 0; i < B; ++i) {
        int m = (int)cycle[i].size();
        int f = (max_tc - t_to_cycle[i]) % m;
        if (f < 0) f += m;
        rotated[i].resize(m);
        for (int j = 0; j < m; ++j) {
            int u = cycle[i][(j + f) % m];
            rotated[i][j] = u;
            zoo_pos[i][u] = j;
        }
    }

    // Now try each position p (1..Z) as potential meeting point.
    long long best_time = LLONG_MAX;
    int best_pos = -1;

    for (int p = 1; p <= Z; ++p) {
        bool ok = true;
        std::vector<i128> mods(B), rems(B);
        for (int i = 0; i < B; ++i) {
            if (zoo_pos[i][p] == -1) { ok = false; break; }
            mods[i] = (i128)rotated[i].size();
            rems[i] = (i128)zoo_pos[i][p];
        }
        if (!ok) continue;
        i128 ans;
        if (crt(mods, rems, ans)) {
            long long time = (long long)ans + max_tc;
            if (time < best_time || (time == best_time && p < best_pos)) {
                best_time = time;
                best_pos = p;
            }
        }
    }

    if (best_pos == -1) return {-1, -1};
    return {best_pos, best_time};
}

#include <cassert>
#include <vector>
#include <utility>

// The function is defined above; we just need main for tests.

int main() {
    // Example 1: beasts meet after 1 step at position 1
    {
        std::vector<std::vector<int>> next = {
            {1, 0, 0},
            {1, 2, 0}
        }; // B=2, Z=2. Beast0: 0->1->0..., Beast1: 0->1->2->0... Actually let's design.
        // Better: design a simple case where all go to 1 at step 1.
        // Beast0: next[0][0]=1, next[0][1]=0, next[0][2]=0
        // Beast1: next[1][0]=1, next[1][1]=1, next[1][2]=1
        // Then at t=1 both at 1.
        next = {
            {1, 0, 0},
            {1, 1, 1}
        };
        auto res = findMeetingTime(next);
        assert(res.first == 1 && res.second == 1);
    }

    // Example 2: meet at start t=0 at position 0
    {
        std::vector<std::vector<int>> next = {
            {0, 1, 0},
            {0, 2, 1}
        };
        auto res = findMeetingTime(next);
        assert(res.first == 0 && res.second == 0);
    }

    // Example 3: no meeting possible
    {
        std::vector<std::vector<int>> next = {
            {1, 0, 0},
            {2, 0, 0}
        }; // Beast0 alternates 0<->1, Beast1 alternates 0<->2, never together
        auto res = findMeetingTime(next);
        assert(res.first == -1 && res.second == -1);
    }

    // Example 4: meet after entering cycles, with different entry times
    {
        // Beast0: starts 0->1->2->3->2->3... cycle [2,3] entry at t=2
        // Beast1: starts 0->1->2->3->4->3->4... cycle [3,4] entry at t=3
        // They meet at position 3? time? Let's simulate:
        // t=0: (0,0)
        // t=1: (1,1)
        // t=2: (2,2)
        // t=3: (3,3) both at 3! So answer (3,3).
        std::vector<std::vector<int>> next = {
            {1,2,3,2,2}, // B0: 0->1,1->2,2->3,3->2,4->2 (but Z=4)
            {1,2,3,4,3}  // B1: 0->1,1->2,2->3,3->4,4->3
        }; // But our matrix size must be B x (Z+1)=2x5. Here Z=4. Good.
        auto res = findMeetingTime(next);
        // At t=3 both at 3. But check if meet earlier? t=0 not, t=1 not, t=2 not (2 vs 2? Actually B0 at 2, B1 at 2 => they are both at 2 at t=2! Let's recalc:
        // B0: t0=0, t1=1, t2=2, t3=3, t4=2...
        // B1: t0=0, t1=1, t2=2, t3=3, t4=4...
        // At t=2 both at 2! So answer (2,2). Let's adjust test.
        // Let's design one where they meet later.
        // B0: 0->1,1->2,2->3,3->2,4->2 (cycle [2,3] after t=2)
        // B1: 0->1,1->3,2->3,3->4,4->3 (cycle [3,4] after t=1? Actually 0->1,1->3, then 3->4->3... so cycle [3,4] after t=1)
        // Check meeting: t0 (0,0), t1 (1,3) no, t2 (2,4) no, t3 (3,3) yes! Both at 3 at t=3.
        next = {
            {1,2,3,2,2},
            {1,3,3,4,3}
        };
        res = findMeetingTime(next);
        assert(res.first == 3 && res.second == 3);
    }

    // Example 5: meeting requires CRT with different moduli
    {
        // Beast0 cycle length 2: positions [1,2] starting at t=0? Actually simpler.
        // Let's craft: Beast0: 0->1->2->1->2... so cycle [1,2] length 2, t_to_cycle=1.
        // Beast1: 0->3->4->5->3->4... cycle [3,4,5] length 3, t_to_cycle=1.
        // We want a common position p that both cycles share. Let's set p=1? Beast1 doesn't have 1. p=3? Beast0 doesn't have 3. So add position 2 to Beast1's cycle? 
        // Let's design both cycles include position 5: Beast0 cycle [1,5] length 2, Beast1 cycle [3,4,5] length 3.
        // Beast0: 0->1->5->1..., t_to_cycle=1, cycle=[1,5]
        // Beast1: 0->3->4->5->3..., t_to_cycle=1, cycle=[3,4,5]
        // max_tc=1. At t=1, Beast0 at 1, Beast1 at 3. Now we need a common position in both cycles: only 5. 
        // beast0 pos of 5 in its cycle after rotation? Original cycle [1,5] at t=1 (offset 0 because t_to_cycle=1=max_tc) so at t=1 position 1, at t=2 position 5, so zoo_pos[0][5]=1. beast1 cycle [3,4,5] at t=1 position 3, so at t=2 position 4, t=3 position 5, so zoo_pos[1][5]=2.
        // CRT: x ≡1 mod2, x≡2 mod3 => solution x=5 (5 mod2=1, 5 mod3=2). So meeting at t=1+5=6 at position 5.
        // Check brute: t1: (1,3), t2:(5,4), t3:(1,5) no, t4:(5,3), t5:(1,4), t6:(5,5) yes! So (5,6).
        std::vector<std::vector<int>> next = {
            {1,5,1,1,1,1}, // B0: 0->1,1->5, others cycle
            {3,4,5,3,4,5}  // B1: 0->3,3->4,4->5,5->3
        };
        auto res = findMeetingTime(next);
        assert(res.first == 5 && res.second == 6);
    }

    return 0;
}

// The solution processes each beast independently. For each beast `i`, we simulate its trajectory starting from position 0 until we detect a cycle (using a visited array indexed by zoo position). This yields for each beast: the time `t_to_cycle[i]` (number of steps before entering the cycle) and the cycle itself as a list of positions `cycle[i]` with size `m[i]`. The cycle is standard: once the beast reaches a position it has seen before, that position is the cycle start, and the cycle is the list from that start until before the repeated position. After processing all beasts, we compute `max_tc = max(t_to_cycle[i])`. For a valid meeting, all beasts must be in their cycles after `max_tc` steps, because before that some beasts might not have entered cycles yet. We then rotate each cycle so that at time `max_tc`, each beast is at the first element of its rotated cycle. Specifically, for beast `i`, we find the offset `f = (max_tc - t_to_cycle[i]) % m[i]`, and rotate the cycle list so that at time `max_tc`, position `cycle[i][(j+f)%m]` becomes the new `j`-th element. We also record for each beast a mapping from zoo position to its relative index in the rotated cycle via `zoo_pos[i][pos]`. Then we iterate over all possible zoo positions `p` from 1 to Z (position 0 is never a meeting point because if all start together at time 0, `try_reach` would have caught it, but we can include it for completeness; the original code excludes it but we can include all). For each position `p`, we require that every beast has `p` in its cycle (i.e., `zoo_pos[i][p] != -1`). If so, the meeting time relative to `max_tc` must satisfy `T + t_to_cycle[i] ≡ zoo_pos[i][p] (mod m[i])` for each beast. This is a system of congruences of the form `x ≡ r[i] (mod m[i])` where `r[i] = zoo_pos[i][p]` and `x` is the number of extra steps after `max_tc`. We solve this system using the Chinese Remainder Theorem (CRT). If a solution exists, the actual meeting time is `ans + max_tc`, where `ans` is the CRT solution (the smallest non-negative `x` satisfying all congruences). We track the smallest `ans + max_tc` overall, and if ties, choose the smallest `p`. If no position yields a feasible CRT system, return `(-1,-1)`. Edge cases: beasts may have different cycle lengths, some positions may not appear in some cycles, and the CRT may be inconsistent. Also, the meeting may occur before all beasts enter cycles; we handle that by first checking the `try_reach` scenario: simulate all beasts for up to `Z` steps from time 0 and see if they all coincide at any of those times. If so, return the earliest such time and smallest position. Complexity: For each beast, cycle detection takes O(Z) time, and we have at most B beasts, so O(B*Z). Then for each position (Z+1) we run CRT with B equations, each CRT step is O(log max_mod) with Euclid, so total O(Z * B * log Z). Overall time O(B*Z + Z*B*log Z) = O(B*Z log Z) which is trivial for B≤10, Z≤100. Space O(B*Z) for cycle storage.
