/*
Write a C++ function `double triangle_cut_ratio(double a, double b, double c, double ratio)` that takes the side lengths of a triangle (positive real numbers) and a target ratio `ratio` (a positive real number). The function must return the length `x` along side `a` such that a smaller triangle is formed by drawing a line parallel to side `a` at a distance `x` from the vertex opposite to side `a`. The original triangle is cut along this line, splitting it into two regions: the smaller triangle at the vertex and the remaining trapezoid. The function must find `x` such that the area of the smaller triangle divided by the area of the trapezoid is exactly equal to `ratio`. Return the value of `x` with high precision (e.g., using 100 iterations of binary search). The input triangle is guaranteed to be valid (satisfies triangle inequality), and the target ratio is positive.
*/

#include <cmath>

// Compute the area of a triangle using Heron's formula.
// All side lengths must be positive and satisfy triangle inequality.
double triangle_area(double a, double b, double c) {
    double s = (a + b + c) / 2.0;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

// Return the length along side 'a' such that the small triangle at the vertex
// (similar to original) has area ratio to the trapezoid equal to 'target_ratio'.
// Uses binary search for robustness and precision.
double triangle_cut_ratio(double a, double b, double c, double target_ratio) {
    const double full_area = triangle_area(a, b, c);
    
    double low = 0.0;
    double high = a;
    
    for (int iter = 0; iter < 100; ++iter) {
        double mid = (low + high) / 2.0;
        
        // Sides of the smaller triangle, proportional to 'a'.
        double small_a = mid;
        double small_b = b * small_a / a;
        double small_c = c * small_a / a;
        
        double small_area = triangle_area(small_a, small_b, small_c);
        double trapezoid_area = full_area - small_area;
        
        // If ratio is >= target, we overshot, move high down.
        if (small_area / trapezoid_area >= target_ratio) {
            high = mid;
        } else {
            low = mid;
        }
    }
    return low;
}

#include <cassert>
#include <cmath>

// The solution function is declared above; include it or copy here.
// For test, use small tolerance because of floating-point.
bool close_enough(double a, double b, double eps = 1e-6) {
    return std::fabs(a - b) < eps;
}

int main() {
    // Right triangle 3-4-5, with a=3, b=4, c=5.
    // Full area = 6. Suppose ratio=1 means small area = trapezoid area => small area = 3.
    // Then (x/3)^2 * 6 = 3 => (x/3)^2 = 0.5 => x = 3/sqrt(2) ≈ 2.12132.
    assert(close_enough(triangle_cut_ratio(3.0, 4.0, 5.0, 1.0), 2.1213203436, 1e-5));
    
    // ratio=0 means x=0 (small area zero).
    assert(close_enough(triangle_cut_ratio(3.0, 4.0, 5.0, 0.0), 0.0, 1e-6));
    
    // Very large ratio => x approaches a.
    assert(close_enough(triangle_cut_ratio(3.0, 4.0, 5.0, 1e6), 3.0, 1e-3));
    
    // Equilateral triangle side 2, area = sqrt(3) ≈ 1.73205.
    // ratio=3 => small_area = 3 * trapezoid_area => small_area = 3*(full-small) => 4*small = 3*full => small = 0.75*full.
    // (x/2)^2 = 0.75 => x = 2*sqrt(0.75)=sqrt(3)≈1.73205.
    assert(close_enough(triangle_cut_ratio(2.0, 2.0, 2.0, 3.0), 1.7320508076, 1e-5));
    
    // Symmetry check: swapping sides doesn't affect x when a is kept same.
    assert(close_enough(triangle_cut_ratio(3.0, 5.0, 4.0, 1.0), triangle_cut_ratio(3.0, 4.0, 5.0, 1.0), 1e-5));
    
    // Edge case: extremely small ratio, x effectively zero.
    assert(close_enough(triangle_cut_ratio(10.0, 6.0, 8.0, 1e-9), 0.0, 1e-4));
    
    return 0;
}

// The key insight is that when a line is drawn parallel to side `a` inside a triangle, the smaller triangle at the vertex is similar to the original triangle. If the distance from the vertex to the cutting line is `x` along the altitude relative to side `a`, then the scale factor between the smaller triangle and the original is `x / h_a` where `h_a` is the altitude to side `a`. Equivalently, if we cut along side `a` at a length `aa` from one endpoint (but here the code uses `mid` as a fraction of `a` scaled to the other sides), the smaller triangle's sides are proportional to `(aa, b*aa/a, c*aa/a)`. The area of the smaller triangle is `area_small = area_orig * (aa/a)^2`. The trapezoid area is `area_orig - area_small = area_orig * (1 - (aa/a)^2)`. We need `area_small / area_trapezoid == ratio`. Solving symbolically: `(aa/a)^2 / (1 - (aa/a)^2) = ratio` → `(aa/a)^2 = ratio/(1+ratio)` → `aa = a * sqrt(ratio/(1+ratio))`. However, the original snippet uses iterative binary search because it avoids solving directly and may be adapted for more complex scenarios. The binary search approach: set `low=0`, `high=a` (the full side length). At each iteration, compute `mid`, then the area of the small triangle using Heron's formula with the proportional sides. Then compute the ratio of small area to the trapezoid area. If the ratio is >= target, we need to shrink `mid` (because increasing `mid` increases the small area, increasing the ratio), so set `high=mid`; else set `low=mid`. After 100 iterations, return `low`. This converges because the function `f(mid) = area_small(mid)/area_trapezoid(mid)` is monotonically increasing with `mid`. Edge cases: `ratio` can be any positive number; if `ratio` is extremely large, the answer approaches `a` but never reaches it; if `ratio` is very small, answer approaches 0. The initial `area` is computed correctly (the snippet has a bug where it calls `area(a,b,c)` but the function name is `find_area`; we fix that). Time complexity is O(iterations) = O(100), constant. Space O(1).
