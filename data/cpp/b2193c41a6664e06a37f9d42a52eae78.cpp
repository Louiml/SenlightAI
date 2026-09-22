// Write a C++ function that generates a table of sine and cosine values for angles from 0 to 90 degrees in increments of 10 degrees. The function should accept two parameters: an integer `n` (representing the number of rows, i.e., the number of angles to generate, starting at 0 degrees and ending at `(n-1)*10` degrees) and a double `degreesPerRow` (the increment between rows). The function must return an `Eigen::ArrayXXf` matrix with `n` rows and 4 columns: column 0 = degrees (as float), column 1 = radians, column 2 = sine(radians), column 3 = cosine(radians). The function must compute values using `Eigen`'s array operations (e.g., `LinSpaced`, `sin()`, `cos()`) and must correctly handle `n = 0` by returning an empty matrix with 0 rows and 4 columns. The function should be named `createTrigTable` and must be `const`-correct (taking parameters by value or const reference as appropriate). You may assume `Eigen` and `<cmath>` are available. Provide a self-contained implementation without a `main` function.

#include <cassert>
#include <cmath>
#include <Eigen/Dense>

// Assume the solution function is declared above (or included here)

int main() {
    // Test with n=10, degreesPerRow=10 (as in original snippet)
    Eigen::ArrayXXf table = createTrigTable(10, 10.0);
    assert(table.rows() == 10);
    assert(table.cols() == 4);
    // Check degrees column: 0,10,20,...,90
    for (int i = 0; i < 10; ++i) {
        assert(std::abs(table(i, 0) - i * 10.0f) < 1e-5);
    }
    // Check radian conversion and sine/cosine for first and last rows
    assert(std::abs(table(0, 1) - 0.0f) < 1e-5);
    assert(std::abs(table(0, 2) - 0.0f) < 1e-5);
    assert(std::abs(table(0, 3) - 1.0f) < 1e-5);
    // 90 degrees -> pi/2 radians, sin=1, cos=0
    assert(std::abs(table(9, 1) - static_cast<float>(M_PI / 2.0)) < 1e-5);
    assert(std::abs(table(9, 2) - 1.0f) < 1e-5);
    assert(std::abs(table(9, 3) - 0.0f) < 1e-5);

    // Test n=1
    Eigen::ArrayXXf single = createTrigTable(1, 30.0);
    assert(single.rows() == 1);
    assert(std::abs(single(0, 0) - 0.0f) < 1e-5);
    assert(std::abs(single(0, 2) - 0.0f) < 1e-5);
    assert(std::abs(single(0, 3) - 1.0f) < 1e-5);

    // Test n=0 returns empty
    Eigen::ArrayXXf empty = createTrigTable(0, 10.0);
    assert(empty.rows() == 0);
    assert(empty.cols() == 4);

    // Test custom increment (non-multiple of 10)
    Eigen::ArrayXXf custom = createTrigTable(3, 15.0);
    assert(custom.rows() == 3);
    assert(std::abs(custom(1, 0) - 15.0f) < 1e-5);
    // Check radian for 15 degrees: pi/12
    assert(std::abs(custom(1, 1) - static_cast<float>(M_PI / 12.0)) < 1e-5);

    return 0;
}

#include <Eigen/Dense>
#include <cmath>

// Generate a table of degrees, radians, sine, and cosine values.
// n: number of rows (angles from 0 to (n-1)*degreesPerRow)
// degreesPerRow: increment in degrees between rows
// Returns an n x 4 Eigen::ArrayXXf with columns:
//   0: degrees, 1: radians, 2: sin(radians), 3: cos(radians)
Eigen::ArrayXXf createTrigTable(int n, double degreesPerRow) {
    if (n <= 0) {
        return Eigen::ArrayXXf(0, 4);
    }

    Eigen::ArrayXXf table(n, 4);

    // Column 0: degrees from 0 to (n-1)*degreesPerRow, spaced evenly
    table.col(0) = Eigen::ArrayXf::LinSpaced(n, 0.0f, static_cast<float>((n - 1) * degreesPerRow));

    // Column 1: radians = degrees * (pi/180)
    table.col(1) = static_cast<float>(M_PI / 180.0) * table.col(0);

    // Columns 2 and 3: sine and cosine of radians
    table.col(2) = table.col(1).sin();
    table.col(3) = table.col(1).cos();

    return table;
}

// The solution uses Eigen's `ArrayXXf` to store the table. First, we create a matrix of size `n x 4`. If `n` is 0, we return an empty matrix (Eigen allows default construction). For non-zero `n`, we fill column 0 using `ArrayXf::LinSpaced(n, 0.0f, (n-1)*degreesPerRow)`. This generates evenly spaced values from 0 to `(n-1)*degreesPerRow` inclusive. Then column 1 is the radian conversion by multiplying column 0 by `M_PI / 180.0f`. Column 2 and 3 are the sine and cosine of column 1 using Eigen's element-wise `sin()` and `cos()` functions. These operations are vectorized and avoid manual loops. Edge cases: `n=1` produces a single row with 0 degrees, 0 radians, sin(0)=0, cos(0)=1. `n=0` must be handled to avoid accessing invalid indices; we simply return an empty matrix. `degreesPerRow` can be any positive value (including non-integer), but we treat it as a float for consistency with the array. Complexity: O(n) time and O(n) space for the matrix, as each column is filled in linear time via Eigen's vectorized operations.
