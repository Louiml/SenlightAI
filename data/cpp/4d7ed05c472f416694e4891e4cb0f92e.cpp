Write a C++ function that computes the center of mass of a serial kinematic chain consisting of exactly 7 links, given an array of link masses, an array of link local center-of-mass positions (as 3D vectors), an array of 4×4 homogeneous transformation matrices representing the relative pose between consecutive links (all given in the chain's base frame, i.e., already composed with the base-to-first-link transform for the first matrix), and the total mass. The function must return the global center of mass as a 3D vector, computed by transforming each link's local COM into the base frame via the cumulative product of the associated transformation matrices, weighting each by its mass, summing, and then dividing by the total mass. You may assume all matrices are valid rigid-body transforms (rotation + translation) and all inputs are pre-initialized. The function must be standalone (no external robotics libraries), use standard C++17 features, and accept inputs via `std::array` and a simple struct/class for 3D vectors and 4×4 matrices.

The solution iterates over the links in order from base to tip, maintaining a cumulative homogeneous transform `current` that starts as the first provided matrix (which already includes the base-to-link1 transform). For each link `i` (0 to 6), the local COM vector `com[i]` is transformed to the base frame by applying `current` rotation and translation: `global = current.rotation * com[i] + current.translation`. That global position is multiplied by `mass[i]` and accumulated into a running sum. Then, before processing the next link, `current` is updated to `current * nextTransform[i+1]` (the transform from link i+1 to link i+2, but since we already have base-to-link1, the product works out). After all links are processed, the sum is divided by the total mass and returned. Edge cases: if a mass is zero, its contribution is zero (fine); if total mass is zero, the result is undefined — we can document that or return a zero vector (we'll assume totalMass > 0 per problem statement). Time complexity is O(7) = O(1) since the chain length is fixed, but for a general n it would be O(n). Space is O(1) auxiliary (only a few temporary matrices/vectors). We must be careful with matrix multiplication order: for a point `p` in local frame of link i, its global coordinates = `T_base_i * p` where `T_base_i = T_base_1 * T_1_2 * ... * T_{i-1,i}`. Our cumulative product correctly represents `T_base_i` if we start with `T_base_1` and multiply on the right by each subsequent transform.

#include <array>
#include <cassert>

// Simple 3D vector
struct Vec3 {
    double x, y, z;
};

// Simple 4x4 homogeneous transformation matrix stored as 4 rows of 4 doubles
struct Mat4 {
    double m[4][4];
};

// Helper: multiply two 4x4 matrices (this * other)
Mat4 matMul(const Mat4& a, const Mat4& b) {
    Mat4 r;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            r.m[i][j] = 0.0;
            for (int k = 0; k < 4; ++k) {
                r.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return r;
}

// Transform a 3D point by a 4x4 matrix (rotation + translation)
Vec3 transformPoint(const Mat4& M, const Vec3& p) {
    // Assume last row is [0 0 0 1]
    Vec3 out;
    out.x = M.m[0][0]*p.x + M.m[0][1]*p.y + M.m[0][2]*p.z + M.m[0][3];
    out.y = M.m[1][0]*p.x + M.m[1][1]*p.y + M.m[1][2]*p.z + M.m[1][3];
    out.z = M.m[2][0]*p.x + M.m[2][1]*p.y + M.m[2][2]*p.z + M.m[2][3];
    return out;
}

// Compute whole-body COM for a 7-link serial chain.
// Parameters:
//   masses: array of 7 link masses
//   coms:   array of 7 link local COM positions
//   transforms: array of 7 matrices:
//            transforms[0] = base_to_link1
//            transforms[i] = link_i_to_link_{i+1} for i=1..6 (so transforms[1] = link1_to_link2, etc.)
//   totalMass: sum of all masses (must be > 0)
// Returns: global COM in base frame
Vec3 wholeBodyCOM(const std::array<double,7>& masses,
                  const std::array<Vec3,7>& coms,
                  const std::array<Mat4,7>& transforms,
                  double totalMass) {
    Vec3 sum = {0.0, 0.0, 0.0};
    Mat4 current = transforms[0];  // base to link1

    for (int i = 0; i < 7; ++i) {
        // Transform local COM of link i to base frame
        Vec3 global = transformPoint(current, coms[i]);
        // Accumulate mass-weighted position
        sum.x += masses[i] * global.x;
        sum.y += masses[i] * global.y;
        sum.z += masses[i] * global.z;
        // Update cumulative transform for next link (if not the last)
        if (i < 6) {
            current = matMul(current, transforms[i+1]);
        }
    }

    assert(totalMass > 0.0);
    Vec3 com;
    com.x = sum.x / totalMass;
    com.y = sum.y / totalMass;
    com.z = sum.z / totalMass;
    return com;
}

#include <cassert>
#include <cmath>

int main() {
    // All masses = 1, all local COMs = (0,0,0), so global COM = (0,0,0) regardless of transforms
    std::array<double,7> masses1 = {1,1,1,1,1,1,1};
    std::array<Vec3,7> coms1;
    for (auto& c : coms1) { c = {0,0,0}; }
    std::array<Mat4,7> transforms1;
    for (auto& M : transforms1) {
        // identity matrix
        for (int i=0;i<4;i++) for (int j=0;j<4;j++) M.m[i][j] = (i==j)?1.0:0.0;
    }
    Vec3 result1 = wholeBodyCOM(masses1, coms1, transforms1, 7.0);
    assert(std::abs(result1.x) < 1e-9 && std::abs(result1.y) < 1e-9 && std::abs(result1.z) < 1e-9);

    // Simple case: one unit mass at (1,0,0), others zero mass but totalMass = 1
    std::array<double,7> masses2 = {1,0,0,0,0,0,0};
    std::array<Vec3,7> coms2;
    coms2[0] = {1,0,0};
    for (int i=1;i<7;i++) coms2[i] = {0,0,0};
    // All transforms identity
    std::array<Mat4,7> transforms2;
    for (auto& M : transforms2) {
        for (int i=0;i<4;i++) for (int j=0;j<4;j++) M.m[i][j] = (i==j)?1.0:0.0;
    }
    Vec3 result2 = wholeBodyCOM(masses2, coms2, transforms2, 1.0);
    assert(std::abs(result2.x - 1.0) < 1e-9 && std::abs(result2.y) < 1e-9 && std::abs(result2.z) < 1e-9);

    // Two equal masses at different locations, offset by a translation
    std::array<double,7> masses3 = {1,1,0,0,0,0,0};
    std::array<Vec3,7> coms3;
    coms3[0] = {1,0,0}; // link1 local COM
    coms3[1] = {0,2,0}; // link2 local COM
    for (int i=2;i<7;i++) coms3[i] = {0,0,0};
    std::array<Mat4,7> transforms3;
    for (auto& M : transforms3) {
        for (int i=0;i<4;i++) for (int j=0;j<4;j++) M.m[i][j] = (i==j)?1.0:0.0;
    }
    // Add translation to first transform: base_to_link1 = translate by (0,0,0) but link1 local (1,0,0)
    // link1_to_link2 = translate by (0,0,0) so global for link2 local (0,2,0) is (0,2,0)
    // Sum masses: 1*(1,0,0) + 1*(0,2,0) = (1,2,0); totalMass=2 => COM = (0.5,1,0)
    Vec3 result3 = wholeBodyCOM(masses3, coms3, transforms3, 2.0);
    assert(std::abs(result3.x - 0.5) < 1e-9 && std::abs(result3.y - 1.0) < 1e-9 && std::abs(result3.z) < 1e-9);

    // Test with a non-identity rotation: rotate link2's local COM by 90 degrees about z
    std::array<double,7> masses4 = {0,1,0,0,0,0,0}; // only link2 has mass
    std::array<Vec3,7> coms4;
    for (auto& c : coms4) c = {0,0,0};
    coms4[1] = {1,0,0}; // local COM in link2 frame
    std::array<Mat4,7> transforms4;
    // identity for all except transforms[1] (link1_to_link2) which rotates 90° about z
    for (auto& M : transforms4) {
        for (int i=0;i<4;i++) for (int j=0;j<4;j++) M.m[i][j] = (i==j)?1.0:0.0;
    }
    // rotation 90° about z: cos=0, sin=1
    transforms4[1].m[0][0] = 0; transforms4[1].m[0][1] = -1; transforms4[1].m[0][2] = 0;
    transforms4[1].m[1][0] = 1; transforms4[1].m[1][1] = 0; transforms4[1].m[1][2] = 0;
    transforms4[1].m[0][3] = 0; transforms4[1].m[1][3] = 0; transforms4[1].m[2][3] = 0;
    // base_to_link1 = identity, link1_to_link2 = Rz(90°) => global COM of (1,0,0) in link2 = (0,1,0)
    Vec3 result4 = wholeBodyCOM(masses4, coms4, transforms4, 1.0);
    assert(std::abs(result4.x) < 1e-9 && std::abs(result4.y - 1.0) < 1e-9 && std::abs(result4.z) < 1e-9);

    return 0;
}
