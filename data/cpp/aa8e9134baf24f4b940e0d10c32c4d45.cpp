/*
Write a C++ function `countWaysToFerry` that takes the number of people `n`, the boat capacity `k` (in units of weight, where each person weighs either 50 or 100 units), and a vector of integers `weights` (each either 50 or 100), and returns a `std::pair<int,int>` where the first element is the minimum number of trips needed to transport all people from the left bank to the right bank, and the second element is the number of distinct ways (modulo 1e9+7) to achieve that minimum number of trips. The boat must always carry at least one person, never exceed the capacity `k`, and can travel back and forth between banks. Starting with all people on the left bank, the goal is to have all people on the right bank. Two ways are considered distinct if, at any trip, the set of people (i.e., the weights composition) chosen differs. The number of trips counts the total boat crossings (including the final one to the right bank). If the state is unreachable, return `{-1, 0}`.
*/

#include <vector>
#include <queue>
#include <utility>
#include <cstdint>

// Count minimum trips and number of ways (mod 1e9+7) to ferry all people from left to right.
// Weights are normalized: each person is weight 1 or 2, boat capacity k is in normalized units.
std::pair<int,int> countWaysToFerry(int n, int k, const std::vector<int>& weights) {
    const int MOD = 1000000007;
    // Normalize weights and capacity
    int capacity = k / 50; // each 50 becomes 1, 100 becomes 2
    int unu_total = 0, doi_total = 0;
    for (int w : weights) {
        if (w == 50) ++unu_total;
        else ++doi_total; // assuming 100, but could also handle other values? but spec says 50 or 100
    }

    // Precompute binomial coefficients up to n
    int maxN = n + 1;
    std::vector<std::vector<int64_t>> comb(maxN + 1, std::vector<int64_t>(maxN + 1, 0));
    for (int i = 0; i <= maxN; ++i) {
        comb[i][0] = 1;
        for (int j = 1; j <= i; ++j) {
            comb[i][j] = (comb[i-1][j-1] + comb[i-1][j]) % MOD;
        }
    }

    // State: (unu_left, doi_left, boat_side) where boat_side=0 left, 1 right
    // distances and ways arrays, initialized to -1 and 0
    std::vector<std::vector<std::vector<int>>> dist(n+1, std::vector<std::vector<int>>(n+1, std::vector<int>(2, -1)));
    std::vector<std::vector<std::vector<int64_t>>> ways(n+1, std::vector<std::vector<int64_t>>(n+1, std::vector<int64_t>(2, 0)));

    // BFS queue of states
    struct State { int unu, doi; bool boat; };
    std::queue<State> q;
    dist[unu_total][doi_total][0] = 0;
    ways[unu_total][doi_total][0] = 1;
    q.push({unu_total, doi_total, false});

    while (!q.empty()) {
        State cur = q.front(); q.pop();
        int d = dist[cur.unu][cur.doi][cur.boat];
        int64_t w = ways[cur.unu][cur.doi][cur.boat];

        // Determine available counts on the side where boat is
        int unu_avail = (cur.boat == false) ? cur.unu : (unu_total - cur.unu);
        int doi_avail = (cur.boat == false) ? cur.doi : (doi_total - cur.doi);

        // Enumerate choices to move
        for (int i = 0; i <= unu_avail; ++i) {
            for (int j = 0; j <= doi_avail && i + 2*j <= capacity; ++j) {
                if (i + j == 0) continue;
                // Compute new left counts
                int new_unu, new_doi;
                if (cur.boat == false) {
                    new_unu = cur.unu - i;
                    new_doi = cur.doi - j;
                } else {
                    new_unu = cur.unu + i;
                    new_doi = cur.doi + j;
                }
                bool new_boat = !cur.boat;
                int64_t add = (w * comb[unu_avail][i]) % MOD;
                add = (add * comb[doi_avail][j]) % MOD;

                if (dist[new_unu][new_doi][new_boat] == -1) {
                    dist[new_unu][new_doi][new_boat] = d + 1;
                    ways[new_unu][new_doi][new_boat] = add;
                    q.push({new_unu, new_doi, new_boat});
                } else if (dist[new_unu][new_doi][new_boat] == d + 1) {
                    ways[new_unu][new_doi][new_boat] = (ways[new_unu][new_doi][new_boat] + add) % MOD;
                }
            }
        }
    }

    if (dist[0][0][1] == -1) return {-1, 0};
    return {dist[0][0][1], static_cast<int>(ways[0][0][1])};
}

#include <cassert>
#include <vector>

// Function prototype (from solution)
std::pair<int,int> countWaysToFerry(int n, int k, const std::vector<int>& weights);

int main() {
    // Test 1: Simple case, one person weight 50, capacity 50 => one trip
    auto res1 = countWaysToFerry(1, 50, {50});
    assert(res1.first == 1);
    assert(res1.second == 1);

    // Test 2: Two people weight 100 each, capacity 100 => two trips? Actually one trip with both? but capacity 100 can carry one 100, so need two trips (go right, come back, go right) = 3 trips? Let's see: 2 people of type2, capacity 2. Start both on left. Trip1: take one type2 to right. Boat right. Need to return? But to bring the second, boat must come back left, so trip2: empty? No, must carry at least one, so come back with the same person (or empty not allowed). So minimal is 3 trips? Actually stay on right? Goal is all on right, boat final right. Start: left has 2, right 0. Trip1: take one to right (left 1, right 1). Boat right. Trip2: need boat left, so bring that person back (left 2, right 0). Trip3: take both? Wait both are type2, capacity 2, cannot take two (4>2). So take one, left 1 right 1. Trip4: come back with that? That would never finish. Actually the correct minimal: take both? cannot. So unreachable? No, possible: trip1 take one to right (left 1, right 1), trip2 boat left with that person (left 2, right 0), trip3 take the other one (left 1, right 1), trip4 boat returns with the other? That doesn't work. Actually the standard puzzle: with capacity 2 and two heavy people, you can take one across, bring it back, take the other across, bring it back? That loops. The correct is: you need to take one across, then bring it back, but then you have the same. Actually, if you have two type2, and capacity 2, you can take one across (left 1, right 1), then boat must return, but you can bring the same back (left 2, right 0) – that's useless. So it's impossible? No, because you can leave the person on right and boat returns empty? But boat must carry at least one person, so you cannot return empty. Thus indeed unreachable. Our code returns -1. Test that.
    auto res2 = countWaysToFerry(2, 100, {100, 100});
    assert(res2.first == -1);
    assert(res2.second == 0);

    // Test 3: Two people weight 50 each, capacity 50 => take both in one trip? capacity 50, each type1, sum 2, but capacity is 1 (normalized), so cannot take both, each trip carries one. So: trip1 take A to right (left B, right A), trip2 boat left (must carry A back? Or can't come empty, so must bring A back, then left A,B, right empty) that leads to 2 trips but state repeats. Actually minimal is 3 trips: take A, return with A, take B? That still leaves A on right? Wait: trip1: A right. Boat right. trip2: return with A (left A,B, right empty). trip3: take A again? That cycles. The correct is: actually with capacity 1 and two light people, you can take one, then bring the same back, then take the other – that never ends. So unreachable? But the classic puzzle with two light people and capacity 2 (i.e., can carry both) works. Here capacity is 50, so only one type1 fits. So you can't move two type1 separately because boat must return, and you have to bring someone back. So unreachable. Test: res3 = countWaysToFerry(2, 50, {50,50}) should be -1.

    // Test 4: One type1 and one type2, capacity 2 (normalized) => total weight 3 > capacity? Actually capacity 2, type1+type2 = 3 > 2, so cannot carry both together. But you can take type2 alone (weight 2) to right, then return? Must bring someone back, so bring type2 back (or type1? type1 alone weight1 also possible). Let's think: start (1,1). Trip1: take type2 (weight2) to right -> left (1,0), right (0,1). Boat right. Trip2: return with type2 (must bring him back) -> left (1,1), right (0,0). Back to start. Or Trip2: return with type1? But type1 is on left, can't. So only way is bring type2 back, cycle. So unreachable. So -1.

    // Test 5: Two type1, capacity 2 (normalized) => can carry both together. Initial (2,0). Trip1: take both to right -> (0,0) boat right. Done in 1 trip. So result 1 trip, 1 way (since only one way to choose i=2,j=0). 
    auto res5 = countWaysToFerry(2, 100, {50, 50});
    assert(res5.first == 1);
    assert(res5.second == 1);

    // Test 6: Three type1, capacity 2. Start (3,0). Trip1: take two -> (1,0) right (0,2) boat right. Trip2: must return with at least one, bring one back -> (2,0) left, boat left. Trip3: take two again -> (0,0) right? Wait after trip2 left has 2, right 1. Trip3 take two -> left 0, right 3. Done in 3 trips. Let's compute ways: For each trip choices. Minimal distance 3. Ways: Trip1: choose 2 of 3 type1 -> C(3,2)=3. Trip2: must return 1 of the 2 on right? Actually right has 2 after trip1, return 1 -> C(2,1)=2. Trip3: from left (2), take both -> C(2,2)=1. Total ways = 3*2*1=6. So result should be {3,6}. 
    auto res6 = countWaysToFerry(3, 100, {50, 50, 50});
    assert(res6.first == 3);
    assert(res6.second == 6);

    // Test 7: One type1 and one type2, capacity 3 (i.e., k=150). Start (1,1). Trip1: take both (weight 3) -> (0,0) right. Done in 1 trip, 1 way.
    auto res7 = countWaysToFerry(2, 150, {50, 100});
    assert(res7.first == 1);
    assert(res7.second == 1);

    // Test 8: Unreachable because capacity too small (k=50, one type1 and one type2) as above.
    auto res8 = countWaysToFerry(2, 50, {50, 100});
    assert(res8.first == -1);

    // Test 9: A classic: 3 type1, 1 type2, capacity 2 (k=100). Start (3,1). Need to move all. Let's trust BFS, but we can test a known case: The snippet's example? Not given. We'll just assert reachable and minimal>0.
    auto res9 = countWaysToFerry(4, 100, {50,50,50,100});
    assert(res9.first > 0);

    // Test 10: Large n modulo, but check that modulo is applied. For n=50 all type1, capacity 50 (k=2500) -> can take all in one trip, ways=1.
    std::vector<int> big(50, 50);
    auto res10 = countWaysToFerry(50, 2500, big);
    assert(res10.first == 1);
    assert(res10.second == 1);

    return 0;
}

// The problem is a classic BFS on the state space of the problem. Each state is defined by the number of people of weight 1 (50 units) and weight 2 (100 units) on the left bank, plus a boolean indicating whether the boat is currently on the left (0) or right (1) bank. Since the boat capacity is given in weight units, we normalize `k` and all weights by dividing by 50, so each person becomes weight 1 or 2, and capacity becomes `k/50`. This keeps the state space small: dims are `n+1` by `n+1` for each bank side, but we can reduce to just the left-bank counts (since total numbers are fixed), so the state space is `O(n^2 * 2)`. BFS from the initial state (all on left, boat left) explores all reachable states, storing both the minimum distance (number of trips) and the number of ways to reach that state with that minimal distance. The transition: for each state, choose `i` people of type 1 and `j` of type 2 to move from the current bank (where they reside) to the other, with `i+2*j <= k` and `i+j >= 1`. The number of ways to choose is the product of binomial coefficients `C(unu_st,i) * C(doi_st,j)` (where the indices are counts on the side where the boat currently is). When updating a neighbor, if it is unvisited, set its distance to current+1 and ways to the product; if already visited with the same distance, add the product to its ways. Important edge cases: the capacity `k` may be less than the minimum person weight (so unreachable), the boat must move at least one person, and the target state is all people on the right, boat on the right (left counts 0,0). The answer distance is `dist[0][0][1] - 1`? Actually in the snippet they store `dist[unu][doi][0] = 1` as the initial "distance", but that 1 represents that we are at step 0? They use `dist` as number of moves+1 and output `dist[0][0][1]-1`. We'll instead store true number of trips, initial=0. The BFS ensures minimal trips because it processes in increasing distance. Complexity: states `O(n^2)`, transitions: for each state, loops over `i` from 0 to `unu`, `j` from 0 to `doi` limited by capacity, so worst-case `O(n^2)` transitions per state, total `O(n^4)`. With `n ≤ 50` this is fine. Space `O(n^2)`.
