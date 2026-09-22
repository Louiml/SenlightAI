// Write a standalone C++ function that computes the pressure-related contribution to a first-order force accumulation in a soft-tissue simulation. Specifically, given a 3×3 stress tensor `P` (stored as a flat array of 9 floats), a 3D direction vector `q`, and a positive scalar `cell_volume`, compute a corrected stress tensor `P_corrected` where the isotropic pressure contribution is added as `P_corrected = P + pressure_term * I`, with `pressure_term = dot(q, q) / cell_volume`. The function must also compute a scalar `energy` equal to `0.5 * pressure_term * cell_volume`. The function should take const references for inputs and return the corrected tensor by modifying an output array, and return the energy by reference. Handle the edge case where `cell_volume` is zero by setting `pressure_term` to zero (no pressure contribution) and `energy` to zero without division by zero.

// The task reduces to a simple linear algebra operation with a division. The main algorithm:
// 1. Compute the dot product of `q` with itself: `q_dot_q = q[0]*q[0] + q[1]*q[1] + q[2]*q[2]`.
// 2. If `cell_volume` is not zero (use a small epsilon, e.g., `1e-12`, to handle floating-point near-zero), compute `pressure_term = q_dot_q / cell_volume`. Otherwise set `pressure_term = 0`.
// 3. Build the corrected tensor by copying the input `P` into `P_corrected`, then adding `pressure_term` to the diagonal entries at indices 0, 4, 8 (since the tensor is stored row-major: P[0] is row0col0, P[4] is row1col1, P[8] is row2col2).
// 4. Compute `energy = 0.5 * pressure_term * cell_volume`.
// 5. Write `P_corrected` into the output array (must be size 9) and `energy` into the reference.
// Edge cases: zero or negative cell volume – treat as no correction. Also, if input tensor contains NaN/Inf, propagate naturally. Complexity is O(1) time and O(1) space (only a few local variables).

#include <array>
#include <cmath>

/**
 * Computes pressure-corrected stress tensor and elastic energy.
 * 
 * @param P Input stress tensor as flat 9-element array (row-major 3x3).
 * @param q Direction vector (3 elements).
 * @param cell_volume Volume of the element (positive scalar).
 * @param P_corrected Output corrected tensor (must point to 9 floats).
 * @param energy Output elastic energy contribution.
 */
void applyPressureCorrection(
    const float* P,
    const float* q,
    float cell_volume,
    float* P_corrected,
    float& energy
) {
    // Compute dot product q·q
    float q_dot_q = q[0] * q[0] + q[1] * q[1] + q[2] * q[2];
    
    // Compute pressure term, guard against division by zero
    const float eps = 1e-12f;
    float pressure_term = 0.0f;
    if (std::fabs(cell_volume) > eps) {
        pressure_term = q_dot_q / cell_volume;
    }
    
    // Copy original tensor
    for (int i = 0; i < 9; ++i) {
        P_corrected[i] = P[i];
    }
    
    // Add pressure to diagonal entries (indices 0,4,8)
    P_corrected[0] += pressure_term;
    P_corrected[4] += pressure_term;
    P_corrected[8] += pressure_term;
    
    // Compute energy
    energy = 0.5f * pressure_term * cell_volume;
}

#include <cassert>
#include <cmath>

// Declaration of the function under test (assume it's in the same translation unit or included)
void applyPressureCorrection(const float*, const float*, float, float*, float&);

int main() {
    // Test 1: Basic case with positive volume
    {
        float P[9] = {1,2,3, 4,5,6, 7,8,9};
        float q[3] = {1,0,0};
        float vol = 2.0f;
        float P_out[9];
        float energy;
        applyPressureCorrection(P, q, vol, P_out, energy);
        // q·q = 1, pressure_term = 0.5, energy = 0.5*0.5*2 = 0.5
        assert(std::fabs(energy - 0.5f) < 1e-6f);
        assert(P_out[0] == 1.5f);
        assert(P_out[4] == 5.5f);
        assert(P_out[8] == 9.5f);
        assert(P_out[1] == 2.0f);
        assert(P_out[7] == 8.0f);
    }
    
    // Test 2: Zero volume – no correction
    {
        float P[9] = {1,0,0, 0,1,0, 0,0,1};
        float q[3] = {2,3,4}; // q·q = 4+9+16 = 29
        float vol = 0.0f;
        float P_out[9];
        float energy;
        applyPressureCorrection(P, q, vol, P_out, energy);
        assert(energy == 0.0f);
        for (int i = 0; i < 9; ++i) assert(P_out[i] == P[i]);
    }
    
    // Test 3: Negative volume treated as zero
    {
        float P[9] = {0};
        float q[3] = {1,1,1}; // q·q = 3
        float vol = -5.0f;
        float P_out[9];
        float energy;
        applyPressureCorrection(P, q, vol, P_out, energy);
        assert(energy == 0.0f);
        for (int i = 0; i < 9; ++i) assert(P_out[i] == 0.0f);
    }
    
    // Test 4: Large q magnitude
    {
        float P[9] = {0};
        float q[3] = {100,0,0}; // q·q = 10000
        float vol = 1.0f;
        float P_out[9];
        float energy;
        applyPressureCorrection(P, q, vol, P_out, energy);
        assert(std::fabs(energy - 5000.0f) < 1e-3f);
        assert(P_out[0] == 10000.0f);
        assert(P_out[4] == 10000.0f);
        assert(P_out[8] == 10000.0f);
    }
    
    // Test 5: Zero q vector – no change
    {
        float P[9] = {1,2,3, 4,5,6, 7,8,9};
        float q[3] = {0,0,0};
        float vol = 3.0f;
        float P_out[9];
        float energy;
        applyPressureCorrection(P, q, vol, P_out, energy);
        assert(energy == 0.0f);
        for (int i = 0; i < 9; ++i) assert(P_out[i] == P[i]);
    }
    
    // Test 6: Verify that the output tensor is a correct copy plus diagonal increment
    {
        float P[9] = {1,2,3, 4,5,6, 7,8,9};
        float q[3] = {2,3,4}; // q·q = 4+9+16 = 29
        float vol = 4.0f;
        float pressure = 29.0f / 4.0f; // 7.25
        float P_expected[9] = {1+pressure,2,3, 4,5+pressure,6, 7,8,9+pressure};
        float P_out[9];
        float energy;
        applyPressureCorrection(P, q, vol, P_out, energy);
        for (int i = 0; i < 9; ++i) assert(std::fabs(P_out[i] - P_expected[i]) < 1e-6f);
        assert(std::fabs(energy - 0.5f * pressure * vol) < 1e-6f);
    }
    
    return 0;
}
