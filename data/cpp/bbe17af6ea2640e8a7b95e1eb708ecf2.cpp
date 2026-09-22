Write a C++ function named `isRotation` that takes two constant string references, `s` and `goal`, and returns a boolean indicating whether `goal` can be obtained by rotating `s` by any number of positions (including zero). The function must handle cases where the strings are of different lengths, empty strings, and strings with repeated characters correctly. For example, `"abcde"` rotated by 2 gives `"cdeab"`, and `"abc"` rotated by 3 gives `"abc"` (itself). The solution should not modify the input strings and should be efficient for large inputs.
// The core idea is that if `goal` is a rotation of `s`, then `goal` must appear as a contiguous substring in the string formed by concatenating `s` with itself. For instance, if `s = "abcde"`, then `s + s = "abcdeabcde"`, and every rotation of `s` (like `"cdeab"`) appears inside this doubled string. The approach first checks whether the lengths of `s` and `goal` are equal; if not, a rotation is impossible. Then it creates a new string `doubled = s + s` and uses the standard library's `find` method to check if `goal` is a substring of `doubled`. Edge cases include both strings being empty (they are trivially rotations) and cases where `goal` is longer than `s` (handled by the length check). The time complexity is \(O(n)\) for the substring search using efficient algorithms (like KMP or Boyer-Moore internally in most standard library implementations), where \(n\) is the length of the strings, and the space complexity is \(O(n)\) for the concatenated string.
#include <string>

// Returns true if goal is a rotation of s.
bool isRotation(const std::string& s, const std::string& goal) {
    if (s.size() != goal.size()) {
        return false;
    }
    if (s.empty()) {
        return true; // Both empty
    }
    std::string doubled = s + s;
    return doubled.find(goal) != std::string::npos;
}
#include <cassert>
#include <string>

// Function declaration (as above, for completeness)
bool isRotation(const std::string& s, const std::string& goal);

int main() {
    // Basic rotations
    assert(isRotation("abcde", "cdeab") == true);
    assert(isRotation("abcde", "abced") == false);
    
    // Zero rotation
    assert(isRotation("hello", "hello") == true);
    
    // Different lengths
    assert(isRotation("abc", "abcd") == false);
    assert(isRotation("abc", "ab") == false);
    
    // Empty strings
    assert(isRotation("", "") == true);
    
    // Single character
    assert(isRotation("a", "a") == true);
    assert(isRotation("a", "b") == false);
    
    // Repeated characters (potential ambiguity)
    assert(isRotation("aaa", "aaa") == true);
    assert(isRotation("abab", "baba") == true);
    
    // Long string with repeated pattern
    assert(isRotation("ababab", "ababab") == true);
    assert(isRotation("ababab", "bababa") == true);
    
    // Non-rotation with same length
    assert(isRotation("abcd", "acbd") == false);
    
    // Case sensitivity
    assert(isRotation("Abc", "bcA") == true);
    assert(isRotation("Abc", "abc") == false);
    
    return 0;
}
