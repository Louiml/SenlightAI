// Write a standalone C++ function `simulateCoolingModel` that simulates both a linear and a nonlinear thermal model over a given number of steps. The linear model updates temperature as `temp = A * temp + B * heat` each step, while the nonlinear model updates as `temp = A * temp - B * prev_temp^2 + C * heat + D * sin(heat)`, where `prev_temp` is the temperature from the previous step (for the first step, use `prev_temp = 0`). The function should take all coefficients (`A`, `B`, `C`, `D`) as doubles, initial temperature, heat input, number of steps as an integer, and a boolean flag `isNonlinear` indicating which model to use. It should return a `std::map<int, double>` mapping step numbers (starting at 1) to the computed temperature at that step. All inputs are guaranteed valid (steps ≥ 1, coefficients finite), and no input validation is needed.

#include <cassert>
#include <cmath>
#include <map>

// Declaration of the solution function (from above)
std::map<int, double> simulateCoolingModel(
    double A, double B, double C, double D,
    double initialTemp, double heat, int steps, bool isNonlinear);

int main() {
    // Linear model with A=1, B=0 means temp stays constant.
    auto lin1 = simulateCoolingModel(1.0, 0.0, 0.0, 0.0, 10.0, 0.0, 3, false);
    assert(lin1.size() == 3);
    assert(lin1[1] == 10.0 && lin1[2] == 10.0 && lin1[3] == 10.0);

    // Linear model with A=0, B=2, heat=1 gives 2,2,2 for steps>=1.
    auto lin2 = simulateCoolingModel(0.0, 2.0, 0.0, 0.0, 100.0, 1.0, 2, false);
    assert(lin2[1] == 2.0 && lin2[2] == 2.0);

    // Nonlinear model with A=1, B=0, C=1, D=0 → temp increases by heat each step.
    auto non1 = simulateCoolingModel(1.0, 0.0, 1.0, 0.0, 0.0, 5.0, 4, true);
    assert(non1[1] == 5.0 && non1[2] == 10.0 && non1[3] == 15.0 && non1[4] == 20.0);

    // Nonlinear model with A=0, B=0, C=0, D=2, heat=π/2 → sin=1, temp becomes 2 each step.
    auto non2 = simulateCoolingModel(0.0, 0.0, 0.0, 2.0, 10.0, M_PI / 2.0, 3, true);
    for (int i = 1; i <= 3; ++i) {
        assert(std::abs(non2[i] - 2.0) < 1e-9);
    }

    // Nonlinear with A=1, B=1, C=0, D=0, initial=1: temp = 1 - prev^2.
    auto non3 = simulateCoolingModel(1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 2, true);
    assert(std::abs(non3[1] - 1.0) < 1e-9);
    assert(std::abs(non3[2] - (1.0 - 1.0)) < 1e-9); // prev was 1 → new=0

    // One step only works for both models.
    auto oneStepLin = simulateCoolingModel(2.0, 3.0, 0.0, 0.0, 4.0, 1.0, 1, false);
    assert(oneStepLin[1] == 2.0 * 4.0 + 3.0 * 1.0);

    auto oneStepNon = simulateCoolingModel(1.0, 1.0, 0.0, 0.0, 5.0, 0.0, 1, true);
    assert(std::abs(oneStepNon[1] - 5.0) < 1e-9); // prev=0, so new=1*5 - 1*0 =5

    // Larger steps, verify linear recurrence with A=0.5, B=2, heat=3: temp=0.5*temp+6
    auto lin3 = simulateCoolingModel(0.5, 2.0, 0.0, 0.0, 0.0, 3.0, 3, false);
    assert(std::abs(lin3[1] - 6.0) < 1e-9);
    assert(std::abs(lin3[2] - (0.5*6.0 + 6.0)) < 1e-9);
    assert(std::abs(lin3[3] - (0.5*lin3[2] + 6.0)) < 1e-9);

    return 0;
}

#include <map>
#include <cmath>

// Simulate a linear or nonlinear thermal model over `steps` steps.
// Returns a map from step number (1..steps) to temperature at that step.
// If isNonlinear is false, uses temp = A*temp + B*heat.
// If isNonlinear is true, uses temp = A*temp - B*prev^2 + C*heat + D*sin(heat).
std::map<int, double> simulateCoolingModel(
    double A, double B, double C, double D,
    double initialTemp, double heat, int steps, bool isNonlinear) {
    std::map<int, double> temps;
    double temp = initialTemp;
    double prev_temp = 0.0;

    for (int i = 1; i <= steps; ++i) {
        if (isNonlinear) {
            double new_temp = A * temp - B * prev_temp * prev_temp + C * heat + D * std::sin(heat);
            prev_temp = temp;
            temp = new_temp;
        } else {
            temp = A * temp + B * heat;
        }
        temps[i] = temp;
    }
    return temps;
}

// The solution iterates from step 1 to the given number of steps, applying the appropriate recurrence. For the linear model, we simply compute `temp = A * temp + B * heat` and store the result. For the nonlinear model, we maintain `prev_temp` initialized to 0. At each step, compute `new_temp = A * temp - B * pow(prev_temp, 2) + C * heat + D * sin(heat)`, then set `prev_temp = temp` and `temp = new_temp` before storing. The use of `std::map` ensures ordered access by step number, and doubles provide sufficient precision. Edge cases: steps ≥ 1, so the map is never empty; no division or other risky operations occur; `sin` and `pow` are available via `<cmath>`. Time complexity is O(steps) because each step does constant work, and space complexity is O(steps) due to storing results in the map. No other auxiliary data structures are needed besides a few local variables.
