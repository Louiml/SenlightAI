// Write a C++ function that models a simplified Lambertian diffuse reflection and its translucent counterpart. Given a surface normal `Vec3 N` (represented as a simple struct with `x`, `y`, `z`), an outgoing direction `Vec3 omega_out` (unit vector), and an incoming direction `Vec3 omega_in` (unit vector), return the bidirectional reflectance distribution function (BRDF) value for diffuse reflection as a `Color3` (struct with `r`, `g`, `b` components). For diffuse, the BRDF is `cos(theta_in) / pi`, where `theta_in` is the angle between `N` and `omega_in`, but clamped to 0 (no negative contributions). For translucent, the same formula applies using `-N` (the opposite side). Additionally, the function should return the probability density function (PDF) value, which equals the same cosine-over-pi term. Implement a free function `Color3 evaluateLambertian(const Vec3& N, const Vec3& omega_in, bool isTransmissive, float& pdf)` that returns the color and sets the PDF appropriately. Assume all vectors are unit length; no normalization is needed. Handle the edge case where the dot product is negative by setting both color and PDF to zero. (This task abstracts the core math from the provided OSL closure code, focusing on the evaluation logic without the full closure framework.)

#include <cassert>
#include <cmath>

int main() {
    // Test 1: Perfectly aligned diffuse reflection (N = omega_in)
    {
        Vec3 N{0, 0, 1};
        Vec3 omega_in{0, 0, 1};
        float pdf = 0;
        Color3 result = evaluateLambertian(N, omega_in, false, pdf);
        float expected = 1.0f / 3.14159265f;
        assert(std::fabs(result.r - expected) < 1e-6f);
        assert(std::fabs(pdf - expected) < 1e-6f);
    }
    
    // Test 2: 45-degree angle diffuse reflection
    {
        Vec3 N{0, 0, 1};
        Vec3 omega_in{0, 0.70710678f, 0.70710678f};
        float pdf = 0;
        Color3 result = evaluateLambertian(N, omega_in, false, pdf);
        float expected = 0.70710678f / 3.14159265f;
        assert(std::fabs(result.g - expected) < 1e-6f);
        assert(std::fabs(pdf - expected) < 1e-6f);
    }
    
    // Test 3: Back-facing diffuse (negative dot product) should be zero
    {
        Vec3 N{0, 0, 1};
        Vec3 omega_in{0, 0, -1};
        float pdf = 1; // ensure overwritten
        Color3 result = evaluateLambertian(N, omega_in, false, pdf);
        assert(result.r == 0.0f && result.g == 0.0f && result.b == 0.0f);
        assert(pdf == 0.0f);
    }
    
    // Test 4: Translucent with N = (0,0,1) and omega_in = (0,0,-1) should be same as diffuse front-facing
    {
        Vec3 N{0, 0, 1};
        Vec3 omega_in{0, 0, -1};
        float pdf = 0;
        Color3 result = evaluateLambertian(N, omega_in, true, pdf);
        float expected = 1.0f / 3.14159265f;
        assert(std::fabs(result.b - expected) < 1e-6f);
        assert(std::fabs(pdf - expected) < 1e-6f);
    }
    
    // Test 5: Grazing angle (perpendicular) should give zero
    {
        Vec3 N{0, 0, 1};
        Vec3 omega_in{1, 0, 0};
        float pdf = 0.5f; // ensure overwritten
        Color3 result = evaluateLambertian(N, omega_in, false, pdf);
        assert(result.r == 0.0f && result.g == 0.0f && result.b == 0.0f);
        assert(pdf == 0.0f);
    }
    
    return 0;
}

#include <algorithm> // for std::max

// Simple 3D vector struct
struct Vec3 {
    float x, y, z;
    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }
};

// Simple RGB color struct
struct Color3 {
    float r, g, b;
};

// Evaluate Lambertian diffuse or translucent BRDF and PDF.
// For diffuse, use the surface normal N. For transmissive, use -N.
// Returns the color (all channels equal) and sets pdf to the cosine-over-pi value.
// If the dot product is <= 0, returns zero color and zero pdf.
Color3 evaluateLambertian(const Vec3& N, const Vec3& omega_in, bool isTransmissive, float& pdf) {
    // Choose the normal direction based on transmission flag
    Vec3 effectiveNormal = isTransmissive ? Vec3{-N.x, -N.y, -N.z} : N;
    
    // Compute cosine of angle between normal and incoming direction
    float cosTheta = effectiveNormal.dot(omega_in);
    
    // Clamp to zero for back-facing or grazing directions
    cosTheta = std::max(cosTheta, 0.0f);
    
    // Compute the value: cos(theta) / pi
    const float M_1_PI = 0.31830988618379067154f; // 1/pi
    float value = cosTheta * M_1_PI;
    
    // Set the PDF to the same value
    pdf = value;
    
    // Return the color (all channels equal)
    return Color3{value, value, value};
}

// The solution computes the dot product between the relevant normal and the incoming direction. For diffuse reflection, use `N`; for transmission, use `-N`. The cosine term is `max(dotProduct, 0.0f)`, which ensures zero for vectors pointing away from the surface (back-facing). The BRDF and PDF both equal `cos_term * (1/pi)`, so we compute that once and set the PDF to it, and return a `Color3` with all three channels set to that value (since Lambertian is wavelength-independent). Time complexity is O(1) with constant space. Edge cases include: when the dot product is exactly zero (grazing angle), the result is zero; when the dot product is negative, we clamp to zero. The function is `const`-correct and uses simple structs for `Vec3` and `Color3`.
