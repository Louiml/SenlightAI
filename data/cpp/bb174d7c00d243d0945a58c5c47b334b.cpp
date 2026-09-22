// Given a string `str` and an integer `k`, write a C++ function that returns a new string where the first `k` characters of the original string have been reversed, while the rest of the string remains unchanged. If `k` is greater than the string length, reverse all characters. If `k` is less than or equal to 0, return the original string unchanged. Use C++ standard algorithms, and ensure the function works for both empty and non-empty strings.

// The main algorithm uses the standard library `std::reverse` on a substring range. Since `std::reverse` requires iterators, we can compute the effective reversal position as `k = max(0, min(k, (int)s.size()))`. Then, we call `std::reverse(s.begin(), s.begin() + effective_k)`. This approach handles all edge cases: when `k` is 0 or negative, the effective `k` becomes 0, so the reverse range is empty and the string is unchanged. When `k` exceeds the length, the min operation caps it to the full length, reversing the entire string. For an empty string, the effective `k` is 0 and the operation does nothing. The time complexity is O(k_eff) where k_eff is the number of characters actually reversed, and the space complexity is O(1) auxiliary (the reversal is in-place on a copy). The function takes the string by value or by const reference and returns a modified copy; using a const reference and returning a new string is cleaner.

#include <algorithm>
#include <string>

// Reverse the first k characters of the input string and return the result.
// If k is non-positive, the string is unchanged. If k exceeds the length,
// the entire string is reversed.
std::string reverseFirstK(std::string s, int k) {
    if (k <= 0 || s.empty()) {
        return s;
    }
    int effective_k = std::min(k, static_cast<int>(s.size()));
    std::reverse(s.begin(), s.begin() + effective_k);
    return s;
}

#include <cassert>
#include <string>

// Declaration of the function under test.
std::string reverseFirstK(std::string s, int k);

int main() {
    assert(reverseFirstK("Hello, World!", 5) == "olleH, World!");
    assert(reverseFirstK("ABCDEF", 3) == "CBADEF");
    assert(reverseFirstK("ABCDEF", 6) == "FEDCBA");
    assert(reverseFirstK("ABCDEF", 10) == "FEDCBA"); // k > length
    assert(reverseFirstK("ABCDEF", 0) == "ABCDEF");
    assert(reverseFirstK("ABCDEF", -2) == "ABCDEF");
    assert(reverseFirstK("", 5) == "");
    assert(reverseFirstK("A", 1) == "A");
    assert(reverseFirstK("A", 2) == "A"); // k > length, but only one char
    assert(reverseFirstK("ab", 1) == "ab");
    return 0;
}
