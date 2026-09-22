/*
Write a C++ function that solves a first-order ordinary differential equation \( \frac{dy}{dx} = f(x,y) \) using the classical fourth-order Runge-Kutta (RK4) method. The function must accept the initial condition \( (x_0, y_0) \), a target \( x_{\text{target}} \), and a step size \( h \). It should return the approximate value of \( y \) at \( x_{\text{target}} \). The differential equation itself is provided as a separate free function `dydx(float x, float y)` that returns the derivative. The solution must handle cases where the interval is not an exact multiple of the step size (by rounding the number of steps to the nearest integer), and it must correctly integrate forward from \( x_0 \) to \( x_{\text{target}} \) in increments of \( h \). The function must be const-correct and use only standard library headers.
*/
#include <cmath>
#include <cstddef>

// Differential equation: dy/dx = f(x, y)
float dydx(float x, float y) {
    // Example: simple sine function (modify as needed)
    return std::cos(x);
}

// Solve ODE using RK4 from (x0, y0) to x_target with step size h.
float solveRK4(float x0, float y0, float x_target, float h) {
    if (h <= 0.0f || x_target <= x0) {
        return y0;
    }

    const int n = static_cast<int>(std::round((x_target - x0) / h));
    float y = y0;
    float x = x0;

    for (int i = 0; i < n; ++i) {
        const float k1 = h * dydx(x, y);
        const float k2 = h * dydx(x + 0.5f * h, y + 0.5f * k1);
        const float k3 = h * dydx(x + 0.5f * h, y + 0.5f * k2);
        const float k4 = h * dydx(x + h, y + k3);

        y += (1.0f / 6.0f) * (k1 + 2.0f * k2 + 2.0f * k3 + k4);
        x += h;
    }
    return y;
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: dy/dx = cos(x), y(0)=1, find y(1). Exact: y = sin(x) + 1
    float result1 = solveRK4(0.0f, 1.0f, 1.0f, 0.01f);
    float expected1 = std::sin(1.0f) + 1.0f;
    assert(std::fabs(result1 - expected1) < 0.001f);

    // Test 2: dy/dx = cos(x), y(0)=1, find y(0) (no steps)
    assert(solveRK4(0.0f, 1.0f, 0.0f, 0.01f) == 1.0f);

    // Test 3: dy/dx = cos(x), y(0)=1, negative step size (invalid)
    assert(solveRK4(0.0f, 1.0f, 2.0f, -0.01f) == 1.0f);

    // Test 4: dy/dx = cos(x), y(0)=1, find y(2) with coarse step
    float result4 = solveRK4(0.0f, 1.0f, 2.0f, 0.5f);
    float expected4 = std::sin(2.0f) + 1.0f;
    assert(std::fabs(result4 - expected4) < 0.1f);

    // Test 5: dy/dx = cos(x), y(0)=1, find y(10) with fine step
    float result5 = solveRK4(0.0f, 1.0f, 10.0f, 0.001f);
    float expected5 = std::sin(10.0f) + 1.0f;
    assert(std::fabs(result5 - expected5) < 0.01f);

    // Test 6: Large step count (stress test)
    float result6 = solveRK4(0.0f, 0.0f, 3.14159f, 0.0001f);
    float expected6 = std::sin(3.14159f);
    assert(std::fabs(result6 - expected6) < 0.001f);

    return 0;
}
// The RK4 method computes the next value of \( y \) using a weighted average of four slope estimates: \( k_1 \) at the beginning of the interval, \( k_2 \) at the midpoint using \( k_1 \), \( k_3 \) at the midpoint using \( k_2 \), and \( k_4 \) at the end using \( k_3 \). The update formula is \( y_{n+1} = y_n + \frac{h}{6}(k_1 + 2k_2 + 2k_3 + k_4) \). The number of steps is computed as \( n = \text{round}((x_{\text{target}} - x_0)/h) \). Important edge cases: if \( x_{\text{target}} \leq x_0 \), the loop should not execute and the function should return \( y_0 \); if \( h \leq 0 \), the function may return \( y_0 \) or handle gracefully. The loop runs exactly \( n \) times, updating `x0` and `y` each iteration. The solution uses only a constant amount of extra space beyond the input, giving \( O(1) \) space. Time complexity is \( O(n) \), where \( n \) is the number of steps. Numerical accuracy depends on step size; smaller `h` yields more accurate results but increases runtime.
