/*
Write a C++ function `scatterRayColor` that simulates a simple ray-tracing color computation for a scene containing exactly three spheres: a blue Lambertian sphere at `(0, 0, -1)` with radius `0.5`, a yellow Lambertian ground sphere at `(0, -100.5, -1)` with radius `100`, and a red metal sphere at `(1, 0, -1)` with radius `0.5` and fuzz `0.3`. The function takes a ray (origin and direction as `std::array<float,3>` values) and a recursion depth limit (non-negative integer). It returns an `std::array<float,3>` RGB color with values in `[0,1]`. Use a simplified model: for each hit, compute exact intersection with spheres using the quadratic formula, use Lambertian diffuse reflection for the blue and yellow spheres (attenuation = albedo), and perfect specular reflection for the red sphere (attenuation = albedo, no fuzz). Background color is a linear gradient from white (`(1,1,1)`) at `y=1` to blue (`(0.5,0.7,1)`) at `y=-1` based on the normalized ray direction's y-component. At depth zero, return black. Use only `#include <array>`, `<cmath>`, and `<limits>`. Do not use any random number generation since this is deterministic. Ensure all calculations are in floating point.
*/
#include <array>
#include <cmath>
#include <limits>

using Vec3 = std::array<float, 3>;

static Vec3 add(const Vec3& a, const Vec3& b) {
    return {a[0]+b[0], a[1]+b[1], a[2]+b[2]};
}
static Vec3 sub(const Vec3& a, const Vec3& b) {
    return {a[0]-b[0], a[1]-b[1], a[2]-b[2]};
}
static Vec3 mul(const Vec3& a, float s) {
    return {a[0]*s, a[1]*s, a[2]*s};
}
static float dot(const Vec3& a, const Vec3& b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}
static Vec3 normalize(const Vec3& a) {
    float len = std::sqrt(dot(a,a));
    return mul(a, 1.0f/len);
}
static Vec3 reflect(const Vec3& v, const Vec3& n) {
    return sub(v, mul(n, 2.0f*dot(v,n)));
}

// Main function: compute ray color for the fixed scene.
std::array<float,3> scatterRayColor(const std::array<float,3>& rayOrigin, const std::array<float,3>& rayDirection, int depth) {
    if (depth <= 0) return {0.0f, 0.0f, 0.0f};

    // Scene spheres: center, radius, albedo, type (0=lambertian,1=metal)
    struct Sphere {
        Vec3 center;
        float radius;
        Vec3 albedo;
        bool isMetal;
    };
    Sphere spheres[3] = {
        {{0.0f, 0.0f, -1.0f}, 0.5f, {0.1f, 0.2f, 0.5f}, false}, // blue
        {{0.0f, -100.5f, -1.0f}, 100.0f, {0.8f, 0.8f, 0.0f}, false}, // ground
        {{1.0f, 0.0f, -1.0f}, 0.5f, {0.8f, 0.6f, 0.2f}, true}   // metal
    };

    float closest_t = std::numeric_limits<float>::max();
    int hit_index = -1;
    Vec3 hit_normal = {0,0,0};

    for (int i = 0; i < 3; ++i) {
        Vec3 oc = sub(rayOrigin, spheres[i].center);
        float a = dot(rayDirection, rayDirection);
        float b = 2.0f * dot(oc, rayDirection);
        float c = dot(oc, oc) - spheres[i].radius * spheres[i].radius;
        float discriminant = b*b - 4.0f*a*c;
        if (discriminant < 0.0f) continue;
        float sqrt_d = std::sqrt(discriminant);
        float t = (-b - sqrt_d) / (2.0f*a);
        if (t < 1e-4f) {
            t = (-b + sqrt_d) / (2.0f*a);
        }
        if (t > 1e-4f && t < closest_t) {
            closest_t = t;
            hit_index = i;
            Vec3 hit_point = add(rayOrigin, mul(rayDirection, t));
            hit_normal = normalize(sub(hit_point, spheres[i].center));
        }
    }

    if (hit_index == -1) {
        // Background gradient
        Vec3 dir = normalize(rayDirection);
        float ratio = 0.5f * (dir[1] + 1.0f);
        Vec3 white = {1.0f, 1.0f, 1.0f};
        Vec3 blue = {0.5f, 0.7f, 1.0f};
        Vec3 result = add(mul(white, 1.0f - ratio), mul(blue, ratio));
        return result;
    }

    Vec3 albedo = spheres[hit_index].albedo;
    if (spheres[hit_index].isMetal) {
        Vec3 reflected_dir = reflect(normalize(rayDirection), hit_normal);
        Vec3 child = scatterRayColor(add(rayOrigin, mul(rayDirection, closest_t)), reflected_dir, depth - 1);
        return {albedo[0]*child[0], albedo[1]*child[1], albedo[2]*child[2]};
    } else {
        // Deterministic diffuse approximation (no randomness)
        Vec3 diffusion_dir = normalize(add(hit_normal, {0.5f, 0.5f, 0.5f}));
        Vec3 child = scatterRayColor(add(rayOrigin, mul(rayDirection, closest_t)), diffusion_dir, depth - 1);
        return {albedo[0]*child[0], albedo[1]*child[1], albedo[2]*child[2]};
    }
}
#include <cassert>
#include <cmath>
#include <array>

// Include the solution function here (assumed above)

int main() {
    // Ray pointing straight down at the ground sphere, should return yellow-ish after diffuse scatter
    std::array<float,3> origin = {0.0f, 1.0f, -1.0f};
    std::array<float,3> dir = {0.0f, -1.0f, 0.0f};
    auto col = scatterRayColor(origin, dir, 10);
    assert(col[0] > 0.2f && col[1] > 0.2f && col[2] < 0.2f); // yellow dominant

    // Ray pointing straight at the blue sphere from front
    origin = {0.0f, 0.0f, 0.0f};
    dir = {0.0f, 0.0f, -1.0f};
    col = scatterRayColor(origin, dir, 10);
    assert(col[0] < 0.3f && col[1] < 0.5f && col[2] > 0.3f); // blue dominant

    // Ray pointing at metal sphere from left
    origin = {0.0f, 0.0f, 0.0f};
    dir = {1.0f, 0.0f, -0.5f}; // rough direction toward (1,0,-1)
    col = scatterRayColor(origin, dir, 10);
    // Metal may scatter to background, but should have some red-ish component in attenuation
    // Just ensure it doesn't return pure black at depth>0
    assert(col[0] > 0.0f || col[1] > 0.0f || col[2] > 0.0f);

    // Depth zero returns black
    col = scatterRayColor(origin, dir, 0);
    assert(col[0] == 0.0f && col[1] == 0.0f && col[2] == 0.0f);

    // Ray that misses all spheres returns background blue at bottom
    origin = {0.0f, 0.0f, 0.0f};
    dir = {10.0f, -1.0f, 0.0f}; // goes right and down, misses
    col = scatterRayColor(origin, dir, 5);
    assert(col[2] > col[0] && col[2] > col[1]); // blue dominant

    // Ray pointing exactly at ground far away, should be mostly yellow
    origin = {5.0f, 10.0f, -1.0f};
    dir = {0.0f, -1.0f, 0.0f};
    col = scatterRayColor(origin, dir, 3);
    assert(col[0] > 0.5f && col[1] > 0.5f && col[2] < 0.3f);

    // Ensure all outputs are within [0,1] for typical rays
    origin = {0.0f, 0.0f, 0.0f};
    dir = {0.1f, 0.2f, -1.0f};
    for (int d = 0; d <= 10; ++d) {
        auto cc = scatterRayColor(origin, dir, d);
        for (int i=0;i<3;++i) assert(cc[i] >= 0.0f && cc[i] <= 1.0f);
    }

    return 0;
}
// The solution must compute ray-sphere intersections for each of the three spheres in order: blue, ground, red. For each sphere, solve the quadratic equation `(dot(d,d)) * t^2 + 2*dot(o-center, d) * t + (dot(o-center, o-center) - r^2) = 0`. If the discriminant is non-negative, take the smallest positive root `t`. Among all spheres that yield a valid `t`, choose the smallest positive `t` (the closest hit). For the blue and yellow spheres, after hitting, compute the surface normal at the hit point, then generate a diffuse reflection direction as `normal + random_unit_vector` (but here we need deterministic; since no randomness, we still compute a deterministic approximation: use `normal` plus a fixed vector like `(0.5,0.5,0.5)` normalized, which approximates a diffuse scatter). However, for a self-contained deterministic task, it is acceptable to use a fixed offset for diffuse scattering; the attenuation is the sphere's albedo. For the red metal, the reflection is exactly `reflect(direction, normal)` (specular). The recursive call reduces depth by 1 and multiplies the attenuation. Edge cases: rays that miss all spheres return background gradient; rays that hit a sphere with depth 0 return black; rays that hit a sphere but after reflection go outside the scene (miss again) result in background color. Time complexity for a single ray is O(depth * number_of_spheres) = O(50 * 3) in worst case, but effectively linear in depth. Space complexity is O(depth) due to recursion stack, but since depth is at most 50, it's constant for practical purposes. The implementation must be careful with floating-point precision to avoid missing hits due to `t` being near zero.
