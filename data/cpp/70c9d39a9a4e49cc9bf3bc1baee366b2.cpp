// Write a C++ function named `sha256RoundSimulation` that takes no arguments and returns a `uint32_t` bitmask indicating which of the three ARMv8 SHA-256 acceleration intrinsics (`vsha256hq_u32`, `vsha256h2q_u32`, and `vsha256su1q_u32`) are available to compile and call on the current platform. The function must attempt to compile and execute a minimal invocation of each intrinsic using a local `uint32x4_t` vector initialized to all zeros, and return a bitmask where bit 0 is set if the first intrinsic (`vsha256hq_u32`) compiled and ran without issue, bit 1 for the second (`vsha256h2q_u32`), and bit 2 for the third (`vsha256su1q_u32`). The function must not include any preprocessor directives that would bypass the actual intrinsic calls; instead, use a runtime detection approach that wraps each intrinsic call inside a separate lambda that captures the result and checks for a successful execution (e.g., by verifying the returned vector is not all zeros after applying a constant shift, though strictly the task is to confirm compilability and basic execution). To ensure portability, the function should gracefully handle platforms where these intrinsics are not available by returning 0, but on platforms where they are available, it should call each intrinsic exactly once and return the appropriate bitmask. Assume the header `<arm_neon.h>` is conditionally included as in the snippet; your function must be self-contained and must include necessary headers, but must not use any `#if` or `#ifdef` guards to avoid calling the intrinsics—use only standard C++ constructs and a compile-time constant to indicate availability, but since you cannot know at runtime if the compiler supports them, your function should simply attempt to call them directly and return the mask; however, for the purpose of this task, you may assume the intrinsics are available only when `__ARM_NEON` is defined, and in that case you must call them, otherwise return 0. Implement the function with proper `const` correctness and no `main` function, and provide a `constexpr` integer constant `kAvailableMask` that is 0b111 if `__ARM_NEON` is defined, otherwise 0, and use that constant to decide whether to call the intrinsics. The function must be named exactly `sha256RoundSimulation` and must return `uint32_t`.
// The core challenge is to write a function that attempts to use three ARMv8 SHA-256 intrinsics, each of which requires a `uint32x4_t` vector (a 128-bit NEON vector of four 32-bit unsigned integers). These intrinsics are only available when the compiler targets ARMv8 with NEON and SHA extensions enabled, and they are guarded by preprocessor macros like `__ARM_NEON` and `__ARM_FEATURE_CRYPTO`. However, the task explicitly prohibits using `#if` or `#ifdef` to guard the calls; instead we use a compile-time constant `kAvailableMask` that is conditionally set via the preprocessor (which is allowed outside the function body, not inside, but the task says "must not include any preprocessor directives that would bypass the actual intrinsic calls" — meaning we cannot use them inside the function to skip calls; we can use them outside to define a constant). The function receives `kAvailableMask` and, if that mask is non-zero, it calls the intrinsics directly on a zero-initialized vector. Since the input is all zeros, the SHA-256 intrinsics may produce undefined or zero output; but the task is to confirm the code compiles and runs, so we simply call each intrinsic and assign the result back to `y`, then compute a checksum by XORing all four lanes and OR-ing with a known constant to distinguish if calls were made. For simplicity, we can set a local `bool` or accumulate a mask based on `kAvailableMask`. Edge cases: On non-ARM platforms or if NEON is not enabled, `kAvailableMask` is 0, so we return 0 without calling any intrinsic. The function must be `const` correct: it takes no arguments, so we can mark it `uint32_t sha256RoundSimulation() const` but since it's a free function, we just return a `uint32_t`. Time complexity is O(1) as it performs a constant number of operations. Space complexity is O(1) as only a few local variables.
#include <stdint.h>
// The header may be conditionally included by the build system, but we include it unconditionally for this task;
// however, if __ARM_NEON is not defined, the intrinsics are not declared, so we must avoid calling them.
// We use a compile-time constant derived from the preprocessor (allowed outside the function body) to decide at compile time.
#if defined(__ARM_NEON) && defined(__ARM_FEATURE_CRYPTO)
# include <arm_neon.h>
constexpr uint32_t kAvailableMask = 0b111;
#else
constexpr uint32_t kAvailableMask = 0;
#endif

// Return a bitmask indicating which SHA-256 intrinsics are available and correctly compile/call.
// Bit 0: vsha256hq_u32, bit 1: vsha256h2q_u32, bit 2: vsha256su1q_u32.
// On platforms without these intrinsics, the function returns 0.
uint32_t sha256RoundSimulation() {
    uint32_t resultMask = 0;
    if (kAvailableMask != 0) {
        uint32x4_t y = {0, 0, 0, 0};
        // Call each intrinsic exactly once; if they compile and execute, the code path is taken.
        y = vsha256hq_u32(y, y, y);
        resultMask |= (1 << 0);
        y = vsha256h2q_u32(y, y, y);
        resultMask |= (1 << 1);
        y = vsha256su1q_u32(y, y, y);
        resultMask |= (1 << 2);
        // Prevent compiler from optimizing away the side-effect-free calls.
        (void)y;
    }
    return resultMask;
}
#include <cassert>
#include <cstdint>

// The solution function must be declared before main.
// Provide the same definition here or include the solution file.
#if defined(__ARM_NEON) && defined(__ARM_FEATURE_CRYPTO)
# include <arm_neon.h>
constexpr uint32_t kAvailableMask = 0b111;
#else
constexpr uint32_t kAvailableMask = 0;
#endif

uint32_t sha256RoundSimulation() {
    uint32_t resultMask = 0;
    if (kAvailableMask != 0) {
        uint32x4_t y = {0, 0, 0, 0};
        y = vsha256hq_u32(y, y, y);
        resultMask |= (1 << 0);
        y = vsha256h2q_u32(y, y, y);
        resultMask |= (1 << 1);
        y = vsha256su1q_u32(y, y, y);
        resultMask |= (1 << 2);
        (void)y;
    }
    return resultMask;
}

int main() {
    // The test must be runnable on any platform, so we check that the returned mask matches the compile-time constant.
    assert(sha256RoundSimulation() == kAvailableMask);
    // If the macro is defined, the mask must be exactly 0b111; otherwise 0.
    #if defined(__ARM_NEON) && defined(__ARM_FEATURE_CRYPTO)
        assert(kAvailableMask == 0b111);
        assert(sha256RoundSimulation() == 7);
    #else
        assert(kAvailableMask == 0);
        assert(sha256RoundSimulation() == 0);
    #endif
    return 0;
}
