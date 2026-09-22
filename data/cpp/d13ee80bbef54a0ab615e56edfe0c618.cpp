Create a C++ function named `isThorAdventure` that takes two parameters: a string `characterName` and an integer `powerLevel`. The function should return a string `"Y"` if the character is exactly `"Thor"` and the power level is strictly greater than or equal to `0` (i.e., any non-negative integer including zero), otherwise return `"N"`. The function must be case-sensitive (so `"thor"` is not accepted) and must handle empty strings and negative power levels correctly. You do not need to validate input types; the function will be called with valid inputs. The function should be a pure function (no side effects, no I/O), and the logic should be trivially based on the two conditions.

#include <cassert>
#include <string>

// The solution function declaration (or include the header here)
std::string isThorAdventure(const std::string& characterName, int powerLevel);

int main() {
    // Basic positive cases
    assert(isThorAdventure("Thor", 0) == "Y");
    assert(isThorAdventure("Thor", 100) == "Y");
    assert(isThorAdventure("Thor", 1) == "Y");

    // Name mismatch (case-sensitive and other names)
    assert(isThorAdventure("thor", 10) == "N");
    assert(isThorAdventure("THOR", 10) == "N");
    assert(isThorAdventure("Odin", 0) == "N");

    // Negative power level even with correct name
    assert(isThorAdventure("Thor", -1) == "N");
    assert(isThorAdventure("Thor", -100) == "N");

    // Empty string
    assert(isThorAdventure("", 5) == "N");

    // Edge: zero power level is non-negative, should be "Y"
    assert(isThorAdventure("Thor", 0) == "Y");

    // Multiple combinations to ensure independence of conditions
    assert(isThorAdventure("Loki", -5) == "N");
    assert(isThorAdventure("Loki", 5) == "N");

    return 0;
}

#include <string>

// Returns "Y" if characterName is exactly "Thor" and powerLevel is non-negative,
// otherwise returns "N".
std::string isThorAdventure(const std::string& characterName, int powerLevel) {
    if (characterName == "Thor" && powerLevel >= 0) {
        return "Y";
    }
    return "N";
}

// The solution is straightforward: check if the input string equals the literal `"Thor"` using `==` (which is case-sensitive and works for `std::string`), and check if the integer is non-negative (`powerLevel >= 0`). If both conditions are true, return `"Y"`, otherwise return `"N"`. Edge cases: empty string returns `"N"` because it does not equal `"Thor"`; negative power levels return `"N"` even if the name matches; a power level of `0` is valid because the condition is `>= 0`. Time complexity is O(1) for the string comparison (since `"Thor"` is a constant, the comparison is O(1) for the length of the input string, but effectively constant for typical inputs), and O(1) auxiliary space.
