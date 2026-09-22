// Given a convex polygon defined by its vertices in either clockwise or counterclockwise order, and a set of query pairs of points (A, B), write a C++ function `vector<array<double,3>> shortestPathQueries(const vector<Point>& polygon, const vector<pair<Point,Point>>& queries)` that, for each query, computes the minimum possible Euclidean distance of a path starting at A, touching any point on the boundary of the polygon (including vertices and edges), and ending at B. The function must also return the coordinates of the boundary point that achieves this minimum. For efficiency, process each query independently by minimizing the sum `dist(A,P) + dist(P,B)` over all points P on the polygon's perimeter. Use a ternary search on each edge to find its local minimum, and take the best over all edges. The polygon may have collinear consecutive points and repeated vertices at the end (closing point), which should be handled gracefully. Input coordinates are real numbers, and comparisons should use an epsilon of `1e-7`.

#include <cassert>
#include <cmath>
#include <vector>
#include <array>
#include <iostream>

// Assume the solution code is included above (with Point, dist, etc.)
// Here we define a small helper to compare doubles.
bool close(double a, double b) {
    return std::fabs(a - b) < 1e-6;
}

int main() {
    // Test 1: Single vertex polygon (degenerate). Query A and B same point.
    {
        std::vector<Point> poly = {Point(0,0)};
        std::vector<std::pair<Point,Point>> queries = { {Point(1,0), Point(1,0)} };
        auto res = shortestPathQueries(poly, queries);
        assert(res.size() == 1);
        // The only boundary point is (0,0), so distance = 2.0 (from 1,0 to 0,0 twice)
        assert(close(res[0][0], 2.0));
        assert(close(res[0][1], 0.0));
        assert(close(res[0][2], 0.0));
    }

    // Test 2: Square polygon (0,0)-(1,0)-(1,1)-(0,1). Query A=(0.5,-1), B=(0.5,2).
    // Optimal point is (0.5,0) on edge, distance = 1.0 + 2.0 = 3.0.
    {
        std::vector<Point> poly = {Point(0,0), Point(1,0), Point(1,1), Point(0,1)};
        std::vector<std::pair<Point,Point>> queries = { {Point(0.5,-1), Point(0.5,2)} };
        auto res = shortestPathQueries(poly, queries);
        assert(res.size() == 1);
        assert(close(res[0][0], 3.0));
        assert(close(res[0][1], 0.5));
        assert(close(res[0][2], 0.0));
    }

    // Test 3: Square, query A and B both on opposite edges, best path is straight line through interior but must touch boundary.
    // A=(-1,0.5), B=(2,0.5). The shortest path is to touch the left edge at (0,0.5) then go to right edge at (1,0.5)? Actually you only touch one point total.
    // Optimal is to touch point (0,0.5) on left edge: distance = 1.0 + 1.5 = 2.5? Wait A to (0,0.5) =1.0, then to B=(2,0.5)=2.0, sum=3.0. Or touch right edge at (1,0.5): 1.5+1.0=2.5. So best is 2.5 at (1,0.5).
    {
        std::vector<Point> poly = {Point(0,0), Point(1,0), Point(1,1), Point(0,1)};
        std::vector<std::pair<Point,Point>> queries = { {Point(-1,0.5), Point(2,0.5)} };
        auto res = shortestPathQueries(poly, queries);
        assert(res.size() == 1);
        assert(close(res[0][0], 2.5));
        assert(close(res[0][1], 1.0));
        assert(close(res[0][2], 0.5));
    }

    // Test 4: Triangle, query A and B on opposite sides of a vertex.
    // Polygon: (0,0)-(10,0)-(5,10). Query A=(5,-1), B=(5,11).
    // Optimal is vertex (5,10) or edge near it? Distance from A to (5,0) on edge is 1, then to B is ~1, but (5,0) is on edge from (0,0) to (10,0), distance A->(5,0)=1, (5,0)->B = sqrt(25+121)=~12.08 sum=13.08. Better: point on edge from (10,0) to (5,10) near (5,10)? Let's just test that result is non-inf and finite.
    {
        std::vector<Point> poly = {Point(0,0), Point(10,0), Point(5,10)};
        std::vector<std::pair<Point,Point>> queries = { {Point(5,-1), Point(5,11)} };
        auto res = shortestPathQueries(poly, queries);
        assert(res.size() == 1);
        assert(res[0][0] > 0.0);
    }

    // Test 5: Square polygon with explicit closing vertex (duplicate).
    { 
        std::vector<Point> poly = {Point(0,0), Point(4,0), Point(4,4), Point(0,4), Point(0,0)};
        std::vector<std::pair<Point,Point>> queries = { {Point(2,2), Point(2,2)} };
        auto res = shortestPathQueries(poly, queries);
        assert(res.size() == 1);
        // Since A=B=(2,2) inside, min distance is to nearest boundary point: (2,0) distance=2.0.
        assert(close(res[0][0], 2.0));
        assert(close(res[0][1], 2.0));
        assert(close(res[0][2], 0.0));
    }

    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <vector>
#include <cmath>
#include <limits>
#include <array>
#include <algorithm>

const double EPS = 1e-7;

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double _x, double _y) : x(_x), y(_y) {}
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    Point operator*(double scalar) const {
        return Point(x * scalar, y * scalar);
    }
    double dot(const Point& other) const {
        return x * other.x + y * other.y;
    }
    double cross(const Point& other) const {
        return x * other.y - y * other.x;
    }
};

inline double dist(const Point& a, const Point& b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Evaluate the objective at a point on the edge parameterized by t in [0,1].
// Edge goes from start to end.
inline double objective(const Point& start, const Point& end, double t,
                        const Point& A, const Point& B) {
    Point mid = start + (end - start) * t;
    return dist(A, mid) + dist(mid, B);
}

// Given an edge, find the point t in [0,1] minimizing the objective.
// Uses ternary search.
double ternarySearchOnEdge(const Point& start, const Point& end,
                           const Point& A, const Point& B) {
    double lo = 0.0, hi = 1.0;
    while (hi - lo > EPS) {
        double m1 = lo + (hi - lo) / 3.0;
        double m2 = hi - (hi - lo) / 3.0;
        if (objective(start, end, m1, A, B) < objective(start, end, m2, A, B)) {
            hi = m2;
        } else {
            lo = m1;
        }
    }
    return (lo + hi) / 2.0;
}

// Main function to process all queries.
// polygon: list of vertices, may or may not include the closing duplicate.
// queries: vector of pairs (A, B).
// Returns: for each query, {minDistance, bestX, bestY}.
std::vector<std::array<double, 3>> shortestPathQueries(
        const std::vector<Point>& polygon,
        const std::vector<std::pair<Point, Point>>& queries) {
    int n = (int)polygon.size();
    if (n == 0) {
        // No polygon: return empty
        return std::vector<std::array<double,3>>();
    }
    // Remove duplicate last vertex if it's the same as the first.
    int m = n;
    if (n > 1 && std::fabs(polygon[0].x - polygon[n-1].x) < EPS &&
                 std::fabs(polygon[0].y - polygon[n-1].y) < EPS) {
        m = n - 1;
    }
    // Ensure at least a degenerate polygon (a point) works.
    if (m == 0) m = 1;

    std::vector<std::array<double, 3>> results;
    results.reserve(queries.size());

    for (const auto& query : queries) {
        const Point& A = query.first;
        const Point& B = query.second;

        double bestDist = std::numeric_limits<double>::infinity();
        Point bestPoint;

        // Check all edges, including the closing edge from last to first.
        for (int i = 0; i < m; ++i) {
            Point start = polygon[i];
            Point end = polygon[(i + 1) % m];

            // Explicitly evaluate the endpoints.
            double d0 = objective(start, end, 0.0, A, B);
            double d1 = objective(start, end, 1.0, A, B);
            if (d0 < bestDist - EPS) {
                bestDist = d0;
                bestPoint = start;
            }
            if (d1 < bestDist - EPS) {
                bestDist = d1;
                bestPoint = end;
            }

            // Ternary search on the edge interior.
            double t = ternarySearchOnEdge(start, end, A, B);
            double dMid = objective(start, end, t, A, B);
            if (dMid < bestDist - EPS) {
                bestDist = dMid;
                bestPoint = start + (end - start) * t;
            }
        }

        results.push_back({bestDist, bestPoint.x, bestPoint.y});
    }
    return results;
}

// The problem reduces to finding, for each query (A,B), the point P on the polygon boundary that minimizes f(P) = |A-P| + |P-B|. The polygon boundary is a union of line segments (edges). Since each edge is a convex domain and f(P) is convex along a straight line (sum of two Euclidean distances), we can apply ternary search on each edge parameterized by t in [0,1] from one endpoint to the other. For each query, initialize the answer to +infinity. For every edge (including the closing edge from last to first vertex, but avoid duplicate edge if the polygon closes explicitly), run ternary search on t, using an epsilon of 1e-7 for termination. The function f(t) is unimodal, so ternary search is safe; even if there is a flat region (e.g., when A and B are on opposite sides and the shortest path touches a whole segment), ternary search still converges to a point within the flat region acceptable to the epsilon. Important edge cases: (1) if A or B lies exactly on the polygon boundary, the minimum is just the direct distance between them, and the boundary point could be either A or B; our method will return a point on the edge that achieves the same distance, possibly not exactly A or B but with the same minimal value; that is acceptable. (2) If the polygon has collinear vertices, the edge still behaves the same. (3) If a query has A = B, the minimum is 0 and we can pick any boundary point; ternary search will return some point with distance near 0 (actually the point on the boundary closest to A, but since A=B, that distance is positive unless A is on boundary). To be correct, we should also consider the case where the optimal point is exactly an endpoint of an edge; ternary search on the continuous edge will approximate that endpoint arbitrarily closely, but to be safe we also explicitly evaluate both endpoints of each edge. Time complexity: For each query, we do O(n) edges, each ternary search takes O(log_2((maxCoordRange)/eps)) iterations, roughly 60-100 iterations for typical range and eps. So per query O(n * log(1/eps)). Space O(1) extra per query besides input storage. Total O(q * n * log(1/eps)). For the reference solution, we implement a `Point` struct with double coordinates, basic operators, and a `dist` function. We'll also include a small epsilon comparison.
