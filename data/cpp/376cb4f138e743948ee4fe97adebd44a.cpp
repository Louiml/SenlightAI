Given a 4x4 grid of homogeneous 2D control points represented as a `std::array<std::array<std::array<double,3>,4>,4>` (where each inner array holds `{x, y, w}` with `w` normally 1.0), write a standalone C++ function that performs one level of Bézier patch subdivision along both parametric directions using the de Casteljau algorithm. The function should take the original patch and return a `std::array` of 4 new sub-patches, each of the same size, ordered as: bottom-left, bottom-right, top-left, top-right (where "bottom" corresponds to the first row and "left" to the first column in the given matrix indexing). Each sub-patch must have its `w` component set to 1.0 after subdivision. The function must be recursive-free, use only C++ standard library headers, and be safe for arbitrary control point coordinates (including negative values). Do not modify the input patch.

// The task requires subdividing a Bézier patch into four smaller patches by first subdividing each of the 4 rows (curves) in the u direction, then subdividing the resulting intermediate patches in the v direction. The de Casteljua algorithm for a cubic Bézier curve with control points `P0, P1, P2, P3` yields two new curves: left (`L0=P0, L1=(P0+P1)/2, L2=(L1+M)/2, L3=(L2+R1)/2`) and right (`R0=L3, R1=(M+R2)/2, R2=(P2+P3)/2, R3=P3`), where `M=(P1+P2)/2`. This is exactly the logic from the snippet's `divide_curve`.
//
// For a patch `p[4][4]`, we first apply this to each row `p[k]` to get two 4x4 patch halves `a` and `b` (each a set of 4 rows). Then we transpose `a` and `b` so that the columns become rows. After transposition, we again apply the curve subdivision to each row of the transposed matrices, producing `q`, `r` from `a`, and `s`, `t` from `b`. These four patches correspond to the four quadrants of the original, and after transposing them back (or equivalently by carefully mapping indices), we get the four sub-patches. The order in the task is: bottom-left, bottom-right, top-left, top-right. In the snippet, after the second subdivision, the patches `q`, `r`, `s`, `t` are already in the correct order when interpreted with original indexing: `q` is the bottom-left, `r` bottom-right, `s` top-left, `t` top-right. But note the snippet does not transpose back because the subdivision in v is equivalent to transposing, subdividing in u, then transposing back. However, for the final result, we must ensure the returned patches have the correct orientation. The simplest approach: after obtaining `q`, `r`, `s`, `t` (which are already in original orientation because we transposed before the second subdivision), we set each `w=1.0`. There's no need to transpose back because the second subdivision on transposed matrices yields correct orientation when you treat the rows as original rows.
//
// Edge cases: The input may have `w` values not equal to 1.0? The problem states "homogeneous 2D control points" with `w` normally 1.0. The subdivision formula assumes affine combinations with equal weights, so we should use the given coordinates directly (x,y) but the w value should be 1.0 after subdivision. We'll simply copy x,y from the originals and set w=1.0 for all resulting control points. No special handling for negative coordinates is needed because the arithmetic is linear. Time complexity: each curve subdivision does a few additions and divisions by 2, and we do 4 (rows) + 4 (after transpose) + 4 (after transpose for each half) = 12 curve subdivisions, each O(1). So total O(1) time and space.

#include <array>
#include <cstddef>

// Homogeneous 2D point: {x, y, w}
using Point2D = std::array<double, 3>;
// A cubic Bézier curve: 4 control points
using Curve = std::array<Point2D, 4>;
// A Bézier patch: 4 rows of 4 control points
using Patch = std::array<Curve, 4>;

// Subdivide a cubic Bézier curve into left and right halves.
// Both halves share the midpoint of the middle control point.
static void subdivideCurve(const Curve& c, Curve& left, Curve& right) {
    Point2D mid;
    mid[0] = (c[1][0] + c[2][0]) / 2.0;
    mid[1] = (c[1][1] + c[2][1]) / 2.0;
    mid[2] = 1.0; // not used, but keep consistent

    left[0] = c[0];
    left[1][0] = (c[0][0] + c[1][0]) / 2.0;
    left[1][1] = (c[0][1] + c[1][1]) / 2.0;
    left[2][0] = (left[1][0] + mid[0]) / 2.0;
    left[2][1] = (left[1][1] + mid[1]) / 2.0;

    right[3] = c[3];
    right[2][0] = (c[2][0] + c[3][0]) / 2.0;
    right[2][1] = (c[2][1] + c[3][1]) / 2.0;
    right[1][0] = (mid[0] + right[2][0]) / 2.0;
    right[1][1] = (mid[1] + right[2][1]) / 2.0;

    // Shared midpoint
    left[3][0] = right[0][0] = (left[2][0] + right[1][0]) / 2.0;
    left[3][1] = right[0][1] = (left[2][1] + right[1][1]) / 2.0;

    // Set w to 1.0 for all points
    for (int i = 0; i < 4; ++i) {
        left[i][2] = 1.0;
        right[i][2] = 1.0;
    }
}

// Transpose a 4x4 patch in place
static void transposePatch(Patch& p) {
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            Point2D temp = p[i][j];
            p[i][j] = p[j][i];
            p[j][i] = temp;
        }
    }
}

// Subdivide a Bézier patch into four sub-patches:
// bottom-left, bottom-right, top-left, top-right.
// Each sub-patch has w=1.0 for all points.
std::array<Patch, 4> subdividePatch(const Patch& p) {
    // Subdivide each row in u direction
    Curve a[4], b[4];
    for (int k = 0; k < 4; ++k) {
        subdivideCurve(p[k], a[k], b[k]);
    }

    // Build patches from these rows and transpose each
    Patch patchA, patchB;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            patchA[i][j] = a[i][j];
            patchB[i][j] = b[i][j];
        }
    }
    transposePatch(patchA);
    transposePatch(patchB);

    // Subdivide each row of the transposed patches in u direction
    Curve q[4], r[4], s[4], t[4];
    for (int k = 0; k < 4; ++k) {
        subdivideCurve(patchA[k], q[k], r[k]);
        subdivideCurve(patchB[k], s[k], t[k]);
    }

    // Build the four result patches (already in correct orientation)
    Patch result[4];
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            result[0][i][j] = q[i][j];
            result[1][i][j] = r[i][j];
            result[2][i][j] = s[i][j];
            result[3][i][j] = t[i][j];
        }
    }

    return {result[0], result[1], result[2], result[3]};
}

#include <cassert>
#include <cmath>
#include <array>
#include <iostream>

// Include the solution header (or copy the code above)
// For testing, we replicate the solution here inline.

// ... (solution code here) ...

int main() {
    // Test 1: Constant patch (all points same)
    Patch p;
    for (auto& row : p) {
        for (auto& pt : row) {
            pt = {2.0, 3.0, 1.0};
        }
    }
    auto res = subdividePatch(p);
    for (const auto& sub : res) {
        for (const auto& row : sub) {
            for (const auto& pt : row) {
                assert(std::fabs(pt[0] - 2.0) < 1e-9);
                assert(std::fabs(pt[1] - 3.0) < 1e-9);
                assert(pt[2] == 1.0);
            }
        }
    }

    // Test 2: A simple linear patch: x varies along rows, y varies along columns
    // Example: p[i][j] = {j, i, 1}
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            p[i][j] = {double(j), double(i), 1.0};
        }
    }
    res = subdividePatch(p);
    // Check w = 1.0 everywhere
    for (const auto& sub : res) {
        for (const auto& row : sub) {
            for (const auto& pt : row) {
                assert(pt[2] == 1.0);
            }
        }
    }
    // For a linear patch, after subdivision the corners should scale correctly.
    // Bottom-left sub-patch should have corners: (0,0), (1.5,0), (0,1.5), (1.5,1.5)
    // But let's check approximate values: the sub-patch control points lie on the original surface.
    // The boundary of bottom-left sub-patch: points along original u from 0 to 0.5, v from 0 to 0.5.
    // For our linear patch, the surface is x = u, y = v, so the bottom-left sub-patch should have x,y in [0,0.5].
    // The actual control points after subdivision are not exactly the corner points but we can check that the first point (p[0][0]) is (0,0).
    assert(std::fabs(res[0][0][0][0] - 0.0) < 1e-9);
    assert(std::fabs(res[0][0][0][1] - 0.0) < 1e-9);
    // The last point of bottom-right sub-patch should be (1,1) actually? The original patch has (1,1) at p[3][3].
    // After subdivision, the bottom-right sub-patch's top-right corner should be (1,1).
    assert(std::fabs(res[1][3][3][0] - 1.0) < 1e-9);
    assert(std::fabs(res[1][3][3][1] - 1.0) < 1e-9);

    // Test 3: Ensure input is not modified
    Patch original = p;
    res = subdividePatch(p);
    assert(p == original);

    // Test 4: Negative coordinates
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            p[i][j] = {double(-j), double(-i), 1.0};
        }
    }
    res = subdividePatch(p);
    // Bottom-left sub-patch's first point should be (0,0) still? Actually original p[0][0] = (0,0)
    // After subdivision, the bottom-left sub-patch's bottom-left control point stays at (0,0).
    assert(std::fabs(res[0][0][0][0] - 0.0) < 1e-9);
    assert(std::fabs(res[0][0][0][1] - 0.0) < 1e-9);
    // The top-right sub-patch's top-right point should be (-3,-3) because p[3][3] = (-3,-3)
    assert(std::fabs(res[3][3][3][0] + 3.0) < 1e-9);
    assert(std::fabs(res[3][3][3][1] + 3.0) < 1e-9);

    std::cout << "All tests passed.\n";
    return 0;
}
