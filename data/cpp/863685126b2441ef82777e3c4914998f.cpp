/*
Write a C++ function that, given a vector of `double` values representing the current point in n-dimensional standard normal space, the gradient of a limit-state function at that point, and a search direction vector, computes the step size for a line search that minimizes the absolute value of the limit-state function along the search direction. Specifically, implement a simple backtracking Armijo line search: start with an initial step size of 1.0, and while the decrease in the absolute value of the limit-state function is not sufficient (i.e., \( |g(u + s \cdot d)| > (1 - c \cdot s) \cdot |g(u)| \) with \( c = 0.1 \)), halve the step size. Return the final step size that satisfies the Armijo condition. Assume the limit-state function is provided as a function pointer that takes a `const std::vector<double>&` and returns a `double`. The function should handle edge cases: if the search direction is zero or the initial step size already satisfies the condition, return 1.0 (or the initial step size); if the absolute value of the limit-state function is zero at the current point, return 1.0 immediately (since the point is on the limit-state surface). Ensure the function is safe against infinite loops by limiting the maximum number of backtracking steps to 100.
*/

#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

/**
 * Perform a backtracking Armijo line search to minimize the absolute value of a
 * limit-state function g along a search direction d from a current point u.
 *
 * The step size s is chosen so that |g(u + s*d)| <= (1 - 0.1*s) * |g(u)|.
 *
 * @param u         Current point in standard normal space.
 * @param gradient  Gradient of g at u (provided for interface compatibility, not used in this simple search).
 * @param g         Limit-state function taking a vector and returning double.
 * @param direction Search direction vector.
 * @param currentValue Pre-computed g(u) (optional; if provided, avoids recomputation).
 * @return Step size s that satisfies the Armijo condition.
 */
double computeStepSizeBacktracking(
    const std::vector<double>& u,
    const std::vector<double>& /*gradient*/,
    double (*g)(const std::vector<double>&),
    const std::vector<double>& direction,
    double currentValue = std::numeric_limits<double>::quiet_NaN())
{
    // Edge case: zero direction
    double dirNorm = 0.0;
    for (double d : direction) {
        dirNorm += d * d;
    }
    if (dirNorm == 0.0) {
        return 1.0;
    }

    // Compute current absolute value if not provided
    double gCurrent;
    if (std::isnan(currentValue)) {
        gCurrent = std::fabs(g(u));
    } else {
        gCurrent = std::fabs(currentValue);
    }

    // If already on the limit-state surface, no step needed
    if (gCurrent == 0.0) {
        return 1.0;
    }

    const double c = 0.1;  // Armijo parameter
    double stepSize = 1.0;
    const int maxIterations = 100;

    for (int iter = 0; iter < maxIterations; ++iter) {
        // Evaluate function at trial point
        std::vector<double> trialPoint(u.size());
        for (size_t i = 0; i < u.size(); ++i) {
            trialPoint[i] = u[i] + stepSize * direction[i];
        }
        double gTrial = std::fabs(g(trialPoint));

        // Check Armijo condition: |g(u+s*d)| <= (1 - c*s) * |g(u)|
        double target = (1.0 - c * stepSize) * gCurrent;
        if (gTrial <= target) {
            return stepSize;
        }

        // Halve step size and retry
        stepSize *= 0.5;
    }

    // If we reached max iterations without satisfying condition, return last step size
    return stepSize;
}

#include <cassert>
#include <cmath>
#include <vector>

// Simple limit-state function: g(u) = u0 + u1 - 1 (a linear plane)
double linearG(const std::vector<double>& u) {
    return u[0] + u[1] - 1.0;
}

// A nonlinear limit-state function: g(u) = u0^2 + u1^2 - 1 (circle)
double circleG(const std::vector<double>& u) {
    return u[0]*u[0] + u[1]*u[1] - 1.0;
}

// A function that is zero at the initial point
double zeroAtStart(const std::vector<double>& u) {
    return u[0] - 1.0;  // zero when u[0] == 1
}

int main() {
    // Test 1: Linear function, starting point (0,0), direction (1,0)
    // g(u) = u0+u1-1, at u=(0,0) g=-1, |g|=1
    // Along dir (1,0): g(s) = s-1, |g(s)|=|s-1|
    // Armijo: |s-1| <= (1 - 0.1*s)*1 for s>0.
    // s=1: |0| <= 0.9? Yes, so step size 1.0 should be returned.
    {
        std::vector<double> u = {0.0, 0.0};
        std::vector<double> dir = {1.0, 0.0};
        double step = computeStepSizeBacktracking(u, {0.0, 0.0}, linearG, dir);
        assert(std::fabs(step - 1.0) < 1e-12);
    }

    // Test 2: Linear function, starting point (2,0), direction (-1,0)
    // g(u)=2+0-1=1, |g|=1. Along dir: g(s)= (2-s)-1 = 1-s, |g|=|1-s|
    // s=1: |0| <= 0.9? Yes -> returns 1.0
    {
        std::vector<double> u = {2.0, 0.0};
        std::vector<double> dir = {-1.0, 0.0};
        double step = computeStepSizeBacktracking(u, {0.0, 0.0}, linearG, dir);
        assert(std::fabs(step - 1.0) < 1e-12);
    }

    // Test 3: Nonlinear circle, start at (0,0), direction (1,1)
    // g(0,0) = -1, |g|=1. Along dir: u(s) = (s,s), g(s)=2s^2-1, |g|=|2s^2-1|
    // s=1: g=1, |g|=1, target=(1-0.1)=0.9, 1 > 0.9 -> fail, halve.
    // s=0.5: g=0.5-1=-0.5, |g|=0.5, target=(1-0.05)=0.95, 0.5 <= 0.95 -> pass.
    // So returned step should be 0.5.
    {
        std::vector<double> u = {0.0, 0.0};
        std::vector<double> dir = {1.0, 1.0};
        double step = computeStepSizeBacktracking(u, {0.0, 0.0}, circleG, dir);
        assert(std::fabs(step - 0.5) < 1e-12);
    }

    // Test 4: Zero direction, should return 1.0
    {
        std::vector<double> u = {0.5, 0.5};
        std::vector<double> dir = {0.0, 0.0};
        double step = computeStepSizeBacktracking(u, {0.0, 0.0}, circleG, dir);
        assert(step == 1.0);
    }

    // Test 5: Starting point where g=0, should return 1.0
    {
        std::vector<double> u = {1.0, 5.0};
        std::vector<double> dir = {1.0, 0.0};
        double step = computeStepSizeBacktracking(u, {0.0, 0.0}, zeroAtStart, dir);
        assert(step == 1.0);
    }

    // Test 6: Verify that the returned step actually satisfies Armijo condition
    {
        std::vector<double> u = {0.0, 0.0};
        std::vector<double> dir = {1.0, 0.0};
        double step = computeStepSizeBacktracking(u, {0.0, 0.0}, linearG, dir);
        std::vector<double> trial = {u[0] + step * dir[0], u[1] + step * dir[1]};
        double gTrial = std::fabs(linearG(trial));
        double gCurrent = std::fabs(linearG(u));
        assert(gTrial <= (1.0 - 0.1 * step) * gCurrent + 1e-12);
    }

    // Test 7: For a function that requires multiple halvings, check result
    // g(u) = u0^2 + u1^2 - 1, start at (10,0), direction (-1,0)
    // g(10,0)=99, |g|=99. Trial s=1: g(9,0)=80, |g|=80, target=99*0.9=89.1, 80<=89.1? Yes -> returns 1.0
    {
        std::vector<double> u = {10.0, 0.0};
        std::vector<double> dir = {-1.0, 0.0};
        double step = computeStepSizeBacktracking(u, {0.0, 0.0}, circleG, dir);
        assert(std::fabs(step - 1.0) < 1e-12);
    }

    // Test 8: Use provided currentValue parameter (avoid recomputation)
    {
        std::vector<double> u = {0.0, 0.0};
        std::vector<double> dir = {1.0, 0.0};
        double current = linearG(u); // -1
        double step = computeStepSizeBacktracking(u, {0.0, 0.0}, linearG, dir, current);
        assert(std::fabs(step - 1.0) < 1e-12);
    }

    // Test 9: Extreme nonlinear case requiring many halvings
    // g(u) = u0^4 - 100, start at (0,0), direction (1,0)
    // g(0)=-100, |g|=100. s=1: g(1)=1-100=-99, |g|=99, target=100*0.9=90, 99>90 fail.
    // s=0.5: g(0.5)=0.0625-100=-99.9375, |g|=99.9375, target=100*0.95=95, fail.
    // s=0.25: g= -99.996..., target=100*0.975=97.5, fail.
    // s=0.125: g≈-99.99975, target=100*0.9875=98.75, fail.
    // ... eventually s must be very small for |g| to be less than 100*(1-0.1*s).
    // The condition is |100 - s^4| <= 100 - 10*s. For small s this is approximately 100 - s^4 <= 100 - 10s -> s^4 >= 10s -> s^3 >= 10 -> s >= 2.15, impossible for small s.
    // So no step satisfies, and we return the smallest step after 100 halvings = 2^-100 ~ 7.9e-31.
    {
        std::vector<double> u = {0.0, 0.0};
        std::vector<double> dir = {1.0, 0.0};
        double step = computeStepSizeBacktracking(u, {0.0, 0.0}, 
            [](const std::vector<double>& v) { return v[0]*v[0]*v[0]*v[0] - 100.0; }, dir);
        // After 100 halvings, step = 2^-100
        double expected = std::pow(2.0, -100);
        assert(std::fabs(step - expected) < 1e-12);
    }

    return 0;
}

// The solution uses a standard backtracking Armijo line search adapted for the absolute value of the function. The armijo condition for minimization of \( f(s) = |g(u + s d)| \) is \( f(s) \leq (1 - c s) f(0) \). We start with \( s = 1.0 \), evaluate \( f(s) \), and if the condition fails, halve \( s \) repeatedly. The algorithm terminates when either the condition holds or the maximum number of iterations (100) is reached, in which case we return the last computed step size (even if it doesn't fully satisfy the condition, to avoid infinite loops). Edge cases: if the norm of the direction is zero, return 1.0; if \( f(0) = 0 \), return 1.0 because the point is on the limit-state surface and no step is needed; if the initial step size works, return it. The function is O(1) space and O(iterations) time, with at most 100 evaluations of the function. For typical cases, the number of iterations is small (usually fewer than 10) because halving converges quickly.
