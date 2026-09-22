Write a standalone C++ function that simulates a single step of a 2D discrete-time dynamical system (a "strange attractor") chosen from one of eight named variants (Clifford, De_Jong, Svensson, Bedhead, Dream, HopalongA, HopalongB, Gumowski_Mira). The function must take as input: an integer selecting the attractor (0–7), four floating-point parameters `a, b, c, d`, and the current state `(x, y)` (as doubles). It must return the new state `(nx, ny)` after applying the corresponding recurrence equation exactly as defined in the provided snippet. The function must not maintain any internal state—it must be purely functional (deterministic, no side effects). Edge cases to consider: when the attractor is changed, the caller is responsible for resetting `(x,y)` to (0,0); the function itself should not reset. For Gumowski_Mira, use the auxiliary function `G(x, mu) = mu*x + 2*(1-mu)*(x*x)/(1.0 + x*x)`. For Hopalong variants, use the sign convention: if the current x is >= 0, use +1, otherwise –1, applied to the square root of the absolute value of the expression inside. Return the new coordinates as an `std::pair<double, double>`.
// The solution is a direct translation of the switch statement from the original code into a standalone function. The key is to preserve the exact arithmetic: all parameters are read as `double` values (converted from the original `float` in the snippet, but the task specifies `double` to avoid precision issues). The recurrence uses the *old* x value (`xp`) for all equations, so we must store `xp = x` and `yp = y` at the start. For Clifford: `nx = sin(a*y) + c*cos(a*xp); ny = sin(b*xp) + d*cos(b*y)`. Note that the original code incorrectly uses `y` (the old value) on the right-hand side for some equations (e.g., Clifford uses `y` in the first equation), so we must replicate that exactly—not replace with `yp` unless the original does. Similarly, for De_Jong: `x = sin(a*y) - cos(b*xp); y = sin(c*xp) - cos(d*y)`—note the second line uses `y` (old) not `yp`, because `y` hasn't been assigned yet. For Svensson: `x = d*sin(a*xp) - sin(b*y); y = c*cos(a*xp) + cos(b*y)`—again uses old `y`. For Bedhead, Dream, HopalongA/B, and Gumowski_Mira, the original uses `xp` and `y` (old) in the first assignment, then for the second assignment uses the newly computed `x` (for Gumowski_Mira, the new `x` is used in `G(x, c)`). We must replicate this sequencing exactly. The Hopalong sign: `(xp >= 0 ? 1.0 : -1.0) * sqrt(fabs(...))`. Include `<cmath>` for `sin`, `cos`, `sqrt`, `fabs`, `pow`. The `G` function is a separate helper. Edge cases: when parameters are such that the square root argument is negative, we take its absolute value before sqrt (as in the original). No other edge cases—the function is defined for all real inputs. Time complexity is O(1) per call; space is O(1). The function should be `const`-correct: parameters are const references, and the helper `G` is `const` or `static` with `const` parameters.
#include <cmath>
#include <utility>

// Helper: G function for Gumowski-Mira attractor.
double G(double x, double mu) {
    return mu * x + 2.0 * (1.0 - mu) * (x * x) / (1.0 + x * x);
}

// Simulate one step of the selected attractor.
// attractor: 0=Clifford, 1=De_Jong, 2=Svensson, 3=Bedhead, 4=Dream,
//            5=HopalongA, 6=HopalongB, 7=Gumowski_Mira
// Returns the new (x, y) state after applying the recurrence.
std::pair<double, double> stepAttractor(int attractor, double a, double b, double c, double d, double x, double y) {
    double xp = x; // old x
    // Note: yp is not needed because the original code never reads "yp" explicitly;
    // it uses the old y (which hasn't been overwritten yet when used in the first line).
    double nx, ny;

    switch (attractor) {
        case 0: // Clifford
            nx = std::sin(a * y) + c * std::cos(a * xp);
            ny = std::sin(b * xp) + d * std::cos(b * y); // uses old y (y is unchanged)
            break;
        case 1: // De_Jong
            nx = std::sin(a * y) - std::cos(b * xp);
            ny = std::sin(c * xp) - std::cos(d * y); // old y
            break;
        case 2: // Svensson
            nx = d * std::sin(a * xp) - std::sin(b * y);
            ny = c * std::cos(a * xp) + std::cos(b * y); // old y
            break;
        case 3: // Bedhead
            nx = std::sin(xp * y / b) * y + std::cos(a * xp - y);
            ny = xp + std::sin(y) / b; // note: uses old y (since y hasn't changed yet)
            break;
        case 4: // Dream
            nx = std::sin(y * b) + c * std::sin(xp * b);
            ny = std::sin(xp * a) + d * std::sin(y * a); // old y
            break;
        case 5: // HopalongA
            nx = y + (xp >= 0.0 ? 1.0 : -1.0) * std::sqrt(std::fabs(b * xp - c));
            ny = a - xp;
            break;
        case 6: // HopalongB
            nx = y - 1.0 + (xp >= 0.0 ? 1.0 : -1.0) * std::sqrt(std::fabs(b * xp - 1.0 - c));
            ny = a - xp - 1.0;
            break;
        case 7: // Gumowski_Mira
            nx = y + a * (1.0 - b * y * y) * y + G(xp, c);
            ny = -xp + G(nx, c); // uses the newly computed nx
            break;
        default:
            // Should not happen; but for safety return (0,0) for invalid attractor.
            nx = 0.0;
            ny = 0.0;
            break;
    }

    return {nx, ny};
}
#include <cassert>
#include <cmath>
#include <utility>

// The solution function is declared here (or included from the solution file).
// For simplicity, we assume the function and G are already defined above.

int main() {
    // Test Clifford with known simple values (a=-1.4, b=1.6, c=1.0, d=0.7)
    {
        auto [nx, ny] = stepAttractor(0, -1.4, 1.6, 1.0, 0.7, 0.1, 0.2);
        // Hand-compute: xp=0.1, y=0.2
        // nx = sin(-1.4*0.2) + 1.0*cos(-1.4*0.1) = sin(-0.28) + cos(-0.14)
        // ny = sin(1.6*0.1) + 0.7*cos(1.6*0.2) = sin(0.16) + 0.7*cos(0.32)
        double expected_nx = std::sin(-1.4*0.2) + 1.0*std::cos(-1.4*0.1);
        double expected_ny = std::sin(1.6*0.1) + 0.7*std::cos(1.6*0.2);
        assert(std::fabs(nx - expected_nx) < 1e-12);
        assert(std::fabs(ny - expected_ny) < 1e-12);
    }

    // Test De_Jong: a=0.4, b=0.7, c=0.5, d=0.2, starting (0,0)
    {
        auto [nx, ny] = stepAttractor(1, 0.4, 0.7, 0.5, 0.2, 0.0, 0.0);
        // xp=0, y=0
        // nx = sin(0) - cos(0) = -1
        // ny = sin(0) - cos(0) = -1
        assert(std::fabs(nx - (-1.0)) < 1e-12);
        assert(std::fabs(ny - (-1.0)) < 1e-12);
    }

    // Test Svensson: a=1.0, b=2.0, c=3.0, d=4.0, (1, -1)
    {
        auto [nx, ny] = stepAttractor(2, 1.0, 2.0, 3.0, 4.0, 1.0, -1.0);
        // xp=1, y=-1
        // nx = 4*sin(1*1) - sin(2*-1) = 4*sin(1) + sin(2)
        // ny = 3*cos(1*1) + cos(2*-1) = 3*cos(1) + cos(2)
        double expected_nx = 4.0*std::sin(1.0) + std::sin(2.0);
        double expected_ny = 3.0*std::cos(1.0) + std::cos(2.0);
        assert(std::fabs(nx - expected_nx) < 1e-12);
        assert(std::fabs(ny - expected_ny) < 1e-12);
    }

    // Test Bedhead with b=1 (to avoid division by zero) and simple values
    {
        auto [nx, ny] = stepAttractor(3, 1.0, 1.0, 0.0, 0.0, 0.5, 0.25);
        // xp=0.5, y=0.25
        // nx = sin(0.5*0.25/1)*0.25 + cos(1*0.5 - 0.25) = sin(0.125)*0.25 + cos(0.25)
        // ny = 0.5 + sin(0.25)/1
        double expected_nx = std::sin(0.125)*0.25 + std::cos(0.25);
        double expected_ny = 0.5 + std::sin(0.25);
        assert(std::fabs(nx - expected_nx) < 1e-12);
        assert(std::fabs(ny - expected_ny) < 1e-12);
    }

    // Test Dream: a=0.3, b=0.5, c=0.7, d=0.9, (0.2, -0.1)
    {
        auto [nx, ny] = stepAttractor(4, 0.3, 0.5, 0.7, 0.9, 0.2, -0.1);
        // xp=0.2, y=-0.1
        // nx = sin(-0.1*0.5) + 0.7*sin(0.2*0.5) = sin(-0.05) + 0.7*sin(0.1)
        // ny = sin(0.2*0.3) + 0.9*sin(-0.1*0.3) = sin(0.06) + 0.9*sin(-0.03)
        double expected_nx = std::sin(-0.05) + 0.7*std::sin(0.1);
        double expected_ny = std::sin(0.06) + 0.9*std::sin(-0.03);
        assert(std::fabs(nx - expected_nx) < 1e-12);
        assert(std::fabs(ny - expected_ny) < 1e-12);
    }

    // Test HopalongA: a=1.0, b=2.0, c=3.0, x=4.0, y=5.0 (xp=4 >=0 so sign +)
    {
        auto [nx, ny] = stepAttractor(5, 1.0, 2.0, 3.0, 0.0, 4.0, 5.0);
        // nx = 5 + sqrt(|2*4 - 3|) = 5 + sqrt(5)
        // ny = 1 - 4 = -3
        assert(std::fabs(nx - (5.0 + std::sqrt(5.0))) < 1e-12);
        assert(std::fabs(ny - (-3.0)) < 1e-12);
    }

    // Test HopalongB: a=1.0, b=2.0, c=3.0, x=0.0, y=0.0 (xp=0 >=0 so sign +)
    {
        auto [nx, ny] = stepAttractor(6, 1.0, 2.0, 3.0, 0.0, 0.0, 0.0);
        // nx = 0 - 1 + sqrt(|2*0 - 1 - 3|) = -1 + sqrt(4) = -1 + 2 = 1
        // ny = 1 - 0 - 1 = 0
        assert(std::fabs(nx - 1.0) < 1e-12);
        assert(std::fabs(ny - 0.0) < 1e-12);
    }

    // Test Gumowski_Mira: a=0.5, b=0.2, c=0.8, x=1.0, y=0.5
    {
        auto [nx, ny] = stepAttractor(7, 0.5, 0.2, 0.8, 0.0, 1.0, 0.5);
        // xp=1.0, y=0.5
        // G(1.0, 0.8) = 0.8*1 + 2*(1-0.8)*(1)/(1+1) = 0.8 + 2*0.2*0.5 = 0.8 + 0.2 = 1.0
        // nx = 0.5 + 0.5*(1 - 0.2*0.25)*0.5 + 1.0 = 0.5 + 0.5*(1 - 0.05)*0.5 + 1.0 = 0.5 + 0.5*0.95*0.5 + 1.0 = 0.5 + 0.2375 + 1.0 = 1.7375
        // ny = -1 + G(1.7375, 0.8) = -1 + [0.8*1.7375 + 2*0.2*(1.7375^2)/(1+1.7375^2)]
        //      = -1 + [1.39 + 0.4*(3.0189)/(1+3.0189)=1.39+0.4*3.0189/4.0189≈1.39+0.3004=1.6904] ≈ -1+1.6904=0.6904
        double G_xp = G(1.0, 0.8); // =1.0
        double expected_nx = 0.5 + 0.5 * (1.0 - 0.2 * 0.5 * 0.5) * 0.5 + G_xp;
        double G_nx = G(expected_nx, 0.8);
        double expected_ny = -1.0 + G_nx;
        assert(std::fabs(nx - expected_nx) < 1e-12);
        assert(std::fabs(ny - expected_ny) < 1e-12);
    }

    // Test that invalid attractor returns (0,0)
    {
        auto [nx, ny] = stepAttractor(99, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
        assert(nx == 0.0);
        assert(ny == 0.0);
    }

    return 0;
}
