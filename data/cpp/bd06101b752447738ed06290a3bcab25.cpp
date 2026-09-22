Write a C++ function that takes an integer representing a direction code (0 for left, 1 for right, 2 for up, 3 for down) and returns a string describing the direction as "Going left", "Going right", "Going up", or "Going down". If the input is not one of these four valid codes, the function should return "Illegal option". The function must handle all possible integer inputs, including negative values and values greater than 3, and must be declared with `const` correctness where appropriate.

// The solution uses a switch statement on the input integer, matching the idea of the original snippet where an enumeration was used. Since the input is a raw integer rather than an enum type, we cannot rely on compiler warnings for missing cases, so we explicitly check all four valid codes (0, 1, 2, 3) in the switch and provide a `default` case for any other value. This guarantees handling of all integers, including negatives and large values. The approach is straightforward: map each valid code directly to its corresponding string literal, and the default case catches invalid inputs. Time complexity is O(1) because only a single switch operation is performed, regardless of the input magnitude. Space complexity is O(1) since we only return a string literal (no dynamic allocation beyond the returned `std::string` copy, which is constant size).

#include <string>

// Return a string describing the direction given by the input code.
// Valid codes: 0=left, 1=right, 2=up, 3=down.
// Any other code returns "Illegal option".
std::string describeDirection(const int directionCode) {
    switch (directionCode) {
        case 0:
            return "Going left";
        case 1:
            return "Going right";
        case 2:
            return "Going up";
        case 3:
            return "Going down";
        default:
            return "Illegal option";
    }
}

#include <cassert>
#include <string>

// Function declaration (ensures the test link matches the provided solution).
std::string describeDirection(const int directionCode);

int main() {
    assert(describeDirection(0) == "Going left");
    assert(describeDirection(1) == "Going right");
    assert(describeDirection(2) == "Going up");
    assert(describeDirection(3) == "Going down");
    assert(describeDirection(4) == "Illegal option");
    assert(describeDirection(-1) == "Illegal option");
    assert(describeDirection(100) == "Illegal option");
    assert(describeDirection(-100) == "Illegal option");
    assert(describeDirection(0) == "Going left"); // repeated to verify stability
    assert(describeDirection(3) == "Going down"); // repeated to cover boundary
    return 0;
}
