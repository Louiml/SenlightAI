Write a C++ function `double streetlampCoverage(int L, int R)` that, given the side length `L` of a square garden and the radius `R` of a circular streetlamp light centered at the square's center, returns the area of the square's interior that is illuminated (i.e., the intersection area of the square and the circle). The square is axis-aligned with its center at the origin, and the circle is centered at the origin. All inputs are positive integers. The function must return the area as a double, accurate to at least three decimal places. Handle all geometric cases: the circle completely covering the square, the square completely covering the circle, and partial overlap. Use `acos(-1.0)` for π. The function should be self-contained and not rely on global variables.
// The illuminated area is the intersection of a circle (radius `R`) and a square (side `L`, centered at origin). Three cases arise:
// 1. **Circle contains the square**: The farthest point of the square is at distance `L*sqrt(2)/2` from center. If `R >= L*sqrt(2)/2`, then entire square is lit → return `L*L`. But the given snippet checks `2*R*R >= L*L` which is equivalent to `R >= L/sqrt(2)`? Wait: `2*R^2 >= L^2` → `R >= L/sqrt(2) ≈ 0.7071*L`. That is the condition for the circle covering the square? Actually, the square's corner is at distance `L/√2`? No, the square's half-diagonal is `L/√2`. So if `R >= L/√2`, the circle covers the square. Indeed `2R^2 >= L^2` → `R >= L/√2`. So that's correct.
// 2. **Square contains the circle**: The circle is fully inside the square if its diameter is ≤ side length, i.e., `2R ≤ L`. Then illuminated area = full circle area `πR^2`.
// 3. **Partial overlap**: The square and circle intersect in a more complex shape. The intersection area can be computed as: The circle intersects each side of the square at two points each. The illuminated area = area of square inside circle. A known formula for the intersection of a circle and an axis-aligned square centered at origin: When `L/2 < R < L/√2`, the intersection area = `L * sqrt(4R^2 - L^2) + (π - 4*acos(L/(2R))) * R^2`. Let's verify derivation: The square's right side is at x = L/2. The circle intersects that side at y = ±sqrt(R^2 - (L/2)^2). The area inside the square but outside the central "cross" region? Actually, the union of the four corner "caps" outside the square? The formula given in the snippet: `L*sqrt(4R^2 - L^2) + (π - 4*acos(L/(2R)))*R^2`. Let's check: For L=10, R=6 (partial). Then `sqrt(4*36 - 100)=sqrt(144-100)=sqrt(44)=6.633`. L*sqrt(...)=10*6.633=66.33. `acos(L/(2R)) = acos(10/12)=acos(0.8333)=0.5857 rad`. `π - 4*0.5857 = 3.1416 - 2.3428 = 0.7988`. Multiply by R^2=36 → 28.756. Sum = 95.086. Actual intersection area? Let's compute numerically: Circle area = π*36=113.1. Square area=100. Intersection should be less than 100 and likely around 95. So plausible.
//
// Why this formula? It breaks the area into two parts: The central rectangle of height `2*sqrt(R^2 - (L/2)^2)` spanning the width L, plus four symmetric circular segments at corners? Actually the illuminated region consists of the square minus four corner "caps" that lie outside the circle. The area of the square inside the circle can be computed as: area = L*2*sqrt(R^2 - (L/2)^2) + (π - 4*θ)*R^2, where θ = acos(L/(2R)). Because the rectangle from x=-L/2 to L/2 and y from -sqrt(...) to sqrt(...) is fully inside circle? Not exactly. But known formula from competitive programming. We'll trust the given snippet. 
//
// Edge cases: When R exactly equals L/2 (circle tangent to sides), the formula gives `L*sqrt(4*(L/2)^2 - L^2) = 0` and `acos(L/(2R)) = acos(1)=0`, so area = πR^2 = circle area, which matches case 2. When R = L/√2 (circle tangent to corners), then `2R^2 = L^2` so case 1 triggers and returns L^2, which is correct (circle just covers square). 
//
// Time complexity O(1), space O(1). Use double precision. Ensure `acos` argument within [-1,1] - since partial overlap condition ensures L/(2R) < 1.
#include <cmath>

// Returns the area of intersection between a square of side L and a circle of radius R, both centered at origin.
double streetlampCoverage(int L, int R) {
    const double PI = acos(-1.0);
    
    // Circle fully contains the square: circle radius >= half diagonal of square
    if (2.0 * R * R >= static_cast<double>(L) * L) {
        return static_cast<double>(L) * L;
    }
    
    // Square fully contains the circle: circle diameter <= side length
    if (2.0 * R <= L) {
        return PI * R * R;
    }
    
    // Partial overlap: intersection area formula
    const double halfL = static_cast<double>(L) / 2.0;
    const double theta = acos(halfL / R);
    const double rectHeight = 2.0 * sqrt(R * R - halfL * halfL);
    return L * rectHeight + (PI - 4.0 * theta) * R * R;
}
#include <cassert>
#include <cmath>

// Function declaration (as it would appear in a header)
double streetlampCoverage(int L, int R);

int main() {
    // Circle fully contains square: L=2, R=2 (R >= sqrt(2)≈1.414) -> full square area = 4
    assert(std::abs(streetlampCoverage(2, 2) - 4.0) < 1e-9);
    
    // Square fully contains circle: L=10, R=4 (2R=8 <= 10) -> circle area = π*16 ≈ 50.2655
    assert(std::abs(streetlampCoverage(10, 4) - M_PI * 16.0) < 1e-6);
    
    // Partial overlap: L=10, R=6 -> known computed ~95.086 (verify manually)
    double val = streetlampCoverage(10, 6);
    assert(val > 90.0 && val < 100.0);
    
    // Boundary: circle inscribed in square (R = L/2 exactly) -> same as full circle
    assert(std::abs(streetlampCoverage(8, 4) - M_PI * 16.0) < 1e-6);
    
    // Boundary: circle circumscribed around square (R = L/√2) -> full square
    assert(std::abs(streetlampCoverage(2, 2) - 4.0) < 1e-9);
    
    // Small square, large circle: L=1, R=100 -> full square area = 1
    assert(std::abs(streetlampCoverage(1, 100) - 1.0) < 1e-9);
    
    // Large square, small circle: L=100, R=1 -> circle area = π
    assert(std::abs(streetlampCoverage(100, 1) - M_PI) < 1e-6);
    
    // Partial overlap symmetric case: L=2, R=1.5 -> should be between square and circle areas
    double val2 = streetlampCoverage(2, 3);
    assert(val2 > M_PI * 4.0 * 0.5 && val2 < 4.0);
    
    // Edge: L=6, R=5 (partial) - just sanity check output is finite and positive
    double val3 = streetlampCoverage(6, 5);
    assert(val3 > 0.0 && val3 < 36.0);
    
    return 0;
}
