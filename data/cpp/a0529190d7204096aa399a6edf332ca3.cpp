Write a C++ function `unsigned long long factorialProduct(int num)` that computes the product of all integers from 1 to `num` inclusive (i.e., the factorial) using the same iterative multiplication style as the provided snippet. The function must handle non-negative integers only. For `num = 0` or `num = 1`, return `1`. For `num >= 2`, compute the product by multiplying `num` by every integer from 1 up to `num-1` (exactly as the snippet does). Use an appropriate unsigned 64-bit type to reduce overflow risk for reasonable inputs, but note that overflow may occur for `num > 20`; you do not need to detect or handle overflow beyond the natural wrap-around of the type. The function must be `const`-correct (mark the parameter as `const` and the function itself as not modifying any external state) and must be self-contained with all necessary headers.

#include <cassert>

int main() {
    // Basic cases
    assert(factorialProduct(0) == 1ULL);
    assert(factorialProduct(1) == 1ULL);
    assert(factorialProduct(2) == 2ULL);
    assert(factorialProduct(3) == 6ULL);
    assert(factorialProduct(4) == 24ULL);
    assert(factorialProduct(5) == 120ULL);

    // Larger values that fit in 64-bit
    assert(factorialProduct(10) == 3628800ULL);
    assert(factorialProduct(15) == 1307674368000ULL);
    assert(factorialProduct(20) == 2432902008176640000ULL);

    // Verify a couple of larger results using known factorial values
    assert(factorialProduct(7) == 5040ULL);
    assert(factorialProduct(12) == 479001600ULL);
}

#include <cassert>

// Compute factorial of a non-negative integer using iterative multiplication.
// Returns 1 for num = 0 or num = 1.
unsigned long long factorialProduct(const int num) {
    assert(num >= 0);  // Only non-negative inputs are valid.
    if (num <= 1) {
        return 1ULL;
    }

    unsigned long long result = static_cast<unsigned long long>(num);
    for (int i = 1; i < num; ++i) {
        result *= static_cast<unsigned long long>(i);
    }
    return result;
}

// The algorithm directly follows the iterative factorial approach from the snippet: initialize the result to `num`, then loop `i` from 1 to `num-1`, multiplying the current result by `i` at each step. This computes `num * (num-1) * ... * 1` but in ascending order of multipliers, which yields the same product due to commutativity. The loop runs exactly `num-1` iterations, and within each iteration we do one multiplication and one increment, so the time complexity is `O(num)` and the auxiliary space is `O(1)`. Edge cases: `num = 0` must be handled specially because the loop would otherwise multiply by 1 and incorrectly return 0; we explicitly return 1 for both 0 and 1. Negative inputs are not allowed per the specification, but if a negative value is passed, the behavior is undefined or we could add an assertion; for safety, the implementation can include an `assert(num >= 0)` to catch misuse in debug builds. Using `unsigned long long` gives a maximum factorial representable up to 20! = 2,432,902,008,176,640,000, which fits since 21! exceeds 64-bit range (approx 5.1e19 > 1.8e19). For performance, no special optimizations are needed.
