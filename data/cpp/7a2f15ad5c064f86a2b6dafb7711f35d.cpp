Write a standalone C++ function that computes a 2D Bézier curve trajectory from a set of control points. Given an array of x-coordinates, an array of y-coordinates, the number of control points, a time step `dt`, and a total `duration`, the function must return a struct containing the sampled trajectory points (x, y, and corresponding time values) where each point is evaluated using the Bézier formula: the sum over i from 0 to n of `C(n,i) * P_i * (1-t)^(n-i) * t^i`, with `n = pt_nb - 1`, parameter `t` normalized from 0 to 1 across the duration, and the number of samples equal to `(int)(duration / dt) + 1`. Handle edge cases: if the input arrays are null or the number of control points is less than 1, or if `dt` or `duration` is non-positive, return an empty trajectory (with `nb = 0`). Use integer math for the binomial coefficient (with factorials, assuming small n, e.g., n ≤ 20) and precompute `(1-t)` and `t` powers for efficiency. The function must be self-contained, not depend on ROS or Eigen, and be usable in a plain C++ program.

The core algorithm is a direct evaluation of the Bézier curve formula at discrete parameter values `t_k = k * dt / duration` for `k = 0` to `nb-1`, where `nb = (int)(duration / dt) + 1`. For each control point index `i` (0 to n), compute the binomial coefficient `C(n,i)` using factorial (since n is small, factorial is safe; for n > 20, consider iterative approaches, but the task states n ≤ 20). Then for each sample `k`, compute `power_one_minus = (1-t_k)^(n-i)` and `power_t = t_k^i` using `std::pow`. Accumulate `x[k] += C(n,i) * x_coord[i] * power_one_minus * power_t` and similarly for y. Edge cases: null pointers, pt_nb ≤ 0 or pt_nb < 1 (should require at least 1 control point, but typically n ≥ 1), non-positive dt or duration → return empty. Also, if `duration < dt` then `nb` could be 1 (since integer division gives 0, +1 = 1), but this is acceptable. Time complexity: O(nb * pt_nb) for the nested loops, plus O(nb) for precomputing `t` and `(1-t)` arrays, and O(n) for factorial computations. Space complexity: O(nb) for the returned struct's large fixed arrays (as per struct definition, TIME_POINT_MAX), but the algorithm itself uses O(nb) for the time arrays and O(1) for coefficients.

#include <cmath>
#include <cstddef>

constexpr int TRAJ_TIME_POINT_MAX = 10000; // max number of trajectory points

struct Trajectory {
    int nb = 0;
    float x[TRAJ_TIME_POINT_MAX] = {0};
    float y[TRAJ_TIME_POINT_MAX] = {0};
    float t[TRAJ_TIME_POINT_MAX] = {0};
};

// Compute binomial coefficient C(n, k) using factorial (safe for n <= 20).
int nChooseK(int n, int k) {
    int result = 1;
    for (int i = 1; i <= k; ++i) {
        result = result * (n - k + i) / i;
    }
    return result;
}

// Compute factorial (alternatively, use nChooseK iterative; kept for clarity).
int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

// Compute a 2D Bézier trajectory from control points.
// Returns a Trajectory with nb samples, where x[k], y[k] are the point at time t[k].
Trajectory computeBezierTrajectory(const float* x_coord, const float* y_coord,
                                    int pt_nb, float dt, float duration) {
    Trajectory traj;

    // Edge cases: invalid inputs.
    if (!x_coord || !y_coord || pt_nb <= 0 || dt <= 0.0f || duration <= 0.0f) {
        return traj; // nb = 0, empty trajectory
    }

    const int n = pt_nb - 1;
    if (n < 0) return traj;

    traj.nb = static_cast<int>(duration / dt) + 1;
    if (traj.nb > TRAJ_TIME_POINT_MAX) {
        traj.nb = TRAJ_TIME_POINT_MAX; // cap for safety
    }

    // Precompute normalized time values and (1-t).
    float* time = new float[traj.nb];
    float* one_minus_time = new float[traj.nb];
    for (int k = 0; k < traj.nb; ++k) {
        time[k] = static_cast<float>(k) * dt / duration;
        one_minus_time[k] = 1.0f - time[k];
        traj.t[k] = static_cast<float>(k) * dt;
    }

    // Precompute binomial coefficients for efficiency.
    int* binom = new int[pt_nb];
    for (int i = 0; i < pt_nb; ++i) {
        binom[i] = nChooseK(n, i);
    }

    // Evaluate Bézier sum.
    for (int i = 0; i < pt_nb; ++i) {
        const float coeff = static_cast<float>(binom[i]);
        const float xi = x_coord[i];
        const float yi = y_coord[i];
        for (int k = 0; k < traj.nb; ++k) {
            const float t_pow_i = std::pow(time[k], i);
            const float one_minus_pow = std::pow(one_minus_time[k], n - i);
            traj.x[k] += coeff * xi * one_minus_pow * t_pow_i;
            traj.y[k] += coeff * yi * one_minus_pow * t_pow_i;
        }
    }

    delete[] time;
    delete[] one_minus_time;
    delete[] binom;

    return traj;
}

#include <cassert>
#include <cmath>
#include <iostream>

// (Include the solution code and Trajectory struct here.)

int main() {
    // Test 1: Quadratic Bézier from (0,0) to (1,0) with control (0.5,1).
    // At t=0 (k=0): should be (0,0).
    // At t=1 (k=nb-1): should be (1,0).
    // At t=0.5: should be (0.5, 0.5).
    float x_coord[3] = {0.0f, 0.5f, 1.0f};
    float y_coord[3] = {0.0f, 1.0f, 0.0f};
    float dt = 0.5f;
    float duration = 1.0f; // nb = 3, t = 0, 0.5, 1.0
    Trajectory traj = computeBezierTrajectory(x_coord, y_coord, 3, dt, duration);
    assert(traj.nb == 3);
    assert(std::fabs(traj.x[0] - 0.0f) < 1e-4f);
    assert(std::fabs(traj.y[0] - 0.0f) < 1e-4f);
    assert(std::fabs(traj.x[1] - 0.5f) < 1e-4f);
    assert(std::fabs(traj.y[1] - 0.5f) < 1e-4f);
    assert(std::fabs(traj.x[2] - 1.0f) < 1e-4f);
    assert(std::fabs(traj.y[2] - 0.0f) < 1e-4f);

    // Test 2: Line from (0,0) to (2,2) (linear Bézier, pt_nb=2).
    float x2[2] = {0.0f, 2.0f};
    float y2[2] = {0.0f, 2.0f};
    dt = 0.5f;
    duration = 1.0f; // nb = 3, t=0,0.5,1.0
    Trajectory traj2 = computeBezierTrajectory(x2, y2, 2, dt, duration);
    assert(traj2.nb == 3);
    assert(std::fabs(traj2.x[0] - 0.0f) < 1e-4f);
    assert(std::fabs(traj2.y[1] - 1.0f) < 1e-4f);
    assert(std::fabs(traj2.x[2] - 2.0f) < 1e-4f);

    // Test 3: Null pointer returns empty.
    Trajectory empty = computeBezierTrajectory(nullptr, y2, 2, dt, duration);
    assert(empty.nb == 0);

    // Test 4: Non-positive dt returns empty.
    Trajectory empty2 = computeBezierTrajectory(x2, y2, 2, 0.0f, duration);
    assert(empty2.nb == 0);

    // Test 5: Non-positive duration returns empty.
    Trajectory empty3 = computeBezierTrajectory(x2, y2, 2, dt, -1.0f);
    assert(empty3.nb == 0);

    // Test 6: Single control point (pt_nb=1) gives constant point.
    float x1[1] = {3.0f};
    float y1[1] = {4.0f};
    dt = 1.0f;
    duration = 2.0f; // nb = 3
    Trajectory traj1 = computeBezierTrajectory(x1, y1, 1, dt, duration);
    assert(traj1.nb == 3);
    for (int k = 0; k < traj1.nb; ++k) {
        assert(std::fabs(traj1.x[k] - 3.0f) < 1e-4f);
        assert(std::fabs(traj1.y[k] - 4.0f) < 1e-4f);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
