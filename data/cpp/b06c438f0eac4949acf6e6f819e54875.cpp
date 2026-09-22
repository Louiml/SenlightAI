Write a C++ function `encryptMessage` that takes three parameters: two integers `a` and `b`, and a string `plaintext` (by reference). The function should modify the string in place by applying an affine cipher to each alphabetic character: for each letter, compute `(a * (ch - base) + b) % 26` where `base` is `'A'` for uppercase and `'a'` for lowercase, then replace the character with the resulting letter (keeping its original case). Non-alphabetic characters (digits, spaces, punctuation) must remain unchanged. The function should assume that `a` is coprime to 26 (no need to validate inside the function). The task requires the function to handle both uppercase and lowercase letters correctly, and to operate directly on the input string without returning a new string.
// The core algorithm iterates over each character in the string. For every character, we check if it is alphabetic using `std::isalpha` (from `<cctype>`). If it is, we determine the ASCII offset base: `'A'` for uppercase, `'a'` for lowercase. We then compute the transformed index by applying the affine formula `(a * (ch - base) + b) % 26`. Since `a` is guaranteed coprime to 26, the result is a valid letter index from 0 to 25. We add the base back to convert to the appropriate ASCII character, then assign it back to the string position. For non-alphabetic characters, we simply skip them. Edge cases include an empty string (loop does nothing), characters like digits or spaces (unchanged), and mixed case within the string (each case handled independently). The time complexity is O(n) where n is the string length, and space complexity is O(1) because we modify the string in place.
#include <string>
#include <cctype>

// Encrypts the input string in place using an affine cipher with key (a, b).
// Assumes a is coprime to 26; only alphabetic characters are transformed.
void encryptMessage(int a, int b, std::string& text) {
    for (char& ch : text) {
        if (std::isalpha(static_cast<unsigned char>(ch))) {
            char base = std::isupper(static_cast<unsigned char>(ch)) ? 'A' : 'a';
            ch = static_cast<char>((a * (ch - base) + b) % 26 + base);
        }
    }
}
#include <cassert>
#include <string>

void encryptMessage(int a, int b, std::string& text);

int main() {
    std::string s1 = "Hello, World!";
    encryptMessage(5, 8, s1);
    assert(s1 == "Rclla, Eapzl!");

    std::string s2 = "abcXYZ";
    encryptMessage(1, 0, s2);
    assert(s2 == "abcXYZ");

    std::string s3 = "Testing 123";
    encryptMessage(3, 1, s3);
    assert(s3 == "Uhvwlqj 123");

    std::string s4 = "";
    encryptMessage(7, 3, s4);
    assert(s4.empty());

    std::string s5 = "aBcDeF";
    encryptMessage(1, 25, s5);
    assert(s5 == "zAbCdE");

    std::string s6 = "Zebra";
    encryptMessage(3, 5, s6);
    assert(s6 == "Ghtwf");

    return 0;
}
