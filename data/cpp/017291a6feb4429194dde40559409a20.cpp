/*
Write a C++ function that simulates a simplified neuron weight-training process. The function should take three parameters: an initial weight `s1` (double, 0.0–1.0), an initial weight `s2` (double, 0.0–1.0), a number of repetitions `repeticao` (positive integer), and a learning rate `taxa_a` (double, suggested between 0 and 1). The function must perform a training loop exactly `repeticao` times, where on each iteration `i` (starting from 1), it computes `soma_pesos = (s1 * i) + (s2 * i)` and `soma_numeros = i + i`, then updates both weights by adding `((soma_numeros - soma_pesos) * taxa_a * i)` to each. After training, the function must return a vector of doubles containing the final trained weights in order `{s1, s2}`. The function should be pure (no I/O, no random numbers), and must handle edge cases like zero repetitions (return the initial weights unchanged) and negative learning rates (still perform the arithmetic update as given, but the result will naturally be adjusted). The function should not modify the input parameters directly; it should work on local copies.
*/
#include <vector>

// Trains two neuron weights according to the given update rule.
// Returns a vector containing the final weights {s1, s2} after `repeticao` iterations.
std::vector<double> trainWeights(double s1, double s2, int repeticao, double taxa_a) {
    // Work on local copies to avoid modifying caller's variables.
    double w1 = s1;
    double w2 = s2;

    for (int i = 1; i <= repeticao; ++i) {
        // Compute the current weighted sum and the target sum.
        double soma_pesos = (w1 * i) + (w2 * i);
        double soma_numeros = i + i;

        // Compute the update delta once, using the value of soma_pesos before any weight update.
        double delta = (soma_numeros - soma_pesos) * taxa_a * i;

        // Apply the same delta to both weights.
        w1 += delta;
        w2 += delta;
    }

    return {w1, w2};
}
#include <cassert>
#include <cmath>
#include <vector>

// Forward declaration of the solution function.
std::vector<double> trainWeights(double s1, double s2, int repeticao, double taxa_a);

int main() {
    // Case 1: Zero repetitions returns the original weights.
    auto result0 = trainWeights(0.5, 0.8, 0, 0.1);
    assert(result0.size() == 2);
    assert(std::abs(result0[0] - 0.5) < 1e-9);
    assert(std::abs(result0[1] - 0.8) < 1e-9);

    // Case 2: One repetition with simple values.
    // Initial: s1=0.2, s2=0.3, taxa=0.5, i=1.
    // soma_pesos = 0.2*1 + 0.3*1 = 0.5
    // soma_numeros = 2
    // delta = (2 - 0.5) * 0.5 * 1 = 0.75
    // s1 = 0.2+0.75 = 0.95, s2 = 0.3+0.75 = 1.05
    auto result1 = trainWeights(0.2, 0.3, 1, 0.5);
    assert(std::abs(result1[0] - 0.95) < 1e-9);
    assert(std::abs(result1[1] - 1.05) < 1e-9);

    // Case 3: Two repetitions with identical initial weights and a small rate.
    // i=1: soma_pesos=0.4, soma_numeros=2, delta=(2-0.4)*0.1*1=0.16
    // w1=w2=0.2+0.16=0.36
    // i=2: soma_pesos=(0.36+0.36)*2=1.44, soma_numeros=4, delta=(4-1.44)*0.1*2=0.512
    // w1=w2=0.36+0.512=0.872
    auto result2 = trainWeights(0.2, 0.2, 2, 0.1);
    assert(std::abs(result2[0] - 0.872) < 1e-9);
    assert(std::abs(result2[1] - 0.872) < 1e-9);

    // Case 4: Negative learning rate still updates but decreases weights.
    // Initial 1.0, 1.0, one iteration, taxa=-0.5.
    // soma_pesos=2.0, soma_numeros=2, delta=(2-2)*(-0.5)*1=0 → unchanged.
    auto result3 = trainWeights(1.0, 1.0, 1, -0.5);
    assert(std::abs(result3[0] - 1.0) < 1e-9);
    assert(std::abs(result3[1] - 1.0) < 1e-9);

    // Case 5: Larger repetition count, ensure loop runs correctly.
    // Use initial 0,0, repeticao=3, taxa=1.0.
    // i=1: soma_pesos=0, soma_numeros=2, delta=2*1*1=2 → w1=w2=2
    // i=2: soma_pesos=(2+2)*2=8, soma_numeros=4, delta=(4-8)*1*2=-8 → w1=w2=2-8=-6
    // i=3: soma_pesos=(-6-6)*3=-36, soma_numeros=6, delta=(6-(-36))*1*3=126 → w1=w2=-6+126=120
    auto result4 = trainWeights(0.0, 0.0, 3, 1.0);
    assert(std::abs(result4[0] - 120.0) < 1e-9);
    assert(std::abs(result4[1] - 120.0) < 1e-9);

    return 0;
}
// The solution is straightforward: copy the input weights into local variables, then iterate from `i = 1` to `repeticao` inclusive, applying the update formula exactly as specified. Each iteration recomputes `soma_pesos` and `soma_numeros` from the current local weights and the loop index. The update is performed sequentially on `s1` and `s2`; note that when updating `s1`, the old `soma_pesos` is used (not a recomputed value after updating `s1`), because the formula uses `soma_pesos` computed before the dual assignment. In the original snippet, both updates use the same `soma_pesos` value, which is based on the previous iteration's weights. So we must compute `delta = (soma_numeros - soma_pesos) * taxa_a * i` once, then add it to both `s1` and `s2`. This is important to match expected behavior. Edge cases: `repeticao <= 0` means the loop never runs, so the function returns the initial weights unchanged. For very large repetition counts, the loop runs in linear time, and no extra space beyond a few doubles is used, so time complexity is O(repeticao) and space complexity O(1). The function must be `const`-correct: parameters are passed by value for copying, and we do not modify external state. We also need to include `<vector>` for the return type.
