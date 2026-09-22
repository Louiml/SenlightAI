Write a C++ function `buildTridiagonalBand(size_t n)` that creates and returns a banded matrix of size `n x n` with lower bandwidth 1 and upper bandwidth 1 (a tridiagonal matrix), where the entry at row `i` and column `j` (0-indexed) is set to `3 * i + j` if `|i - j| <= 1`, and remains zero otherwise. The function must work for `n >= 1`, handle both even and odd sizes, and return the matrix by value. The implementation should use `boost::numeric::ublas::banded_matrix<double>` and fill only the valid band entries (no out-of-band writes). The task must be self-contained (include necessary Boost headers) and the solution must be a free function that can be called from test code.

#include <cassert>
#include <iostream>

// Assume buildTridiagonalBand is declared above or included here.

int main() {
    // n = 1
    {
        auto m = buildTridiagonalBand(1);
        assert(m.size1() == 1 && m.size2() == 1);
        assert(m(0,0) == 0.0);
    }
    // n = 2
    {
        auto m = buildTridiagonalBand(2);
        assert(m.size1() == 2 && m.size2() == 2);
        assert(m(0,0) == 0.0);
        assert(m(0,1) == 1.0);
        assert(m(1,0) == 3.0);
        assert(m(1,1) == 4.0);
    }
    // n = 3
    {
        auto m = buildTridiagonalBand(3);
        assert(m.size1() == 3 && m.size2() == 3);
        assert(m(0,0) == 0.0 && m(0,1) == 1.0 && m(0,2) == 0.0);
        assert(m(1,0) == 3.0 && m(1,1) == 4.0 && m(1,2) == 5.0);
        assert(m(2,0) == 0.0 && m(2,1) == 7.0 && m(2,2) == 8.0);
    }
    // n = 4 (check corners and off-band zeros)
    {
        auto m = buildTridiagonalBand(4);
        assert(m(0,0) == 0.0 && m(0,1) == 1.0 && m(0,2) == 0.0 && m(0,3) == 0.0);
        assert(m(1,1) == 4.0 && m(1,2) == 5.0);
        assert(m(2,1) == 7.0 && m(2,2) == 8.0 && m(2,3) == 9.0);
        assert(m(3,2) == 11.0 && m(3,3) == 12.0);
    }
    // n = 5 (verify diagonal and a few off-diagonal values)
    {
        auto m = buildTridiagonalBand(5);
        assert(m(4,3) == 15.0 && m(4,4) == 16.0);
        assert(m(0,0) == 0.0 && m(0,1) == 1.0);
        assert(m(2,1) == 7.0 && m(2,3) == 9.0);
    }
    std::cout << "All tests passed." << std::endl;
    return 0;
}

#include <boost/numeric/ublas/banded.hpp>
#include <algorithm>
#include <cstddef>

// Build an n x n tridiagonal banded matrix where entry (i,j) = 3*i + j within the band.
boost::numeric::ublas::banded_matrix<double> buildTridiagonalBand(std::size_t n) {
    using namespace boost::numeric::ublas;
    banded_matrix<double> m(n, n, 1, 1); // lower bandwidth 1, upper bandwidth 1

    for (signed i = 0; i < static_cast<signed>(n); ++i) {
        signed col_start = std::max(i - 1, 0);
        signed col_end = std::min(i + 1, static_cast<signed>(n) - 1);
        for (signed j = col_start; j <= col_end; ++j) {
            m(i, j) = 3 * i + j;
        }
    }
    return m;
}

// The main algorithm is straightforward: allocate a `banded_matrix<double>` of dimensions `n x n` with lower and upper bandwidths both equal to 1. Since the matrix is tridiagonal, valid entries satisfy `j >= i - 1` and `j <= i + 1`. Loop over rows `i` from 0 to `n-1`, and for each row, loop over columns `j` from `max(i-1, 0)` to `min(i+1, n-1)` (both inclusive), assigning `m(i, j) = 3 * i + j`. All other entries remain zero by default. Edge cases: when `n == 1`, the only valid entry is at `(0,0)` and both bandwidth loops naturally handle it since `i-1 < 0` and `i+1 >= n`. For `n == 2`, the band is full (since width 2 is fully covered by a tridiagonal), but the algorithm still works. Time complexity is \(O(n)\) because the number of band entries is \(3n - 2\) for \(n \ge 2\) (and 1 for `n == 1`). Space complexity is \(O(n^2)\) because the `banded_matrix` stores the full square matrix (Boost's `banded_matrix` stores all elements, not compressed), but that is inherent to the data structure. Use `signed` types for loop indices to avoid underflow when computing `i-1`, and `std::max`/`std::min` from `<algorithm>`.
