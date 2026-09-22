Given a vector of axis-aligned rectangles, where each rectangle is represented by four integers `(x, y, width, height)` with `x` and `y` being the top-left corner (non-negative), and `width` and `height` being positive integers, write a C++ function that identifies all rectangles that are "fully closed hands" — meaning they do not overlap with any other rectangle in the input (including edge or corner touching counts as overlap) and their area (width * height) is strictly greater than 100. The function should return a vector of the same rectangle structures (as `std::array<int,4>`) for those that qualify, preserving the original input order. If no rectangles qualify, return an empty vector. For overlap detection, two rectangles overlap if their projections on the X-axis and Y-axis both intersect (i.e., not disjoint in either axis).
// The main algorithm is a brute-force pairwise comparison because the problem doesn't specify constraints on the number of rectangles, but a reasonable approach is O(n²) where n is the count. For each rectangle, first check if its area is > 100; if not, skip. Then compare against every other rectangle (including itself? No — we compare against index j != i). Two rectangles overlap if they intersect in both axes: for X-axis, overlap if `a.x < b.x + b.width && b.x < a.x + a.width`; similarly for Y-axis with `a.y < b.y + b.height && b.y < a.y + a.height`. If any such pair exists, the rectangle is disqualified. If none, add it to the result. Edge cases: input vector empty → return empty; duplicate rectangles (same coordinates) overlap with each other → both disqualified; single rectangle with area > 100 qualifies; rectangles that only touch at edges or corners (e.g., one ends exactly where another begins) are considered overlapping because the condition uses strict `<`, so touching does count as overlap. Time complexity is O(n²) for pairwise comparisons, and space complexity is O(n) for the result (plus O(1) auxiliary). For large n this would be inefficient, but it's a typical straightforward approach for a teaching task.
#include <vector>
#include <array>

// Represents a rectangle as {x, y, width, height} where (x,y) is top-left corner.
// Returns rectangles that have area > 100 and do not overlap with any other rectangle.
std::vector<std::array<int,4>> findFullyClosedHands(
    const std::vector<std::array<int,4>>& rectangles) {

    std::vector<std::array<int,4>> result;
    const std::size_t n = rectangles.size();

    for (std::size_t i = 0; i < n; ++i) {
        const auto& rect = rectangles[i];

        // Check area condition
        const int area = rect[2] * rect[3];
        if (area <= 100) {
            continue; // Does not meet area threshold
        }

        bool overlaps = false;
        // Compare against all other rectangles
        for (std::size_t j = 0; j < n && !overlaps; ++j) {
            if (j == i) continue;

            const auto& other = rectangles[j];

            // X-axis overlap: not disjoint
            const bool xOverlap = (rect[0] < other[0] + other[2]) &&
                                  (other[0] < rect[0] + rect[2]);
            // Y-axis overlap: not disjoint
            const bool yOverlap = (rect[1] < other[1] + other[3]) &&
                                  (other[1] < rect[1] + rect[3]);

            if (xOverlap && yOverlap) {
                overlaps = true;
            }
        }

        if (!overlaps) {
            result.push_back(rect);
        }
    }

    return result;
}
#include <cassert>
#include <vector>
#include <array>

// Include the solution function here (or in a separate header).

int main() {
    // Empty input
    {
        std::vector<std::array<int,4>> input;
        auto out = findFullyClosedHands(input);
        assert(out.empty());
    }

    // Single rectangle with area > 100
    {
        std::vector<std::array<int,4>> input = {{0,0,20,10}}; // area 200
        auto out = findFullyClosedHands(input);
        assert(out.size() == 1);
        assert(out[0] == (std::array<int,4>{0,0,20,10}));
    }

    // Single rectangle with area <= 100
    {
        std::vector<std::array<int,4>> input = {{0,0,10,10}}; // area 100
        auto out = findFullyClosedHands(input);
        assert(out.empty());
    }

    // Two non-overlapping rectangles, both large
    {
        std::vector<std::array<int,4>> input = {{0,0,20,10}, {30,30,15,15}};
        auto out = findFullyClosedHands(input);
        assert(out.size() == 2);
    }

    // Two overlapping rectangles, both large
    {
        std::vector<std::array<int,4>> input = {{0,0,20,10}, {10,5,15,15}};
        auto out = findFullyClosedHands(input);
        assert(out.empty());
    }

    // Touching edges (overlap because strict <)
    {
        std::vector<std::array<int,4>> input = {{0,0,10,10}, {10,0,10,10}}; // share vertical edge
        // Both have area 100, so area condition fails anyway. Use larger widths:
        std::vector<std::array<int,4>> input2 = {{0,0,20,10}, {20,0,20,10}};
        auto out = findFullyClosedHands(input2);
        assert(out.empty()); // touching edge counts as overlap
    }

    // One large non-overlapping, one small overlapping with large
    {
        std::vector<std::array<int,4>> input = {{0,0,20,10}, {5,5,5,5}};
        auto out = findFullyClosedHands(input);
        // large is overlapped by small (small overlaps, but small area fails), so large is disqualified
        assert(out.empty());
    }

    // Mixed: one large non-overlapping, one large overlapping with another, one small non-overlapping
    {
        std::vector<std::array<int,4>> input = {{0,0,20,10}, {10,5,30,20}, {100,100,5,5}};
        // rect2 overlaps rect1, rect1 area 200, rect2 area 600, rect3 area 25
        auto out = findFullyClosedHands(input);
        assert(out.empty()); // both large overlap
    }

    // A case where only one qualifies
    {
        std::vector<std::array<int,4>> input = {{0,0,20,10}, {100,100,20,20}, {100,100,5,5}};
        // rect1 area 200, no overlap; rect2 area 400, but overlaps with rect3 (area 25) -> disqualified; rect3 area 25 fails
        auto out = findFullyClosedHands(input);
        assert(out.size() == 1);
        assert(out[0] == (std::array<int,4>{0,0,20,10}));
    }

    return 0;
}
