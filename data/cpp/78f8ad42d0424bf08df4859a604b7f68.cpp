// Implement a C++ function that simulates a discrete-time PID controller with the same algorithmic structure as the provided code snippet. The function should take a `PIDController` struct (defined by you) by reference, along with a setpoint and a measurement (both `float`), and return the updated controller output as a `float`. The `PIDController` struct must contain at least the following members: `Kp`, `Kd`, `T` (sample time), `tau` (filter coefficient), `limMin`, `limMax`, `limMinInt`, `limMaxInt`, `integrator`, `prevError`, `differentiator`, `prevMeasurement`, and `out`. The function must exactly replicate the computations and clamping behavior shown in the snippet: proportional term from current error; integral term using trapezoidal approximation with integrator clamping; derivative term using a low-pass-filtered measurement difference with the exact given formula; output as sum of three terms then output clamping; and finally update `prevError` and `prevMeasurement`. Provide the struct definition and a `PIDController_Init` function that initializes all internal variables to zero (except gains and limits, which must be pre-set by the caller). The solution must be self-contained with necessary headers, and use `const` correctness where appropriate.

// The task requires modeling a discrete PID controller with anti-windup (integrator clamping) and derivative filtering. The main algorithm is event-driven: each call to the update function processes one sample. The key steps are:
// 1. Compute error = setpoint - measurement.
// 2. Compute proportional = Kp * error.
// 3. Update integrator with trapezoidal rule: integrator += 0.5 * Kp * T * (error + prevError), then clamp between limMinInt and limMaxInt.
// 4. Compute differentiator using the provided recursive formula: differentiator = -(2*Kd*(measurement - prevMeasurement) + (2*tau - T)*prevDifferentiator) / (2*tau + T). Note: the original snippet uses `2.0*pid->tau` (double literal) but assigns to float; we should use float literals for consistency.
// 5. Compute output = proportional + integrator + differentiator, clamp between limMin and limMax.
// 6. Update prevError = error and prevMeasurement = measurement.
// Edge cases: the sample time T must be positive; tau must be positive to avoid division by zero; gains and limits are assumed pre-set correctly. The initial state must be initialized (all internal variables zero). The function must handle any float values; clamping is straightforward. The time complexity is O(1) per call, space O(1). The function is deterministic and stateless aside from the passed struct.

#include <algorithm>

// PID Controller structure with all necessary parameters and state variables.
struct PIDController {
    // Tuning parameters (must be set by caller before use)
    float Kp;
    float Kd;
    float T;      // sample time
    float tau;    // low-pass filter coefficient for derivative
    float limMin; // output minimum
    float limMax; // output maximum
    float limMinInt; // integrator minimum
    float limMaxInt; // integrator maximum

    // State variables (initialized by PIDController_Init)
    float integrator;
    float prevError;
    float differentiator;
    float prevMeasurement;
    float out;
};

// Initialize all state variables to zero. Gains and limits are left untouched.
void PIDController_Init(PIDController* pid) {
    pid->integrator = 0.0f;
    pid->prevError = 0.0f;
    pid->differentiator = 0.0f;
    pid->prevMeasurement = 0.0f;
    pid->out = 0.0f;
}

// Update the PID controller with a new setpoint and measurement, returning the output.
float PIDController_Update(PIDController* pid, float setpoint, float measurement) {
    // Error signal
    float error = setpoint - measurement;

    // Proportional term
    float proportional = pid->Kp * error;

    // Integral term with trapezoidal approximation and anti-windup clamping
    pid->integrator += 0.5f * pid->Kp * pid->T * (error + pid->prevError);
    if (pid->integrator > pid->limMaxInt) {
        pid->integrator = pid->limMaxInt;
    } else if (pid->integrator < pid->limMinInt) {
        pid->integrator = pid->limMinInt;
    }

    // Derivative term with low-pass filter (derivative of measurement, not error)
    pid->differentiator = -(2.0f * pid->Kd * (measurement - pid->prevMeasurement)
                            + (2.0f * pid->tau - pid->T) * pid->differentiator)
                           / (2.0f * pid->tau + pid->T);

    // Compute controller output
    pid->out = proportional + pid->integrator + pid->differentiator;

    // Clamp output
    if (pid->out > pid->limMax) {
        pid->out = pid->limMax;
    } else if (pid->out < pid->limMin) {
        pid->out = pid->limMin;
    }

    // Store error and measurement for next iteration
    pid->prevError = error;
    pid->prevMeasurement = measurement;

    return pid->out;
}

#include <cassert>
#include <cmath>

int main() {
    // Test 1: Initial state produces zero output with zero setpoint and measurement
    PIDController pid1{1.0f, 0.0f, 0.1f, 1.0f, -10.0f, 10.0f, -5.0f, 5.0f};
    PIDController_Init(&pid1);
    float out1 = PIDController_Update(&pid1, 0.0f, 0.0f);
    assert(std::fabs(out1) < 1e-6);

    // Test 2: Pure proportional behavior (Kd=0, integrator limited)
    PIDController pid2{2.0f, 0.0f, 0.1f, 1.0f, -100.0f, 100.0f, -50.0f, 50.0f};
    PIDController_Init(&pid2);
    float out2 = PIDController_Update(&pid2, 5.0f, 1.0f); // error=4, prop=8, integral initially 0+0.5*2*0.1*4=0.4, derivative=0
    assert(std::fabs(out2 - 8.4f) < 1e-5);
    // Second call: error=4 again, integral adds 0.5*2*0.1*(4+4)=0.8, total integral=1.2, out=8+1.2=9.2
    float out2b = PIDController_Update(&pid2, 5.0f, 1.0f);
    assert(std::fabs(out2b - 9.2f) < 1e-5);

    // Test 3: Output clamping
    PIDController pid3{10.0f, 0.0f, 0.1f, 1.0f, -1.0f, 1.0f, -0.5f, 0.5f};
    PIDController_Init(&pid3);
    float out3 = PIDController_Update(&pid3, 100.0f, 0.0f); // error=100, prop=1000, integral clamped to 0.5, output clamped to 1.0
    assert(std::fabs(out3 - 1.0f) < 1e-6);

    // Test 4: Derivative action with measurement change
    PIDController pid4{0.0f, 2.0f, 0.1f, 0.5f, -100.0f, 100.0f, -10.0f, 10.0f};
    PIDController_Init(&pid4);
    // First call: prevMeasurement=0, measurement=2, error=0 (setpoint 2), proportional=0, integral=0, differentiator = -(2*2*(2-0) + (2*0.5-0.1)*0)/(2*0.5+0.1) = -(8)/(1.1) ≈ -7.2727
    float out4 = PIDController_Update(&pid4, 2.0f, 2.0f);
    assert(std::fabs(out4 + 7.2727f) < 1e-3);
    // Second call: measurement stays 2, differentiator = -(0 + (1.0-0.1)*prevDiff)/(1.1) = -(0.9*prevDiff)/1.1, decays
    float prevDiff = pid4.differentiator;
    float out4b = PIDController_Update(&pid4, 2.0f, 2.0f);
    assert(std::fabs(out4b - (-( (1.0f-0.1f)*prevDiff)/1.1f )) < 1e-3);
}
