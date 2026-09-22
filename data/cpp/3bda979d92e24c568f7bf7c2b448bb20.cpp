// Write a C++ function named `addSaturatedFloat` that takes a pointer to a constant source array of `float`, a pointer to a destination array of `float`, and a length `n` as a 16-bit signed integer. The function must perform element-wise saturated addition: for each index `i` from 0 to n-1, compute `src[i] + dst[i]`, clamp the result to the valid range of `float` (i.e., between `-FLT_MAX` and `FLT_MAX`), and store the clamped result back into `dst[i]`. The function should modify the destination array in place and return `void`. You may assume both pointers are valid and point to at least `n` contiguous elements. The length `n` will always be non-negative; if `n` is zero, the function should do nothing. Use `std::numeric_limits<float>::max()` to obtain the maximum finite float value and clamp using `std::min` and `std::max`. Ensure the function is const-correct: the source pointer must be `const float*` and the destination pointer must be `float*` (non-const). The function must compile in standard C++ without external libraries beyond the standard headers.
The core task is a straightforward element-wise sum with saturation. The main algorithm is a simple loop that iterates `n` times. In each iteration, we read the source value and the current destination value, compute their sum, and then clamp the sum to the range `[-FLT_MAX, FLT_MAX]`. The clamping is done by taking the maximum of the sum with `-FLT_MAX` and then the minimum of that result with `FLT_MAX`. This handles both positive and negative overflow. Since `float` arithmetic can produce `+inf` or `-inf` when adding large finite values (though this is rare for typical float inputs within the finite range, the clamp is still safe and will convert inf to `FLT_MAX` or `-FLT_MAX` correctly because `min(inf, FLT_MAX)` yields `FLT_MAX` and `max(-inf, -FLT_MAX)` yields `-FLT_MAX`). The loop runs exactly `n` times, so time complexity is O(n). Space complexity is O(1) as we only use a temporary variable for the sum. Edge cases: zero length does nothing, negative length is not expected (but function can ignore or handle safely; we assume non-negative). The function uses `std::numeric_limits<float>::max()` which is a compile-time constant `FLT_MAX`, but using the standard library is portable. No special handling is needed for NaN because adding NaN would yield NaN, and `min`/`max` behavior with NaN is implementation-defined; but the problem does not explicitly require NaN handling, and we can assume inputs are finite. If we wanted to be extra safe, we could check `std::isfinite`, but that is not required.
#include <algorithm>
#include <cstdint>
#include <limits>

// Adds src[i] to dst[i] for each i in [0, n), clamping the result to the
// valid range of finite float values. Modifies dst in place.
void addSaturatedFloat(const float* src, float* dst, std::int16_t n) {
    const float max_float = std::numeric_limits<float>::max();
    for (std::int16_t i = 0; i < n; ++i) {
        float sum = src[i] + dst[i];
        // Clamp to [-max_float, max_float]
        sum = std::max(-max_float, sum);
        sum = std::min(max_float, sum);
        dst[i] = sum;
    }
}
#include <cassert>
#include <cstdint>
#include <limits>

// Declaration of the function under test (must match the solution)
void addSaturatedFloat(const float* src, float* dst, std::int16_t n);

int main() {
    // Basic addition
    float src1[] = {1.0f, 2.0f, 3.0f};
    float dst1[] = {0.5f, -1.0f, 4.0f};
    addSaturatedFloat(src1, dst1, 3);
    assert(dst1[0] == 1.5f);
    assert(dst1[1] == 1.0f);
    assert(dst1[2] == 7.0f);

    // Zero length does nothing
    float src2[] = {10.0f};
    float dst2[] = {20.0f};
    addSaturatedFloat(src2, dst2, 0);
    assert(dst2[0] == 20.0f);

    // Clamping positive overflow
    float max_val = std::numeric_limits<float>::max();
    float src3[] = {max_val, max_val};
    float dst3[] = {max_val, -1.0f};
    addSaturatedFloat(src3, dst3, 2);
    assert(dst3[0] == max_val);
    assert(dst3[1] == max_val - 1.0f); // No overflow, sum is max-1 which is finite

    // Clamping negative overflow
    float min_val = -max_val;
    float src4[] = {min_val, min_val};
    float dst4[] = {min_val, 1.0f};
    addSaturatedFloat(src4, dst4, 2);
    assert(dst4[0] == min_val);
    assert(dst4[1] == min_val + 1.0f);

    // Mixed values with clamping to extremes
    float src5[] = {max_val, min_val};
    float dst5[] = {max_val, min_val};
    addSaturatedFloat(src5, dst5, 2);
    assert(dst5[0] == max_val); // sums to 2*max -> clamps to max
    assert(dst5[1] == min_val); // sums to 2*min -> clamps to min

    // Single element
    float src6[] = {5.0f};
    float dst6[] = {-3.0f};
    addSaturatedFloat(src6, dst6, 1);
    assert(dst6[0] == 2.0f);
}
