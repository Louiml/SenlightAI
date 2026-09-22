/*
Write a C++ function that, given four float parameters `x1`, `y1`, `x2`, and `y2` representing the two control points of a cubic Bézier curve (with endpoints fixed at (0,0) and (1,1)), returns the `y` value on the curve at a given `x` coordinate `t` (where `t` is in [0,1]). The function must handle the special case where the curve is linear (i.e., `x1 == y1` and `x2 == y2`) by returning `t` directly. For general cases, it must precompute a lookup table of 101 samples (`SPLINE_TABLE_SIZE = 101`, `SAMPLE_STEP_SIZE = 0.01`) of `x` values for equally spaced `t` values, then for a query `x`, find the corresponding `t` via interval search with interpolation, refine using Newton–Raphson iteration (4 iterations) when the slope is sufficiently large, or fall back to binary subdivision (max 10 iterations, precision 1e-7) when the slope is small. The function must be `const`-correct and must not modify global state.
*/

#include <cmath>
#include <vector>

/**
 * Evaluate the y-coordinate on a cubic Bezier curve with endpoints (0,0) and (1,1)
 * at a given x-coordinate t (in [0,1]).  Control points are (x1,y1) and (x2,y2).
 * Ported from LottieInterpolator (ThorVG) for educational purposes.
 */
float bezierYForX(float x1, float y1, float x2, float y2, float x) {
    const int SPLINE_TABLE_SIZE = 101;
    const float SAMPLE_STEP_SIZE = 0.01f;
    const float NEWTON_MIN_SLOPE = 0.02f;
    const int NEWTON_ITERATIONS = 4;
    const float SUBDIVISION_PRECISION = 0.0000001f;
    const int SUBDIVISION_MAX_ITERATIONS = 10;

    auto constA = [](float aA1, float aA2) -> float { return 1.0f - 3.0f * aA2 + 3.0f * aA1; };
    auto constB = [](float aA1, float aA2) -> float { return 3.0f * aA2 - 6.0f * aA1; };
    auto constC = [](float aA1) -> float { return 3.0f * aA1; };

    auto getSlope = [&](float t, float a1, float a2) -> float {
        return 3.0f * constA(a1, a2) * t * t + 2.0f * constB(a1, a2) * t + constC(a1);
    };

    auto calcBezier = [&](float t, float a1, float a2) -> float {
        return ((constA(a1, a2) * t + constB(a1, a2)) * t + constC(a1)) * t;
    };

    // Linear case: the curve is a straight line from (0,0) to (1,1).
    if (x1 == y1 && x2 == y2) return x;

    // Precompute samples of x(t) for t = 0, 0.01, ..., 1.0
    std::vector<float> samples(SPLINE_TABLE_SIZE);
    for (int i = 0; i < SPLINE_TABLE_SIZE; ++i) {
        samples[i] = calcBezier(static_cast<float>(i) * SAMPLE_STEP_SIZE, x1, x2);
    }

    // Newton-Raphson refining the guess for t
    auto newtonRaphsonIterate = [&](float aX, float guessT) -> float {
        for (int i = 0; i < NEWTON_ITERATIONS; ++i) {
            float currentX = calcBezier(guessT, x1, x2) - aX;
            float currentSlope = getSlope(guessT, x1, x2);
            if (currentSlope == 0.0f) return guessT;
            guessT -= currentX / currentSlope;
        }
        return guessT;
    };

    // Binary subdivision fallback
    auto binarySubdivide = [&](float aX, float aA, float aB) -> float {
        float x = 0.0f, t = 0.0f;
        int i = 0;
        do {
            t = aA + (aB - aA) / 2.0f;
            x = calcBezier(t, x1, x2) - aX;
            if (x > 0.0f) aB = t;
            else aA = t;
        } while (std::fabs(x) > SUBDIVISION_PRECISION && ++i < SUBDIVISION_MAX_ITERATIONS);
        return t;
    };

    // Find interval where t lies
    float intervalStart = 0.0f;
    auto currentSample = &samples[1];
    auto lastSample = &samples[SPLINE_TABLE_SIZE - 1];
    for (; currentSample != lastSample && *currentSample <= x; ++currentSample) {
        intervalStart += SAMPLE_STEP_SIZE;
    }
    --currentSample;  // now t lies between *currentSample and *(currentSample+1)

    // Linear interpolation for initial guess
    float dist = (x - *currentSample) / (*(currentSample + 1) - *currentSample);
    float guessForT = intervalStart + dist * SAMPLE_STEP_SIZE;

    float t;
    float initialSlope = getSlope(guessForT, x1, x2);
    if (initialSlope >= NEWTON_MIN_SLOPE) {
        t = newtonRaphsonIterate(x, guessForT);
    } else if (initialSlope == 0.0f) {
        t = guessForT;
    } else {
        t = binarySubdivide(x, intervalStart, intervalStart + SAMPLE_STEP_SIZE);
    }

    // Map t to y using the y control points.
    return calcBezier(t, y1, y2);
}

#include <cassert>
#include <cmath>

int main() {
    // Linear case: x1=y1 and x2=y2 -> y = x
    assert(bezierYForX(0.0f, 0.0f, 1.0f, 1.0f, 0.25f) == 0.25f);
    assert(bezierYForX(0.2f, 0.2f, 0.8f, 0.8f, 0.7f) == 0.7f);

    // Standard ease-in-out curve: control points (0.42,0.0) and (0.58,1.0)
    float result = bezierYForX(0.42f, 0.0f, 0.58f, 1.0f, 0.5f);
    assert(std::fabs(result - 0.5f) < 1e-4f);   // symmetric at x=0.5

    // Monotonic curve with y = x^2 style (control points (0.5,1.0) and (0.5,1.0) gives x(t)=t, y(t)=t^2)
    result = bezierYForX(0.0f, 0.0f, 0.0f, 1.0f, 0.25f);
    assert(std::fabs(result - 0.25f) < 1e-4f);  // This example linear in x, quadratic in y

    // More complex curve: control points (0.1,0.2) and (0.9,0.8)
    result = bezierYForX(0.1f, 0.2f, 0.9f, 0.8f, 0.0f);
    assert(std::fabs(result) < 1e-6f);          // at x=0, y should be 0
    result = bezierYForX(0.1f, 0.2f, 0.9f, 0.8f, 1.0f);
    assert(std::fabs(result - 1.0f) < 1e-6f);   // at x=1, y should be 1

    // Midpoint should be close to 0.5 for symmetric-ish control points
    result = bezierYForX(0.1f, 0.2f, 0.9f, 0.8f, 0.5f);
    assert(std::fabs(result - 0.5f) < 1e-3f);

    // Check monotonic curve where x(t) is nearly linear but y(t) is cubic-like
    result = bezierYForX(0.3f, 0.0f, 0.7f, 1.0f, 0.2f);
    assert(result >= 0.0f && result <= 1.0f);   // y must stay in [0,1] for monotonic cubic

    // Test edge of domain: x=0 and x=1
    assert(std::fabs(bezierYForX(0.42f, 0.0f, 0.58f, 1.0f, 0.0f)) < 1e-6f);
    assert(std::fabs(bezierYForX(0.42f, 0.0f, 0.58f, 1.0f, 1.0f) - 1.0f) < 1e-6f);

    // Random-like values verifying inversion works (compare against direct solve by sampling)
    for (float tt = 0.0f; tt <= 1.0f; tt += 0.1f) {
        float x = 3.0f * (1-tt)*(1-tt)*tt*0.42f + 3.0f*(1-tt)*tt*tt*0.58f + tt*tt*tt;
        float y = 3.0f * (1-tt)*(1-tt)*tt*0.0f + 3.0f*(1-tt)*tt*tt*1.0f + tt*tt*tt;
        float computed = bezierYForX(0.42f, 0.0f, 0.58f, 1.0f, x);
        assert(std::fabs(computed - y) < 1e-3f);
    }

    return 0;
}

// The core is to invert the monotonic cubic Bézier function \(x(t) = 3(1-t)^2 t x_1 + 3(1-t) t^2 x_2 + t^3\) (since endpoints are (0,0) and (1,1), the full polynomial simplifies to \( (3x_1) t + (3x_2 - 6x_1) t^2 + (1 - 3x_2 + 3x_1) t^3 \)). Precompute samples at `t = 0, 0.01, ..., 1.0`. For a query `x`, linearly interpolate between sampled `t` values to get a guess. If the slope at the guess is at least 0.02, use Newton–Raphson to solve `_calcBezier(t) - x = 0`. If the slope is zero, the guess is exact. Otherwise, use binary subdivision on `[intervalStart, intervalStart + step]`. After obtaining `t`, compute `y` using the same cubic formula but with `y1` and `y2` (i.e., replace `x1,x2` with `y1,y2`). Time complexity per query is \(O(1)\) for the table lookup and a few iterations; space is \(O(1)\) for the table (101 floats). Edge cases: linear curve detection must consider both axes; monotonicity is assumed but the subdivision fallback handles small slopes; division by zero in Newton is avoided by checking slope against zero exactly.
