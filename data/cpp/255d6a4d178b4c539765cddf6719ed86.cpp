// You are given a circular queue of `n` animals (3 ≤ n ≤ 6000), each with three positive integer attack powers `k[0]`, `k[1]`, `k[2]`. Initially, the first animal (index 0) is the current "champion" with a victory streak counter starting at 0. Repeatedly, the champion compares its `k[0]` against the `k[0]` of the next animal in the circular queue. If the champion's `k[0]` is strictly greater than the challenger's `k[0]`, the champion wins, increments its streak by 1, and the challenger moves to the back of the queue. Otherwise, the challenger becomes the new champion with a streak of 1, and the old champion moves to the back. The process stops when some animal achieves a streak of exactly 3 consecutive wins; that animal's original index and the 1‑based round number (starting from 1) are reported. If no one ever achieves a streak of 3 within the first 20,000 rounds, or if the cycle repeats with no possibility of a streak of 3, output `-1 -1`. Write a C++ function `pair<int,long long> findWinner(const vector<array<int,3>>& animals)` that returns the winner's original index (as the first element) and the round number (as the second element), or `{-1, -1}` if the process never produces a streak of 3. Implement the simulation exactly as described, including the special fast‑path logic for large `n` that the provided snippet uses.

// The problem is a simulation with a potential long period. A brute‑force simulation of up to 20,000 rounds is fine for the first check, but for large `n` the cycle can exceed that limit. The snippet uses an optimization: after a possible early exit, it reorders the queue so that the initially weaker animal (in terms of `k[0]` vs the next) becomes the first champion. Then it classifies animals as "red" if they can never become champion because their left neighbor has a higher `k[0]`. Non‑red animals are stored in a smaller list `b`, and we track for each animal its next red neighbor (if any). The main loop uses a monotonic stack (`st`) over `b` to find, for each red neighbor, the closest (in circular distance) animal in `b` with a `k[1]` value that is ≥ the red's `k[0]`; that distance is the number of rounds needed for the current champion to traverse to that red. We take the minimum such distance over all reds; if none exists, there's no possible streak and we return `-1`. Otherwise, we advance all animals' positions by that distance, effectively skipping many rounds at once. Then we handle actual wins/losses at the boundary, updating streaks and colors. Finally, we check for a green animal (streak already 2) whose next is a red (so it beats it in the next round), giving the answer as the sum of skipped rounds plus the red's position + 2. This process repeats until an answer is found. Key edge cases: when the minimum distance is 0, we must not skip any rounds, and we directly process the conflicting boundary; when `b` becomes empty or no red exists, output `-1`. Complexity: the brute–force runs in O(20,000·n) worst‑case but for `n` large it terminates early; the optimized loop runs at most O(n) iterations, each doing O(|b|) work, giving O(n²) worst‑case, which is acceptable for `n≤6000`. Space is O(n).

#include <bits/stdc++.h>
using namespace std;

using Animal = array<int,3>;

// Simulates the described battle process and returns {winner_index, round} or {-1,-1}.
pair<int,long long> findWinner(const vector<Animal>& animals) {
    int n = (int)animals.size();
    if (n < 3) return {-1, -1}; // not possible per constraints but safe

    struct A {
        array<int,3> k;
        int id, pos, nxt;
        int color; // 0=red,1=blue,2=green
    };

    vector<A> a(n);
    for (int i = 0; i < n; ++i) {
        a[i].k = animals[i];
        a[i].id = i;
        a[i].color = 1; // blue initially
        a[i].nxt = -1;
    }

    // Brute force first 20000 rounds
    {
        deque<A> q;
        for (int i = 1; i < n; ++i) q.push_back(a[i]);
        A cur = a[0];
        int cnt = 0;
        const int LIM = 20000;
        for (int x = 1; x <= LIM; ++x) {
            A now = q.front(); q.pop_front();
            if (cur.k[0] > now.k[0]) {
                ++cnt;
                q.push_back(now);
            } else {
                cnt = 1;
                swap(cur, now);
                q.push_back(now);
            }
            if (cnt == 3) return {cur.id, (long long)x};
        }
    }

    // Rearrange so that first animal is weaker (or equal) than second in k0
    if (a[0].k[0] > a[1].k[0]) swap(a[0], a[1]);

    // Rotate to make a[0] the "current champion" at start of main loop
    vector<A> tmp;
    for (int i = 0; i < n; ++i) tmp.push_back(a[(i+1)%n]);
    a = tmp;

    // Mark red: cannot become champion because left neighbor has higher k0
    for (int i = 0; i < n; ++i) {
        a[i].pos = i;
        if (i > 0 && a[i-1].k[0] > a[i].k[0]) a[i].color = 0;
    }

    // Assign colors and compute nxt reds
    for (int i = 0; i < n; ++i) {
        if (a[i].color == 0) continue;
        int nxt = (i+1)%n;
        if (a[nxt].color == 0) nxt = (nxt+1)%n;
        a[i].color = (a[i].k[2] > a[nxt].k[0]) ? 2 : 1; // green or blue
    }

    // Build b: only non-red, but keep link to next red if any
    vector<A> b;
    vector<int> mapToB(n, -1);
    for (int i = 0; i < n; ++i) {
        if (a[i].color == 0) continue;
        int nxt = (i+1)%n;
        if (a[nxt].color == 0) a[i].nxt = a[nxt].pos;
        b.push_back(a[i]);
        mapToB[i] = (int)b.size()-1;
    }

    long long sum = 0;
    const long long INF = 0x3f3f3f3f3f3f3f3f;

    while (true) {
        // Build monotonic stack of (k1, pos_in_b, index_in_b) for blue and green
        vector<tuple<long long, long long, long long>> st; // (k1, pos_in_b, idx_in_b)
        vector<pair<long long, long long>> reds; // (nxt_pos, idx_in_b)

        st.clear();
        for (int i = 0; i < (int)b.size(); ++i) {
            long long key = (b[i].color == 1 ? b[i].k[1] : -INF);
            while (!st.empty() && get<0>(st.back()) >= key) st.pop_back();
            st.push_back({key, b[i].pos, i});
        }

        long long minDist = INF;
        for (int i = 0; i < (int)b.size(); ++i) {
            long long key = (b[i].color == 1 ? b[i].k[1] : -INF);
            while (!st.empty() && get<0>(st.back()) >= key) st.pop_back();
            st.push_back({key, b[i].pos, i});

            int nxtr = b[i].nxt;
            if (nxtr == -1) continue;
            reds.push_back({nxtr, i});

            auto it = lower_bound(st.begin(), st.end(), make_tuple((long long)a[nxtr].k[0], -1LL, -1LL));
            if (it == st.begin()) continue;
            --it;
            long long dis = i - get<2>(*it);
            if (dis < 0) dis += b.size();
            minDist = min(minDist, dis);
        }

        if (minDist >= INF) return {-1, -1};
        else if (minDist > 0) {
            sum += minDist * (n - 1);
            for (auto& r : reds) b[r.second].nxt = -1;
            for (auto& r : reds) {
                int pos = (r.second - (int)minDist + (int)b.size()) % (int)b.size();
                b[pos].nxt = r.first;
            }
        }

        pair<long long, int> best = {INF, -1};
        for (int i = 0; i < (int)b.size(); ++i) {
            if (b[i].nxt == -1) continue;
            auto& nxtA = a[b[i].nxt];
            if (b[i].k[1] < nxtA.k[0]) {
                // b[i] will lose next round, so insert the red into b
                b[i].nxt = -1;
                b.insert(b.begin() + i + 1, nxtA);
                b[i].color = (b[i].k[2] > nxtA.k[0]) ? 2 : 1;
                int nnxt = i + 2;
                if (nnxt == (int)b.size()) nnxt = 0;
                b[i+1].color = (b[i+1].k[2] > b[nnxt].k[0]) ? 2 : 1;
            }
            else if (b[i].color == 2) {
                // streak already 2, next wins gives answer
                long long ansRound = sum + nxtA.pos + 2;
                best = min(best, {ansRound, b[i].id});
            }
        }
        if (best.first != INF) return {best.second, best.first};
    }
}

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Include the solution function here or link it.

int main() {
    // Basic test: simple win in first few rounds
    {
        vector<array<int,3>> animals = {
            {5, 1, 1},
            {3, 1, 1},
            {1, 1, 1}
        };
        auto res = findWinner(animals);
        // Animal 0 wins: beats 1 (round1), beats 2 (round2), beats 1 again (round3)
        assert(res == make_pair(0, 3LL));
    }

    // Immediate loss for champion, new champion gets streak
    {
        vector<array<int,3>> animals = {
            {1, 1, 1},
            {5, 1, 1},
            {5, 1, 1}
        };
        auto res = findWinner(animals);
        // Animal 1 becomes champion at round1 (streak1), beats 2 at round2 (streak2),
        // but then beats 0 at round3? Animal1 k0=5 > animal0 k0=1 => streak3 at round3
        assert(res == make_pair(1, 3LL));
    }

    // No one can ever get 3 consecutive wins because all k0 equal
    {
        vector<array<int,3>> animals = {
            {2, 1, 1},
            {2, 1, 1},
            {2, 1, 1},
            {2, 1, 1}
        };
        auto res = findWinner(animals);
        assert(res.first == -1 && res.second == -1);
    }

    // Larger case where brute force would exceed 20000, but optimized works
    {
        int n = 10;
        vector<array<int,3>> animals(n);
        for (int i = 0; i < n; ++i) {
            animals[i] = {i+1, 1000, 0}; // k0 increasing, so first champion loses quickly
        }
        // Actually set up so that first champion (index0) has medium k0, others high
        animals[0] = {50, 1, 1};
        for (int i = 1; i < n; ++i) animals[i] = {100, 1, 1};
        // All they fight: index0 loses at round1 to index1, index1 beats index2 (round2),
        // then index1 beats index3 (round3) → answer index1 round3
        auto res = findWinner(animals);
        assert(res == make_pair(1, 3LL));
    }

    // Test with many animals and specific streak pattern
    {
        int n = 6;
        vector<array<int,3>> animals(n);
        animals[0] = {10, 1, 1};
        animals[1] = {20, 1, 1};
        animals[2] = {5, 1, 1};
        animals[3] = {30, 1, 1};
        animals[4] = {5, 1, 1};
        animals[5] = {40, 1, 1};
        // Simulate: champion0 loses to 1 (r1), 1 beats 2 (r2), 1 beats 3? 20<30 so loses,
        // new champion 3 beats 4 (r3), beats 5 (r4)? 30<40 loses, champion5 beats 0 (r5),
        // beats 1? 40>20 wins (r6), beats 2 (r7) → streak3 at round7
        auto res = findWinner(animals);
        assert(res == make_pair(5, 7LL));
    }

    // Edge: only one animal with k0 high but streak requires 3 wins, it will win all
    {
        vector<array<int,3>> animals = {
            {10, 1, 1},
            {5, 1, 1},
            {5, 1, 1},
            {5, 1, 1}
        };
        auto res = findWinner(animals);
        // Champion0 beats 1 (r1), 2 (r2), 3 (r3) → streak3 at round3
        assert(res == make_pair(0, 3LL));
    }

    return 0;
}
