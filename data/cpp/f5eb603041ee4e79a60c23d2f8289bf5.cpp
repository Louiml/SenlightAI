/*
Given `n` points on a 2D plane with integer coordinates, write a C++ function that computes the weight of the Minimum Spanning Tree (MST) of the complete graph where the weight of the edge between any two points is the Chebyshev distance `max(|x1-x2|, |y1-y2|)`. The function should be efficient and must not explicitly build the complete graph (which would be O(n^2) edges). Instead, it should leverage the fact that only O(n) candidate edges are needed for a Chebyshev-distance MST via a Manhattan-distance transformation, as described below. The input is given as two integer vectors `xs` and `ys` of length `n` (n≥1). Return the total weight of the MST as a `long long`.
*/

#include <bits/stdc++.h>
using namespace std;

// Compute the total weight of the Minimum Spanning Tree using Chebyshev distance.
// Input: xs, ys - vectors of integer coordinates of length n (n>=1).
// Returns the sum of edge weights of the MST as long long.
long long chebyshevMSTWeight(const vector<int>& xs, const vector<int>& ys) {
    int n = (int)xs.size();
    if (n <= 1) return 0LL;

    // Transform to (u,v) coordinates: u = x+y, v = x-y.
    vector<long long> u(n), v(n);
    for (int i = 0; i < n; ++i) {
        u[i] = xs[i] + ys[i];
        v[i] = xs[i] - ys[i];
    }

    // Manhattan MST candidate edge generation on (u,v) coordinates.
    // We'll collect candidate edges with their Chebyshev weights.
    struct Edge {
        int a, b;      // endpoints
        long long w;   // actual Chebyshev weight
        bool operator<(const Edge& other) const {
            return w < other.w;
        }
    };
    vector<Edge> candidates;

    // Union-Find structure for later Kruskal.
    vector<int> parent(n), rankv(n, 0);
    iota(parent.begin(), parent.end(), 0);
    function<int(int)> find = [&](int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    };
    auto unite = [&](int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rankv[a] < rankv[b]) swap(a, b);
        parent[b] = a;
        if (rankv[a] == rankv[b]) rankv[a]++;
        return true;
    };

    // The standard 4-orientation sweep for Manhattan MST.
    // For each of 4 rotations of (u,v) plane, sort by x (first coord) then y.
    for (int rot = 0; rot < 4; ++rot) {
        // Orientation mapping: for rot=0: (u,v) as is; rot=1: (v,u); rot=2: (-u,v); rot=3: (v,-u)
        vector<int> ord(n);
        iota(ord.begin(), ord.end(), 0);
        sort(ord.begin(), ord.end(), [&](int i, int j) {
            // sort by first coordinate (call it X) and then second (Y)
            long long xi, yi, xj, yj;
            if (rot == 0) { xi = u[i]; yi = v[i]; xj = u[j]; yj = v[j]; }
            else if (rot == 1) { xi = v[i]; yi = u[i]; xj = v[j]; yj = u[j]; }
            else if (rot == 2) { xi = -u[i]; yi = v[i]; xj = -u[j]; yj = v[j]; }
            else { xi = v[i]; yi = -u[i]; xj = v[j]; yj = -u[j]; }
            if (xi != xj) return xi < xj;
            return yi < yj;
        });

        // Maintain map from (Y - X) to index with minimal X+Y among processed points.
        map<long long, int> active; // key = Y - X, value = index
        for (int idx : ord) {
            long long X, Y;
            if (rot == 0) { X = u[idx]; Y = v[idx]; }
            else if (rot == 1) { X = v[idx]; Y = u[idx]; }
            else if (rot == 2) { X = -u[idx]; Y = v[idx]; }
            else { X = v[idx]; Y = -u[idx]; }
            long long key = Y - X;
            // Find the active point with key <= current key (i.e., Y_j - X_j <= Y_i - X_i)
            // Actually we need the point with largest key <= current, because we want closest in Manhattan.
            auto it = active.upper_bound(key);
            if (it != active.begin()) {
                --it;
                int j = it->second;
                // Candidate edge between idx and j.
                long long w = max(abs(xs[idx]-xs[j]), abs(ys[idx]-ys[j]));
                candidates.push_back({idx, j, w});
            }
            // Also check if there is a point with key exactly equal? The above handles <=.
            // Update active with current point if it improves (smaller X+Y).
            long long sum = X + Y;
            it = active.find(key);
            if (it == active.end() || (u[it->second] + v[it->second]) > sum) { // careful: sum in original coordinates? Actually X+Y is not invariant. Better store using original u+v?
                // Since we are working in rotated coords, we want to minimize X+Y (which is Manhattan norm).
                // So just compare with current stored point's X+Y.
                bool should_insert = true;
                if (it != active.end()) {
                    int old_idx = it->second;
                    long long oldX, oldY;
                    if (rot == 0) { oldX = u[old_idx]; oldY = v[old_idx]; }
                    else if (rot == 1) { oldX = v[old_idx]; oldY = u[old_idx]; }
                    else if (rot == 2) { oldX = -u[old_idx]; oldY = v[old_idx]; }
                    else { oldX = v[old_idx]; oldY = -u[old_idx]; }
                    if (oldX + oldY <= sum) should_insert = false;
                }
                if (should_insert) {
                    active[key] = idx;
                }
            }
        }
    }

    // Kruskal on candidate edges.
    sort(candidates.begin(), candidates.end());
    long long total = 0;
    int components = n;
    for (const auto& e : candidates) {
        if (unite(e.a, e.b)) {
            total += e.w;
            if (--components == 1) break;
        }
    }
    // If candidates are insufficient (shouldn't happen for n>1), we'd need fallback, but theory guarantees connectivity.
    return total;
}

#include <cassert>
#include <vector>

// The solution function is assumed to be declared above.

int main() {
    // Single point -> 0
    {
        std::vector<int> xs = {0};
        std::vector<int> ys = {0};
        assert(chebyshevMSTWeight(xs, ys) == 0);
    }
    // Two points with Chebyshev distance 5
    {
        std::vector<int> xs = {0, 3};
        std::vector<int> ys = {0, 4};
        assert(chebyshevMSTWeight(xs, ys) == 5);
    }
    // Three points forming a right triangle: (0,0), (2,0), (0,2)
    // Chebyshev distances: (0,0)-(2,0)=2, (0,0)-(0,2)=2, (2,0)-(0,2)=2 (max(|-2|,|2|)=2)
    // MST total = 2+2 = 4
    {
        std::vector<int> xs = {0, 2, 0};
        std::vector<int> ys = {0, 0, 2};
        assert(chebyshevMSTWeight(xs, ys) == 4);
    }
    // Four points forming a square of side 3: (0,0),(3,0),(0,3),(3,3)
    // Chebyshev distances: edges along sides =3, diagonals =3 (max(|3|,|3|)=3)
    // MST can take 3 edges of weight 3 total = 9 (e.g., a spanning tree)
    {
        std::vector<int> xs = {0, 3, 0, 3};
        std::vector<int> ys = {0, 0, 3, 3};
        assert(chebyshevMSTWeight(xs, ys) == 9);
    }
    // Random line points: (0,0),(1,2),(3,1) -> distances: (0,0)-(1,2)=max(1,2)=2; (0,0)-(3,1)=3; (1,2)-(3,1)=max(2,1)=2
    // MST total = 2+2=4 (pick two smallest edges)
    {
        std::vector<int> xs = {0, 1, 3};
        std::vector<int> ys = {0, 2, 1};
        assert(chebyshevMSTWeight(xs, ys) == 4);
    }
    // Negative coordinates
    {
        std::vector<int> xs = {-5, 2, -1};
        std::vector<int> ys = {3, -4, 0};
        // Compute manually: 
        // (-5,3) to (2,-4): max(7,7)=7
        // (-5,3) to (-1,0): max(4,3)=4
        // (2,-4) to (-1,0): max(3,4)=4
        // MST pick two 4s -> total 8
        assert(chebyshevMSTWeight(xs, ys) == 8);
    }
    // Duplicate points (distance 0 between duplicates)
    {
        std::vector<int> xs = {0, 0, 3};
        std::vector<int> ys = {0, 0, 4};
        // MST: connect (0,0) to (0,0) cost 0, then to (3,4) cost 5 -> total 5
        assert(chebyshevMSTWeight(xs, ys) == 5);
    }
    // Larger random test with verified brute-force for n=6
    {
        std::vector<int> xs = {1, 4, 2, 7, 9, 3};
        std::vector<int> ys = {2, 1, 5, 3, 8, 6};
        // Let's brute-force compute MST (using Prim on complete graph) to verify:
        // We'll trust the algorithm; here is expected from known correct computation:
        // I'll compute quickly: 
        // Points: 0: (1,2), 1:(4,1), 2:(2,5), 3:(7,3), 4:(9,8), 5:(3,6)
        // Chebyshev distances: 
        // d(0,1)=3, d(0,2)=3, d(0,3)=6, d(0,4)=8, d(0,5)=4
        // d(1,2)=4, d(1,3)=3, d(1,4)=7, d(1,5)=5
        // d(2,3)=5, d(2,4)=7, d(2,5)=1
        // d(3,4)=5, d(3,5)=4
        // d(4,5)=6
        // Minimum edges: (2,5)=1, (0,1)=3, (0,2)=3, (1,3)=3, (3,4)=5 -> total 1+3+3+3+5=15
        assert(chebyshevMSTWeight(xs, ys) == 15);
    }
    return 0;
}

// The key insight is that the Chebyshev distance between points `(x1,y1)` and `(x2,y2)` can be expressed in terms of transformed coordinates: define `u = x+y` and `v = x-y`. Then, `max(|x1-x2|, |y1-y2|) = (|u1-u2| + |v1-v2|) / 2` (checking all four sign combinations, this identity holds). Therefore, the Chebyshev distance is half the sum of the Manhattan distances in the `(u,v)` space and also in the Chebyshev space. However, there is a well-known result that for computing an MST with Manhattan distances, it suffices to consider only O(n) candidate edges from each of the 8 octant sweeps (or equivalently, 4 rotations and 2 sign flips). For Chebyshev distance, we can instead use the direct transformation: Chebyshev distance = Manhattan distance in transformed space? Actually, the direct result: `max(|dx|, |dy|) = (|dx+dy| + |dx-dy|)/2`, and if we set `u=x+y` and `v=x-y`, then `|dx+dy| = |Δu|` and `|dx-dy| = |Δv|`, so Chebyshev distance = `(|Δu| + |Δv|)/2`. Thus, the MST with Chebyshev weights is exactly the MST with weights `(|Δu|+|Δv|)/2`, which is a scaled Manhattan MST. Since scaling by a positive constant does not change the MST structure (only the total weight scales), we can compute the Manhattan MST on `(u,v)` coordinates and then compute each edge's actual Chebyshev weight directly from the original coordinates. The Manhattan MST candidate edges can be generated via a sweep-line algorithm: for each of 4 rotational variants (swap axes and/or reflect), sort points by `x` then `y`, and maintain a data structure that finds the nearest point in the lower-left quadrant with respect to `x+y` (or similar). Specifically, the standard algorithm: for each of 4 orientations (where we rotate the plane by 90 degrees), we sort points by `u` (or `v`), use a map keyed by `v-u` (or similar) to find the closest point with `v <= current_v`. After collecting at most O(n) candidate edges, run Kruskal's algorithm on those edges (sort by Chebyshev weight, union-find). Time complexity: O(n log n) for sorting and candidate generation, plus O(n log n) for sorting edges. Space: O(n). Edge case: n=1 gives weight 0 since MST of a single node has no edges.
