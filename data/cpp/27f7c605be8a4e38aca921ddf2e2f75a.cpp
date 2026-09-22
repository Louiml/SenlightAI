// Write a standalone C++ function named `basinStabilityGyration` that simulates a one-dimensional coupled oscillator network and computes the fraction of random initial perturbations of a single node (with all other nodes at a fixed baseline) that lead to a synchronized final state. The function must accept, in order: the number of nodes `n`, a coupling strength `coupling`, a baseline initial value `initialValue`, a maximum perturbation magnitude `perturbRange` (perturbation drawn uniformly from 0 to this positive value), an integration time step `dt`, a transient duration `transients`, and an integer `noic` (number of independent trials). The internal dynamics must be governed by the following update rule for each node `i` at each time step:  
// `x_i(t+dt) = x_i(t) + dt * ( -x_i(t)^3 + x_i(t) + (coupling / n) * sum_j (x_j(t) - x_i(t)) )`, with no self-interaction term (sum over all j, including j=i, yields zero net contribution from self because `x_i - x_i = 0`). After evolving for `transients` seconds, the system is considered synchronized if all node states have the same sign (i.e., for every node `k`, `x_k * x_0 >= 0`). Return a `double` between 0 and 1 representing the fraction of trials that synchronize. The function must be deterministic given the same seeds? No—use a local `std::mt19937` seeded with a fixed constant (e.g., `12345`) to ensure reproducibility across runs. Ensure the function handles `n >= 1` and `noic >= 1`. Provide only the function definition, no `main`.

The algorithm initializes a vector `x` of size `n` with `initialValue` for every node. For each of `noic` independent trials, it selects a uniformly random perturbation node index (between `0` and `n-1` inclusive) and then sets that node’s initial value to a uniform random number in `[0, perturbRange]` using a separate uniform distribution for the amount. After this initialization, we run Euler integration for `t = 0; t < transients; t += dt` (using a `while` loop to avoid floating‑point accumulation errors). At each step, compute the current average of all nodes, then for each node `i`, update `x[i] += dt * (-x[i]*x[i]*x[i] + x[i] + coupling * (mean - x[i]))` (since `sum_j (x_j - x_i)/n = mean - x_i`). After the loop, check synchronization: if any node has a different sign from node 0 (i.e., product `< 0`), the trial fails. After `noic` trials, compute the fraction of successes. Edge cases: `n=1` always synchronizes (no other node to conflict), `perturbRange=0` still works (uniform dist with zero range returns 0), and `noic=1` returns 0 or 1. Floating‑point comparisons for sign use strict `< 0` for the product, which handles zero safely (product zero means okay). Time complexity is `O(noic * (transients/dt) * n)` per call; space complexity is `O(n)` for the state vector. The use of a fixed seed makes the function deterministic, which aids testing.

#include <vector>
#include <random>
#include <cmath>

// Computes basin stability for a coupled oscillator network with single-node perturbations.
double basinStabilityGyration(int n, double coupling, double initialValue, double perturbRange,
                              double dt, double transients, int noic) {
    // Local random generators with fixed seed for reproducibility
    std::mt19937 gen(12345);
    std::uniform_int_distribution<int> nodeDist(0, n - 1);
    std::uniform_real_distribution<double> perturbDist(0.0, perturbRange);

    int syncCount = 0;

    for (int trial = 0; trial < noic; ++trial) {
        // Initialize all nodes to baseline
        std::vector<double> x(n, initialValue);

        // Perturb one random node with a random value in [0, perturbRange]
        int perturbedNode = nodeDist(gen);
        x[perturbedNode] = perturbDist(gen);

        // Euler integration for transient duration
        double t = 0.0;
        while (t < transients) {
            // Compute current mean of all nodes
            double sum = 0.0;
            for (int i = 0; i < n; ++i) {
                sum += x[i];
            }
            double mean = sum / n;

            // Update each node using the given ODE
            std::vector<double> xNew(n);
            for (int i = 0; i < n; ++i) {
                double x_i = x[i];
                double dx = -x_i * x_i * x_i + x_i + coupling * (mean - x_i);
                xNew[i] = x_i + dt * dx;
            }
            x = xNew;
            t += dt;
        }

        // Check synchronization: all nodes must have the same sign as node 0
        bool isSync = true;
        for (int i = 1; i < n; ++i) {
            if (x[i] * x[0] < 0.0) {
                isSync = false;
                break;
            }
        }
        if (isSync) {
            ++syncCount;
        }
    }

    return static_cast<double>(syncCount) / noic;
}

#include <cassert>
#include <cmath>

// Declaration of the solution function (already defined above)
double basinStabilityGyration(int n, double coupling, double initialValue, double perturbRange,
                              double dt, double transients, int noic);

int main() {
    // Single node always synchronizes
    assert(std::abs(basinStabilityGyration(1, 1.0, 1.0, 1.0, 0.01, 1.0, 10) - 1.0) < 1e-9);

    // Zero coupling, simple case: positive baseline and positive perturbation -> both positive, synchronizes
    // With no coupling, nodes evolve independently; baseline positive, perturbation non-negative, all positive
    assert(std::abs(basinStabilityGyration(5, 0.0, 1.0, 0.5, 0.01, 0.1, 10) - 1.0) < 1e-9);

    // Strong negative coupling may cause anti-synchronization (different signs)
    // For n=2, coupling = -10, baseline=1, perturbation small: often ends with opposite signs
    double bsStrongNeg = basinStabilityGyration(2, -10.0, 1.0, 0.1, 0.01, 0.5, 20);
    assert(bsStrongNeg >= 0.0 && bsStrongNeg <= 1.0);
    // We expect it to be less than 1 (some trials fail) – just check it's not 1
    assert(bsStrongNeg < 1.0);

    // Random stability with large noic gives valid range
    double bsAny = basinStabilityGyration(3, 0.5, 1.0, 0.2, 0.01, 1.0, 50);
    assert(bsAny >= 0.0 && bsAny <= 1.0);

    // Deterministic with same parameters and seed
    double bs1 = basinStabilityGyration(4, 2.0, 0.5, 0.3, 0.01, 0.2, 30);
    double bs2 = basinStabilityGyration(4, 2.0, 0.5, 0.3, 0.01, 0.2, 30);
    assert(std::abs(bs1 - bs2) < 1e-9);

    // Edge: perturbRange = 0 gives deterministic outcome (perturbation exactly 0)
    // With baseline 1 and no coupling, all nodes stay positive, so observed 1
    assert(std::abs(basinStabilityGyration(3, 0.0, 1.0, 0.0, 0.01, 0.1, 5) - 1.0) < 1e-9);

    return 0;
}
