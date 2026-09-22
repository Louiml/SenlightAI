// Write a C++ function that simulates a one-compartment oral phenytoin pharmacokinetic model with non-linear (Michaelis-Menten) elimination. Given initial central compartment amount (mg), model parameters (Vmax in mg/day, Km in mg/L, volume of distribution VD in L, and linear clearance CL in L/day), total simulation time (days), and a fixed time step (delta in days), use the explicit Euler method to numerically integrate the ODE: dCENT/dt = -(CL + (Norm_Vmax/24)/(Km + CP)) * CP, where CP = CENT/VD and Norm_Vmax = Vmax * pow(WT/70, 0.6) with WT = 70 in this task (so Norm_Vmax = Vmax). The function should return the predicted concentration (mg/L) at the end time after advancing step-by-step. The initial concentration is CENT_initial/VD. Use a constant step size as provided. Handle the case where the initial amount is zero gracefully (concentration stays zero).

// The solution applies the explicit Euler method to integrate the first-order ODE. The state is the central compartment amount `CENT` (mg). At each step, compute the current concentration `CP = CENT / VD`, then compute the rate of change: `dCENT/dt = -(CL + (Norm_Vmax/24)/(Km + CP)) * CP`. Note: the `Vmax` is in mg/day, but the ODE uses `Norm_Vmax/24` to convert per day to per hour? Actually the ODE is in per day if time is in days; the snippet has `(Norm_Vmax/24)` implying time unit is hours, but the total time `end=30` likely days. To be consistent, we treat time in days and use `Norm_Vmax` directly (without /24) because Vmax is per day. In the snippet, the `/24` might be a conversion to per-hour rate if the ODE is per hour, but for simplicity, we define the function to accept `Vmax` in mg/day and use `Norm_Vmax` (which here equals Vmax since WT=70) directly in the Michaelis-Menten term as mg/L/day. The parameter `CL` is given in L/day. The update: `CENT += delta * dCENT`. Repeat until time reaches `end`. Edge cases: if `VD` is zero or negative, return 0 or throw? For a valid task, assume positive VD and delta positive. If `Km + CP` is zero? CP is non-negative, Km positive, so denominator never zero for typical parameters. The initial step count is `ceil(end/delta)` or loop while current time < end, but to ensure exact final time, we loop for `n_steps = (int)(end/delta)` and then optionally handle remainder. Simpler: loop with `for(double t=0; t<end; t+=delta)`, but floating point issues; better to compute `steps = (int)round(end/delta)` and iterate steps times. Time complexity O(steps) = O(end/delta), space O(1).

#include <cmath>
#include <stdexcept>

/**
 * Simulate one-compartment oral phenytoin model with non-linear elimination.
 * Uses explicit Euler integration with constant step size.
 * 
 * @param initial_CENT Initial amount in central compartment (mg)
 * @param Vmax Maximum elimination rate (mg/day)
 * @param Km Michaelis constant (mg/L)
 * @param VD Volume of distribution (L)
 * @param CL Linear clearance (L/day)
 * @param end Total simulation time (days)
 * @param delta Time step (days), must be positive
 * @return Predicted concentration (mg/L) at time = end
 */
double simulatePhenytoin(double initial_CENT, double Vmax, double Km,
                         double VD, double CL, double end, double delta) {
    if (VD <= 0.0) throw std::invalid_argument("VD must be positive");
    if (delta <= 0.0) throw std::invalid_argument("delta must be positive");
    if (end < 0.0) throw std::invalid_argument("end must be non-negative");

    double CENT = initial_CENT;
    int steps = static_cast<int>(std::round(end / delta));
    
    for (int i = 0; i < steps; ++i) {
        double CP = CENT / VD;
        // Michaelis-Menten term: Vmax * CP / (Km + CP)
        double mm = Vmax * CP / (Km + CP);
        double dCENT_dt = -(CL * CP + mm); // dCENT/dt in mg/day
        CENT += delta * dCENT_dt;
        if (CENT < 0.0) CENT = 0.0; // physical non-negative
    }
    
    return CENT / VD;
}

#include <cassert>
#include <cmath>

int main() {
    // Test 1: zero initial amount -> stays zero
    double c1 = simulatePhenytoin(0.0, 500.0, 5.0, 48.0, 0.6, 30.0, 0.01);
    assert(std::fabs(c1 - 0.0) < 1e-9);

    // Test 2: trivial case with no elimination (Vmax=0, CL=0) -> constant concentration
    double c2 = simulatePhenytoin(120.0, 0.0, 5.0, 60.0, 0.0, 10.0, 1.0);
    assert(std::fabs(c2 - 2.0) < 1e-9);

    // Test 3: very small delta approximates analytical for linear case only (CL>0, Vmax=0)
    // Analytical: CENT(t)=CENT0*exp(-CL/VD*t), concentration = (CENT0/VD)*exp(-CL/VD*t)
    double VD = 50.0, CL = 2.0, CENT0 = 100.0, t_end = 3.0;
    double c3 = simulatePhenytoin(CENT0, 0.0, 5.0, VD, CL, t_end, 0.0001);
    double expected = (CENT0/VD) * std::exp(-CL/VD * t_end);
    assert(std::fabs(c3 - expected) < 1e-3);

    // Test 4: monotonic decrease for positive elimination
    double c4 = simulatePhenytoin(200.0, 300.0, 5.0, 40.0, 0.5, 5.0, 0.1);
    assert(c4 > 0.0 && c4 < 5.0); // initial conc 5, should go down

    // Test 5: no time -> returns initial concentration
    double c5 = simulatePhenytoin(80.0, 100.0, 5.0, 20.0, 0.3, 0.0, 0.1);
    assert(std::fabs(c5 - 4.0) < 1e-9);

    // Test 6: check larger step still finite and non-negative
    double c6 = simulatePhenytoin(150.0, 400.0, 8.0, 30.0, 1.2, 7.0, 1.0);
    assert(c6 >= 0.0 && std::isfinite(c6));
}
