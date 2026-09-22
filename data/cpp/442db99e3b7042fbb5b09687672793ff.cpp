// Write a self-contained C++ function named `volSwingsTest` that implements the core logic of the `VOL_swing` class (not shown in the snippet) for deciding among green, yellow, and red states in the Volume Algorithm. The function must take four parameters: the current dual objective value `lcost`, the previous dual objective value `last_lcost`, the computed subgradient ascent value `ascent`, and the current iteration number `iter`. Based on these inputs, the function shall return a character: `'G'` for green (meaning the ascent is positive and above a fixed threshold of 0.0001 times the absolute value of `lcost`), `'Y'` for yellow (meaning the ascent is positive but not above the green threshold), and `'R'` for red (meaning the ascent is zero or negative). The red state also triggers a reset behavior—indicated by the function setting a reference boolean parameter `reset_dual` to `true` when red is detected; otherwise `reset_dual` is set to `false`. Edge cases: if `lcost` is zero, use a small positive constant (1e-12) as the denominator for the threshold calculation; if `iter` is 0 or negative, treat it as iteration 1 for state decision purposes.
// The function must evaluate the improvement in the dual objective between consecutive iterations. The ascent value represents the inner product between the new subgradient direction and the change in dual variables; a positive ascent indicates potential improvement. The green state occurs when the ascent is sufficiently large relative to the current objective magnitude, specifically when `ascent > 0.0001 * fabs(lcost)`. Yellow is a weaker positive improvement that doesn't meet the green threshold. Red means no progress (ascent <= 0) and triggers a reset of the dual solution to the best-known dual vector. The reset logic is central to the Volume Algorithm's robustness—it prevents the algorithm from drifting away from good solutions. The threshold uses absolute value to handle negative dual objectives, and the zero-case guard prevents division by zero. Time complexity is O(1), and space usage is O(1), as no additional data structures are needed.
#include <cmath>
#include <cstdlib>

/**
 * Decide the swing state (green, yellow, red) for the Volume Algorithm.
 * 
 * @param lcost       Current dual objective value.
 * @param last_lcost  Previous dual objective value (unused in decision, kept for interface compatibility).
 * @param ascent      Subgradient ascent value (inner product of v and u - last_u).
 * @param iter        Current iteration number (non-positive treated as 1).
 * @param reset_dual  Output reference: set to true if state is red (indicating reset needed), false otherwise.
 * @return            'G' for green, 'Y' for yellow, 'R' for red.
 */
char volSwingsTest(double lcost, double last_lcost, double ascent, int iter, bool& reset_dual) {
    // Normalize iteration (non-positive becomes 1)
    int effective_iter = (iter > 0) ? iter : 1;
    (void)effective_iter; // iter not actually used in decision, but kept for signature completeness

    // Prevent division by zero when lcost is zero
    double abs_lcost = std::fabs(lcost);
    double threshold_base = (abs_lcost < 1e-12) ? 1e-12 : abs_lcost;

    // Compute threshold for green: 0.0001 * |lcost| (or 1e-12 if lcost near zero)
    double green_threshold = 0.0001 * threshold_base;

    if (ascent > green_threshold) {
        reset_dual = false;
        return 'G';
    } else if (ascent > 0.0) {
        reset_dual = false;
        return 'Y';
    } else {
        // ascent <= 0
        reset_dual = true;
        return 'R';
    }
}
#include <cassert>

int main() {
    // Positive improvement exceeding threshold -> Green
    bool reset = false;
    assert(volSwingsTest(100.0, 99.0, 0.02, 1, reset) == 'G');
    assert(reset == false);

    // Positive improvement below threshold -> Yellow
    assert(volSwingsTest(100.0, 99.0, 0.0005, 2, reset) == 'Y');
    assert(reset == false);

    // Zero ascent -> Red, reset requested
    assert(volSwingsTest(100.0, 100.0, 0.0, 3, reset) == 'R');
    assert(reset == true);

    // Negative ascent -> Red, reset requested
    assert(volSwingsTest(50.0, 51.0, -0.01, 4, reset) == 'R');
    assert(reset == true);

    // lcost zero: use small constant threshold, ascent positive but below 1e-12 -> Yellow
    assert(volSwingsTest(0.0, 0.0, 1e-13, 5, reset) == 'Y');
    assert(reset == false);

    // lcost zero, ascent exceeds 1e-12 -> Green
    assert(volSwingsTest(0.0, 0.0, 1e-11, 6, reset) == 'G');
    assert(reset == false);

    // lcost negative, threshold uses absolute value
    assert(volSwingsTest(-200.0, -199.0, 0.03, 7, reset) == 'G');
    assert(reset == false);

    // lcost negative, small but positive ascent -> Yellow
    assert(volSwingsTest(-200.0, -199.0, 0.0001, 8, reset) == 'Y');
    assert(reset == false);

    // Non-positive iteration treated as 1; should still work
    assert(volSwingsTest(10.0, 9.0, 0.005, 0, reset) == 'G');
    assert(reset == false);

    // Exactly at threshold: green requires > threshold, so equal goes to yellow
    assert(volSwingsTest(1000.0, 999.0, 0.1, 10, reset) == 'Y');
    assert(reset == false);
}
