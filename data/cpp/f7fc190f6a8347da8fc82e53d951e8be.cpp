Given an array of `n` 3D axis-aligned bounding boxes, each defined by its center coordinates `(cx, cy, cz)` and half-extents `(hx, hy, hz)`, write a C++ function that returns `true` if any two boxes intersect (overlap in all three dimensions, including touching faces as intersection) and `false` otherwise. The input is a vector of a simple struct (provided), and the function must be `const`-correct and handle empty or single-element arrays gracefully. Boxes may have zero or negative half-extents—treat any half-extent ≤ 0 as causing that dimension to be a single point or invalid; for intersection testing, treat the box as degenerate but still check overlap with the given intervals (i.e., if half-extent ≤ 0, the interval is just the center point). Provide a self-contained solution with no global state.
#include <cassert>
#include <vector>

// (Box3D and AnyBoxesIntersect definitions here)

int main() {
    // Empty vector
    assert(AnyBoxesIntersect({}) == false);

    // Single box
    std::vector<Box3D> single = {{0,0,0, 1,1,1}};
    assert(AnyBoxesIntersect(single) == false);

    // Touching faces (share a plane at x=0)
    std::vector<Box3D> touch = {
        {-1,0,0, 1,1,1},  // x in [-2,0]
        {1,0,0, 1,1,1}    // x in [0,2]
    };
    assert(AnyBoxesIntersect(touch) == true);

    // Non-overlapping in all axes
    std::vector<Box3D> separate = {
        {0,0,0, 1,1,1},   // x,y,z all in [-1,1]
        {5,5,5, 0.5,0.5,0.5} // x,y,z all in [4.5,5.5]
    };
    assert(AnyBoxesIntersect(separate) == false);

    // Overlap in all axes
    std::vector<Box3D> overlap = {
        {0,0,0, 2,2,2},   // x,y,z all in [-2,2]
        {1,1,1, 1,1,1}    // x,y,z all in [0,2]
    };
    assert(AnyBoxesIntersect(overlap) == true);

    // Mixed: first with second overlap, third separate
    std::vector<Box3D> mixed = {
        {0,0,0, 1,1,1},
        {0.5,0.5,0.5, 0.1,0.1,0.1},  // inside first
        {10,10,10, 0.2,0.2,0.2}
    };
    assert(AnyBoxesIntersect(mixed) == true);

    // Degenerate half-extent zero (point box) touching a box
    std::vector<Box3D> pointOnSurface = {
        {0,0,0, 1,1,1},   // box
        {1,0,0, 0,0,0}    // point at x=1, y=0, z=0 (on the surface)
    };
    assert(AnyBoxesIntersect(pointOnSurface) == true);

    // Degenerate half-extent negative (invalid but treat as point)
    std::vector<Box3D> negativeHalf = {
        {0,0,0, 1,1,1},
        {3,3,3, -1,-1,-1} // effectively point at (3,3,3), no overlap
    };
    assert(AnyBoxesIntersect(negativeHalf) == false);

    // Many boxes, none intersect
    std::vector<Box3D> manySeparate;
    for (int i = 0; i < 10; ++i) {
        manySeparate.push_back({(double)i*10, 0, 0, 1,1,1});
    }
    assert(AnyBoxesIntersect(manySeparate) == false);

    // Many boxes, last two intersect
    std::vector<Box3D> manyWithIntersection;
    for (int i = 0; i < 8; ++i) {
        manyWithIntersection.push_back({(double)i*10, 0, 0, 1,1,1});
    }
    manyWithIntersection.push_back({75,0,0, 2,2,2}); // overlaps with box at x=70
    manyWithIntersection.push_back({78,0,0, 1,1,1});
    assert(AnyBoxesIntersect(manyWithIntersection) == true);

    return 0;
}
#include <vector>
#include <cmath>

// Axis-aligned bounding box with center and half-extents.
struct Box3D {
    double cx, cy, cz; // center coordinates
    double hx, hy, hz; // half-extents (can be ≤ 0)
};

// Return true if any two boxes intersect (including touching faces).
bool AnyBoxesIntersect(const std::vector<Box3D>& boxes) {
    const size_t n = boxes.size();
    for (size_t i = 0; i < n; ++i) {
        const Box3D& a = boxes[i];
        // Intervals for box a
        const double a_min_x = a.cx - a.hx;
        const double a_max_x = a.cx + a.hx;
        const double a_min_y = a.cy - a.hy;
        const double a_max_y = a.cy + a.hy;
        const double a_min_z = a.cz - a.hz;
        const double a_max_z = a.cz + a.hz;

        for (size_t j = i + 1; j < n; ++j) {
            const Box3D& b = boxes[j];
            // Check overlap on all three axes
            const bool overlap_x = (a_min_x <= b.cx + b.hx) && (b.cx - b.hx <= a_max_x);
            const bool overlap_y = (a_min_y <= b.cy + b.hy) && (b.cy - b.hy <= a_max_y);
            const bool overlap_z = (a_min_z <= b.cz + b.hz) && (b.cz - b.hz <= a_max_z);
            if (overlap_x && overlap_y && overlap_z) {
                return true;
            }
        }
    }
    return false;
}
// The core algorithm is straightforward pairwise comparison: for each pair of boxes `(i, j)` with `i < j`, check if their intervals overlap on the x, y, and z axes. Two intervals `[a_min, a_max]` and `[b_min, b_max]` overlap if `a_min <= b_max` and `b_min <= a_max`. For each box, the min coordinate along an axis is `center - halfExtent` and the max is `center + halfExtent`. If all three axes overlap, the boxes intersect. The function runs in O(n²) time and O(1) extra space. Edge cases: empty vector returns `false` (no pairs), single element returns `false` (no pair to test). Degenerate boxes (half-extent ≤ 0) naturally produce a single-point interval; overlap still works with the same inequality. Touching faces (e.g., one box's max equals another's min) count as intersection because the inequality uses `<=`. The implementation must be careful with floating-point comparisons; using direct `<=` is acceptable for this task. We assume no `NaN` values. The solution uses a nested loop with `i` from 0 to n-1 and `j` from i+1 to n-1, early-exit on first true.
