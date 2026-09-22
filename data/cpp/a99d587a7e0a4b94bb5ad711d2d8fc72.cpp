// Write a C++ function `grade_split_quality` that takes two integer coordinates `(x1, y1)` and `(x2, y2)` representing two points, and two floating-point parameters: `length_weight` (positive) and `sharpness_knob` (positive). The function must compute a combined grade for the split between these points based on two sub-scores: a length penalty and a sharpness penalty. The length penalty is computed as: if the Euclidean distance between the points is ≤ 0, length penalty = 0; otherwise, length penalty = `sqrt(distance) * length_weight`, clipped to a minimum of 0. The sharpness penalty is computed from the sum of the absolute values of the x- and y-coordinates of each point (i.e., Manhattan distance from origin for each point), denoted `sum = |x1|+|y1|+|x2|+|y2|`. If `sum < 360`, sharpness penalty = 0; otherwise, sharpness penalty = `(sum - 360) * sharpness_knob`. The function returns the sum of the two penalties. Assume all inputs are valid and non-negative for the weights. Provide a free function that takes parameters by value and returns a `double`.

The solution computes two independent penalties and sums them. For the length penalty, we first compute the Euclidean distance using `std::hypot(dx, dy)` or `std::sqrt(dx*dx + dy*dy)`. If distance is zero or negative (zero is only possible when both points are identical), we set length penalty to 0. Otherwise, we take the square root of the distance (which is the fourth root of squared distance) and multiply by `length_weight`, then clamp to 0 if it's negative (though with non-negative inputs, it won't be). For the sharpness penalty, we compute the Manhattan distance from origin for each point, sum them, and if the sum is less than 360, the penalty is 0; otherwise, we subtract 360 and multiply by `sharpness_knob`. The result is the sum. Time complexity is O(1), space complexity O(1). Edge cases: identical points (distance = 0, penalty = 0), points near origin (sum < 360 gives sharpness = 0), and very large coordinates (overflow in distance squared? Use `std::hypot` to avoid overflow). The function should use `const` parameters and return a `double`.

#include <cmath>
#include <algorithm>

// Compute combined grade for a split between two points.
// length_weight and sharpness_knob must be non-negative.
// Returns sum of length penalty and sharpness penalty.
double grade_split_quality(double x1, double y1, double x2, double y2,
                           double length_weight, double sharpness_knob) {
    // Length penalty
    double distance = std::hypot(x1 - x2, y1 - y2);
    double length_penalty = 0.0;
    if (distance > 0.0) {
        length_penalty = std::sqrt(distance) * length_weight;
        length_penalty = std::max(0.0, length_penalty);
    }

    // Sharpness penalty
    double sum_manhattan = std::abs(x1) + std::abs(y1) + std::abs(x2) + std::abs(y2);
    double sharpness_penalty = 0.0;
    if (sum_manhattan >= 360.0) {
        sharpness_penalty = (sum_manhattan - 360.0) * sharpness_knob;
    }

    return length_penalty + sharpness_penalty;
}

#include <cassert>
#include <cmath>

int main() {
    // Identical points: distance zero, sharpness sum small
    assert(grade_split_quality(0, 0, 0, 0, 1.0, 1.0) == 0.0);

    // Small distance, small sharpness sum (sum < 360)
    double result1 = grade_split_quality(1, 1, 2, 2, 0.5, 1.0);
    double expected1 = std::sqrt(std::sqrt(2.0)) * 0.5; // distance = sqrt(2), sqrt distance = sqrt(sqrt(2))
    assert(std::abs(result1 - expected1) < 1e-9);

    // Large coordinates: sharpness penalty kicks in
    double result2 = grade_split_quality(200, 200, -200, -200, 0.1, 2.0);
    // sum_manhattan = 200+200+200+200 = 800, sharpness = (800-360)*2 = 880
    // distance = sqrt((400)^2 + (400)^2) = 400*sqrt(2) ≈ 565.685, sqrt(distance) ≈ 23.789, length*0.1=2.3789
    double expected2 = (800 - 360) * 2.0 + std::sqrt(400.0 * std::sqrt(2.0)) * 0.1;
    assert(std::abs(result2 - expected2) < 1e-6);

    // Zero sharpness knob ignores sharpness, only length matters
    double result3 = grade_split_quality(1000, 0, 0, 0, 1.0, 0.0);
    double expected3 = std::sqrt(1000.0) * 1.0; // distance=1000, sqrt=31.622..., length
    assert(std::abs(result3 - expected3) < 1e-9);

    // Zero length weight ignores length, only sharpness matters
    double result4 = grade_split_quality(500, 500, 0, 0, 0.0, 1.0);
    // sum=2000, sharpness=(2000-360)*1=1640
    assert(std::abs(result4 - 1640.0) < 1e-9);

    // Exact boundary sum=360 gives sharpness zero
    double result5 = grade_split_quality(90, 90, 90, 90, 1.0, 1.0); // sum=360
    // length distance=0 => length penalty 0
    assert(result5 == 0.0);

    // Negative coordinates use absolute values
    double result6 = grade_split_quality(-100, -100, 100, 100, 0.1, 0.5);
    // sum = 100+100+100+100=400, sharpness=(400-360)*0.5=20
    // distance = sqrt(200^2+200^2)=200*sqrt(2)≈282.842, sqrt(distance)≈16.817, length=1.6817
    double expected6 = 20.0 + std::sqrt(200.0 * std::sqrt(2.0)) * 0.1;
    assert(std::abs(result6 - expected6) < 1e-6);
}
