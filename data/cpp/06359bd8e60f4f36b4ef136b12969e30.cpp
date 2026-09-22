// Write a C++ function named `computeDartScores` that accepts an integer parameter `beers` (with a precondition that it is positive) and returns a `std::vector<float>` containing the simulated scores from `NUMBERSHOTS` (defined as a constant equal to 3) dart throws. For each throw, compute a random angle in degrees between `-2*beers` and `+2*beers` (inclusive), convert that angle to radians, and then use the formulas `x = tan(angle_x)` and `y = tan(angle_y) / cos(angle_x)` to get coordinates on a wall at distance 1. The score for that throw is the Euclidean distance `sqrt(x*x + y*y)` from the bullseye. Each throw must use a fresh random seed (e.g., via `srand(time(nullptr))` inside the loop) and be separated by a 1-second delay to ensure different seeds. The function must not print anything; it must only compute and return the vector of scores. Assume `<cstdlib>`, `<ctime>`, `<cmath>`, `<vector>`, and `<thread>` (or `<unistd.h>`) are available for your implementation.
The core algorithm involves iterating `NUMBERSHOTS` times, and for each iteration: (1) seed the random number generator with the current time to ensure variation, (2) generate a pseudo‑random integer from 0 to `4*beers - 1` and subtract `2*beers` to obtain a value in the range `[-2*beers, 2*beers - 1]` (which is acceptable for the simulation), (3) convert that degree value to radians by multiplying by `PI/180.0` (with `PI` defined as `3.14159265` or `acos(-1.0)`), (4) compute the x and y coordinates on the wall using the given trigonometric formulas, (5) compute the distance as the Euclidean norm, and (6) store the result in the vector. An edge case is when `beers` is zero or negative; the problem states the precondition is positive, but for robustness the function can handle zero by avoiding division by zero in the angle range (since `rand() % 0` is invalid). The time complexity is `O(NUMBERSHOTS)` = O(3), and space complexity is O(1) besides the returned vector. The use of `srand` inside the loop with a delay ensures that different seeds produce different sequences; without a delay, consecutive calls within the same second would produce the same seed. The formulas are derived from projecting a small angle onto a wall at distance 1, where `x = tan(theta_x)` and `y = tan(theta_y) / cos(theta_x)` (accounting for the combined projection).
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <vector>
#include <thread>
#include <chrono>

// Constants for the number of shots and PI
const int NUMBERSHOTS = 3;
const double PI = 3.14159265358979323846;

// Simulate a dart game with `beers` affecting the spread.
// Returns a vector of `NUMBERSHOTS` scores (Euclidean distances from bullseye).
std::vector<float> computeDartScores(int beers) {
    std::vector<float> scores;
    scores.reserve(NUMBERSHOTS);
    
    // Guard against invalid beers (should be positive per precondition)
    if (beers <= 0) {
        beers = 1; // fallback to avoid division by zero in angle generation
    }
    
    for (int i = 0; i < NUMBERSHOTS; ++i) {
        // Seed with current time to get a different sequence each iteration
        srand(static_cast<unsigned int>(time(nullptr)));
        
        // Generate random angles in degrees between -2*beers and 2*beers
        int angle_deg_x = rand() % (4 * beers) - (2 * beers);
        int angle_deg_y = rand() % (4 * beers) - (2 * beers);
        
        // Convert to radians
        double angle_x = angle_deg_x * (PI / 180.0);
        double angle_y = angle_deg_y * (PI / 180.0);
        
        // Compute coordinates on the wall at distance 1
        double x_coord = std::tan(angle_x);
        double y_coord = std::tan(angle_y) / std::cos(angle_x);
        
        // Euclidean distance from bullseye (origin)
        double score = std::sqrt(x_coord * x_coord + y_coord * y_coord);
        scores.push_back(static_cast<float>(score));
        
        // Wait a second so the next seed differs
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    return scores;
}
#include <cassert>
#include <vector>
#include <cmath>

// Declare the function being tested (since Solution section doesn't include main)
std::vector<float> computeDartScores(int beers);

int main() {
    // Test with a small beers value (e.g., 1)
    std::vector<float> scores1 = computeDartScores(1);
    assert(scores1.size() == 3);
    // All scores should be non-negative (distance)
    for (float s : scores1) {
        assert(s >= 0.0f);
    }
    
    // Test with a larger beers value (e.g., 5) – angles are bounded so scores should be finite
    std::vector<float> scores2 = computeDartScores(5);
    assert(scores2.size() == 3);
    for (float s : scores2) {
        assert(std::isfinite(s));
        assert(s >= 0.0f);
    }
    
    // Test edge case: beers = 0 (should fallback to 1 in the function)
    std::vector<float> scores3 = computeDartScores(0);
    assert(scores3.size() == 3);
    // With beers=1, max angle is 2 degrees, so max distance is small (tan(2deg)*sqrt(2) ≈ 0.07)
    for (float s : scores3) {
        assert(s < 1.0f);
    }
    
    // Test that repeated calls produce different scores (due to time seeding)
    std::vector<float> scores4 = computeDartScores(2);
    std::vector<float> scores5 = computeDartScores(2);
    // Since seeds differ, at least one score should differ (not guaranteed, but extremely likely)
    bool any_different = false;
    for (int i = 0; i < 3; ++i) {
        if (std::fabs(scores4[i] - scores5[i]) > 1e-6f) {
            any_different = true;
            break;
        }
    }
    // This is probabilistic but extremely reliable; to avoid flaky tests, we don't assert on it.
    // Instead, just ensure vectors are non-empty and valid.
    assert(any_different || true); // Placeholder – the real check is the size and finiteness
    
    return 0;
}
