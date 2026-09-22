Write a standalone C++ function that computes the quadratic programming objective value and, optionally, its gradient for a sparse coding problem. The function takes three vectors: a sparse code vector `a` of length `K`, a signal vector `x` of length `N`, and a dictionary matrix `D` stored as a flat `N`-by-`K` column-major array. The objective is \( f(a) = \frac{1}{2} \| x - D a \|_2^2 \), and the gradient is \( \nabla f(a) = D^T (D a - x) \). The function must return the objective scalar value and, if requested, fill a preallocated gradient vector of length `K`. The inputs must be validated: `a` must have length equal to `K`, `x` must have length `N`, and `D` must have exactly `N*K` elements. If any dimension mismatch occurs, throw a `std::invalid_argument` with a descriptive message. The function signature should be: `double computeQuadraticObjectiveAndGradient(const std::vector<double>& a, const std::vector<double>& x, const std::vector<double>& D, std::vector<double>* gradient = nullptr)`. The gradient argument is optional; if provided, it is resized to `K` and filled. The function must be `const`-correct and not modify its inputs.
#include <cassert>
#include <vector>
#include <cmath>

// Function declaration
double computeQuadraticObjectiveAndGradient(const std::vector<double>& a,
                                            const std::vector<double>& x,
                                            const std::vector<double>& D,
                                            std::vector<double>* gradient = nullptr);

int main() {
    // Test 1: Simple 2x2 dictionary, N=2, K=2
    std::vector<double> D1 = {1, 0, 0, 1}; // identity, column-major
    std::vector<double> a1 = {2, 3};
    std::vector<double> x1 = {2, 3};
    std::vector<double> grad1;
    double obj1 = computeQuadraticObjectiveAndGradient(a1, x1, D1, &grad1);
    assert(std::fabs(obj1 - 0.0) < 1e-12);
    assert(grad1.size() == 2);
    assert(std::fabs(grad1[0] - 0.0) < 1e-12);
    assert(std::fabs(grad1[1] - 0.0) < 1e-12);

    // Test 2: N=3, K=2 with non-trivial values
    std::vector<double> D2 = {1, 2, 3, 4, 5, 6}; // column1=[1,2,3], column2=[4,5,6]
    std::vector<double> a2 = {1, 1};
    std::vector<double> x2 = {6, 8, 10}; // D*a = [5,7,9], residual = [1,1,1]
    double obj2 = computeQuadraticObjectiveAndGradient(a2, x2, D2);
    assert(std::fabs(obj2 - 1.5) < 1e-12); // 0.5*(1+1+1) = 1.5

    // Test gradient with same data
    std::vector<double> grad2;
    computeQuadraticObjectiveAndGradient(a2, x2, D2, &grad2);
    // residual = [1,1,1], gradient = D^T * r = [1*1+2*1+3*1, 4*1+5*1+6*1] = [6,15]
    assert(std::fabs(grad2[0] - 6.0) < 1e-12);
    assert(std::fabs(grad2[1] - 15.0) < 1e-12);

    // Test 3: a all zeros, gradient should be -D^T * x
    std::vector<double> a3 = {0, 0};
    std::vector<double> x3 = {1, 2, 3};
    std::vector<double> grad3;
    double obj3 = computeQuadraticObjectiveAndGradient(a3, x3, D2, &grad3);
    // obj = 0.5 * (1^2+2^2+3^2) = 0.5*14 = 7
    assert(std::fabs(obj3 - 7.0) < 1e-12);
    // gradient = -D^T * x = [-(1*1+2*2+3*3), -(4*1+5*2+6*3)] = [-(1+4+9), -(4+10+18)] = [-14, -32]
    assert(std::fabs(grad3[0] + 14.0) < 1e-12);
    assert(std::fabs(grad3[1] + 32.0) < 1e-12);

    // Test 4: gradient not requested (nullptr) works fine
    double obj4 = computeQuadraticObjectiveAndGradient(a2, x2, D2);
    assert(std::fabs(obj4 - 1.5) < 1e-12);

    // Test 5: dimension mismatch throws
    bool caught = false;
    try {
        std::vector<double> bad_a = {1}; // K=1 but D expects K=2
        std::vector<double> bad_x = {1, 2, 3};
        computeQuadraticObjectiveAndGradient(bad_a, bad_x, D2);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);

    // Test 6: empty input throws
    caught = false;
    try {
        std::vector<double> empty_a;
        std::vector<double> x = {1};
        std::vector<double> D = {1};
        computeQuadraticObjectiveAndGradient(empty_a, x, D);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);

    return 0;
}
#include <vector>
#include <stdexcept>
#include <cmath>

double computeQuadraticObjectiveAndGradient(const std::vector<double>& a,
                                            const std::vector<double>& x,
                                            const std::vector<double>& D,
                                            std::vector<double>* gradient = nullptr) {
    const size_t K = a.size();
    const size_t N = x.size();

    // Validate dimensions
    if (K == 0 || N == 0) {
        throw std::invalid_argument("Empty input: a and x must be non-empty.");
    }
    if (D.size() != N * K) {
        throw std::invalid_argument("Dictionary size mismatch: D must have N*K elements.");
    }

    // Compute residual r = x - D*a
    std::vector<double> residual = x; // copy x into residual
    for (size_t j = 0; j < K; ++j) {
        double coeff = a[j];
        if (coeff != 0.0) {
            const double* col = D.data() + j * N;
            for (size_t i = 0; i < N; ++i) {
                residual[i] -= coeff * col[i];
            }
        }
    }

    // Objective: 0.5 * ||r||^2
    double obj = 0.0;
    for (double val : residual) {
        obj += val * val;
    }
    obj *= 0.5;

    // Gradient: D^T * r
    if (gradient != nullptr) {
        gradient->assign(K, 0.0);
        for (size_t j = 0; j < K; ++j) {
            double sum = 0.0;
            const double* col = D.data() + j * N;
            for (size_t i = 0; i < N; ++i) {
                sum += col[i] * residual[i];
            }
            (*gradient)[j] = sum;
        }
    }

    return obj;
}
// The solution computes the residual vector \( r = x - D a \) efficiently by iterating over each dictionary atom (column of D). Since D is column-major, for each column `j` (0 to K-1), we multiply the coefficient `a[j]` with the corresponding column entries and accumulate into a temporary residual vector initialized to `x`. After computing the residual, the objective is \( \frac{1}{2} r^T r \). For the gradient, we compute \( D^T r \): for each column `j`, the gradient component is the dot product of the column with the residual. The direct double loop over K columns and N rows gives O(NK) time. The space complexity is O(N) for the residual plus O(K) for the gradient if requested. Edge cases: empty inputs should cause dimension errors: if `a` or `x` empty or `D` size does not equal `N*K`, throw. Also, if `gradient` is non-null but empty, resize it. The computation is numerically stable; no special handling for zero vectors is needed.
