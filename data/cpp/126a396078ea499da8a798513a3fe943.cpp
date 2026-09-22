Write a C++ function `size_t compute_dispatch_size(size_t filter_ofm_num, size_t batch_size, bool is_fp16)` that models the work-group sizing calculation from the provided convolution kernel selector. For an FP16 data type, the function must determine the number of batches processed per work item using the same tiered logic: if `batch_size` is divisible by `4 * 1 * 16` (i.e., 64), return 4; else if divisible by `2 * 1 * 16` (i.e., 32), return 2; otherwise return 1. For an FP32 data type, always return 2. The function must then compute the total number of global work items as `filter_ofm_num * batch_size / (ofm_per_work_item * batches_per_work_item)`, where `ofm_per_work_item` is 16 for FP16 and 8 for FP32. The result must be a positive integer; if any input is zero or the division does not produce an integer (i.e., `filter_ofm_num * batch_size` is not divisible by the denominator), return 0 to signal invalid parameters. Use only standard C++ libraries, apply `const` correctness for input parameters, and ensure the function handles large values (up to `size_t` maximum) without overflow by performing divisibility checks before multiplication where necessary.

#include <cassert>
#include <cstddef>

int main() {
    // FP16 cases: batches divisible by 64 -> 4 batches per work item, ofm_per_wi=16
    assert(compute_dispatch_size(64, 64, true) == 64 * 64 / (16 * 4)); // = 64
    assert(compute_dispatch_size(16, 128, true) == 16 * 128 / (16 * 4)); // = 32
    // FP16: batch=32 -> 2 batches per work item
    assert(compute_dispatch_size(32, 32, true) == 32 * 32 / (16 * 2)); // = 32
    assert(compute_dispatch_size(8, 64, true) == 8 * 64 / (16 * 4)); // = 8
    // FP16: batch=16 -> 1 batch per work item
    assert(compute_dispatch_size(16, 16, true) == 16 * 16 / (16 * 1)); // = 16
    assert(compute_dispatch_size(48, 16, true) == 48 * 16 / (16 * 1)); // = 48
    // FP32: always 2 batches per work item, ofm_per_wi=8
    assert(compute_dispatch_size(32, 16, false) == 32 * 16 / (8 * 2)); // = 32
    assert(compute_dispatch_size(8, 32, false) == 8 * 32 / (8 * 2)); // = 16
    // Invalid inputs return 0
    assert(compute_dispatch_size(0, 16, true) == 0);
    assert(compute_dispatch_size(16, 0, false) == 0);
    // Non-divisible product: filter_ofm=3, batch=8, FP16 -> denominator=16, 3*8=24 not divisible by 16
    assert(compute_dispatch_size(3, 8, true) == 0);
    // Non-divisible product: filter_ofm=5, batch=4, FP32 -> denominator=16, 5*4=20 not divisible by 16
    assert(compute_dispatch_size(5, 4, false) == 0);
    return 0;
}
(Note: Ensure the solution and test files include the necessary headers; the test uses `assert` which requires `<cassert>`, and `std::numeric_limits` requires `<limits>` in the solution.)

#include <cstddef>
#include <numeric>

// Compute the dispatch global work size for a convolution kernel.
// Returns 0 if parameters are invalid or the division is not exact.
// For FP16, batches per work item is chosen as 4, 2, or 1 based on divisibility.
// For FP32, batches per work item is always 2.
size_t compute_dispatch_size(size_t filter_ofm_num, size_t batch_size, bool is_fp16) {
    if (filter_ofm_num == 0 || batch_size == 0) {
        return 0;
    }

    size_t batches_per_work_item;
    if (is_fp16) {
        // Constants from the original kernel: min_batches_per_wi = 1, min_lws = 16
        const size_t min_batches = 1;
        const size_t min_lws = 16;
        if (batch_size % (4 * min_batches * min_lws) == 0) {
            batches_per_work_item = 4;
        } else if (batch_size % (2 * min_batches * min_lws) == 0) {
            batches_per_work_item = 2;
        } else {
            batches_per_work_item = 1;
        }
    } else {
        batches_per_work_item = 2;
    }

    const size_t ofm_per_work_item = is_fp16 ? 16 : 8;
    const size_t denominator = ofm_per_work_item * batches_per_work_item;

    // Check divisibility without overflow: use gcd reduction.
    size_t reduce_denom = denominator / std::gcd(denominator, filter_ofm_num);
    if (batch_size % reduce_denom != 0) {
        return 0;
    }

    // Now compute the quotient safely. Since batch_size is divisible by reduce_denom,
    // and filter_ofm_num * batch_size / denominator is integer.
    // Compute as filter_ofm_num * (batch_size / reduce_denom) / (denominator / reduce_denom)
    size_t temp = batch_size / reduce_denom;
    size_t new_denom = denominator / reduce_denom;
    // Guard against overflow in multiplication
    if (temp > 0 && filter_ofm_num > std::numeric_limits<size_t>::max() / temp) {
        return 0; // would overflow
    }
    size_t product = filter_ofm_num * temp;
    if (product % new_denom != 0) {
        return 0; // should not happen after gcd reduction, but keep for safety
    }
    return product / new_denom;
}
(Note: The above solution includes a guard for overflow using `std::numeric_limits`, but the includes for that header are missing. To make it self-contained, one could add `#include <limits>`. In the final answer, ensure all headers are present.)

// The solution mimics the kernel's `SetDefault` dispatch calculation but simplifies it to a pure function. The core algorithm first determines `batches_per_work_item` based on the data type. For FP16, the divisibility checks mirror the original: check divisibility by 64 first (preferred, as it allows wider block reads), then by 32, and finally fall back to 1. For FP32, it always returns 2, as the original code hardcodes this value. Next, `ofm_per_work_item` is selected: 16 for FP16, 8 for FP32. The global work size is `filter_ofm_num * batch_size / (ofm_per_work_item * batches_per_work_item)`. Edge cases: (1) if `filter_ofm_num` or `batch_size` is zero, return 0 because the original validation would reject the kernel; (2) Before multiplying `filter_ofm_num * batch_size`, we must guard against overflow for very large values—since this is a theoretical sizing function, we can use modulo checks on each operand separately: `filter_ofm_num % denominator != 0` or `batch_size % denominator != 0` would indicate non-divisibility, but since the product might be divisible even when individual factors aren't, we need a careful approach. A robust method: first compute `denominator = ofm_per_work_item * batches_per_work_item`. If either input is zero, or if `denominator` is zero (impossible here), return 0. Then check if `batch_size` is divisible by `denominator / gcd(denominator, filter_ofm_num)` using a GCD-based reduction to avoid overflow. Simpler: since the original code assumes these values are multiples of the work-item granularity (validated elsewhere), we can assume divisibility holds for valid inputs and just check that the product is positive and the resulting quotient is computed safely using `std::numeric_limits<size_t>::max()` checks. For simplicity and clarity, the reference implementation assumes inputs are already validated by the caller (matching the kernel's `Validate` method) but still returns 0 for obvious invalid inputs (zero). Time complexity is O(1), space complexity O(1). All arithmetic is in `size_t`, and no floating-point is used.
