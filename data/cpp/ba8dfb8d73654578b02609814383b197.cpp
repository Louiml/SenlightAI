Write a standalone C++ function `countPointsOnRectBoundary` that, given a list of \(n\) distinct 2D integer points and a list of \(K\) axis-aligned rectangles (each defined by its bottom-left corner \((x_1, y_1)\) and top-right corner \((x_2, y_2)\) with \(x_1 < x_2\) and \(y_1 < y_2\)), returns for each rectangle the number of points that lie exactly on its perimeter boundary (including its corners, edges, but not interior). The function should take inputs as constant references to vectors of pairs and rectangles (where a rectangle is a pair of coordinate pairs), and return a vector of integers with the counts in the same order as the input rectangles. All coordinates are positive `long` integers, and \(n, K \le 300000\). Points are guaranteed to be distinct. The solution must handle up to \(10^5\) rectangles efficiently.
// The key is to count points that lie on the four sides of each rectangle without scanning all points for every rectangle. For each rectangle, the perimeter consists of:  
// - Left vertical side: all points with \(x = x_1\) and \(y\) in \([y_1, y_2]\).  
// - Right vertical side: all points with \(x = x_2\) and \(y\) in \([y_1, y_2]\).  
// - Bottom horizontal side: all points with \(y = y_1\) and \(x\) in \((x_1, x_2)\) (exclude corners already counted by vertical sides).  
// - Top horizontal side: all points with \(y = y_2\) and \(x\) in \((x_1, x_2)\).  
//
// We preprocess the points by grouping their \(y\)-coordinates sorted for each distinct \(x\), and their \(x\)-coordinates sorted for each distinct \(y\). Use `unordered_map<long, vector<long>>` for both. For each rectangle, we binary search (`lower_bound`/`upper_bound`) in the appropriate vectors:  
// - For left and right sides, count in `mx[x]` the values in \([y_1, y_2]\).  
// - For bottom and top, count in `my[y]` the values in \([x_1+1, x_2-1]` to avoid double-counting corners.  
//
// Since points are distinct, corners are counted exactly once (by vertical sides). Total time is \(O(n \log n + K \log n)\) for preprocessing and each query. Space is \(O(n)\) for the maps plus vectors.
#include <vector>
#include <unordered_map>
#include <algorithm>

using coord = long;
using point = std::pair<coord, coord>;
using rectangle = std::pair<point, point>;

// Counts points lying exactly on the perimeter boundary of each rectangle.
std::vector<long> countPointsOnRectBoundary(
    const std::vector<point>& points,
    const std::vector<rectangle>& rectangles
) {
    // Group y-coordinates by x, and x-coordinates by y.
    std::unordered_map<coord, std::vector<coord>> byX;
    std::unordered_map<coord, std::vector<coord>> byY;
    for (const auto& p : points) {
        byX[p.first].push_back(p.second);
        byY[p.second].push_back(p.first);
    }
    for (auto& kv : byX) std::sort(kv.second.begin(), kv.second.end());
    for (auto& kv : byY) std::sort(kv.second.begin(), kv.second.end());

    auto countRange = [](const std::vector<coord>& v, coord left, coord right) -> long {
        if (left > right) return 0;
        return std::upper_bound(v.begin(), v.end(), right) -
               std::lower_bound(v.begin(), v.end(), left);
    };

    std::vector<long> result;
    result.reserve(rectangles.size());
    for (const auto& rec : rectangles) {
        coord x1 = rec.first.first;
        coord y1 = rec.first.second;
        coord x2 = rec.second.first;
        coord y2 = rec.second.second;

        long cnt = 0;
        // Vertical sides (including corners).
        auto itX1 = byX.find(x1);
        if (itX1 != byX.end()) cnt += countRange(itX1->second, y1, y2);
        auto itX2 = byX.find(x2);
        if (itX2 != byX.end()) cnt += countRange(itX2->second, y1, y2);

        // Horizontal sides, excluding corners (x strictly between x1 and x2).
        auto itY1 = byY.find(y1);
        if (itY1 != byY.end()) cnt += countRange(itY1->second, x1 + 1, x2 - 1);
        auto itY2 = byY.find(y2);
        if (itY2 != byY.end()) cnt += countRange(itY2->second, x1 + 1, x2 - 1);

        result.push_back(cnt);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <iostream>

using coord = long;
using point = std::pair<coord, coord>;
using rectangle = std::pair<point, point>;

// Include the solution function here (or link it).
// For demonstration, we insert the function definition above.

int main() {
    // Example 1: Simple square with points on edges and corners.
    std::vector<point> pts1 = {{0,0}, {0,1}, {0,2}, {1,0}, {1,1}, {1,2}, {2,0}, {2,1}, {2,2}};
    std::vector<rectangle> recs1 = {{{0,0},{2,2}}};
    auto res1 = countPointsOnRectBoundary(pts1, recs1);
    assert(res1.size() == 1);
    assert(res1[0] == 8); // all except interior point (1,1)

    // Example 2: Multiple rectangles, points only on horizontal sides.
    std::vector<point> pts2 = {{1,5}, {2,5}, {3,5}, {1,1}, {2,1}, {3,1}};
    std::vector<rectangle> recs2 = {{{1,1},{3,5}}, {{0,0},{4,6}}};
    auto res2 = countPointsOnRectBoundary(pts2, recs2);
    assert(res2.size() == 2);
    assert(res2[0] == 6); // all six points lie on the boundary
    assert(res2[1] == 6);

    // Example 3: Empty boundary (no points on rectangle).
    std::vector<point> pts3 = {{10,10}, {20,20}};
    std::vector<rectangle> recs3 = {{{0,0},{5,5}}};
    auto res3 = countPointsOnRectBoundary(pts3, recs3);
    assert(res3.size() == 1);
    assert(res3[0] == 0);

    // Example 4: Points on corners only.
    std::vector<point> pts4 = {{2,2}, {5,2}, {2,7}, {5,7}};
    std::vector<rectangle> recs4 = {{{2,2},{5,7}}};
    auto res4 = countPointsOnRectBoundary(pts4, recs4);
    assert(res4[0] == 4);

    // Example 5: Narrow rectangle with many points on vertical sides.
    std::vector<point> pts5;
    for (long i = 0; i <= 10; ++i) pts5.push_back({3,i});
    for (long i = 0; i <= 10; ++i) pts5.push_back({7,i});
    std::vector<rectangle> recs5 = {{{3,0},{7,10}}};
    auto res5 = countPointsOnRectBoundary(pts5, recs5);
    assert(res5[0] == 22); // all points on vertical sides, including corners

    std::cout << "All tests passed.\n";
    return 0;
}
