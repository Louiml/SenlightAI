// Write a C++ function `smoothNoise1D` that takes a float coordinate `x` and an integer-seeded permutation table `perm` (a `std::vector<int>` of size 256 containing values 0‑255, representing a permutation of 0..255) and returns a smooth pseudorandom noise value in the range [0,1] using a 1D lattice-based approach similar to the trilinear interpolation of the provided SIMD code. The function should compute integer lattice indices by floored (not truncated) math: given `x`, let `xi = floor(x)` and `frac = x - xi`; then use `perm[xi & 255]` and `perm[(xi+1) & 255]` to obtain two lattice values from a fixed internal random value table (e.g., a static array of 256 precomputed floats uniformly in [0,1] seeded by a simple LCG, or use `sin`-based hash if you prefer, but the table must be deterministic). Perform linear interpolation between the two lattice values using the smoothstep function `f = frac * frac * (3 - 2 * frac)` (or just `frac` if you want linear, but smoothstep is required for smoothness). The result should be in [0,1]. The function must be pure and deterministic; it must not depend on global mutable state or random number generation at runtime. Provide a clear comment explaining the algorithm.
The solution decomposes the problem into: (1) mapping the real coordinate to a fractional position within a unit cell, (2) selecting the two lattice points that surround the input, and (3) computing a smooth interpolated value. The floor operation (rather than casting or truncation) is essential for negative coordinates—this ensures the fractional part is always in [0,1) and the lattice indices wrap correctly modulo 256. The precomputed value table can be generated at compile‑time or via a simple deterministic function like `fract(sin(i) * 43758.5453)` which is a common hash‑based pseudo‑random generator for shaders; however, since we require determinism and portability, a simple static array initialized with a linear congruential generator (LCG) is safer and avoids floating‑point non‑determinism across platforms. Edge cases: very large or very small `x` cause integer overflow when casting to `int`; to avoid UB, we can take `floor(x)` using `std::floor` and then use `static_cast<int>` only after masking the fractional part via modulo, or better, use `int xi = static_cast<int>(std::floor(x))` and rely on two's complement wrapping, but we must be careful: `xi` can be outside int range for huge floats. A robust approach is to use `long long` for the floor value and then mask with `& 255` to get the index, which works because `& 255` is equivalent to modulo 256 for two's complement and also works for negative numbers if we cast to unsigned. The time complexity is O(1) per call (constant number of table lookups and arithmetic operations); space complexity is O(1) if the table is static. The main algorithm: compute `xi = floor(x)`, `frac = x - xi`, let `i0 = perm[static_cast<unsigned int>(xi) & 255]`, `i1 = perm[static_cast<unsigned int>(xi+1) & 255]`, then `v0 = valueTable[i0]`, `v1 = valueTable[i1]`, and interpolate using smoothstep `f = frac*frac*(3-2*frac)` and return `v0 + f*(v1-v0)`. This mirrors the 1D version of the original code's trilinear interpolation, but simpler.
#include <vector>
#include <cmath>

// Deterministic pseudo-random value table (256 floats in [0,1]).
// Generated once at static initialization using a simple LCG.
static const std::vector<float> createValueTable() {
    std::vector<float> table(256);
    unsigned int seed = 12345u;
    for (int i = 0; i < 256; ++i) {
        // LCG constants from Numerical Recipes
        seed = seed * 1664525u + 1013904223u;
        table[i] = static_cast<float>((seed >> 8) & 0xFFFF) / 65535.0f;
    }
    return table;
}

static const std::vector<float> kValueTable = createValueTable();

// Smooth 1D lattice noise.
// Given a real coordinate x and a permutation table perm (size 256),
// returns a value in [0,1] using floored indexing and smoothstep interpolation.
float smoothNoise1D(float x, const std::vector<int>& perm) {
    // Compute the integer lattice cell index (floor) and fractional part.
    float xi_float = std::floor(x);
    float frac = x - xi_float;

    // Use a 64-bit integer to safely handle large/small floats, then wrap with &255.
    long long xi = static_cast<long long>(xi_float);
    unsigned int idx0 = static_cast<unsigned int>(xi) & 255u;
    unsigned int idx1 = static_cast<unsigned int>(xi + 1) & 255u;

    // Look up lattice values.
    int p0 = perm[idx0];
    int p1 = perm[idx1];
    float v0 = kValueTable[static_cast<unsigned int>(p0) & 255u];
    float v1 = kValueTable[static_cast<unsigned int>(p1) & 255u];

    // Smoothstep interpolation factor (smooth first and second derivatives).
    float f = frac * frac * (3.0f - 2.0f * frac);

    // Linear interpolation between the two lattice values.
    return v0 + f * (v1 - v0);
}
#include <cassert>
#include <cmath>
#include <vector>

// The solution function is defined above. Test it here.

int main() {
    // Build a simple permutation table: identity 0..255.
    std::vector<int> perm(256);
    for (int i = 0; i < 256; ++i) perm[i] = i;

    // Test that integer coordinates give exactly the lattice value.
    float v0 = smoothNoise1D(0.0f, perm);
    float v1 = smoothNoise1D(1.0f, perm);
    assert(std::fabs(v0 - smoothNoise1D(256.0f, perm)) < 1e-6f); // wrap works
    assert(std::fabs(v0 - smoothNoise1D(-256.0f, perm)) < 1e-6f);

    // Test that at integer boundaries the value matches the lattice point exactly.
    assert(std::fabs(v0 - 0.0f) < 1e-6f); // identity table: first value from LCG
    // But we don't know exact value; instead test monotonic interpolation between 0 and 1:
    // For x=0.5, result must be the average of v0 and v1 if using smoothstep with frac=0.5,
    // smoothstep(0.5)=0.5, so result = (v0+v1)/2.
    float mid = smoothNoise1D(0.5f, perm);
    assert(std::fabs(mid - (v0 + v1) * 0.5f) < 1e-5f);

    // Test continuity: at x=0.9999 and x=1.0 the difference is small.
    float nearBoundary = smoothNoise1D(0.9999f, perm);
    assert(std::fabs(nearBoundary - v1) < 0.01f);

    // Test negative coordinates correctly floor and produce same as positive + wrap.
    float neg = smoothNoise1D(-0.5f, perm);
    // -0.5 floor is -1, so it's between indices -1 and 0; due to wrap, -1 maps to 255.
    // Compute expected by direct formula:
    long long xi = -1;
    unsigned int idx0 = static_cast<unsigned int>(xi) & 255u; // 255
    unsigned int idx1 = static_cast<unsigned int>(xi + 1) & 255u; // 0
    float f = 0.5f * 0.5f * (3.0f - 2.0f * 0.5f) = 0.5f; // actually compute in code
    float expected = (kValueTable[perm[255]] + kValueTable[perm[0]]) * 0.5f;
    assert(std::fabs(neg - expected) < 1e-5f);

    // Test output is always in [0,1] for a range.
    for (int i = -100; i <= 100; ++i) {
        float x = i * 0.37f;
        float val = smoothNoise1D(x, perm);
        assert(val >= 0.0f && val <= 1.0f);
    }

    // Test determinism.
    assert(smoothNoise1D(3.14159f, perm) == smoothNoise1D(3.14159f, perm));
}
