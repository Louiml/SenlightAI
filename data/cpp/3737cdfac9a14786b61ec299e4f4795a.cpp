Write a standalone C++ function named `caesarEncryptDecrypt` that takes a plaintext string, an integer key, and a boolean flag (`true` for encryption, `false` for decryption), and returns the resulting string after applying a Caesar cipher over a fixed alphabet of 27 characters: the 26 lowercase English letters followed by a space (`'a'` through `'z'` and `' '`). The key can be any non-negative integer (including 0 and values larger than 27, which should wrap around using modulo arithmetic). The function must only handle lowercase letters and spaces; any other character (uppercase, digits, punctuation) should be left unchanged in the output. The function must be `const`-correct (i.e., not modify the input string) and should handle empty strings gracefully. Your implementation should not rely on any external libraries beyond the C++ standard library. Do not provide a `main` function in the solution; just the function itself with necessary headers and comments.

// The solution maps each character in the input to its position in the fixed 27-character alphabet. For encryption, the new position is `(position + key) % 27`; for decryption, it is `(position - key % 27 + 27) % 27` (or equivalently `(position + (27 - key%27)) % 27`). The modulo ensures wrap-around for keys larger than 26. Characters not in the alphabet (e.g., `'A'`, `'1'`, `'.'`) are appended unchanged. The algorithm iterates through each character once, performing constant-time lookup in the alphabet string (using `find`), so time complexity is O(n * 27) due to the linear search, but since 27 is constant, effectively O(n). Space complexity is O(n) for the result string (plus O(1) for the alphabet). Edge cases include: empty string returns empty; key 0 returns the same string; key larger than 27 wraps correctly; characters outside the alphabet are preserved; spaces are part of the alphabet and shift accordingly.

#include <string>

// Constants for the Caesar cipher alphabet: 26 lowercase letters + space
const std::string CAESAR_ALPHABET = "abcdefghijklmnopqrstuvwxyz ";

// Encrypt or decrypt a string using a Caesar cipher over the 27-character alphabet.
// If encrypt is true, shift forward by key; if false, shift backward by key.
// Non-alphabet characters are left unchanged.
std::string caesarEncryptDecrypt(const std::string& input, int key, bool encrypt) {
    // Normalize key to a non-negative value less than alphabet size
    int effectiveKey = key % 27;
    if (effectiveKey < 0) effectiveKey += 27; // handle negative keys defensively

    int shift = encrypt ? effectiveKey : (27 - effectiveKey) % 27;

    std::string result;
    result.reserve(input.size());

    for (char ch : input) {
        size_t pos = CAESAR_ALPHABET.find(ch);
        if (pos == std::string::npos) {
            result += ch; // not in alphabet, keep unchanged
        } else {
            result += CAESAR_ALPHABET[(pos + shift) % 27];
        }
    }
    return result;
}

#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
int main() {
    // Basic encryption and decryption round-trip
    std::string plain = "hello world";
    std::string encrypted = caesarEncryptDecrypt(plain, 3, true);
    assert(encrypted == "khoor zruog");
    assert(caesarEncryptDecrypt(encrypted, 3, false) == plain);

    // Key 0 does nothing
    assert(caesarEncryptDecrypt(plain, 0, true) == plain);
    assert(caesarEncryptDecrypt(plain, 0, false) == plain);

    // Large key wraps around
    assert(caesarEncryptDecrypt(plain, 30, true) == caesarEncryptDecrypt(plain, 3, true));

    // Space character shifts
    assert(caesarEncryptDecrypt("a ", 1, true) == "b a"); // 'a'->'b', ' '->'a'
    assert(caesarEncryptDecrypt("b a", 1, false) == "a ");

    // Characters outside alphabet unchanged
    assert(caesarEncryptDecrypt("A1! ", 1, true) == "A1!a"); // space shifts to 'a'
    assert(caesarEncryptDecrypt("A1!a", 1, false) == "A1! ");

    // Empty string
    assert(caesarEncryptDecrypt("", 5, true) == "");

    // Key exactly equal to alphabet size does nothing
    assert(caesarEncryptDecrypt("test", 27, true) == "test");

    // All alphabet characters shifted by 1: 'a'->'b', ..., 'z'->' ', ' '->'a'
    std::string all = "abcdefghijklmnopqrstuvwxyz ";
    assert(caesarEncryptDecrypt(all, 1, true) == "bcdefghijklmnopqrstuvwxyz a");
    assert(caesarEncryptDecrypt("bcdefghijklmnopqrstuvwxyz a", 1, false) == all);
}
