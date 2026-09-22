// Write a C++ function `int minimumAmountOfTime(const std::vector<int>& d, int n, int g, int r)` that determines the minimum total time needed to travel from position `0` to position `n` along a straight road, given `m` volunteer stations at sorted distinct positions `d[0] < d[1] < ... < d[m-1]` (all strictly between 0 and n). You start at position `0` with full "energy" `g`. Moving 1 unit costs 1 unit of energy and 1 time unit. You may stop at any station (including ones you’ve passed before). At a station you may "refill" your energy to full `g` — this refill takes `r` time units, but you may refill at most once per visit to that station, and only if your current energy is exactly 0 (you must arrive with zero energy to be allowed to refill; otherwise you cannot stop for refill). After refilling, you continue with energy `g`. You may also choose to stop at a station without refilling (that takes no time). You may pass through stations without stopping. You cannot travel beyond position `n`. If it is impossible to reach `n`, return -1. Otherwise return the minimum total time (moving + refilling) required. Constraints: `2 ≤ m ≤ 10000`, `1 ≤ g ≤ 1000`, `0 ≤ r ≤ 10^9`, `0 < d[0] < d[1] < ... < d[m-1] < n ≤ 10^9`. The stations are not at position 0 (the start) or at n (the goal); you start at position 0 and you finish exactly at n; you do not need to stop at n.

This problem is modeled in the original snippet as a graph on the m stations plus implicit constraints on energy. The key insight is that the state (station index, current energy) is too large, so we compress using modular arithmetic. Since energy always resets to g after a refill, and you can only refill when energy is exactly 0, the energy at any point is determined by the distance traveled since the last refill modulo (g+r) in time. In the original solution, it builds two sets of reachability bitsets: `pre` for energy residues reachable before visiting a station, and `nxt` for residues that can reach the final goal without further refills. It then computes, for each pair of stations i and j (i<j), whether it is possible to travel from i to j in a single "leg" that starts with full energy at i (possibly after a refill) and ends at j with zero energy (or exactly at the goal) without any intermediate refills. This leg is possible if the distance d[j]-d[i] ≤ g and there exists a residue k such that k + (d[j]-d[i]) ≡ 0 mod g and the relevant bitset conditions hold. The bitsets `pre` and `nxt` are built using dynamic programming over bitsets: `pre[i]` represents which residues mod g are reachable at station i from start without any refill in between (using only shifts by distances), and `nxt[i]` represents residues at station i that can reach the goal. Then an edge is added between i and j if the bitset intersection is non-empty. After constructing this graph (with edges representing a single leg that ends with zero energy at the destination, requiring a refill there unless it is the final goal), a BFS from station 0 (which is not explicitly in `d` but treated as index 0 with position 0) computes the minimum number of legs needed to reach each station. The total time is `(number_of_legs - 1) * (r + g) + distance_of_final_leg` if the station can directly reach the goal with remaining energy. Edge cases: if the goal cannot be reached in one leg from any reachable station, the BFS explores multiple legs; if no path exists, return -1. Complexity: building bitsets is O(m * g / 64) per station because bitset operations are word-level, giving O(m^2 / 64 * g) worst-case in the bitset intersection checks, but with pruning by distance ≤ g. The BFS is O(m + edges) where edges are at most O(m * g) in worst case, but typically smaller due to distance constraint. Space is O(m * g) bits for the bitsets, which is about 10^7 bits ≈ 1.25 MB, plus graph adjacency bitsets of size O(m^2) bits ≈ 100 million bits ≈ 12.5 MB.

#include <vector>
#include <algorithm>
#include <bitset>
#include <queue>
#include <limits>

int minimumAmountOfTime(const std::vector<int>& d, int n, int g, int r) {
    const int m = static_cast<int>(d.size());
    std::vector<std::bitset<1001>> pre(m), nxt(m);
    std::vector<std::bitset<2001>> e(m);

    // nxt[i] : residues at station i that can reach the goal without refill
    nxt[m-1][g] = 1;
    for (int i = m-2; i >= 0; --i) {
        int dif = d[i+1] - d[i];
        for (int j = 2*dif; j <= g; j += 2*dif) {
            nxt[i] |= (nxt[i+1] >> j);
        }
        nxt[i][g] = 1;
    }

    // pre[i] : residues reachable at station i from start without refill
    std::bitset<1001> mask;
    for (int i = 0; i <= g; ++i) mask[i] = 1;
    std::vector<int> used_idx(m+1, -1);
    int tt = 0;
    pre[0][0] = 1;

    for (int i = 0; i < m; ++i) {
        std::bitset<1001> now;
        if (i == 0) now[0] = 1;
        else {
            // Build pre[i] from pre[i-1] by shifting by the distance from i-1 to i
            int dif = d[i] - d[i-1];
            now.reset();
            now[0] = 1;
            for (int j = 2*dif; j <= g; j += 2*dif) {
                now |= (pre[i-1] << j);
            }
        }
        pre[i] = now;

        tt++;
        // Try to add edges from i to later stations j
        for (int j = i+1; j < m; ++j) {
            if (d[j] - d[i] > g) break;
            int dif_step = d[j] - d[j-1];
            if (used_idx[dif_step] != tt) {
                used_idx[dif_step] = tt;
                // For each residue k in now, add k + (d[j]-d[i]) as reachable
                for (int k = 0; k + (d[j]-d[i]) <= g; ++k) {
                    if (now[k]) now[k + (d[j]-d[i])] = 1;
                }
            }
            // Check if there exists a residue in now that can also reach goal from j
            std::bitset<1001> combined = (now & (nxt[j] >> (d[j]-d[i]))) & mask;
            if (combined.any()) {
                e[i][j - i + 1000] = 1;
                e[j][i - j + 1000] = 1;
            }
        }

        // Prepare pre for next i (not needed outside loop as we compute directly)
    }

    // BFS on stations using graph e
    std::vector<int> dist(m, std::numeric_limits<int>::max());
    std::queue<int> q;
    // Start at station 0? But station 0 is not the start; start is position 0, which is before d[0].
    // We need to treat start as a virtual node. The original code sets pre[0][0]=1 and expands from there.
    // In BFS, we start with station 0 as the first reachable node, because we can reach d[0] with energy d[0] without refill.
    // Actually the original BFS starts with u[0]=1 and bfs[0]=0, treating index 0 as the first station.
    // But we must ensure d[0] <= g, otherwise start cannot even reach the first station. The original doesn't handle that case; we add a check.
    if (d[0] > g) return -1;

    dist[0] = 0;
    q.push(0);
    while (!q.empty()) {
        int x = q.front(); q.pop();
        // Forward edges
        for (int j = 1; j < m - x; ++j) {
            if (d[x+j] - d[x] > g) break;
            if (e[x][j + 1000] && dist[x+j] > dist[x] + 1) {
                dist[x+j] = dist[x] + 1;
                q.push(x+j);
            }
        }
        // Backward edges
        for (int j = 1; j <= x; ++j) {
            if (d[x] - d[x-j] > g) break;
            if (e[x][1000 - j] && dist[x-j] > dist[x] + 1) {
                dist[x-j] = dist[x] + 1;
                q.push(x-j);
            }
        }
    }

    int ans = std::numeric_limits<int>::max();
    for (int i = 0; i < m; ++i) {
        if (dist[i] == std::numeric_limits<int>::max()) continue;
        if (d[i] + g >= n) {
            long long total = 1LL * dist[i] * (r + g) + (n - d[i]);
            if (total < ans) ans = static_cast<int>(total);
        }
    }

    if (ans == std::numeric_limits<int>::max()) return -1;
    return ans;
}

#include <cassert>
#include <vector>

// The solution function is defined elsewhere (include the header or paste above).
// For testing, we include the solution code here mentally.

int main() {
    // Basic case: direct travel from start to finish without any station needed
    std::vector<int> d1 = {100};
    assert(minimumAmountOfTime(d1, 200, 200, 0) == 200);

    // Need one refill at the only station
    std::vector<int> d2 = {150};
    // Start at 0, travel 150 to station, but energy=150>g=100? Actually g=100, so cannot reach station 150.
    // Adjust test: g=200, station at 150, n=400. Direct reach station with energy 200, but cannot reach n=400 because 150+200=350 < 400.
    // Need to refill at station, but arrive with energy 0? Actually you arrive with energy 50 (200-150=50), not zero, so you cannot refill.
    // So it's impossible. Let's design a valid refill scenario: g=100, station at 100, n=250.
    // Start with 100, reach station with 0 energy, refill takes r=50, then travel to n=250, need 150 > g=100, so still cannot finish. Need another station.
    // Simpler: g=100, stations at 100 and 200, n=250. From start to 100 (energy 0), refill (r=10), to 200 (energy 0), refill (r=10), then to 250 (energy 50) but cannot finish because need 50 <= g, yes finish. Total time = 100 + 10 + 100 + 10 + 50 = 270.
    std::vector<int> d3 = {100, 200};
    assert(minimumAmountOfTime(d3, 250, 100, 10) == 270);

    // Impossible case: gap too large
    std::vector<int> d4 = {90, 190};
    // g=100, from start to 90 (energy 10), cannot refill because energy not zero, cannot reach 190 because need 100 more, but only have 10+? Actually from 90 to 190 is 100, but energy at 90 is 10, so cannot make it. Also cannot go back. So impossible.
    assert(minimumAmountOfTime(d4, 300, 100, 5) == -1);

    // Multiple stations, best path may skip some
    std::vector<int> d5 = {50, 150, 250};
    // g=100, r=0. Start to 50 (energy 50), cannot refill, then to 150 (distance 100, energy 0), refill costs 0, then to 250 (distance 100, energy 0), refill, then to n=280 (distance 30) finish. Total time = 50+100+100+30 = 280.
    assert(minimumAmountOfTime(d5, 280, 100, 0) == 280);

    // Backward movement allowed? The original code allows edges in both directions. Test a path that benefits from going backward.
    // For simplicity, not tested here as it's complex; but we can test a symmetric case with two stations.
    // g=10, stations at {5, 8}, n=15. Start to 5 (energy 5), cannot refill, to 8 (distance 3, energy 2), cannot refill, to 15 (distance 7 > 2) impossible. No backward helps.
    // Another: g=6, stations {4, 7}, n=10. Start to 4 (energy 2), cannot refill, to 7 (distance 3, energy 0? actually 2-3=-1 impossible). Not reachable.
    // We'll not include backward test to keep simple.

    // Large r value
    std::vector<int> d6 = {10};
    // g=20, r=1000, n=40. Start to 10 (energy 10), cannot refill, to 40 (distance 30 > 10) impossible.
    assert(minimumAmountOfTime(d6, 40, 20, 1000) == -1);

    // Edge case: station exactly at distance g
    std::vector<int> d7 = {100};
    // g=100, r=1, n=200. Reach station with 0 energy, refill, then to n=200 distance 100, finish. Total = 100 + 1 + 100 = 201.
    assert(minimumAmountOfTime(d7, 200, 100, 1) == 201);

    // Edge case: n is exactly d[i] + g for some station
    std::vector<int> d8 = {150};
    // g=50, n=200. Start to 150 distance 150 > g=50, impossible.
    assert(minimumAmountOfTime(d8, 200, 50, 0) == -1);

    // Multiple refills needed
    std::vector<int> d9 = {100, 200, 300};
    // g=100, r=2, n=350. Start to 100 (energy 0) refill, to 200 (energy 0) refill, to 300 (energy 0) refill, to 350 (energy 50) finish. Total = 100+2+100+2+100+2+50 = 356.
    assert(minimumAmountOfTime(d9, 350, 100, 2) == 356);

    // No station needed if g >= n
    std::vector<int> d10 = {10};
    // g=500, n=20, station at 10 but direct is fine. Start to 20 directly (no need to stop) since g>=n. The function returns 20.
    assert(minimumAmountOfTime(d10, 20, 500, 0) == 20);

    return 0;
}
