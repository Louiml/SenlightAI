/*
Given a list of convex quadrilaterals (each represented by four vertices in counterclockwise order), write a C++ function `int countConnectedComponents(const std::vector<std::vector<Point>>& quads)` that returns the number of connected components when two quadrilaterals are considered connected if they touch, overlap, or share any boundary point (including a vertex or edge intersection). The input vertices are arbitrary real numbers, and quadrilaterals can be degenerate. Two quadrilaterals are connected if any vertex of one lies inside or on the boundary of the other, or if any edge of one intersects any edge of the other (including collinear overlapping edges). The function must correctly handle floating-point rounding errors using an epsilon of `1e-10`.
*/

#include <vector>
#include <algorithm>
#include <cmath>

struct Point {
    double x, y;
    Point() : x(0), y(0) {}
    Point(double x_, double y_) : x(x_), y(y_) {}
};

Point operator-(const Point& a, const Point& b) {
    return Point(a.x - b.x, a.y - b.y);
}

double dot(const Point& a, const Point& b) {
    return a.x * b.x + a.y * b.y;
}

double cross(const Point& a, const Point& b) {
    return a.x * b.y - b.x * a.y;
}

double norm(const Point& p) {
    return dot(p, p);
}

const double EPS = 1e-10;

enum {
    COUNTER_CLOCKWISE = 1,
    CLOCKWISE = -1,
    ONLINE_BACK = 2,
    ONLINE_FRONT = -2,
    ON_SEGMENT = 0
};

int ccw(const Point& p0, const Point& p1, const Point& p2) {
    Point a = p1 - p0;
    Point b = p2 - p0;
    if (cross(a, b) > EPS) return COUNTER_CLOCKWISE;
    if (cross(a, b) < -EPS) return CLOCKWISE;
    if (dot(a, b) < -EPS) return ONLINE_BACK;
    if (norm(a) < norm(b)) return ONLINE_FRONT;
    return ON_SEGMENT;
}

int contain(const std::vector<Point>& q, const Point& p) {
    bool in = false;
    for (int i = 0; i < 4; i++) {
        Point a = q[i] - p, b = q[(i + 1) % 4] - p;
        if (a.y > b.y) std::swap(a, b);
        if (a.y <= 0 && 0 < b.y && cross(a, b) < 0) in = !in;
        if (cross(a, b) == 0 && dot(a, b) <= 0) return 1;
    }
    return (in ? 2 : 0);
}

bool intersect(const Point& a, const Point& b, const Point& c, const Point& d) {
    return (ccw(a, b, c) * ccw(a, b, d) <= 0 &&
            ccw(c, d, a) * ccw(c, d, b) <= 0);
}

class DisjointSet {
public:
    explicit DisjointSet(int n) {
        par.resize(n);
        rnk.resize(n, 0);
        for (int i = 0; i < n; ++i) par[i] = i;
    }
    int find(int x) {
        if (par[x] == x) return x;
        return par[x] = find(par[x]);
    }
    void unite(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) return;
        if (rnk[x] < rnk[y]) {
            par[x] = y;
        } else {
            par[y] = x;
            if (rnk[x] == rnk[y]) rnk[x]++;
        }
    }
    bool same(int x, int y) {
        return find(x) == find(y);
    }
    int numComponents() {
        int cnt = 0;
        std::vector<bool> seen(par.size(), false);
        for (int i = 0; i < (int)par.size(); ++i) {
            int r = find(i);
            if (!seen[r]) {
                seen[r] = true;
                cnt++;
            }
        }
        return cnt;
    }
private:
    std::vector<int> par, rnk;
};

// Count connected components among convex quadrilaterals.
int countConnectedComponents(const std::vector<std::vector<Point>>& quads) {
    int M = quads.size();
    if (M == 0) return 0;
    DisjointSet dsu(M);
    for (int i = 0; i < M; ++i) {
        for (int j = i + 1; j < M; ++j) {
            bool connected = false;
            // Check if any vertex of j is inside or on boundary of i
            for (int k = 0; k < 4; ++k) {
                if (contain(quads[i], quads[j][k]) != 0) {
                    connected = true;
                    break;
                }
            }
            // Check edge intersections
            if (!connected) {
                for (int l = 0; l < 4; ++l) {
                    Point a = quads[i][l];
                    Point b = quads[i][(l + 1) % 4];
                    for (int k = 0; k < 4; ++k) {
                        Point c = quads[j][k];
                        Point d = quads[j][(k + 1) % 4];
                        if (intersect(a, b, c, d)) {
                            connected = true;
                            break;
                        }
                    }
                    if (connected) break;
                }
            }
            if (connected) dsu.unite(i, j);
        }
    }
    return dsu.numComponents();
}

#include <cassert>
#include <vector>

int countConnectedComponents(const std::vector<std::vector<Point>>& quads);

int main() {
    // Test 1: Two disjoint squares
    std::vector<std::vector<Point>> quads1 = {
        {Point(0,0), Point(1,0), Point(1,1), Point(0,1)},
        {Point(2,2), Point(3,2), Point(3,3), Point(2,3)}
    };
    assert(countConnectedComponents(quads1) == 2);

    // Test 2: Two touching at a corner
    std::vector<std::vector<Point>> quads2 = {
        {Point(0,0), Point(1,0), Point(1,1), Point(0,1)},
        {Point(1,1), Point(2,1), Point(2,2), Point(1,2)}
    };
    assert(countConnectedComponents(quads2) == 1);

    // Test 3: One inside the other
    std::vector<std::vector<Point>> quads3 = {
        {Point(0,0), Point(4,0), Point(4,4), Point(0,4)},
        {Point(1,1), Point(2,1), Point(2,2), Point(1,2)}
    };
    assert(countConnectedComponents(quads3) == 1);

    // Test 4: Three in a chain, all connected
    std::vector<std::vector<Point>> quads4 = {
        {Point(0,0), Point(1,0), Point(1,1), Point(0,1)},
        {Point(1,0), Point(2,0), Point(2,1), Point(1,1)},
        {Point(2,0), Point(3,0), Point(3,1), Point(2,1)}
    };
    assert(countConnectedComponents(quads4) == 1);

    // Test 5: One pair connected, third far away
    std::vector<std::vector<Point>> quads5 = {
        {Point(0,0), Point(1,0), Point(1,1), Point(0,1)},
        {Point(1,1), Point(2,1), Point(2,2), Point(1,2)},
        {Point(10,10), Point(11,10), Point(11,11), Point(10,11)}
    };
    assert(countConnectedComponents(quads5) == 2);

    // Test 6: Single quadrilateral
    std::vector<std::vector<Point>> quads6 = {
        {Point(0,0), Point(1,0), Point(1,1), Point(0,1)}
    };
    assert(countConnectedComponents(quads6) == 1);

    // Test 7: Degenerate (line segment) quadrilaterals touching at a point
    std::vector<std::vector<Point>> quads7 = {
        {Point(0,0), Point(2,0), Point(2,0), Point(0,0)},
        {Point(2,0), Point(4,0), Point(4,0), Point(2,0)}
    };
    assert(countConnectedComponents(quads7) == 1);

    // Test 8: Cross-shaped overlap (edges intersect interior)
    std::vector<std::vector<Point>> quads8 = {
        {Point(0,0), Point(4,0), Point(4,4), Point(0,4)},
        {Point(2,1), Point(3,1), Point(3,3), Point(2,3)}
    };
    assert(countConnectedComponents(quads8) == 1);

    // Test 9: Empty input
    std::vector<std::vector<Point>> quads9;
    assert(countConnectedComponents(quads9) == 0);

    // Test 10: Two identical quadrilaterals
    std::vector<std::vector<Point>> quads10 = {
        {Point(0,0), Point(1,0), Point(1,1), Point(0,1)},
        {Point(0,0), Point(1,0), Point(1,1), Point(0,1)}
    };
    assert(countConnectedComponents(quads10) == 1);

    return 0;
}

// The solution builds a graph where each quadrilateral is a node and an edge exists between two nodes if they are geometrically connected. To test connectivity between two quadrilaterals `A` and `B`:  
// 1. Check if any vertex of `B` is inside or on the boundary of `A` using a point-in-convex-polygon test (ray casting for interior, plus boundary checks for edge cases).  
// 2. If not, check if any edge of `A` intersects any edge of `B` using the standard segment intersection test based on the `ccw` orientation function, which also handles collinear cases by returning `ON_SEGMENT`.  
// If either condition holds, union the two nodes using a disjoint-set (union-find) data structure. After processing all pairs, count the number of distinct roots.  
// Edge cases include:  
// - Identical quadrilaterals or one fully inside the other (handled by vertex-in-polygon).  
// - Touching at a single point or along an edge (handled by intersection test that allows endpoints).  
// - Degenerate quadrilaterals (e.g., collinear vertices) where the point-in-polygon test must handle boundary cases correctly.  
// Time complexity: For `M` quadrilaterals, we do `O(M^2)` pair checks. Each vertex-in-polygon test is `O(1)` because it loops over a fixed 4 vertices, and each segment intersection is `O(1)`. So total time is `O(M^2)`, and space is `O(M)` for union-find and auxiliary arrays.
