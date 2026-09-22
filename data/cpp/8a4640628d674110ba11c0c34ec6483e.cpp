// Write a C++ function named `print_magnetic_fields` that takes six parameters: three positive integer dimensions `cz`, `cxm`, and `cym` (representing the maximum indices in the z, x, and y directions respectively), and four handle pointers to 3D arrays of type `double`—`bza`, `ex`, `ey`, and `hz`—each allocated with size `(cxm+1) × (cym+1) × (cz+1)`. The function must output the values from all four arrays using the interleaved format: for each `i` from 0 to `cz`, for each `j` from 0 to `cym`, for each `k` from 0 to `cxm`, print in order: `bza[k][j][i]`, then `ex[k][j][i]`, then `ey[k][j][i]`, then `hz[k][j][i]`. Each value must be printed with a fixed precision of 6 decimal places, separated by exactly one space after each value. A newline must be inserted after every 20 values printed (counting across all arrays and all iterations), and after completing all loops, an extra newline is printed. Ensure the function is `const`-correct: the arrays are read-only, and the output formatting is set inside the function before the loops.

// The solution involves iterating through three nested loops in the exact order specified: outer loop over `i` (z-index), middle over `j` (y-index), inner over `k` (x-index). For each iteration, print four values from the four arrays using the given indexing order `[k][j][i]`. To manage newline insertion every 20 values, maintain a counter that increments after each printed value (i.e., after each of the four values). When the counter reaches 20, print a newline and reset the counter to 0. The function should set `std::cout << std::fixed << std::setprecision(6)` at the beginning to ensure fixed-point notation with six decimal places. Edge cases include empty arrays? The task assumes positive dimensions, so arrays have at least one element. If any dimension is zero, the loops simply do not execute, but dimensions are positive per spec. The time complexity is \(O(cz \cdot cxm \cdot cym)\) because each of the four arrays has that many elements, and we print each exactly once—total printed values = \(4 \times (cz+1)(cxm+1)(cym+1)\). Space complexity is \(O(1)\) auxiliary, ignoring the input arrays themselves. Const-correctness: parameters for the 3D arrays should be `const double* const* const*` or use `const` in the pointer declarations to indicate the data is not modified.

#include <iostream>
#include <iomanip>

// Print interleaved values from four 3D arrays with fixed precision and line breaks.
void print_magnetic_fields(int cz, int cxm, int cym,
                           const double* const* const* bza,
                           const double* const* const* ex,
                           const double* const* const* ey,
                           const double* const* const* hz) {
    std::cout << std::fixed << std::setprecision(6);
    int value_count = 0;

    for (int i = 0; i <= cz; ++i) {
        for (int j = 0; j <= cym; ++j) {
            for (int k = 0; k <= cxm; ++k) {
                std::cout << bza[k][j][i] << ' ';
                ++value_count;
                if (value_count % 20 == 0) std::cout << '\n';

                std::cout << ex[k][j][i] << ' ';
                ++value_count;
                if (value_count % 20 == 0) std::cout << '\n';

                std::cout << ey[k][j][i] << ' ';
                ++value_count;
                if (value_count % 20 == 0) std::cout << '\n';

                std::cout << hz[k][j][i] << ' ';
                ++value_count;
                if (value_count % 20 == 0) std::cout << '\n';
            }
        }
    }
    std::cout << '\n';
}

#include <cassert>
#include <sstream>
#include <string>

// Helper to capture output from the function
std::string capture_output(int cz, int cxm, int cym,
                           const double* const* const* bza,
                           const double* const* const* ex,
                           const double* const* const* ey,
                           const double* const* const* hz) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    print_magnetic_fields(cz, cxm, cym, bza, ex, ey, hz);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // Test 1: Small dimensions, known values
    int cz = 0, cxm = 0, cym = 0;
    double bza[1][1][1] = {{{1.0}}};
    double ex[1][1][1] = {{{2.0}}};
    double ey[1][1][1] = {{{3.0}}};
    double hz[1][1][1] = {{{4.0}}};
    auto out = capture_output(cz, cxm, cym,
                              (const double* const* const*)bza,
                              (const double* const* const*)ex,
                              (const double* const* const*)ey,
                              (const double* const* const*)hz);
    assert(out == "1.000000 2.000000 3.000000 4.000000 \n\n");

    // Test 2: Two values in x direction, check line break after 20 values
    cz = 0; cxm = 1; cym = 0;
    double bza2[2][1][1] = {{{1.0}}, {{2.0}}};
    double ex2[2][1][1] = {{{3.0}}, {{4.0}}};
    double ey2[2][1][1] = {{{5.0}}, {{6.0}}};
    double hz2[2][1][1] = {{{7.0}}, {{8.0}}};
    out = capture_output(cz, cxm, cym,
                         (const double* const* const*)bza2,
                         (const double* const* const*)ex2,
                         (const double* const* const*)ey2,
                         (const double* const* const*)hz2);
    // Expect 8 values total, no newline break until end
    assert(out == "1.000000 3.000000 5.000000 7.000000 2.000000 4.000000 6.000000 8.000000 \n\n");

    // Test 3: Verify exact formatting with negative values
    cz = 0; cxm = 0; cym = 0;
    double bza3[1][1][1] = {{{-123.456}}};
    double ex3[1][1][1] = {{{0.000001}}};
    double ey3[1][1][1] = {{{100000.0}}};
    double hz3[1][1][1] = {{{-0.5}}};
    out = capture_output(cz, cxm, cym,
                         (const double* const* const*)bza3,
                         (const double* const* const*)ex3,
                         (const double* const* const*)ey3,
                         (const double* const* const*)hz3);
    assert(out == "-123.456000 0.000001 100000.000000 -0.500000 \n\n");

    return 0;
}
