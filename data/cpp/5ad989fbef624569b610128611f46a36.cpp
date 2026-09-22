/*
Write a C++ function `findPolygonsContainingPoint` that takes a vector of `GeoObject*` (polymorphic geometry objects, each having a `getType()` method returning a C-string like "POLYGON" or "MULTIPOLYGON") and a 2D point `(x, y)`. The function must return a `std::vector<GeoObject*>` containing all polygons (single `GeoPolygon` or `GeoMultiPolygon`) from the input vector that strictly contain the given point **inside their interior** (not on the boundary). For each `GeoMultiPolygon`, the entire multi-polygon is considered to contain the point if **any** of its constituent polygons contains the point. Ignore all other geometry types (e.g., "POINT", "POLYLINE", "MULTIPOINT", etc.) and never add them to the result. Use the helper function `isPointInPolygon({x, y}, polygon)` (provided externally) to test point-in-polygon membership; this helper already returns `true` only for points strictly inside (interior), not on the boundary. The original input vector must not be modified, and the returned vector must preserve the order of the qualifying features as they appear in the input. Assume `GeoObject` is an abstract base class with a virtual `const char* getType() const` and a virtual destructor, and that both `GeoPolygon` and `GeoMultiPolygon` are concrete derived classes, with `GeoMultiPolygon` having a public member `std::vector<GeoPolygon*> polygons`. The function should handle null pointers in the input vector gracefully by skipping them.
*/

#include <vector>
#include <cstring>

// Forward declarations (assumed to be provided elsewhere)
class GeoObject {
public:
    virtual ~GeoObject() = default;
    virtual const char* getType() const = 0;
};

class GeoPolygon : public GeoObject {
public:
    const char* getType() const override { return "POLYGON"; }
};

class GeoMultiPolygon : public GeoObject {
public:
    std::vector<GeoPolygon*> polygons;
    const char* getType() const override { return "MULTIPOLYGON"; }
};

// External helper (assumed to be provided)
bool isPointInPolygon(const std::pair<float, float>& point, const GeoPolygon* polygon);

// Main solution function
std::vector<GeoObject*> findPolygonsContainingPoint(
    const std::vector<GeoObject*>& features, 
    float x, float y) 
{
    std::vector<GeoObject*> result;
    const std::pair<float, float> point = {x, y};

    for (GeoObject* feature : features) {
        if (feature == nullptr) {
            continue;
        }

        const char* geomType = feature->getType();
        if (std::strcmp(geomType, "POLYGON") == 0) {
            GeoPolygon* polygon = static_cast<GeoPolygon*>(feature);
            if (isPointInPolygon(point, polygon)) {
                result.push_back(feature);
            }
        } else if (std::strcmp(geomType, "MULTIPOLYGON") == 0) {
            GeoMultiPolygon* multiPolygon = static_cast<GeoMultiPolygon*>(feature);
            for (GeoPolygon* polygon : multiPolygon->polygons) {
                if (polygon != nullptr && isPointInPolygon(point, polygon)) {
                    result.push_back(feature);
                    break; // Avoid adding the same multi-polygon twice
                }
            }
        }
        // Ignore any other geometry types
    }

    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// Minimal test stubs for the helper and geometry classes
// In real code these would be provided by the framework.
bool isPointInPolygon(const std::pair<float, float>& point, const GeoPolygon* polygon) {
    // Simple stub: return true only for known test cases.
    // In real usage, this would be a proper point-in-polygon test.
    static const std::pair<float,float> origin = {0.0f, 0.0f};
    return point == origin; // Only (0,0) is "inside" for testing purposes
}

// Concrete implementations for testing (minimal, no real geometry)
class TestPolygon : public GeoPolygon {
    // Use base class, no extra data needed
};
class TestMultiPolygon : public GeoMultiPolygon {
    // Use base class, no extra data needed
};

int main() {
    // Create test objects
    TestPolygon p1; // type "POLYGON"
    TestPolygon p2; // type "POLYGON"
    TestMultiPolygon mp1; // type "MULTIPOLYGON"
    TestMultiPolygon mp2; // type "MULTIPOLYGON"

    // Give mp1 two sub-polygons, mp2 none (empty)
    mp1.polygons.push_back(&p1);
    mp1.polygons.push_back(&p2);

    // Build input vector with mixed types and a null pointer
    std::vector<GeoObject*> features = {&p1, nullptr, &mp1, &p2, &mp2};

    // Test 1: point (0,0) — both p1, mp1 (since sub-polygon p1 contains), and p2? 
    // Wait: our stub returns true for any polygon when point==(0,0). So p1, p2, and mp1 will qualify.
    auto result1 = findPolygonsContainingPoint(features, 0.0f, 0.0f);
    assert(result1.size() == 3);
    assert(result1[0] == &p1);
    assert(result1[1] == &mp1); 
    assert(result1[2] == &p2);

    // Test 2: point (5,5) — none contain (our stub returns false)
    auto result2 = findPolygonsContainingPoint(features, 5.0f, 5.0f);
    assert(result2.empty());

    // Test 3: point (0,0) with input vector containing only mp2 (empty multi-polygon)
    std::vector<GeoObject*> features2 = {&mp2};
    auto result3 = findPolygonsContainingPoint(features2, 0.0f, 0.0f);
    assert(result3.empty()); // empty multi-polygon cannot contain

    // Test 4: input with only null pointers
    std::vector<GeoObject*> features3 = {nullptr, nullptr};
    auto result4 = findPolygonsContainingPoint(features3, 0.0f, 0.0f);
    assert(result4.empty());

    // Test 5: duplicate feature appears twice — should appear twice in output
    std::vector<GeoObject*> features4 = {&p1, &p1};
    auto result5 = findPolygonsContainingPoint(features4, 0.0f, 0.0f);
    assert(result5.size() == 2);
    assert(result5[0] == &p1 && result5[1] == &p1);

    // All assertions passed
    return 0;
}

// The solution iterates over each pointer in the input vector. For each non-null pointer, we inspect its type string. If the type is "POLYGON", we static_cast to `GeoPolygon*` and test membership with `isPointInPolygon`. If it matches, we add the pointer to the result. If the type is "MULTIPOLYGON", we static_cast to `GeoMultiPolygon*`, then iterate through its `polygons` vector; if any constituent polygon passes the point-in-polygon test, we add the multi-polygon pointer to the result and break out of the inner loop (to avoid duplicate additions for the same multi-polygon). For any other type, we skip it. Edge cases: null pointers are ignored; for an empty multi-polygon (zero constituent polygons), it will never contain the point; if the point lies exactly on a polygon boundary, `isPointInPolygon` is expected to return `false` per the specification, so we don't add it. The time complexity is O(T), where T is the total number of polygons across all single and multi polygons in the input (each polygon is tested at most once). The space complexity is O(1) auxiliary plus O(R) for the result vector, where R is the number of qualifying features.
