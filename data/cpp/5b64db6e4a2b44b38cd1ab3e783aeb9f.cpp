Write a standalone C++ function named `vigenereDecode` that takes two non-empty strings as input: a ciphertext string `s` and a key string `key`, both consisting only of lowercase English letters ('a'–'z'). The function must return the plaintext string obtained by decrypting `s` using the Vigenère cipher, where each plaintext character is recovered by shifting each ciphertext character backward by the corresponding key character's alphabetical index (0–25). The key is repeated cyclically across the length of the ciphertext. The function must handle keys of any positive length, a ciphertext of any positive length, and must not modify the input strings. Assume the input is always valid (non-empty, lowercase letters only). Provide a complete implementation with necessary headers and a descriptive comment, but do not include a `main` function or any other entry-point wrapper.
The core algorithm mirrors the standard Vigenère decryption formula: for each position `i` in the ciphertext, compute `( (s[i] - 'a') - (key[i % keyLength] - 'a') + 26 ) % 26` and convert back to a character by adding `'a'`. The `+ 26` ensures the result of the subtraction is non-negative before applying the modulo. The key index is `i % keyLength` to cycle the key. Edge cases include a key longer than the ciphertext (the modulo still works correctly since it only uses the first as many key characters as needed) and a key of length 1 (which behaves like a Caesar cipher). Since all inputs are guaranteed lowercase and non-empty, no validating checks are needed. Time complexity is O(n) where n is the length of the ciphertext, as each character is processed exactly once. Space complexity is O(n) for the output string, plus O(1) auxiliary space for the loop variables and the integer result.
#include <string>

// Decrypt a Vigenère ciphertext using a repeating key.
// Both inputs must be non-empty lowercase English strings.
std::string vigenereDecode(const std::string& s, const std::string& key) {
    std::string plaintext;
    plaintext.reserve(s.size());
    const size_t keyLength = key.size();

    for (size_t i = 0; i < s.size(); ++i) {
        int shift = key[i % keyLength] - 'a';
        int decrypted = ((s[i] - 'a') - shift + 26) % 26;
        plaintext.push_back(static_cast<char>(decrypted + 'a'));
    }
    return plaintext;
}
#include <cassert>
#include <string>

// Declaration of the solution function (assumed to be included from the header)
std::string vigenereDecode(const std::string& s, const std::string& key);

int main() {
    // Basic case: key "key" repeating, ciphertext "riivflx" decrypts to "hello"
    assert(vigenereDecode("riivflx", "key") == "hello");

    // Key length 1 (Caesar shift): ciphertext "ifmmp" with key "b" -> "hello"
    assert(vigenereDecode("ifmmp", "b") == "hello");

    // Key longer than ciphertext: only first characters of key are used
    assert(vigenereDecode("abc", "xyz") == "abc");

    // Empty is not allowed, but test with one-character inputs
    assert(vigenereDecode("q", "a") == "q"); // shift 0
    assert(vigenereDecode("q", "b") == "p"); // shift 1

    // Decryption of a known encryption: encode "attack" with key "lemon"
    // manually: a+l=a, t+e=p, t+m=h, a+o=o, c+n=p, k+l=v -> "aphhpl"? Let's compute correctly:
    // shift: a(0)+l(11)=11 -> l? Wait, encode? We only test decode.
    // Let's compute decode "lxfopvefrnhr" with key "lemon" -> "attackatdawn" (standard example)
    assert(vigenereDecode("lxfopvefrnhr", "lemon") == "attackatdawn");

    // Key repeated exactly: "abc" with key "abc" gives "aaa" (since a-a=0, b-b=0, c-c=0)
    assert(vigenereDecode("abc", "abc") == "aaa");

    // Full alphabet wrap-around: s="zzz", key="aaa" -> "zzz"; s="aaa", key="zzz" -> "bbb" (wrap)
    assert(vigenereDecode("zzz", "aaa") == "zzz");
    assert(vigenereDecode("aaa", "zzz") == "bbb");

    // Long key and long ciphertext, ensure no out-of-bound access
    std::string longCipher(1000, 'a');
    std::string longKey(500, 'b');
    // each char shift = 1, so decryption of 'a' becomes 'z'
    std::string expected(1000, 'z');
    assert(vigenereDecode(longCipher, longKey) == expected);
}
