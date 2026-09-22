// Write a C++ function `simulateWaterCycleStep` that models a simplified one-step water cycle simulation based on the given snippet. The function should accept: a `std::vector<double>` representing soil moisture levels at different depths, a `bool` flag `useTwoPoreModel` (simulating the two-pore domain option), and a `bool` flag `trackWaterAge`. It should perform the following operations in order: (1) simulate canopy fluxes by subtracting 0.1 from each soil moisture value (clamped to minimum 0.0), (2) simulate surface fluxes by subtracting 0.05 from each value (clamped to minimum 0.0), (3) simulate forest growth by increasing each value by 0.02 (clamped to a maximum of 1.0), (4) simulate groundwater routing by adding 0.01 to the last element only, (5) if `useTwoPoreModel` is true, compute the average of all values and set any value lower than that average to 90% of its original value (simulating mobile/immobile water partitioning), (6) if `trackWaterAge` is true and the average soil moisture is below 0.3, then add 0.05 to all values (simulating age increment effect). The function should return a `std::vector<double>` containing the updated soil moisture levels. The input vector must not be empty, and the function should not modify the original input vector. Ensure all clamping operations maintain values in the range [0.0, 1.0].

// The solution iterates through the vector multiple times to apply each simulated process sequentially. First, we make a copy of the input to avoid modifying the original. Each canopy and surface flux step subtracts a fixed amount per element, using `std::max(0.0, value - amount)` to enforce non-negativity. Forest growth adds a fixed amount but uses `std::min(1.0, value + amount)`. Groundwater routing only affects the last element by adding 0.01 and clamping. For the two-pore model, we compute the average of the current values and then for each element if its value is less than the average, multiply it by 0.9 (i.e., reduce by 10%), still respecting bounds. For water age tracking, we first compute the average; if it is below 0.3, we add 0.05 to each element, clamping to max 1.0. Edge cases include an empty input vector (we should return an empty vector or assert), values already at boundaries (clamping ensures validity), and the order of operations matters as specified. Time complexity is O(n) for each pass, with a total of O(n) passes (constant number of passes), so overall O(n) time and O(n) auxiliary space for the copy (or O(1) extra space if we were allowed to modify in-place, but we need a copy per spec).

#include <vector>
#include <algorithm>
#include <cassert>

// Simulates one time step of a simplified water cycle.
// Returns updated soil moisture levels; input vector is not modified.
std::vector<double> simulateWaterCycleStep(
    const std::vector<double>& soilMoisture,
    bool useTwoPoreModel,
    bool trackWaterAge) {
    
    // Handle empty input gracefully
    if (soilMoisture.empty()) {
        return {};
    }
    
    // Make a copy to avoid modifying the caller's data
    std::vector<double> result = soilMoisture;
    
    // 1. Canopy fluxes: subtract 0.1, clamp to minimum 0.0
    for (double& v : result) {
        v = std::max(0.0, v - 0.1);
    }
    
    // 2. Surface fluxes: subtract 0.05, clamp to minimum 0.0
    for (double& v : result) {
        v = std::max(0.0, v - 0.05);
    }
    
    // 3. Forest growth: add 0.02, clamp to maximum 1.0
    for (double& v : result) {
        v = std::min(1.0, v + 0.02);
    }
    
    // 4. Groundwater routing: add 0.01 to last element only
    if (!result.empty()) {
        result.back() = std::min(1.0, result.back() + 0.01);
    }
    
    // 5. Two-pore model: if enabled, reduce values below average by 10%
    if (useTwoPoreModel) {
        double sum = 0.0;
        for (double v : result) {
            sum += v;
        }
        double average = sum / static_cast<double>(result.size());
        for (double& v : result) {
            if (v < average) {
                v = std::max(0.0, v * 0.9);
            }
        }
    }
    
    // 6. Water age tracking: if enabled and average < 0.3, add 0.05
    if (trackWaterAge) {
        double sum = 0.0;
        for (double v : result) {
            sum += v;
        }
        double average = sum / static_cast<double>(result.size());
        if (average < 0.3) {
            for (double& v : result) {
                v = std::min(1.0, v + 0.05);
            }
        }
    }
    
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>

// Include the solution function declaration from the solution file
// (In a real setup, the solution would be in a separate header)
// For this example, we assume the function is defined above.

int main() {
    // Test 1: Basic operation with no flags
    {
        std::vector<double> input = {0.5, 0.6, 0.7};
        auto result = simulateWaterCycleStep(input, false, false);
        // Step1: [0.4, 0.5, 0.6]
        // Step2: [0.35, 0.45, 0.55]
        // Step3: [0.37, 0.47, 0.57]
        // Step4: [0.37, 0.47, 0.58]
        std::vector<double> expected = {0.37, 0.47, 0.58};
        for (size_t i = 0; i < expected.size(); ++i) {
            assert(std::fabs(result[i] - expected[i]) < 1e-9);
        }
        // Original unchanged
        assert(input[0] == 0.5);
    }
    
    // Test 2: Two-pore model enabled
    {
        std::vector<double> input = {0.2, 0.8, 0.5};
        auto result = simulateWaterCycleStep(input, true, false);
        // Step1: [0.1, 0.7, 0.4]
        // Step2: [0.05, 0.65, 0.35]
        // Step3: [0.07, 0.67, 0.37]
        // Step4: [0.07, 0.67, 0.38]
        // avg = (0.07+0.67+0.38)/3 = 0.37333...
        // v<avg: 0.07 -> 0.063, 0.38 -> 0.342; 0.67 not reduced
        // result: [0.063, 0.67, 0.342]
        std::vector<double> expected = {0.063, 0.67, 0.342};
        for (size_t i = 0; i < expected.size(); ++i) {
            assert(std::fabs(result[i] - expected[i]) < 1e-9);
        }
    }
    
    // Test 3: Water age tracking with low average
    {
        std::vector<double> input = {0.1, 0.2, 0.3};
        auto result = simulateWaterCycleStep(input, false, true);
        // Step1: [0.0, 0.1, 0.2]
        // Step2: [0.0, 0.05, 0.15]
        // Step3: [0.02, 0.07, 0.17]
        // Step4: [0.02, 0.07, 0.18]
        // avg = (0.02+0.07+0.18)/3 = 0.09 < 0.3, so add 0.05:
        // [0.07, 0.12, 0.23]
        std::vector<double> expected = {0.07, 0.12, 0.23};
        for (size_t i = 0; i < expected.size(); ++i) {
            assert(std::fabs(result[i] - expected[i]) < 1e-9);
        }
    }
    
    // Test 4: Water age tracking with high average (no change)
    {
        std::vector<double> input = {0.7, 0.8, 0.9};
        auto result = simulateWaterCycleStep(input, false, true);
        // Step1: [0.6, 0.7, 0.8]
        // Step2: [0.55, 0.65, 0.75]
        // Step3: [0.57, 0.67, 0.77]
        // Step4: [0.57, 0.67, 0.78]
        // avg = (0.57+0.67+0.78)/3 = 0.6733 > 0.3, no addition
        std::vector<double> expected = {0.57, 0.67, 0.78};
        for (size_t i = 0; i < expected.size(); ++i) {
            assert(std::fabs(result[i] - expected[i]) < 1e-9);
        }
    }
    
    // Test 5: Clamping to max 1.0
    {
        std::vector<double> input = {0.95, 1.0};
        auto result = simulateWaterCycleStep(input, false, false);
        // Step1: [0.85, 0.9]
        // Step2: [0.8, 0.85]
        // Step3: [0.82, 0.87]
        // Step4: [0.82, 0.88] (last receives +0.01)
        std::vector<double> expected = {0.82, 0.88};
        for (size_t i = 0; i < expected.size(); ++i) {
            assert(std::fabs(result[i] - expected[i]) < 1e-9);
        }
    }
    
    // Test 6: Empty input
    {
        std::vector<double> input;
        auto result = simulateWaterCycleStep(input, false, false);
        assert(result.empty());
    }
    
    // Test 7: Both flags together
    {
        std::vector<double> input = {0.1, 0.4, 0.2};
        auto result = simulateWaterCycleStep(input, true, true);
        // Step1: [0.0, 0.3, 0.1]
        // Step2: [0.0, 0.25, 0.05]
        // Step3: [0.02, 0.27, 0.07]
        // Step4: [0.02, 0.27, 0.08]
        // avg = (0.02+0.27+0.08)/3 = 0.12333
        // Two-pore: v<avg: 0.02->0.018, 0.08->0.072; 0.27 unchanged
        // result now: [0.018, 0.27, 0.072]
        // avg = (0.018+0.27+0.072)/3 = 0.12 < 0.3, add 0.05:
        // [0.068, 0.32, 0.122]
        std::vector<double> expected = {0.068, 0.32, 0.122};
        for (size_t i = 0; i < expected.size(); ++i) {
            assert(std::fabs(result[i] - expected[i]) < 1e-9);
        }
    }
    
    return 0;
}
