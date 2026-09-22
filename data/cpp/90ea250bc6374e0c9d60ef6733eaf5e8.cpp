Write a C++ function named `boxQueryStatic` that simulates the static-part behavior of the original `CObjectSpace::BoxQuery` function. Given a vector of axis-aligned bounding boxes (each represented by a center and half-size, i.e., a `Box` struct with `Fvector`-like `center` and `half_size` of type `double`), a query box (also center + half-size), and a `flags` parameter where bit `0x1` enables coarse mode (skipping full triangle tests) and bit `0x2` enables only-first-hit (stopping after the first intersection), the function must return the indices of all boxes that intersect the query box (or just the first index if only-first is set), respecting the coarse mode: in coarse mode, treat a box as intersecting even if it merely touches the boundary (inclusive), while in full-test mode require strict overlap (the interiors must intersect; touching faces not counted). The function should be `const`-correct and take the boxes by `const std::vector<Box>&`, the query box by `const Box&`, and `unsigned flags`. Assume no empty input; ignore any dynamic-object logic. Implement the function as a free function in a header-only style.
#include <cassert>
#include <vector>

// Box struct and boxQueryStatic declaration are assumed to be included above.

int main() {
    // Sample boxes
    std::vector<Box> boxes = {
        {{0,0,0},{1,1,1}},       // box 0: centered at origin, half-size 1
        {{3,3,3},{1,1,1}},       // box 1: far away
        {{0,0,0},{1,1,1}},       // box 2: identical to box 0
        {{2,0,0},{1,1,1}},       // box 3: touches box 0 on x=1 face (diff=2, sum=2)
        {{2.5,0,0},{1,1,1}}      // box 4: overlaps box 0 (diff=2.5, sum=2, strict fails)
    };

    Box query = {{0,0,0},{1,1,1}}; // same as boxes 0 and 2

    // Full mode (no flags): strict overlap
    auto r = boxQueryStatic(boxes, query, 0);
    assert(r.size() == 2); // indices 0 and 2
    assert(r[0] == 0 && r[1] == 2);

    // Coarse mode (0x1): inclusive, includes touching box 3
    r = boxQueryStatic(boxes, query, 0x1);
    assert(r.size() == 3); // 0, 2, 3

    // Only first (0x2): returns first intersecting index
    r = boxQueryStatic(boxes, query, 0x2);
    assert(r.size() == 1 && r[0] == 0);

    // Coarse + only first: still first intersecting
    r = boxQueryStatic(boxes, query, 0x1 | 0x2);
    assert(r.size() == 1 && r[0] == 0);

    // Query box that touches box 3 in strict mode should not include it
    Box query2 = {{2,0,0},{1,1,1}}; // touches box 0 and box 3 exactly
    r = boxQueryStatic(boxes, query2, 0);
    assert(r.empty()); // strict: no overlap (touching only)
    // Coarse mode: should include box 0 and box 3 (touching counts)
    r = boxQueryStatic(boxes, query2, 0x1);
    assert(r.size() == 2);
    assert((r[0] == 0 && r[1] == 3) || (r[0] == 3 && r[1] == 0));

    return 0;
}
#include <vector>
#include <cmath>
#include <cstdint>

struct Box {
    double center[3];   // x, y, z
    double half_size[3]; // half extents along each axis
};

// Returns indices of boxes that intersect the query box according to flags:
// bit 0 (0x1): coarse mode – inclusive intersection (touching counts)
// bit 1 (0x2): only first – return after first intersection found
std::vector<size_t> boxQueryStatic(const std::vector<Box>& boxes, const Box& query, unsigned flags) {
    const bool coarse = (flags & 0x1) != 0;
    const bool only_first = (flags & 0x2) != 0;
    std::vector<size_t> result;

    for (size_t i = 0; i < boxes.size(); ++i) {
        const Box& b = boxes[i];
        bool intersects = true;
        for (int dim = 0; dim < 3; ++dim) {
            double diff = std::abs(b.center[dim] - query.center[dim]);
            double sum = b.half_size[dim] + query.half_size[dim];
            if (coarse) {
                if (diff > sum) { intersects = false; break; }
            } else {
                if (diff >= sum) { intersects = false; break; } // strict: touching not allowed
            }
        }
        if (intersects) {
            result.push_back(i);
            if (only_first) break;
        }
    }
    return result;
}
// The solution iterates over each box in the input vector. For each box, we check intersection with the query box. A box A (center `ca`, half-size `ha`) and box B (center `cb`, half-size `hb`) intersect if the absolute difference along each axis is less than or equal to the sum of half-sizes for inclusive (coarse) or strictly less than for strict overlap (full test). Specifically, for each dimension `d` (x, y, z), we compute `diff = fabs(ca[d] - cb[d])` and `sum = ha[d] + hb[d]`. If `flags & 0x1` (coarse mode), we require `diff <= sum`; otherwise, we require `diff < sum`. If all three dimensions satisfy the condition, the boxes intersect. For only-first mode (`flags & 0x2`), we stop and return a vector containing just the first intersecting index. Otherwise, we collect all intersecting indices. Edge cases: boxes that are identical (always intersect), boxes that touch exactly on a face (handled differently by mode), and zero-size boxes (a point) — treat as a degenerate box with half-size 0, so inclusive works, strict requires the other box’s half-size to be > diff. Time complexity: O(n) in the worst case for full mode, O(1) in the best case for only-first; space O(n) for the result vector. No sorting or additional structures needed.
