/*
Write a standalone C++ function named `evaluateCubicBezierBasis` that takes a parameter `u` (a double, expected to be in the range [0,1] but not enforced), a pointer to a 4-element array `bs` (for basis values), and an optional pointer `db_dus` (which points to a 4-element array for derivatives). The function must compute and store in `bs[0]` through `bs[3]` the values of the four cubic Bernstein basis polynomials at `u`: \( B_0(u) = (1-u)^3 \), \( B_1(u) = 3u(1-u)^2 \), \( B_2(u) = 3u^2(1-u) \), and \( B_3(u) = u^3 \). If `db_dus` is not `nullptr`, the function must also compute and store in the dereferenced array the first derivatives with respect to `u` for each basis function: \( \frac{dB_0}{du} = -3(1-u)^2 \), \( \frac{dB_1}{du} = 3(1-u)(1-3u) \), \( \frac{dB_2}{du} = 6u(1-u) - 3u^2 \), and \( \frac{dB_3}{du} = 3u^2 \). The function must be `const`-correct, take the output arrays as raw pointers (not references), and use only standard library headers if needed—but no external dependencies. The implementation should avoid using any precomputed global constants; compute everything directly from `u` using simple arithmetic. Edge cases include `u = 0` (where `B0=1`, others=0) and `u = 1` (where `B3=1`, others=0); the function must handle these correctly without special casing. The time complexity must be O(1) and auxiliary space O(1). The function must be declared with `const` on the method signature if written as a member, but since this is a free function, ensure that it does not modify any external state and that parameters `bs` and `db_dus` are written to only, never read from.
*/
#include <cstddef>

// Evaluate the four cubic Bernstein basis polynomials and optionally their
// derivatives at parameter u.
// bs[4] must be a valid array of four doubles; if db_dus is not nullptr,
// *(db_dus) must also point to a valid array of four doubles.
void evaluateCubicBezierBasis(double u, double bs[4], double (*db_dus)[4]) {
    // Compute powers of u for efficiency and clarity
    const double u2 = u * u;
    const double u3 = u2 * u;

    // Bernstein basis values (expanded forms)
    bs[0] = 1.0 - 3.0 * u + 3.0 * u2 - u3;          // (1-u)^3
    bs[1] = 3.0 * u - 6.0 * u2 + 3.0 * u3;         // 3u(1-u)^2
    bs[2] = 3.0 * u2 - 3.0 * u3;                  // 3u^2(1-u)
    bs[3] = u3;                                   // u^3

    // Derivatives if requested
    if (db_dus != nullptr) {
        (*db_dus)[0] = -3.0 + 6.0 * u - 3.0 * u2;   // -3(1-u)^2
        (*db_dus)[1] = 3.0 - 12.0 * u + 9.0 * u2;   // 3(1-u)(1-3u)
        (*db_dus)[2] = 6.0 * u - 9.0 * u2;          // 6u(1-u)-3u^2
        (*db_dus)[3] = 3.0 * u2;                    // 3u^2
    }
}
#include <cassert>
#include <cmath>

// forward declaration if needed; but for test we assume solution is included above
void evaluateCubicBezierBasis(double u, double bs[4], double (*db_dus)[4]);

int main() {
    // Test at u=0
    double bs[4], db[4];
    evaluateCubicBezierBasis(0.0, bs, &db);
    assert(fabs(bs[0] - 1.0) < 1e-12);
    assert(fabs(bs[1]) < 1e-12);
    assert(fabs(bs[2]) < 1e-12);
    assert(fabs(bs[3]) < 1e-12);
    assert(fabs(db[0] + 3.0) < 1e-12);
    assert(fabs(db[1] - 3.0) < 1e-12);
    assert(fabs(db[2]) < 1e-12);
    assert(fabs(db[3]) < 1e-12);

    // Test at u=1
    evaluateCubicBezierBasis(1.0, bs, &db);
    assert(fabs(bs[0]) < 1e-12);
    assert(fabs(bs[1]) < 1e-12);
    assert(fabs(bs[2]) < 1e-12);
    assert(fabs(bs[3] - 1.0) < 1e-12);
    assert(fabs(db[0]) < 1e-12);
    assert(fabs(db[1] + 3.0) < 1e-12);
    assert(fabs(db[2] - 3.0) < 1e-12);
    assert(fabs(db[3] - 3.0) < 1e-12);

    // Test at u=0.5: known values sum to 1 and derivatives sum to 0
    evaluateCubicBezierBasis(0.5, bs, &db);
    double sum = 0.0, dsum = 0.0;
    for (int i = 0; i < 4; ++i) {
        sum += bs[i];
        dsum += db[i];
    }
    assert(fabs(sum - 1.0) < 1e-12);
    assert(fabs(dsum) < 1e-12);
    // Specific values at u=0.5: B0=1/8, B1=3/8, B2=3/8, B3=1/8
    assert(fabs(bs[0] - 0.125) < 1e-12);
    assert(fabs(bs[1] - 0.375) < 1e-12);
    assert(fabs(bs[2] - 0.375) < 1e-12);
    assert(fabs(bs[3] - 0.125) < 1e-12);

    // Test derivative with nullptr (should not crash)
    evaluateCubicBezierBasis(0.3, bs, nullptr);
    assert(fabs(bs[0] + bs[1] + bs[2] + bs[3] - 1.0) < 1e-12);

    return 0;
}
// The solution approach is to directly implement the closed-form polynomial expansions of the Bernstein basis functions and their derivatives. For clarity and correct implementation, we expand each basis function in powers of `u` to avoid any floating-point cancellation issues that could arise from subtracting large near-equal numbers (though for single evaluations it’s fine). The basis functions are:
// - \( B_0 = (1-u)^3 = 1 - 3u + 3u^2 - u^3 \)
// - \( B_1 = 3u(1-u)^2 = 3u - 6u^2 + 3u^3 \)
// - \( B_2 = 3u^2(1-u) = 3u^2 - 3u^3 \)
// - \( B_3 = u^3 \)
// Derivatives:
// - \( B_0' = -3(1-u)^2 = -3 + 6u - 3u^2 \)
// - \( B_1' = 3(1-u)(1-3u) \) expand to \( 3 - 12u + 9u^2 \)
// - \( B_2' = 6u(1-u) - 3u^2 = 6u - 9u^2 \)
// - \( B_3' = 3u^2 \)
// To compute each value, we first compute `u2 = u*u` and `u3 = u2*u`. Then fill the `bs` array using the expanded formulas. If `db_dus` is not null, we compute the derivative formulas similarly. We must be careful with signs and coefficients; a simple way to verify is to test with `u=0` and `u=1`, where the sums of basis functions equal 1 and the derivatives have known properties (e.g., sum of derivatives is 0). The algorithm is O(1) time and O(1) space. Edge cases: `u` outside [0,1] is allowed by the task (no clamping), so the formulas hold for all real `u`. Ensure no division by zero (none). Use `const` on the function signature if wanted, but it's a free function so mark parameters as non-const pointers to write to. The reference solution will be self-contained with no `main` function; the test section provides a separate `main` with asserts.
