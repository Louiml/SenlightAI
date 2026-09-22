Write a C++ function `double computeSphereSegmentVolume(int radius, int height)` that computes the volume of a spherical segment (a portion of a sphere cut by a plane) using the formula \( V = \pi h^2 (R - \frac{h}{3}) \), where \( R \) is the sphere's radius and \( h \) is the height of the segment. The function must return the result as a `double` (not rounded to an integer), and it must validate its inputs: if either argument is non-positive, or if the height exceeds twice the radius (\( h > 2R \)), the function should return `-1.0` to indicate an invalid input (since such a segment cannot exist). Additionally, implement a helper function that calculates the summed volume over two parallel arrays of radii and heights, correctly handling the case where any pair is invalid (in that case, the sum should become `-1.0` and stop further computation). The helper function must not modify the input arrays and must accept them as constant pointers.

// The problem breaks into two parts. First, the core calculation: given valid `R` and `h`, apply the formula directly using `M_PI` from `<cmath>` and convert to `double` by using `static_cast<double>` on one operand or by using double literals (e.g., `3.0`). Edge cases: invalid inputs (non-positive `R` or `h`, or `h > 2*R`) must return `-1.0`. Second, the summation function: iterate through both arrays index by index, call the core function for each pair; if any call returns `-1.0`, immediately return `-1.0` (short-circuit) because the summation is meaningless with invalid data. Otherwise accumulate the double results. Since the arrays are fixed-size runtime inputs, we pass them as `const int*` and a length parameter. Time complexity is \(O(n)\) where \(n\) is the array length, because we process each pair once. Space complexity is \(O(1)\) beyond the input arrays, as we only use a few local variables.

#include <cmath>

// Compute volume of spherical segment. Returns -1.0 for invalid input.
double computeSphereSegmentVolume(int radius, int height) {
    if (radius <= 0 || height <= 0 || height > 2 * radius) {
        return -1.0;
    }
    double R = static_cast<double>(radius);
    double h = static_cast<double>(height);
    return M_PI * h * h * (R - h / 3.0);
}

// Sum volumes over parallel arrays. Returns -1.0 if any pair is invalid.
double sumSegmentVolumes(const int* radii, const int* heights, int length) {
    double total = 0.0;
    for (int i = 0; i < length; ++i) {
        double volume = computeSphereSegmentVolume(radii[i], heights[i]);
        if (volume < 0.0) {
            return -1.0;
        }
        total += volume;
    }
    return total;
}

#include <cassert>
#include <cmath>

int main() {
    // Valid: R=3, h=2 -> V = pi * 4 * (3 - 2/3) = pi * 4 * (7/3) ≈ 29.3215
    assert(std::fabs(computeSphereSegmentVolume(3, 2) - (M_PI * 4.0 * (3.0 - 2.0/3.0))) < 1e-9);

    // Valid: R=1, h=1 -> V = pi * 1 * (1 - 1/3) = pi * 2/3 ≈ 2.0944
    assert(std::fabs(computeSphereSegmentVolume(1, 1) - (M_PI * 2.0 / 3.0)) < 1e-9);

    // Invalid: negative radius
    assert(computeSphereSegmentVolume(-1, 5) == -1.0);

    // Invalid: zero height
    assert(computeSphereSegmentVolume(5, 0) == -1.0);

    // Invalid: height exceeds 2R
    assert(computeSphereSegmentVolume(2, 5) == -1.0);

    // Sum with valid arrays: R={3,1}, H={2,1} -> sum = ~29.3215 + ~2.0944 ≈ 31.4159
    int radii1[] = {3, 1};
    int heights1[] = {2, 1};
    double sum1 = sumSegmentVolumes(radii1, heights1, 2);
    double expected1 = M_PI * 4.0 * (3.0 - 2.0/3.0) + M_PI * 2.0 / 3.0;
    assert(std::fabs(sum1 - expected1) < 1e-9);

    // Sum with an invalid pair -> returns -1.0
    int radii2[] = {3, 2};
    int heights2[] = {2, 5}; // second pair invalid because H=5 > 2*2=4
    assert(sumSegmentVolumes(radii2, heights2, 2) == -1.0);

    // Empty array (length 0) -> sum of zero volumes = 0.0
    int radii3[] = {};
    int heights3[] = {};
    assert(sumSegmentVolumes(radii3, heights3, 0) == 0.0);
}
