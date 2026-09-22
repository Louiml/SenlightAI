// Write a C++ function named `isInsideUnitBox` that takes two 3D coordinates, `point` and `minCorner` and `maxCorner`, each represented as `std::array<double, 3>`, and returns a `bool` indicating whether `point` lies strictly inside the axis-aligned box defined by `minCorner` and `maxCorner`. Strictly inside means that for each dimension, the point's coordinate must be strictly greater than the minimum corner's coordinate and strictly less than the maximum corner's coordinate. The function must handle any finite double values, including negative numbers, zero, and values that are exactly equal to the box boundaries (which should return `false`). Assume that `minCorner` is always less than or equal to `maxCorner` component-wise. The main interaction is to test whether a given point is inside the unit box with corners at (0,0,0) and (1,1,1), but the function must work for any valid box. Provide a self-contained implementation with necessary headers and comments.

The solution involves iterating over the three dimensions (x, y, z) and checking the strict inequality condition for each dimension. For a point to be strictly inside the box, its coordinate must satisfy `min < coordinate < max` for all three axes. If any coordinate fails this condition (i.e., it is less than or equal to min, or greater than or equal to max), the function should immediately return `false`. Otherwise, after all three checks pass, return `true`. Edge cases include points exactly on a face (e.g., x == min, or x == max) which must return `false` due to the strict inequalities. Also, if the box has zero volume (min == max in some dimension), no point can be inside. The time complexity is O(1) since we only do a fixed number of comparisons (3 dimensions). Space complexity is O(1) as well, no extra memory needed. No special handling for NaN or infinities is required, but they will fail the comparisons naturally.

#include <array>
#include <cstddef>

// Checks if a point lies strictly inside the axis-aligned box defined by minCorner and maxCorner.
// The point is inside if for each dimension i: minCorner[i] < point[i] < maxCorner[i].
bool isInsideBox(const std::array<double, 3>& point,
                 const std::array<double, 3>& minCorner,
                 const std::array<double, 3>& maxCorner) {
    for (std::size_t i = 0; i < 3; ++i) {
        if (point[i] <= minCorner[i] || point[i] >= maxCorner[i]) {
            return false;
        }
    }
    return true;
}

#include <cassert>
#include <array>

// Assume isInsideBox is defined as above.
int main() {
    const std::array<double, 3> min = {0.0, 0.0, 0.0};
    const std::array<double, 3> max = {1.0, 1.0, 1.0};

    // Point strictly inside the unit box
    assert(isInsideBox({0.5, 0.5, 0.5}, min, max) == true);

    // Point on the minimum face (x == 0)
    assert(isInsideBox({0.0, 0.5, 0.5}, min, max) == false);

    // Point on the maximum face (y == 1)
    assert(isInsideBox({0.5, 1.0, 0.5}, min, max) == false);

    // Point outside the box on one axis
    assert(isInsideBox({1.5, 0.5, 0.5}, min, max) == false);

    // Point with negative coordinate
    assert(isInsideBox({-0.1, 0.5, 0.5}, min, max) == false);

    // Custom box with different min/max
    const std::array<double, 3> min2 = {-2.0, -1.0, 0.0};
    const std::array<double, 3> max2 = {2.0, 1.0, 3.0};
    assert(isInsideBox({0.0, 0.0, 1.5}, min2, max2) == true);
    assert(isInsideBox({-2.0, 0.0, 1.5}, min2, max2) == false); // exactly on min x
    assert(isInsideBox({0.0, 1.0, 1.5}, min2, max2) == false);  // exactly on max y

    // Box with zero volume in z
    const std::array<double, 3> min3 = {0.0, 0.0, 0.5};
    const std::array<double, 3> max3 = {1.0, 1.0, 0.5};
    assert(isInsideBox({0.5, 0.5, 0.5}, min3, max3) == false);
}
