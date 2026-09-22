Write a C++ function `double arrivalTime(int n, double a, double d, const std::vector<std::pair<int, double>>& participants)` that simulates a single-lane race where each participant starts at a fixed time `t` and accelerates from rest at constant acceleration `a` until reaching a maximum speed `v` (which may be lower than the speed needed to cover the full distance `d` at constant acceleration). The participants start in the order given and cannot overtake; if a participant would arrive earlier than the previous participant, they must queue behind and arrive at the previous participant's time. The function returns the arrival time of the last participant (the maximum over all participants of their individual arrival time, after applying the no-overtaking rule). The input `n` is the number of participants, `a` is the positive acceleration, `d` is the positive distance, and each pair in `participants` is `{start_time, max_speed}`. The distance `d` and acceleration `a` are given in consistent units; speeds are positive. The function should compute each participant's ideal arrival time as `t + time_to_reach_max_speed` if the distance to accelerate to max speed is less than or equal to `d`, otherwise `t + sqrt(2*d/a)`. Then sequentially enforce that each arrival time is at least the previous one. Return the final arrival time (the last participant's actual arrival). Use `double` precision and assume the input is valid.

// For each participant, we need to compute the minimal time to cover distance `d` under constant acceleration `a` from rest, but capped by a maximum speed `v`. First, compute the time to reach max speed: `t_v = v / a`. The distance covered during acceleration is `d_v = 0.5 * a * t_v^2`. If `d_v >= d`, then the participant never reaches max speed and the time is `sqrt(2*d/a)`. Otherwise, the remaining distance `d - d_v` is covered at constant speed `v`, so additional time is `(d - d_v)/v`, giving total time `t_v + (d - d_v)/v`. Add the start time `t` to get the ideal arrival time. Then we iterate through participants in order, maintaining the actual arrival time of the previous participant. For each, set `current_arrival = max(ideal_arrival, previous_arrival)`. After processing all, return the last `current_arrival`. Edge cases: if `d` is exactly the distance to accelerate to max speed, the formula gives `t_v + 0`, which is correct. If `v` is very large, the max-speed branch may not trigger; if `v` is very small, the acceleration distance may be negligible. Time complexity O(n) because we process each participant once; space complexity O(1) beyond input storage.

#include <vector>
#include <utility>
#include <cmath>
#include <algorithm>

// Compute the arrival time of the last participant in a non-overtaking race.
// participants: vector of {start_time, max_speed}
// a: constant acceleration (positive), d: race distance (positive)
// Returns the arrival time of the last participant as a double.
double arrivalTime(int n, double a, double d, const std::vector<std::pair<int, double>>& participants) {
    double lastArrival = 0.0;
    double accelTimeToMax = 0.0;
    double accelDistance = 0.0;
    double idealArrival = 0.0;

    for (int i = 0; i < n; ++i) {
        double startTime = static_cast<double>(participants[i].first);
        double maxSpeed = participants[i].second;

        // Time to reach max speed and distance covered during acceleration
        accelTimeToMax = maxSpeed / a;
        accelDistance = 0.5 * a * accelTimeToMax * accelTimeToMax;

        if (accelDistance >= d) {
            // Never reaches max speed; time to cover d under constant acceleration
            idealArrival = startTime + std::sqrt(2.0 * d / a);
        } else {
            // Accelerates to max speed, then cruises at constant speed
            double cruiseDistance = d - accelDistance;
            double cruiseTime = cruiseDistance / maxSpeed;
            idealArrival = startTime + accelTimeToMax + cruiseTime;
        }

        // Enforce no overtaking: actual arrival is at least the previous arrival
        lastArrival = std::max(idealArrival, lastArrival);
    }

    return lastArrival;
}

#include <cassert>
#include <vector>
#include <utility>
#include <cmath>

// The solution function is declared above. This test main verifies correctness.

int main() {
    // Case 1: Single participant that reaches max speed before finish
    // a=2, d=10, v=4. t_v=2, d_v=4, cruise=6 at v=4 => 1.5, total ideal=3.5, start=0 => 3.5
    {
        std::vector<std::pair<int, double>> participants = {{0, 4.0}};
        assert(std::fabs(arrivalTime(1, 2.0, 10.0, participants) - 3.5) < 1e-9);
    }

    // Case 2: Single participant that does not reach max speed
    // a=2, d=100, v=100 (very high), sqrt(2*100/2)=10, start=0 => 10
    {
        std::vector<std::pair<int, double>> participants = {{0, 100.0}};
        assert(std::fabs(arrivalTime(1, 2.0, 100.0, participants) - 10.0) < 1e-9);
    }

    // Case 3: Two participants with same start time; second is slower so queues
    // First: a=2, d=10, v=4 => ideal 3.5. Second: a=2, d=10, v=2 => t_v=1, d_v=1, cruise=9/2=4.5, total=5.5, start=0 => 5.5. Since 5.5>3.5, arrival=5.5
    {
        std::vector<std::pair<int, double>> participants = {{0, 4.0}, {0, 2.0}};
        assert(std::fabs(arrivalTime(2, 2.0, 10.0, participants) - 5.5) < 1e-9);
    }

    // Case 4: Second participant starts later but is faster; no overtaking, so they arrive at their own time
    // First: start=0, a=2, d=10, v=4 => 3.5. Second: start=10, a=2, d=10, v=4 => ideal=13.5, previous=3.5, so 13.5
    {
        std::vector<std::pair<int, double>> participants = {{0, 4.0}, {10, 4.0}};
        assert(std::fabs(arrivalTime(2, 2.0, 10.0, participants) - 13.5) < 1e-9);
    }

    // Case 5: Multiple participants, some overlapping times, check monotonic non-decreasing
    // Run a simple scenario: three participants
    // p1: start=0, v=4 => 3.5
    // p2: start=1, v=1 => t_v=0.5, d_v=0.25, cruise=9.75/1=9.75 => total=10.25, ideal=11.25, max(11.25,3.5)=11.25
    // p3: start=2, v=8 => t_v=4, d_v=16>10 => sqrt(10)=3.1623, ideal=5.1623, max(5.1623,11.25)=11.25
    // result 11.25
    {
        std::vector<std::pair<int, double>> participants = {{0, 4.0}, {1, 1.0}, {2, 8.0}};
        double expected = 11.25;
        double result = arrivalTime(3, 2.0, 10.0, participants);
        assert(std::fabs(result - expected) < 1e-6);
    }

    // Case 6: Two participants with same start and same speed => same ideal, arrival equals ideal
    {
        std::vector<std::pair<int, double>> participants = {{5, 3.0}, {5, 3.0}};
        // a=1, d=8, v=3: t_v=3, d_v=4.5, cruise=3.5/3=1.1666667, total=4.1666667, ideal=9.1666667
        double expected = 9.166666666666666;
        double result = arrivalTime(2, 1.0, 8.0, participants);
        assert(std::fabs(result - expected) < 1e-6);
    }

    return 0;
}
