// Write a standalone C++ function `fitChebyshevLeastSquares` that takes a vector of `DataPoint` structs (each containing `double x` and `double y` members) and an integer polynomial degree `p` (where `p >= 0`). The function must sort the data points by `x` in ascending order, then fit a Chebyshev polynomial of degree `p` to the data using least squares with singular value decomposition (SVD). It should return a `ChebyshevPolynomial` object (which you must also define as part of the solution) supporting evaluation via `operator()(double x)`. The fitting must map the original `x`-coordinates to the domain [-1,1] using the formula `xi = (2*x - (xMax+xMin)) / (xMax - xMin)`. The returned polynomial must be defined over the original `[xMin, xMax]` interval, so evaluating it at any original `x` yields the fitted value. Handle edge cases: if `p` is larger than or equal to the number of points, still fit the best possible (use a least-squares SVD solution even if underdetermined, but note it will be a minimum-norm solution). If all `x` values are identical, set the slope term to zero and fit a constant. The function must be `const`-correct and must not print anything. Provide the full definitions for `DataPoint` and `ChebyshevPolynomial` (the latter must store coefficients and the interval endpoints, and support `operator[]` for coefficient access, `init`, and `operator()`).
The main algorithm involves three steps: (1) sorting the data points by `x` using `std::sort` with a lambda, (2) constructing the Vandermonde-like design matrix `A` where each column corresponds to a Chebyshev polynomial term evaluated at the mapped `xi`, and (3) solving the linear least squares problem `A * c ≈ b` (where `b` holds the `y` values) using SVD (e.g., `Eigen::bdcSvd`). The Chebyshev polynomials are generated recursively: `T0=1`, `T1=xi`, and `Tn = 2*xi*T(n-1) - T(n-2)`. After obtaining coefficients, we initialize a `ChebyshevPolynomial` object with the interval `[xMin, xMax]` and set its coefficients. Important edge cases: if all `x` are identical (range zero), we must avoid division by zero; we can set the mapping to `xi=0` for all points and treat the fit as a constant (set all coefficients above degree 0 to zero). For `p >= n`, the system is underdetermined, but SVD yields a minimum-norm solution, which is acceptable. Time complexity is `O(n log n + n*p^2 + p^3)` due to sorting, filling the matrix, and SVD (which for an `n x (p+1)` matrix with `p+1 <= n` is `O(n*(p+1)^2)`; if `p+1 > n`, the SVD cost is `O(n^2*(p+1))`). Space complexity is `O(n*(p+1))` for the matrices.
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <Eigen/Dense>

// DataPoint struct: holds x and y coordinates
struct DataPoint {
    double x;
    double y;
};

// ChebyshevPolynomial class: represents a polynomial as a sum of Chebyshev basis functions
// defined on the interval [a, b].
class ChebyshevPolynomial {
private:
    std::vector<double> coeffs; // coefficients for T0, T1, ..., Tp
    double a, b;                // interval endpoints (a < b)

public:
    // Default constructor: empty polynomial on [0,1]
    ChebyshevPolynomial() : a(0.0), b(1.0) {}

    // Initialize with number of coefficients n and interval [lower, upper]
    void init(int n, double lower, double upper) {
        if (n <= 0) throw std::invalid_argument("Number of coefficients must be positive");
        if (lower >= upper) throw std::invalid_argument("Interval lower must be less than upper");
        coeffs.assign(n, 0.0);
        a = lower;
        b = upper;
    }

    // Access coefficient for T_j
    double& operator[](int j) {
        if (j < 0 || j >= static_cast<int>(coeffs.size()))
            throw std::out_of_range("Coefficient index out of range");
        return coeffs[j];
    }

    const double& operator[](int j) const {
        if (j < 0 || j >= static_cast<int>(coeffs.size()))
            throw std::out_of_range("Coefficient index out of range");
        return coeffs[j];
    }

    // Evaluate polynomial at x using Clenshaw's algorithm for stability
    double operator()(double x) const {
        if (coeffs.empty()) return 0.0;
        // Map x from [a,b] to [-1,1]
        double xi = (2.0 * x - (a + b)) / (b - a);
        int n = coeffs.size();
        // Clenshaw's recurrence for Chebyshev sums
        double b_k2 = 0.0, b_k1 = 0.0;
        for (int k = n - 1; k >= 1; --k) {
            double b_k = 2.0 * xi * b_k1 - b_k2 + coeffs[k];
            b_k2 = b_k1;
            b_k1 = b_k;
        }
        return coeffs[0] + xi * b_k1 - b_k2;
    }

    // Get the degree (highest power)
    int degree() const { return static_cast<int>(coeffs.size()) - 1; }
};

// Fit a Chebyshev polynomial of degree p to the given data points using least squares SVD.
// Returns a ChebyshevPolynomial defined on the original x-interval.
ChebyshevPolynomial fitChebyshevLeastSquares(std::vector<DataPoint>& data, int p) {
    if (p < 0) throw std::invalid_argument("Degree must be non-negative");
    int n = static_cast<int>(data.size());
    if (n == 0) throw std::invalid_argument("No data points provided");

    // Sort data by x in ascending order
    std::sort(data.begin(), data.end(),
              [](const DataPoint& a, const DataPoint& b) { return a.x < b.x; });

    double xMin = data.front().x;
    double xMax = data.back().x;
    double xRange = xMax - xMin;

    // Prepare mapping: if all x identical, handle separately (fit constant)
    if (xRange == 0.0) {
        // Compute mean y
        double sumY = 0.0;
        for (const auto& d : data) sumY += d.y;
        double meanY = sumY / n;

        ChebyshevPolynomial P;
        P.init(p + 1, xMin - 1.0, xMin + 1.0); // arbitrary interval with xMin inside
        // Set constant coefficient to meanY, all others to 0
        P[0] = meanY;
        for (int j = 1; j <= p; ++j) P[j] = 0.0;
        return P;
    }

    // Build design matrix A (n x (p+1)) and vector b (n)
    Eigen::MatrixXd A(n, p + 1);
    Eigen::VectorXd b(n);

    for (int i = 0; i < n; ++i) {
        double xi = (2.0 * data[i].x - (xMax + xMin)) / xRange;
        double Tn2 = 1.0;
        A(i, 0) = Tn2;
        if (p >= 1) {
            double Tn1 = xi;
            A(i, 1) = Tn1;
            for (int j = 2; j <= p; ++j) {
                double Tn = 2.0 * Tn1 * xi - Tn2;
                A(i, j) = Tn;
                Tn2 = Tn1;
                Tn1 = Tn;
            }
        }
        b(i) = data[i].y;
    }

    // Solve least squares using SVD
    Eigen::VectorXd coeffs = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

    // Create and populate the ChebyshevPolynomial
    ChebyshevPolynomial P;
    P.init(p + 1, xMin, xMax);
    for (int j = 0; j <= p; ++j) {
        P[j] = coeffs(j);
    }
    return P;
}
#include <cassert>
#include <cmath>
#include <vector>

// Assume DataPoint and ChebyshevPolynomial are defined as in the solution.

int main() {
    // Test 1: Perfect linear fit y = 2x + 1
    {
        std::vector<DataPoint> data = {{0.0, 1.0}, {1.0, 3.0}, {2.0, 5.0}, {3.0, 7.0}};
        ChebyshevPolynomial P = fitChebyshevLeastSquares(data, 1);
        assert(std::abs(P(0.0) - 1.0) < 1e-9);
        assert(std::abs(P(1.0) - 3.0) < 1e-9);
        assert(std::abs(P(2.0) - 5.0) < 1e-9);
        assert(std::abs(P(3.0) - 7.0) < 1e-9);
    }

    // Test 2: Constant data (all x identical)
    {
        std::vector<DataPoint> data = {{2.0, 4.0}, {2.0, 4.0}, {2.0, 4.0}};
        ChebyshevPolynomial P = fitChebyshevLeastSquares(data, 2);
        assert(std::abs(P(2.0) - 4.0) < 1e-9);
        assert(std::abs(P(2.5) - 4.0) < 1e-9); // value should remain constant
    }

    // Test 3: Quadratic fit with exact data y = x^2 on [-1, 1]
    {
        std::vector<DataPoint> data = {{-1.0, 1.0}, {-0.5, 0.25}, {0.0, 0.0}, {0.5, 0.25}, {1.0, 1.0}};
        ChebyshevPolynomial P = fitChebyshevLeastSquares(data, 2);
        assert(std::abs(P(-1.0) - 1.0) < 1e-8);
        assert(std::abs(P(-0.5) - 0.25) < 1e-8);
        assert(std::abs(P(0.0) - 0.0) < 1e-8);
        assert(std::abs(P(0.5) - 0.25) < 1e-8);
        assert(std::abs(P(1.0) - 1.0) < 1e-8);
    }

    // Test 4: Underdetermined system (p >= n) with two points, degree 3
    {
        std::vector<DataPoint> data = {{0.0, 0.0}, {1.0, 1.0}};
        ChebyshevPolynomial P = fitChebyshevLeastSquares(data, 3);
        // The minimum-norm solution should still pass through both points
        assert(std::abs(P(0.0) - 0.0) < 1e-8);
        assert(std::abs(P(1.0) - 1.0) < 1e-8);
    }

    // Test 5: Degree 0 (constant fit) gives mean y
    {
        std::vector<DataPoint> data = {{0.0, 2.0}, {1.0, 4.0}, {2.0, 6.0}};
        ChebyshevPolynomial P = fitChebyshevLeastSquares(data, 0);
        double meanY = (2.0 + 4.0 + 6.0) / 3.0;
        assert(std::abs(P(1.0) - meanY) < 1e-9);
        assert(std::abs(P(10.0) - meanY) < 1e-9); // constant everywhere
    }

    // Test 6: Data not sorted originally; check sorted order in returned polynomial
    {
        std::vector<DataPoint> data = {{3.0, 10.0}, {0.0, 1.0}, {1.0, 4.0}, {2.0, 7.0}};
        ChebyshevPolynomial P = fitChebyshevLeastSquares(data, 1);
        assert(std::abs(P(0.0) - 1.0) < 1e-9);
        assert(std::abs(P(1.0) - 4.0) < 1e-9);
        assert(std::abs(P(2.0) - 7.0) < 1e-9);
        assert(std::abs(P(3.0) - 10.0) < 1e-9);
    }

    // Test 7: Non-zero mean with noise, check approximation reasonable
    {
        std::vector<DataPoint> data = {
            {0.0, 1.1}, {0.1, 1.2}, {0.2, 1.0}, {0.3, 1.3}, {0.4, 1.2},
            {0.5, 1.4}, {0.6, 1.3}, {0.7, 1.5}, {0.8, 1.4}, {0.9, 1.6}
        };
        ChebyshevPolynomial P = fitChebyshevLeastSquares(data, 2);
        // Should be close to constant around 1.3
        for (int i = 0; i < 10; ++i) {
            double x = 0.1 * i;
            assert(std::abs(P(x) - 1.3) < 0.3);
        }
    }

    return 0;
}
