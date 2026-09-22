/*
Implement a C++ function `integrateLorenzWithCustomVector` that models a simplified Lorenz system using a custom fixed-capacity vector-like container. The function must accept an initial state as a `std::vector<double>` of exactly 3 elements (x, y, z), integrate the system over time using the classical fourth-order Runge-Kutta method with a fixed step size of 0.1 from t=0 to t=10, and return the final state as a `std::vector<double>`. The integration must be performed using a custom container `MyVector3` (similar to the snippet's `my_vector<3>`) that internally stores data in a `std::vector<double>` but is designed to be compatible with generic numerical algorithms. The function must not use any external ODE library—only the standard template library is allowed for integration and data storage. The Lorenz equations are: dx/dt = 10*(y - x), dy/dt = 28*x - y - x*z, dz/dt = -8/3*z + x*y. Edge cases: the input must always have exactly 3 elements; if not, throw `std::invalid_argument`. The returned vector must have 3 elements containing the final state after 100 integration steps (since t=0 to t=10 with dt=0.1). Time complexity is O(1) since the number of steps is fixed, and space complexity is O(1) for the custom container (which internally uses O(3) storage).
*/
#include <vector>
#include <stdexcept>
#include <cstddef>

// Custom fixed-capacity vector-like container for state (max size 3)
class MyVector3 {
public:
    using iterator = std::vector<double>::iterator;
    using const_iterator = std::vector<double>::const_iterator;

    MyVector3() : m_v() {
        m_v.reserve(3);
    }

    explicit MyVector3(size_t n) : m_v(n) {
        if (n > 3) {
            throw std::invalid_argument("MyVector3 supports at most 3 elements");
        }
        m_v.reserve(3);
    }

    double& operator[](size_t index) { return m_v[index]; }
    const double& operator[](size_t index) const { return m_v[index]; }

    iterator begin() { return m_v.begin(); }
    const_iterator begin() const { return m_v.begin(); }
    iterator end() { return m_v.end(); }
    const_iterator end() const { return m_v.end(); }

    size_t size() const { return m_v.size(); }
    void resize(size_t new_size) {
        if (new_size > 3) {
            throw std::invalid_argument("Cannot resize beyond 3 elements");
        }
        m_v.resize(new_size);
    }

private:
    std::vector<double> m_v;
};

// Lorenz derivative function: returns d(state)/dt
MyVector3 lorenzDerivative(const MyVector3& state) {
    const double sigma = 10.0;
    const double R = 28.0;
    const double b = 8.0 / 3.0;

    MyVector3 result(3);
    result[0] = sigma * (state[1] - state[0]);
    result[1] = R * state[0] - state[1] - state[0] * state[2];
    result[2] = -b * state[2] + state[0] * state[1];
    return result;
}

// Perform one RK4 step
MyVector3 rk4Step(const MyVector3& current, double t, double dt) {
    const MyVector3 k1 = lorenzDerivative(current);

    MyVector3 temp(3);
    for (size_t i = 0; i < 3; ++i) {
        temp[i] = current[i] + 0.5 * dt * k1[i];
    }
    const MyVector3 k2 = lorenzDerivative(temp);

    for (size_t i = 0; i < 3; ++i) {
        temp[i] = current[i] + 0.5 * dt * k2[i];
    }
    const MyVector3 k3 = lorenzDerivative(temp);

    for (size_t i = 0; i < 3; ++i) {
        temp[i] = current[i] + dt * k3[i];
    }
    const MyVector3 k4 = lorenzDerivative(temp);

    MyVector3 next(3);
    for (size_t i = 0; i < 3; ++i) {
        next[i] = current[i] + (dt / 6.0) * (k1[i] + 2.0 * k2[i] + 2.0 * k3[i] + k4[i]);
    }
    return next;
}

// Main function: integrate Lorenz system from t=0 to t=10 with dt=0.1
std::vector<double> integrateLorenzWithCustomVector(const std::vector<double>& initialState) {
    if (initialState.size() != 3) {
        throw std::invalid_argument("Initial state must contain exactly 3 elements");
    }

    MyVector3 state(3);
    for (size_t i = 0; i < 3; ++i) {
        state[i] = initialState[i];
    }

    const double t_start = 0.0;
    const double t_end = 10.0;
    const double dt = 0.1;
    const size_t num_steps = static_cast<size_t>((t_end - t_start) / dt);

    double t = t_start;
    for (size_t step = 0; step < num_steps; ++step) {
        state = rk4Step(state, t, dt);
        t += dt;
    }

    return {state[0], state[1], state[2]};
}
#include <cassert>
#include <cmath>
#include <vector>

// Function declaration from solution
std::vector<double> integrateLorenzWithCustomVector(const std::vector<double>& initialState);

bool closeEnough(double a, double b, double tol = 1e-6) {
    return std::fabs(a - b) < tol;
}

int main() {
    // Test 1: Initial state from snippet (5,10,10) -> run integration, just check size and non-NaN
    std::vector<double> final1 = integrateLorenzWithCustomVector({5.0, 10.0, 10.0});
    assert(final1.size() == 3);
    for (double val : final1) {
        assert(std::isfinite(val));
    }

    // Test 2: All zeros initial state: Lorenz at origin stays at origin
    std::vector<double> final2 = integrateLorenzWithCustomVector({0.0, 0.0, 0.0});
    assert(closeEnough(final2[0], 0.0) && closeEnough(final2[1], 0.0) && closeEnough(final2[2], 0.0));

    // Test 3: Deterministic behavior: same input gives same output
    std::vector<double> final3a = integrateLorenzWithCustomVector({1.0, 2.0, 3.0});
    std::vector<double> final3b = integrateLorenzWithCustomVector({1.0, 2.0, 3.0});
    for (int i = 0; i < 3; ++i) {
        assert(closeEnough(final3a[i], final3b[i]));
    }

    // Test 4: Different initial state yields different result
    std::vector<double> final4 = integrateLorenzWithCustomVector({4.0, 5.0, 6.0});
    bool different = false;
    for (int i = 0; i < 3; ++i) {
        if (!closeEnough(final3a[i], final4[i])) {
            different = true;
            break;
        }
    }
    assert(different);

    // Test 5: Invalid input size throws
    bool threw = false;
    try {
        integrateLorenzWithCustomVector({1.0, 2.0});
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test 6: Extremely large values still produce finite outputs (stability check)
    std::vector<double> final6 = integrateLorenzWithCustomVector({1e6, -1e6, 1e6});
    for (double val : final6) {
        assert(std::isfinite(val));
    }

    return 0;
}
// The solution involves implementing a custom container `MyVector3` that behaves like a resizeable vector of doubles but with a fixed maximum capacity of 3, providing essential member functions: `operator[]`, `begin()`, `end()`, `size()`, and `resize()`. This container will mimic the snippet's `my_vector<3>`. Then, implement the Runge-Kutta 4 (RK4) integration manually: for each of the 100 steps, compute the four slopes k1, k2, k3, k4 using the Lorenz derivative function. To handle the state and derivative, we can use the custom container for the state and temporary derivative calculations. The algorithm: given current state `x`, step size `dt`, and time `t`, compute `k1 = f(t, x)`. Then compute `x2 = x + 0.5*dt*k1`, `k2 = f(t+0.5*dt, x2)`. Similarly `x3 = x + 0.5*dt*k2`, `k3 = f(t+0.5*dt, x3)`. Then `x4 = x + dt*k3`, `k4 = f(t+dt, x4)`. Finally update `x += (dt/6)*(k1 + 2*k2 + 2*k3 + k4)`. The derivative function returns a `MyVector3` containing (10*(y-x), 28*x - y - x*z, -8/3*z + x*y). Edge cases: verify input size exactly 3 at start. Since the integration is fixed, no other edge cases. Time complexity is O(1) with 100 steps, each doing constant work; space complexity is O(1) for local temporaries, but the custom container holds a `std::vector<double>` which allocates 3 doubles on the heap—so practically O(1) but technically O(3) = O(1) as n is fixed.
