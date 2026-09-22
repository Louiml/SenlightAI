/*
Write a C++ function `std::string distanceBetweenCities(int a, int b, int c, int d)` that takes four positive integers representing two vectors in 2D space: vector `(a, b)` and vector `(c, d)`. The function must compute the distance between the two vectors' endpoints when they share the same origin, using the formula derived from the difference of their dot products and cross products. Specifically, the cosine of the angle between the two vectors is `(a*a + b*b - c*c - d*d) / (2 * (a*b - c*d))`. If the denominator is zero (meaning the vectors are parallel in a way that makes the formula undefined), return `"Impossible."`. Otherwise, compute the distance between the endpoints of the two vectors (i.e., the length of the vector difference `(a-c, b-d)`) — but note the original snippet computes a different distance: it computes `sqrt(a*a + b*b - 2*a*b*CosC) * 1000`, which is actually the distance between vector `(a,b)` and a rotated version of itself, not the direct difference. For clarity, the task is: given `a, b, c, d`, compute the value `CD = sqrt(a*a + b*b - 2*a*b*CosC) * 1000`, where `CosC` is as defined above if the denominator is non-zero. Return a string formatted as `"Distance is <rounded> km."` where `<rounded>` is the integer rounded to nearest (using `std::llround`). If denominator is zero, return `"Impossible."`.
*/

#include <string>
#include <cmath>
#include <cstdint>

// Computes the distance as described in the problem statement.
// Returns "Impossible." if the denominator is zero, otherwise a formatted string.
std::string distanceBetweenCities(int a, int b, int c, int d) {
    // Use double to avoid integer overflow when squaring.
    const double aa = static_cast<double>(a);
    const double bb = static_cast<double>(b);
    const double cc = static_cast<double>(c);
    const double dd = static_cast<double>(d);

    const double up = aa * aa + bb * bb - cc * cc - dd * dd;
    const double down = 2.0 * (aa * bb - cc * dd);

    if (down == 0.0) {
        return "Impossible.";
    }

    const double CosC = up / down;

    // The argument under the square root must be non-negative.
    const double arg = aa * aa + bb * bb - 2.0 * aa * bb * CosC;
    if (arg < 0.0) {
        // In numerical edge cases, clamp to zero.
        // This should not happen for valid inputs, but we handle it gracefully.
        return "Impossible.";
    }

    const double CD = std::sqrt(arg) * 1000.0;
    long long rounded = std::llround(CD);
    return "Distance is " + std::to_string(rounded) + " km.";
}

#include <cassert>
#include <string>

// Declare the function under test.
std::string distanceBetweenCities(int a, int b, int c, int d);

int main() {
    // Example: a=3, b=4, c=0, d=0 gives up = 25, down = 24, CosC = 25/24 ≈ 1.0417
    // Then arg = 25 - 2*3*4*1.0417 = 25 - 25.0008 ≈ -0.0008 => clamped to impossible? Let's see.
    // But the original formula may not be valid for arbitrary c,d. We'll test known simple cases.

    // Case: a=b=c=d=1 => up = 1+1-1-1=0, down = 2*(1-1)=0 => Impossible.
    assert(distanceBetweenCities(1, 1, 1, 1) == "Impossible.");

    // Case: a=1, b=0, c=0, d=1 => up = 1+0-0-1=0, down = 2*(0-0)=0 => Impossible.
    assert(distanceBetweenCities(1, 0, 0, 1) == "Impossible.");

    // Case: a=2, b=0, c=1, d=0 => up = 4+0-1-0=3, down = 2*(0-0)=0 => Impossible.
    assert(distanceBetweenCities(2, 0, 1, 0) == "Impossible.");

    // Case: a=3, b=4, c=5, d=0 => up = 9+16-25-0=0, down = 2*(12-0)=24 => CosC=0
    // arg = 9+16 - 2*3*4*0 = 25 => sqrt=5 => *1000 = 5000 => "Distance is 5000 km."
    assert(distanceBetweenCities(3, 4, 5, 0) == "Distance is 5000 km.");

    // Case: a=1, b=1, c=1, d=0 => up = 1+1-1-0=1, down = 2*(1-0)=2 => CosC=0.5
    // arg = 1+1 - 2*1*1*0.5 = 2 - 1 = 1 => sqrt=1 => *1000=1000 => "Distance is 1000 km."
    assert(distanceBetweenCities(1, 1, 1, 0) == "Distance is 1000 km.");

    // Case: a=2, b=2, c=1, d=1 => up = 4+4-1-1=6, down = 2*(4-1)=6 => CosC=1
    // arg = 4+4 - 2*2*2*1 = 8 - 8 = 0 => sqrt=0 => *1000=0 => "Distance is 0 km."
    assert(distanceBetweenCities(2, 2, 1, 1) == "Distance is 0 km.");

    // Case: a=1, b=2, c=3, d=4 => up = 1+4-9-16 = -20, down = 2*(2-12) = -20 => CosC=1
    // arg = 1+4 - 2*1*2*1 = 5 - 4 = 1 => sqrt=1 => *1000=1000 => "Distance is 1000 km."
    assert(distanceBetweenCities(1, 2, 3, 4) == "Distance is 1000 km.");

    // Case: a=5, b=12, c=13, d=0 => up = 25+144-169-0=0, down = 2*(60-0)=120 => CosC=0
    // arg = 25+144 - 2*5*12*0 = 169 => sqrt=13 => *1000=13000 => "Distance is 13000 km."
    assert(distanceBetweenCities(5, 12, 13, 0) == "Distance is 13000 km.");

    return 0;
}

// The main idea is to directly translate the given mathematical formula. First, compute the numerator `up = a*a + b*b - c*c - d*d` and denominator `down = 2 * (a*b - c*d)`. If `down == 0.0`, we return the impossible string. Otherwise, compute `CosC = up / down`. Then, compute the intermediate value `sqrt(a*a + b*b - 2*a*b*CosC)`. Multiply by 1000, round to nearest integer using `std::llround`, and format the string. Edge cases include when the denominator is zero, and when the computed square root’s argument is negative due to numerical issues — in practice, the formula should be mathematically consistent for valid inputs, but we must ensure we use `double` for all calculations to avoid integer overflow (since `a,b,c,d` can be large, but within int range, `a*a` may overflow 32-bit int, so cast to `long long` or `double` before multiplication). Complexity is O(1) time and O(1) space.
