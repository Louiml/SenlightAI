// Write a C++ function `kalmanFilterStep` that performs one Kalman filter prediction and update step for a 2-dimensional state vector. The function takes the current state `x` (a `float[2]`), covariance `P` (a `float[4]` representing a symmetric 2x2 matrix in row-major order), control input `u`, measurement `z`, system matrices `A` (4-element `float[4]`), `B` (2-element `float[2]`), `C` (2-element `float[2]`), process noise covariance `Q` (4-element `float[4]`), and measurement noise variance `R` (a single `float`). It must update `x` and `P` in-place to the posterior values after processing the measurement. The matrices are assumed to be valid for a linear time-invariant model `x_{k+1} = A*x_k + B*u`, `z = C*x_k + noise`. The function must handle potential division by zero in the Kalman gain calculation by guarding against `S <= 0` (in practice, `S` will be positive since `R>0`, but the guard ensures robustness). No dynamic memory allocation is allowed; use only stack allocation and simple loops or direct formulas. The solution must be self-contained with necessary headers and comments.
The algorithm follows the standard discrete Kalman filter equations for a linear time-invariant system. In the prediction step, the state is propagated using `x_pred = A * x + B * u`, and the covariance is updated via `P_pred = A * P * A^T + Q`. In the update step, the innovation covariance is computed as `S = C * P_pred * C^T + R`, the Kalman gain as `L = P_pred * C^T / S` (where division is scalar since `S` is a scalar), and the state and covariance are corrected: `x = x_pred + L * (z - C * x_pred)` and `P = (I - L * C) * P_pred`. The implementation uses direct 2x2 matrix operations with explicit formulas to avoid loops and keep the code clear. Edge cases: `S` must be positive; if it is zero or negative, the function should leave state and covariance unchanged (or set to NaNs in a degenerate way—here we guard by checking `if (S <= 0) return;`). Time complexity is O(1) since the matrices are fixed size; space complexity is O(1) using only scalar temporaries.
#include <cstddef>

// Perform one Kalman filter step (predict + update) for a 2-state system.
// Updates x[0], x[1] and P[0..3] in-place.
// A[4]: 2x2 state transition matrix (row-major).
// B[2]: control input matrix (2x1).
// C[2]: measurement matrix (1x2).
// Q[4]: 2x2 process noise covariance (row-major, symmetric).
// R: measurement noise variance (scalar).
void kalmanFilterStep(float x[2], float P[4],
                      const float A[4], const float B[2], const float C[2],
                      const float Q[4], float R,
                      float u, float z) {
    // Prediction step
    float x1_pred = A[0] * x[0] + A[1] * x[1] + B[0] * u;
    float x2_pred = A[2] * x[0] + A[3] * x[1] + B[1] * u;

    // P_pred = A * P * A^T + Q
    // Compute A*P first (2x2)
    float ap00 = A[0] * P[0] + A[1] * P[2];
    float ap01 = A[0] * P[1] + A[1] * P[3];
    float ap10 = A[2] * P[0] + A[3] * P[2];
    float ap11 = A[2] * P[1] + A[3] * P[3];
    // Then (A*P)*A^T + Q
    float p11_pred = ap00 * A[0] + ap01 * A[1] + Q[0];
    float p12_pred = ap00 * A[2] + ap01 * A[3] + Q[1];
    float p21_pred = ap10 * A[0] + ap11 * A[1] + Q[2];
    float p22_pred = ap10 * A[2] + ap11 * A[3] + Q[3];

    // Innovation covariance S = C * P_pred * C^T + R
    float CP0 = C[0] * p11_pred + C[1] * p21_pred;
    float CP1 = C[0] * p12_pred + C[1] * p22_pred;
    float S = C[0] * CP0 + C[1] * CP1 + R;

    // Guard against non-positive S (should not happen if R>0)
    if (S <= 0.0f) {
        return; // No update possible; leave state and covariance unchanged.
    }

    // Kalman gain L = P_pred * C^T / S
    float invS = 1.0f / S;
    float L1 = (p11_pred * C[0] + p12_pred * C[1]) * invS;
    float L2 = (p21_pred * C[0] + p22_pred * C[1]) * invS;

    // Innovation (measurement residual)
    float innovation = z - (C[0] * x1_pred + C[1] * x2_pred);

    // Measurement update for state
    x[0] = x1_pred + L1 * innovation;
    x[1] = x2_pred + L2 * innovation;

    // Covariance update: P = (I - L*C) * P_pred
    // I - L*C is a 2x2 matrix, multiply by P_pred
    float ILC00 = 1.0f - L1 * C[0];
    float ILC01 = -L1 * C[1];
    float ILC10 = -L2 * C[0];
    float ILC11 = 1.0f - L2 * C[1];

    P[0] = ILC00 * p11_pred + ILC01 * p21_pred;
    P[1] = ILC00 * p12_pred + ILC01 * p22_pred;
    P[2] = ILC10 * p11_pred + ILC11 * p21_pred;
    P[3] = ILC10 * p12_pred + ILC11 * p22_pred;
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Simple static model with zero process noise and zero control
    // Model: x_{k+1} = x_k, measurement = x[0] (i.e., A=I, B=0, C=[1,0])
    float A[4] = {1,0, 0,1};
    float B[2] = {0,0};
    float C[2] = {1,0};
    float Q[4] = {0,0, 0,0};
    float R = 1.0f;
    float x[2] = {0.0f, 0.0f};
    float P[4] = {1,0, 0,1};
    // One step with measurement z=2.0
    kalmanFilterStep(x, P, A, B, C, Q, R, 0.0f, 2.0f);
    // Expected: gain L1 = P_pred[0]*C[0]/(C^2*P_pred[0]+R) = 1/(1+1)=0.5
    // x[0] = 0 + 0.5*(2) = 1.0, x[1] = 0
    assert(fabs(x[0] - 1.0f) < 1e-5);
    assert(fabs(x[1]) < 1e-5);
    assert(fabs(P[0] - 0.5f) < 1e-5); // P = (1-0.5)*1 = 0.5

    // Test 2: Prediction with control input and measurement update
    // Model: x1' = x1 + x2 + u, x2' = x2, measure x1
    A[0]=1; A[1]=1; A[2]=0; A[3]=1;
    B[0]=0.5f; B[1]=0.0f;
    C[0]=1.0f; C[1]=0.0f;
    float Q2[4] = {0.1f,0, 0,0.1f};
    R = 0.5f;
    float x2[2] = {1.0f, 1.0f};
    float P2[4] = {1,0, 0,1};
    // Predict with u=2, then update with z=3
    kalmanFilterStep(x2, P2, A, B, C, Q2, R, 2.0f, 3.0f);
    // Predicted state: x1_pred = 1+1+1=3, x2_pred=1
    // Predicted P11 = A[0]^2*P[0]+... +Q[0] = (1*1 +1*0)+(1*0+1*1)+0.1 = 2.1
    // Innovation S = P11_pred + R = 2.6, gain L1 = 2.1/2.6 ≈ 0.80769
    // x1 = 3 + L1*(3-3) = 3 (since z equals pred), x2 = 1
    assert(fabs(x2[0] - 3.0f) < 1e-4);
    assert(fabs(x2[1] - 1.0f) < 1e-4);
    assert(fabs(P2[0] - (1 - 2.1/2.6)*2.1) < 1e-4);

    // Test 3: No measurement update when S<=0 (degenerate R=0 but S could be 0 if P=0)
    // Set P=0, Q=0, R=0, then S=0 and function returns unchanged
    float A3[4] = {1,0, 0,1};
    float B3[2] = {0,0};
    float C3[2] = {1,0};
    float Q3[4] = {0,0, 0,0};
    float R3 = 0.0f;
    float x3[2] = {5.0f, 5.0f};
    float P3[4] = {0,0, 0,0};
    kalmanFilterStep(x3, P3, A3, B3, C3, Q3, R3, 0.0f, 10.0f);
    // Since S=0, no change
    assert(x3[0] == 5.0f && x3[1] == 5.0f);
    assert(P3[0] == 0.0f && P3[1] == 0.0f && P3[2] == 0.0f && P3[3] == 0.0f);

    // Test 4: Identity model with measurement of both states (C=[1,1])
    // Initial x=[0,0], P=I, R=1, z=10
    float A4[4] = {1,0, 0,1};
    float B4[2] = {0,0};
    float C4[2] = {1,1};
    float Q4[4] = {0,0, 0,0};
    float R4 = 1.0f;
    float x4[2] = {0.0f, 0.0f};
    float P4[4] = {1,0, 0,1};
    kalmanFilterStep(x4, P4, A4, B4, C4, Q4, R4, 0.0f, 10.0f);
    // S = C*P*C^T + R = (1+1)*1*1 +1 = 3? Actually P=I, C*P*C^T = 1*1*1 + 1*1*1 =2? Wait: C[0]*P[0]*C[0] + C[0]*P[1]*C[1] + C[1]*P[2]*C[0] + C[1]*P[3]*C[1] = 1*1*1 + 1*0*1 + 1*0*1 + 1*1*1 = 2. So S=3.
    // L = P*C^T / S = [1,1]/3 = [1/3,1/3]
    // innovation = 10 - (1*0+1*0)=10
    // x = [0+10/3, 0+10/3] = [3.333,3.333]
    assert(fabs(x4[0] - (10.0f/3.0f)) < 1e-4);
    assert(fabs(x4[1] - (10.0f/3.0f)) < 1e-4);
    // P = (I - L*C)*P = [[1-1/3, -1/3],[-1/3,1-1/3]] = [[2/3,-1/3],[-1/3,2/3]]
    assert(fabs(P4[0] - (2.0f/3.0f)) < 1e-4);
    assert(fabs(P4[1] + (1.0f/3.0f)) < 1e-4);
    assert(fabs(P4[2] + (1.0f/3.0f)) < 1e-4);
    assert(fabs(P4[3] - (2.0f/3.0f)) < 1e-4);

    // Test 5: Multiple steps converge to steady state (sanity check)
    float x5[2] = {100.0f, 100.0f};
    float P5[4] = {10,0, 0,10};
    for (int i = 0; i < 1000; ++i) {
        kalmanFilterStep(x5, P5, A, B, C, Q, R, 0.0f, 1.0f);
    }
    // With A=I, Q=0, R=1, C=[1,0], steady-state P[0] should approach 0 (perfect measurement)
    assert(fabs(x5[0] - 1.0f) < 1e-2);
    assert(fabs(P5[0]) < 1e-3);
}
