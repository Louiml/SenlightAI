Implement a C++ function that simulates the diffuse and specular shading of a single surface point under a set of discrete light sources. The function should take as input a surface normal, a view direction vector, a base color (diffuse albedo), a diffuse gain, a specular gain, a specular color, a roughness parameter (in the range (0,1]), and a vector of light sources, where each light source is defined by a direction vector and a color. The function must return the total outgoing radiance color as a `std::array<float,3>` in linear RGB space. The diffuse term follows Lambert’s law: `Cl * Kd * Cs * max(0, dot(N,L))`. The specular term uses a Blinn-Phong style model: `Cl * Ks * specularColor * pow(max(0, dot(N,H)), 1/roughness)` where `H = normalize(L + V)`. The view vector V should be normalized internally. The function must correctly handle cases where the light direction points away from the surface (dot(N,L) <= 0) by contributing zero for that light, and handle roughness values close to zero without numerical instability (clamp roughness to a small epsilon like 1e-4). The sum must be accumulated over all lights and returned as a float array with components possibly exceeding 1.0 (no clamping). Assume all inputs are already normalized vectors or colors in the [0,1] range except for the vectors which are arbitrary directions.

#include <cassert>
#include <cmath>
#include <array>
#include <vector>

// Include the solution function (or copy it here)
struct Light {
    std::array<float,3> direction;
    std::array<float,3> color;
};

std::array<float,3> shadePoint(
    const std::array<float,3>& normal,
    const std::array<float,3>& viewDir,
    const std::array<float,3>& Cs,
    float Kd,
    float Ks,
    const std::array<float,3>& specularColor,
    float roughness,
    const std::vector<Light>& lights);

int main() {
    // Test 1: single light directly above (normal up, light from same direction), view same direction
    std::array<float,3> N = {0,0,1};
    std::array<float,3> V = {0,0,1};
    std::array<float,3> Cs = {1,1,1};
    std::vector<Light> lights = {{{0,0,1}, {1,1,1}}};
    auto res = shadePoint(N, V, Cs, 1.0f, 0.0f, {1,1,1}, 0.5f, lights);
    // ndotl=1, diffuse=1, spec=0, so result = (1,1,1)
    assert(std::fabs(res[0]-1.0f) < 1e-6 && std::fabs(res[1]-1.0f) < 1e-6 && std::fabs(res[2]-1.0f) < 1e-6);
    
    // Test 2: perpendicular light (from side) diffuse zero, spec zero
    lights = {{{1,0,0}, {1,0,0}}};
    res = shadePoint(N, V, Cs, 1.0f, 1.0f, {1,1,1}, 0.5f, lights);
    // ndotl=0, diffuse=0, spec=0 (no spec since ndotl<=0)
    assert(res[0]==0.0f && res[1]==0.0f && res[2]==0.0f);
    
    // Test 3: light behind, both zero
    lights = {{{0,0,-1}, {1,1,1}}};
    res = shadePoint(N, V, Cs, 1.0f, 1.0f, {1,1,1}, 0.5f, lights);
    assert(res[0]==0.0f && res[1]==0.0f && res[2]==0.0f);
    
    // Test 4: two lights, one frontal intense, one dimmer, sum
    lights = {{{0,0,1}, {0.5f,0,0}}, {{0,0.7071f,0.7071f}, {0,0.5f,0}}};
    // Light1: ndotl=1, kd=0.5, diffuse=0.5, spec maybe 0 if Ks=0
    // Light2: ndotl=0.7071, diffuse=0.35355, spec if Ks=0
    res = shadePoint(N, V, {1,1,1}, 0.5f, 0.0f, {1,1,1}, 0.1f, lights);
    // Result = (0.5*1*0.5, 0.5*0.5*0.7071? actually light2 color is (0,0.5,0), so red from first, green from second)
    assert(std::fabs(res[0]-0.25f) < 1e-6);
    assert(std::fabs(res[1]-0.176775f) < 1e-5);
    
    // Test 5: specular with perfect alignment: N=(0,0,1), L=(0,0,1), V=(0,0,1), H=(0,0,1), ndoth=1, pow=1, Ks=1, specColor=1, roughness=1
    lights = {{{0,0,1}, {1,1,1}}};
    res = shadePoint(N, V, {1,1,1}, 0.0f, 1.0f, {1,1,1}, 1.0f, lights);
    assert(std::fabs(res[0]-1.0f) < 1e-6);
    
    // Test 6: high roughness makes spec dimmer (roughness=100 gives pow=1/100=0.01 exponent, ndoth=1 still 1)
    // Actually pow(1, 0.01)=1, same. Use ndoth=0.5 with roughness=2 => pow(0.5,0.5)=0.7071
    // Construct L=(0.577,0.577,0.577) and V=(0,0,1) => H normal? Let's compute H=(0.577,0.577,1.577) normalized .. dot with N=1.577/len, approx 0.92? Hard to assert precise. Instead just test that function doesn't crash with roughness near zero
    lights = {{{0,0,1}, {1,1,1}}};
    res = shadePoint(N, V, {1,1,1}, 0.0f, 1.0f, {1,1,1}, 1e-10f, lights); // roughness tiny
    // Should still return something (clamped to 1e-4)
    assert(res[0] >= 0.0f);
    
    // Test 7: degenerate view vector zero, should not crash
    V = {0,0,0};
    res = shadePoint(N, V, {1,1,1}, 0.5f, 0.0f, {1,1,1}, 0.5f, lights);
    assert(res[0] >= 0.0f);
    
    return 0;
}

#include <array>
#include <vector>
#include <cmath>
#include <algorithm>

struct Light {
    std::array<float,3> direction; // normalized or any direction
    std::array<float,3> color;     // linear color in [0,1] or beyond
};

// Compute shaded color at a point given normal, view, material parameters, and lights.
std::array<float,3> shadePoint(
    const std::array<float,3>& normal,
    const std::array<float,3>& viewDir,
    const std::array<float,3>& Cs,
    float Kd,
    float Ks,
    const std::array<float,3>& specularColor,
    float roughness,
    const std::vector<Light>& lights)
{
    // Normalize view direction (if zero, use fallback? assume non-zero)
    float viewLen = std::sqrt(viewDir[0]*viewDir[0] + viewDir[1]*viewDir[1] + viewDir[2]*viewDir[2]);
    std::array<float,3> V = {0.0f,0.0f,0.0f};
    if (viewLen > 1e-8f) {
        V = {viewDir[0]/viewLen, viewDir[1]/viewLen, viewDir[2]/viewLen};
    }
    
    // Clamp roughness to avoid instability
    float r = std::max(roughness, 1e-4f);
    float invRough = 1.0f / r;
    
    // Normalize normal just in case
    float nLen = std::sqrt(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);
    std::array<float,3> N = {0.0f,0.0f,0.0f};
    if (nLen > 1e-8f) {
        N = {normal[0]/nLen, normal[1]/nLen, normal[2]/nLen};
    }
    
    std::array<float,3> result = {0.0f,0.0f,0.0f};
    
    for (const auto& light : lights) {
        // Normalize light direction
        float lLen = std::sqrt(light.direction[0]*light.direction[0] + light.direction[1]*light.direction[1] + light.direction[2]*light.direction[2]);
        if (lLen < 1e-8f) continue; // skip zero direction
        std::array<float,3> L = {light.direction[0]/lLen, light.direction[1]/lLen, light.direction[2]/lLen};
        
        // Diffuse term
        float ndotl = N[0]*L[0] + N[1]*L[1] + N[2]*L[2];
        float diffuse = std::max(0.0f, ndotl) * Kd;
        
        // Specular term
        float spec = 0.0f;
        if (ndotl > 0.0f) { // only specular if light is on correct side
            // Half vector
            std::array<float,3> H = {L[0]+V[0], L[1]+V[1], L[2]+V[2]};
            float hLen = std::sqrt(H[0]*H[0] + H[1]*H[1] + H[2]*H[2]);
            if (hLen > 1e-8f) {
                H = {H[0]/hLen, H[1]/hLen, H[2]/hLen};
                float ndoth = N[0]*H[0] + N[1]*H[1] + N[2]*H[2];
                spec = std::pow(std::max(0.0f, ndoth), invRough) * Ks;
            }
        }
        
        // Combine with light color and albedo
        for (int c = 0; c < 3; ++c) {
            result[c] += light.color[c] * (Cs[c] * diffuse + specularColor[c] * spec);
        }
    }
    
    return result;
}

// The main algorithm iterates over each light source in the input vector. For each light, we compute the diffuse contribution by taking the dot product between the surface normal and the light direction, clamping it to zero if negative (since light from behind the surface contributes no diffuse). For specular, we compute the half-vector `H` as the normalized sum of the light direction and the view direction (the view direction should be normalized first; if it is already normalized, we can just use it). Then compute the dot product between the normal and `H`, clamp it to zero, raise it to the power `1/roughness` (with roughness clamped to a minimum epsilon to avoid division by zero and negative exponents leading to infinities). Multiply the specular color and specular gain, and add that to the diffuse term. Multiply the sum by the light color `Cl`. Accumulate across all lights. Edge cases: if the light direction is zero (degenerate), skip that light or treat contribution as zero. If view direction is zero, we may need to handle but assume valid. If roughness is zero, we set it to epsilon. Complexity: `O(num_lights)` time, `O(1)` extra space (excluding the output). All operations are simple vector math and floating-point operations.
