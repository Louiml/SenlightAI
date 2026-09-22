// Write a C++ function `double minimumTravelPath(int N, int M, int L, const std::vector<Star>& stars)` that takes a number of stars `N`, two indices `M` and `L` (1-based), and a list of `Star` structures, where each `Star` contains a center point (x, y), an angle `a` in degrees, and a radius `r`. Each star is a regular convex pentagon (5 vertices) defined by rotating 5 points around the center: for vertex `j` (0..4), the angle is `a + j*144 + 90` degrees, and the point is `center + r * (cos(theta), sin(theta))` in radians. The function must compute the shortest travel distance from star `M` to star `L` (1-based indexing) where travel can go from any point of one pentagon to any point of another pentagon, and the distance between two stars is defined as the Euclidean distance between the two closest points on their respective pentagon boundaries. You may assume the function `dist(const Polygon&, const Polygon&)` is pre‑written to compute this closest‑boundary distance (you can implement it or simulate it for the test case). The travel path is through intermediate stars, so we need the minimum over all paths using the Floyd–Warshall algorithm. Return the total minimum distance as a `double`. Input stars are given as a vector of `Star` where `Star` = { double x, y, aDeg, r; }. Use `long double` internally for accuracy, but return `double`. Clamp the answer to a relative error of 1e-9.
We first convert each star's definition (center, angle, radius) into a polygon of 5 vertices. For each vertex, compute the angle in degrees as `a + j*144 + 90`, convert to radians, then the vertex is `(cx + r*cos(theta), cy + r*sin(theta))`. Build a `Polygon` for each star. Then compute the pair‑wise distance between all polygons using the provided `dist` function (which returns the minimum Euclidean distance between two convex polygons – typical implementation uses rotating calipers or sampling; we assume it is exact or approximate). Fill the adjacency matrix `dis[i][j]` where `i,j` are 0‑based indices. Then run Floyd–Warshall: `dis[i][j] = min(dis[i][j], dis[i][k] + dis[k][j])` for all k. The answer is `dis[M-1][L-1]`. Edge cases: when `M == L`, the distance is 0; when the polygons are identical (all vertices overlap), distance is 0. Also handle N up to, say, 100, so Floyd–Warshall is O(N^3) which is fine. The polygon `dist` computation is O(1) per pair if the polygons are small (5 vertices each), so total O(N^2) for building distances. Complexity: O(N^3) time, O(N^2) space.
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

struct Point {
    long double x, y;
    Point(long double x_ = 0, long double y_ = 0) : x(x_), y(y_) {}
    Point operator+(const Point& o) const { return Point(x+o.x, y+o.y); }
    Point operator*(long double s) const { return Point(x*s, y*s); }
};

struct Star {
    long double cx, cy, aDeg, r;
};

using Polygon = std::vector<Point>;

// Distance between two convex polygons (5 vertices each) – implemented with simple point‑to‑segment for each pair
long double pointToSegment(const Point& p, const Point& a, const Point& b) {
    long double dx = b.x - a.x, dy = b.y - a.y;
    long double len2 = dx*dx + dy*dy;
    if (len2 == 0) {
        long double dx2 = p.x - a.x, dy2 = p.y - a.y;
        return std::sqrt(dx2*dx2 + dy2*dy2);
    }
    long double t = ((p.x - a.x)*dx + (p.y - a.y)*dy) / len2;
    t = std::max((long double)0, std::min((long double)1, t));
    long double rx = a.x + t*dx - p.x;
    long double ry = a.y + t*dy - p.y;
    return std::sqrt(rx*rx + ry*ry);
}

long double dist(const Polygon& P, const Polygon& Q) {
    long double best = std::numeric_limits<long double>::infinity();
    for (const auto& p : P) {
        for (size_t i = 0; i < Q.size(); ++i) {
            best = std::min(best, pointToSegment(p, Q[i], Q[(i+1)%Q.size()]));
        }
    }
    for (const auto& q : Q) {
        for (size_t i = 0; i < P.size(); ++i) {
            best = std::min(best, pointToSegment(q, P[i], P[(i+1)%P.size()]));
        }
    }
    return best;
}

Polygon makePentagon(const Star& s) {
    const long double pi = std::acos((long double)-1);
    Polygon vertices;
    for (int j = 0; j < 5; ++j) {
        long double deg = s.aDeg + j * 144 + 90;
        long double rad = deg * pi / 180.0L;
        long double x = s.cx + s.r * std::cos(rad);
        long double y = s.cy + s.r * std::sin(rad);
        vertices.push_back(Point(x, y));
    }
    return vertices;
}

double minimumTravelPath(int N, int M, int L, const std::vector<Star>& stars) {
    // Validate input indices (1-based)
    if (M < 1 || L < 1 || M > N || L > N) return -1.0;

    // Build polygons for all stars
    std::vector<Polygon> polys(N);
    for (int i = 0; i < N; ++i) polys[i] = makePentagon(stars[i]);

    // Initialize distance matrix
    std::vector<std::vector<long double>> dis(N, std::vector<long double>(N, std::numeric_limits<long double>::infinity()));
    for (int i = 0; i < N; ++i) dis[i][i] = 0;

    // Compute pair distances
    for (int i = 0; i < N; ++i) {
        for (int j = i+1; j < N; ++j) {
            long double d = dist(polys[i], polys[j]);
            dis[i][j] = dis[j][i] = d;
        }
    }

    // Floyd–Warshall
    for (int k = 0; k < N; ++k) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (dis[i][k] + dis[k][j] < dis[i][j]) {
                    dis[i][j] = dis[i][k] + dis[k][j];
                }
            }
        }
    }

    return static_cast<double>(dis[M-1][L-1]);
}
#include <cassert>
#include <cmath>
#include <vector>

// Assume the above solution definitions are included here (Point, Star, Polygon, dist, makePentagon, minimumTravelPath)

int main() {
    // Test 1: N=2, M=1, L=2, two overlapping stars (same center, same angle, same radius) -> distance 0
    {
        std::vector<Star> stars = { {0,0,0,5}, {0,0,0,5} };
        assert(std::fabs(minimumTravelPath(2,1,2,stars) - 0.0) < 1e-9);
    }

    // Test 2: N=2, M=1, L=2, two stars far apart, center distance 10, radius 1 -> min distance = 10 - 2 = 8
    {
        std::vector<Star> stars = { {0,0,0,1}, {10,0,0,1} };
        // Actually the pentagon vertices are rotated; the closest points may be slightly > 8? We'll approximate expected ~8.0 for large radius? Check: pentagon with r=1, center distance 10 => min distance = 10 - 1 - 1 = 8 if they are circles, but pentagon boundary is inside circle, so min distance is > 8? Actually pentagon is inscribed in circle of radius r, so farthest from center is r=1, closest is r*cos(36°)=0.809, so min distance between shapes is >= 10 - 2*1 = 8 but maybe a bit larger due to pentagon shape. We'll just check it is >8 and <9.
        double d = minimumTravelPath(2,1,2,stars);
        assert(d > 8.0 && d < 9.0);
    }

    // Test 3: N=1, M=1, L=1 -> distance 0
    {
        std::vector<Star> stars = { {3,4,30,2} };
        assert(minimumTravelPath(1,1,1,stars) == 0.0);
    }

    // Test 4: N=3, path via middle star is shorter than direct? We'll just test that the result is non-negative and finite
    {
        std::vector<Star> stars = { {0,0,0,2}, {5,0,0,2}, {10,0,0,2} };
        double d13 = minimumTravelPath(3,1,3,stars);
        assert(d13 > 0.0 && d13 <= 20.0);
        // Direct distance should be >= distance via star 2? Actually direct might be shorter. We just check it's reasonable.
        assert(std::isfinite(d13));
    }

    // Test 5: N=4, all stars same position -> any pair distance 0
    {
        std::vector<Star> stars = { {1,2,45,3}, {1,2,45,3}, {1,2,45,3}, {1,2,45,3} };
        assert(minimumTravelPath(4,1,4,stars) == 0.0);
    }

    // Test 6: N=2, M=2, L=1 should be symmetric
    {
        std::vector<Star> stars = { {0,0,0,1}, {3,0,0,1} };
        double d12 = minimumTravelPath(2,1,2,stars);
        double d21 = minimumTravelPath(2,2,1,stars);
        assert(std::fabs(d12 - d21) < 1e-12);
    }

    // Test 7: N=5, random small numbers, just check output is finite and non-negative
    {
        std::vector<Star> stars = { {0,0,0,1}, {1,1,90,2}, {2,0,180,1}, {1,-1,270,3}, {-1,0,45,2} };
        double ans = minimumTravelPath(5,2,4,stars);
        assert(ans >= 0.0 && std::isfinite(ans));
    }

    return 0;
}
