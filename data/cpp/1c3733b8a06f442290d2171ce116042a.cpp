// Write a C++ function `swapPositions` that takes a string and two 1-based indices `a` and `b` as input, and returns a new string where the characters at positions `a` and `b` are swapped. The input string may contain any printable ASCII characters including spaces, digits, punctuation, and uppercase/lowercase letters. The indices are guaranteed to be valid (1 ≤ a, b ≤ length of the string). The function must not modify the original string (pass it by `const` reference). The solution should be efficient for strings up to 10^5 characters in length. Your function must handle the case where `a` and `b` are equal (in which case the string is unchanged) and must be safe for empty strings (though the problem guarantees at least one character, still handle it gracefully). Return the modified string.
The solution is straightforward: copy the input string into a local mutable string, then use `std::swap` on the two positions (adjusted to 0-based indices by subtracting 1). If `a == b`, no swap is performed (or swapping an element with itself is harmless). Edge cases: valid indices are guaranteed, but we protect against empty input by checking if the string is empty before accessing positions (though this is defensive). Time complexity is O(n) due to copying the string once (which is required since we must not modify the input). Space complexity is O(n) for the copy. The approach uses `string::at` or direct indexing; direct indexing with `[ ]` is fine given guaranteed valid indices. Using `std::swap` is safe and clear. The function should be `const`-correct: take the input as `const std::string&` and return by value.
#include <string>
#include <utility> // for std::swap

// Swap characters at 1-based positions a and b in a copy of the input string.
// The original string is not modified.
std::string swapPositions(const std::string& s, int a, int b) {
    if (s.empty()) return s;
    std::string result = s;
    const int idx_a = a - 1;
    const int idx_b = b - 1;
    // If indices are valid (guaranteed by problem), safe to swap.
    if (idx_a >= 0 && idx_a < static_cast<int>(result.size()) &&
        idx_b >= 0 && idx_b < static_cast<int>(result.size())) {
        std::swap(result[idx_a], result[idx_b]);
    }
    return result;
}
#include <cassert>
#include <string>

// The solution function is assumed to be declared above.
int main() {
    // Basic swap
    assert(swapPositions("hello", 1, 2) == "ehllo");
    // Swap middle and end
    assert(swapPositions("abcdef", 3, 6) == "abcfed");
    // Same index
    assert(swapPositions("abc", 2, 2) == "abc");
    // One-character string
    assert(swapPositions("z", 1, 1) == "z");
    // String with spaces and punctuation
    assert(swapPositions("a b!c", 2, 5) == "acb! ");
    // Longer string
    assert(swapPositions("The quick brown fox", 1, 19) == "xhe quick brown foT");
    // All same characters
    assert(swapPositions("aaaa", 1, 4) == "aaaa");
    // String with digits
    assert(swapPositions("12345", 2, 4) == "14325");
    // Empty string (defensive though not valid per constraints)
    assert(swapPositions("", 1, 1) == "");
    // String length 10^5 (just a moderate test)
    std::string big(100000, 'x');
    big[0] = 'A';
    big[99999] = 'Z';
    std::string res = swapPositions(big, 1, 100000);
    assert(res[0] == 'Z' && res[99999] == 'A');
    return 0;
}
