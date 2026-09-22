Write a C++ function named `isRotation` that takes two strings, `s` and `goal`, and returns a boolean indicating whether `goal` can be obtained by rotating `s` any number of positions (including zero). Rotating a string means shifting all characters left by one and moving the first character to the end. The strings may contain any printable ASCII characters, including spaces, and may be empty. The comparison must be case-sensitive. You should not mutate the input strings, and you must handle the edge case where both strings are empty (which is a valid rotation). The function signature is `bool isRotation(const std::string& s, const std::string& goal)`.

// The key observation is that if we concatenate the original string `s` with itself (`s + s`), then every possible rotation of `s` appears as a contiguous substring of this doubled string. For example, if `s = "abc"`, then `s + s = "abcabc"`, which contains `"abc"`, `"bca"`, and `"cab"` — all rotations. Therefore, we just need to check if `goal` is a substring of `s + s`. However, we must first verify that the lengths match, because if the lengths differ, `goal` cannot be a rotation (e.g., `s = "abc"` and `goal = "ab"`). Also, if the length is zero, both strings are empty, and `(s + s)` is empty, which contains the empty substring — so we should return `true`. We must also consider the case where `s` is non-empty but `goal` is empty, which should return `false` since lengths differ. The time complexity is O(n + m) for the string concatenation and substring search, where n is the length of `s` and m is length of `goal` (which equals n after the length check). The space complexity is O(n) for the temporary doubled string, since `s + s` creates a new string.

#include <string>

// Returns true if `goal` is a rotation of `s` (any number of positions), false otherwise.
bool isRotation(const std::string& s, const std::string& goal) {
    // A rotation must preserve length.
    if (s.length() != goal.length()) {
        return false;
    }
    // Concatenating s with itself contains all rotations of s.
    // For empty strings, the concatenation is empty, and find("") returns 0, which is valid.
    return (s + s).find(goal) != std::string::npos;
}

#include <cassert>

int main() {
    // Basic rotations
    assert(isRotation("abc", "abc") == true);
    assert(isRotation("abc", "bca") == true);
    assert(isRotation("abc", "cab") == true);
    // Non-rotation
    assert(isRotation("abc", "acb") == false);
    // Length mismatch
    assert(isRotation("abc", "ab") == false);
    assert(isRotation("ab", "abc") == false);
    // Edge case: both empty
    assert(isRotation("", "") == true);
    // Edge case: one empty, other not
    assert(isRotation("", "a") == false);
    assert(isRotation("a", "") == false);
    // Single character strings
    assert(isRotation("a", "a") == true);
    assert(isRotation("a", "b") == false);
    // Repeated characters
    assert(isRotation("aaaa", "aaaa") == true);
    // Strings with spaces
    assert(isRotation("ab cd", "cd ab") == true);
    assert(isRotation("ab cd", "ab dc") == false);
    return 0;
}
