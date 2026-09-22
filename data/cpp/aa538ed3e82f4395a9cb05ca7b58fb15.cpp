/*
Write a C++ function `FrustumCorners` that takes six floating-point parameters `hTan`, `vTan`, `nearClip`, `farClip`, and two output pointers `nearCorners` and `farCorners`. The function must compute the eight corners of a perspective frustum in the camera space, where the camera looks down the positive Z axis, using the formulas `nearX = hTan * nearClip`, `nearY = vTan * nearClip`, `farX = hTan * farClip`, `farY = vTan * farClip`. The four near corners must be written to `nearCorners` in exactly the order: lower-left, upper-left, lower-right, upper-right, with coordinates `(-nearX, -nearY, nearClip)`, `(-nearX, nearY, nearClip)`, `(nearX, -nearY, nearClip)`, `(nearX, nearY, nearClip)`. The four far corners must be written to `farCorners` in the same lower-left, upper-left, lower-right, upper-right order with coordinates `(-farX, -farY, farClip)`, `(-farX, farY, farClip)`, `(farX, -farY, farClip)`, `(farX, farY, farClip)`. All inputs are guaranteed positive and finite. The output arrays must be pre-allocated by the caller to hold at least four elements each. The function should be `const`-correct and use a simple custom struct for points. Provide a free function with a descriptive name, not a class method.
*/

#include <cstddef>

struct Point3 {
    float x, y, z;
};

// Compute the eight corners of a perspective frustum in camera space.
// nearCorners and farCorners must each point to an array of at least 4 Point3.
void FrustumCorners(float hTan, float vTan, float nearClip, float farClip,
                    Point3* nearCorners, Point3* farCorners) {
    const float nearX = hTan * nearClip;
    const float nearY = vTan * nearClip;
    const float farX = hTan * farClip;
    const float farY = vTan * farClip;

    // Near plane corners (lower-left, upper-left, lower-right, upper-right)
    nearCorners[0] = { -nearX, -nearY, nearClip };
    nearCorners[1] = { -nearX,  nearY, nearClip };
    nearCorners[2] = {  nearX, -nearY, nearClip };
    nearCorners[3] = {  nearX,  nearY, nearClip };

    // Far plane corners (same ordering)
    farCorners[0] = { -farX, -farY, farClip };
    farCorners[1] = { -farX,  farY, farClip };
    farCorners[2] = {  farX, -farY, farClip };
    farCorners[3] = {  farX,  farY, farClip };
}

#include <cassert>

int main() {
    Point3 near[4], far[4];

    // Test with simple values
    FrustumCorners(1.0f, 0.5f, 1.0f, 10.0f, near, far);

    // Near corners
    assert(near[0].x == -1.0f && near[0].y == -0.5f && near[0].z == 1.0f);
    assert(near[1].x == -1.0f && near[1].y ==  0.5f && near[1].z == 1.0f);
    assert(near[2].x ==  1.0f && near[2].y == -0.5f && near[2].z == 1.0f);
    assert(near[3].x ==  1.0f && near[3].y ==  0.5f && near[3].z == 1.0f);

    // Far corners
    assert(far[0].x == -10.0f && far[0].y == -5.0f && far[0].z == 10.0f);
    assert(far[1].x == -10.0f && far[1].y ==  5.0f && far[1].z == 10.0f);
    assert(far[2].x ==  10.0f && far[2].y == -5.0f && far[2].z == 10.0f);
    assert(far[3].x ==  10.0f && far[3].y ==  5.0f && far[3].z == 10.0f);

    // Test with different tangents and clips
    FrustumCorners(2.0f, 1.0f, 2.0f, 20.0f, near, far);
    assert(near[0].x == -4.0f && near[0].y == -2.0f && near[0].z == 2.0f);
    assert(near[3].x ==  4.0f && near[3].y ==  2.0f && near[3].z == 2.0f);
    assert(far[0].x == -40.0f && far[0].y == -20.0f && far[0].z == 20.0f);
    assert(far[3].x ==  40.0f && far[3].y ==  20.0f && far[3].z == 20.0f);

    // Test with zero tangents (degenerate but valid)
    FrustumCorners(0.0f, 0.0f, 5.0f, 50.0f, near, far);
    for (int i = 0; i < 4; ++i) {
        assert(near[i].x == 0.0f && near[i].y == 0.0f && near[i].z == 5.0f);
        assert(far[i].x == 0.0f && far[i].y == 0.0f && far[i].z == 50.0f);
    }

    return 0;
}

// The main algorithm is straightforward arithmetic. For each of the two pairs of clip-space extents (near and far), compute the X and Y half-widths by multiplying the tangent values by the clip distance. Then generate the four combinations of ±X and ±Y at each depth. The order of corners is fixed: start with negative X and negative Y, then negative X with positive Y, then positive X with negative Y, then positive X with positive Y. This matches the conventional frustum corner enumeration used in graphics code. The function has no branches or edge cases beyond validation of input positivity, which is not required since inputs are guaranteed valid. Time complexity is \(O(1)\) because exactly eight assignments are performed, and space complexity is \(O(1)\) beyond the output buffers. The output uses `Vector3`-like struct with `x`, `y`, `z` floats. The function should be `const`-qualified on its parameters where appropriate (parameters are passed by value, so `const` on them is optional but harmless). No dynamic memory is used, and the caller owns the output arrays.
