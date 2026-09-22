// Write a C++ function that takes an integer `n` and a floating-point scalar `scale` as parameters, creates a dynamic vector of size `n` initialized to zero, sets the first two elements to 1 and 2, and then performs 1,000,000 iterations where it adds `scale * current_sum` to the first element (i.e., `v[0] += scale * v.sum()` each iteration). The function must return the final sum of all vector elements as a `double`. Handle edge cases where `n` is 0 (return 0.0) or 1 (only the first element exists, but you still set `v[1]` only if `n >= 2`), and consider numerical stability (the scale is expected to be very small, like `1e-20`, but your code should not assume a specific value).

// The solution uses the Eigen library (`Eigen::Matrix<double, Eigen::Dynamic, 1>` for the vector). We initialize the vector with `setZero()`, then conditionally set `v[0]=1` and if `n>=2`, `v[1]=2`. In the loop, we repeatedly call `v.sum()` which computes the sum of all elements in O(n) time, and update `v[0]` by adding `scale * sum`. Since the sum changes each iteration, this is an iterative numerical process. The main algorithm is O(n + 1,000,000 * n) = O(1,000,000 * n) time, which is linear in `n` per iteration. Space complexity is O(n) for the vector. Edge cases: if `n==0`, the vector is empty, and we must return 0.0 immediately to avoid undefined behavior (sum of empty vector). If `n==1`, only `v[0]` is set; `v[1]` would be out of bounds, so we guard with an `if (n >= 2)`. Also note that the final sum can be very close to 3 plus a tiny perturbation, but we just return the computed value. The function should be `const`-correct and take parameters appropriately.

#include <Eigen/Core>
#include <stdexcept>

// Compute the final sum after iterative updates to a dynamic vector.
// Parameters: n - vector size, scale - multiplicative factor for each update.
// Returns the final sum of all vector elements as a double.
double iterativeVectorSum(int n, double scale) {
    if (n < 0) {
        throw std::invalid_argument("n must be non-negative");
    }
    if (n == 0) {
        return 0.0;
    }

    using Vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
    Vec v(n);
    v.setZero();
    v[0] = 1.0;
    if (n >= 2) {
        v[1] = 2.0;
    }

    const int iterations = 1000000;
    for (int i = 0; i < iterations; ++i) {
        v.coeffRef(0) += v.sum() * scale;
    }

    return v.sum();
}

#include <cassert>
#include <cmath>

// Global main function for testing the solution function.
int main() {
    // n=0 returns 0.0
    assert(iterativeVectorSum(0, 1e-20) == 0.0);

    // n=1: v=[1], sum stays 1 because adding scale*1 to v[0] changes sum but tiny; after 1e6 iterations, sum ~1 + 1e6*1e-20*1 = 1+1e-14 (approx). Check within tolerance.
    double sum1 = iterativeVectorSum(1, 1e-20);
    assert(std::fabs(sum1 - (1.0 + 1e6 * 1e-20)) < 1e-9);

    // n=2: v=[1,2], sum initially 3. Each iteration adds scale*sum to v[0], so sum increases slightly. Use exact formula: sum_k+1 = sum_k + scale*sum_k = sum_k*(1+scale). After 1e6 iterations, sum = 3*(1+1e-20)^1000000 ≈ 3*(1 + 1e-14) = 3 + 3e-14.
    double sum2 = iterativeVectorSum(2, 1e-20);
    assert(std::fabs(sum2 - (3.0 * std::pow(1.0 + 1e-20, 1000000))) < 1e-7);

    // n=3: v=[1,2,0], sum initially 3. Same dynamics as n=2 because v[2] stays 0. So formula matches.
    double sum3 = iterativeVectorSum(3, 1e-20);
    assert(std::fabs(sum3 - sum2) < 1e-12);

    // n=5 with larger scale: scale=1e-10, sum grows factor (1+1e-10)^1e6 ≈ exp(1e-4) ≈ 1.000100005. Check within tolerance.
    double sum5 = iterativeVectorSum(5, 1e-10);
    double expected5 = 3.0 * std::pow(1.0 + 1e-10, 1000000);
    assert(std::fabs(sum5 - expected5) < 1e-4);

    // Negative scale not expected but test: n=2, scale=-1e-20, sum decreases slowly, use formula.
    double sumNeg = iterativeVectorSum(2, -1e-20);
    double expectedNeg = 3.0 * std::pow(1.0 - 1e-20, 1000000);
    assert(std::fabs(sumNeg - expectedNeg) < 1e-7);

    return 0;
}
