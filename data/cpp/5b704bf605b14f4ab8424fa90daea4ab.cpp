/*
Write a C++ function named `computeOperation` that takes two integers `a` and `b` and an integer `choice` representing an operation (1 for addition, 2 for subtraction, 3 for multiplication, 4 for division). The function must return a string that displays the operation and its result in the format `"a + b = result"` (using the actual operator symbol `+`, `-`, `*`, or `/`). For division, if the divisor `b` is zero, return the string `"Division by zero error"` instead of performing the division. For any invalid `choice` (not between 1 and 4), return `"Invalid choice"`. The function should not print anything to the console—it only builds and returns the result string. Assume all inputs are integers, so division should use integer division (truncating toward zero). The output string must exactly match the format with single spaces around the operator and equals sign, and no extra spaces.
*/

#include <string>

// Computes an arithmetic operation based on a choice and returns a formatted result string.
// choice: 1=+, 2=-, 3=*, 4=/ (integer division). Division by zero returns an error string.
// Invalid choice returns "Invalid choice".
std::string computeOperation(int a, int b, int choice) {
    switch (choice) {
        case 1:
            return std::to_string(a) + " + " + std::to_string(b) + " = " + std::to_string(a + b);
        case 2:
            return std::to_string(a) + " - " + std::to_string(b) + " = " + std::to_string(a - b);
        case 3:
            return std::to_string(a) + " * " + std::to_string(b) + " = " + std::to_string(a * b);
        case 4:
            if (b == 0) {
                return "Division by zero error";
            }
            return std::to_string(a) + " / " + std::to_string(b) + " = " + std::to_string(a / b);
        default:
            return "Invalid choice";
    }
}

#include <cassert>
#include <string>

// Function under test (already defined above)
std::string computeOperation(int a, int b, int choice);

int main() {
    // Basic addition
    assert(computeOperation(5, 3, 1) == "5 + 3 = 8");
    // Negative numbers subtraction
    assert(computeOperation(-2, 7, 2) == "-2 - 7 = -9");
    // Multiplication with zero
    assert(computeOperation(0, 5, 3) == "0 * 5 = 0");
    // Integer division (truncates)
    assert(computeOperation(10, 4, 4) == "10 / 4 = 2");
    // Division by negative divisor
    assert(computeOperation(-8, -2, 4) == "-8 / -2 = 4");
    // Division by zero
    assert(computeOperation(1, 0, 4) == "Division by zero error");
    // Invalid choice
    assert(computeOperation(1, 2, 5) == "Invalid choice");
    // Choice zero
    assert(computeOperation(1, 2, 0) == "Invalid choice");
    // Large numbers
    assert(computeOperation(1000000, 1000000, 1) == "1000000 + 1000000 = 2000000");
    // Exact format check with single-digit numbers
    assert(computeOperation(1, 1, 3) == "1 * 1 = 1");
    return 0;
}

// The solution uses a simple conditional (or switch) to map each `choice` to its operator and compute the result. Edge cases are: division by zero (return a special error string), invalid choice (return a special string), and guaranteeing the exact output format. For division, use integer arithmetic (`a / b`), which truncates toward zero for positive/negative integers in C++ (implementation-defined for negative before C++11, but modern compilers truncate toward zero; we assume standard behavior). The function constructs a string using `std::to_string` for the numbers and the result, concatenating with the operator character and spaces. Time complexity is O(1) because only a constant number of operations and string concatenations are performed. Space complexity is also O(1) auxiliary, aside from the returned string whose length is proportional to the number of digits in the numbers and result, but this is dependent on input size and not counted as extra algorithmic space.
