/*
Write a C++ function named `extractSortedDigits` that takes a non-negative integer as input and returns a `std::string` listing the decimal digit at each positional place value from the least significant digit (1's place) to the most significant digit, in the format `"The <position>'s digit is: <digit>"` with each description on its own line, where `<position>` is the actual power of 10 (e.g., 1, 10, 100, ...) and `<digit>` is the digit at that place. The function should handle numbers with trailing zeros gracefully (e.g., input `100` should output lines for 1's, 10's, and 100's places, showing digit `0` for the 1's and 10's places). The string should end with a newline after each line but no extra blank lines. The function must not print to stdout; it must return the formatted string.
*/

#include <string>

// Return a multi-line string describing the digit at each decimal place
// from the 1's place up to the most significant digit.
std::string extractSortedDigits(int num) {
    if (num < 0) num = -num; // Ensure non-negative for digit extraction
    std::string result;
    int place = 1;

    // Use do-while so that num == 0 still processes the 1's place.
    do {
        int digit = (num / place) % 10;
        result += "The " + std::to_string(place) + "'s digit is: " + std::to_string(digit) + "\n";
        place *= 10;
    } while (place <= num);

    return result;
}

#include <cassert>
#include <string>

// Function declaration (matching solution; placed here for test completeness)
std::string extractSortedDigits(int num);

int main() {
    // Single digit
    assert(extractSortedDigits(7) == "The 1's digit is: 7\n");

    // Two digits
    assert(extractSortedDigits(42) == "The 1's digit is: 2\nThe 10's digit is: 4\n");

    // Number with trailing zeros
    assert(extractSortedDigits(100) == "The 1's digit is: 0\nThe 10's digit is: 0\nThe 100's digit is: 1\n");

    // Zero itself
    assert(extractSortedDigits(0) == "The 1's digit is: 0\n");

    // Larger number with repeated digits
    assert(extractSortedDigits(2024) == "The 1's digit is: 4\nThe 10's digit is: 2\nThe 100's digit is: 0\nThe 1000's digit is: 2\n");

    // Negative input treated as positive
    assert(extractSortedDigits(-35) == "The 1's digit is: 5\nThe 10's digit is: 3\n");

    return 0;
}

// The core algorithm extracts digits place by place using division and modulo. Start with `place = 1` (representing the current positional multiplier). While `place <= num` (or equivalently `num / place != 0`), compute `digit = (num % (place * 10)) / place` — this isolates the digit at the current place. Append a formatted line to a string using `std::to_string` and direct concatenation. Then multiply `place` by 10. This loop naturally handles trailing zeros because, for example, with `num = 100`, after extracting the 1's digit (0) and 10's digit (0), `place` becomes 100, which is ≤ 100, so the loop extracts the 100's digit (1). Important edge cases: input `0` should produce one line for the 1's digit with value `0` (since `place=1` equals `num=0`, the condition `place <= num` would be false, so we must adjust the loop to run at least once — use `place <= num` or `num / place != 0` but handle `0` separately). To simplify, use a `do-while` loop that always runs at least once. Time complexity is \(O(d)\) where \(d\) is the number of digits (at most 10 for a 32-bit int), and space complexity is \(O(d)\) for the returned string.
