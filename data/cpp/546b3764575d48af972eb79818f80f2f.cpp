Write a C++ function named `shouldFillWater` that takes three integer parameters `b1`, `b2`, and `b3`, each representing whether a bottle is empty (0) or full (1). The function must return a `std::string` containing `"Water filling time"` if at least two of the three bottles are empty, and `"Not now"` otherwise. Ensure the function is `const`-correct, uses meaningful parameter names, and does not read from standard input or print to standard output.
// The solution counts the number of zeroes among the three inputs. If the count is 2 or 3, at least two bottles are empty, so we return `"Water filling time"`. If fewer than two are empty (i.e., zero or one zero), return `"Not now"`. This handles all eight possible combinations of 0/1 values. The main edge case is when all three are empty (`0 0 0`) — the condition still holds because at least two are empty, so the result is `"Water filling time"`. No input validation is needed because the problem guarantees inputs are only 0 or 1. Time complexity is O(1) constant operations, space complexity is O(1) aside from the returned string.
#include <string>

// Return "Water filling time" if at least two of three bottles are empty (0),
// otherwise return "Not now".
std::string shouldFillWater(const int b1, const int b2, const int b3) {
    // Count empty bottles (value 0).
    const int emptyCount = (b1 == 0) + (b2 == 0) + (b3 == 0);
    
    if (emptyCount >= 2) {
        return "Water filling time";
    }
    return "Not now";
}
#include <cassert>
#include <string>

// Include the solution function declaration here (e.g., from header or copy above).

int main() {
    // At least two empty
    assert(shouldFillWater(0, 0, 1) == "Water filling time");
    assert(shouldFillWater(0, 1, 0) == "Water filling time");
    assert(shouldFillWater(1, 0, 0) == "Water filling time");
    assert(shouldFillWater(0, 0, 0) == "Water filling time");

    // Fewer than two empty
    assert(shouldFillWater(0, 1, 1) == "Not now");
    assert(shouldFillWater(1, 0, 1) == "Not now");
    assert(shouldFillWater(1, 1, 0) == "Not now");
    assert(shouldFillWater(1, 1, 1) == "Not now");
}
