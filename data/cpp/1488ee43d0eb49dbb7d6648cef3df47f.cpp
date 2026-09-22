// Write a C++ function named `buildHermitianMatrix` that takes an unsigned integer `n` as its only parameter and returns a `boost::numeric::ublas::hermitian_matrix<std::complex<double>, boost::numeric::ublas::lower>` of size `n x n`. The matrix must be filled with the following rules: For every row `i` (0-based) and column `j` with `j < i` (strictly lower triangular part), set the element to `std::complex<double>(3 * i + j, 3 * i + j)` — i.e., real and imaginary parts both equal to `3*i + j`. For diagonal elements (`i == j`), set the value to `std::complex<double>(4 * i, 0)` — real part `4*i`, imaginary part `0`. The upper triangular part must not be explicitly set (it is automatically the conjugate transpose of the lower part). The function must handle `n = 0` gracefully by returning an empty matrix. Ensure the function is `const`-correct and uses appropriate header includes.
The solution relies on the properties of `boost::numeric::ublas::hermitian_matrix` with the `lower` storage option. In this representation, only the diagonal and lower triangular elements are stored explicitly; the upper triangular elements are implicitly defined as the conjugate transpose of the lower part. Therefore, we only need to fill the diagonal and the strictly lower triangle. The main algorithm iterates over all rows `i` from 0 to `n-1`. For each row, we first set the diagonal element `(i,i)` to `std::complex<double>(4*i, 0)`. Then we iterate over columns `j` from 0 to `i-1` and set `(i,j)` to `std::complex<double>(3*i + j, 3*i + j)`. Because the matrix is Hermitian, the upper triangular entries are automatically correctly populated by the library, so we must not set them manually — doing so could cause undefined behavior or inefficiency. Edge cases: when `n = 0`, the loop does not execute, and we can simply return a default-constructed matrix of size 0x0 (which the library supports). Also note that the imaginary part of diagonal entries is zero, consistent with Hermitian property. Time complexity is \(O(n^2)\) because we iterate over the lower triangular part (including the diagonal), which has \(\frac{n(n+1)}{2}\) elements. Space complexity is \(O(n)\) because the `hermitian_matrix` with `lower` storage internally stores only about half the elements (actually \(\frac{n(n+1)}{2}\) complex numbers, which is still \(O(n^2)\) if we count the full matrix size, but conceptually we allocate only the stored portion; for complexity analysis we typically say \(O(n^2)\) storage for the matrix, but that's inherent to the object we return). For the algorithm's auxiliary space (excluding the returned matrix), it's \(O(1)\).
#include <boost/numeric/ublas/hermitian.hpp>
#include <complex>

// Builds a lower-stored Hermitian matrix of size n x n.
// Diagonal (i,i) = complex(4*i, 0) for all i.
// Lower (i,j) for i>j = complex(3*i + j, 3*i + j).
// Upper part is implicit by Hermitian symmetry.
boost::numeric::ublas::hermitian_matrix<std::complex<double>, boost::numeric::ublas::lower>
buildHermitianMatrix(unsigned int n) {
    using namespace boost::numeric::ublas;
    hermitian_matrix<std::complex<double>, lower> result(n, n);

    for (unsigned int i = 0; i < n; ++i) {
        // Diagonal
        result(i, i) = std::complex<double>(4 * i, 0);
        // Strictly lower part
        for (unsigned int j = 0; j < i; ++j) {
            result(i, j) = std::complex<double>(3 * i + j, 3 * i + j);
        }
    }

    return result;
}
#include <cassert>
#include <complex>
#include <boost/numeric/ublas/hermitian.hpp>

// Declaration (assumes the solution function is in the same translation unit)
boost::numeric::ublas::hermitian_matrix<std::complex<double>, boost::numeric::ublas::lower>
buildHermitianMatrix(unsigned int n);

int main() {
    using namespace boost::numeric::ublas;
    using std::complex;

    // Test n = 0: empty matrix
    auto m0 = buildHermitianMatrix(0);
    assert(m0.size1() == 0);
    assert(m0.size2() == 0);

    // Test n = 1: only diagonal
    auto m1 = buildHermitianMatrix(1);
    assert(m1.size1() == 1);
    assert(m1(0, 0) == complex<double>(0, 0));

    // Test n = 2: check lower and diagonal
    auto m2 = buildHermitianMatrix(2);
    assert(m2(0, 0) == complex<double>(0, 0));
    assert(m2(1, 1) == complex<double>(4, 0));
    assert(m2(1, 0) == complex<double>(3*1 + 0, 3*1 + 0)); // (3,3) -> 3+0i? Wait 3*i+j = 3
    // Actually for i=1, j=0: 3*1+0 = 3, so should be complex(3,3)
    assert(m2(1, 0) == complex<double>(3, 3));
    // Check symmetry: upper (0,1) should be conjugate of (1,0) = (3, -3)
    assert(m2(0, 1) == complex<double>(3, -3));

    // Test n = 3: verify multiple entries
    auto m3 = buildHermitianMatrix(3);
    // Diagonal
    assert(m3(0, 0) == complex<double>(0, 0));
    assert(m3(1, 1) == complex<double>(4, 0));
    assert(m3(2, 2) == complex<double>(8, 0));
    // Lower entries
    assert(m3(1, 0) == complex<double>(3, 3)); // 3*1+0 = 3
    assert(m3(2, 0) == complex<double>(6, 6)); // 3*2+0 = 6
    assert(m3(2, 1) == complex<double>(7, 7)); // 3*2+1 = 7
    // Upper entries are conjugates
    assert(m3(0, 1) == complex<double>(3, -3));
    assert(m3(0, 2) == complex<double>(6, -6));
    assert(m3(1, 2) == complex<double>(7, -7));
    
    // Ensure all diagonal imaginary parts are zero (Hermitian property)
    for (unsigned int i = 0; i < 3; ++i) {
        assert(m3(i, i).imag() == 0.0);
    }
    
    // Check that size matches n
    assert(m3.size1() == 3 && m3.size2() == 3);

    return 0;
}
