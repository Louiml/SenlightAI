// Write a C++ function named `validate_pooling_tensor` that takes four integer parameters: `features`, `batch`, `feature_block_size`, and `batch_block_size`. The function must return `true` if both `features` and `batch` are non-negative multiples of their respective block sizes, and `false` otherwise. The function should also reject any negative input values regardless of divisibility. For example, inputs where either dimension is divisible by its block size but is negative must return `false`. Provide a standalone implementation that can be compiled and tested independently.
// The solution is straightforward: validate each dimension independently. For `features`, check that it is greater than or equal to zero and that `features % feature_block_size == 0`. Similarly, for `batch`, check it is non-negative and divisible by `batch_block_size`. The function must short-circuit safely: if any dimension is negative, return `false` immediately because negative values are always invalid. Edge cases include zero values—zero is a valid multiple of any positive block size (0 % k == 0), so zero should pass if block sizes are positive. However, block sizes themselves should be positive; if a block size is zero or negative, the function should return `false` to avoid undefined behavior from modulo by zero or negative modulus semantics. The algorithm runs in O(1) time and O(1) space, checking a constant number of conditions.
#include <cstddef>

// Validates that feature and batch dimensions are non-negative and
// multiples of their respective block sizes. Block sizes must be positive.
bool validate_pooling_tensor(int features, int batch,
                             int feature_block_size, int batch_block_size) {
    // Block sizes must be strictly positive to avoid modulo by zero or negative.
    if (feature_block_size <= 0 || batch_block_size <= 0) {
        return false;
    }

    // Reject negative dimensions; zero is acceptable as a multiple.
    if (features < 0 || batch < 0) {
        return false;
    }

    // Check divisibility for each dimension.
    return (features % feature_block_size == 0) &&
           (batch % batch_block_size == 0);
}
#include <cassert>

int main() {
    // Basic valid cases
    assert(validate_pooling_tensor(16, 32, 16, 16) == true);
    assert(validate_pooling_tensor(0, 0, 16, 16) == true);
    assert(validate_pooling_tensor(16, 16, 16, 16) == true);

    // Invalid divisibility
    assert(validate_pooling_tensor(17, 32, 16, 16) == false);
    assert(validate_pooling_tensor(16, 33, 16, 16) == false);
    assert(validate_pooling_tensor(17, 33, 16, 16) == false);

    // Negative dimensions are rejected even if divisible
    assert(validate_pooling_tensor(-16, 32, 16, 16) == false);
    assert(validate_pooling_tensor(16, -32, 16, 16) == false);
    assert(validate_pooling_tensor(-16, -32, 16, 16) == false);

    // Non-positive block sizes are rejected
    assert(validate_pooling_tensor(16, 32, 0, 16) == false);
    assert(validate_pooling_tensor(16, 32, 16, -1) == false);
    assert(validate_pooling_tensor(16, 32, -16, 16) == false);

    // Different block sizes
    assert(validate_pooling_tensor(8, 24, 8, 8) == true);
    assert(validate_pooling_tensor(16, 24, 8, 8) == true);
    assert(validate_pooling_tensor(12, 24, 8, 8) == false);

    return 0;
}
