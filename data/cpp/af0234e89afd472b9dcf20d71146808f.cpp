Write a C++ function named `sampleFlatRadiance` that takes a `const Ray&`, a `const Scene&`, and a `const Sampler&` as parameters, and returns an `art::Color` representing the radiance for a simple flat (constant-color) integrator. The function should first attempt to find the closest intersection of the ray with the scene using `scene.intersect(ray, &isect)`. If no intersection occurs, it should return `scene.background.sample(ray)` (where `sample` is overloaded to take a `Ray`). If an intersection occurs, it must retrieve the material from the intersected primitive via `isect.primitive->get_material()`, use a `dynamic_cast` to `FlatMaterial*` to ensure the material is flat, and if the cast succeeds, return the material’s `kd()` color. If the cast fails (i.e., the material is not flat), return a solid black color `art::Color(0,0,0)`. Ensure the function is `const`-qualified and uses only the provided types and methods; you may include the necessary headers and assume all classes (`Ray`, `Scene`, `Sampler`, `SurfaceInteraction`, `Primitive`, `Material`, `FlatMaterial`, `Color`) are already defined in the `art` namespace.

The solution follows the logic of the given snippet but formalizes it as a standalone function. The algorithm is straightforward: (1) Declare a `SurfaceInteraction` object to be filled by the intersection test. (2) Call `scene.intersect(ray, &isect)`; if it returns `false`, return `scene.background.sample(ray)`. (3) If intersection occurs, obtain the material pointer via `isect.primitive->get_material()`. (4) Use `dynamic_cast<const FlatMaterial*>` (or a non-const cast if the material method is non-const, but the snippet suggests `kd()` is a member that likely returns `Color` and is `const`-safe; we'll use `dynamic_cast<FlatMaterial*>` if `get_material` returns a non-const pointer, but for robustness we can cast to `const FlatMaterial*` if needed—here we follow the snippet’s non-const cast). (5) If the cast succeeds, return `fm->kd()`; otherwise return `art::Color(0,0,0)`. Edge cases to consider: no intersection (background returns), a non-flat material (dynamic_cast returns null), and an empty background if `sample` itself fails (not handled here per spec). Time complexity is O(1) for the material handling plus the intersection cost (which depends on the scene’s acceleration structure, typically O(log n) for BVH or O(n) for linear search). Space complexity is O(1) beyond local variables.

#include "../include/flat_integrator.h"

namespace art {

// Sample the radiance for a flat (constant-color) integrator.
// Returns background color if no hit, else the flat material's diffuse color.
// Returns black if the hit material is not a FlatMaterial.
Color sampleFlatRadiance(const Ray& ray, const Scene& scene, const Sampler& sampler) {
    (void)sampler; // Sampler is unused in this simple integrator.
    SurfaceInteraction isect;
    if (!scene.intersect(ray, &isect)) {
        return scene.background.sample(ray);
    }
    Material* mat = isect.primitive->get_material();
    FlatMaterial* flatMat = dynamic_cast<FlatMaterial*>(mat);
    if (flatMat != nullptr) {
        return flatMat->kd();
    }
    return Color((component_t)0, (component_t)0, (component_t)0);
}

} // namespace art

#include <cassert>
#include "../include/flat_integrator.h"

// Minimal mock classes for testing (not part of solution).
// In a real setup, these come from the project's headers.
namespace art {
    class Ray { public: int dummy; };
    class Sampler { public: int dummy; };
    class Color { public: component_t r, g, b; Color(component_t r_, component_t g_, component_t b_) : r(r_), g(g_), b(b_) {} };
    class SurfaceInteraction { public: Primitive* primitive; };
    class Material { public: virtual ~Material() {} };
    class FlatMaterial : public Material { public: Color kd() const { return Color(1,0,0); } };
    class Primitive { public: Material* get_material() const { return mat; } void set_material(Material* m) { mat = m; } private: Material* mat; };
    class Scene {
    public:
        Scene() : background(Color(0.5,0.5,0.5)) {}
        bool intersect(const Ray&, SurfaceInteraction* isect) {
            if (has_hit) { isect->primitive = &prim; return true; }
            return false;
        }
        Color background;
        bool has_hit = false;
        Primitive prim;
    };
}

// For simplicity, test the function directly with mock scene.
// Note: This test file would normally include the real headers.
// The solution function is defined in the .cpp, but for test, we replicate it here.
// We'll just test the logic by re-implementing? Not needed if we include solution.
// Instead, we assume the solution is compiled separately. But for standalone test, we can copy the function here.
// To keep the test self-contained, we'll inline a copy of the solution function here.

namespace art {
    // Copy of solution function for testing (since we are not including the real .cpp).
    Color sampleFlatRadiance(const Ray& ray, const Scene& scene, const Sampler& sampler) {
        (void)sampler;
        SurfaceInteraction isect;
        if (!scene.intersect(ray, &isect)) {
            return scene.background.sample(ray);
        }
        Material* mat = isect.primitive->get_material();
        FlatMaterial* flatMat = dynamic_cast<FlatMaterial*>(mat);
        if (flatMat != nullptr) {
            return flatMat->kd();
        }
        return Color((component_t)0, (component_t)0, (component_t)0);
    }
}

int main() {
    using namespace art;
    // Setup scene with no hit.
    Scene scene1;
    scene1.has_hit = false;
    Ray ray; Sampler sampler;
    Color bg = scene1.background; // (0.5,0.5,0.5)
    Color c1 = sampleFlatRadiance(ray, scene1, sampler);
    assert(c1.r == 0.5 && c1.g == 0.5 && c1.b == 0.5); // background

    // Setup scene with a flat material.
    Scene scene2;
    scene2.has_hit = true;
    FlatMaterial flat;
    scene2.prim.set_material(&flat);
    Color c2 = sampleFlatRadiance(ray, scene2, sampler);
    assert(c2.r == 1.0 && c2.g == 0.0 && c2.b == 0.0); // flat kd

    // Setup scene with a non-flat material.
    class OtherMaterial : public Material {}; // minimal
    OtherMaterial other;
    Scene scene3;
    scene3.has_hit = true;
    scene3.prim.set_material(&other);
    Color c3 = sampleFlatRadiance(ray, scene3, sampler);
    assert(c3.r == 0.0 && c3.g == 0.0 && c3.b == 0.0); // black

    return 0;
}
