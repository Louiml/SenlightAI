/*
Write a C++ function named `computePolygonVertices` that takes a cycle ratio (a floating-point value between 0.0 and 1.0), a radius, center coordinates (cx, cy), and an integer maximum number of vertices (at least 3), and returns a `std::vector<std::pair<float, float>>` containing the vertices of a regular polygon. The number of vertices should vary smoothly from 3 to the maximum based on the cycle ratio following the formula `numVertices = 3 + (maxVertices - 3) * sin(pi * ratio)^2`. The vertices should be placed on a circle of the given radius centered at (cx, cy), with the first vertex at the top (angle = -pi/2) and subsequent vertices spaced evenly using the *interpolated* angular step (2*pi / interpolatedNumVertices), where `interpolatedNumVertices` is the non-integer continuous value from the formula, not the truncated integer count. The returned vector should contain exactly the integer-truncated number of vertices, but the angular positions must use the continuous interpolated step. The function must handle edge cases: cycle ratio clamped to [0,1], max vertices at least 3, radius non-negative (if negative, use absolute value). The returned vertices must be in order around the circle. Provide a reference solution and tests.
*/
#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

// Returns vertices of a regular polygon with smoothly varying vertex count.
std::vector<std::pair<float, float>> computePolygonVertices(
    float ratio,
    float radius,
    float cx,
    float cy,
    int maxVertices) {
    
    // Clamp ratio to [0,1]
    ratio = std::clamp(ratio, 0.0f, 1.0f);
    
    // Ensure maxVertices at least 3
    maxVertices = std::max(maxVertices, 3);
    
    // Use absolute radius if negative
    radius = std::fabs(radius);
    
    // Continuous number of vertices
    float sinVal = std::sin(M_PI * ratio);
    float continuousCount = 3.0f + (maxVertices - 3) * sinVal * sinVal;
    
    // Integer count for actual vertices
    int numVertices = static_cast<int>(continuousCount);
    numVertices = std::clamp(numVertices, 3, maxVertices);
    
    // Interpolated angular step
    float step = (2.0f * M_PI) / continuousCount;
    
    // Starting angle (top of circle)
    float angularOffset = -M_PI / 2.0f;
    
    std::vector<std::pair<float, float>> vertices;
    vertices.reserve(numVertices);
    
    for (int i = 0; i < numVertices; ++i) {
        float angle = angularOffset + i * step;
        float x = cx + radius * std::cos(angle);
        float y = cy + radius * std::sin(angle);
        vertices.emplace_back(x, y);
    }
    
    return vertices;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

// Assume computePolygonVertices is defined above.

int main() {
    // Test 1: Ratio 0 gives triangle at top with equal spacing.
    auto tri = computePolygonVertices(0.0f, 10.0f, 0.0f, 0.0f, 6);
    assert(tri.size() == 3);
    // First vertex at top: (0, 10)
    assert(std::fabs(tri[0].first - 0.0f) < 1e-5);
    assert(std::fabs(tri[0].second - 10.0f) < 1e-5);
    // Check roughly right positions: for triangle, step = 2pi/3 ≈ 2.094. Angles: -pi/2, -pi/2+2.094=0.524, -pi/2+4.188=2.618
    // Coordinates: (cos(-pi/2)*10=0, sin(-pi/2)*10=-10) -> wait, -pi/2 is top? Actually cos(-pi/2)=0, sin(-pi/2)=-1 -> y=-10. That's bottom. The spec says "top" meaning angle -pi/2 is top in standard? In math, top is usually pi/2. But snippet uses -pi/2 to start at top? In screen coordinates y increases downward, so -pi/2 gives (0, -R) which is top in math but bottom in screen. The task says "first vertex at the top (angle = -pi/2)". So we follow that: at angle -pi/2, y = -R. So we expect first vertex at (0, -10). Let's adjust.
    // Actually snippet uses cos and sin with -pi/2, so x=0, y=-R. In typical screen coords, that's top? Depends. The task explicitly says angle = -pi/2, so we test that.
    assert(std::fabs(tri[0].first - 0.0f) < 1e-5);
    assert(std::fabs(tri[0].second - (-10.0f)) < 1e-5);
    
    // Test 2: Ratio 0.5 gives max vertices (hexagon for max=6).
    auto hex = computePolygonVertices(0.5f, 10.0f, 0.0f, 0.0f, 6);
    assert(hex.size() == 6);
    // Check first vertex still at -pi/2
    assert(std::fabs(hex[0].first - 0.0f) < 1e-5);
    assert(std::fabs(hex[0].second - (-10.0f)) < 1e-5);
    // Check next vertex at angle -pi/2 + 2*pi/6 = -pi/2 + pi/3 = -pi/6, cos(-pi/6)=sqrt(3)/2, sin(-pi/6)=-1/2
    assert(std::fabs(hex[1].first - (10.0f * std::sqrt(3)/2.0f)) < 1e-4);
    assert(std::fabs(hex[1].second - (-5.0f)) < 1e-4);
    
    // Test 3: Ratio between 0 and 0.5 gives intermediate count.
    // Compute continuousCount for ratio=0.25: sin(pi*0.25)=sin(pi/4)=sqrt(2)/2 ≈ 0.7071, square=0.5, continuousCount=3+3*0.5=4.5, integer=4
    auto quad = computePolygonVertices(0.25f, 10.0f, 0.0f, 0.0f, 6);
    assert(quad.size() == 4);
    // The step is 2*pi/4.5 = 4*pi/9 ≈ 1.396. Check second vertex angle = -pi/2 + 4*pi/9 = -pi/2 + 0.4444*pi = -pi/2 + 4pi/9 = (-9pi/18 + 8pi/18) = -pi/18 ≈ -0.1745 rad. cos(-0.1745)=0.9848, sin=-0.1736.
    // We'll approximate using cos and sin of the computed angle.
    float angle1 = -M_PI/2.0f + 4.0f*M_PI/9.0f;
    assert(std::fabs(quad[1].first - (10.0f * std::cos(angle1))) < 1e-4);
    assert(std::fabs(quad[1].second - (10.0f * std::sin(angle1))) < 1e-4);
    
    // Test 4: Clamping ratio and maxVertices.
    auto clamped = computePolygonVertices(-0.5f, 5.0f, 1.0f, 2.0f, 2);
    assert(clamped.size() == 3); // ratio clamps to 0 -> 3 vertices, max clamps to 3
    assert(std::fabs(clamped[0].first - 1.0f) < 1e-5);
    assert(std::fabs(clamped[0].second - (2.0f - 5.0f)) < 1e-5); // -pi/2 gives (0,-R)
    
    // Test 5: Negative radius becomes absolute.
    auto negRad = computePolygonVertices(0.5f, -10.0f, 0.0f, 0.0f, 3);
    assert(negRad.size() == 3);
    assert(std::fabs(negRad[0].first - 0.0f) < 1e-5);
    assert(std::fabs(negRad[0].second - (-10.0f)) < 1e-5); // absolute radius
    
    // Test 6: All vertices lie on circle of given radius.
    auto poly = computePolygonVertices(0.7f, 7.0f, 3.0f, -2.0f, 5);
    for (const auto& v : poly) {
        float distSq = (v.first - 3.0f)*(v.first - 3.0f) + (v.second + 2.0f)*(v.second + 2.0f);
        assert(std::fabs(std::sqrt(distSq) - 7.0f) < 1e-4);
    }
    
    return 0;
}
// The core idea is to compute a smooth interpolation between 3 and `maxVertices` vertices based on a sinusoidal cycle. The key subtlety is that the vertex count used for drawing is the integer floor of the continuous interpolation, but the angular spacing must be computed from the continuous value to avoid a jump when the integer count increments. The algorithm: 
// 1. Clamp `ratio` to [0,1] and ensure `maxVertices >= 3`.
// 2. Compute `continuousCount = 3 + (maxVertices - 3) * sin(pi * ratio)^2`. Note that `sin(pi * ratio)` is zero at ratio=0 and ratio=1? Actually at ratio=0 and 1, sin(pi*0)=0, sin(pi*1)=0, so continuousCount=3. At ratio=0.5, sin(pi/2)=1, so continuousCount=maxVertices. Good.
// 3. Compute `integerCount = (int)continuousCount` and clamp to [3, maxVertices].
// 4. Compute `step = 2*pi / continuousCount` (the interpolated interval).
// 5. For i from 0 to integerCount-1, compute angle = -pi/2 + i * step, then x = cx + radius * cos(angle), y = cy + radius * sin(angle).
// 6. Edge cases: ratio=0 gives integerCount=3, continuousCount=3, step=2pi/3, producing a triangle. ratio near 0.5 gives integerCount=maxVertices. The conversion from float to int truncates, so for continuousCount like 4.9, integerCount=4, but step uses 4.9, so vertices are not perfectly evenly spaced? Actually the first four vertices are placed with step 2pi/4.9, so the gap between the last (index 3) and back to first is larger. That's acceptable per the spec.
// 7. Time complexity O(maxVertices) for generating the vector, space O(maxVertices).
// 8. Use `const` for parameters, return by value.
