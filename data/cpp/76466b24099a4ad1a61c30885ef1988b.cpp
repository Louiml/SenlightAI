/*
Write a C++ function that, given a vector of axis-aligned rectangles (each represented by its lower-left and upper-right corners as integer coordinates) and a query rectangle, returns the total number of rectangles that overlap with the query rectangle in at least one point (boundary touching counts as overlap). The rectangles are axis-aligned, so overlap occurs if the intervals overlap in both x and y dimensions. The rectangles may have zero area (a point), lie completely inside the query, completely outside, or partially overlap. All coordinates are integers. The function must be efficient for a large number of rectangles (up to 10^6) and handle duplicate rectangles. Return the count as an unsigned 64-bit integer.
*/

#include <cstdint>
#include <vector>

struct Rectangle {
    int x_min;
    int y_min;
    int x_max;
    int y_max;
};

// Count how many rectangles overlap with the query rectangle.
// Overlap includes touching at boundaries. Rectangles may have zero area.
std::uint64_t count_overlapping_rectangles(
    const std::vector<Rectangle>& rectangles,
    const Rectangle& query) {
    std::uint64_t count = 0;
    for (const auto& r : rectangles) {
        // Check overlap in both dimensions (inclusive boundaries)
        if (r.x_min <= query.x_max && r.x_max >= query.x_min &&
            r.y_min <= query.y_max && r.y_max >= query.y_min) {
            ++count;
        }
    }
    return count;
}

#include <cassert>
#include <cstdint>

// (Rectangle struct and function assumed from solution)

int main() {
    // Basic overlap
    std::vector<Rectangle> rects = {{0, 0, 2, 2}, {3, 3, 4, 4}};
    Rectangle query = {1, 1, 3, 3};
    assert(count_overlapping_rectangles(rects, query) == 2); // first overlaps, second touches at (3,3)

    // No overlap
    query = {5, 5, 6, 6};
    assert(count_overlapping_rectangles(rects, query) == 0);

    // Boundary touch in x only
    query = {2, 0, 3, 2}; // touches first at x=2, y overlaps
    assert(count_overlapping_rectangles(rects, query) == 1);

    // Contained rectangle
    rects = {{1, 1, 2, 2}};
    query = {0, 0, 10, 10};
    assert(count_overlapping_rectangles(rects, query) == 1);

    // Degenerate point rectangle (zero area)
    rects = {{5, 5, 5, 5}};
    query = {4, 4, 6, 6};
    assert(count_overlapping_rectangles(rects, query) == 1);

    // Many rectangles, count correctness
    rects.clear();
    for (int i = 0; i < 100; ++i) {
        rects.push_back({i, 0, i+1, 1}); // width 1, height 1
    }
    query = {50, 0, 60, 1};
    assert(count_overlapping_rectangles(rects, query) == 11); // indices 50..60 inclusive

    // Query rectangle inside a single rectangle
    rects = {{-10, -10, 10, 10}};
    query = {0, 0, 1, 1};
    assert(count_overlapping_rectangles(rects, query) == 1);

    // Empty input
    rects.clear();
    query = {0, 0, 1, 1};
    assert(count_overlapping_rectangles(rects, query) == 0);

    // Duplicate rectangles all overlap
    rects = {{0, 0, 1, 1}, {0, 0, 1, 1}, {0, 0, 1, 1}};
    query = {0, 0, 1, 1};
    assert(count_overlapping_rectangles(rects, query) == 3);
}

// The problem reduces to counting how many rectangles in the list have a non-empty intersection with the query rectangle. For each rectangle, check overlap in the x-dimension by verifying `rect.x_min <= query.x_max && rect.x_max >= query.x_min`, and likewise for y. If both conditions hold, the rectangles overlap. The main algorithm is a simple linear scan over all input rectangles, which is optimal for an unsorted list. Edge cases include: rectangles that are degenerate lines or points (treated same as rectangles), rectangles that touch only at a boundary (the comparison uses `<=` and `>=` so boundaries count), and query rectangles that are also degenerate. Time complexity is O(n) where n is the number of rectangles, and space complexity is O(1) beyond the input. For n up to 10^6, this is acceptable. If the input were sorted or in a spatial index, we could do better, but the specification does not provide that. The function should use `std::uint64_t` for the count to avoid overflow even if all rectangles overlap.
