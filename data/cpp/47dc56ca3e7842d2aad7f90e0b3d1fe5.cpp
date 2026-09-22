// Write a C++ function `countIntersectingCircles` that takes a list of circles (each specified by integer grid coordinates and a non-negative real radius), and for each of several axis-aligned query line segments (given by two integer endpoints), returns the number of distinct circles that intersect (including touching) the segment. The segment endpoints are integer points (x1,y1) and (x2,y2). A circle intersects the segment if the circle's interior or boundary contains any point of the segment. Implement the function to process all queries efficiently without checking every circle per query. The grid coordinates and circle centers are between 1 and 500 (inclusive), and the number of circles may be up to 100,000, with up to 100,000 queries. Your function should receive a vector of circles (each as a tuple of three doubles: x,y,rᵢ) and a vector of queries (each as a tuple of four ints: x1,y1,x2,y2) and return a vector of integers, one for each query, representing the count.
#include <cassert>
#include <tuple>
#include <vector>
#include <cmath>

// The solution function is defined above.
// Test cases:

int main() {
    // Case 1: Single circle at (2,3) radius 1, query segment (1,3) to (3,3)
    {
        std::vector<Circle> circles = {{2.0, 3.0, 1.0}};
        std::vector<std::tuple<int,int,int,int>> queries = {{1,3,3,3}};
        auto res = countIntersectingCircles(circles, queries);
        assert(res.size() == 1);
        assert(res[0] == 1); // segment passes through circle
    }

    // Case 2: Two circles, one far, query segment horizontal at y=0 from x=0 to x=10
    {
        std::vector<Circle> circles = {{2.0, 0.0, 1.0}, {8.0, 5.0, 1.0}};
        std::vector<std::tuple<int,int,int,int>> queries = {{0,0,10,0}};
        auto res = countIntersectingCircles(circles, queries);
        assert(res.size() == 1);
        assert(res[0] == 1); // only the circle at (2,0) intersects
    }

    // Case 3: Circle touching segment at endpoint only
    {
        std::vector<Circle> circles = {{5.0, 5.0, 0.0}}; // zero radius
        std::vector<std::tuple<int,int,int,int>> queries = {{5,5,5,5}};
        auto res = countIntersectingCircles(circles, queries);
        assert(res[0] == 1); // point segment coincides with center, radius 0 includes point
    }

    // Case 4: Empty circles
    {
        std::vector<Circle> circles;
        std::vector<std::tuple<int,int,int,int>> queries = {{1,1,2,2}};
        auto res = countIntersectingCircles(circles, queries);
        assert(res[0] == 0);
    }

    // Case 5: Diagonal segment, circle near line
    {
        std::vector<Circle> circles = {{3.0, 3.0, 1.0}};
        std::vector<std::tuple<int,int,int,int>> queries = {{1,1,5,5}};
        auto res = countIntersectingCircles(circles, queries);
        assert(res[0] == 1); // line passes through circle
    }

    // Case 6: Multiple circles on same vertical line
    {
        std::vector<Circle> circles = {{4.0, 2.0, 0.5}, {4.0, 3.0, 0.5}};
        std::vector<std::tuple<int,int,int,int>> queries = {{4,1,4,4}};
        auto res = countIntersectingCircles(circles, queries);
        assert(res[0] == 2); // both intersect vertical segment
    }

    // Case 7: Circle not intersecting because far away
    {
        std::vector<Circle> circles = {{1.0, 1.0, 0.5}};
        std::vector<std::tuple<int,int,int,int>> queries = {{10,10,20,20}};
        auto res = countIntersectingCircles(circles, queries);
        assert(res[0] == 0);
    }

    // Case 8: Segment with both axes different, circle near but not intersecting (distance > radius)
    {
        std::vector<Circle> circles = {{3.0, 1.0, 0.1}};
        std::vector<std::tuple<int,int,int,int>> queries = {{0,0,10,0}};
        auto res = countIntersectingCircles(circles, queries);
        assert(res[0] == 0); // circle is above line, radius too small
    }

    // Case 9: Large radius circle covering multiple grid cells
    {
        std::vector<Circle> circles = {{250.0, 250.0, 200.0}};
        std::vector<std::tuple<int,int,int,int>> queries = {{1,250,500,250}};
        auto res = countIntersectingCircles(circles, queries);
        assert(res[0] == 1);
    }

    // Case 10: Two circles same point (last overwrites)
    {
        std::vector<Circle> circles = {{2.0, 2.0, 1.0}, {2.0, 2.0, 3.0}};
        std::vector<std::tuple<int,int,int,int>> queries = {{2,2,2,2}};
        auto res = countIntersectingCircles(circles, queries);
        assert(res[0] == 1); // only one circle recorded at that point
    }

    return 0;
}
#include <vector>
#include <cmath>
#include <algorithm>
#include <cassert>

// Helper struct to hold circle data
struct Circle {
    double x, y, r;
};

// Count circles intersecting each query segment.
// circles: each contains x, y (integer center) and radius r (>=0)
// queries: each contains x1, y1, x2, y2 (integer endpoints)
// Returns a vector of counts per query.
std::vector<int> countIntersectingCircles(const std::vector<Circle>& circles,
                                          const std::vector<std::tuple<int,int,int,int>>& queries) {
    const int MAX_COORD = 500;
    // Grid to store whether a circle exists at (x,y) and its radius
    // We use 1-indexed coordinates to match original, but can use 0-indexed array with offset.
    // We'll use 1-indexed arrays of size MAX_COORD+3 to be safe.
    std::vector<std::vector<double>> radius(MAX_COORD+3, std::vector<double>(MAX_COORD+3, 0.0));
    std::vector<std::vector<bool>> present(MAX_COORD+3, std::vector<bool>(MAX_COORD+3, false));

    // Populate grid
    for (const auto& c : circles) {
        int xi = (int)std::llround(c.x);
        int yi = (int)std::llround(c.y);
        // Ensure within bounds (task says within 1..500, but we guard)
        if (xi < 1) xi = 1;
        if (xi > MAX_COORD) xi = MAX_COORD;
        if (yi < 1) yi = 1;
        if (yi > MAX_COORD) yi = MAX_COORD;
        radius[xi][yi] = c.r;
        present[xi][yi] = true; // overwrite as per snippet
    }

    const double eps = 1e-7;

    // Helper lambda to test if a circle (at integer center) intersects segment p1->p2
    auto intersects = [&](int cx, int cy, double tr, 
                          double x1, double y1, double x2, double y2) -> bool {
        // Check endpoints inside circle
        double dx1 = cx - x1, dy1 = cy - y1;
        if (dx1*dx1 + dy1*dy1 < tr*tr) return true;
        double dx2 = cx - x2, dy2 = cy - y2;
        if (dx2*dx2 + dy2*dy2 < tr*tr) return true;

        // Vector along segment
        double vx = x2 - x1, vy = y2 - y1;
        double len2 = vx*vx + vy*vy;
        if (len2 <= eps) return false; // point segment, already checked endpoints

        // Check projection: dot product of (center - p1) with (p2-p1) must be between 0 and len2
        double dx = cx - x1, dy = cy - y1;
        double dot1 = dx*vx + dy*vy;
        if (dot1 < eps) return false; // center is before p1
        double dx2b = cx - x2, dy2b = cy - y2;
        double dot2 = dx2b*(-vx) + dy2b*(-vy);
        if (dot2 < eps) return false; // center is after p2

        // Perpendicular distance from center to line through p1,p2
        double cross = std::fabs(dx*vy - dy*vx);
        double dist = cross / std::sqrt(len2);
        return dist < tr;
    };

    std::vector<int> result;
    result.reserve(queries.size());

    for (const auto& q : queries) {
        int x1, y1, x2, y2;
        std::tie(x1, y1, x2, y2) = q;

        int ans = 0;
        if (x1 == x2) {
            // Vertical segment
            if (y1 > y2) std::swap(y1, y2);
            for (int yy = y1; yy <= y2; ++yy) {
                if (present[x1][yy]) ans++;
            }
        } else if (y1 == y2) {
            // Horizontal segment
            if (x1 > x2) std::swap(x1, x2);
            for (int xx = x1; xx <= x2; ++xx) {
                if (present[xx][y1]) ans++;
            }
        } else {
            // General line, iterate along dominant axis
            if (std::abs(x2 - x1) > std::abs(y2 - y1)) {
                // Ensure x1 < x2
                if (x1 > x2) {
                    std::swap(x1, x2);
                    std::swap(y1, y2);
                }
                double slope = (double)(y2 - y1) / (double)(x2 - x1);
                double cury = y1;
                for (int xx = x1; xx <= x2; ++xx) {
                    int my = (int)std::floor(cury);
                    for (int yy = my - 2; yy <= my + 2; ++yy) {
                        if (yy < 1 || yy > MAX_COORD) continue;
                        if (!present[xx][yy]) continue;
                        if (intersects(xx, yy, radius[xx][yy],
                                       (double)x1, (double)y1, (double)x2, (double)y2)) {
                            ans++;
                        }
                    }
                    cury += slope;
                }
            } else {
                // Dominant axis is y, ensure y1 < y2
                if (y1 > y2) {
                    std::swap(x1, x2);
                    std::swap(y1, y2);
                }
                double slope = (double)(x2 - x1) / (double)(y2 - y1);
                double curx = x1;
                for (int yy = y1; yy <= y2; ++yy) {
                    int mx = (int)std::floor(curx);
                    for (int xx = mx - 2; xx <= mx + 2; ++xx) {
                        if (xx < 1 || xx > MAX_COORD) continue;
                        if (!present[xx][yy]) continue;
                        if (intersects(xx, yy, radius[xx][yy],
                                       (double)x1, (double)y1, (double)x2, (double)y2)) {
                            ans++;
                        }
                    }
                    curx += slope;
                }
            }
        }
        result.push_back(ans);
    }
    return result;
}
// The solution uses a grid hashing approach because coordinates are bounded (1..500) and circle centers are on integer grid points. We store for each grid cell (x,y) whether a circle exists there and its radius (since coordinates are integer, multiple circles at the same point are not distinguished? Actually careful: the original snippet uses arrays indexed by x,y and overwrites radius if multiple circles at same point? In original, `r[x][y] = rr[i]` overwrites, so duplicates are not counted multiple times. I'll follow that: each grid cell may have at most one circle recorded; if multiple provided, the last one wins for radius, and the cell is marked present). For each query segment, we iterate over grid cells that are near the line segment. Since the segment endpoints are integer and within grid, we walk along the dominant axis (the axis with larger absolute difference). For each step along that axis, we compute the approximate perpendicular coordinate (using double interpolation) and check the 5x5 neighborhood around that cell (i.e., from −2 to +2 in the other axis) to find any circles. For each such circle, we test if the circle intersects the segment using a mathematical test: if either endpoint is inside the circle, or the perpendicular distance from the circle's center to the infinite line is less than radius and the projection of the center onto the line lies between the endpoints (within epsilon tolerance). This avoids checking all circles per query. Time complexity: For each query, the number of iterations is the length of the dominant axis (≤500) and for each iteration we check at most 5 cells, each with an O(1) intersection test. So per query O(dominant length) = O(500) in worst case. For 100k queries, that's about 50 million operations, acceptable. Space complexity: O(grid size) = O(500^2) for the grid arrays. Edge cases: vertical and horizontal segments handled separately with direct loops over grid cells on that row or column. Segments with zero length (same points) handled as well (the loop will check that single point). Sorting endpoints so that we iterate increasing along the dominant axis. Use epsilon to avoid floating point precision issues when projecting.
