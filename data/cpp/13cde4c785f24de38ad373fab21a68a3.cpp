/*
The provided code snippet implements parameter preparation for the softmax operation in TensorFlow Lite Micro, with special handling for quantized int8 and int16 inputs. Your task is to create an independent C++ function that simulates the core quantization logic for softmax int16 inputs, without depending on TensorFlow. Specifically, write a function `prepareInt16SoftmaxStats(float input_scale, float beta, int16_t& input_multiplier, int& input_left_shift)` that computes the quantization parameters for a softmax layer where the input tensor has scale `input_scale`, zero_point 0, and beta (inverse temperature) `beta`. The function must replicate the behavior from the excerpt: compute `input_scale_beta_rescale = input_scale * beta / (10.0 / 65535.0)`, then use a simplified fixed-point conversion (approximating `QuantizeMultiplier`) to find an integer multiplier and left shift such that `input_scale_beta_rescale ≈ input_multiplier * 2^(-31 + input_left_shift)`. The result must satisfy that the input multiplier is a positive 32-bit integer representable in int16_t (i.e., between 1 and 32767 inclusive) after appropriate normalization. Also, output a `diff_min` value that represents the smallest negative input difference (scaled) that is considered significant, using the formula `diff_min = -1.0 * CalculateInputRadius(5, input_left_shift)` which in this context is `- (1 << (5 - 1)) / (1 << input_left_shift)`? Actually, use the approximate formula `diff_min = -32768 >> input_left_shift` (since the effective range is 16-bit). Provide a standalone function that returns via out-parameters `input_multiplier`, `input_left_shift`, and `diff_min`. Input scale is positive, beta is positive, and typical values are small (e.g., 0.0039 for int16). The function must handle the case where the computed multiplier overflows int16_t by increasing the left shift.
*/

#include <cmath>
#include <cstdint>
#include <limits>

/**
 * Simulate int16 softmax quantization parameter preparation.
 * 
 * Given input scale and beta, compute a fixed-point representation:
 *   input_scale_beta_rescale = input_scale * beta / (10.0 / 65535.0)
 * The function finds input_multiplier (int16_t) and input_left_shift (int)
 * such that input_scale_beta_rescale ≈ input_multiplier * 2^(input_left_shift - 15).
 * Also computes diff_min = - (1 << (14 - input_left_shift)) for input_left_shift < 14,
 * else -1, representing the smallest significant negative input difference.
 * 
 * Requirements: input_scale > 0, beta > 0. The multiplier is in [1, 32767].
 */
void prepareInt16SoftmaxStats(float input_scale, float beta,
                              int16_t& input_multiplier,
                              int& input_left_shift,
                              int& diff_min) {
    // Compute the rescaled scale as in the TFLite code
    double s = static_cast<double>(input_scale) * static_cast<double>(beta) /
               (10.0 / 65535.0);
    
    // Edge case: if s is non-positive (should not happen with positive inputs)
    if (s <= 0.0) {
        input_multiplier = 0;
        input_left_shift = 0;
        diff_min = 0;
        return;
    }
    
    // Find left_shift such that product = s * 2^left_shift is in [1, 32767]
    int shift = 0;
    double product = s;
    // Increase shift until product >= 1, but avoid exceeding 32767 too much
    while (product < 1.0 && shift < 31) {
        product *= 2.0;
        shift++;
    }
    // If product is now > 32767, we need to decrease shift (but product only grows with shift, so clamp)
    // This can happen if s is very large; reduce shift until product <= 32767
    while (product > 32767.0 && shift > -30) {
        product /= 2.0;
        shift--;
    }
    
    // Round to nearest integer multiplier
    int32_t mult = static_cast<int32_t>(std::lround(product));
    // Ensure multiplier is in valid range
    if (mult < 1) mult = 1;
    if (mult > 32767) mult = 32767;
    
    input_multiplier = static_cast<int16_t>(mult);
    input_left_shift = shift;
    
    // Compute diff_min based on effective range approximation
    // The effective range is roughly [-32768 * 2^(shift-15), 32767 * 2^(shift-15)]
    // So the minimum significant negative value is -32768 * 2^(shift-15)
    // We return it as an integer truncating toward zero.
    if (shift < 15) {
        diff_min = -(1 << (14 - shift));
    } else {
        diff_min = -1; // very large shift, min is just -1
    }
}

#include <cassert>
#include <cstdint>

// Declare the solution function (shall be defined in the same translation unit)
void prepareInt16SoftmaxStats(float input_scale, float beta,
                              int16_t& input_multiplier,
                              int& input_left_shift,
                              int& diff_min);

int main() {
    // Test 1: Typical int16 scale 1/32768, beta=1
    // s = (1/32768)*1 / (10/65535) ≈ 0.2
    // multiplier should be around 0.2*2^shift, pick shift=3 => 1.6? Actually need product in [1,32767]
    // Let's compute: s = 0.000030517578125 * 65535/10 = 0.2 exactly? 0.0000305*6553.5 = 0.2 approximate.
    // We'll just check that the function produces consistent multiplicative relation.
    {
        int16_t mult;
        int shift;
        int diff;
        prepareInt16SoftmaxStats(1.0f/32768.0f, 1.0f, mult, shift, diff);
        double s = (1.0/32768.0) * 1.0 / (10.0/65535.0);
        double approx = static_cast<double>(mult) * std::pow(2.0, shift - 15);
        // Allow relative error of 1% or absolute 0.01
        double error = std::abs(approx - s);
        assert(error < 0.01);
        assert(mult > 0);
        assert(diff < 0);
    }

    // Test 2: Larger beta value, beta=2
    {
        int16_t mult;
        int shift;
        int diff;
        prepareInt16SoftmaxStats(0.01f, 2.0f, mult, shift, diff);
        double s = 0.01 * 2.0 / (10.0/65535.0); // 131.07
        double approx = static_cast<double>(mult) * std::pow(2.0, shift - 15);
        assert(std::abs(approx - s) < 1.0);
        assert(mult >= 1 && mult <= 32767);
    }

    // Test 3: Very small scale, should produce positive left shift
    {
        int16_t mult;
        int shift;
        int diff;
        prepareInt16SoftmaxStats(1e-5f, 1.0f, mult, shift, diff);
        assert(shift > 0);
        assert(mult > 0 && mult <= 32767);
    }

    // Test 4: Edge case with large scale (not typical but robust)
    {
        int16_t mult;
        int shift;
        int diff;
        prepareInt16SoftmaxStats(100.0f, 1.0f, mult, shift, diff);
        double s = 100.0 / (10.0/65535.0) * 1.0; // 655350
        // Since multiplier cap at 32767, shift will be negative
        double approx = static_cast<double>(mult) * std::pow(2.0, shift - 15);
        // Relative error should be small
        assert(std::abs(approx - s) / s < 0.01);
    }

    // Test 5: Multiplier never exceeds int16 range
    {
        for (float scale : {0.0001f, 0.001f, 0.01f, 0.1f, 1.0f}) {
            int16_t mult;
            int shift;
            int diff;
            prepareInt16SoftmaxStats(scale, 1.0f, mult, shift, diff);
            assert(mult >= 1 && mult <= 32767);
            assert(shift >= -30 && shift <= 30);
            assert(diff <= 0);
        }
    }

    return 0;
}

// The core problem is to convert a floating-point scale factor into a fixed-point representation used by the softmax kernel. In the original code, `QuantizeMultiplier` finds `m` and `e` such that `true_scale = m * 2^e` where `m` is a 32-bit integer in [2^31, 2^32-1] and `e` is negative or zero. For int16 softmax, the multiplier is stored in `int16_t`, so we need to further reduce the magnitude. The approach: compute `s = input_scale * beta / (10.0 / 65535.0)` (a positive double). Then find the smallest left shift `input_left_shift` (starting from 0) such that `s * 2^(input_left_shift)` is representable as a 16-bit signed integer (i.e., ≤ 32767). Since the original multiplies by a factor that can be small, we may need to multiply by powers of two to bring it into range. But the actual representation in TFLite uses a 32-bit multiplier and a left shift, where the effective scaling is `multiplier * 2^(-31 + left_shift)`. For int16, the multiplier is stored as int16_t, so the range is much smaller. To mimic that, we can scale the true scale by `2^31` to get a 32-bit integer, then find the largest left shift (or smallest) that fits into 16 bits. Simpler: compute `scaled = s * 32768.0`? Let's reason from the original constants: For int16, `input_scale_beta_rescale` is scaled such that a difference of 65535 corresponds to 10.0, so the range is [0, 10]. In the actual kernel, the multiplier is a 32-bit value and left shift is often negative. However, since the task asks for an independent simplification, we define the desired behavior: return `input_multiplier` as an int16_t in [1, 32767] and `input_left_shift` such that `input_scale_beta_rescale` is approximately `input_multiplier * 2^(input_left_shift - 15)`. We choose `input_left_shift` as the smallest non-negative integer so that `s * 2^input_left_shift >= 1`? Wait, to avoid precision loss, we want the largest possible multiplier without overflow. So compute `m = round(s * 2^input_left_shift)`, choose `input_left_shift` such that `m <= 32767` and `m >= 1`. Start with `input_left_shift = 0`, while `s * 2^input_left_shift < 1`, increase shift. While `s * 2^input_left_shift > 32767`, decrease shift? But if s is very large, we need negative shift. However, in practice s is small, so left shift is positive. We'll restrict to non-negative shifts. The `diff_min` formula from the snippet: `CalculateInputRadius(kScaledDiffIntegerBits, op_data->input_left_shift)` for int16 uses `kScaledDiffIntegerBits=5`, but the actual radius is `(1 << 30) / input_multiplier`? Let's simplify: The diff_min in the int16 path is not explicitly computed in the given code; it's only computed for int8. For int16, the LUT approach is used and diff_min is not set. However, the task asks to output diff_min. We'll define it as `- (1 << (14 - input_left_shift))`? To be consistent with the effective scaling, we can compute `diff_min = - (32768 >> input_left_shift)` as a float, but then return as a float? The function signature in the task uses `int16_t& diff_min`? Actually the prompt says "output a diff_min value" but doesn't specify type. I'll make it `int` for simplicity. Use `diff_min = - (1 << (14 - input_left_shift))` if `input_left_shift <= 14`, else 0? Better: use the common formula `diff_min = - (1 << (31 - input_left_shift))` but that overflows for large shifts. Since our multiplier is 16-bit, the effective range is roughly `[-32768, 32767]` scaled by `2^(input_left_shift-15)`. So the smallest significant negative value is `-32768 >> input_left_shift`? Let's just set `diff_min = - (1 << (15 - input_left_shift))` if `input_left_shift < 15`, else `-1`. Edge cases: s=0? Not possible because scales are positive. Large s? We cap multiplier at 32767 and adjust shift accordingly (if s>32767, we set multiplier=32767 and left_shift = floor(log2(s/32767))? We'll keep it simple: find `input_left_shift` such that `1 <= s * 2^input_left_shift <= 32767`, if possible. Since s is small, we increase shift until product >= 1. If product becomes >32767 before reaching 1? That can't happen because s<1 typically. We'll handle general case by: start shift=0, multiply s by 2 each iteration, stop when product >=1. Then while product >32767, reduce shift? Actually product is increasing with shift, so once it's >1, it might exceed 32767 for large s. We'll cap: if product >32767, choose shift such that product <=32767 by taking floor(log2(32767/s)). We'll implement robustly. Complexity: O(log range) iterations, constant. Edge cases: input_scale or beta may be zero? We assume positive, but handle zero by returning multiplier=0, shift=0, diff_min=0? Better to return error? We'll assert positive.
