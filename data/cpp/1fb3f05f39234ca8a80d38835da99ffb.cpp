// Write a standalone C++ function named `applyTwistDeformation` that applies a twist deformation to a 3D point. The function takes a 3D input vector (`std::array<double,3>`), a twist angle in radians (`double angle`), a twist axis (`int axis`, where 0=X, 1=Y, 2=Z), and a bounding box height (`double height`, which is the extent along the twist axis; if zero or negative, return the original point). The twist rotates the point around the specified axis by an angle proportional to the normalized position along that axis. More specifically: first, translate the point so the twist axis origin is at 0 (i.e., if axis=Y, the y-coordinate is the height coordinate), then compute the normalized coordinate `u = coord / height`, where `coord` is the component along the chosen axis. Then rotate the two perpendicular coordinates (the other two axes) around the chosen axis by an angle equal to `angle * u`. Finally, translate back by adding the original coordinate along the axis to the rotated point. The output is the deformed point. For simplicity, assume the bounding box is centered at the origin, so no translation is needed; just rotate the perpendicular components by `angle * (coord / height)`. If `height` is zero, negative, or if `angle` is zero, return the input unchanged. Also handle negative `coord` values: use the signed normalized value (not absolute value) so that points below the center twist in the opposite direction. The function should be const-correct and use `std::array<double,3>` for both input and output. The function must be self-contained (include necessary headers only: `<array>`, `<cmath>`) and must not contain a `main` function.
// The core algorithm is a simple rotation around one coordinate axis, with the rotation angle linearly interpolating from 0 at the axis origin (coordinate = 0) to `angle` at coordinate `height`. For each input point, we identify the coordinate along the twist axis (e.g., for axis=1, that's `y`). The normalized parameter is `u = y / height`, with `height` provided as a positive value. The rotation is applied to the two orthogonal coordinates (for axis=1, those are x and z). The rotation matrix is standard: for a rotation angle `a = angle * u`, we compute `cos(a)` and `sin(a)`, then update the perpendicular coordinates: `new_x = x*cos(a) - z*sin(a)` and `new_z = x*sin(a) + z*cos(a)`. The axis coordinate remains unchanged. Edge cases: if `height` is zero or negative, we cannot normalize; return the input unchanged. If `angle` is zero, the rotation is identity, so we can return early. Also, since we are using `double`, we should tolerate floating-point imprecision; but for the test, we compare with a tolerance. However, the task requires using `assert` with `==` or suitable comparison; we'll compare with a small epsilon. The time complexity is O(1) per point, and space complexity is O(1) aside from the output array.
#include <array>
#include <cmath>

// Apply a twist deformation to a 3D point.
// axis: 0 = X, 1 = Y, 2 = Z. The twist rotates the two perpendicular axes
// by an angle proportional to the coordinate along the given axis.
// height is the full extent along the twist axis (must be positive).
// If height <= 0 or angle == 0, returns the original point.
std::array<double, 3> applyTwistDeformation(
    const std::array<double, 3>& point,
    double angle,
    int axis,
    double height)
{
    // Early exit for invalid parameters or zero angle.
    if (height <= 0.0 || angle == 0.0) {
        return point;
    }

    // Extract coordinates.
    double x = point[0];
    double y = point[1];
    double z = point[2];

    // Determine the axis coordinate and the two perpendicular coordinates.
    double coord, a, b; // a and b are the two rotating coordinates.
    if (axis == 0) { // X axis: rotate y and z around x
        coord = x;
        a = y;
        b = z;
    } else if (axis == 1) { // Y axis: rotate x and z around y
        coord = y;
        a = x;
        b = z;
    } else if (axis == 2) { // Z axis: rotate x and y around z
        coord = z;
        a = x;
        b = y;
    } else {
        // Invalid axis, return unchanged.
        return point;
    }

    // Normalized position along the axis.
    double u = coord / height;

    // Rotation angle proportional to u.
    double theta = angle * u;

    double cos_t = std::cos(theta);
    double sin_t = std::sin(theta);

    // Rotate the two perpendicular coordinates.
    double a_new = a * cos_t - b * sin_t;
    double b_new = a * sin_t + b * cos_t;

    // Reconstruct the rotated point.
    std::array<double, 3> result;
    if (axis == 0) {
        result[0] = coord;
        result[1] = a_new;
        result[2] = b_new;
    } else if (axis == 1) {
        result[0] = a_new;
        result[1] = coord;
        result[2] = b_new;
    } else { // axis == 2
        result[0] = a_new;
        result[1] = b_new;
        result[2] = coord;
    }

    return result;
}
#include <cassert>
#include <cmath>
#include <array>

// The solution function is assumed to be declared above.
// For the test, we copy the function definition here or rely on forward declaration.
// To keep this self-contained, we include the function definition in the same file.
// In this test section, we just include a main() that calls the function.

int main() {
    // Test 1: Zero angle returns identical point.
    std::array<double,3> p1 = {1.0, 2.0, 3.0};
    auto r1 = applyTwistDeformation(p1, 0.0, 1, 5.0);
    assert(r1[0] == 1.0 && r1[1] == 2.0 && r1[2] == 3.0);

    // Test 2: Height zero or negative returns original.
    auto r2 = applyTwistDeformation(p1, 1.0, 1, 0.0);
    assert(r2 == p1);
    auto r3 = applyTwistDeformation(p1, 1.0, 1, -3.0);
    assert(r3 == p1);

    // Test 3: Point at the center (coord=0) is unchanged (angle*0=0).
    std::array<double,3> p3 = {2.0, 0.0, 4.0}; // y=0, axis=1
    auto r4 = applyTwistDeformation(p3, 2.0, 1, 8.0);
    assert(fabs(r4[0]-2.0) < 1e-9 && fabs(r4[1]) < 1e-9 && fabs(r4[2]-4.0) < 1e-9);

    // Test 4: Point at coord=height, angle=pi/2 around Y axis: 
    // (1, height, 0) -> (0, height, 1) for a 90-degree rotation? 
    // Actually rotating x,z by +90 deg: x'= -z? Standard rotation: new_x = x*cos - z*sin, new_z = x*sin + z*cos.
    // For (x=1, z=0), angle=pi/2: new_x = 1*0 - 0*1 = 0; new_z = 1*1 + 0*0 = 1.
    // So we expect (0, height, 1) approximately.
    std::array<double,3> p4 = {1.0, 10.0, 0.0}; // y=height=10, axis=1
    double half_pi = 3.14159265358979323846 / 2.0;
    auto r5 = applyTwistDeformation(p4, half_pi, 1, 10.0);
    assert(fabs(r5[0] - 0.0) < 1e-9);
    assert(fabs(r5[1] - 10.0) < 1e-9);
    assert(fabs(r5[2] - 1.0) < 1e-9);

    // Test 5: Negative coordinate below center rotates in opposite direction.
    // Point (1, -5, 0) with height=10, angle=pi/2: u=-0.5, theta=-pi/4.
    // cos = sqrt(2)/2, sin = -sqrt(2)/2. new_x = 1*cos - 0*sin = cos, new_z = 1*sin = -sin.
    // So expect x = sqrt(2)/2, z = -sqrt(2)/2.
    std::array<double,3> p5 = {1.0, -5.0, 0.0};
    auto r6 = applyTwistDeformation(p5, half_pi, 1, 10.0);
    double expected = std::sqrt(2.0)/2.0;
    assert(fabs(r6[0] - expected) < 1e-9);
    assert(fabs(r6[1] - (-5.0)) < 1e-9);
    assert(fabs(r6[2] - (-expected)) < 1e-9);

    // Test 6: Axis 0 (X) twist: point (5,1,0) with height=5, angle=pi/2 -> u=1, rotate y,z by 90 deg.
    // (y=1,z=0) -> new_y = 1*0 - 0*1 = 0, new_z = 1*1 = 1, x unchanged.
    std::array<double,3> p6 = {5.0, 1.0, 0.0};
    auto r7 = applyTwistDeformation(p6, half_pi, 0, 5.0);
    assert(fabs(r7[0] - 5.0) < 1e-9);
    assert(fabs(r7[1] - 0.0) < 1e-9);
    assert(fabs(r7[2] - 1.0) < 1e-9);

    // Test 7: Axis 2 (Z) twist: point (1,1,5) with height=5, angle=pi/2 -> u=1, rotate x,y by 90 deg.
    // (x=1,y=1) -> new_x = 1*0 - 1*1 = -1, new_y = 1*1 + 1*0 = 1, z unchanged.
    std::array<double,3> p7 = {1.0, 1.0, 5.0};
    auto r8 = applyTwistDeformation(p7, half_pi, 2, 5.0);
    assert(fabs(r8[0] - (-1.0)) < 1e-9);
    assert(fabs(r8[1] - 1.0) < 1e-9);
    assert(fabs(r8[2] - 5.0) < 1e-9);

    // Test 8: Invalid axis (3) returns original.
    auto r9 = applyTwistDeformation(p1, half_pi, 3, 5.0);
    assert(r9 == p1);

    return 0;
}
