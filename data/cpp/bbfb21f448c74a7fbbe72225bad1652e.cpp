// Write a C++ function named `computePiSections` that takes an integer `numSteps` (assumed to be positive and divisible by 10) and returns the approximate value of π using a fixed-section parallel-style summation. The computation must divide the total number of steps into exactly 10 equal sections. For each section, compute the partial sum using the midpoint rectangle rule: for each step index `i` in that section, set `x = (i + 0.5) * step` (where `step = 1.0 / numSteps`) and accumulate `4.0 / (1.0 + x * x)`. Each section’s partial sum is multiplied by `step` and stored. Finally, sum the 10 section results to produce the approximation of π. The function must use a plain loop-based simulation of OpenMP-style partitioning (no actual OpenMP required) — i.e., iterate over the sections sequentially, but within each section, correctly calculate the starting and ending indices based on `sectionIndex * (numSteps / 10)` and `(sectionIndex + 1) * (numSteps / 10)`. The function must not print anything; it only returns the computed π value. Edge cases: if `numSteps` is not positive or not divisible by 10, return 0.0.

#include <cassert>
#include <cmath>

// Forward declaration of the function under test.
double computePiSections(int numSteps);

int main() {
    // For 100000 steps, the result should be very close to true pi.
    double pi_100k = computePiSections(100000);
    assert(std::fabs(pi_100k - 3.141592653589793) < 1e-5);

    // For 1000 steps, still close but less precise.
    double pi_1k = computePiSections(1000);
    assert(std::fabs(pi_1k - 3.141592653589793) < 1e-3);

    // For 10 steps, the approximation is rough but should be around 3.14ish.
    double pi_10 = computePiSections(10);
    assert(std::fabs(pi_10 - 3.0) < 0.5);

    // Edge cases: invalid inputs return 0.
    assert(computePiSections(0) == 0.0);
    assert(computePiSections(-100) == 0.0);
    assert(computePiSections(15) == 0.0);  // not divisible by 10

    // All sections are summed exactly 10 times.
    // For numSteps = 20, each section has 2 steps; verify no off-by-one.
    double pi_20 = computePiSections(20);
    assert(std::fabs(pi_20 - 3.0916) < 0.01); // approximate known value
    
    return 0;
}

#include <cstddef>

// Return approximation of pi using 10 equal sections of midpoint rectangle sum.
// If numSteps is not positive or not divisible by 10, return 0.0.
double computePiSections(int numSteps) {
    if (numSteps <= 0 || numSteps % 10 != 0) {
        return 0.0;
    }

    const double step = 1.0 / static_cast<double>(numSteps);
    const int stepsPerSection = numSteps / 10;
    const int numSections = 10;

    double pi = 0.0;

    for (int section = 0; section < numSections; ++section) {
        int start = section * stepsPerSection;
        int end = start + stepsPerSection;
        double localSum = 0.0;

        for (int i = start; i < end; ++i) {
            double x = (static_cast<double>(i) + 0.5) * step;
            localSum += 4.0 / (1.0 + x * x);
        }

        pi += step * localSum;
    }

    return pi;
}

// The approach replicates the logic from the given snippet but without OpenMP parallelism, since the task asks for a standalone function. We first validate the input: `numSteps` must be positive and divisible by 10; otherwise, return 0.0. The step size is `1.0 / numSteps`. The number of sections is 10, and each section covers `stepsPerSection = numSteps / 10` consecutive indices. For each section `s` from 0 to 9, initialize `localSum = 0.0`, then loop over indices `i` from `s * stepsPerSection` to `(s + 1) * stepsPerSection - 1`. For each index, compute `x = (i + 0.5) * step`, add `4.0 / (1.0 + x * x)` to `localSum`. After the inner loop, multiply `localSum` by `step` and add to the total `pi`. The time complexity is O(numSteps) because we iterate over all steps exactly once. Space complexity is O(1) auxiliary, since we only need a few doubles and integers (no arrays needed because we can sum directly). Numerically, the result approximates π to about `double` precision; the error is about `O(1/numSteps)` for the midpoint rule. The algorithm correctly handles large `numSteps` as long as overflow does not occur in the integer loops (safe for typical values). Edge case: if `numSteps` is 0 or negative, return 0.0; if not divisible by 10, return 0.0 per spec.
