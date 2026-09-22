Write a standalone C++ function named `shiftCipher` that takes a `std::string` containing only lowercase English letters (`'a'`–`'z'`) and an integer `shift`, and returns a new string where each character is shifted forward in the alphabet by `shift` positions. The shift wraps around the alphabet (e.g., shifting `'z'` by 3 yields `'c'`), and negative shifts should shift backward (e.g., shifting `'a'` by −1 yields `'z'`). You may assume the input is non-empty and contains only lowercase letters.

#include <cassert>
#include <string>

std::string shiftCipher(const std::string& s, int shift);

int main() {
    assert(shiftCipher("cat", 3) == "fdw");
    assert(shiftCipher("z", 1) == "a");
    assert(shiftCipher("a", -1) == "z");
    assert(shiftCipher("hello", 5) == "mjqqt");
    assert(shiftCipher("abc", 26) == "abc");
    assert(shiftCipher("xyz", -26) == "xyz");
    assert(shiftCipher("a", 100) == "w");
    assert(shiftCipher("z", -100) == "f");
    assert(shiftCipher("abc", 52) == "abc");
    assert(shiftCipher("zzz", -27) == "yyy");
}

#include <string>

// Shifts each lowercase letter in s by shift positions, wrapping around 'z'->'a'.
std::string shiftCipher(const std::string& s, int shift) {
    std::string output = s;
    for (std::size_t i = 0; i < s.size(); ++i) {
        int index = s[i] - 'a';
        int shifted = (index + shift) % 26;
        if (shifted < 0) {
            shifted += 26;  // Handle negative modulo
        }
        output[i] = static_cast<char>('a' + shifted);
    }
    return output;
}

// The solution iterates over each character in the input string and applies the shift mathematically. For a character `c`, convert it to a zero-based index (`c - 'a'`), add the shift, and then take the result modulo 26 to ensure wrapping. Because C++'s modulo operator can yield negative results for negative left operands, add 26 before taking modulo 26 to normalize negative shifts (e.g., `(index + shift + 26) % 26`). Then convert the result back to a character by adding `'a'`. Edge cases include large positive or negative shifts (e.g., 100 or −100), which are handled naturally by the modular arithmetic. The algorithm runs in O(n) time and O(1) auxiliary space (excluding the returned string), where `n` is the length of the input.
