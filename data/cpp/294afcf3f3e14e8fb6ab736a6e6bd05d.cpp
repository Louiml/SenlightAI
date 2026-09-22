/*
Given a 3D vector `direction` and a positive scaling factor `radius`, write a C++ function named `createTangentFrame` that computes a local orthonormal frame (basis) aligned with `direction`. The function must return a `std::tuple` of three `Eigen::Vector3f` values: the normalized `direction` as the Z-axis, and two perpendicular unit vectors (X-axis and Y-axis) that form a right-handed coordinate system. The X-axis must be computed as the normalized cross product of the world Y-axis (0,1,0) with `direction`, unless `direction` is nearly parallel to the Y-axis (within a small epsilon), in which case use the world X-axis (1,0,0) instead. The Y-axis is then the cross product of `direction` with the X-axis. Implement this without relying on Eigen's `unitOrthogonal()` method; compute the cross products manually. The function must be `const`-correct and handle the degenerate case where the input vector is zero by returning the standard basis (X=(1,0,0), Y=(0,1,0), Z=(0,0,1)). The function must be self-contained with appropriate `#include` directives and use only Eigen's `Vector3f` and `Matrix` types.
*/
#include <Eigen/Core>
#include <tuple>
#include <cmath>

// Compute a right-handed orthonormal frame aligned with the given direction.
// The returned tuple contains (X, Y, Z), where Z = normalized direction,
// X and Y are perpendicular unit vectors such that X × Y = Z.
// If the input is zero, returns the standard basis.
std::tuple<Eigen::Vector3f, Eigen::Vector3f, Eigen::Vector3f>
createTangentFrame(const Eigen::Vector3f& direction) {
    const float eps = 1e-6f;
    const float lenSq = direction.squaredNorm();

    // Handle zero vector by returning standard basis.
    if (lenSq < eps * eps) {
        return std::make_tuple(Eigen::Vector3f::UnitX(),
                               Eigen::Vector3f::UnitY(),
                               Eigen::Vector3f::UnitZ());
    }

    // Normalize to get the Z-axis.
    Eigen::Vector3f z = direction / std::sqrt(lenSq);

    // Choose a reference vector that is not collinear with z.
    // If z is nearly parallel to world Y, use world X; else use world Y.
    Eigen::Vector3f reference = (std::abs(z.y()) > 1.0f - eps)
                                    ? Eigen::Vector3f::UnitX()
                                    : Eigen::Vector3f::UnitY();

    // Compute X = normalize(reference × z).
    Eigen::Vector3f x = reference.cross(z);
    x.normalize();

    // Compute Y = z × x (unit vector, orthogonal to both).
    Eigen::Vector3f y = z.cross(x);

    return std::make_tuple(x, y, z);
}
#include <cassert>
#include <cmath>

int main() {
    using Eigen::Vector3f;

    // Test 1: Basic direction along X-axis (Z = X, frame identity-like).
    auto [x1, y1, z1] = createTangentFrame(Vector3f(1, 0, 0));
    assert((z1 - Vector3f::UnitX()).norm() < 1e-6);
    assert((x1 - Vector3f::UnitZ()).norm() < 1e-6); // reference Y × Z = (0,1,0)×(1,0,0) = (0,0,1)
    assert((y1 - Vector3f::UnitY()).norm() < 1e-6); // Z × X = (1,0,0)×(0,0,1) = (0,1,0)
    // Check orthonormality: dot products zero, norms one.
    assert(std::abs(x1.dot(y1)) < 1e-6);
    assert(std::abs(x1.dot(z1)) < 1e-6);
    assert(std::abs(y1.dot(z1)) < 1e-6);
    assert(std::abs(x1.norm() - 1.0f) < 1e-6);
    assert(std::abs(y1.norm() - 1.0f) < 1e-6);
    assert(std::abs(z1.norm() - 1.0f) < 1e-6);

    // Test 2: Direction along Y-axis (must use X-axis as reference).
    auto [x2, y2, z2] = createTangentFrame(Vector3f(0, 1, 0));
    assert((z2 - Vector3f::UnitY()).norm() < 1e-6);
    assert((x2 - Vector3f::UnitZ()).norm() < 1e-6); // X × Y = (1,0,0)×(0,1,0) = (0,0,1)
    assert((y2 - Vector3f::UnitX()).norm() < 1e-6); // Y × Z = (0,1,0)×(0,0,1) = (1,0,0)
    assert(std::abs(x2.dot(y2)) < 1e-6);
    assert(std::abs(x2.dot(z2)) < 1e-6);
    assert(std::abs(y2.dot(z2)) < 1e-6);

    // Test 3: Direction along Z-axis.
    auto [x3, y3, z3] = createTangentFrame(Vector3f(0, 0, 1));
    assert((z3 - Vector3f::UnitZ()).norm() < 1e-6);
    assert((x3 - Vector3f::UnitX()).norm() < 1e-6); // Y × Z = (0,1,0)×(0,0,1) = (1,0,0)
    assert((y3 - Vector3f::UnitY()).norm() < 1e-6); // Z × X = (0,0,1)×(1,0,0) = (0,1,0)

    // Test 4: Arbitrary direction, check right-handedness and orthonormality.
    auto [x4, y4, z4] = createTangentFrame(Vector3f(1, 2, 3));
    assert(std::abs(x4.norm() - 1.0f) < 1e-6);
    assert(std::abs(y4.norm() - 1.0f) < 1e-6);
    assert(std::abs(z4.norm() - 1.0f) < 1e-6);
    assert(std::abs(x4.dot(y4)) < 1e-6);
    assert(std::abs(x4.dot(z4)) < 1e-6);
    assert(std::abs(y4.dot(z4)) < 1e-6);
    // Cross product X × Y should equal Z (right-handed).
    Vector3f crossXY = x4.cross(y4);
    assert((crossXY - z4).norm() < 1e-6);

    // Test 5: Zero vector → standard basis.
    auto [x5, y5, z5] = createTangentFrame(Vector3f::Zero());
    assert((x5 - Vector3f::UnitX()).norm() < 1e-6);
    assert((y5 - Vector3f::UnitY()).norm() < 1e-6);
    assert((z5 - Vector3f::UnitZ()).norm() < 1e-6);

    // Test 6: Normalization of input (long vector).
    auto [x6, y6, z6] = createTangentFrame(Vector3f(0, 0, 100));
    assert((z6 - Vector3f::UnitZ()).norm() < 1e-6);
    assert((x6 - Vector3f::UnitX()).norm() < 1e-6);
    assert((y6 - Vector3f::UnitY()).norm() < 1e-6);

    // Test 7: Negative direction along X-axis.
    auto [x7, y7, z7] = createTangentFrame(Vector3f(-1, 0, 0));
    assert((z7 - (-Vector3f::UnitX())).norm() < 1e-6);
    assert((x7 - Vector3f::UnitZ()).norm() < 1e-6); // Y × (-X) = (0,1,0)×(-1,0,0) = (0,0,-1)? Check: (0,1,0)×(-1,0,0) = (1*0-0*0, 0*(-1)-0*0, 0*0-1*(-1)) = (0,0,1)
    assert((y7 - Vector3f::UnitY()).norm() < 1e-6); // (-X) × Z = (-1,0,0)×(0,0,1) = (0*1-0*0, 0*0-(-1)*1, (-1)*0-0*0) = (0,1,0)

    return 0;
}
// The solution constructs an orthonormal basis from a single direction vector, which is a common operation in computer graphics for orienting objects or building local coordinate systems (e.g., for placement or animation). The algorithm proceeds as follows:
// 1. **Normalize the input**: If the input vector has negligible squared norm (e.g., < 1e-12), return the identity basis. Otherwise, normalize `direction` to obtain the Z-axis.
// 2. **Choose a reference vector**: To avoid a degenerate cross product, check if the absolute value of the Z-axis's Y-component is close to 1 (i.e., parallel to the world Y-axis). If so, use the world X-axis (1,0,0); otherwise, use the world Y-axis (0,1,0). This ensures the cross product with `direction` is non-zero.
// 3. **Compute the X-axis**: Take the cross product of the chosen reference vector with the Z-axis, then normalize the result. This yields a vector perpendicular to `direction`.
// 4. **Compute the Y-axis**: Take the cross product of the Z-axis with the X-axis. Since both X and Z are unit vectors and orthogonal, this automatically yields a unit vector orthogonal to both, forming a right-handed basis.
// 5. **Return the tuple**: Assemble the three vectors into a `std::tuple<Vector3f, Vector3f, Vector3f>`.
//
// **Edge cases**: Zero vector (return identity basis). Vector parallel to Y-axis (choose X-axis as reference). The epsilon threshold (e.g., 1e-6) for parallelism is small enough to avoid false positives but large enough to prevent floating-point precision issues. The squared value is used to avoid a square root in the comparison.
//
// **Complexity**: The function performs a constant number of vector operations (cross products, dot products, normalizations), so time complexity is O(1). Space complexity is O(1) aside from the returned tuple, which holds three fixed-size vectors.
