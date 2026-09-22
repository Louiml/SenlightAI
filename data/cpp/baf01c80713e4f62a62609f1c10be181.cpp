// Given a 3D convex shape that can report a supporting vertex (the point on its surface farthest in a given direction) and has a non-negative margin (a uniform outward offset applied to the shape), write a C++ function that computes the axis-aligned bounding box (AABB) of the shape after it has been transformed by a rigid transformation (rotation + translation). The function must compute the AABB by sampling supporting vertices in the six axis directions (positive and negative X, Y, Z) in the local space, applying the transformation to each, and then adding or subtracting the margin along each axis. The input shape is modeled as a class with a pure virtual `localGetSupportingVertexWithoutMargin` method, a getter `getMargin()`, and a `getAabbSlow` method that takes a transform (represented by a basis matrix and an origin vector) and output min/max AABB vectors. The function should be standalone and not depend on external linear algebra libraries; instead, use simple 3-float arrays or a minimal struct/class for vectors and transforms.

The core algorithm mirrors the classic support-mapping AABB computation for convex objects. For each of the three axes (x, y, z), we first compute the supporting vertex in the positive direction (e.g., local direction (1,0,0)). This vertex is the point on the shape (without margin) that is farthest along that direction. We then transform this local point into world space using the rigid transform: world_point = rotation * local_point + translation. The maximum AABB coordinate along that axis is then `world_point[axis] + margin`. Similarly, for the negative direction (e.g., local direction (-1,0,0)), the supporting vertex gives the minimum extreme, and the minimum AABB coordinate is `world_point[axis] - margin`. This works because the supporting vertex in a given direction gives the point with the highest projection onto that direction; for the negative direction, it gives the lowest projection. The margin is a uniform offset added outward, so for max we add margin and for min we subtract. Edge cases: if the margin is zero, no offset is applied; if the shape has no vertices (should not happen, but we can assume the supporting vertex returns a valid point). The direction vector passed to the supporting function must be transformed by the rotation basis, because the supporting vertex is defined in local space and the direction must be expressed in local coordinates. However, the code snippet uses `vec * trans.getBasis()` which is unusual; in standard math, you would compute `local_dir = trans.getBasis().transpose() * world_dir` for a rotation matrix. But since we sample fixed world axes (1,0,0) etc., we actually need the supporting vertex in the local direction that corresponds to the world axis after applying the inverse rotation. For a pure rotation, the local direction is the world direction rotated by the inverse of the basis. The snippet multiplies the vector by the basis, which is effectively applying the rotation to the direction; but that is not correct unless the basis is orthogonal and the multiplication is interpreted as row-vector multiplication. For simplicity, we assume the transform is given by a 3x3 rotation matrix `basis` and a translation `origin`, and we compute the local direction as the transpose of the basis multiplied by the world direction. However, to avoid complexity and keep the task standalone, we can design the shape interface so that the supporting function takes a local direction directly, and we precompute the local direction by applying the inverse rotation. In the reference solution, we will assume the transform’s basis is an orthonormal matrix, and we will compute the local direction as `basis[0]`, `basis[1]`, `basis[2]` for positive axes, and their negatives for negative axes, because for a rigid body, the world axis `(1,0,0)` corresponds to the local direction equal to the first column of the basis (if column-vector convention). Actually, for a rotation matrix R, the local direction that points along world +X is the first row of R (if using row-vector convention) or the first column (if using column-vector). To be safe, we can simply use the transform’s basis matrix and multiply the world axis by the transpose of the basis, but since the supporting function expects a local vector, we can compute `local_dir = (world_dir * basis)` if basis is row-major. To avoid ambiguity, we will define a minimal `Transform` struct with three basis vectors (e.g., `basis[0]`, `basis[1]`, `basis[2]`) representing the orientation, and we will compute the local direction by taking the dot product of the world axis with each basis row. In the test, we will use a simple identity transform and a simple shape like a sphere (supporting vertex = direction * radius) to verify correctness. Time complexity is O(1) because we only perform 6 supporting queries, each of which is assumed to be constant time. Space complexity is O(1).

#include <array>
#include <cmath>
#include <utility>

// Minimal 3D vector representation using std::array
using Vec3 = std::array<double, 3>;

// Minimal rigid transform: rotation basis (3x3 matrix stored as array of 3 row vectors) and translation vector
struct Transform {
    std::array<Vec3, 3> basis; // each basis[i] is a row vector
    Vec3 origin;
};

// Abstract shape with a supporting vertex function (without margin) and a margin getter
class ConvexShape {
public:
    virtual ~ConvexShape() = default;
    // Returns the point on the shape's surface (excluding margin) farthest in the given local direction.
    // The direction is NOT normalized and can be zero.
    virtual Vec3 localGetSupportingVertexWithoutMargin(const Vec3& direction) const = 0;
    virtual double getMargin() const = 0;
};

// Compute the AABB of the transformed shape.
// The shape's margin is added outward (max + margin, min - margin).
// The transform rotates the shape's local coordinates into world space.
std::pair<Vec3, Vec3> computeTransformedAabb(const ConvexShape& shape, const Transform& t) {
    // The six world directions: +X, +Y, +Z, -X, -Y, -Z
    std::array<Vec3, 6> worldDirs = {{
        {1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0},
        {-1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, -1.0}
    }};

    Vec3 minAabb, maxAabb;
    const double margin = shape.getMargin();

    // For each axis i, we need the supporting vertex in the local direction that corresponds to the world direction.
    // Since the supporting function works in local coordinates, we must convert the world direction to local space.
    // For an orthonormal rotation matrix R (stored as rows), the local direction for a world direction d is (d * R),
    // i.e., component j = sum_i d_i * basis[i][j]. We compute that for each of the six directions.
    for (int axis = 0; axis < 3; ++axis) {
        // Positive direction along axis
        Vec3 localDirPos = {0.0, 0.0, 0.0};
        // We use worldDirs[axis] which is the unit vector along +axis
        // localDirPos[j] = sum_i worldDirs[axis][i] * t.basis[i][j]
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                localDirPos[j] += worldDirs[axis][i] * t.basis[i][j];
            }
        }
        Vec3 supportLocal = shape.localGetSupportingVertexWithoutMargin(localDirPos);
        // Transform local point to world: p_world = R * p_local + origin
        Vec3 supportWorld = {0.0, 0.0, 0.0};
        for (int j = 0; j < 3; ++j) {
            // supportWorld[j] = sum_i t.basis[j][i] * supportLocal[i] + t.origin[j]
            double acc = t.origin[j];
            for (int i = 0; i < 3; ++i) {
                acc += t.basis[j][i] * supportLocal[i];
            }
            supportWorld[j] = acc;
        }
        maxAabb[axis] = supportWorld[axis] + margin;

        // Negative direction along axis -> worldDirs[axis+3]
        Vec3 localDirNeg = {0.0, 0.0, 0.0};
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                localDirNeg[j] += worldDirs[axis + 3][i] * t.basis[i][j];
            }
        }
        supportLocal = shape.localGetSupportingVertexWithoutMargin(localDirNeg);
        supportWorld = {0.0, 0.0, 0.0};
        for (int j = 0; j < 3; ++j) {
            double acc = t.origin[j];
            for (int i = 0; i < 3; ++i) {
                acc += t.basis[j][i] * supportLocal[i];
            }
            supportWorld[j] = acc;
        }
        minAabb[axis] = supportWorld[axis] - margin;
    }

    return {minAabb, maxAabb};
}

#include <cassert>
#include <cmath>
#include <iostream>

// A simple sphere shape with radius and margin (for testing)
class SphereShape : public ConvexShape {
public:
    SphereShape(double radius, double margin = 0.0) : m_radius(radius), m_margin(margin) {}
    Vec3 localGetSupportingVertexWithoutMargin(const Vec3& direction) const override {
        // For a sphere centered at origin, the support point in direction d is (radius * d.normalized()).
        double len = std::sqrt(direction[0]*direction[0] + direction[1]*direction[1] + direction[2]*direction[2]);
        if (len < 1e-12) {
            return {m_radius, 0.0, 0.0}; // arbitrary point if direction is zero
        }
        double scale = m_radius / len;
        return {direction[0]*scale, direction[1]*scale, direction[2]*scale};
    }
    double getMargin() const override { return m_margin; }
private:
    double m_radius;
    double m_margin;
};

// Helper to check AABB equality with tolerance
void assertAabbClose(const std::pair<Vec3, Vec3>& got, const std::pair<Vec3, Vec3>& expected, double eps = 1e-9) {
    for (int i = 0; i < 3; ++i) {
        assert(std::fabs(got.first[i] - expected.first[i]) < eps);
        assert(std::fabs(got.second[i] - expected.second[i]) < eps);
    }
}

int main() {
    // Identity transform
    Transform identity;
    identity.basis = {{{1,0,0},{0,1,0},{0,0,1}}};
    identity.origin = {0,0,0};

    // Test 1: Sphere radius 2, no margin, no translation
    SphereShape sphere(2.0, 0.0);
    auto aabb1 = computeTransformedAabb(sphere, identity);
    assertAabbClose(aabb1, {{-2.0,-2.0,-2.0}, {2.0,2.0,2.0}});

    // Test 2: Sphere radius 1 with margin 0.5 -> AABB should expand by 0.5 on each side
    SphereShape sphere_margined(1.0, 0.5);
    auto aabb2 = computeTransformedAabb(sphere_margined, identity);
    assertAabbClose(aabb2, {{-1.5,-1.5,-1.5}, {1.5,1.5,1.5}});

    // Test 3: Sphere translated by (10, -5, 3)
    Transform translated;
    translated.basis = {{{1,0,0},{0,1,0},{0,0,1}}};
    translated.origin = {10.0, -5.0, 3.0};
    auto aabb3 = computeTransformedAabb(sphere, translated);
    assertAabbClose(aabb3, {{8.0, -7.0, 1.0}, {12.0, -3.0, 5.0}});

    // Test 4: Sphere rotated 90 degrees about Z axis (identity-like since sphere symmetric)
    // Rotating a sphere doesn't change AABB, but test the rotation path.
    Transform rotated;
    // 90-degree rotation about Z: basis rows: (cos90, -sin90,0) = (0,-1,0), (sin90, cos90,0) = (1,0,0), (0,0,1)
    rotated.basis = {{{0,-1,0}, {1,0,0}, {0,0,1}}};
    rotated.origin = {0,0,0};
    auto aabb4 = computeTransformedAabb(sphere, rotated);
    assertAabbClose(aabb4, {{-2.0,-2.0,-2.0}, {2.0,2.0,2.0}});

    // Test 5: A non-spherical shape would need custom shape; but for simplicity enough.

    // Test 6: Zero direction support (should not crash) - sphere still returns valid.
    // Not directly testable as the function always uses nonzero directions, but okay.

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
