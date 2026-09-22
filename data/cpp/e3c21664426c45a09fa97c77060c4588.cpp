Write a C++ function named `apply_gemm_post_ops` that simulates the core post-processing logic from the given GPU inner-product snippet. The function takes three vectors of `float`: `dst` (output values, size `mb * oc`), `bias` (size `oc`), and `scales` (size `oc`, may be empty for no scaling). Additionally, it takes a boolean `use_bias` (whether to add bias) and a boolean `use_scale` (whether to multiply by scales). The function must update `dst` in place by first adding bias (if enabled) and then multiplying by scales (if enabled), following the same order as the real kernel (bias then scale). The computation is: for each index `i` in `[0, mb*oc)`, `dst[i] = (dst[i] + (use_bias ? bias[i % oc] : 0.0f)) * (use_scale ? scales[i % oc] : 1.0f)`. Assume `mb > 0`, `oc > 0`, and the sizes of `bias` and `scales` are exactly `oc` when their respective flags are true. The function must be `const`-correct with respect to its parameters (i.e., only `dst` is modified). Return `bool` indicating success: `true` if inputs are valid (correct sizes), `false` otherwise. Do not include a `main` function.
The solution iterates once over the `dst` array. For each element, it determines the corresponding bias and scale index using the modulo operation `i % oc`. Even if `use_bias` or `use_scale` is false, we avoid out-of-bounds access by only reading from `bias`/`scales` when the flag is true, otherwise using a constant (0.0f for bias, 1.0f for scale). The order of operations matters: first add bias to the original value, then multiply by scale. The function must validate that `dst` is non-empty, and if `use_bias` is true then `bias.size() == oc`, if `use_scale` is true then `scales.size() == oc`. If any validation fails, return `false` without modifying `dst`. Time complexity is \(O(mb \cdot oc)\) because we touch each element exactly once. Space complexity is \(O(1)\) auxiliary, as only a few loop variables are needed. Edge cases: `oc` could be 1, causing all elements to share the same bias/scale; `mb` could be large, but the loop remains linear; `dst` may already contain data that we overwrite in place.
#include <vector>

// Apply bias and scale post-operations to a flattened matrix of shape (mb, oc).
// dst is modified in place. bias and scales have size oc. Returns false on invalid input.
bool apply_gemm_post_ops(std::vector<float>& dst,
                         const std::vector<float>& bias,
                         const std::vector<float>& scales,
                         bool use_bias,
                         bool use_scale,
                         size_t mb,
                         size_t oc) {
    if (dst.size() != mb * oc || mb == 0 || oc == 0) {
        return false;
    }
    if (use_bias && bias.size() != oc) {
        return false;
    }
    if (use_scale && scales.size() != oc) {
        return false;
    }

    for (size_t i = 0; i < dst.size(); ++i) {
        size_t idx = i % oc;
        float value = dst[i];
        if (use_bias) {
            value += bias[idx];
        }
        if (use_scale) {
            value *= scales[idx];
        }
        dst[i] = value;
    }
    return true;
}
#include <cassert>
#include <vector>

// Declaration of the solution function
bool apply_gemm_post_ops(std::vector<float>& dst,
                         const std::vector<float>& bias,
                         const std::vector<float>& scales,
                         bool use_bias,
                         bool use_scale,
                         size_t mb,
                         size_t oc);

int main() {
    // Basic test: both bias and scale
    std::vector<float> dst1 = {1.0f, 2.0f, 3.0f, 4.0f};
    std::vector<float> bias1 = {0.5f, -1.0f};
    std::vector<float> scales1 = {2.0f, 3.0f};
    bool ok1 = apply_gemm_post_ops(dst1, bias1, scales1, true, true, 2, 2);
    assert(ok1);
    assert(dst1.size() == 4);
    // (1+0.5)*2=3.0, (2-1)*3=3.0, (3+0.5)*2=7.0, (4-1)*3=9.0
    assert(dst1[0] == 3.0f);
    assert(dst1[1] == 3.0f);
    assert(dst1[2] == 7.0f);
    assert(dst1[3] == 9.0f);

    // Only bias
    std::vector<float> dst2 = {1.0f, 2.0f};
    std::vector<float> bias2 = {10.0f};
    std::vector<float> scales2;
    bool ok2 = apply_gemm_post_ops(dst2, bias2, scales2, true, false, 2, 1);
    assert(ok2);
    assert(dst2[0] == 11.0f && dst2[1] == 12.0f);

    // Only scale
    std::vector<float> dst3 = {5.0f, 6.0f};
    std::vector<float> bias3;
    std::vector<float> scales3 = {2.0f, 4.0f};
    bool ok3 = apply_gemm_post_ops(dst3, bias3, scales3, false, true, 1, 2);
    assert(ok3);
    assert(dst3[0] == 10.0f && dst3[1] == 24.0f);

    // No post-ops (no bias, no scale)
    std::vector<float> dst4 = {7.0f, 8.0f, 9.0f};
    std::vector<float> bias4;
    std::vector<float> scales4;
    bool ok4 = apply_gemm_post_ops(dst4, bias4, scales4, false, false, 3, 1);
    assert(ok4);
    assert(dst4[0] == 7.0f && dst4[1] == 8.0f && dst4[2] == 9.0f);

    // Invalid input: dst size mismatch
    std::vector<float> dst5 = {1.0f, 2.0f};
    std::vector<float> bias5 = {0.0f, 0.0f};
    std::vector<float> scales5 = {1.0f, 1.0f};
    bool ok5 = apply_gemm_post_ops(dst5, bias5, scales5, true, true, 2, 2);
    assert(!ok5);

    // Invalid input: bias size mismatch when use_bias true
    std::vector<float> dst6 = {1.0f, 2.0f, 3.0f, 4.0f};
    std::vector<float> bias6 = {0.0f};
    std::vector<float> scales6 = {1.0f, 1.0f};
    bool ok6 = apply_gemm_post_ops(dst6, bias6, scales6, true, true, 2, 2);
    assert(!ok6);

    // Invalid input: scales size mismatch when use_scale true
    std::vector<float> dst7 = {1.0f, 2.0f};
    std::vector<float> bias7 = {0.0f};
    std::vector<float> scales7 = {1.0f};
    bool ok7 = apply_gemm_post_ops(dst7, bias7, scales7, true, true, 2, 1);
    assert(!ok7);

    // Edge: oc=1, mb large
    std::vector<float> dst8(10, 2.0f);
    std::vector<float> bias8 = {3.0f};
    std::vector<float> scales8 = {4.0f};
    bool ok8 = apply_gemm_post_ops(dst8, bias8, scales8, true, true, 10, 1);
    assert(ok8);
    for (float v : dst8) {
        assert(v == (2.0f + 3.0f) * 4.0f);
    }

    return 0;
}
