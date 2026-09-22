// Write a C++ function named `decodeMorse` that takes a non-empty string `code` consisting only of the characters `'.'` (dot) and `'-'` (dash), and returns a string representing the decoded digits according to this rule: a single dot (`'.'`) decodes to the digit `'0'`; a dash followed immediately by a dot (`"-."`) decodes to `'1'`; and a dash followed immediately by another dash (`"--"`) decodes to `'2'`. The input is guaranteed to be a valid sequence, meaning that every dash is always followed by at least one more character (dot or dash), and there are no leading or trailing dashes without a partner. Process the string from left to right, decoding each symbol as you go, and concatenate the resulting digit characters into the output string. Note that dashes are only ever used as part of the two‑character pairs described, never alone. The function must not modify its input and must be declared with `const` correctness.

#include <cassert>
#include <string>

std::string decodeMorse(const std::string& code);

int main() {
    assert(decodeMorse(".") == "0");
    assert(decodeMorse("-.") == "1");
    assert(decodeMorse("--") == "2");
    assert(decodeMorse(".-.") == "10");
    assert(decodeMorse(".--") == "11");
    assert(decodeMorse("..") == "00");
    assert(decodeMorse("..--.-") == "0010");
    assert(decodeMorse(".--.") == "110");
    assert(decodeMorse("....") == "0000");
    assert(decodeMorse("--.") == "20");
    return 0;
}

#include <string>

// Decode a Morse-like sequence of dots and dashes into digits 0, 1, 2.
std::string decodeMorse(const std::string& code) {
    std::string result;
    size_t i = 0;
    while (i < code.length()) {
        if (code[i] == '.') {
            result += '0';
            ++i;
        } else { // code[i] == '-'
            if (code[i + 1] == '.') {
                result += '1';
            } else { // code[i + 1] == '-'
                result += '2';
            }
            i += 2;
        }
    }
    return result;
}

// The decoding rule is straightforward: iterate through the input string `code` from index 0 to `code.size() - 1`. For each position, inspect the current character:
// - If it is a dot `'.'`, then append `'0'` to the result and move to the next character (increment index by 1).
// - If it is a dash `'-'`, then we know it is part of a two‑character sequence. Look at the next character (`code[i+1]`):
//   - If the next character is a dot `'.'`, append `'1'` and skip both characters by incrementing the index by 2.
//   - If the next character is a dash `'-'`, append `'2'` and skip both characters by incrementing the index by 2.
// The input is valid, so we never encounter a lone dash, and `i+1` is always within bounds when `code[i]` is a dash. Time complexity is `O(n)` where `n` is the length of the input, because each character is examined once and each position is either consumed as a single dot or as part of a pair. Space complexity is `O(n)` for the output string, but no extra auxiliary data structures are used besides the result string.
