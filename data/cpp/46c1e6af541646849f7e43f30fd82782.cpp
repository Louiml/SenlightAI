Write a C++ function that takes a 3x3 rotation matrix represented by three row vectors (each of type `Vector3<double>`) and returns the equivalent yaw angle in radians, in the range `(-π, π]`, as computed by converting the matrix back to Euler angles using the standard aerospace convention (yaw around the Z-axis, pitch around the Y-axis, roll around the X-axis). The input matrix is guaranteed to be a valid rotation matrix (orthonormal with determinant +1). The function should be named `getYawFromRotationMatrix` and must accept the matrix as a `const` reference. You may define a minimal `Vector3` struct and a `Matrix3` struct as needed, but the function itself must be the only externally visible interface. The yaw angle should satisfy: if the rotation matrix is applied to a vector pointing along the positive X-axis, the resulting vector's angle with the original X-axis (measured in the XY plane) equals the returned yaw.

#include <cassert>
#include <cmath>

// Vector3 and Matrix3 definitions are assumed to be available from the solution.
// For the test, we re-define them here to ensure self-containment.
struct Vector3 {
    double x, y, z;
    Vector3(double x_ = 0, double y_ = 0, double z_ = 0) : x(x_), y(y_), z(z_) {}
};

struct Matrix3 {
    Vector3 a, b, c;
    Matrix3(const Vector3& a_ = Vector3(), const Vector3& b_ = Vector3(), const Vector3& c_ = Vector3())
        : a(a_), b(b_), c(c_) {}
};

// Solution function (copy from above).
double getYawFromRotationMatrix(const Matrix3& m) {
    const double eps = 1e-12;
    double ax = m.a.x;
    double bx = m.b.x;
    if (std::abs(ax) < eps && std::abs(bx) < eps) {
        return 0.0;
    }
    return std::atan2(bx, ax);
}

int main() {
    const double eps = 1e-9;
    // Identity matrix -> yaw 0
    Matrix3 identity(Vector3(1,0,0), Vector3(0,1,0), Vector3(0,0,1));
    assert(std::abs(getYawFromRotationMatrix(identity)) < eps);

    // Yaw 90 degrees: rotation matrix for yaw=pi/2
    Matrix3 yaw90(Vector3(0,-1,0), Vector3(1,0,0), Vector3(0,0,1));
    double y = getYawFromRotationMatrix(yaw90);
    assert(std::abs(y - M_PI/2) < eps);

    // Yaw 45 degrees
    double s = std::sqrt(0.5);
    Matrix3 yaw45(Vector3(s, -s, 0), Vector3(s, s, 0), Vector3(0,0,1));
    assert(std::abs(getYawFromRotationMatrix(yaw45) - M_PI/4) < eps);

    // Yaw 180 degrees
    Matrix3 yaw180(Vector3(-1,0,0), Vector3(0,-1,0), Vector3(0,0,1));
    double y180 = getYawFromRotationMatrix(yaw180);
    // atan2(-0, -1) returns pi (since -0 is negative zero, but we use 0.0)
    assert(std::abs(y180 - M_PI) < eps || std::abs(y180 + M_PI) < eps);

    // Yaw -90 degrees (or 270)
    Matrix3 yawNeg90(Vector3(0,1,0), Vector3(-1,0,0), Vector3(0,0,1));
    assert(std::abs(getYawFromRotationMatrix(yawNeg90) + M_PI/2) < eps);

    // Yaw -45 degrees
    Matrix3 yawNeg45(Vector3(s, s, 0), Vector3(-s, s, 0), Vector3(0,0,1));
    assert(std::abs(getYawFromRotationMatrix(yawNeg45) + M_PI/4) < eps);

    // Pitch 90 degrees: a.x = 0, b.x = 0 -> returns 0
    Matrix3 pitch90(Vector3(0,0,1), Vector3(0,1,0), Vector3(-1,0,0));
    // Note: this is a valid rotation matrix for pitch=pi/2 with roll=yaw=0
    // The exact matrix for pitch=pi/2: R = [0 0 1; 0 1 0; -1 0 0]
    assert(std::abs(getYawFromRotationMatrix(pitch90)) < eps);

    // A random valid rotation (yaw=1.0, pitch=0.2, roll=0.3)
    double cy = std::cos(1.0), sy = std::sin(1.0);
    double cp = std::cos(0.2), sp = std::sin(0.2);
    double cr = std::cos(0.3), sr = std::sin(0.3);
    Matrix3 euler(
        Vector3(cp*cy, sr*sp*cy - cr*sy, cr*sp*cy + sr*sy),
        Vector3(cp*sy, sr*sp*sy + cr*cy, cr*sp*sy - sr*cy),
        Vector3(-sp, sr*cp, cr*cp)
    );
    assert(std::abs(getYawFromRotationMatrix(euler) - 1.0) < eps);

    return 0;
}

#include <cmath>
#include <algorithm>

// Minimal 3D vector representation.
struct Vector3 {
    double x, y, z;
    Vector3(double x_ = 0, double y_ = 0, double z_ = 0) : x(x_), y(y_), z(z_) {}
};

// Minimal 3x3 matrix stored as three row vectors.
struct Matrix3 {
    Vector3 a, b, c;
    Matrix3(const Vector3& a_ = Vector3(), const Vector3& b_ = Vector3(), const Vector3& c_ = Vector3())
        : a(a_), b(b_), c(c_) {}
};

// Return the yaw angle (in radians) from a rotation matrix.
// Uses the aerospace convention: R = Rz(yaw) * Ry(pitch) * Rx(roll).
// The yaw is extracted via atan2(b.x, a.x).
// If pitch is +-90 degrees (gimbal lock), a.x and b.x are both zero;
// we then return 0.0 as a canonical choice.
double getYawFromRotationMatrix(const Matrix3& m) {
    const double eps = 1e-12;
    // a.x = cos(pitch)*cos(yaw), b.x = cos(pitch)*sin(yaw)
    double ax = m.a.x;
    double bx = m.b.x;
    if (std::abs(ax) < eps && std::abs(bx) < eps) {
        // Gimbal lock: pitch = +-90 deg, yaw is not uniquely defined.
        return 0.0;
    }
    return std::atan2(bx, ax);
}

// The key is to extract the yaw angle from a rotation matrix. Using the standard aerospace Euler angle convention (roll-pitch-yaw, i.e., R = Rz(yaw) * Ry(pitch) * Rx(roll)), the rotation matrix has the form:
// - Row 0 (a): [cos(pitch)*cos(yaw), sin(roll)*sin(pitch)*cos(yaw) - cos(roll)*sin(yaw), cos(roll)*sin(pitch)*cos(yaw) + sin(roll)*sin(yaw)]
// - Row 1 (b): [cos(pitch)*sin(yaw), sin(roll)*sin(pitch)*sin(yaw) + cos(roll)*cos(yaw), cos(roll)*sin(pitch)*sin(yaw) - sin(roll)*cos(yaw)]
// - Row 2 (c): [-sin(pitch), sin(roll)*cos(pitch), cos(roll)*cos(pitch)]
//
// From this, yaw = atan2(b.x, a.x), where b.x = cos(pitch)*sin(yaw) and a.x = cos(pitch)*cos(yaw). If cos(pitch) is zero (i.e., pitch = ±90°), this formula degenerates because both a.x and b.x become zero, causing atan2(0,0) to be technically undefined (though the standard library returns 0). However, the problem guarantees a valid rotation matrix, but does not exclude pitch = ±90°. To handle this gimbal lock case robustly, we must detect when a.x and b.x are both near zero and then define yaw arbitrarily (e.g., set to 0) because yaw and roll become coupled. Since the task only asks for a yaw angle, we can define it as follows: when |a.x| and |b.x| are both below a small epsilon (e.g., 1e-12), return 0.0. Otherwise, return `atan2(b.x, a.x)`. The `atan2` function returns values in `(-π, π]`, which satisfies the range requirement. For a valid rotation matrix, `a.x` and `b.x` cannot both be zero unless pitch is ±90°, so this edge case is handled. Time complexity is O(1) and space complexity O(1), as the function performs a constant number of arithmetic operations and no dynamic allocation.
