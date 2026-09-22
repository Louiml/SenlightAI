// Write a C++ function `simulatePendulumWithSensor(int steps, float initialTheta, float period, float noiseVariance)` that simulates a damped-free pendulum whose angular acceleration is `-g * sin(theta)` with `g = 9.81` and `length = 1`, but adds Gaussian measurement noise to the observed angle. The function must return a vector of pairs (time, trueTheta) for `steps` ticks after the initial state, where at each tick the angular velocity `w` is updated using the Euler method with the given `period`, and the true angle is measured with added Gaussian noise of mean 0 and variance `noiseVariance`. The function must generate deterministic reproducible results using a fixed seed (e.g., `std::mt19937` with seed 42). The output vector must contain exactly `steps` pairs, starting from time `period` (first tick) and ending at time `steps * period`. The noise must be added to the true angle before recording. Ignore any influence of noise on the dynamics—only the measurement is noisy. Edge cases: steps must be ≥ 0; if steps == 0, return an empty vector.

The core simulation uses the Euler method for numerical integration of the pendulum ODE: at each step, update angular velocity `w += -period * 9.81 * sin(theta)`, then update angle `theta += period * w`. The true angle at each step is then perturbed by adding a sample from a normal distribution with mean 0 and variance `noiseVariance`. To make results reproducible, use a fixed-seed random engine (e.g., `std::mt19937` seeded with 42) and a `std::normal_distribution<double>` with the given variance. No covariance is needed; we only produce observations. The function must return a `std::vector<std::pair<double,double>>` where the first element is the time at that step (`period * stepIndex`) and the second is the noisy observed angle. Time complexity is O(steps) and space complexity O(steps) for the output vector, with constant extra space for simulation state. Edge cases: if `steps` is 0, return empty; if `noiseVariance` is negative, treat it as 0 (clamp to 0). The initial angular velocity is assumed to be 0.0. The initial angle is given as a parameter. The function must be const-correct and self-contained (include `<vector>`, `<utility>`, `<random>`, `<cmath>`, `<cstddef>`).

#include <vector>
#include <utility>
#include <random>
#include <cmath>
#include <cstddef>

// Simulates a pendulum with noisy angle measurements.
// Returns vector of (time, observedAngle) for each tick.
std::vector<std::pair<double, double>> simulatePendulumWithSensor(
    int steps, double initialTheta, double period, double noiseVariance) {
    if (steps < 0) {
        steps = 0;
    }
    if (noiseVariance < 0.0) {
        noiseVariance = 0.0;
    }

    std::vector<std::pair<double, double>> result;
    result.reserve(static_cast<std::size_t>(steps));

    constexpr double g = 9.81;
    constexpr double length = 1.0;

    double th = initialTheta;
    double w = 0.0;

    // Fixed seed for reproducibility.
    std::mt19937 gen(42);
    std::normal_distribution<double> noise(0.0, std::sqrt(noiseVariance));

    for (int i = 1; i <= steps; ++i) {
        // Euler integration: update w first, then theta.
        w = w - period * (g / length) * std::sin(th);
        th = th + period * w;

        // Add measurement noise to the true angle.
        double observedTheta = th + noise(gen);

        result.emplace_back(period * i, observedTheta);
    }

    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <utility>

int main() {
    // Test 1: Zero steps returns empty vector.
    auto emptyResult = simulatePendulumWithSensor(0, 0.5, 0.1, 0.01);
    assert(emptyResult.empty());

    // Test 2: Negative steps treated as zero.
    auto negativeResult = simulatePendulumWithSensor(-5, 0.5, 0.1, 0.01);
    assert(negativeResult.empty());

    // Test 3: Negative noise variance clamped to zero -> no noise added.
    // With seed fixed, we can compute expected exact Euler result.
    auto noNoise = simulatePendulumWithSensor(3, 0.5, 0.1, -1.0);
    assert(noNoise.size() == 3);
    // First step exactly: w = 0 - 0.1*9.81*sin(0.5) ≈ -0.4704; th = 0.5 + 0.1*(-0.4704) = 0.45296
    assert(std::fabs(noNoise[0].first - 0.1) < 1e-9);
    assert(std::fabs(noNoise[0].second - 0.45296) < 1e-3); // approximate due to sin
    assert(std::fabs(noNoise[1].first - 0.2) < 1e-9);
    assert(std::fabs(noNoise[2].first - 0.3) < 1e-9);

    // Test 4: Deterministic with fixed seed (noise variance > 0).
    auto firstRun = simulatePendulumWithSensor(5, 0.0, 0.05, 0.02);
    auto secondRun = simulatePendulumWithSensor(5, 0.0, 0.05, 0.02);
    assert(firstRun == secondRun);
    assert(firstRun.size() == 5);

    // Test 5: Times are periodically spaced.
    for (std::size_t i = 0; i < firstRun.size(); ++i) {
        assert(std::fabs(firstRun[i].first - 0.05 * (i + 1)) < 1e-9);
    }

    // Test 6: With zero noise variance and zero initial angle, pendulum stays at zero.
    auto zeroState = simulatePendulumWithSensor(10, 0.0, 0.1, 0.0);
    for (const auto& p : zeroState) {
        assert(std::fabs(p.second) < 1e-9);
    }
}
