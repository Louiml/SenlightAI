Write a standalone C++ function `int escapeTime(double cRe, double cIm, int maxIter, double escapeRadius)` that simulates the Mandelbrot iteration `z_{n+1} = z_n^2 + c` starting from `z_0 = 0`, where `c` is the complex number with real part `cRe` and imaginary part `cIm`. The function must return the iteration count at which the magnitude of `z` first exceeds `escapeRadius`, and return `0` if the point has not escaped after `maxIter` iterations (i.e., the point is considered inside the set). The function must also handle edge cases: if `maxIter` is less than or equal to 0, return `0`; if `escapeRadius` is less than or equal to 0, use the default value `2.0` instead. The function must be `const`-correct (though as a free function, it should not modify input parameters) and must not use any global state.
// The algorithm simulates the recurrence `z_{n+1} = z_n^2 + c` with `z_0 = 0`. At each iteration, compute the squared magnitude `|z|^2 = re^2 + im^2`. If `|z|^2 > escapeRadius^2`, return the current iteration number (starting from 1 for the first check). If the loop completes without escaping, return `0`. Important edge cases: `maxIter <= 0` should immediately return `0`; `escapeRadius <= 0` should be replaced with `2.0` (the standard Mandelbrot escape radius). The iteration must use doubles to avoid overflow issues, and the magnitude comparison should be done on squared values to avoid unnecessary square roots. Time complexity is `O(maxIter)` per call, and space complexity is `O(1)`.
#include <cmath>

// Simulates the Mandelbrot iteration for a given complex constant c.
// Returns the iteration count when |z| exceeds escapeRadius, or 0 if not escaped.
int escapeTime(double cRe, double cIm, int maxIter, double escapeRadius) {
    if (maxIter <= 0) {
        return 0;
    }
    if (escapeRadius <= 0.0) {
        escapeRadius = 2.0;
    }

    double zRe = 0.0;
    double zIm = 0.0;
    double escapeRadiusSquared = escapeRadius * escapeRadius;

    for (int iter = 1; iter <= maxIter; ++iter) {
        double zReSquared = zRe * zRe;
        double zImSquared = zIm * zIm;

        // Check if |z|^2 > escapeRadius^2
        if (zReSquared + zImSquared > escapeRadiusSquared) {
            return iter;
        }

        // Compute z_{n+1} = z_n^2 + c
        double newRe = zReSquared - zImSquared + cRe;
        double newIm = 2.0 * zRe * zIm + cIm;

        zRe = newRe;
        zIm = newIm;
    }

    return 0;
}
#include <cassert>
#include <cmath>

int main() {
    // Inside the set (c = 0) should never escape, even with many iterations
    assert(escapeTime(0.0, 0.0, 100, 2.0) == 0);

    // c = 1 escapes immediately: z_1 = 1, |z|=1 <= 2; z_2 = 1+1=2, |z|=2 not >2; z_3 = 4+1=5 >2
    // Let's compute: iterations: z0=0, z1=1 (check |1|=1 not escape), z2=2 (check |2|=2 not >2), z3=5 (check |5|=5>2) -> returns 3
    assert(escapeTime(1.0, 0.0, 100, 2.0) == 3);

    // c = -1: z1=-1 (|1| not escape), z2=0 (check), z3=-1 (cycle) -> never escapes
    assert(escapeTime(-1.0, 0.0, 1000, 2.0) == 0);

    // c = 2: z1=2 (|2| not >2), z2=4+2=6 (>2) -> returns 2
    assert(escapeTime(2.0, 0.0, 100, 2.0) == 2);

    // c = 0.5 + 0.5i: known to escape, but slow; test with low maxIter returns 0
    assert(escapeTime(0.5, 0.5, 2, 2.0) == 0);

    // With larger maxIter, it should escape eventually (just check it's not 0 for enough iterations)
    int result = escapeTime(0.5, 0.5, 1000, 2.0);
    assert(result > 0 && result <= 1000);

    // Edge cases: maxIter <= 0 returns 0
    assert(escapeTime(1.0, 0.0, 0, 2.0) == 0);
    assert(escapeTime(1.0, 0.0, -5, 2.0) == 0);

    // Edge case: escapeRadius <= 0 uses default 2.0
    assert(escapeTime(1.0, 0.0, 100, 0.0) == 3);
    assert(escapeTime(1.0, 0.0, 100, -1.0) == 3);

    // Edge case: escapeRadius very small should escape quickly
    assert(escapeTime(0.1, 0.0, 100, 0.1) == 1); // z1 = 0.1, |0.1|^2 = 0.01 > 0.01? equal not >, but next iteration? Actually |0.1| = 0.1, squared=0.01, radius squared=0.01, not >. So z2 = 0.01+0.1=0.11, |0.11|^2=0.0121 > 0.01 -> return 2
    // Let's correct: the above comment is wrong; re-evaluate:
    // c=0.1, z0=0, z1=0.1, check |0.1|^2=0.01, radius^2=0.01, not >, so continue. z2=0.1^2+0.1=0.01+0.1=0.11, check |0.11|^2=0.0121 > 0.01 -> return 2
    assert(escapeTime(0.1, 0.0, 100, 0.1) == 2);

    return 0;
}
