/*
Write a C++ function named `canPlacePicture` that takes four integer parameters: `wallWidth`, `wallHeight`, `pictureWidth`, and `pictureHeight`. The function must return `true` if a picture with the given dimensions can fit inside a wall after removing a 2-unit wide border from each side of the wall (i.e., reduce wall width and height by 2 on each side, effectively subtracting 4 total from each dimension or subtracting 2 from each side as in the original logic). Specifically, after subtracting 2 from both wall width and height, the resulting width must be greater than or equal to the picture's width, and the resulting height must be greater than or equal to the picture's height. If any parameter is negative, the function should return `false`. The function should handle all integer values within the range from 0 to 10,000 (inclusive). Use `const` correctly for parameters and return a `bool`.
*/
#include <algorithm> // for std::max

// Check if a picture can fit inside a wall after reducing both wall dimensions by 2 units on each side.
// Returns false if any input is negative.
bool canPlacePicture(const int wallWidth, const int wallHeight, const int pictureWidth, const int pictureHeight) {
    if (wallWidth < 0 || wallHeight < 0 || pictureWidth < 0 || pictureHeight < 0) {
        return false;
    }
    const int reducedWidth = wallWidth - 2;
    const int reducedHeight = wallHeight - 2;
    return (reducedWidth >= pictureWidth) && (reducedHeight >= pictureHeight);
}
#include <cassert>

int main() {
    // Basic fitting cases
    assert(canPlacePicture(10, 10, 8, 8) == true);   // 10-2=8, fits exactly
    assert(canPlacePicture(10, 10, 9, 9) == false);  // 9 > 8
    assert(canPlacePicture(10, 10, 8, 7) == true);   // height fits
    assert(canPlacePicture(0, 0, 0, 0) == false);    // reduced to -2, negative fails
    assert(canPlacePicture(5, 5, 3, 3) == true);     // 5-2=3
    // Negative inputs
    assert(canPlacePicture(-1, 5, 3, 3) == false);
    assert(canPlacePicture(5, -1, 3, 3) == false);
    // Large values
    assert(canPlacePicture(10000, 10000, 9998, 9998) == true);
    assert(canPlacePicture(10000, 10000, 9999, 9998) == false);
    // Picture larger than reduced wall
    assert(canPlacePicture(2, 2, 1, 1) == false);    // reduced to 0
    assert(canPlacePicture(3, 3, 1, 1) == true);     // reduced to 1
    return 0;
}
// The solution is straightforward: first validate that all inputs are non-negative (though the original snippet only allowed up to 1000 for wall dimensions and up to 10000 for picture dimensions, we generalize to allow up to 10000 for all). Then subtract 2 from wall width and height to simulate the border removal. If either subtraction results in a negative value, that’s fine because we then compare with picture dimensions; if the picture width or height is larger than the reduced wall dimensions, return `false`, otherwise `true`. Edge cases include when wall dimensions are less than 2 (so reduction makes them negative) — such cases will naturally fail the comparison if picture dimensions are non-negative, because a negative reduced dimension can never be ≥ a non-negative picture dimension (unless picture is also negative, but we reject negatives). So the algorithm is O(1) time and O(1) space.
