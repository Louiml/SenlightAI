Implement a C++ function `simulateBallEmitter` that models the behavior of a Chipmunk2D-style ball emitter with blocking and catch sensors. The function should take parameters: `totalBalls` (number of balls to emit), `canEmit` (bool indicating whether the emitter is currently unblocked), and `dt` (simulation time step). It must simulate a queue of balls: balls are released one per time step only if `canEmit` is true; otherwise they accumulate in the queue. Additionally, whenever the emitter becomes unblocked, released balls travel downward under constant gravity (use a simple algebraic fall rather than full physics), and each ball that falls below a threshold line (i.e., `y < 0`) is caught by a sensor and adds one ball back to the queue (i.e., the catch sensor "recycles" the ball). The simulation continues until no balls remain in either the queue or active falling balls. Return the total number of time steps taken to clear all balls, assuming infinite time steps are allowed. The function must work for positive integers and any `dt > 0`, handle `canEmit` being initially true or false, and must not use any external physics library—only standard C++.
The core algorithm simulates discrete time steps. At each step, two things happen:  
(1) If the queue is non-empty and `canEmit` is true, remove one ball from the queue and start its fall from `y = 150` with initial vertical velocity `0`. The ball accelerates downward with `gravity = -100` (in units per second squared). To determine when the ball crosses `y = 0`, use the kinematic equation: `y(t) = 150 + v0*t + 0.5*g*t^2`. With `v0 = 0` and `g = -100`, the ball reaches `y = 0` at `t = sqrt(2 * 150 / 100) = sqrt(3) ≈ 1.732` seconds. The number of time steps needed for the ball to be caught is `ceil(sqrt(3) / dt)`. However, because the ball is only caught after falling below `y = 0` during a step, we need to track when the ball’s `y` becomes `< 0`. A simple approach: for each active ball, maintain the time since it was emitted. At each step, increment its age by `dt`. When `age * age * 50 >= 150` (since `y = 150 - 50 * age^2`), the ball is caught, and the queue is incremented by 1. The simulation ends when the queue is empty and no active balls remain. If `canEmit` is initially false, the queue accumulates until `canEmit` becomes true—but since the parameter `canEmit` is a bool passed once, we interpret it as: if false, the emitter is blocked for the entire simulation, so no balls are ever emitted and the queue never drains (infinite steps). To handle that edge case, if `canEmit` is false, the function should return `-1` to indicate an impossible task. If `canEmit` is true, the simulation proceeds. The time complexity is `O(totalBalls * fallDurationSteps)` because each ball requires a fixed number of steps to fall (independent of other balls), and the number of steps is proportional to `1/dt`. Space complexity is `O(totalBalls)` if we store active balls; but since the fall duration is fixed, we can instead track a count of active balls and decrement the queue when a ball is caught—no need to store per-ball state if we use a circular counter based on the fixed fall time. More precisely, we can maintain `activeFalls` queue of timestamps, but simpler: since all balls follow identical motion, we can count how many balls are "in flight" by tracking the number of balls emitted and the number caught. Each ball takes exactly `K = ceil(sqrt(3) / dt)` steps to fall. So after a ball is emitted, it is caught exactly `K` steps later. Thus we can keep a FIFO of emission times (or just a counter and an index offset). The simplest is to store the times when each ball was emitted in a queue; at each step, check the front. But that violates O(1) space if we don't store? Actually storing timestamps is fine. Edge cases: `dt` very large (e.g., `dt=10`) means a ball is caught in 1 step; `dt` very small means more steps. The function must handle `totalBalls=0` (return 0). The return type is `int`. The reference solution uses a queue of ints storing step numbers when each ball was emitted, then at each step, pop front if the ball has been in flight for `K` or more steps. The loop increments `steps` until queue and active count are both zero.
#include <queue>
#include <cmath>

// Simulate a ball emitter with a queue and catch sensor.
// Returns the number of time steps to clear all balls, or -1 if blocked forever.
int simulateBallEmitter(int totalBalls, bool canEmit, double dt) {
    if (totalBalls < 0 || dt <= 0.0) return -1;
    if (!canEmit) return -1; // blocked forever, cannot emit
    if (totalBalls == 0) return 0;

    // Ball falls from y=150 with v0=0, g=-100.
    // y(t) = 150 - 50*t^2. When y < 0, t > sqrt(3).
    // Number of steps needed: ceil(sqrt(3)/dt)
    const double fallRatio = std::sqrt(3.0) / dt;
    int fallSteps = static_cast<int>(std::ceil(fallRatio));

    std::queue<int> emissionTimes; // stores step index when a ball was emitted
    int queueSize = totalBalls;
    int steps = 0;

    // Emit one ball per step when queue is non-empty and emitter is unblocked.
    while (queueSize > 0 || !emissionTimes.empty()) {
        // Emit if possible
        if (queueSize > 0 && canEmit) {
            --queueSize;
            emissionTimes.push(steps);
        }

        // Advance time by one step
        ++steps;

        // Catch balls that have been in flight for at least fallSteps steps
        while (!emissionTimes.empty() && steps - emissionTimes.front() >= fallSteps) {
            emissionTimes.pop();
            ++queueSize; // recycle the ball back to queue
        }
    }

    return steps;
}
#include <cassert>

int main() {
    // Basic cases
    assert(simulateBallEmitter(0, true, 1.0) == 0);
    assert(simulateBallEmitter(1, true, 1.0) == 2); // emit at step0, fall for sqrt(3)~1.732 => needs 2 steps, caught after step2
    assert(simulateBallEmitter(1, false, 1.0) == -1);
    assert(simulateBallEmitter(1, true, 10.0) == 1); // dt large -> fall in 1 step

    // Larger totals: compute manually with dt=1
    // Each ball takes ceil(sqrt(3))=2 steps to fall after emission
    // For N balls, each emitted 1 step apart, the last ball is emitted at step N-1,
    // caught at step (N-1)+2 = N+1, total steps = N+1
    assert(simulateBallEmitter(5, true, 1.0) == 6);
    assert(simulateBallEmitter(3, true, 1.0) == 4);

    // dt = 0.5 => fall takes ceil(1.732/0.5)=4 steps
    assert(simulateBallEmitter(1, true, 0.5) == 4);
    assert(simulateBallEmitter(2, true, 0.5) == 5); // emit at 0, catch at 4; emit at 1, catch at 5

    // Invalid inputs
    assert(simulateBallEmitter(-1, true, 1.0) == -1);
    assert(simulateBallEmitter(1, true, 0.0) == -1);

    // Edge: many balls with very small dt
    assert(simulateBallEmitter(10, true, 0.1) > 0); // just ensure it runs
}
