/*
Given a weighted undirected graph represented by `N` nodes (numbered 1 through N), `M` edges (each with a distinct weight `P[i]`), and three special nodes `A`, `B`, and `C`, write a C++ function `long long solveProblem(ll N, ll M, ll A, ll B, ll C, const vector<ll>& P)` that computes the minimum possible total cost of a route that starts at node `A`, passes through node `B`, and ends at node `C`. The graph has no predefined edges; instead, you must choose exactly `M` distinct edge weights from the given list `P`, and you may add any edges between any pair of nodes (including multiple edges between the same pair, but each edge must have a distinct weight from the list). The cost of a path is the sum of the weights of the edges traversed. You may traverse edges any number of times, but the total cost must be minimized. The function should return this minimal total cost using 64-bit integers. If it is impossible to construct a graph and route that satisfies the constraints, return a large sentinel value (e.g., `1e15`). Note that `N >= 3`, `M >= 2`, and all edge weights are positive.
*/

#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll INF = (ll)1e15;

// Given sorted edge weights, return minimum total cost to go from A to B to C.
ll solveProblem(ll N, ll M, ll A, ll B, ll C, const vector<ll>& P) {
    // If start and end are the same as B, no edge needed.
    if (A == B && B == C) return 0;
    // Need at least two distinct edges for two segments (A->B and B->C) unless A==B or B==C.
    ll neededEdges = 0;
    if (A != B) neededEdges++;
    if (B != C) neededEdges++;
    if (M < neededEdges) return INF;

    // The cheapest strategy: use the smallest available distinct weights.
    vector<ll> sortedP = P;
    sort(sortedP.begin(), sortedP.end());
    ll total = 0;
    for (ll i = 0; i < neededEdges; ++i) {
        total += sortedP[i];
    }
    return total;
}

#include <bits/stdc++.h>
using namespace std;

// Declare the solution function (as above).
ll solveProblem(ll, ll, ll, ll, ll, const vector<ll>&);

int main() {
    // Case 1: standard distinct A,B,C, M=3
    assert(solveProblem(3, 3, 1, 2, 3, {4, 1, 2}) == 3); // smallest two: 1+2
    // Case 2: only one needed edge when A==B
    assert(solveProblem(3, 1, 1, 1, 2, {7}) == 7);
    // Case 3: zero cost when trivial
    assert(solveProblem(3, 2, 1, 1, 1, {5, 9}) == 0);
    // Case 4: impossible when not enough edges
    assert(solveProblem(3, 1, 1, 2, 3, {7}) == (ll)1e15);
    // Case 5: large values, ensure long long
    assert(solveProblem(10, 5, 1, 2, 3, {1000000000, 2000000000, 1, 2, 3}) == 3);
    // Case 6: duplicates? input guarantees distinct but we handle
    assert(solveProblem(4, 2, 1, 2, 4, {5, 5}) == 10);
    // Case 7: A==C but B different, need two edges?
    assert(solveProblem(3, 2, 1, 2, 1, {3, 4}) == 7);
    // Case 8: M very large
    vector<ll> big = {10, 20, 30, 40, 50};
    assert(solveProblem(5, 5, 1, 2, 3, big) == 30);
    // Case 9: three nodes, minimal weights
    assert(solveProblem(3, 2, 3, 2, 1, {8, 6}) == 14);
    // Case 10: all same node but M=0? not possible but handle
    assert(solveProblem(3, 0, 1, 1, 1, {}) == 0);

    return 0;
}

// The key insight is that the route from `A` to `B` to `C` can be decomposed into two segments: `A` to `B` and `B` to `C`. The minimum cost for each segment is achieved by using the cheapest possible edge weights, since we can create direct edges with those weights. For the segment `A` to `B`, the minimum cost is the smallest available edge weight (use a direct edge). For the segment `B` to `C`, similarly the minimum is the next smallest available edge weight (since we must use distinct weights). However, the route must be a single connected path, so we need to ensure that the two segments share the node `B` and that we have at least two distinct edge weights. The optimal strategy is: sort `P`, then take the two smallest distinct values (if there are duplicates, we need to handle carefully because each edge must have a distinct weight, but we can have multiple edges between the same pair with different weights). The minimal total cost is `P[0] + P[1]` if `M >= 2`; otherwise impossible. But there is a subtlety: the problem statement might require that the route is a simple path (no repeated vertices) but the problem allows repeated edges? Actually, since we can add as many edges as we want (but each edge must have a distinct weight from the list), we can always create a direct edge `A-B` with the cheapest weight and a direct edge `B-C` with the second cheapest weight. However, if `N` is exactly 3, and `A`, `B`, `C` are distinct, this works fine. If `A` and `C` are the same node? The problem says special nodes `A`, `B`, `C`, but not necessarily distinct. If `A == C`, then the route is just `A` to `B` and back to `A`, which can be done with two distinct edges (e.g., two parallel edges between `A` and `B` with different weights) so still `P[0] + P[1]`. If `A == B` or `B == C`, then the segment is zero cost but we still need at least one edge? Actually if `A == B`, the route starts at `A` (which is also `B`) and then goes to `C`, so only one segment, requiring one edge weight `P[0]`. But the input guarantees `M >= 2`? Not necessarily, but we handle general case. The main edge case: if `M < 2` and we need two segments, impossible. Also, if there are duplicate weights, sorting still gives the two smallest values, but if `P[0] == P[1]`, we cannot use both because they are not distinct? The problem says each edge must have a distinct weight from the list – but the list `P` may contain duplicates? The problem says "each with a distinct weight `P[i]`" so the list has distinct values. So `P` is already distinct, but we still sort to get the smallest two. So the answer is simply `P[0] + P[1]` if `M >= 2`; otherwise `INF`. But wait, there is a constraint that the graph has `N` nodes and we must use exactly `M` edges? The problem says "you must choose exactly `M` distinct edge weights from the given list `P`" – meaning we select all `M` weights to be used as edge weights, but we can place them arbitrarily. We still need to construct a path. The total cost only depends on the edges used in the route, not the extra edges. So the minimal total cost is just the sum of the two smallest weights. However, we must ensure that we can place these two edges so that the path exists. If `N` is huge, we can always add isolated nodes. So the answer is `P[0] + P[1]` when `M >= 2`. If `M == 1` but the path requires two edges (when `A`, `B`, `C` are all distinct), then impossible. If `M == 1` and `A == B` or `B == C`, then still need one edge, so answer is `P[0]`. But to simplify, the problem likely expects `M >= 2` always, but we handle general. The reference solution will sort `P`, and if `M < 2` return `INF`, else return `P[0] + P[1]`. Edge case: if `A == B` and `B == C`, then no edge needed, cost 0, but M is positive, but we can ignore extra edges. So the minimal cost is 0 if `A == B && B == C`. But the problem likely has distinct nodes. We'll implement robustly: if `A == B` and `B == C`, return 0. Else if `M < 2` return INF, else return `P[0] + P[1]`. Time complexity O(M log M) for sorting, space O(M) for the vector. This is a trivial problem, but the given code snippet shows a template with `solve()` reading inputs, so we adapt.
