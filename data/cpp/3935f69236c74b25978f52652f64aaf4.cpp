Write a C++ function `int countPaintStrokes(int height, const std::vector<int>& wall)` that, given a wall of `height` rows (numbered from the top as height down to 1 at the bottom) and a sequence of column heights `wall[i]` representing how many rows are painted in each column from the top, returns the minimum number of vertical brush strokes needed to paint exactly the specified pattern. A single stroke can paint a contiguous horizontal segment of the wall in one pass from the top downward, and strokes can start and stop at any row. The pattern is always valid (each column height is between 1 and `height` inclusive). The function must handle empty wall vectors (return 0) and any positive integer height.
The problem is a classic "skyline painting" counting problem. The key observation is that each time the height of a column increases from the previous column, we need to add that many new strokes because those newly exposed rows cannot be covered by strokes that started in earlier columns (which ended when the previous column was shorter). If the height decreases, no new strokes are needed for those rows because strokes from earlier columns already covered the taller portion. The algorithm iterates through the wall, starting with the first column: it adds `height - wall[0]` strokes to paint the exposed top portion where no previous strokes exist. For each subsequent column `i`, if `wall[i] > wall[i-1]`, we add `wall[i] - wall[i-1]` strokes for the newly exposed rows; if it stays the same or decreases, no additional strokes are needed. Edge cases: when height equals a column's height, no strokes needed from the top for that column; when the wall is empty, return 0. Time complexity is O(n) where n is the number of columns, and space complexity is O(1) beyond the input vector.
#include <vector>

// Counts the minimum number of vertical brush strokes to paint the given wall pattern.
// height: total rows of the wall (top row = height, bottom = 1).
// wall: vector of column heights (each in [1, height]).
// Returns 0 for an empty wall.
int countPaintStrokes(int height, const std::vector<int>& wall) {
    if (wall.empty()) return 0;
    
    int strokes = height - wall[0]; // strokes to cover top exposed rows in first column
    for (size_t i = 1; i < wall.size(); ++i) {
        if (wall[i] > wall[i-1]) {
            strokes += (wall[i] - wall[i-1]);
        }
    }
    return strokes;
}
#include <cassert>
#include <vector>

int main() {
    // Test cases
    assert(countPaintStrokes(5, {5}) == 0);          // full column, no strokes needed
    assert(countPaintStrokes(5, {1}) == 4);          // only bottom row painted, 4 strokes from top
    assert(countPaintStrokes(5, {1, 2, 3}) == 4 + 1 + 1 == 6); // first 4, then +1 each
    assert(countPaintStrokes(5, {3, 1, 3}) == 2 + 0 + 2 == 4); // decrease no add, increase adds
    assert(countPaintStrokes(4, {2, 2, 2}) == 2);    // same heights, only first adds
    assert(countPaintStrokes(5, {}) == 0);           // empty wall
    assert(countPaintStrokes(10, {10, 1, 10}) == 0 + 0 + 9 == 9); 
    // Actually: first column height 10, strokes=10-10=0; second 1 <= 10, no add; third 10 > 1, add 9 => total 9.
    assert(countPaintStrokes(3, {1, 2, 1}) == 2 + 1 + 0 == 3);
    assert(countPaintStrokes(7, {7, 7, 1, 7}) == 0 + 0 + 0 + 6 == 6);
    assert(countPaintStrokes(2, {1, 2}) == 1 + 1 == 2);
    
    return 0;
}
