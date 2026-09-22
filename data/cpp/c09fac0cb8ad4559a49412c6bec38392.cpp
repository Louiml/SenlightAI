Write a C++ function named `matrixProductFlops` that takes three integer dimensions `m`, `n`, and `k` as inputs and returns the expected number of floating-point operations (FLOPs) required to compute the matrix product of an `m x k` matrix with a `k x n` matrix using the standard triple-nested-loop algorithm. The function should return a `long long` value representing `2 * m * n * k`. You must also handle negative or zero dimensions by returning `0` for such invalid inputs. The function should be generic and usable with any integer types, but must accept `int` parameters and return `long long` to avoid overflow for larger dimensions. The purpose is to provide a simple, deterministic calculation that can be used in benchmarking scenarios to compare theoretical versus actual performance.

#include <cassert>
#include <cstddef>

// The function declaration is assumed to be provided above; include directly or via header.
long long matrixProductFlops(int, int, int);

int main() {
    // Basic positive cases
    assert(matrixProductFlops(1, 1, 1) == 2);
    assert(matrixProductFlops(2, 3, 4) == 2LL * 2 * 3 * 4); // 48
    assert(matrixProductFlops(10, 10, 10) == 2000);
    
    // Edge cases with zero dimensions
    assert(matrixProductFlops(0, 5, 5) == 0);
    assert(matrixProductFlops(5, 0, 5) == 0);
    assert(matrixProductFlops(5, 5, 0) == 0);
    
    // Negative dimensions
    assert(matrixProductFlops(-3, 4, 5) == 0);
    assert(matrixProductFlops(3, -4, 5) == 0);
    assert(matrixProductFlops(3, 4, -5) == 0);
    assert(matrixProductFlops(-2, -2, -2) == 0);
    
    // Larger values to check no overflow
    assert(matrixProductFlops(10000, 10000, 10000) == 2000000000000LL);
    assert(matrixProductFlops(46340, 46340, 46340) == 2LL * 46340 * 46340 * 46340); // ~1.99e14
    
    return 0;
}

#include <cstddef>

// Compute the number of floating-point operations (FLOPs) for an m x k times k x n matrix product.
// Returns 0 if any dimension is non-positive (invalid).
long long matrixProductFlops(int m, int n, int k) {
    // Check for invalid dimensions
    if (m <= 0 || n <= 0 || k <= 0) {
        return 0;
    }
    // Standard triple-loop matrix multiplication: each output element requires k multiplications
    // and k-1 additions, but common practice counts as 2*k per output element, hence 2*m*n*k total.
    // Use long long to avoid overflow for large dimensions.
    return 2LL * static_cast<long long>(m) * static_cast<long long>(n) * static_cast<long long>(k);
}

// The solution is straightforward: the number of FLOPs for matrix multiplication of an `m x k` matrix and a `k x n` matrix using the standard algorithm is `m * n * k` multiplications and `m * n * k` additions, totaling `2 * m * n * k` operations. The main edge case is when any of the dimensions is zero or negative—in such cases, the product is undefined or trivial, so we return `0` to indicate no meaningful FLOPs. The function must use `long long` to safely accommodate large multiplication results (e.g., `m = 10000`, `n = 10000`, `k = 10000` gives `2e12`, which fits in `long long` but not `int`). The algorithm is `O(1)` in time and space since it performs only a few arithmetic operations. No loops or data structures are needed. The function is `const`-correct by taking parameters by value and not modifying any external state.
