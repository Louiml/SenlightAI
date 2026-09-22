// Write a standalone C++ function that, given a compile-time integer `SIZE` (a positive integer) and a scalar type `SCALAR` (either `float` or `double`), creates a dynamic vector of length `SIZE`, initializes it with 1.0 at index 0, 2.0 at index 1, and zeros elsewhere, then performs exactly 1,000,000 iterations of the operation `v[0] += v.sum() * 1e-20`. The function must return the final value of `v.sum()` as a `SCALAR`. Handle the case where `SIZE` is 1 (only index 0 exists) by setting `v[1]` only if `SIZE >= 2`. The focus is on numerical stability and correctness of the accumulation over many iterations. You may use Eigen's `Matrix` with dynamic size. Do not use any external libraries other than Eigen and standard headers.

// The core of the task is to correctly perform a repeated update where the sum of the entire vector is used to update the first element, with a tiny scaling factor `1e-20`. Since the sum is recomputed each iteration from scratch, the time complexity is \(O(\text{iterations} \times \text{SIZE})\), and space is \(O(\text{SIZE})\). The main edge case is `SIZE < 2`: we must avoid writing to index 1 if it doesn't exist; `v[1]` is only valid when `SIZE >= 2`. Numerically, because the scalar is tiny, the sum may stay close to 3 or 1 (if SIZE=1) depending on the floating-point type, but the function should perform exactly the same operations as the given snippet, using `coeffRef(0)` for the update. The method is straightforward: allocate the vector, set the first two elements conditionally, then loop a fixed number of times, each time reading `v.sum()` and updating `v.coeffRef(0)`. The result is `v.sum()` after the loop. Time complexity: \(O(1{,}000{,}000 \times SIZE)\), space \(O(SIZE)\).

#include <Eigen/Core>

template <typename SCALAR, int SIZE>
SCALAR repeated_sum_update() {
    using Vec = Eigen::Matrix<SCALAR, Eigen::Dynamic, 1>;
    Vec v(SIZE);
    v.setZero();
    v[0] = SCALAR(1);
    if (SIZE >= 2) {
        v[1] = SCALAR(2);
    }
    for (int i = 0; i < 1000000; ++i) {
        v.coeffRef(0) += v.sum() * SCALAR(1e-20);
    }
    return v.sum();
}

#include <cassert>
#include <cmath>

int main() {
    // Double precision tests with various sizes
    assert(std::fabs(repeated_sum_update<double, 1>() - 1.0) < 1e-9);
    assert(std::fabs(repeated_sum_update<double, 2>() - 3.0) < 1e-9);
    assert(std::fabs(repeated_sum_update<double, 5>() - 3.0) < 1e-9);
    assert(std::fabs(repeated_sum_update<double, 10>() - 3.0) < 1e-9);
    // Float precision, allow larger tolerance due to accumulation
    assert(std::fabs(repeated_sum_update<float, 1>() - 1.0f) < 1e-3f);
    assert(std::fabs(repeated_sum_update<float, 2>() - 3.0f) < 1e-3f);
    assert(std::fabs(repeated_sum_update<float, 3>() - 3.0f) < 1e-3f);
    // Larger size still yields same sum because only indices 0 and 1 are non-zero
    assert(std::fabs(repeated_sum_update<double, 100>() - 3.0) < 1e-9);
    assert(std::fabs(repeated_sum_update<double, 1000>() - 3.0) < 1e-9);
    return 0;
}
