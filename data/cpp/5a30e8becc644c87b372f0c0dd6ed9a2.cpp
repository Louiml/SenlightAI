/*
Write a C++ function named `decodeOldPhoneInput` that takes a single string representing a sequence of button presses on a classic multi-tap mobile phone keypad (digits '2'–'9' select letters, '0' acts as a separator/confirmation, and repeated presses of the same key cycle through the letters mapped to that key: "2"→"abc", "3"→"def", "4"→"ghi", "5"→"jkl", "6"→"mno", "7"→"pqrs", "8"→"tuv", "9"→"wxyz"). The function returns the decoded string. Pressing a key once gives the first letter, pressing it again immediately gives the second, and so on, wrapping around cyclically. A '0' press confirms the current selection (if any pending letter) and resets the key state. Consecutive identical key presses without an intervening '0' continue cycling; consecutive different key presses automatically select the previous key's letter and start a new cycle. The input string contains only digits and is guaranteed non-empty; it may contain multiple '0's in a row, which should be ignored. For example, `"4440555666"` decodes to `"HOLO"` (wait, actually `"44"`→'H', `"0"` confirm, `"555"`→'L', `"666"`→'O' gives `"HLO"`; but the correct example: `"4433555555666"` → `"HELLO"`). Ensure the function handles single presses, rapid repeats, and trailing separators correctly.
*/

#include <string>
#include <vector>
#include <cctype>

// Multi-tap keypad mapping for digits '2' through '9' (index 0 unused).
static const std::vector<std::string> kKeypad = {
    "",       // '0'
    "",       // '1'
    "abc",    // '2'
    "def",    // '3'
    "ghi",    // '4'
    "jkl",    // '5'
    "mno",    // '6'
    "pqrs",   // '7'
    "tuv",    // '8'
    "wxyz"    // '9'
};

// Decode a multi-tap phone keypad input string into the corresponding text.
std::string decodeOldPhoneInput(const std::string& input) {
    std::string output;
    char lastKey = '0';      // last digit pressed (excluding '0' as reset)
    int pressIndex = 0;      // current position in the key's letter list
    char pending = '\0';     // letter selected by current key sequence, if any

    auto flushPending = [&]() {
        if (pending != '\0') {
            output += pending;
            pending = '\0';
        }
    };

    for (char ch : input) {
        if (!std::isdigit(static_cast<unsigned char>(ch))) {
            continue; // ignore invalid characters (though input is digits only)
        }

        if (ch == '0') {
            flushPending();
            lastKey = '0';
            pressIndex = 0;
        } else {
            if (ch != lastKey) {
                flushPending();
                pressIndex = 0;
            } else {
                // Same key pressed again: cycle to next letter, wrapping.
                int keyIndex = ch - '0';
                pressIndex = (pressIndex + 1) % static_cast<int>(kKeypad[keyIndex].size());
            }
            pending = kKeypad[ch - '0'][pressIndex];
            lastKey = ch;
        }
    }

    flushPending(); // flush any trailing pending letter
    return output;
}

#include <cassert>
#include <string>

// Declaration of the function under test (assumed in same translation unit).
std::string decodeOldPhoneInput(const std::string& input);

int main() {
    // Basic single letters
    assert(decodeOldPhoneInput("2") == "a");
    assert(decodeOldPhoneInput("22") == "b");
    assert(decodeOldPhoneInput("222") == "c");
    assert(decodeOldPhoneInput("2222") == "a"); // wrap around

    // Multi-letter words with separators
    assert(decodeOldPhoneInput("4433555555666") == "HELLO");
    assert(decodeOldPhoneInput("4440555666") == "HLO"); // '0' separates H and L
    assert(decodeOldPhoneInput("77773399") == "SEY");   // S, E, Y

    // Consecutive different keys without '0' – auto flush
    assert(decodeOldPhoneInput("23") == "ad"); // '2' gives 'a', then '3' gives 'd'
    assert(decodeOldPhoneInput("22233") == "ce"); // "222"→'c', "33"→'e'

    // Multiple '0's and leading/trailing separators
    assert(decodeOldPhoneInput("000") == "");
    assert(decodeOldPhoneInput("0002") == "a");
    assert(decodeOldPhoneInput("2 0 0 3") == "ad"); // spaces ignored? no, only digits, but test with digits only
    assert(decodeOldPhoneInput("20003") == "ad");   // '2'→'a', '0' flush, '0' no-op, '0' no-op, '3'→'d'
    assert(decodeOldPhoneInput("555666777") == "LMO"); // '5'×3→'L', '6'×3→'M', '7'×3→'O'

    // Single key with trailing separator
    assert(decodeOldPhoneInput("50") == "j");
    assert(decodeOldPhoneInput("50") == "j");

    // Full phrase: "TEST" → 8 33 7777 8 (T is 8, E is 33, S is 7777, T is 8)
    assert(decodeOldPhoneInput("83377778") == "TEST");

    return 0;
}

// The algorithm processes the input string character by character, maintaining three state variables: the last key pressed (`lastKey`, a character `'0'`–`'9'`), the current cycling index (`index`), and a pending letter (`pending`). When a digit `'0'` is encountered, if `pending` is set, append it to the output, then clear `pending` and reset `index` to 0; `lastKey` is updated to `'0'` so that the next non-zero press starts a fresh cycle. When a non-zero digit `ch` is read, check if it equals `lastKey`. If yes, increment `index` modulo the length of the letter string for that key; if no, first flush any existing `pending` letter (append it to output), then set `index` to 0. In both cases, compute `pending` as `data[ch-'0'][index]` and update `lastKey` to `ch`. After processing all characters, append any remaining `pending` letter to the output. Edge cases: multiple consecutive `'0'`s simply reset state each time (but since `pending` is consumed on the first `'0'`, subsequent `'0'`s do nothing); leading `'0'`s are ignored; a single key press without any separator still produces one letter; duplicate letters like "222" produce 'C' after wrapping; and a sequence like "2220" produces "C". Time complexity is O(L) where L is the input length, and space complexity is O(L) for the output string (plus constant auxiliary space for state).
