Write a C++ function `matrixEigenvalues` that takes a square matrix represented as an `Eigen::MatrixXd` and returns a `std::string` containing the eigenvalues of that matrix, formatted as space-separated complex numbers printed with scientific notation and 4 decimal places each (matching the default Eigen output format for complex types). The function must accept any square matrix (including 1x1, diagonal, singular, or with repeated eigenvalues) and return the eigenvalues in the order Eigen computes them (which is not sorted). Ensure the function handles all square sizes (n ≥ 1) and does not modify the input matrix. The output string must exactly match what `std::cout << eigenvalues` would produce for the same matrix.

#include <cassert>
#include <Eigen/Dense>
#include <string>
#include <sstream>
#include <iomanip>

// Assume matrixEigenvalues is defined as above.
std::string matrixEigenvalues(const Eigen::MatrixXd& matrix);

int main() {
    // Test 1: 3x3 all-ones matrix (eigenvalues: 3, 0, 0)
    Eigen::MatrixXd ones = Eigen::MatrixXd::Ones(3, 3);
    std::string onesStr = matrixEigenvalues(ones);
    // Expected: "3.0000e+00+0.0000i 0.0000e+00+0.0000i 0.0000e+00+0.0000i"
    assert(onesStr == "3.0000e+00+0.0000i 0.0000e+00+0.0000i 0.0000e+00+0.0000i");

    // Test 2: Identity matrix (all eigenvalues 1)
    Eigen::MatrixXd identity = Eigen::MatrixXd::Identity(2, 2);
    std::string identStr = matrixEigenvalues(identity);
    assert(identStr == "1.0000e+00+0.0000i 1.0000e+00+0.0000i");

    // Test 3: 1x1 matrix with value 5
    Eigen::MatrixXd single(1, 1);
    single(0, 0) = 5.0;
    assert(matrixEigenvalues(single) == "5.0000e+00+0.0000i");

    // Test 4: 2x2 rotation matrix (complex conjugate eigenvalues)
    Eigen::MatrixXd rotation(2, 2);
    rotation << 0.0, -1.0, 1.0, 0.0; // eigenvalues: i and -i
    std::string rotStr = matrixEigenvalues(rotation);
    // Eigen might order as (0+1i) and (0-1i) or the reverse; check both possibilities
    assert(rotStr == "0.0000e+00+1.0000e+00i 0.0000e+00-1.0000e+00i" ||
           rotStr == "0.0000e+00-1.0000e+00i 0.0000e+00+1.0000e+00i");

    // Test 5: Diagonal matrix with mixed eigenvalues
    Eigen::MatrixXd diag(3, 3);
    diag << 2.0, 0.0, 0.0,
            0.0, -1.0, 0.0,
            0.0, 0.0, 4.0;
    std::string diagStr = matrixEigenvalues(diag);
    assert(diagStr == "2.0000e+00+0.0000i -1.0000e+00+0.0000i 4.0000e+00+0.0000i");

    // Test 6: Matrix with repeated eigenvalues (e.g., 2x2 with both eigenvalues 0)
    Eigen::MatrixXd zeroMat(2, 2);
    zeroMat << 0.0, 0.0, 0.0, 0.0;
    std::string zeroStr = matrixEigenvalues(zeroMat);
    assert(zeroStr == "0.0000e+00+0.0000i 0.0000e+00+0.0000i");

    return 0;
}

#include <Eigen/Dense>
#include <complex>
#include <sstream>
#include <iomanip>
#include <string>

// Returns a string containing the eigenvalues of a square matrix, formatted
// exactly as Eigen's default output (scientific notation, 4 decimal places,
// space-separated, complex numbers like "1.0000+0.0000i").
std::string matrixEigenvalues(const Eigen::MatrixXd& matrix) {
    // Compute eigenvalues (matrix must be square, checked by Eigen).
    Eigen::VectorXcd values = matrix.eigenvalues();

    std::ostringstream output;
    output << std::scientific << std::setprecision(4);

    for (Eigen::Index i = 0; i < values.size(); ++i) {
        if (i > 0) {
            output << " ";
        }
        const std::complex<double>& z = values(i);
        // Format real and imaginary parts separately to match Eigen's format.
        output << z.real();
        // Eigen always prints the imaginary part with a sign, even if positive.
        if (z.imag() >= 0.0) {
            output << "+";
        }
        output << z.imag() << "i";
    }
    return output.str();
}

// The solution uses Eigen's built-in `eigenvalues()` method, which is available for square matrices and returns a `VectorXcd` (a column vector of complex doubles). The algorithm is straightforward: construct the output string by iterating over the complex numbers in the vector and formatting each using the same precision and notation Eigen uses by default (scientific with 4 decimal places). The key is to mimic Eigen's output exactly: for each complex number, the real part is printed first, then if the imaginary part is non-negative, a `+` sign is inserted, otherwise the negative sign is included automatically; the imaginary unit is denoted by `i` (e.g., `1.0000+0.0000i`). A subtle edge case: complex numbers with zero imaginary part are printed with the imaginary part including the plus sign (e.g., `1.0000+0.0000i`). Also, Eigen's default precision is 4 decimal places with scientific notation (e.g., `3.0000e+00`). The function must not include a trailing newline (the `cout` example added one, but the string returned should just be the space-separated numbers). Edge cases include 1x1 matrices (one eigenvalue), matrices with repeated eigenvalues (e.g., identity matrix → all 1+0i), and matrices with complex conjugate pairs (e.g., rotation matrices). Time complexity is \(O(n^3)\) due to the eigenvalue computation, and space complexity is \(O(n)\) for the eigenvalue vector and the output string.
