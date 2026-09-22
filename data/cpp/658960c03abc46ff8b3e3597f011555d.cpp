Write a standalone C++ function named `reverseCString` that accepts a C-style string (a `const char*` pointing to a null-terminated character array) and returns a `std::string` containing the characters of the input in reverse order. The function must not modify the input array, must handle an empty string (returning an empty string), and must operate on the entire string without using any standard library reversal functions (e.g., `std::reverse`). The solution should demonstrate manual character swapping using a two‑pointer technique, similar to the provided snippet, but adapted to be const‑correct and return a new string.

#include <cassert>
#include <string>

// Declare the function to test (assumes it is in the same translation unit).
std::string reverseCString(const char* input);

int main() {
    // Basic reversal
    assert(reverseCString("hello") == "olleh");
    // Single character
    assert(reverseCString("a") == "a");
    // Empty string
    assert(reverseCString("") == "");
    // Null pointer
    assert(reverseCString(nullptr) == "");
    // Even length with spaces
    assert(reverseCString("ab cd") == "dc ba");
    // Palindrome (unchanged)
    assert(reverseCString("racecar") == "racecar");
    // Mixed case and punctuation
    assert(reverseCString("A!b@C") == "C@b!A");
    // Longer string
    assert(reverseCString("123456789") == "987654321");
    // All same characters
    assert(reverseCString("zzz") == "zzz");
    // Two characters
    assert(reverseCString("xy") == "yx");
    
    return 0;
}

#include <string>
#include <cstddef>

// Return a new string containing the characters of the given C-string in reverse order.
// The input is not modified.
std::string reverseCString(const char* input) {
    // Handle null pointer gracefully (treat as empty).
    if (input == nullptr) {
        return std::string();
    }
    
    // Copy the input into a std::string to allow modification.
    std::string result(input);
    
    // Two-pointer swap from both ends toward the center.
    std::size_t i = 0;
    std::size_t j = result.size() - 1;
    while (i < j) {
        char temp = result[i];
        result[i] = result[j];
        result[j] = temp;
        ++i;
        --j;
    }
    
    return result;
}

// The algorithm uses two indices: one starting at the beginning (`i = 0`) and one at the end (`j = length - 1`). In a loop that continues while `i < j`, we swap the characters at these positions, then increment `i` and decrement `j`. This in‑place swap requires no extra buffer and processes each character exactly once. Because we must not modify the input, we first copy the input into a local `std::string` (or create a new string of the same length) and then perform the swaps on that copy. Edge cases include: empty string (returns empty), single character (unchanged), and strings with even or odd lengths (the middle character in an odd‑length string stays in place). Time complexity is O(n) where n is the string length, because we iterate over roughly half the characters. Space complexity is O(n) due to the returned reversed copied string, excluding the input itself.
