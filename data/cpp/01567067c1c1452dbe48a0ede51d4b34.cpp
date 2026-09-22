// Write a C++ function named `christmasMessage` that takes an integer `D` representing the day of December (between 22 and 25 inclusive) and returns the exact corresponding Christmas countdown phrase as a `std::string`: for 25 → `"Christmas"`, for 24 → `"Christmas Eve"`, for 23 → `"Christmas Eve Eve"`, and for 22 → `"Christmas Eve Eve Eve"`. The function must handle only these four valid inputs; for any other integer value, it should return an empty string. The function should not print anything to the console; it should only return the result. Your implementation must use a `switch` statement for the mapping and be `const`-correct where appropriate.

The solution is a direct mapping from the integer day to a constant string. A `switch` statement is the most natural choice because the domain is small, fixed, and discrete, and it avoids chains of `if-else` for clarity. The function receives the day by value (since it’s a small integer) and returns a `std::string` by value. We define a default case to return an empty string for invalid inputs, ensuring the function is total. The time complexity is O(1) because the switch performs constant-time dispatch, and the space complexity is O(1) for storage, excluding the returned string’s dynamic allocation (which is still proportional to the small fixed length of the result). The key edge case is the invalid input (e.g., 21 or 26), which must produce an empty string rather than falling through to an uninitialized return. Another edge case is ensuring the returned strings have exactly the correct number of "Eve" repetitions, which the fixed cases guarantee.

#include <string>

// Returns the Christmas countdown phrase for day D in December (22–25).
// For any other integer, returns an empty string.
std::string christmasMessage(int D) {
    switch (D) {
        case 25: return "Christmas";
        case 24: return "Christmas Eve";
        case 23: return "Christmas Eve Eve";
        case 22: return "Christmas Eve Eve Eve";
        default: return "";
    }
}

#include <cassert>
#include <string>

// The solution function is declared here (or included from a header).
std::string christmasMessage(int D);

int main() {
    assert(christmasMessage(25) == "Christmas");
    assert(christmasMessage(24) == "Christmas Eve");
    assert(christmasMessage(23) == "Christmas Eve Eve");
    assert(christmasMessage(22) == "Christmas Eve Eve Eve");
    assert(christmasMessage(21) == "");
    assert(christmasMessage(26) == "");
    assert(christmasMessage(0) == "");
    assert(christmasMessage(-5) == "");
}
