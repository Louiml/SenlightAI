// Write a standalone C++ function `dualQuaternionSkinning` that deforms a set of vertices `V` (an `Nx3` matrix of 3D points) using a linear blend of dual quaternion transformations. The function takes as input: `V` (input vertices), `W` (an `NxM` weight matrix where each row sums to 1 and each column corresponds to a bone), `vQ` (a vector of `M` unit quaternions representing bone rotations), and `vT` (a vector of `M` 3D translation vectors representing bone translations). The output `U` must be a matrix of the same size as `V` containing the deformed vertices. For each vertex, the algorithm must: (1) convert each rotation-translation pair into a dual quaternion (real part = rotation quaternion, dual part = `0.5 * t * q` using quaternion multiplication where `t` is treated as a pure quaternion `(0, tx, ty, tz)`), (2) blend the real and dual parts of all dual quaternions using the vertex's weights, (3) normalize the blended real part to unit norm and set the blended dual part accordingly (using the formula `be' = (be * b0_conj) / |b0|^2` or the shorter equivalent `be' = be / |b0|` after normalizing b0 to unit norm before computing the division, as in the reference snippet), and (4) apply the resulting unit dual quaternion to the vertex using the dual quaternion transformation formula: `v' = v + 2 * cross(r, cross(r, v) + real_w * v) + 2 * (real_w * dual_v - dual_w * r + cross(r, dual_v))`, where `r = (x, y, z)` of the real part, `real_w` is the scalar part, and `dual_v` is the vector part of the dual part. The function must handle arbitrary numbers of vertices and bones, ensure correct output sizing, and be robust to zero-weight rows (where the blended quaternion may be zero; in that case leave the vertex unchanged).

// The solution follows the dual quaternion blending algorithm from Kavan et al. The core steps are: for each bone `c`, build the dual quaternion `D_c = (q_c, d_c)` where `d_c` is computed from the translation vector `t_c` and rotation quaternion `q_c` using the formula: `d.w = -0.5*(t.x*q.x + t.y*q.y + t.z*q.z)`, `d.x = 0.5*(t.x*q.w + t.y*q.z - t.z*q.y)`, `d.y = 0.5*(-t.x*q.z + t.y*q.w + t.z*q.x)`, `d.z = 0.5*(t.x*q.y - t.y*q.x + t.z*q.w)`. This matches `d = 0.5 * t * q` in quaternion multiplication where `t` is a pure quaternion. For each vertex `i`, accumulate the weighted sum of real parts `b0 = sum_c w_ic * q_c` and dual parts `be = sum_c w_ic * d_c`. Then normalize the real part to unit norm: `c0 = b0 / |b0|`. The dual part is corrected by dividing by the norm of `b0`: `ce = be / |b0|`. This yields a unit dual quaternion `(c0, ce)`. Then apply the transformation to the vertex: using the formula from the snippet, compute `v = V.row(i)`, `d0 = c0.vec()`, `de = ce.vec()`, `a0 = c0.w()`, `ae = ce.w()`, and then `U.row(i) = v + 2*d0.cross(d0.cross(v) + a0*v) + 2*(a0*de - ae*d0 + d0.cross(de))`. Edge case: If all weights for a vertex are zero, then `b0` is a zero quaternion, and division by zero occurs. To avoid this, check if `b0.norm()` is near zero (e.g., epsilon) and leave the vertex unchanged. Also ensure the input matrices have compatible sizes: `V.rows() == W.rows()` and `W.cols() == vQ.size() == vT.size()`. The time complexity is O(N * M) due to the nested loops over vertices and bones, and space complexity is O(N * 3) for the output matrix plus O(M) for the dual quaternion storage. The implementation uses Eigen for matrix and quaternion operations, which simplify vector math.

#include <Eigen/Dense>
#include <vector>
#include <cassert>

// Deform vertices V using dual quaternion skinning with given weights, rotations, and translations.
// V: Nx3 matrix of input vertices.
// W: NxM weight matrix, each row sums to 1.
// vQ: vector of M unit quaternions (rotations).
// vT: vector of M 3D translation vectors.
// U: output Nx3 matrix of deformed vertices (same size as V).
template <typename DerivedV, typename DerivedW, typename DerivedU>
void dualQuaternionSkinning(
    const Eigen::MatrixBase<DerivedV>& V,
    const Eigen::MatrixBase<DerivedW>& W,
    const std::vector<Eigen::Quaternion<typename DerivedV::Scalar>>& vQ,
    const std::vector<Eigen::Matrix<typename DerivedV::Scalar, 3, 1>>& vT,
    Eigen::PlainObjectBase<DerivedU>& U)
{
    using Scalar = typename DerivedV::Scalar;
    using Vec3 = Eigen::Matrix<Scalar, 3, 1>;
    using Quat = Eigen::Quaternion<Scalar>;

    assert(V.rows() == W.rows());
    assert(W.cols() == static_cast<int>(vQ.size()));
    assert(W.cols() == static_cast<int>(vT.size()));

    U.resizeLike(V);
    const int nv = static_cast<int>(V.rows());
    const int nc = static_cast<int>(W.cols());

    // Convert each (rotation, translation) into a dual quaternion (real = q, dual = d).
    std::vector<Quat> vD(nc);
    for (int c = 0; c < nc; ++c) {
        const Quat& q = vQ[c];
        const Vec3& t = vT[c];
        vD[c].w() = -0.5 * (t(0) * q.x() + t(1) * q.y() + t(2) * q.z());
        vD[c].x() =  0.5 * (t(0) * q.w() + t(1) * q.z() - t(2) * q.y());
        vD[c].y() =  0.5 * (-t(0) * q.z() + t(1) * q.w() + t(2) * q.x());
        vD[c].z() =  0.5 * (t(0) * q.y() - t(1) * q.x() + t(2) * q.w());
    }

    const Scalar eps = Scalar(1e-12);

    for (int i = 0; i < nv; ++i) {
        Quat b0(0, 0, 0, 0);
        Quat be(0, 0, 0, 0);

        for (int c = 0; c < nc; ++c) {
            Scalar w = W(i, c);
            b0.coeffs() += w * vQ[c].coeffs();
            be.coeffs() += w * vD[c].coeffs();
        }

        // If the blended real part is near zero, leave the vertex unchanged.
        Scalar norm_b0 = b0.norm();
        if (norm_b0 < eps) {
            U.row(i) = V.row(i);
            continue;
        }

        // Normalize the real part to unit quaternion.
        Quat c0 = b0;
        c0.coeffs() /= norm_b0;

        // Adjust dual part by dividing by the norm of b0 (since c0 is unit).
        Quat ce = be;
        ce.coeffs() /= norm_b0;

        // Apply the dual quaternion transformation: v' = v + 2*cross(r, cross(r, v) + a0*v) + 2*(a0*de - ae*r + cross(r, de))
        Vec3 v = V.row(i);
        Vec3 d0 = c0.vec();
        Vec3 de = ce.vec();
        Scalar a0 = c0.w();
        Scalar ae = ce.w();

        Vec3 result = v + Scalar(2) * d0.cross(d0.cross(v) + a0 * v)
                          + Scalar(2) * (a0 * de - ae * d0 + d0.cross(de));
        U.row(i) = result;
    }
}

#include <Eigen/Dense>
#include <vector>
#include <cassert>
#include <cmath>

// Include the solution function (or paste it above).
// For brevity, we assume the function is defined above.

int main() {
    using Scalar = double;
    using Vec3 = Eigen::Matrix<Scalar, 3, 1>;
    using Quat = Eigen::Quaternion<Scalar>;
    using MatX3 = Eigen::Matrix<Scalar, Eigen::Dynamic, 3>;

    // Test 1: Single bone, identity rotation, no translation => unchanged.
    {
        MatX3 V(2, 3);
        V << 1, 2, 3,
             4, 5, 6;
        MatX3 W(2, 1);
        W << 1.0,
             1.0;
        std::vector<Quat> vQ(1, Quat(1, 0, 0, 0)); // identity
        std::vector<Vec3> vT(1, Vec3(0, 0, 0));
        MatX3 U;
        dualQuaternionSkinning(V, W, vQ, vT, U);
        assert(U.isApprox(V, 1e-12));
    }

    // Test 2: Single bone, translation only => vertex shifted by translation.
    {
        MatX3 V(1, 3);
        V << 0, 0, 0;
        MatX3 W(1, 1);
        W << 1.0;
        std::vector<Quat> vQ(1, Quat(1, 0, 0, 0));
        std::vector<Vec3> vT(1, Vec3(1, 2, 3));
        MatX3 U;
        dualQuaternionSkinning(V, W, vQ, vT, U);
        assert(U.isApprox((MatX3() << 1, 2, 3).finished(), 1e-12));
    }

    // Test 3: Single bone, 90-degree rotation about Z axis.
    {
        MatX3 V(1, 3);
        V << 1, 0, 0;
        MatX3 W(1, 1);
        W << 1.0;
        Quat q; q = Eigen::AngleAxis<Scalar>(M_PI / 2, Vec3(0, 0, 1));
        std::vector<Quat> vQ(1, q);
        std::vector<Vec3> vT(1, Vec3(0, 0, 0));
        MatX3 U;
        dualQuaternionSkinning(V, W, vQ, vT, U);
        MatX3 expected(1, 3);
        expected << 0, 1, 0; // cos(90)=0, sin(90)=1
        assert(U.isApprox(expected, 1e-12));
    }

    // Test 4: Two bones with equal weights 0.5 each, one identity and one translation.
    {
        MatX3 V(1, 3);
        V << 0, 0, 0;
        MatX3 W(1, 2);
        W << 0.5, 0.5;
        std::vector<Quat> vQ;
        vQ.push_back(Quat(1, 0, 0, 0)); // identity
        vQ.push_back(Quat(1, 0, 0, 0)); // identity
        std::vector<Vec3> vT;
        vT.push_back(Vec3(0, 0, 0));
        vT.push_back(Vec3(2, 0, 0));
        MatX3 U;
        dualQuaternionSkinning(V, W, vQ, vT, U);
        // Each dual quaternion: (1, d) with d = (0, t/2). Blend gives dual part (0, (0+2)/4 = 0.5) => translation (0,1,0)
        MatX3 expected(1, 3);
        expected << 1, 0, 0;
        assert(U.isApprox(expected, 1e-12));
    }

    // Test 5: Zero-weight row leaves vertex unchanged.
    {
        MatX3 V(1, 3);
        V << 5, -1, 2;
        MatX3 W(1, 2);
        W << 0.0, 0.0;
        std::vector<Quat> vQ(2, Quat(1, 0, 0, 0));
        std::vector<Vec3> vT(2, Vec3(10, 10, 10));
        MatX3 U;
        dualQuaternionSkinning(V, W, vQ, vT, U);
        assert(U.isApprox(V, 1e-12));
    }

    // Test 6: Rotation and translation combined (single bone) matches manual rotation then translation.
    {
        MatX3 V(1, 3);
        V << 1, 0, 0;
        MatX3 W(1, 1);
        W << 1.0;
        Quat q; q = Eigen::AngleAxis<Scalar>(M_PI / 2, Vec3(0, 0, 1));
        std::vector<Quat> vQ(1, q);
        std::vector<Vec3> vT(1, Vec3(0, 2, 0));
        MatX3 U;
        dualQuaternionSkinning(V, W, vQ, vT, U);
        // Rotate (1,0,0) by 90° around Z gives (0,1,0), then translate by (0,2,0) gives (0,3,0)
        MatX3 expected(1, 3);
        expected << 0, 3, 0;
        assert(U.isApprox(expected, 1e-12));
    }

    return 0;
}
