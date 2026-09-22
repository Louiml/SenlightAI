/*
Write a C++ function `describeRange(int a, int b)` that takes two positive integers `a` and `b` (with `a ≤ b`) and returns a single string containing the descriptions of all integers from `a` to `b` inclusive, each on its own line. For numbers 1 through 9, the function must output their English word (e.g., "one", "two", ..., "nine"). For numbers greater than 9, output "even" if the number is even, and "odd" if odd. The returned string must have exactly one newline after each line, including the last line (i.e., the string should end with a newline). Assume both `a` and `b` are at least 1, and `a` may equal `b`. Ensure the function is `const`-correct (no modifications to inputs) and can handle a range as large as the integer range.
*/

#include <string>

// Returns a string with one line per integer from a to b inclusive.
// Numbers 1-9 are spelled out in English; numbers >9 are labeled "even" or "odd".
std::string describeRange(int a, int b) {
    const char* words[9] = {
        "one", "two", "three", "four", "five",
        "six", "seven", "eight", "nine"
    };
    
    std::string result;
    for (int i = a; i <= b; ++i) {
        if (i >= 1 && i <= 9) {
            result += words[i - 1];
        } else if (i > 9) {
            if (i % 2 == 0) {
                result += "even";
            } else {
                result += "odd";
            }
        }
        result += '\n';
    }
    return result;
}

#include <cassert>
#include <string>

// Assume describeRange is defined above

int main() {
    // Single number in 1-9
    assert(describeRange(1, 1) == "one\n");
    assert(describeRange(9, 9) == "nine\n");
    
    // Single number >9 even and odd
    assert(describeRange(10, 10) == "even\n");
    assert(describeRange(11, 11) == "odd\n");
    
    // Range fully in 1-9
    assert(describeRange(1, 3) == "one\ntwo\nthree\n");
    
    // Range crossing from 1-9 to >9
    assert(describeRange(8, 11) == "eight\nnine\neven\nodd\n");
    
    // Range fully >9
    assert(describeRange(20, 22) == "even\nodd\neven\n");
    
    // Large range starting at 1
    assert(describeRange(1, 2) == "one\ntwo\n");
    
    // b == a == 9 and a > 9
    assert(describeRange(100, 100) == "even\n");
    assert(describeRange(101, 101) == "odd\n");
    
    return 0;
}

// The solution iterates from `a` to `b` inclusive using a simple loop. For each number `i`, check if it is between 1 and 9. If so, map it to its corresponding English word using a lookup table (an array of strings indexed by the number minus 1, or a switch/if-else chain). If `i` is greater than 9, check its parity using the modulo operator `% 2` and append "even" or "odd". Each line must be appended to a `std::string` result, followed by a newline character. Edge cases include `a == b` (only one line) and ranges that cross from 1-9 to >9 (handle both branches correctly). Time complexity is O(b - a + 1) for iterating the range, and each operation is O(1), so overall O(n) where n is the number of integers. Space complexity is O(n) for the resulting string that stores all output lines, but the auxiliary space used by the function (excluding the returned string) is O(1) for the loop variable and lookup table.
