Write a C++ function `std::string yesOrNo(const std::string& input)` that, given a string `input` of length exactly 3, returns the string `"YES"` if and only if the characters are `'Y'`/`'y'`, `'E'`/`'e'`, `'S'`/`'s'` in that exact order (case-insensitive), and `"NO"` otherwise. The function should handle only valid inputs of length 3 (you may assume the caller provides a string of length 3). The output must be exactly `"YES"` or `"NO"` in uppercase. This is a standalone task; do not include any input reading or main function in the solution function.

// The problem is a straightforward character comparison. The main algorithm is: first, check if the length of the string is exactly 3 (although not strictly necessary if we assume correct input, it's good practice to guard), then compare the first character to `'Y'` or `'y'`, the second to `'E'` or `'e'`, and the third to `'S'` or `'s'`. If all three conditions hold, return `"YES"`; otherwise return `"NO"`. Edge cases: case variations must be handled (e.g., "yEs" should be YES, but "Yes" is also YES because first char 'Y', second 'e', third 's' — wait "Yes" is 'Y','e','s' → YES). Any other combination like "YEs" (first 'Y', second 'E', third 's' → YES) also works. Only strings that exactly match the pattern (ignoring case) yield YES. Time complexity is O(1) since the string has fixed length 3 (or O(n) if we consider n=3, but constant). Space complexity is O(1) for the returned string.

#include <string>

// Returns "YES" if the 3-character input spells YES (case-insensitive), else "NO".
std::string yesOrNo(const std::string& input) {
    // Assume input.size() == 3 for this task, but guard defensively.
    if (input.size() != 3) {
        return "NO";
    }
    bool first = (input[0] == 'Y' || input[0] == 'y');
    bool second = (input[1] == 'E' || input[1] == 'e');
    bool third = (input[2] == 'S' || input[2] == 's');
    return (first && second && third) ? "YES" : "NO";
}

#include <cassert>
#include <string>

// Declaration of the solution function (provided above)
std::string yesOrNo(const std::string& input);

int main() {
    assert(yesOrNo("YES") == "YES");
    assert(yesOrNo("yes") == "YES");
    assert(yesOrNo("yEs") == "YES");
    assert(yesOrNo("YeS") == "YES");
    assert(yesOrNo("YEs") == "YES");
    assert(yesOrNo("yES") == "YES");
    assert(yesOrNo("NO") == "NO");
    assert(yesOrNo("yes") == "YES"); // duplicate
    assert(yesOrNo("YyY") == "NO");
    assert(yesOrNo("yee") == "NO");
    assert(yesOrNo("") == "NO");
    assert(yesOrNo("longer") == "NO");
    return 0;
}
