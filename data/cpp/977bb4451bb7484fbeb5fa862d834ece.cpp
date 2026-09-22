/*
Write a standalone C++ function named `computeMessageValue` that accepts a `std::string` representing a line of text (without trailing newline) and returns an `int` equal to the weighted sum of the alphabetic characters in that line, ignoring spaces. Each character's weight is its position in the alphabet (A=1, B=2, ..., Z=26) multiplied by its 1-based index in the original input string (first character index=1, second=2, etc.). Only letters A–Z (uppercase) are considered; if the input contains any other non-space character (e.g., digits, punctuation, lowercase letters), the function should treat them as zero-weight but still count their index for subsequent characters. The function must handle empty strings by returning 0. Do not read from standard input; the function must be pure and testable.
*/

#include <string>

// Compute the weighted sum of uppercase letters in the input string.
// Each letter contributes (position in string) * (alphabet index).
// All other characters (spaces, digits, lowercase) contribute 0 but still advance position.
int computeMessageValue(const std::string& message) {
    int sum = 0;
    int position = 1; // 1-based index in the original string
    for (char ch : message) {
        if (ch >= 'A' && ch <= 'Z') {
            int alphabetValue = ch - 'A' + 1;
            sum += position * alphabetValue;
        }
        // For every character (including spaces and ignored ones), advance position.
        ++position;
    }
    return sum;
}

#include <cassert>
#include <string>

int computeMessageValue(const std::string& message); // forward declaration

int main() {
    // Empty string returns 0
    assert(computeMessageValue("") == 0);

    // "A" -> position 1 * alphabet 1 = 1
    assert(computeMessageValue("A") == 1);

    // "AB" -> 1*1 + 2*2 = 1 + 4 = 5
    assert(computeMessageValue("AB") == 5);

    // "A A" -> positions: 1(A=1),2(space=0),3(A=1) -> 1*1 + 3*1 = 4
    assert(computeMessageValue("A A") == 4);

    // "ABC" -> 1*1 + 2*2 + 3*3 = 1+4+9=14
    assert(computeMessageValue("ABC") == 14);

    // Lowercase and digits are ignored but affect position: "aB" -> position1(a=0), position2(B=2) => 2*2=4
    assert(computeMessageValue("aB") == 4);

    // " Z " -> positions:1(space),2(Z=26),3(space) -> 2*26 = 52
    assert(computeMessageValue(" Z ") == 52);

    // Mixed: "A1B" -> 1*1 + 2*0 + 3*2 = 1+0+6=7
    assert(computeMessageValue("A1B") == 7);
}

// The core algorithm is a single pass over the input string. Maintain a running total `sum` and an integer `position` that starts at 1. For each character in the string (including spaces and any non-letter), increment `position` after processing. If the character is an uppercase letter ('A' to 'Z'), compute its alphabet value as `(ch - 'A' + 1)` and add `position * alphabetValue` to `sum`. If the character is a space or any other character, add 0 but still increment `position`. Edge cases: empty string returns 0; spaces at any position contribute zero but affect the index of later letters; lowercase letters or digits are ignored but still consume an index; the input may contain any printable ASCII characters. The time complexity is O(n) where n is the string length, and the auxiliary space is O(1) since only a few integer variables are used. The function should be `const`-correct by taking the string by const reference. Potential overflow: with a very long string and large index values, the sum could exceed int range, but for typical test inputs (length < 10^4) int is safe; if needed, the function could use `long long`, but the task expects `int` for simplicity.
