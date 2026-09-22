// Write a C++ function named `angleZoneDescription` that takes an integer angle in degrees (assumed to be in the range 0 to 359 inclusive, but your function should still handle values outside this range gracefully). The function should return a `std::string` describing the angle's position: `"X"` if the angle lies exactly on the positive X-axis (0 or 180 degrees, but for 180 we return the negative X-axis, so both are "X"), `"Y"` if it lies exactly on the positive or negative Y-axis (90 or 270 degrees), or `"Quadrant 1"`, `"Quadrant 2"`, `"Quadrant 3"`, or `"Quadrant 4"` depending on which quadrant the angle falls into (first quadrant: 0 < angle < 90; second: 90 < angle < 180; third: 180 < angle < 270; fourth: 270 < angle < 360). For input values outside [0, 360), the function should return `"Out of range"`. The original snippet printed Cyrillic labels and only handled 0 to 359; your function should be clear, English-label based, and robust.
The solution uses a series of comparisons to map an integer angle to its corresponding zone. Start by checking if the angle is outside the valid range [0, 360); if so, return `"Out of range"`. Then check the exact axis cases: 0 or 180 → `"X"`; 90 or 270 → `"Y"`. For the quadrants, use strict inequalities: 0 < angle < 90 → Quadrant 1; 90 < angle < 180 → Quadrant 2; 180 < angle < 270 → Quadrant 3; 270 < angle < 360 → Quadrant 4. Edge cases include exactly 0, 90, 180, 270, 360 (which is out of range per original spec but we treat as out of range since 360 is not < 360). The algorithm runs in O(1) time and uses O(1) auxiliary space, as it performs constant number of comparisons and returns a string literal (no dynamic allocation).
#include <string>

// Return a string describing the zone of a given angle in degrees.
// Valid angles: 0 to 359 inclusive. Returns "Out of range" otherwise.
std::string angleZoneDescription(int angle) {
    if (angle < 0 || angle >= 360) {
        return "Out of range";
    }
    if (angle == 0 || angle == 180) {
        return "X";
    }
    if (angle == 90 || angle == 270) {
        return "Y";
    }
    if (angle > 0 && angle < 90) {
        return "Quadrant 1";
    }
    if (angle > 90 && angle < 180) {
        return "Quadrant 2";
    }
    if (angle > 180 && angle < 270) {
        return "Quadrant 3";
    }
    // angle > 270 && angle < 360
    return "Quadrant 4";
}
#include <cassert>
#include <string>

// Declaration of the function being tested (assumed defined elsewhere or above).
std::string angleZoneDescription(int angle);

int main() {
    // Axis cases
    assert(angleZoneDescription(0) == "X");
    assert(angleZoneDescription(90) == "Y");
    assert(angleZoneDescription(180) == "X");
    assert(angleZoneDescription(270) == "Y");

    // Quadrant cases
    assert(angleZoneDescription(1) == "Quadrant 1");
    assert(angleZoneDescription(89) == "Quadrant 1");
    assert(angleZoneDescription(91) == "Quadrant 2");
    assert(angleZoneDescription(179) == "Quadrant 2");
    assert(angleZoneDescription(181) == "Quadrant 3");
    assert(angleZoneDescription(269) == "Quadrant 3");
    assert(angleZoneDescription(271) == "Quadrant 4");
    assert(angleZoneDescription(359) == "Quadrant 4");

    // Out of range
    assert(angleZoneDescription(-1) == "Out of range");
    assert(angleZoneDescription(360) == "Out of range");
    assert(angleZoneDescription(1000) == "Out of range");

    return 0;
}
