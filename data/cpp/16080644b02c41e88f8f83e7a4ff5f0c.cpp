Write a C++ function `sampleCoherentNoise(float x, int resolution, unsigned int seed)` that generates a one-dimensional coherent noise value at position `x` using a periodic gradient-noise approach. The function must internally create a periodic array of `resolution` random gradient values (derivatives) derived from a seeded linear congruential generator, wrap `x` into the interval `[0, resolution)`, interpolate smoothly between adjacent gradient points using a quintic smoothstep, and return the Hermite-style interpolated value based on those gradients, matching the behavior of a gradient-coherent noise sampler. The `resolution` must be a power of two greater than 1, and `x` may be negative or greater than `resolution`, requiring proper wrapping. The noise output should be continuous and periodic with period `resolution`, and must be deterministic for a given `seed`. The function must not use global state or side effects.

The core idea is to precompute a periodic array of random gradient values (derivatives) of size `resolution`, where each gradient is a random float in a bounded range (e.g., derived from a seeded LCG and scaled to `[-2, 2]`). For a query `x`, we first wrap it into the periodic domain: compute `x_wrapped = x - floor(x / resolution) * resolution`, ensuring it lies in `[0, resolution)`. Then we identify the integer cell index `i = floor(x_wrapped)` and the fractional offset `t = x_wrapped - i`. The two neighboring gradients are `g1 = gradients[i % resolution]` and `g2 = gradients[(i+1) % resolution]` (using bitmask if resolution is power of two). The output is a Hermite interpolation that vanishes at the endpoints (ensuring continuity): `result = (g1 * (1 - t) - g2 * t) * t * (1 - t)`. This polynomial is zero when `t=0` or `t=1`, and smoothly blends between gradients. Edge cases include negative `x` (handled by the floor wrap) and `x` exactly at an integer boundary (the fractional part becomes 0, giving zero output). Time complexity is `O(resolution)` for initialization and `O(1)` per sample. Space is `O(resolution)` for the gradient array. The LCG must be seeded to ensure reproducibility; using a simple `seed = seed * 1103515245 + 12345` and `(seed / 65536) % 32768` for randomness yields a deterministic sequence.

#include <cmath>
#include <vector>
#include <cstdint>

// Sample a periodic gradient-coherent noise at position x.
// resolution must be a power of two > 1.
// seed determines the deterministic random gradient sequence.
float sampleCoherentNoise(float x, int resolution, unsigned int seed) {
    std::vector<float> gradients(resolution);
    unsigned int state = seed;
    for (int i = 0; i < resolution; ++i) {
        state = state * 1103515245u + 12345u;
        float rand01 = static_cast<float>((state >> 16) & 0x7fff) / 32767.0f;
        gradients[i] = (rand01 * 2.0f - 1.0f) * 4.0f;  // range [-4, 4]
    }

    // Wrap x into [0, resolution)
    x -= std::floor(x / static_cast<float>(resolution)) * static_cast<float>(resolution);
    
    int i = static_cast<int>(std::floor(x));
    float t = x - std::floor(x);
    
    int mask = resolution - 1;  // works because resolution is power of two
    float g1 = gradients[i & mask];
    float g2 = gradients[(i + 1) & mask];
    
    // Hermite-style interpolation with zero endpoints
    return (g1 * (1.0f - t) - g2 * t) * t * (1.0f - t);
}

#include <cassert>
#include <cmath>
#include <cstdint>

int main() {
    // Period 4: output at x=0 and x=4 must match (periodic)
    float v0 = sampleCoherentNoise(0.0f, 4, 12345u);
    float v4 = sampleCoherentNoise(4.0f, 4, 12345u);
    assert(std::fabs(v0 - v4) < 1e-6f);

    // Negative x wraps properly: x=-0.5 is equivalent to x=3.5 for period 4
    float vneg = sampleCoherentNoise(-0.5f, 4, 12345u);
    float vpos = sampleCoherentNoise(3.5f, 4, 12345u);
    assert(std::fabs(vneg - vpos) < 1e-6f);

    // At integer positions, output is zero (endpoints vanish)
    assert(std::fabs(sampleCoherentNoise(1.0f, 4, 999u)) < 1e-6f);
    assert(std::fabs(sampleCoherentNoise(2.0f, 4, 999u)) < 1e-6f);

    // Deterministic: same seed and x gives identical result
    float a = sampleCoherentNoise(1.25f, 8, 777u);
    float b = sampleCoherentNoise(1.25f, 8, 777u);
    assert(a == b);

    // Different seeds generally produce different values
    float c = sampleCoherentNoise(1.25f, 8, 778u);
    assert(a != c);

    // Larger resolution still periodic
    float p0 = sampleCoherentNoise(0.0f, 16, 42u);
    float p16 = sampleCoherentNoise(16.0f, 16, 42u);
    assert(std::fabs(p0 - p16) < 1e-6f);
}
