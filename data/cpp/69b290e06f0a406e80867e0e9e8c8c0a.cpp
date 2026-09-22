// Write a C++ function named `extractBinaryBits` that accepts an unsigned 64-bit integer (`uint64_t`) and returns an `unsigned fract` value (a 64-bit fixed-point fractional type defined in this task as a custom struct with a single `uint64_t` member) by performing a bit-level reinterpretation of the input’s bit patterns. The function must exactly preserve all 64 bits of the input, treating them as the raw binary representation of the fractional number, without any arithmetic conversion or sign reinterpretation. The output type `unsigned fract` is a fixed-point type where the entire 64-bit value represents a fraction in the range [0,1) when interpreted as `value / 2^64`. The returned value must be bit‑identical to the input, so the solution must rely on `memcpy` or a union-based type‑punning approach (since `reinterpret_cast` between unrelated types is undefined behavior). The function must be marked `constexpr` to allow compile‑time evaluation, and it must be robust for all possible input bit patterns, including all zeros, all ones, and any arbitrary pattern. No overflow or conversion checks are needed, since the operation is purely a bit‑copy.
The core challenge is safely reinterpreting the bit pattern of a `uint64_t` as an `unsigned fract` without invoking undefined behavior through strict aliasing violations. The cleanest and most portable method is to use `memcpy`, which is defined by the C++ standard to copy the object representation bytes from one type to another. Alternatively, a union-based approach is also legal in practice, but `memcpy` is preferred because unions are technically only safe for reading the member that was last written (though most compilers support type‑punning via unions as an extension). The solution will define a custom `unsigned fract` struct with a single `uint64_t` data member (to mimic the fixed‑point type from the snippet, which is opaque). Then the function uses `std::memcpy` to copy the bytes of the input `uint64_t` into the output `unsigned fract` object. Since the struct is exactly 8 bytes and has standard layout, this is well‑defined. Edge cases: all‑zero input produces an `unsigned fract` whose underlying bits are zero, which when interpreted as a fraction equals 0.0. All‑ones input produces the largest possible fraction just below 1.0 (i.e., `0xFFFFFFFFFFFFFFFF / 2^64`). No special handling is required because the copy is exact for every pattern. Time complexity is O(1) (fixed‑size copy), space complexity is O(1). The function is `constexpr` only if the compiler supports constant expressions with `memcpy` (C++20 and later), otherwise remove `constexpr`; we'll keep it `constexpr` for modern compilers and note that a fallback with `std::bit_cast` (C++20) is even simpler, but we’ll stick to `memcpy` to avoid C++20 dependency.
#include <cstdint>
#include <cstring>

// A fixed-point fractional type holding 64 bits, representing value / 2^64.
struct unsigned_fract {
    uint64_t bits;
};

// Reinterpret the raw bits of a uint64_t as an unsigned_fract.
// This is a pure bit-copy; no arithmetic conversion or sign handling occurs.
constexpr unsigned_fract extractBinaryBits(uint64_t x) noexcept {
    unsigned_fract result{};
    // Use memcpy to safely copy the object representation.
    // This is well-defined for standard-layout types of identical size.
    std::memcpy(&result.bits, &x, sizeof(x));
    return result;
}
#include <cassert>
#include <cstdint>

// The solution function is declared above. Include it here for completeness.
struct unsigned_fract {
    uint64_t bits;
};

constexpr unsigned_fract extractBinaryBits(uint64_t x) noexcept;

int main() {
    // Test 1: All zero bits should produce zero fraction.
    unsigned_fract zero = extractBinaryBits(0ULL);
    assert(zero.bits == 0ULL);

    // Test 2: All one bits should preserve the exact bit pattern.
    unsigned_fract ones = extractBinaryBits(0xFFFFFFFFFFFFFFFFULL);
    assert(ones.bits == 0xFFFFFFFFFFFFFFFFULL);

    // Test 3: A specific pattern (0x123456789ABCDEF0) must be preserved exactly.
    unsigned_fract pattern = extractBinaryBits(0x123456789ABCDEF0ULL);
    assert(pattern.bits == 0x123456789ABCDEF0ULL);

    // Test 4: Alternating bits.
    unsigned_fract alt = extractBinaryBits(0xAAAAAAAAAAAAAAAAULL);
    assert(alt.bits == 0xAAAAAAAAAAAAAAAAULL);

    // Test 5: Only the highest bit set.
    unsigned_fract high = extractBinaryBits(0x8000000000000000ULL);
    assert(high.bits == 0x8000000000000000ULL);

    // Test 6: Only the lowest bit set.
    unsigned_fract low = extractBinaryBits(0x0000000000000001ULL);
    assert(low.bits == 0x0000000000000001ULL);

    // Test 7: A non‑aligned random pattern (e.g., 0xDEADBEEFCAFEF00D).
    unsigned_fract random = extractBinaryBits(0xDEADBEEFCAFEF00DULL);
    assert(random.bits == 0xDEADBEEFCAFEF00DULL);

    // Test 8: Compile‑time constant expression ensures constexpr-ness.
    constexpr unsigned_fract compile_time = extractBinaryBits(0x0123456789ABCDEFULL);
    static_assert(compile_time.bits == 0x0123456789ABCDEFULL, "Compile‑time mismatch");

    return 0;
}
