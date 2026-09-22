// Given two arrays of integer pairs — `rectangles` where each rectangle is defined by its width `x_i` and height `y_i` (with the bottom-left corner at (0,0) and top-right corner at (x_i, y_i)), and `points` where each point is defined by its coordinates `(x_j, y_j)` — write a C++ function that returns, for each point, the number of rectangles that contain that point. A rectangle contains a point if the point lies on or inside the rectangle, i.e., `x_j <= x_i` and `y_j <= y_i`. All coordinates are non-negative integers, and both dimensions are in the range [0, 100] inclusive. The function should be efficient for up to 10^5 rectangles and points.
The key observation is that the height coordinate is limited to 0..100. For each possible height `h` from 0 to 100, store a sorted list of all rectangle widths that have that height. Then, for a query point `(x, y)`, iterate over every height `h` from `y` to 100 (since any rectangle with height less than `y` cannot contain the point). For each such `h`, count how many rectangles with that height have width >= `x`, which can be done via binary search (`lower_bound`) on the sorted width list. Summing these counts over all relevant heights gives the answer for that point. Important edge cases: a point exactly on the boundary counts (use `<=`), rectangles with height exactly equal to `y` are included, and if `y > 100` then no rectangle can contain the point (but constraints ensure y ≤ 100). Time complexity: Preprocessing is O(R * log R) for sorting each height list. Each query takes O(101 * log R) in the worst case, which is essentially O(log R) per query with a small constant. Overall O((R + Q) * 101 * log R) but the constant 101 is small. Space complexity is O(R) for storing the lists.
#include <vector>
#include <algorithm>

// Count for each point how many rectangles contain it.
// rectangles[i] = {width, height}, points[j] = {x, y}
std::vector<int> countContainingRectangles(const std::vector<std::vector<int>>& rectangles,
                                           const std::vector<std::vector<int>>& points) {
    constexpr int MAX_HEIGHT = 101; // heights from 0 to 100 inclusive
    std::vector<std::vector<int>> widths_by_height(MAX_HEIGHT);
    
    // Group rectangle widths by their height
    for (const auto& rect : rectangles) {
        int width = rect[0];
        int height = rect[1];
        widths_by_height[height].push_back(width);
    }
    
    // Sort each width list for binary search
    for (auto& widths : widths_by_height) {
        std::sort(widths.begin(), widths.end());
    }
    
    std::vector<int> result;
    result.reserve(points.size());
    
    for (const auto& point : points) {
        int x = point[0];
        int y = point[1];
        int count = 0;
        
        // Only rectangles with height >= y can contain the point
        for (int h = y; h < MAX_HEIGHT; ++h) {
            const auto& widths = widths_by_height[h];
            // Number of widths >= x in this height bucket
            auto it = std::lower_bound(widths.begin(), widths.end(), x);
            count += static_cast<int>(widths.end() - it);
        }
        
        result.push_back(count);
    }
    
    return result;
}
#include <cassert>
#include <vector>

// The solution function is declared above.

int main() {
    // Basic test: one rectangle, one point inside
    {
        std::vector<std::vector<int>> rects = {{3, 4}};
        std::vector<std::vector<int>> pts = {{2, 3}};
        assert(countContainingRectangles(rects, pts) == std::vector<int>{1});
    }
    
    // Point on the boundary counts
    {
        std::vector<std::vector<int>> rects = {{3, 4}};
        std::vector<std::vector<int>> pts = {{3, 4}};
        assert(countContainingRectangles(rects, pts) == std::vector<int>{1});
    }
    
    // Multiple rectangles with different heights
    {
        std::vector<std::vector<int>> rects = {{5, 1}, {2, 3}, {4, 2}};
        std::vector<std::vector<int>> pts = {{3, 2}};
        // Rectangles containing (3,2): (5,1)? height 1 < 2, no. (2,3)? width 2 < 3, no. (4,2)? yes. => 1
        assert(countContainingRectangles(rects, pts) == std::vector<int>{1});
    }
    
    // Point outside all
    {
        std::vector<std::vector<int>> rects = {{5, 5}};
        std::vector<std::vector<int>> pts = {{6, 5}};
        assert(countContainingRectangles(rects, pts) == std::vector<int>{0});
    }
    
    // Multiple points
    {
        std::vector<std::vector<int>> rects = {{1, 1}, {2, 2}, {3, 3}};
        std::vector<std::vector<int>> pts = {{2, 2}, {0, 0}, {3, 3}};
        // (2,2): all three contain? (1,1) no (width), (2,2) yes, (3,3) yes => 2
        // (0,0): all three => 3
        // (3,3): only (3,3) => 1
        assert(countContainingRectangles(rects, pts) == std::vector<int>({2, 3, 1}));
    }
    
    // Zero height and zero width rectangles
    {
        std::vector<std::vector<int>> rects = {{0, 0}};
        std::vector<std::vector<int>> pts = {{0, 0}, {1, 0}};
        // (0,0) is inside, (1,0) is not (width too big)
        assert(countContainingRectangles(rects, pts) == std::vector<int>({1, 0}));
    }
    
    // Many rectangles with same height
    {
        std::vector<std::vector<int>> rects = {{1, 10}, {2, 10}, {3, 10}};
        std::vector<std::vector<int>> pts = {{2, 10}};
        // Only widths >= 2: {2,3} => 2
        assert(countContainingRectangles(rects, pts) == std::vector<int>{2});
    }
    
    // Edge case: y = 0 includes all heights
    {
        std::vector<std::vector<int>> rects = {{3, 1}, {5, 2}};
        std::vector<std::vector<int>> pts = {{4, 0}};
        // Rectangles with height>=0 and width>=4: (5,2) only => 1
        assert(countContainingRectangles(rects, pts) == std::vector<int>{1});
    }
    
    // Large coordinates within limit
    {
        std::vector<std::vector<int>> rects = {{100, 100}, {50, 50}};
        std::vector<std::vector<int>> pts = {{100, 100}, {51, 51}};
        // (100,100): both contain? (100,100) yes, (50,50) no (width<100) => 1
        // (51,51): only (100,100) => 1
        assert(countContainingRectangles(rects, pts) == std::vector<int>({1, 1}));
    }
    
    // Empty rectangles list
    {
        std::vector<std::vector<int>> rects = {};
        std::vector<std::vector<int>> pts = {{1, 1}};
        assert(countContainingRectangles(rects, pts) == std::vector<int>{0});
    }
    
    return 0;
}
