Write a C++ function named `printLoopOutcome` that takes a positive integer `n` as input and returns a `std::string` representing the exact output produced by the given loop logic. The loop iterates from `i = 1` to `i = n` inclusive. For each `i`, if `i == 3`, append the line `"Skipping number 3"` (followed by a newline) to the result and continue to the next iteration. If `i == 10`, append the decimal value of `i` followed by a newline to the result, then stop the loop entirely (no further iterations). For all other values of `i`, append `i` followed by a newline to the result. The function must handle cases where `n` is less than 3 (no skipping), where `n` is between 3 and 9 (skipping but no break), where `n` is exactly 10 (the break occurs), and where `n` is greater than 10 (the break prevents numbers after 10 from being included). The returned string must have no trailing extra newline beyond the content of the loop lines; each line ends with a newline character, but if no iterations occur (i.e., `n` is 0 or negative, which are not valid inputs but still handle gracefully), return an empty string. The function should be `const`-correct (no side effects) and use `std::to_string` for conversion.

// The solution simulates a `for` loop from 1 to `n` inclusive, building a `std::string` result. The main algorithm is straightforward: initialize an empty string, loop `i` from 1 to `n`, and for each `i` apply the three-case branch: skip (when `i==3`), break (when `i==10`), or append. The edge cases are: if `n < 3`, the skip and break never trigger, and all numbers from 1 to `n` are appended; if `n` is between 3 and 9, the skip at 3 is triggered but the break never occurs; if `n` is exactly 10, the break triggers at 10 and stops the loop, so numbers 1-2, 4-9 are appended, 3 is skipped, and 10 is appended; if `n > 10`, the break still occurs at 10, so no numbers after 10 appear. For invalid input like `n <= 0`, the loop does not execute and we return an empty string. Time complexity is O(n) because we iterate up to `n` times, and space complexity is O(n) for the output string (since it stores up to `n` lines). No additional auxiliary data structures are needed.

#include <string>

// Simulate the given loop and return its exact output as a string.
// Iterates i from 1 to n. Skips printing when i==3, breaks when i==10.
std::string printLoopOutcome(int n) {
    std::string result;
    for (int i = 1; i <= n; ++i) {
        if (i == 3) {
            result += "Skipping number 3\n";
            continue;
        }
        if (i == 10) {
            result += std::to_string(i) + "\n";
            break;
        }
        result += std::to_string(i) + "\n";
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the solution function (in actual code, this would be from the header).
std::string printLoopOutcome(int n);

int main() {
    // Case where n < 3: no skip or break, all numbers from 1 to n.
    assert(printLoopOutcome(2) == "1\n2\n");
    // Case where n is between 3 and 9: skip at 3, no break.
    assert(printLoopOutcome(5) == "1\n2\nSkipping number 3\n4\n5\n");
    // Case where n is exactly 10: break occurs after printing 10.
    assert(printLoopOutcome(10) == "1\n2\nSkipping number 3\n4\n5\n6\n7\n8\n9\n10\n");
    // Case where n > 10: break at 10 prevents later numbers.
    assert(printLoopOutcome(12) == "1\n2\nSkipping number 3\n4\n5\n6\n7\n8\n9\n10\n");
    // Edge case n = 0: loop never runs, returns empty string.
    assert(printLoopOutcome(0) == "");
    // Edge case n = 1: only number 1.
    assert(printLoopOutcome(1) == "1\n");
    // Edge case n = 3: skip at 3, then loop ends.
    assert(printLoopOutcome(3) == "1\n2\nSkipping number 3\n");
    // Edge case n = 9: no break, skip at 3, all others printed.
    assert(printLoopOutcome(9) == "1\n2\nSkipping number 3\n4\n5\n6\n7\n8\n9\n");
    // Edge case n negative: treated as invalid but returns empty.
    assert(printLoopOutcome(-5) == "");
    return 0;
}
