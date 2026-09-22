You are given a simplified model of a push-broom line-scan camera that images a distant ground point as the spacecraft moves. The camera has a fixed focal length `focalLength`, and its pointing direction (unit vector `lookC`) at any time `et` can be computed by a provided callback `lookDirection(double et, double lookC[3])`. The mapping from a look direction to a detector line offset (in units of pixels from the detector center line) is `lineOffset = focalLength * lookC[1] / lookC[2]` plus a constant `detectorLineOffset`. Write a C++ free function `double findImagingTime(double ra, double dec, double focalLength, double detectorLineOffset, double lineRate, double cacheStartTime, double cacheEndTime, const std::function<void(double, double, double[3])>& lookDirection)` that returns the time `et` within the cache interval `[cacheStartTime, cacheEndTime]` when the ground point (defined by a fixed direction, here represented by the pair `(ra, dec)`, but for simplicity the function only needs the look direction values returned by the callback at that time) exactly falls on the detector center line (i.e., line offset is zero). The function must use the same bracketing-and-regula-falsi-style iteration as the provided snippet: first evaluate the line offset at both cache endpoints; if both are on the same side of zero (both negative or both positive), return `NaN` (no solution); otherwise, iterate up to 30 times using linear interpolation to guess the root, updating the bracket based on the sign of the current offset, and stop when the time step is less than `lineRate/10.0` or the offset is exactly zero. The returned value must be the time at which the offset is zero. The solution must handle the case where the time direction is reversed (start time greater than end time) by appropriately ordering the bracket.
#include <cassert>
#include <cmath>
#include <functional>

// The solution function is assumed to be declared here (or included from the solution file).

int main() {
    // Simple linear case: lookC[1] increases linearly with time.
    // Choose parameters so that the root is exactly at the midpoint.
    double focal = 10.0;
    double detOffset = 5.0; // So line offset = 5 - 10*(lookC[1]/lookC[2]).
    double rate = 2.0;

    // Look direction: z = 1, y = t (so y/z = t), x = 0.
    auto linearLook = [](double et, double lookC[3]) {
        lookC[0] = 0.0;
        lookC[1] = et; // y = time
        lookC[2] = 1.0;
    };

    // Root when 5 - 10*t = 0 => t = 0.5.
    double t1 = findImagingTime(0.0, 0.0, focal, detOffset, rate,
                                0.0, 1.0, linearLook);
    assert(std::fabs(t1 - 0.5) < 1e-9);

    // Reverse bracket: start=1.0, end=0.0. Root is still 0.5.
    double t2 = findImagingTime(0.0, 0.0, focal, detOffset, rate,
                                1.0, 0.0, linearLook);
    assert(std::fabs(t2 - 0.5) < 1e-9);

    // No solution: both endpoints give offset > 0 (e.g., root outside bracket).
    double t3 = findImagingTime(0.0, 0.0, focal, detOffset, rate,
                                0.0, 0.2, linearLook);
    assert(std::isnan(t3));

    // No solution: both endpoints give offset < 0.
    double t4 = findImagingTime(0.0, 0.0, focal, detOffset, rate,
                                0.6, 1.0, linearLook);
    assert(std::isnan(t4));

    // Root exactly at left endpoint (offset zero at t=0.5 if bracket starts there).
    double t5 = findImagingTime(0.0, 0.0, focal, detOffset, rate,
                                0.5, 1.0, linearLook);
    assert(std::fabs(t5 - 0.5) < 1e-9);

    // Root exactly at right endpoint.
    double t6 = findImagingTime(0.0, 0.0, focal, detOffset, rate,
                                0.0, 0.5, linearLook);
    assert(std::fabs(t6 - 0.5) < 1e-9);

    // Nonlinear case: y = sin(t), z = 1. For detOffset=0, root at t=0.
    auto sinLook = [](double et, double lookC[3]) {
        lookC[0] = 0.0;
        lookC[1] = std::sin(et);
        lookC[2] = 1.0;
    };
    double t7 = findImagingTime(0.0, 0.0, 1.0, 0.0, 0.1,
                                -0.5, 0.5, sinLook);
    assert(std::fabs(t7) < 1e-9);

    // Nonlinear case with offset 0.5: root when sin(t)=0.5 => t=pi/6 ~0.5236.
    double t8 = findImagingTime(0.0, 0.0, 1.0, 0.5, 0.1,
                                -0.5, 0.5, sinLook);
    assert(std::fabs(t8 - 0.5235987755982988) < 1e-6);

    // Constant offset (never zero): both endpoints same sign -> NaN.
    auto constLook = [](double et, double lookC[3]) {
        lookC[0] = 0.0;
        lookC[1] = 1.0;
        lookC[2] = 1.0;
    };
    double t9 = findImagingTime(0.0, 0.0, 2.0, 1.0, 1.0,
                                0.0, 1.0, constLook);
    assert(std::isnan(t9));

    return 0;
}
#include <cmath>
#include <functional>
#include <limits>

/**
 * Find the time in [cacheStartTime, cacheEndTime] when the line-scan camera
 * images a fixed ground direction exactly on the detector center line.
 *
 * @param ra                 (unused) right ascension in degrees (kept for interface parity)
 * @param dec                (unused) declination in degrees (kept for interface parity)
 * @param focalLength        camera focal length in pixels
 * @param detectorLineOffset constant pixel offset of the detector center line
 * @param lineRate           time per scan line in seconds
 * @param cacheStartTime     start of the time search interval
 * @param cacheEndTime       end of the time search interval
 * @param lookDirection      callback returning the unit look vector (lookC[0..2])
 *                           in the camera frame at a given ephemeris time
 * @return the time when line offset is zero, or NaN if no solution exists
 */
double findImagingTime(double ra, double dec, double focalLength,
                       double detectorLineOffset, double lineRate,
                       double cacheStartTime, double cacheEndTime,
                       const std::function<void(double, double, double[3])>& lookDirection) {
    (void)ra;  // unused in this simplified model
    (void)dec; // unused in this simplified model

    // Evaluate line offset at both bounding times.
    auto lineOffsetAt = [&](double et) -> double {
        double lookC[3];
        lookDirection(et, lookC);
        // The line offset is detector center minus projected y position.
        return detectorLineOffset - focalLength * lookC[1] / lookC[2];
    };

    double startOffset = lineOffsetAt(cacheStartTime);
    double endOffset = lineOffsetAt(cacheEndTime);

    // If both offsets are on the same side of zero, no root exists.
    if ((startOffset < 0.0 && endOffset < 0.0) ||
        (startOffset > 0.0 && endOffset > 0.0)) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // Order the bracket so that xl < xh and fl, fh have opposite signs.
    double fl, fh, xl, xh;
    if (startOffset <= endOffset) {
        fl = startOffset;
        fh = endOffset;
        xl = cacheStartTime;
        xh = cacheEndTime;
    } else {
        fl = endOffset;
        fh = startOffset;
        xl = cacheEndTime;
        xh = cacheStartTime;
    }

    // Regula falsi iteration (same as the given snippet).
    double timeTol = lineRate / 10.0;
    for (int j = 0; j < 30; ++j) {
        double etGuess = xl + (xh - xl) * fl / (fl - fh);
        double f = lineOffsetAt(etGuess);

        double delTime;
        if (f < 0.0) {
            delTime = xl - etGuess;
            xl = etGuess;
            fl = f;
        } else {
            delTime = xh - etGuess;
            xh = etGuess;
            fh = f;
        }

        if (std::fabs(delTime) < timeTol || f == 0.0) {
            return etGuess;
        }
    }
    return std::numeric_limits<double>::quiet_NaN();
}
// The algorithm closely mirrors the original snippet’s iterative root-finding. Key steps:
// 1. Evaluate the line offset at both bounding times (`cacheStartTime` and `cacheEndTime`) using the provided `lookDirection` callback. The line offset `f(t)` is defined as `detectorLineOffset - (focalLength * lookC[1] / lookC[2])`. If both offsets have the same sign (both positive or both negative), no root exists in the interval, so return `NaN`.
// 2. Order the bracket so that `xl` is associated with `fl` (the offset at `xl`) and `xh` with `fh`, and ensure that `fl` and `fh` have opposite signs. If the natural ordering has `startOffset` greater than `endOffset`, swap them along with the corresponding times.
// 3. Use regula falsi (false position) iteration: for up to 30 iterations, compute the guess `etGuess = xl + (xh - xl) * fl / (fl - fh)`. Evaluate `f(etGuess)`. If `f < 0`, then the root lies between `etGuess` and `xh`, so set `xl = etGuess`, `fl = f`; else set `xh = etGuess`, `fh = f`. The previous guess gives `delTime` (the distance from the new guess to the old boundary). Convergence is declared when `abs(delTime) < lineRate/10.0` or `f == 0.0`.
// 4. If convergence occurs, return `etGuess`. If not, after 30 iterations return `NaN`.
// Edge cases: The case where the offset is exactly zero at an endpoint is naturally handled because the initial check will find opposite signs (one zero and one nonzero), and the iteration stops immediately. If the look direction points behind the camera (i.e., `lookC[2] <= 0`), the division is invalid; the callback is assumed to return valid forward-looking directions for the given times. Time complexity is \(O(1)\) (constant number of iterations), space complexity is \(O(1)\).
// The solution is a standalone function that takes the callback as a `std::function` for flexibility, and uses `<cmath>` for `std::fabs` and `std::numeric_limits<double>::quiet_NaN()`.
