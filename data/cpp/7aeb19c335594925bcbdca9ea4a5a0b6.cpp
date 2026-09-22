// Write a C++ function `analyzeCityGrid` that takes a non-negative integer `n` representing the number of streets in a Euclidean city grid, and returns a `std::vector<std::pair<double,double>>` containing the following four ordered pairs (each as a pair of coordinates with double precision): (1) the point `A` closest to the origin `(0,0)` among all integer lattice points `(x,y)` with `0 ≤ x ≤ n` and `0 ≤ y ≤ n` excluding the origin itself (if n=0, the only point is the origin, so return an empty vector); (2) the point `B` farthest from the origin within the same square grid excluding the origin (for n=0 return empty vector); (3) the midpoint of the line segment connecting `A` and `B`; (4) the point `C` that forms the largest area triangle with `A` and `B` as two of its vertices, chosen from the same grid points (excluding duplicates of A and B), and if no such point exists, use `(0,0)`. All four pairs must be returned in that order. If `n=0`, return an empty vector. For ties (e.g., multiple points equally close or far), choose the point with the smallest `x` first, then smallest `y`. For the largest-area triangle, if multiple points give the same maximal area, choose the one with smallest `x`, then smallest `y`. The function must be `const`-correct and use only standard library headers.
#include <cassert>
#include <vector>
#include <utility>

// Function declaration (provided in solution)
std::vector<std::pair<double,double>> analyzeCityGrid(int n);

int main() {
    // n=0 -> empty
    assert(analyzeCityGrid(0).empty());

    // n=1: points (0,1),(1,0),(1,1)
    // closest: (0,1) and (1,0) tie, pick (0,1) due to smaller x? Actually both x=0 and x=1, so (0,1)
    // farthest: (1,1)
    // midpoint: (0.5,1.0)
    // C: points left: (1,0) only, area with A=(0,1) B=(1,1) is base=1 height=1 -> area 0.5, so C=(1,0)
    auto r1 = analyzeCityGrid(1);
    assert(r1.size() == 4);
    assert(r1[0] == std::make_pair(0.0, 1.0));
    assert(r1[1] == std::make_pair(1.0, 1.0));
    assert(r1[2] == std::make_pair(0.5, 1.0));
    assert(r1[3] == std::make_pair(1.0, 0.0));

    // n=2: points all in 0..2 except origin
    // Closest: (0,1) or (1,0) distance 1; tie-break smaller x -> (0,1)
    // Farthest: (2,2) distance sqrt(8)
    // Midpoint: (1.0,1.5)
    // For C: Points excluding A=(0,1) and B=(2,2). Max area is with (2,0) or (0,2) or (1,0) etc.
    // Compute cross for (2,0): AB=(2,1), AP=(2,-1) cross=2*(-1)-1*2=-4 -> abs 4
    // (0,2): AP=(0,1) cross=2*1-1*0=2
    // (1,0): AP=(1,-1) cross=2*(-1)-1*1=-3
    // (2,1): AP=(2,0) cross=0-2=-2
    // (1,2): AP=(1,1) cross=2-1=1
    // (0,2) gives 2, (2,0) gives 4, so C=(2,0)
    auto r2 = analyzeCityGrid(2);
    assert(r2.size() == 4);
    assert(r2[0] == std::make_pair(0.0, 1.0));
    assert(r2[1] == std::make_pair(2.0, 2.0));
    assert(r2[2] == std::make_pair(1.0, 1.5));
    assert(r2[3] == std::make_pair(2.0, 0.0));

    // n=3: closest (0,1), farthest (3,3), midpoint (1.5,2.0)
    // C: try (3,0) with A=(0,1) B=(3,3): AB=(3,2), AP=(3,-1) cross=3*(-1)-2*3=-9 -> abs 9
    // (0,3): AP=(0,2) cross=3*2-2*0=6 -> abs 6
    // (1,0): AP=(1,-1) cross=3*(-1)-2*1=-5
    // (3,1): AP=(3,0) cross=0-6=-6
    // So (3,0) gives max, but also (3,0) has x=3,y=0; no other point gives 9, so C=(3,0)
    auto r3 = analyzeCityGrid(3);
    assert(r3.size() == 4);
    assert(r3[0] == std::make_pair(0.0, 1.0));
    assert(r3[1] == std::make_pair(3.0, 3.0));
    assert(r3[2] == std::make_pair(1.5, 2.0));
    assert(r3[3] == std::make_pair(3.0, 0.0));

    // n=4: closest (0,1), farthest (4,4), midpoint (2.0,2.5)
    // C: point giving max area with A=(0,1), B=(4,4). AB=(4,3)
    // Try (4,0): AP=(4,-1) cross=4*(-1)-3*4=-16 -> abs 16
    // Try (0,4): AP=(0,3) cross=4*3-0=12
    // Try (1,0): AP=(1,-1) cross=4*(-1)-3*1=-7
    // So C=(4,0)
    auto r4 = analyzeCityGrid(4);
    assert(r4.size() == 4);
    assert(r4[0] == std::make_pair(0.0, 1.0));
    assert(r4[1] == std::make_pair(4.0, 4.0));
    assert(r4[2] == std::make_pair(2.0, 2.5));
    assert(r4[3] == std::make_pair(4.0, 0.0));

    // Additional: n=1 with only A and B? Already checked above, but test tie-break
    // For n=1, closest tie between (0,1) and (1,0), we pick (0,1) due to smaller x.
    // Farthest is (1,1). C is (1,0). Already covered.

    return 0;
}
#include <vector>
#include <utility>
#include <cmath>
#include <limits>
#include <algorithm>

// Analyze city grid of size n x n (including axes) and return:
// {closestPointToOrigin, farthestPointFromOrigin, midpointOfThose, pointForMaxTriangleArea}
// If n == 0, return empty vector.
std::vector<std::pair<double,double>> analyzeCityGrid(int n) {
    if (n == 0) return {};

    // Collect all grid points except origin
    std::vector<std::pair<int,int>> points;
    for (int x = 0; x <= n; ++x) {
        for (int y = 0; y <= n; ++y) {
            if (x == 0 && y == 0) continue;
            points.push_back({x, y});
        }
    }

    // Find closest and farthest by squared distance, tie-break by x then y
    auto compareCloser = [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
        long long da = (long long)a.first*a.first + (long long)a.second*a.second;
        long long db = (long long)b.first*b.first + (long long)b.second*b.second;
        if (da != db) return da < db;
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    };
    auto compareFarther = [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
        long long da = (long long)a.first*a.first + (long long)a.second*a.second;
        long long db = (long long)b.first*b.first + (long long)b.second*b.second;
        if (da != db) return da > db;
        if (a.first != b.first) return a.first < b.first;
        return a.second < b.second;
    };

    auto itA = std::min_element(points.begin(), points.end(), compareCloser);
    auto itB = std::min_element(points.begin(), points.end(), compareFarther);
    std::pair<int,int> A = *itA;
    std::pair<int,int> B = *itB;

    // Midpoint
    std::pair<double,double> mid = { (A.first + B.first)/2.0, (A.second + B.second)/2.0 };

    // Find point C maximizing triangle area with A and B (maximize |cross product|)
    long long bestCross = -1;
    std::pair<int,int> bestC = {0, 0};
    bool foundC = false;
    long long ABx = B.first - A.first;
    long long ABy = B.second - A.second;

    // If points list has less than 3 distinct points (after excluding A and B), use (0,0)
    // but (0,0) is not in points, so we handle separately later.
    // Check all points for candidate C
    for (const auto& P : points) {
        if (P == A || P == B) continue;
        long long cross = ABx * (P.second - A.second) - ABy * (P.first - A.first);
        long long absCross = std::abs(cross);
        if (!foundC || absCross > bestCross) {
            bestCross = absCross;
            bestC = P;
            foundC = true;
        } else if (absCross == bestCross) {
            // Tie-break: smaller x then smaller y
            if (P.first < bestC.first || (P.first == bestC.first && P.second < bestC.second)) {
                bestC = P;
            }
        }
    }

    // If no valid C found (only A and B in grid), default to (0,0)
    if (!foundC) {
        bestC = {0, 0};
    }

    return {{(double)A.first, (double)A.second},
            {(double)B.first, (double)B.second},
            mid,
            {(double)bestC.first, (double)bestC.second}};
}
// The solution requires enumerating all integer points in a square grid from `(0,0)` to `(n,n)`, excluding the origin if `n>0`. Compute squared distance from origin to avoid floating-point inaccuracies when comparing distances. Find the minimum and maximum squared distance, and among those with equal distance, select by lexicographic (`x` then `y`) order. For the area maximization, given fixed points A and B, the area of triangle formed with any point P is `0.5 * |(B.x-A.x)*(P.y-A.y) - (B.y-A.y)*(P.x-A.x)|`. We can maximize the absolute value of the cross product (ignoring the 0.5 factor) over all grid points except A and B. The maximum area will be achieved at some corner of the grid because the cross product is linear in P's coordinates, so its absolute value over a convex polygon (the square) is maximized at a vertex. However, to handle edge cases and ties, simply iterate over all points and compute the cross product, tracking the maximum and the tie-breaker. Time complexity is O(n^2) because we enumerate all points in the grid for both distance and area calculations; space complexity is O(1) auxiliary (excluding the returned vector). Edge cases: n=0 returns empty; n=1 yields only points (0,1),(1,0),(1,1) besides origin, so A and B are determined, and C must be one of the remaining (but if A and B are the only points, then C defaults to (0,0)). The midpoint is computed as `( (A.first+B.first)/2.0, (A.second+B.second)/2.0 )`.
