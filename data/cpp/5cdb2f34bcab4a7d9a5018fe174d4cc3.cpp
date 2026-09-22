Write a C++ function `double orient2d(double ax, double ay, double bx, double by, double cx, double cy)` that computes the exact sign of the 2D orientation determinant of three points: it returns a positive value if the points `(ax,ay)`, `(bx,by)`, and `(cx,cy)` are in counterclockwise order, a negative value if they are in clockwise order, and zero if they are collinear. The function must be robust against floating-point rounding errors, meaning that for arbitrary double-precision inputs (including very close or collinear points), the sign of the returned value must always be correct, even when the naive determinant `(bx-ax)*(cy-ay) - (by-ay)*(cx-ax)` evaluates to zero or has the wrong sign due to cancellation. The magnitude of the return value should be a reasonable approximation of twice the signed area of the triangle, but only the sign is guaranteed to be exact. The function must be self-contained (no external libraries beyond the C++ standard library) and must not rely on `long double` or arbitrary-precision arithmetic; instead, it should implement a robust adaptive exact predicate using error-free transformations and expansion arithmetic similar to Shewchuk's approach.
The naive determinant formula `det = (bx-ax)*(cy-ay) - (by-ay)*(cx-ax)` is vulnerable to catastrophic cancellation when the points are nearly collinear or when the coordinates are large and close together. The robust solution computes the determinant using an adaptive strategy: first, compute the determinant naively and estimate an error bound based on the sum of absolute values of the intermediate products. If the determinant is clearly larger than the error bound (in magnitude), return it immediately—this is the common fast path. Otherwise, compute a more accurate result using error-free transformations: split each input into high and low parts using a `splitter` constant (2^27+1 for double), then compute the exact products and differences using `Two_Product` and `Two_Two_Diff` macros that produce multi-component expansions representing the exact value. The exact determinant is expressed as an expansion of up to 16 components, and we sum these components with a fast expansion summation algorithm that eliminates zero components to avoid noise from floating-point arithmetic. If after adding the first-order correction term (involving the tails of the differences) the result is still within the error bound, we compute the full exact expansion from the product of the difference tails and return the highest-order component, which is the exact value. The key edge cases are: exactly collinear points (must return exactly 0.0, not a tiny nonzero), points where the naive determinant overflows or underflows (mitigated by using the expansion for the final answer), and points where all differences are exactly representable (the function returns the naive result immediately). Time complexity is O(1) for the fast path and O(1) with a larger constant for the slow path (since the expansions are of fixed maximum length); space complexity is O(1) for fixed-size arrays.
#include <cmath>
#include <cstdlib>

// -----------------------------------------------------------------------------
// Robust 2D orientation test (adaptive exact predicate).
// Returns >0 if (ax,ay),(bx,by),(cx,cy) are counterclockwise, <0 if clockwise,
// and 0 if collinear. The sign is always exact; magnitude approximates twice area.
// -----------------------------------------------------------------------------

namespace robust_orient {

// Constants for splitting doubles (2^27+1 for double precision).
static const double splitter = 134217729.0;  // 2^27 + 1
static const double epsilon = 2.220446049250313e-16; // 2^-52
static const double resulterrbound = (3.0 + 8.0 * epsilon) * epsilon;
static const double ccwerrboundA = (3.0 + 16.0 * epsilon) * epsilon;
static const double ccwerrboundB = (2.0 + 12.0 * epsilon) * epsilon;
static const double ccwerrboundC = (9.0 + 64.0 * epsilon) * epsilon * epsilon;

inline double absolute(double a) { return a >= 0.0 ? a : -a; }

// Error-free sum: x = a+b, y = exact roundoff error.
inline void two_sum(double a, double b, double& x, double& y) {
    x = a + b;
    double bvirt = x - a;
    double avirt = x - bvirt;
    double bround = b - bvirt;
    double around = a - avirt;
    y = around + bround;
}

// Error-free difference: x = a-b, y = exact roundoff error.
inline void two_diff(double a, double b, double& x, double& y) {
    x = a - b;
    double bvirt = a - x;
    double avirt = x + bvirt;
    double bround = bvirt - b;
    double around = a - avirt;
    y = around + bround;
}

// Split a into high and low parts.
inline void split(double a, double& ahi, double& alo) {
    double c = splitter * a;
    double abig = c - a;
    ahi = c - abig;
    alo = a - ahi;
}

// Error-free product: x = a*b, y = exact roundoff error.
inline void two_product(double a, double b, double& x, double& y) {
    x = a * b;
    double ahi, alo, bhi, blo;
    split(a, ahi, alo);
    split(b, bhi, blo);
    double err1 = x - (ahi * bhi);
    double err2 = err1 - (alo * bhi);
    double err3 = err2 - (ahi * blo);
    y = (alo * blo) - err3;
}

// Difference of two 2-component expansions: (a1,a0) - (b1,b0) => (x3,x2,x1,x0)
inline void two_two_diff(double a1, double a0, double b1, double b0,
                         double& x3, double& x2, double& x1, double& x0) {
    double _j, _0;
    two_diff(a0, b0, _j, x0);
    two_diff(a1, b1, x3, _0);
    two_sum(_0, _j, x2, x1);
}

// Fast expansion sum of two 4-component expansions, eliminating zeros.
inline int fast_expansion_sum_zeroelim(int elen, const double* e,
                                       int flen, const double* f,
                                       double* h) {
    double Q;
    double Qnew;
    double hh;
    int eindex, findex, hindex;
    double enow, fnow;

    enow = e[0];
    fnow = f[0];
    eindex = findex = 0;
    if ((fnow > enow) == (fnow > -enow)) {
        Q = enow;
        enow = e[++eindex];
    } else {
        Q = fnow;
        fnow = f[++findex];
    }
    hindex = 0;
    if ((eindex < elen) && (findex < flen)) {
        if ((fnow > enow) == (fnow > -enow)) {
            double bvirt = enow - Q;
            hh = enow - (Q + bvirt);
            Qnew = Q + enow;
            enow = e[++eindex];
        } else {
            double bvirt = fnow - Q;
            hh = fnow - (Q + bvirt);
            Qnew = Q + fnow;
            fnow = f[++findex];
        }
        Q = Qnew;
        if (hh != 0.0) {
            h[hindex++] = hh;
        }
        while ((eindex < elen) && (findex < flen)) {
            if ((fnow > enow) == (fnow > -enow)) {
                two_sum(Q, enow, Qnew, hh);
                enow = e[++eindex];
            } else {
                two_sum(Q, fnow, Qnew, hh);
                fnow = f[++findex];
            }
            Q = Qnew;
            if (hh != 0.0) {
                h[hindex++] = hh;
            }
        }
    }
    while (eindex < elen) {
        two_sum(Q, enow, Qnew, hh);
        enow = e[++eindex];
        Q = Qnew;
        if (hh != 0.0) {
            h[hindex++] = hh;
        }
    }
    while (findex < flen) {
        two_sum(Q, fnow, Qnew, hh);
        fnow = f[++findex];
        Q = Qnew;
        if (hh != 0.0) {
            h[hindex++] = hh;
        }
    }
    if ((Q != 0.0) || (hindex == 0)) {
        h[hindex++] = Q;
    }
    return hindex;
}

// Compute the approximate sum of an expansion.
inline double estimate(int elen, const double* e) {
    double Q = e[0];
    for (int i = 1; i < elen; i++) {
        Q += e[i];
    }
    return Q;
}

// Exact tail computation for a difference: given a-b = x (rounded), compute y such that a-b = x+y.
inline void two_diff_tail(double a, double b, double x, double& y) {
    double bvirt = a - x;
    double avirt = x + bvirt;
    double bround = bvirt - b;
    double around = a - avirt;
    y = around + bround;
}

// Two_One_Sum: add scalar to 2-component expansion.
inline void two_one_sum(double a1, double a0, double b,
                        double& x2, double& x1, double& x0) {
    double _i;
    two_sum(a0, b, _i, x0);
    two_sum(a1, _i, x2, x1);
}

// Two_One_Diff: subtract scalar from 2-component expansion.
inline void two_one_diff(double a1, double a0, double b,
                         double& x2, double& x1, double& x0) {
    double _i;
    two_diff(a0, b, _i, x0);
    two_sum(a1, _i, x2, x1);
}

} // namespace robust_orient

// Public function: exact 2D orientation test.
double orient2d(double ax, double ay, double bx, double by, double cx, double cy) {
    using namespace robust_orient;

    double acx = ax - cx;
    double bcx = bx - cx;
    double acy = ay - cy;
    double bcy = by - cy;

    double detleft, detright;
    double detlefttail, detrighttail;
    two_product(acx, bcy, detleft, detlefttail);
    two_product(acy, bcx, detright, detrighttail);

    double B3, B2, B1, B0;
    two_two_diff(detleft, detlefttail, detright, detrighttail, B3, B2, B1, B0);
    double B[4] = {B0, B1, B2, B3};

    double det = estimate(4, B);
    double detsum = absolute(detleft) + absolute(detright);
    double errbound = ccwerrboundB * detsum;
    if ((det >= errbound) || (-det >= errbound)) {
        return det;
    }

    // Compute tails of differences.
    double acxtail, acytail, bcxtail, bcytail;
    two_diff_tail(ax, cx, acx, acxtail);
    two_diff_tail(bx, cx, bcx, bcxtail);
    two_diff_tail(ay, cy, acy, acytail);
    two_diff_tail(by, cy, bcy, bcytail);

    if ((acxtail == 0.0) && (acytail == 0.0) &&
        (bcxtail == 0.0) && (bcytail == 0.0)) {
        return det;
    }

    errbound = ccwerrboundC * detsum + resulterrbound * absolute(det);
    det += (acx * bcytail + bcy * acxtail) -
           (acy * bcxtail + bcx * acytail);
    if ((det >= errbound) || (-det >= errbound)) {
        return det;
    }

    // Compute the exact result using expansions.
    double s1, s0, t1, t0;
    double u3, u2, u1, u0;
    double C1[8];
    int C1length;

    two_product(acxtail, bcy, s1, s0);
    two_product(acytail, bcx, t1, t0);
    two_two_diff(s1, s0, t1, t0, u3, u2, u1, u0);
    double u[4] = {u0, u1, u2, u3};
    C1length = fast_expansion_sum_zeroelim(4, B, 4, u, C1);

    double C2[12];
    int C2length;
    two_product(acx, bcytail, s1, s0);
    two_product(acy, bcxtail, t1, t0);
    two_two_diff(s1, s0, t1, t0, u3, u2, u1, u0);
    double u2arr[4] = {u0, u1, u2, u3};
    C2length = fast_expansion_sum_zeroelim(C1length, C1, 4, u2arr, C2);

    double D[16];
    int Dlength;
    two_product(acxtail, bcytail, s1, s0);
    two_product(acytail, bcxtail, t1, t0);
    two_two_diff(s1, s0, t1, t0, u3, u2, u1, u0);
    double u3arr[4] = {u0, u1, u2, u3};
    Dlength = fast_expansion_sum_zeroelim(C2length, C2, 4, u3arr, D);

    return D[Dlength - 1];
}
#include <cassert>
#include <cmath>

// Declaration of the solution function.
double orient2d(double ax, double ay, double bx, double by, double cx, double cy);

int main() {
    // CCW order: (0,0), (1,0), (0,1) -> positive
    assert(orient2d(0.0, 0.0, 1.0, 0.0, 0.0, 1.0) > 0.0);

    // CW order: swap two points -> negative
    assert(orient2d(0.0, 0.0, 0.0, 1.0, 1.0, 0.0) < 0.0);

    // Collinear points -> exactly zero
    assert(orient2d(1.0, 2.0, 3.0, 6.0, 5.0, 10.0) == 0.0);

    // Nearly collinear points with extreme values (must have correct sign)
    double huge = 1e15;
    double eps = 1e-3;
    // Points: (0,0), (huge, huge+eps), (1,1) slightly clockwise
    assert(orient2d(0.0, 0.0, huge, huge + eps, 1.0, 1.0) < 0.0);
    // Points: (0,0), (huge, huge-eps), (1,1) slightly counterclockwise
    assert(orient2d(0.0, 0.0, huge, huge - eps, 1.0, 1.0) > 0.0);

    // Degenerate line where naive determinant is exactly zero but points are not collinear
    // Points: (1,1), (2,2), (1+1e-16, 1-1e-16) - extremely small but nonzero determinant
    assert(orient2d(1.0, 1.0, 2.0, 2.0, 1.0 + 1e-16, 1.0 - 1e-16) != 0.0);

    // Symmetric triangle area check (magnitude ~ 1.0 for unit square)
    assert(std::fabs(orient2d(0.0, 0.0, 1.0, 0.0, 0.0, 1.0) - 1.0) < 1e-12);

    // Negative coordinates
    assert(orient2d(-3.0, -2.0, 0.0, 1.0, 2.0, -1.0) > 0.0);

    // All identical points -> zero
    assert(orient2d(0.5, 0.5, 0.5, 0.5, 0.5, 0.5) == 0.0);

    // Large coordinates with cancellation-prone layout
    double a1 = 1e16, a2 = 1e16 + 1;
    double b1 = 2e16, b2 = 2e16 + 2;
    double c1 = 3e16, c2 = 3e16 + 3;
    assert(orient2d(a1, a2, b1, b2, c1, c2) == 0.0); // collinear

    return 0;
}
