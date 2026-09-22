Write a C++ function that takes integers `N` (number of gauge groups), `k` (N-ality, where 1 ≤ k ≤ N), `m` (number of partitions), `o` (order), and `prec` (precision in bits) as parameters, and returns a 2D array of `mpfr_t` values (using the GNU MPFR library) of size `(N+2) × (m*o+1)`. The array must satisfy: for rows 1 through k (inclusive), column 0 contains `π*(N-k)/N`, for rows k+1 through N (inclusive), column 0 contains `-π*k/N`, and for row indices 1 through N, column `j` (for 1 ≤ j ≤ m*o) is a linear interpolation from the row's column 0 value down to zero at column `m*o`. All rows 0 and N+1 must be initialized to zero in all columns. The function must handle arbitrary precision as specified by `prec` and free all dynamically allocated memory before returning. The result must be a pointer to an array of pointers to `mpfr_t` values, where each `mpfr_t` value has precision `prec`.

The core task is to compute linear interpolation arrays with boundary values. The main algorithm: first, initialize the MPFR library by creating temporary variables for π, numerators `(N-k)` and `(-k)`, denominator `N`, and their ratios. Use `mpfr_const_pi` to get π with rounding toward negative infinity. For rows 1 through k, set the starting value at column 0 to `π*(N-k)/N`; for rows k+1 through N, set it to `-π*k/N`. Then for each interior column j from 1 to m*o−1, compute the interpolation: `value[j] = start_value * (1 - j/(m*o))`, which can be implemented as `start_value - (start_value/(m*o))*j`. Row 0 and row N+1 must contain all zeros. Edge cases: when k=0 or k=N, one of the row groups becomes empty (handle naturally by loop bounds); when m*o is 1, there are no interior columns so only column 0 and column 1 (which is the last index) get set. Time complexity is O(N*m*o) for filling the array, plus O(N) for initialization; space complexity is O((N+2)*(m*o+1)) for the data plus O(N) for temporary arrays. Care must be taken to properly initialize all `mpfr_t` variables to avoid memory leaks, and to free all allocations at the end.

#include <mpfr.h>
#include <cstddef>

// Returns a 2D array of mpfr_t values of size (N+2) x (m*o+1) with linear interpolation for rows 1..N.
// Row 0 and row N+1 are all zeros.
// Rows 1..k: start with pi*(N-k)/N, rows k+1..N: start with -pi*k/N.
// Column j (1 <= j < m*o) is linearly interpolated from the start value to 0 at column m*o.
// All mpfr_t values use precision prec bits. Memory is allocated dynamically and must be freed by caller via freeMatrix().
mpfr_t** createInitialArray(int N, int k, int m, int o, int prec) {
    // Allocate the 2D array using a single contiguous block for efficiency.
    mpfr_t** array = new mpfr_t*[N + 2];
    // Total number of columns including index 0 and index m*o.
    int cols = m * o + 1;
    // Allocate all mpfr_t entries in one contiguous block.
    mpfr_t* data = new mpfr_t[(N + 2) * cols];
    // Set row pointers.
    for (int i = 0; i < N + 2; ++i) {
        array[i] = &data[i * cols];
    }

    // Initialize all mpfr_t values with given precision.
    for (int i = 0; i < (N + 2) * cols; ++i) {
        mpfr_init2(data[i], prec);
    }

    // Temporary variables for computation.
    mpfr_t pi, num1, num2, den, ratio1, ratio2, temp;
    mpfr_init2(pi, prec);
    mpfr_init2(num1, prec);
    mpfr_init2(num2, prec);
    mpfr_init2(den, prec);
    mpfr_init2(ratio1, prec);
    mpfr_init2(ratio2, prec);
    mpfr_init2(temp, prec);

    // Compute constants with round toward negative infinity.
    mpfr_const_pi(pi, MPFR_RNDD);
    mpfr_set_si(num1, N - k, MPFR_RNDD);
    mpfr_set_si(num2, -k, MPFR_RNDD);
    mpfr_set_si(den, N, MPFR_RNDD);
    mpfr_div(ratio1, num1, den, MPFR_RNDD);
    mpfr_div(ratio2, num2, den, MPFR_RNDD);

    // Set row 0 and row N+1 to zero (they are already zero after init, but ensure explicitly).
    for (int j = 0; j < cols; ++j) {
        mpfr_set_zero(array[0][j], 0);
        mpfr_set_zero(array[N + 1][j], 0);
    }

    // For each data row (1..N), set column 0 and the last column, then interpolate.
    for (int p = 1; p <= N; ++p) {
        mpfr_t* row = array[p];
        // Determine starting value based on row index.
        mpfr_t start;
        mpfr_init2(start, prec);
        if (p <= k) {
            mpfr_mul(start, pi, ratio1, MPFR_RNDD);
        } else {
            mpfr_mul(start, pi, ratio2, MPFR_RNDD);
        }

        // Set column 0 to the starting value.
        mpfr_set(row[0], start, MPFR_RNDD);
        // Set the last column to zero (as required by the original).
        mpfr_set_zero(row[m * o], 0);

        // Fill interior columns 1 .. m*o-1 with linear interpolation.
        if (m * o > 1) {
            // Compute step = start / (m*o)
            mpfr_div_si(temp, start, m * o, MPFR_RNDD);
            for (int j = 1; j < m * o; ++j) {
                // value = start - step * j
                mpfr_mul_si(row[j], temp, j, MPFR_RNDD);
                mpfr_neg(row[j], row[j], MPFR_RNDD);
                mpfr_add(row[j], row[j], start, MPFR_RNDD);
            }
        }
        mpfr_clear(start);
    }

    // Free temporary variables.
    mpfr_clear(pi);
    mpfr_clear(num1);
    mpfr_clear(num2);
    mpfr_clear(den);
    mpfr_clear(ratio1);
    mpfr_clear(ratio2);
    mpfr_clear(temp);

    return array;
}

// Frees the memory allocated by createInitialArray.
void freeInitialArray(mpfr_t** array, int N, int cols) {
    if (array == nullptr) return;
    // data block is array[0] minus offset? Actually data pointer is array[0] is the first row, but we allocated data as a contiguous block.
    // We know the data block is the first pointer in array, but to be safe, we need to know it's contiguous.
    // In our implementation, array[0] points to the beginning of the data block.
    mpfr_t* data = array[0];
    // Clear all mpfr_t values.
    for (int i = 0; i < (N + 2) * cols; ++i) {
        mpfr_clear(data[i]);
    }
    delete[] data;
    delete[] array;
}

#include <cassert>
#include <mpfr.h>
#include <cmath>

// Helper to compare mpfr_t with a double using a tolerance.
bool near(const mpfr_t& value, double expected, double tol = 1e-9) {
    double actual = mpfr_get_d(value, MPFR_RNDN);
    return std::fabs(actual - expected) < tol;
}

int main() {
    int N = 5, k = 2, m = 2, o = 3, prec = 128;
    int cols = m * o + 1; // 7
    mpfr_t** arr = createInitialArray(N, k, m, o, prec);

    // Check dimensions: rows 0..6 (N+2=7), columns 0..6
    // Row 0 should be all zeros.
    for (int j = 0; j < cols; ++j) {
        assert(mpfr_zero_p(arr[0][j]) != 0);
    }
    // Row N+1 (index 6) should be all zeros.
    for (int j = 0; j < cols; ++j) {
        assert(mpfr_zero_p(arr[N + 1][j]) != 0);
    }

    // Check row 1 (p=1, p<=k) start value: pi*(5-2)/5 = 3*pi/5 ≈ 1.884955592...
    double expected1 = 3.0 * M_PI / 5.0;
    assert(near(arr[1][0], expected1));
    // Check row 2 (p=2, p<=k) same start.
    assert(near(arr[2][0], expected1));
    // Row 3 (p=3, p>k) start value: -pi*2/5 ≈ -1.256637061...
    double expected3 = -2.0 * M_PI / 5.0;
    assert(near(arr[3][0], expected3));
    // Row 4 and 5 also have same negative start.
    assert(near(arr[4][0], expected3));
    assert(near(arr[5][0], expected3));

    // Check interpolation for row 1: columns j=1..6, with m*o=6
    // column j = start * (1 - j/6)
    for (int j = 1; j < cols - 1; ++j) {
        double expected = expected1 * (1.0 - j / 6.0);
        assert(near(arr[1][j], expected, 1e-7));
    }
    // Column 6 should be zero.
    assert(mpfr_zero_p(arr[1][cols - 1]) != 0);

    // Check interpolation for row 3 similarly.
    for (int j = 1; j < cols - 1; ++j) {
        double expected = expected3 * (1.0 - j / 6.0);
        assert(near(arr[3][j], expected, 1e-7));
    }

    // Check a different k=N case: all rows have positive start.
    int N2 = 4, k2 = 4, m2 = 1, o2 = 2;
    int cols2 = m2 * o2 + 1; // 3
    mpfr_t** arr2 = createInitialArray(N2, k2, m2, o2, prec);
    double expected_all = M_PI * (N2 - k2) / N2; // pi*0 = 0! Actually all zeros.
    for (int p = 1; p <= N2; ++p) {
        for (int j = 0; j < cols2; ++j) {
            assert(near(arr2[p][j], 0.0));
        }
    }

    // Check k=0 case.
    int N3 = 3, k3 = 0, m3 = 2, o3 = 1;
    int cols3 = m3 * o3 + 1; // 3
    mpfr_t** arr3 = createInitialArray(N3, k3, m3, o3, prec);
    // All rows have start value -pi*k/N = 0.
    for (int p = 1; p <= N3; ++p) {
        for (int j = 0; j < cols3; ++j) {
            assert(near(arr3[p][j], 0.0));
        }
    }

    // Clean up.
    freeInitialArray(arr, N, cols);
    freeInitialArray(arr2, N2, cols2);
    freeInitialArray(arr3, N3, cols3);

    return 0;
}
