// Write a C++ function named `searchPointsInRect` that takes a vector of `Point` structs (where `Point` has integer fields `x`, `y`, and `rank`), a `Rect` struct (with integer fields `lx`, `ly`, `hx`, `hy` representing the inclusive lower and upper bounds), and an integer `count`. The function must return a vector containing up to `count` points from the input that lie inside the rectangle, in non-decreasing order of their `rank` field. If there are fewer matching points than `count`, return all matching points. The function must not modify the input vector and should be `const`-correct. Points are considered inside if `rect.lx <= x <= rect.hx` and `rect.ly <= y <= rect.hy`.
#include <cassert>
#include <vector>

// Point and Rect definitions are expected from the solution.
// (Here they are repeated for completeness, but in a real test they would be included.)
struct Point {
    int x;
    int y;
    int rank;
};
struct Rect {
    int lx;
    int ly;
    int hx;
    int hy;
};

std::vector<Point> searchPointsInRect(const std::vector<Point>& points,
                                      const Rect& rect,
                                      int count);

int main() {
    // Test 1: Basic filtering and ordering.
    std::vector<Point> pts = {{0,0,3}, {1,1,1}, {5,5,2}, {2,2,4}, {10,10,0}};
    Rect rect1 = {0, 0, 5, 5};
    std::vector<Point> r1 = searchPointsInRect(pts, rect1, 10);
    assert(r1.size() == 4);
    assert(r1[0].rank == 1);
    assert(r1[1].rank == 2);
    assert(r1[2].rank == 3);
    assert(r1[3].rank == 4);

    // Test 2: Respect count limit.
    std::vector<Point> r2 = searchPointsInRect(pts, rect1, 2);
    assert(r2.size() == 2);
    assert(r2[0].rank == 1);
    assert(r2[1].rank == 2);

    // Test 3: Empty input.
    std::vector<Point> empty;
    assert(searchPointsInRect(empty, rect1, 5).empty());

    // Test 4: count zero returns empty.
    assert(searchPointsInRect(pts, rect1, 0).empty());

    // Test 5: No points inside rectangle.
    Rect rect_far = {100, 100, 200, 200};
    assert(searchPointsInRect(pts, rect_far, 10).empty());

    // Test 6: All points inside, count larger than size.
    Rect rect_all = {-100, -100, 100, 100};
    std::vector<Point> r3 = searchPointsInRect(pts, rect_all, 100);
    assert(r3.size() == 5);
    assert(r3[0].rank == 0);
    assert(r3[1].rank == 1);
    assert(r3[2].rank == 2);
    assert(r3[3].rank == 3);
    assert(r3[4].rank == 4);

    // Test 7: Boundary inclusive.
    Rect rect_boundary = {0, 0, 1, 1};
    std::vector<Point> boundary_pts = {{0,0,5}, {1,1,1}, {1,0,3}, {0,1,2}};
    std::vector<Point> r4 = searchPointsInRect(boundary_pts, rect_boundary, 10);
    assert(r4.size() == 4); // all are on boundary

    // Test 8: Duplicate ranks (order among them is unspecified but they all appear).
    std::vector<Point> dup_pts = {{0,0,1}, {1,0,1}, {2,0,1}};
    std::vector<Point> r5 = searchPointsInRect(dup_pts, rect_all, 2);
    assert(r5.size() == 2);
    // Just check all ranks are 1.
    for (const Point& p : r5) assert(p.rank == 1);

    // Test 9: Negative coordinates.
    std::vector<Point> neg_pts = {{-3,-3,2}, {-1,-1,1}, {2,2,3}};
    Rect neg_rect = {-2, -2, 0, 0};
    std::vector<Point> r6 = searchPointsInRect(neg_pts, neg_rect, 10);
    assert(r6.size() == 1);
    assert(r6[0].rank == 1); // only (-1,-1) is inside

    // Test 10: Count larger than matches but smaller than all points.
    std::vector<Point> sparse = {{0,0,10}, {100,100,1}, {0,100,5}};
    Rect small_rect = {0,0,10,10};
    std::vector<Point> r7 = searchPointsInRect(sparse, small_rect, 5);
    assert(r7.size() == 1);
    assert(r7[0].rank == 10);
    return 0;
}
#include <vector>
#include <algorithm>

struct Point {
    int x;
    int y;
    int rank;
};

struct Rect {
    int lx;
    int ly;
    int hx;
    int hy;
};

// Return up to 'count' points from 'points' that lie inside 'rect',
// ordered by ascending 'rank'.
std::vector<Point> searchPointsInRect(const std::vector<Point>& points,
                                      const Rect& rect,
                                      int count) {
    std::vector<Point> result;
    if (count <= 0 || points.empty()) {
        return result;
    }

    // Copy and sort by rank ascending.
    std::vector<Point> sorted = points;
    std::sort(sorted.begin(), sorted.end(),
              [](const Point& a, const Point& b) { return a.rank < b.rank; });

    // Collect points inside the rectangle until count reached or exhausted.
    for (const Point& p : sorted) {
        if (p.x >= rect.lx && p.x <= rect.hx &&
            p.y >= rect.ly && p.y <= rect.hy) {
            result.push_back(p);
            if (static_cast<int>(result.size()) == count) {
                break;
            }
        }
    }
    return result;
}
// The key requirements are: (1) filter points by rectangle inclusion, (2) sort the matching points by `rank` ascending, and (3) limit the output to `count` elements. The simplest approach is to copy the input vector, sort it by `rank` (stable if needed, but not required because rank is unique in typical tasks, but even if duplicates exist order among equal ranks is unspecified), then iterate through the sorted copy and collect points that pass the inside test, stopping once `count` elements have been collected or the vector is exhausted. Edge cases include: `count` <= 0 (should return an empty vector), empty input vector, rectangle that contains no points, and points with equal `rank` (we may output them in any order, but stable sort preserves input order for equal ranks if desired). Time complexity is O(n log n) for sorting + O(n) for filtering, and space complexity is O(n) for the copy plus O(count) for the output (but the copy dominates). Alternative: sort indices to avoid copying the whole structs, but copying is simpler and acceptable for this task. The function should be a free function taking `const std::vector<Point>&` and returning `std::vector<Point>`.
