/*
Write a C++ function that takes a single character as input representing a person's employment status: `'s'` or `'S'` for senior, `'j'` or `'J'` for junior. The function should return a string containing the appropriate monthly salary as `"Rs.400"` for senior, `"Rs.275"` for junior, or `"Error"` for any other character. The function must be case-insensitive for valid statuses, and must handle whitespace or newline characters gracefully (treating them as invalid input). Do not read from `std::cin` inside the function; instead, accept the character as a parameter and return the result.
*/

#include <string>

// Returns the salary string based on status character.
// 's' or 'S' -> "Rs.400", 'j' or 'J' -> "Rs.275", else -> "Error".
std::string getSalary(char status) {
    if (status == 's' || status == 'S') {
        return "Rs.400";
    }
    if (status == 'j' || status == 'J') {
        return "Rs.275";
    }
    return "Error";
}

#include <cassert>
#include <string>

// Function declaration from solution.
std::string getSalary(char status);

int main() {
    // All valid senior inputs.
    assert(getSalary('s') == "Rs.400");
    assert(getSalary('S') == "Rs.400");
    
    // All valid junior inputs.
    assert(getSalary('j') == "Rs.275");
    assert(getSalary('J') == "Rs.275");
    
    // Invalid inputs.
    assert(getSalary('a') == "Error");
    assert(getSalary('1') == "Error");
    assert(getSalary(' ') == "Error"); // space character
    assert(getSalary('\n') == "Error"); // newline character
    assert(getSalary('x') == "Error");
    
    return 0;
}

// The solution uses a simple branching approach. First, check if the input character is either `'s'` or `'S'` using logical OR. If true, return the senior salary string. Otherwise, check if it is `'j'` or `'J'`. If true, return the junior salary string. Otherwise, return `"Error"`. The main edge case is case sensitivity — we handle both uppercase and lowercase. Another edge case is any other character, including digits, punctuation, or whitespace, which will all fall into the final `else` branch. The algorithm runs in constant time `O(1)` and uses constant auxiliary space `O(1)` — it performs at most two character comparisons and returns a pre-allocated string (no dynamic allocation beyond the returned string literal, which is typically static in C++). For extremely large inputs (not applicable here), time complexity would remain constant.
