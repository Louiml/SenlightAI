Write a C++ function named `calculateDistanceBetweenCars` that takes three parameters: a float `hours` representing elapsed time in hours, a float `accelerationKmPerSecSq` representing acceleration in kilometers per second squared, and a float `speedKmPerHour` representing the uniform speed of two vehicles in kilometers per hour. The function must return the distance in kilometers between two cars that start 120 km apart, both moving in the same direction (the slower one ahead, the faster one behind) where each vehicle initially travels at the given speed, but the trailing car also accelerates at the given rate. The distance formula is: `distance = 120 + 2 * speedKmPerHour * hours + (2 * accelerationKmPerSecSq * hours^2) / 4`. The inputs are read from standard input in the original program, but the function must accept them directly and return the computed float distance. Assume non‑negative inputs, but the function must handle zero values gracefully (e.g., if acceleration is zero, the formula reduces to `120 + 2 * speed * hours`).
// The solution is straightforward: compute the distance using the given algebraic expression. The formula has three terms: the initial 120 km separation, the distance each car travels due to its uniform speed (since only the trailing car's speed is given, but the formula already accounts for relative motion via `2 * Y * T`), and the acceleration contribution which is halved because the formula as written uses `(2 * A * T^2) / 4` which simplifies to `(A * T^2) / 2`. No special edge cases arise beyond handling zero values, which naturally evaluate correctly. Floating‑point arithmetic is used, so the result is a `float`. Time complexity is O(1) and space complexity is O(1). The function should be `const`‑correct by taking parameters by value (no need to modify them), and the return type is `float`. For testing, direct equality with `==` may be problematic due to floating point rounding; it is safer to use a small epsilon tolerance or use `assert` with a tolerance helper, but the task requires `==` or another suitable comparison. Since the test inputs will be exact decimals (e.g., 1.0, 2.0), the computed results may be exactly representable in binary floating point (e.g., 2.0*1.0*1.0 = 2.0, 2.0*1.0*1.0/4 = 0.5), so `==` can be used if test values are chosen carefully.
// Calculate the distance between two cars given initial separation, speed, and acceleration.
float calculateDistanceBetweenCars(float hours, float accelerationKmPerSecSq, float speedKmPerHour) {
    // Formula: 120 + 2 * speed * hours + (2 * acceleration * hours^2) / 4
    // Simplify: (2 * acceleration * hours^2) / 4 = acceleration * hours^2 / 2
    const float initialDistance = 120.0f;
    const float speedTerm = 2.0f * speedKmPerHour * hours;
    const float accelTerm = accelerationKmPerSecSq * hours * hours / 2.0f;
    return initialDistance + speedTerm + accelTerm;
}
#include <cassert>

int main() {
    // Exact representable values: hours = 1, accel = 1, speed = 1 => 120 + 2*1*1 + (1*1*1)/2 = 122.5
    assert(calculateDistanceBetweenCars(1.0f, 1.0f, 1.0f) == 122.5f);
    // hours = 2, accel = 1, speed = 1 => 120 + 2*1*2 + (1*4)/2 = 124 + 2 = 126.0
    assert(calculateDistanceBetweenCars(2.0f, 1.0f, 1.0f) == 126.0f);
    // hours = 0, accel = 0, speed = 0 => 120
    assert(calculateDistanceBetweenCars(0.0f, 0.0f, 0.0f) == 120.0f);
    // hours = 0.5, accel = 2, speed = 10 => 120 + 2*10*0.5 + (2*0.25)/2 = 120 + 10 + 0.25 = 130.25
    assert(calculateDistanceBetweenCars(0.5f, 2.0f, 10.0f) == 130.25f);
    // hours = 3, accel = 0, speed = 5 => 120 + 2*5*3 + 0 = 150
    assert(calculateDistanceBetweenCars(3.0f, 0.0f, 5.0f) == 150.0f);
    // hours = 1, accel = 4, speed = 0 => 120 + 0 + (4*1)/2 = 122
    assert(calculateDistanceBetweenCars(1.0f, 4.0f, 0.0f) == 122.0f);
    // hours = 4, accel = 1.5, speed = 2 => 120 + 2*2*4 + (1.5*16)/2 = 120 + 16 + 12 = 148
    assert(calculateDistanceBetweenCars(4.0f, 1.5f, 2.0f) == 148.0f);
}
