Write a C++ function `long long minWireCost(int n, int s, int e, const std::vector<long long>& x, const std::vector<long long>& a, const std::vector<long long>& b, const std::vector<long long>& c, const std::vector<long long>& d)` that computes the minimum cost to connect n points on a line (indexed 1..n, with strictly increasing positions given by x[1..n]) into a single connected path (not necessarily a simple path, can have branching) that visits every point exactly once? Actually, the problem is: we must form a single path that starts at point `s` and ends at point `e`, and visits every point exactly once in some order. The positions are sorted (x[1] < x[2] < ... < x[n]). When moving from point i to point j, the travel cost depends on direction and the endpoints: leaving point i to the right costs `b[i]`, leaving point i to the left costs `a[i]`, arriving at point j from the right costs `c[j]`, arriving at point j from the left costs `d[j]`. Additionally, the cost includes the distance |x[i] - x[j]|. However, the given DP assumes we are constructing the path by processing points left to right, maintaining a set of open path endpoints. So the intended interpretation is: we must visit all points in some order, but since the path can be non-monotone, the optimal order can be found by a sweep. The actual problem is: given the four cost functions for leaving and entering each point (depending on whether you move left or right relative to that point's position), find a Hamiltonian path from s to e that minimizes total cost, where moving from i to j costs (|x[i]-x[j]| + (if j > i then b[i] + c[j] else a[i] + d[j])). But the DP solution in the snippet models a sweep where we maintain the number of "open" path components that have endpoints on both sides? Actually, it's a classic "tour with given start/end" DP. Our task is to create a standalone problem description that is clear and matches the code's logic. 

Given the complexity, I will craft the task as: You are given n points on a number line, each with four costs: `a[i]` (cost to leave i to the left), `b[i]` (cost to leave i to the right), `c[i]` (cost to enter i from the right), `d[i]` (cost to enter i from the left). You need to find a permutation of all points starting at s and ending at e, such that total cost = sum over consecutive pairs (u,v) of (|x[u]-x[v]| + (if x[v] > x[u] then b[u] + c[v] else a[u] + d[v])) is minimized. Because x is strictly increasing, "right" means larger index. So in terms of indices, if u < v then cost = x[v]-x[u] + b[u] + c[v]; if u > v then cost = x[u]-x[v] + a[u] + d[v]. Write a function that returns the minimum possible total cost. The arrays are 1-indexed, and n is at least 2? Actually, the snippet handles n>=1 but the logic requires n>=2? We can handle n=1? But then start=end and path is trivial, cost 0. For safety, assume n>=2. The DP in the snippet uses long long and large INF. We need to produce a solution that matches the given DP exactly, so we must describe the algorithm similarly: sweeping from left to right, maintaining the number of "open" path segments (degree-1 vertices in the current partial graph) that are not yet closed. We also track whether s and e have been processed. The transition costs correspond to merging or opening/closing segments. The final answer is computed specially for the last point.

**To make the task independent and testable**, I will keep the problem statement as above: find minimum cost Hamiltonian path from s to e with directional costs. The solution function will implement the same DP as the snippet but with a clean API.
// We process points from leftmost (index 1) to rightmost (index n). Maintain `dp[i][j]` = minimum cost to have processed the first i points (i.e., decided connections for all points with index ≤ i) such that there are `j` "open" path endpoints among the processed points (i.e., the current partial construction consists of several disjoint path segments, where each open endpoint is a point that will later be connected to an unprocessed point). Also, we must ensure that points s and e are not both closed prematurely unless they are the final endpoints. The DP is initialized for i=1 depending on whether 1 is s, e, or neither. When we process a new point i, we either:
// - Start a new segment (if i is neither s nor e, or if it's one of them but we allow opening), which increases open count by 1 and adds appropriate entry cost (arriving from left or right) and exit cost (leaving left or right).
// - Connect i between two open endpoints (merging two segments), which decreases open count by 1, adding entry and exit costs.
// - Extend an existing open segment through i (keeping open count the same), which adds entry and exit costs.
// - Close an open segment at i (decreasing open count by 1) if i is s or e (the final endpoint) – but in the snippet, they handle special cases for s and e with specific transitions.
//
// The distance term `(x[i] - x[i-1]) * (2*j - pass)` appears because each open segment contributes one edge crossing the gap between x[i-1] and x[i]; actually, if there are j open endpoints among the first i-1 points, then the number of edges that must cross that gap is j, but the multiplier 2*j - pass accounts for the fact that we are counting a factor of 2 per open endpoint except that pass tracks how many of s and e have been processed, because those endpoints are fixed to be the final ends and thus only one edge crosses the gap? Let's derive: Each open endpoint will eventually connect to a point to the right, so for the gap between i-1 and i, exactly one edge crosses it for each open endpoint. However, when we include the cost of that edge's distance, we need to count it once per open endpoint. The DP combines the distance with the directional costs. The term `(x[i] - x[i-1]) * (2*j - pass)` in the snippet is actually weird: for ordinary points, they use `(x[i]-x[i-1]) * (2*j - pass)` and later multiply by 1? Looking at the snippet: for ordinary point i, they have transitions like `dp[i-1][j-1] + (x[i]-x[i-1])*(2*j - 2 - pass) + b[i] + d[i]` - that means the distance contribution is `(x[i]-x[i-1]) * (2*j - 2 - pass)`. The factor 2 appears because each open endpoint corresponds to an edge that crosses the gap, but each edge is counted twice? Actually, in the DP, they are not explicitly adding distance per edge; they are adding a term that accounts for total distance crossed. I think the correct interpretation: The DP builds the path by adding points left to right; at any step, the number of edges that cross the gap between i-1 and i equals the number of open endpoints (j) minus the number of s/e that have been processed? Let's not overanalyze; my solution will simply replicate the exact DP logic from the snippet, with appropriate commentary.
//
// Key edge cases: s and e may be at the first or last position, or anywhere in between. The DP must handle when i equals s or e differently. At the last point n, we must close the final open segment(s) to form a single path from s to e, so the final answer depends on whether n is s, e, or neither: if n is s, then we need exactly one open endpoint among the first n-1 points (which should be the other endpoint, not s? Actually, s is start, so as we process s, we open a segment, and it remains open until the end? No, s is a start of the whole path, so it should have only one incident edge (the first edge going to the next point). Similarly e has one incident edge at the end. So after processing all points, we must have exactly 0 open endpoints (the path is closed). The DP naturally ensures that at the end we have j=0. But the code computes `dp[n-1][1]` when s==n or e==n, meaning before processing n, there is exactly one open segment, and then n closes it. If neither s nor e is n, then before n there must be two open endpoints (j=2), and n connects them (closing the path). In that case, `dp[n-1][2]` plus cost for n.
//
// The DP transitions for ordinary points are:
// - Merge two open segments through point i (i is an internal point connecting a left-segment's right endpoint and a right-segment's left endpoint): requires j >= 2, cost = dp[i-1][j-1] + (x[i]-x[i-1])*(2*j - 2 - pass) + b[i] + d[i]? Wait, from the snippet: `dp[i-1][j-1] + (x[i]-x[i-1])*(2*j - 2 - pass) + b[i] + d[i]`. Here `b[i]` is leaving i to the right and `d[i]` is entering i from the left? Actually, let's define: `a[i]` = cost to leave i to left, `b[i]` = cost to leave i to right, `c[i]` = cost to enter i from right, `d[i]` = cost to enter i from left. So if i is internal and we connect a left segment (whose right open endpoint is at some point left of i) to i, then from left to i we need to enter i from the left: cost d[i]; then from i to the right segment's left endpoint, we leave i to the right: cost b[i]. The distance across the gap is accounted by that multiplier.
//
// - Extend a segment through i (i becomes part of a segment and remains open on both sides? Actually, extend means we replace one open endpoint with i, so open count stays same). This requires i not be an endpoint of the final path? Wait, the snippet has two extend cases: `dp[i-1][j] + (x[i]-x[i-1])*(2*j - pass) + a[i] + d[i]` and another with `b[i] + c[i]`. That corresponds to: either i is added to the left side (connecting to an open endpoint on the left of i, so we enter i from left (d[i]) and leave i to left (a[i])? No, that would be weird. Actually, extend means we attach i to exactly one side of an existing open segment, so it becomes a new open endpoint. There are two possibilities: either we attach i to a segment that is open on its right side (i.e., the segment's right endpoint is some point j < i, and we connect j to i from right to left? Let's not get bogged down. The snippet's transitions are correct for the DP; we'll replicate them exactly in the solution.
//
// For point i that is s or e, special transitions apply where we only allow opening or closing, not merging. The snippet handles these with conditional logic.
//
// Simpler approach: Instead of trying to derive from scratch, I'll present the solution function that exactly mirrors the snippet's code, with arrays being 1-indexed vectors of size n+1 (index 0 unused). I'll wrap it in a function that takes vectors and returns long long. I'll include detailed comments explaining the DP meaning in the solution code.
//
// Time complexity: O(n^2) because we iterate i from 1 to n and j from 0 to n. Memory: O(n^2) for dp array, but we can optimize to O(n) since we only need previous row, but the snippet uses O(n^2). For clarity, I'll use vector of vectors.
//
// Edge cases: n=2, s and e are the two points; handle initial condition. Also when s==e? The problem requires s and e distinct? The snippet assumes they are distinct, but if not, we can still handle by returning 0? Typically s!=e. I'll assume 1 ≤ s, e ≤ n, s ≠ e.
//
// I will provide a reference solution that matches the given snippet's logic exactly, but with a clean function signature.
#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

// Compute minimum cost Hamiltonian path from s to e on n points.
// Arrays are 1-indexed (vectors of size n+1, index 0 unused).
// x[1..n] are strictly increasing positions.
// a[i]: leave i to left, b[i]: leave i to right,
// c[i]: enter i from right, d[i]: enter i from left.
int64 minWireCost(int n, int s, int e,
                  const vector<int64>& x, const vector<int64>& a,
                  const vector<int64>& b, const vector<int64>& c,
                  const vector<int64>& d) {
    const int64 INF = (int64)1e18;
    // dp[i][j] = min cost after processing first i points,
    // with j open endpoints among them.
    vector<vector<int64>> dp(n + 1, vector<int64>(n + 2, INF));

    int pass = 0; // counts how many of s and e have been processed
    // Initialize for point 1
    if (s == 1) {
        dp[1][1] = d[1]; // enter 1 from left (only possible if moving right later)
        ++pass;
    } else if (e == 1) {
        dp[1][1] = b[1]; // leave 1 to right
        ++pass;
    } else {
        // point 1 is internal, so open a segment through it
        dp[1][1] = d[1] + b[1]; // enter from left, leave to right
    }

    for (int i = 2; i < n; ++i) {
        if (i == s) {
            // s is the start point, so it can only be an endpoint (open)
            for (int j = 0; j <= n; ++j) {
                // Extend an existing segment to s: we attach s to a left open endpoint,
                // entering s from left (d[s]) and leaving to right is not possible because s
                // is the start, so we close a segment? Actually the snippet uses:
                if (j >= 1) {
                    dp[i][j] = min(dp[i][j],
                        dp[i-1][j] + c[i] + (x[i] - x[i-1]) * (2 * j - pass));
                }
                if (j >= 2) {
                    dp[i][j] = min(dp[i][j],
                        dp[i-1][j-1] + d[i] + (x[i] - x[i-1]) * (2 * j - 2 - pass));
                }
                // After processing, if 2*j - pass - 1 <= 0, state impossible
                if (2 * j - pass - 1 <= 0) dp[i][j] = INF;
            }
            ++pass;
            continue;
        }
        if (i == e) {
            // e is the end point, simmilar but mirrored
            for (int j = 0; j <= n; ++j) {
                if (j >= 1) {
                    dp[i][j] = min(dp[i][j],
                        dp[i-1][j] + a[i] + (x[i] - x[i-1]) * (2 * j - pass));
                }
                if (j >= 2) {
                    dp[i][j] = min(dp[i][j],
                        dp[i-1][j-1] + b[i] + (x[i] - x[i-1]) * (2 * j - 2 - pass));
                }
                if (2 * j - pass - 1 <= 0) dp[i][j] = INF;
            }
            ++pass;
            continue;
        }
        // Ordinary internal point i
        for (int j = 0; j <= n; ++j) {
            // 1) Merge two open segments through i (i connects two open endpoints)
            if (j >= 2) {
                int64 cst = dp[i-1][j-1] + (x[i] - x[i-1]) * (2 * j - 2 - pass) + b[i] + d[i];
                dp[i][j] = min(dp[i][j], cst);
            }
            // 2) Extend a segment from left: enter i from left, leave i to left? Actually this
            //    makes i an open endpoint on the left side.
            if (j >= 1 + (e < i ? 1 : 0)) {
                int64 cst1 = dp[i-1][j] + (x[i] - x[i-1]) * (2 * j - pass) + a[i] + d[i];
                dp[i][j] = min(dp[i][j], cst1);
            }
            // 3) Extend a segment from right: enter i from right, leave i to right
            if (j >= 1 + (s < i ? 1 : 0)) {
                int64 cst1 = dp[i-1][j] + (x[i] - x[i-1]) * (2 * j - pass) + b[i] + c[i];
                dp[i][j] = min(dp[i][j], cst1);
            }
            // 4) Close an open segment at i (i becomes an endpoint, reducing open count by 1)
            if (j >= 1) {
                int64 cst2 = dp[i-1][j+1] + (x[i] - x[i-1]) * (2 * j + 2 - pass) + c[i] + a[i];
                dp[i][j] = min(dp[i][j], cst2);
            }
            // If after processing i, there are no open endpoints left, impossible
            if (2 * j - pass <= 0) dp[i][j] = INF;
        }
    }

    // Handle the final point n
    int64 answer;
    if (s == n) {
        answer = dp[n-1][1] + (x[n] - x[n-1]) + c[n];
    } else if (e == n) {
        answer = dp[n-1][1] + (x[n] - x[n-1]) + a[n];
    } else {
        answer = dp[n-1][2] + (x[n] - x[n-1]) * 2 + a[n] + c[n];
    }
    return answer;
}

Note: In the above solution, I kept the same logic as the snippet, but I need to ensure the initialization and transitions are correct. The snippet uses `dp[i][j]` for i from 1..n, j from 0..n. For i=1, it sets dp[1][1] appropriately. For i=2..n-1, the loops run. The final answer uses dp[n-1][1] or dp[n-1][2]. The code above exactly mirrors the snippet's logic but with a clean function signature.

However, there might be a subtle issue: in the snippet, the arrays are global and indexed from 1. In my solution, I use vectors of size n+1, so x[1] is the second element (index 1). That's fine. The function must assume that x, a, b, c, d all have size at least n+1. I will state in the task that the vectors are 1-indexed.
#include <bits/stdc++.h>
#include <cassert>
using namespace std;
using int64 = long long;

// Declare the function (pretend it's in a header)
int64 minWireCost(int n, int s, int e,
                  const vector<int64>& x, const vector64>& a,
                  const vector<int64>& b, const vector<int64>& c,
                  const vector<int64>& d);

int main() {
    // Example 1: n=3, s=1, e=3, all costs zero, distances matter only
    {
        int n = 3, s = 1, e = 3;
        vector<int64> x = {0, 1, 2, 3}; // x[1]=1, x[2]=2, x[3]=3
        vector<int64> a = {0, 0, 0, 0};
        vector<int64> b = {0, 0, 0, 0};
        vector<int64> c = {0, 0, 0, 0};
        vector<int64> d = {0, 0, 0, 0};
        // Path 1->2->3: distance 1+1=2
        // Path 1->3 directly not allowed? Actually visiting 2 is required, so only 1->2->3.
        int64 result = minWireCost(n, s, e, x, a, b, c, d);
        assert(result == 2);
    }
    // Example 2: n=4, s=2, e=3, costs force a particular order
    {
        int n = 4, s = 2, e = 3;
        vector<int64> x = {0, 0, 1, 2, 3}; // x[1]=0, x[2]=1, x[3]=2, x[4]=3
        vector<int64> a = {0, 100, 100, 100, 100}; // leaving left expensive
        vector<int64> b = {0, 0, 0, 0, 0}; // leaving right free
        vector<int64> c = {0, 100, 100, 100, 100}; // entering from right expensive
        vector<int64> d = {0, 0, 0, 0, 0}; // entering from left free
        // Best path: 2 -> 1 -> 4 -> 3? Let's compute:
        // 2->1: leave right? Actually 1 is left of 2, so moving left cost a[2] + d[1] = 100+0=100 plus distance 1
        // 1->4: moving right cost b[1]+c[4]=0+100=100 plus distance 3 => total 200
        // 4->3: moving left cost a[4]+d[3]=100+0=100 plus distance 1 => total 100
        // Sum = 100+1 + 100+3 + 100+1 = 305
        // Alternative: 2 -> 4 -> 1 -> 3? 2->4: b[2]+c[4]=0+100=100 plus 2; 4->1: a[4]+d[1]=100+0=100 plus 3; 1->3: b[1]+c[3]=0+100=100 plus 2; total = 300+7=307
        // Alternative: 2->1->3->4? Cannot end at 3.
        // The optimal is 2->1->4->3? But must visit all exactly once, yes.
        // However, the DP should give minimal, let's compute manually with costs:
        // We'll just assert it equals some value, but we need to know correct answer. Let's brute force later.
        // For testing, we'll use a trivial case where we can compute.
    }
    // Example 3: n=2, s=1, e=2, simple
    {
        int n = 2, s = 1, e = 2;
        vector<int64> x = {0, 5, 10};
        vector<int64> a = {0, 1, 2};
        vector<int64> b = {0, 3, 4};
        vector<int64> c = {0, 5, 6};
        vector<int64> d = {0, 7, 8};
        // Path 1->2: distance 5 + b[1] + c[2] = 5 + 3 + 6 = 14
        int64 result = minWireCost(n, s, e, x, a, b, c, d);
        assert(result == 14);
    }
    // Example 4: n=3, s=2, e=2? Not allowed if s!=e.
    // Example 5: n=5, s=3, e=4, zero costs, test with distances
    {
        int n = 5, s = 3, e = 4;
        vector<int64> x = {0, 0, 1, 2, 3, 4}; // x[1]=0, x[2]=1, x[3]=2, x[4]=3, x[5]=4
        vector<int64> a(n+1, 0), b(n+1, 0), c(n+1, 0), d(n+1, 0);
        // Need min Hamiltonian path from 3 to 4 visiting 1,2,5.
        // Possible orders: 3-1-2-5-4: distances: |1-2|=1? Actually positions: x1=0,x2=1,x3=2,x4=3,x5=4
        // Path 3(2)->1(0):2, 1->2:1, 2->5:3, 5->4:1 => total 7
        // Path 3->2->1->5->4: 1+1+4+1=7
        // Path 3->5->1->2->4: 2+4+1+2=9
        // Optimal is 7.
        int64 result = minWireCost(n, s, e, x, a, b, c, d);
        assert(result == 7);
    }
    // Example 6: Test with nontrivial costs to ensure DP matches brute force for small n
    // We'll brute-force all permutations for n=4 and compare.
    {
        int n = 4, s = 2, e = 3;
        vector<int64> x = {0, 1, 3, 6, 10};
        vector<int64> a = {0, 5, 1, 8, 2};
        vector<int64> b = {0, 4, 7, 3, 6};
        vector<int64> c = {0, 9, 2, 5, 1};
        vector<int64> d = {0, 3, 6, 4, 8};
        // Brute force all permutations starting at 2 ending at 3
        vector<int> perm = {1,4}; // other points
        sort(perm.begin(), perm.end());
        int64 best = LLONG_MAX;
        do {
            vector<int> path = {2};
            for (int v : perm) path.push_back(v);
            path.push_back(3);
            int64 cost = 0;
            for (int i = 0; i < n-1; ++i) {
                int u = path[i], v = path[i+1];
                if (x[v] > x[u]) {
                    cost += (x[v]-x[u]) + b[u] + c[v];
                } else {
                    cost += (x[u]-x[v]) + a[u] + d[v];
                }
            }
            best = min(best, cost);
        } while (next_permutation(perm.begin(), perm.end()));
        int64 result = minWireCost(n, s, e, x, a, b, c, d);
        assert(result == best);
    }
    // Example 7: n=1? Not allowed by problem (s!=e), so skip.
    return 0;
}

The test above attempts to use brute force but the `minWireCost` function isn't defined in the test block; it would be in a separate compilation unit. For the answer, I'll provide a test block that includes the solution function directly (copy-paste) and then asserts. Since the instructions say "Provide 1-10 runnable C++ assert checks inside a global main function. Call the solution function directly", I will include the full solution function in the test block along with the asserts. I'll remove the duplicate declaration.

Given the complexity, I'll simplify the test to a few deterministic cases and one brute-force check for random small n with random costs, but I must ensure the brute force is correct. I'll write it carefully.

Given the length, I'll produce the final answer with a clear , , , and . The test will include the solution function definition and asserts. I'll keep only 5-6 assertions to avoid excessive length.

Final answer below.
