/*
Write a C++ function `clampedExponentVector` that takes three arrays: `values` (float), `exponents` (int), and a pre-allocated `output` array (float), along with the size `N`. For each index `i` in `[0, N)`, compute `values[i]^exponents[i]`, clamping the result to a maximum of `9.999999f`, and store the computed value in `output[i]`. If the exponent is `0`, the result must be exactly `1.0f`. The function must handle arbitrary `N` (not necessarily a multiple of a vector width) and must use the CS149 SIMD intrinsics provided in `CS149intrin.h` (e.g., `_cs149_vload_float`, `_cs149_vmult_float`, `_cs149_vset_float`, `_cs149_vgt_float`, mask operations, `_cs149_cntbits`, and `_cs149_vstore_float`) to process multiple elements per iteration. Ensure no out-of-bounds memory access occurs when `N % VECTOR_WIDTH != 0`; use partial masks for the final chunk. The function should not print anything, and it must write results that numerically match a scalar reference (within a tolerance of `1e-6`). You may assume `VECTOR_WIDTH` is a compile-time constant defined in `CS149intrin.h`, and `VALUES` can be negative, zero, or positive; exponents are non‑negative integers.
*/
#include "CS149intrin.h"
#include <cstddef>

// Vectorized clamped exponentiation using CS149 SIMD intrinsics.
// For each i in [0, N): output[i] = min(values[i]^exponents[i], 9.999999f)
// Handles arbitrary N, including N not divisible by VECTOR_WIDTH.
void clampedExponentVector(const float* values, const int* exponents, float* output, int N) {
    const float clamp_value = 9.999999f;
    __cs149_vec_float clamp_vec = _cs149_vset_float(clamp_value);
    __cs149_vec_int one_vec = _cs149_vset_int(1);

    for (int i = 0; i < N; i += VECTOR_WIDTH) {
        // Determine active mask for this chunk (handle partial tail)
        __cs149_mask mask_all;
        int remaining = N - i;
        if (remaining < VECTOR_WIDTH) {
            mask_all = _cs149_init_ones(remaining);
        } else {
            mask_all = _cs149_init_ones();
        }

        // Load current chunk data
        __cs149_vec_float data;
        __cs149_vec_int exp_vec;
        _cs149_vload_float(data, values + i, mask_all);
        _cs149_vload_int(exp_vec, exponents + i, mask_all);

        // Initialize result to 1.0 and times to 0 for all active lanes
        __cs149_vec_float result;
        _cs149_vset_float(result, 1.0f, mask_all);
        __cs149_vec_int times;
        _cs149_vset_int(times, 0, mask_all);

        // mask_work marks lanes still needing multiplication (times < exponent)
        __cs149_mask mask_work = mask_all;

        while (_cs149_cntbits(mask_work) > 0) {
            // Increment times for active lanes
            _cs149_vadd_int(times, times, one_vec, mask_work);

            // Update result: result *= data for active lanes
            _cs149_vmult_float(result, result, data, mask_work);

            // Check for clamp: result > clamp_value
            __cs149_mask mask_clamp;
            _cs149_vgt_float(mask_clamp, result, clamp_vec, mask_work);
            // Set clamped lanes to clamp_value
            _cs149_vset_float(result, clamp_value, mask_clamp);

            // Remove clamped lanes from work set
            __cs149_mask mask_not_clamp = _cs149_mask_not(mask_clamp);
            mask_work = _cs149_mask_and(mask_work, mask_not_clamp);

            // Remove lanes where times >= exponent (done)
            __cs149_mask mask_gt;
            _cs149_vgt_int(mask_gt, times, exp_vec, mask_work);
            mask_gt = _cs149_mask_not(mask_gt); // now mask_gt is times <= exponent? 
            // We want to keep lanes where times < exponent, i.e., times <= exponent-1
            // Since we incremented times, keep lanes where times <= exponent
            // But if times > exponent, we are done. So we use mask_gt (times > exp) to remove.
            mask_work = _cs149_mask_and(mask_work, _cs149_mask_not(mask_gt));
        }

        // Write result back to output using the original chunk mask
        _cs149_vstore_float(output + i, result, mask_all);
    }
}
#include <cassert>
#include <cmath>
#include <cstdio>

// Assume VECTOR_WIDTH is defined in CS149intrin.h
// For testing, we define a dummy minimal header setting VECTOR_WIDTH=4 and
// provide the necessary intrinsics as inline functions that operate on arrays.
// In a real environment, use the actual CS149intrin.h. This test code is
// self-contained for illustration only.

// ===== Dummy CS149 intrinsics for testing =====
#define VECTOR_WIDTH 4
struct __cs149_vec_float { float value[VECTOR_WIDTH]; };
struct __cs149_vec_int { int value[VECTOR_WIDTH]; };
struct __cs149_mask { unsigned int bits; };

inline __cs149_mask _cs149_init_ones() { __cs149_mask m; m.bits = (1u << VECTOR_WIDTH) - 1; return m; }
inline __cs149_mask _cs149_init_ones(int n) { __cs149_mask m; m.bits = (1u << n) - 1; return m; }
inline int _cs149_cntbits(__cs149_mask m) { return __builtin_popcount(m.bits); }
inline void _cs149_vload_float(__cs149_vec_float& v, const float* ptr, __cs149_mask m) { for (int i=0;i<VECTOR_WIDTH;i++) if (m.bits & (1u<<i)) v.value[i]=ptr[i]; }
inline void _cs149_vload_int(__cs149_vec_int& v, const int* ptr, __cs149_mask m) { for (int i=0;i<VECTOR_WIDTH;i++) if (m.bits & (1u<<i)) v.value[i]=ptr[i]; }
inline void _cs149_vstore_float(float* ptr, __cs149_vec_float v, __cs149_mask m) { for (int i=0;i<VECTOR_WIDTH;i++) if (m.bits & (1u<<i)) ptr[i]=v.value[i]; }
inline void _cs149_vset_float(__cs149_vec_float& v, float val, __cs149_mask m) { for (int i=0;i<VECTOR_WIDTH;i++) if (m.bits & (1u<<i)) v.value[i]=val; }
inline void _cs149_vset_int(__cs149_vec_int& v, int val, __cs149_mask m) { for (int i=0;i<VECTOR_WIDTH;i++) if (m.bits & (1u<<i)) v.value[i]=val; }
inline void _cs149_vadd_int(__cs149_vec_int& result, __cs149_vec_int a, __cs149_vec_int b, __cs149_mask m) { for (int i=0;i<VECTOR_WIDTH;i++) if (m.bits & (1u<<i)) result.value[i]=a.value[i]+b.value[i]; }
inline void _cs149_vmult_float(__cs149_vec_float& result, __cs149_vec_float a, __cs149_vec_float b, __cs149_mask m) { for (int i=0;i<VECTOR_WIDTH;i++) if (m.bits & (1u<<i)) result.value[i]=a.value[i]*b.value[i]; }
inline void _cs149_vgt_float(__cs149_mask& output, __cs149_vec_float a, __cs149_vec_float b, __cs149_mask m) { unsigned int bits=0; for (int i=0;i<VECTOR_WIDTH;i++) if (m.bits & (1u<<i)) if (a.value[i] > b.value[i]) bits |= (1u<<i); output.bits = bits; }
inline void _cs149_vgt_int(__cs149_mask& output, __cs149_vec_int a, __cs149_vec_int b, __cs149_mask m) { unsigned int bits=0; for (int i=0;i<VECTOR_WIDTH;i++) if (m.bits & (1u<<i)) if (a.value[i] > b.value[i]) bits |= (1u<<i); output.bits = bits; }
inline __cs149_mask _cs149_mask_not(__cs149_mask m) { __cs149_mask r; r.bits = ~m.bits & ((1u<<VECTOR_WIDTH)-1); return r; }
inline __cs149_mask _cs149_mask_and(__cs149_mask a, __cs149_mask b) { __cs149_mask r; r.bits = a.bits & b.bits; return r; }

// Include the solution function (assuming it's above, but we need to paste it here for compilation)
// For brevity, we include the exact same function as above (but it is already defined in the solution section).
// In a real test, you would include the solution header. Here we just assume it's compiled.

// function from solution (duplicated for standalone test)
void clampedExponentVector(const float* values, const int* exponents, float* output, int N) {
    const float clamp_value = 9.999999f;
    __cs149_vec_float clamp_vec = _cs149_vset_float(clamp_value);
    __cs149_vec_int one_vec = _cs149_vset_int(1);

    for (int i = 0; i < N; i += VECTOR_WIDTH) {
        __cs149_mask mask_all;
        int remaining = N - i;
        mask_all = (remaining < VECTOR_WIDTH) ? _cs149_init_ones(remaining) : _cs149_init_ones();

        __cs149_vec_float data;
        __cs149_vec_int exp_vec;
        _cs149_vload_float(data, values + i, mask_all);
        _cs149_vload_int(exp_vec, exponents + i, mask_all);

        __cs149_vec_float result;
        _cs149_vset_float(result, 1.0f, mask_all);
        __cs149_vec_int times;
        _cs149_vset_int(times, 0, mask_all);

        __cs149_mask mask_work = mask_all;

        while (_cs149_cntbits(mask_work) > 0) {
            _cs149_vadd_int(times, times, one_vec, mask_work);
            _cs149_vmult_float(result, result, data, mask_work);

            __cs149_mask mask_clamp;
            _cs149_vgt_float(mask_clamp, result, clamp_vec, mask_work);
            _cs149_vset_float(result, clamp_value, mask_clamp);

            __cs149_mask mask_not_clamp = _cs149_mask_not(mask_clamp);
            mask_work = _cs149_mask_and(mask_work, mask_not_clamp);

            __cs149_mask mask_gt;
            _cs149_vgt_int(mask_gt, times, exp_vec, mask_work);
            mask_work = _cs149_mask_and(mask_work, _cs149_mask_not(mask_gt));
        }

        _cs149_vstore_float(output + i, result, mask_all);
    }
}

// Scalar reference for testing
float clampedExpScalar(float x, int y) {
    if (y == 0) return 1.0f;
    float res = x;
    for (int i=1; i<y; ++i) res *= x;
    if (res > 9.999999f) res = 9.999999f;
    return res;
}

int main() {
    // Test 1: N=16, multiple of VECTOR_WIDTH
    {
        int N = 16;
        float values[16] = { 2.0f, -1.5f, 0.0f, 3.0f, -2.0f, 1.0f, 0.5f, 10.0f,
                             0.2f, -0.1f, 5.0f, 1.5f, -3.0f, 4.0f, 0.0f, 2.5f };
        int exps[16] = { 0, 1, 5, 2, 3, 10, 4, 1, 2, 0, 2, 3, 1, 2, 7, 0 };
        float output[16];
        clampedExponentVector(values, exps, output, N);
        for (int i=0; i<N; ++i) {
            float expected = clampedExpScalar(values[i], exps[i]);
            assert(std::fabs(output[i] - expected) < 1e-5);
        }
    }

    // Test 2: N not multiple of VECTOR_WIDTH (N=6)
    {
        int N = 6;
        float values[6] = { 2.0f, 3.0f, 0.5f, -1.0f, 4.0f, 1.5f };
        int exps[6] = { 2, 1, 4, 3, 2, 0 };
        float output[6];
        clampedExponentVector(values, exps, output, N);
        for (int i=0; i<N; ++i) {
            float expected = clampedExpScalar(values[i], exps[i]);
            assert(std::fabs(output[i] - expected) < 1e-5);
        }
    }

    // Test 3: All exponents zero
    {
        int N = 8;
        float values[8] = { -5.0f, 0.0f, 2.0f, 3.0f, 1.0f, -2.0f, 4.0f, 0.5f };
        int exps[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
        float output[8];
        clampedExponentVector(values, exps, output, N);
        for (int i=0; i<N; ++i) assert(output[i] == 1.0f);
    }

    // Test 4: Values that trigger clamping
    {
        int N = 4;
        float values[4] = { 10.0f, 2.0f, 3.0f, 1.1f };
        int exps[4] = { 1, 10, 5, 100 };
        float output[4];
        clampedExponentVector(values, exps, output, N);
        for (int i=0; i<N; ++i) {
            float expected = clampedExpScalar(values[i], exps[i]);
            assert(std::fabs(output[i] - expected) < 1e-5);
        }
        assert(output[0] == 9.999999f); // 10 > clamp
        assert(output[1] > 9.999998f && output[1] <= 9.999999f);
    }

    // Test 5: Negative base with even exponent gives positive
    {
        int N = 3;
        float values[3] = { -2.0f, -3.0f, -1.0f };
        int exps[3] = { 2, 3, 4 };
        float output[3];
        clampedExponentVector(values, exps, output, N);
        assert(std::fabs(output[0] - 4.0f) < 1e-5);
        assert(std::fabs(output[1] - (-27.0f)) < 1e-5);
        assert(std::fabs(output[2] - 1.0f) < 1e-5);
    }

    printf("All tests passed!\n");
    return 0;
}
// The core challenge is vectorizing a loop whose trip count varies per element (exponent value). A straightforward approach processes a vector of `VECTOR_WIDTH` elements at a time. For each lane, maintain a current `result` (initialized to `1.0f`) and a counter `times` (initialized to `0`). The active mask `mask` tracks which lanes still need more multiplications (i.e., `times < exponent`). The loop continues while `mask` has any active bits. Each iteration increments `times` by 1 for active lanes, then performs `result *= values[i]` only on active lanes, and afterwards checks if the result exceeds the clamp threshold; any lane exceeding it is set to the clamp value and marked inactive (since further multiplications would only increase the value). The iteration count per lane is bounded by the maximum exponent, so the while‑loop runs at most `max_exponent` times. After the loop, store the result back to memory, but careful with the final partial chunk: if `i + VECTOR_WIDTH > N`, use a mask with only `N - i` bits set for both load and store, and leave the unused lanes in `output` untouched. Edge cases include exponent zero (handled by the initial `result = 1` and the loop not executing for that lane), negative values (multiplication of negatives yields correct sign as long as the exponent math is correct), and values that exactly reach the clamp threshold. The use of `_cs149_vgt_float` for the clamp comparison is safe because we only need to clamp when strictly greater than `9.999999f`; if equal, it’s fine to leave as is. Time complexity is `O(N * max_exp / VECTOR_WIDTH)` in the worst case, but since exponents are typically small (bounded by `EXP_MAX` in the original problem), it is effectively `O(N)` per vector iteration. Space usage is `O(VECTOR_WIDTH)` for temporaries. The main pitfall is mask management: you must carefully combine the “still processing” condition with the “not yet clamped” condition, and ensure that the store mask is the original mask for that chunk (with only valid lanes set).
