/*
Write a C++ function named `swapDescending` that takes two `double` values by reference and rearranges them so that the first parameter holds the larger value and the second parameter holds the smaller value. The function must handle equal values correctly (no change needed) and must not produce any output. The function should be `const`-correct where applicable and should not use any external libraries beyond the standard ones.
*/

#include <utility> // for std::swap (optional, but used for clarity)

// Rearranges two doubles so that first >= second.
// If they are equal, no change occurs.
void swapDescending(double& first, double& second) {
    if (first < second) {
        std::swap(first, second);
    }
}

#include <cassert>

int main() {
    double a = 3.5, b = 2.1;
    swapDescending(a, b);
    assert(a == 3.5 && b == 2.1);

    double c = -1.2, d = 4.7;
    swapDescending(c, d);
    assert(c == 4.7 && d == -1.2);

    double e = 0.0, f = 0.0;
    swapDescending(e, f);
    assert(e == 0.0 && f == 0.0);

    double g = 5.0, h = 5.0;
    swapDescending(g, h);
    assert(g == 5.0 && h == 5.0);

    double i = 10.0, j = 1.0;
    swapDescending(i, j);
    assert(i == 10.0 && j == 1.0);

    double k = -0.5, l = -0.9;
    swapDescending(k, l);
    assert(k == -0.5 && l == -0.9);

    double m = 2.2, n = 2.2;
    swapDescending(m, n);
    assert(m == 2.2 && n == 2.2);

    return 0;
}

// The solution is straightforward: compare the two values, and if the first is strictly less than the second, swap them. If they are equal or the first is already greater, do nothing. The function uses a temporary variable to perform the swap, which takes O(1) time and O(1) auxiliary space. Edge cases include both inputs being equal (no swap), and inputs where one is negative and the other positive (normal numeric comparison works). Since we are using `double`, we don't need to worry about special values like NaN (though if NaN is passed, comparison may be unpredictable, but that is outside typical constraints).
