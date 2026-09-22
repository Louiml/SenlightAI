// Write a standalone C++ function that takes a fixed-size 2x2 matrix of single-precision complex floats (represented as `std::complex<float>` in a plain C-style array or `std::array<std::array<std::complex<float>,2>,2>`) and returns a new 2x2 matrix that is the conjugate transpose (adjoint) of the input. The function must handle complex numbers correctly: for each element at position (i,j) in the output, take the element at position (j,i) from the input and apply complex conjugation (negate the imaginary part). The function should be `const`‑correct, take the input by const reference, and return the result by value. No modifications to the input are allowed. You may use `std::complex<float>` and any standard containers; include all necessary headers. The function must work identically for matrices with zeros, purely real values, purely imaginary values, and mixed complex entries.
// The solution requires transposing a 2x2 complex matrix and conjugating each element. The main algorithm is straightforward: iterate over all four positions (i,j) in the output, set output[i][j] = conj(input[j][i]). Here `conj` returns the complex conjugate of a `std::complex<float>`, which flips the sign of the imaginary component. Edge cases include purely real matrices (where conjugation has no effect) and purely imaginary matrices (where only the sign of the imaginary part changes). No special handling for zero or NaN is needed; `std::conj` handles these naturally. Time complexity is O(1) since the matrix size is fixed at 4 elements; auxiliary space is also O(1) because we only allocate the output matrix (which is part of the return value). Using `std::array` provides bounds safety and clear indexing. Key implementation detail: ensure the input is taken by const reference to avoid copying, and make the function itself `const` if it is a member, but here it is a free function that directly takes a const reference. Use `std::conj` from `<complex>` header.
#include <array>
#include <complex>

// Compute the conjugate transpose (adjoint) of a 2x2 complex<float> matrix.
// Returns a new matrix; does not modify the input.
std::array<std::array<std::complex<float>, 2>, 2> adjoint(
    const std::array<std::array<std::complex<float>, 2>, 2>& m) {
    std::array<std::array<std::complex<float>, 2>, 2> result;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            result[i][j] = std::conj(m[j][i]);
        }
    }
    return result;
}
#include <cassert>
#include <complex>
#include <array>

// The solution function is assumed to be defined above.
// Test with various matrices.

int main() {
    using Mat = std::array<std::array<std::complex<float>, 2>, 2>;

    // Test 1: Identity matrix (real, no change in conjugation)
    Mat ident = {{{ {1,0}, {0,0} }, { {0,0}, {1,0} }}};
    Mat adj_ident = adjoint(ident);
    assert(adj_ident[0][0] == std::complex<float>(1,0));
    assert(adj_ident[0][1] == std::complex<float>(0,0));
    assert(adj_ident[1][0] == std::complex<float>(0,0));
    assert(adj_ident[1][1] == std::complex<float>(1,0));

    // Test 2: Purely imaginary matrix; conjugation flips sign of imaginary parts
    Mat imag = {{{ {0,2}, {0,0} }, { {0,0}, {0,-3} }}};
    Mat adj_imag = adjoint(imag);
    assert(adj_imag[0][0] == std::complex<float>(0,-2));
    assert(adj_imag[1][1] == std::complex<float>(0,3));

    // Test 3: Non-symmetric complex matrix with mixed entries
    Mat mixed = {{{ {1,2}, {3,-4} }, { {-5,6}, {7,8} }}};
    Mat adj_mixed = adjoint(mixed);
    // output[0][0] = conj(m[0][0]) = conj(1+2i) = 1-2i
    assert(adj_mixed[0][0] == std::complex<float>(1,-2));
    // output[0][1] = conj(m[1][0]) = conj(-5+6i) = -5-6i
    assert(adj_mixed[0][1] == std::complex<float>(-5,-6));
    // output[1][0] = conj(m[0][1]) = conj(3-4i) = 3+4i
    assert(adj_mixed[1][0] == std::complex<float>(3,4));
    // output[1][1] = conj(m[1][1]) = conj(7+8i) = 7-8i
    assert(adj_mixed[1][1] == std::complex<float>(7,-8));

    // Test 4: Zero matrix
    Mat zero = {{{ {0,0}, {0,0} }, { {0,0}, {0,0} }}};
    Mat adj_zero = adjoint(zero);
    for (auto& row : adj_zero) {
        for (auto& elem : row) {
            assert(elem == std::complex<float>(0,0));
        }
    }

    // Test 5: Verify input is not modified by the function
    Mat original = mixed;
    Mat adj_again = adjoint(mixed);
    assert(mixed == original); // original unchanged
    // Also double-check that adjoint of adjoint returns original (unitary property for non-Hermitian)
    Mat double_adj = adjoint(adj_mixed);
    assert(double_adj == original);

    return 0;
}
