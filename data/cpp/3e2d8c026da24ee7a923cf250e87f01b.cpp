/*
Write a C++ function that takes a positive integer `N` and returns a string of length `N + 3` constructed as follows: the string consists of a capital letter `'L'`, followed by exactly `N` lowercase letter `'o'` characters, followed by the characters `"ng"`. In other words, create the word `"Long"` but replace the single `'o'` with `N` consecutive `'o'` characters, so that the result always starts with `L`, ends with `ng`, and has exactly `N` `'o'` characters in between. The input `N` is guaranteed to be at least 1. For example, if `N = 1`, the result is `"Long"`; if `N = 3`, the result is `"Looong"` (note that there are three `'o'` characters). Your function must be named `stretchLong` and accept the integer `N` as a parameter, returning the resulting string. Do not worry about extremely large `N` (assume it fits in a standard `int`), but your solution must be efficient.
*/

#include <string>

// Returns a string that starts with 'L', contains exactly n lowercase 'o's,
// and ends with "ng". For example, stretchLong(3) returns "Looong".
std::string stretchLong(int n) {
    // Create a string consisting of n copies of 'o'.
    std::string result = "L" + std::string(n, 'o') + "ng";
    return result;
}

#include <cassert>
#include <string>

// The solution function is declared above (or copy it here).
std::string stretchLong(int n);

int main() {
    // Basic cases.
    assert(stretchLong(1) == "Long");
    assert(stretchLong(2) == "Loong");
    assert(stretchLong(3) == "Looong");
    assert(stretchLong(4) == "Loooong");

    // Check length and content for a larger value.
    std::string result = stretchLong(10);
    assert(result.size() == 13);  // 1 'L' + 10 'o's + 2 "ng" = 13
    assert(result[0] == 'L');
    assert(result.substr(1, 10) == std::string(10, 'o'));
    assert(result.substr(11) == "ng");

    // Edge case: N = 1.
    assert(stretchLong(1) == "Long");
    assert(stretchLong(1).size() == 4);

    // Verify no unexpected characters.
    std::string r2 = stretchLong(5);
    for (char c : r2) {
        assert(c == 'L' || c == 'o' || c == 'n' || c == 'g');
    }
}

// The problem is straightforward: build a string that starts with the character `'L'`, then append `N` copies of the character `'o'`, then append the substring `"ng"`. The main algorithm uses a `std::string` and its `push_back` method in a loop to add the `N` `'o'` characters, then concatenates the final `"ng"`. Alternatively, one could use the `std::string` constructor `std::string(n, 'o')` to create the repeated characters in O(N) time with a single allocation, which is slightly more efficient. The only edge case to consider is `N = 1`, which simply produces `"Long"`; no special handling is needed because the general construction already works for any positive `N`. The time complexity is O(N) because we need to create a string containing N characters, and the space complexity is O(N) for the returned string (plus negligible constant space for temporaries). The function should be marked `const`-correct by accepting the input by value (since it is a simple integer) and returning the string by value.
