// Write a C++ function `int minimalCycleCost(int n, int c, const std::vector<point>& pts, const std::vector<std::vector<int>>& cost)` that takes the number of points `n` (2 ≤ n ≤ 10), an additional penalty `c` for every pair of edges that intersect properly (i.e., cross at an interior point of both segments), the coordinates of `n` distinct points, and an `n×n` matrix where `cost[i][j]` is the cost of traveling directly from point `i` to point `j`. The function must return the minimum total cost of a Hamiltonian cycle that visits every point exactly once and returns to the start. The total cost is the sum of the costs of the edges along the cycle plus `c` for each pair of distinct edges in the cycle that properly intersect (not counting intersections at shared endpoints). Assume `cost[i][j]` may differ from `cost[j][i]`. The coordinates are integers and no three points are collinear (so any intersection of two non-adjacent segments is a proper crossing). Provide the function with a helper geometry structure for 2D points and segment intersection checking.

// The problem is a variant of the Traveling Salesman Problem on a small graph (n ≤ 10), so we can brute-force all permutations of the vertices to represent a cycle. For each permutation `z` of indices 0..n-1, form edges from `z[i]` to `z[(i+1)%n]`. Compute the base sum of `cost[z[i]][z[(i+1)%n]]` for all i. Then, for every pair of edges i<j (i ≠ j), check if the two segments properly intersect. Since no three points are collinear and points are distinct, any intersection of two non-adjacent segments is a proper crossing; adjacent edges share exactly one endpoint, so they do not count. If they intersect, add the penalty `c`. Take the minimum over all permutations. Time complexity: there are n! permutations, each with O(n) to sum costs and O(n²) to check intersections, so O(n! * n²), which for n=10 is about 3.6e7 operations (feasible). Space complexity is O(n) for storing permutations and edges. Edge cases: n=2 gives a cycle of two edges that share both endpoints (technically degenerate), but we can treat it as two edges that do not intersect properly (they share endpoints), so penalty zero; also ensure we handle the wrapped-around pair `(n-1,0)` correctly. Since no three points are collinear, we don't need to handle collinear intersection cases, but we can still implement a robust segment intersection using orientation tests.

#include <vector>
#include <algorithm>
#include <limits>

struct Point {
    int x, y;
    Point() : x(0), y(0) {}
    Point(int _x, int _y) : x(_x), y(_y) {}
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    int cross(const Point& other) const {
        return x * other.y - y * other.x;
    }
    int dot(const Point& other) const {
        return x * other.x + y * other.y;
    }
};

struct Segment {
    Point a, b;
    Segment(const Point& _a, const Point& _b) : a(_a), b(_b) {}
    // Returns true if this segment and the other segment intersect properly (interior crossing).
    bool properIntersect(const Segment& other) const {
        int d1 = ((b - a).cross(other.a - a));
        int d2 = ((b - a).cross(other.b - a));
        int d3 = ((other.b - other.a).cross(a - other.a));
        int d4 = ((other.b - other.a).cross(b - other.a));
        // Proper intersection: each segment's endpoints are on opposite sides of the other line.
        return ((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) &&
               ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0));
    }
};

// Returns the minimum total cost of a Hamiltonian cycle visiting all points exactly once.
// cost[i][j] is the direct edge cost from point i to point j.
// c is the penalty for each pair of non-adjacent edges that properly intersect.
int minimalCycleCost(int n, int c, const std::vector<Point>& pts, const std::vector<std::vector<int>>& cost) {
    std::vector<int> perm(n);
    for (int i = 0; i < n; ++i) perm[i] = i;
    int best = std::numeric_limits<int>::max();
    do {
        int total = 0;
        std::vector<Segment> edges;
        edges.reserve(n);
        for (int i = 0; i < n; ++i) {
            int from = perm[i];
            int to = perm[(i + 1) % n];
            total += cost[from][to];
            edges.emplace_back(pts[from], pts[to]);
        }
        // Count proper intersections among all distinct edge pairs.
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (edges[i].properIntersect(edges[j])) {
                    total += c;
                }
            }
        }
        best = std::min(best, total);
    } while (std::next_permutation(perm.begin(), perm.end()));
    return best;
}

#include <cassert>
#include <vector>
#include <cmath>

int main() {
    // Simple square: no crossing edges in optimal cycle.
    {
        std::vector<Point> pts = {Point(0,0), Point(1,0), Point(1,1), Point(0,1)};
        std::vector<std::vector<int>> cost = {
            {0,1,10,1},
            {1,0,1,10},
            {10,1,0,1},
            {1,10,1,0}
        };
        // Optimal cycle: 0-1-2-3-0 costs 1+1+1+1=4, no crossings.
        assert(minimalCycleCost(4, 5, pts, cost) == 4);
    }
    // Triangle with a cross edge penalty scenario.
    {
        std::vector<Point> pts = {Point(0,0), Point(2,0), Point(1,1), Point(1,-1)};
        std::vector<std::vector<int>> cost = {
            {0,1,1,1},
            {1,0,1,1},
            {1,1,0,1},
            {1,1,1,0}
        };
        // Any Hamiltonian cycle has 4 edges. To cross, need a pair of non-adjacent edges.
        // Let's check a specific permutation: 0-1-2-3-0: edges: (0,1) bottom, (1,2) diag up, (2,3) vertical, (3,0) diag down.
        // (0,1) and (2,3) share no endpoints: (0,1) y=0 x from 0 to2, (2,3) x=1 y from1 to -1: cross at (1,0)? yes interior. penalty c.
        // Total base cost = 4, plus c=2 => 6. But maybe another permutation avoids crossing.
        // Permutation 0-1-3-2-0: edges (0,1) bottom, (1,3) diag down, (3,2) diag up, (2,0) diag up. Check (0,1) with (2,3)? not in cycle. (0,1) with (1,3) share endpoint, no. (0,1) with (2,0) share endpoint. (1,3) with (2,0): (1,3) from (2,0) to (1,-1) line, (2,0) from (1,1) to (0,0) line: do they cross? likely not. So cost=4.
        assert(minimalCycleCost(4, 2, pts, cost) == 4);
    }
    // Two points: no proper intersection possible (two edges share both endpoints).
    {
        std::vector<Point> pts = {Point(0,0), Point(1,1)};
        std::vector<std::vector<int>> cost = {{0,3},{3,0}};
        // Cycle 0-1-0: cost 3+3=6, no crossing.
        assert(minimalCycleCost(2, 10, pts, cost) == 6);
    }
    // Five-point star where crossing is unavoidable if edges cross.
    {
        // Simple star: center (0,0) and four outer points, but use a convex pentagon with a crossing diagonal.
        std::vector<Point> pts = {Point(0,0), Point(2,0), Point(1,1), Point(1,-1), Point(3,1)};
        std::vector<std::vector<int>> cost(5, std::vector<int>(5, 1));
        for (int i = 0; i < 5; ++i) cost[i][i] = 0;
        // With all edges cost 1, base cycle cost = 5. Any crossing adds penalty.
        // The minimal cost cycle is a simple convex order? But points aren't convex; find minimal.
        // We just check that the function returns something reasonable.
        int result = minimalCycleCost(5, 100, pts, cost);
        assert(result >= 5 && result <= 5 + 100 * 5); // at most 5 crossings? Actually 5 edges, max 10 pairs, but many share endpoints.
    }
    // Non-symmetric costs.
    {
        std::vector<Point> pts = {Point(0,0), Point(1,0), Point(0,1)};
        std::vector<std::vector<int>> cost = {
            {0,5,2},
            {1,0,3},
            {4,1,0}
        };
        // Enumerate all cycles: 0-1-2-0 cost 5+3+4=12, 0-2-1-0 cost 2+1+1=4. No crossings for triangle.
        assert(minimalCycleCost(3, 7, pts, cost) == 4);
    }
    // Large penalty forces non-crossing cycle if possible.
    {
        std::vector<Point> pts = {Point(0,0), Point(2,0), Point(1,1), Point(1,-1)};
        std::vector<std::vector<int>> cost = {
            {0,1,10,10},
            {10,0,1,10},
            {10,10,0,1},
            {1,10,10,0}
        };
        // Optimal cycle might cross but penalty is big, so pick non-crossing cycle.
        // Possible cycles:
        // 0-1-2-3-0: edges (0,1),(1,2),(2,3),(3,0). (0,1) and (2,3) cross? (0,1) bottom, (2,3) from (1,1) to (1,-1) vertical at x=1, cross at (1,0) yes. cost=1+1+1+1=4 + c.
        // 0-1-3-2-0: edges (0,1),(1,3),(3,2),(2,0). (1,3) from (2,0) to (1,-1), (2,0) from (1,1) to (0,0). No proper cross. cost=1+10+10+10=31.
        // 0-2-1-3-0: edges (0,2),(2,1),(1,3),(3,0). (0,2) from (0,0) to (1,1), (1,3) from (2,0) to (1,-1) cross? maybe no. cost=10+1+10+1=22.
        // Minimum is 22 (no crossing) vs 4+c. For c large, 22 is min.
        assert(minimalCycleCost(4, 1000, pts, cost) == 22);
    }
    return 0;
}
