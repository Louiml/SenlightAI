/*
Write a C++ function named `contestRoundName` that takes a single string `s` representing one of two possible contest round names, "ABC" or "ARC", and returns the opposite round name as a string. If the input is exactly "ABC", the function must return "ARC"; for any other input (including "ARC" or any unexpected string), it must return "ABC". The function should be case-sensitive and must not modify the input string.
*/

#include <string>

// Given a contest round name, return the opposite name.
// If input is exactly "ABC", return "ARC"; otherwise return "ABC".
std::string contestRoundName(const std::string& s) {
    // Compare the input string with the literal "ABC".
    // Since the task specifies only two valid names, any non-"ABC" input maps to "ABC".
    if (s == "ABC") {
        return "ARC";
    } else {
        return "ABC";
    }
}

#include <cassert>
#include <string>

// Declaration of the solution function (replace with actual include if needed).
std::string contestRoundName(const std::string& s);

int main() {
    // Test the known valid inputs.
    assert(contestRoundName("ABC") == "ARC");
    assert(contestRoundName("ARC") == "ABC");
    
    // Edge cases: unexpected inputs should return "ABC".
    assert(contestRoundName("") == "ABC");
    assert(contestRoundName("abc") == "ABC");
    assert(contestRoundName("ABC ") == "ABC");  // trailing space
    assert(contestRoundName(" ARC") == "ABC"); // leading space
    assert(contestRoundName("XYZ") == "ABC");
    assert(contestRoundName("123") == "ABC");
    assert(contestRoundName("AbC") == "ABC");  // case sensitivity check
    
    return 0;
}

// The solution is a straightforward conditional check. Since the task explicitly defines only two valid contest names and specifies that any non-"ABC" input should map to "ABC", the logic reduces to comparing the input string with the literal "ABC". If equal, return the literal "ARC"; otherwise, return "ABC". This handles all edge cases: empty strings, lowercase strings, strings with whitespace, and any other unexpected input all fall into the else branch. No special error handling is needed because the problem statement specifies the fallback behavior. Time complexity is O(1) because string comparison of constant-size literals is constant-time in practice (though technically linear in the length of the compared strings, the inputs here are trivially small). Space complexity is O(1) auxiliary, ignoring the returned string.
