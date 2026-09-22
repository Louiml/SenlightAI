// Write a C++ function `calculateDialTime(const std::string& dialLog)` that, given a string containing only uppercase English letters (A–Z), returns the total time required to type the string using an old telephone keypad. Each letter maps to a key: keys 2 through 9, where keys 2–6 have three letters, key 7 has four (PQRS), key 8 has three (TUV), and key 9 has four (WXYZ). The time to type a letter is the number of presses required: the first letter on a key takes 1 press, the second takes 2, the third takes 3, and the fourth (if any) takes 4. You must compute the mapping exactly as described, not hard-code the presses. The input will be non-empty and contain only uppercase letters. Return the total press count.

// We need to simulate the construction of the digit-to-letters mapping from the historical phone keypad layout: digits 2–9, where 2=ABC, 3=DEF, 4=GHI, 5=JKL, 6=MNO, 7=PQRS, 8=TUV, 9=WXYZ. The original code builds an `unordered_map<char, int>` from each letter to its needed presses (starting j=0, then j+3 meaning presses = key position + 3? Actually the code sets `dial[65+i] = j + 3;` where j starts at 0, so first letters get 3, but that is incorrect for the real problem—wait, in the original snippet, the mapping seems to assign a "dial number" not presses. In our task, we need the number of presses per letter. However, the original snippet computes something else (time as sum of dial[log], which is digit value), but our task is to implement a correct press-count function. The key is to derive press counts logically. For letters A–C, first letter of key 2 → 1 press. D–F → 2 presses, etc. For PQRS on key 7, P=1, Q=2, R=3, S=4. For WXYZ, W=1, X=2, Y=3, Z=4. Edge case: no letter requires 1 press? Actually all first letters take 1. So we can compute directly: for a given letter c, find its group. But to avoid hard-coding, we can compute the mapping algorithmically: iterate through letters A–Z, keep a pointer to current key digit (starting at 2), and a press counter (1..3 or 1..4) that resets when moving to next key. For keys 2–6 and 8, after 3 letters move to next key; for keys 7 and 9, after 4 letters. Then store the press count for each letter. Then sum for the input string. Edge case: ensure the mapping is correct for all 26 letters. Time complexity O(n) where n is length of input string; space O(1) for mapping (fixed 26 entries). Provide a const-correct function.

#include <string>
#include <unordered_map>

// Returns the total number of key presses needed to type dialLog on a standard phone keypad.
// Key 2: ABC, 3: DEF, 4: GHI, 5: JKL, 6: MNO, 7: PQRS, 8: TUV, 9: WXYZ.
// The first letter on a key takes 1 press, the second 2, the third 3, and the fourth 4.
int calculateDialTime(const std::string& dialLog) {
    // Build the press-count mapping for each uppercase letter.
    std::unordered_map<char, int> pressCount;
    int currentKey = 2;          // Start at key 2
    int currentPress = 1;        // First letter on a key takes 1 press
    int lettersOnThisKey = 0;    // How many letters assigned to current key

    for (char c = 'A'; c <= 'Z'; ++c) {
        pressCount[c] = currentPress;

        // Determine how many letters this key holds (3 or 4).
        // Keys 7 and 9 have 4 letters; all others have 3.
        int maxLetters = (currentKey == 7 || currentKey == 9) ? 4 : 3;

        lettersOnThisKey++;
        if (lettersOnThisKey == maxLetters) {
            // Move to next key, reset press counter.
            currentKey++;
            currentPress = 1;
            lettersOnThisKey = 0;
        } else {
            currentPress++;
        }
    }

    // Sum presses for each character in the log.
    int totalTime = 0;
    for (char ch : dialLog) {
        totalTime += pressCount.at(ch);
    }
    return totalTime;
}

#include <cassert>

int main() {
    // Simple cases
    assert(calculateDialTime("A") == 1);
    assert(calculateDialTime("B") == 2);
    assert(calculateDialTime("C") == 3);
    // Edge of a 3-letter key
    assert(calculateDialTime("D") == 1); // D is first on key 3
    assert(calculateDialTime("F") == 3); // F last on key 3
    // Key 7 has 4 letters
    assert(calculateDialTime("P") == 1);
    assert(calculateDialTime("S") == 4);
    // Key 9 has 4 letters
    assert(calculateDialTime("W") == 1);
    assert(calculateDialTime("Z") == 4);
    // Multi-letter string
    assert(calculateDialTime("HELLO") == 1+2+3+3+1); // H=2, E=2, L=3, O=1 => 11
    assert(calculateDialTime("WORLD") == 1+3+3+1+3); // W=1, O=3, R=3, L=1, D=3 => 11
    // Full alphabet sum: A-Z presses: 1+2+3 + 1+2+3 + ... for keys 2-6 (5 keys*6=30) plus key7 (1+2+3+4=10), key8 (1+2+3=6), key9 (1+2+3+4=10) total 56
    std::string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    assert(calculateDialTime(alphabet) == 56);
    // Empty? Not required, but test that it returns 0 if empty (not in spec, but safe)
    assert(calculateDialTime("") == 0);
    return 0;
}
