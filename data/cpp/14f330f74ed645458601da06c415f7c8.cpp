// Write a C++ function named `determineWinner` that takes a single positive integer `n` and returns a string representing the winner of a simple game. The game rules are: if `n` is odd, player "Ehab" wins; if `n` is even, player "Mahmoud" wins. The function must return exactly `"Ehab"` or `"Mahmoud"` (without quotes) and must handle any positive integer input. Edge cases include very large values of `n` (up to the limits of a 64-bit signed integer) and values like 1 (odd) or 2 (even). The function should not read from or write to standard input/output; it should only compute and return the result.
// The solution is straightforward: determine the parity of the input integer `n` using the modulo operator. If `n % 2` equals 1 (or equivalently, `n % 2 != 0`), the number is odd, so return `"Ehab"`; otherwise, the number is even, so return `"Mahmoud"`. No special handling is needed for edge cases because the modulo operation works correctly for all positive integers, including 1 and 2. For very large numbers, using `long long` or `int64_t` ensures no overflow. Time complexity is O(1), and space complexity is O(1), since we only perform a constant-time arithmetic operation and return a constant-size string.
#include <string>

// Determines the winner based on the parity of n.
// Returns "Ehab" if n is odd, "Mahmoud" if n is even.
std::string determineWinner(long long n) {
    if (n % 2 != 0) {
        return "Ehab";
    } else {
        return "Mahmoud";
    }
}
#include <cassert>
#include <string>

// Forward declaration of the solution function
std::string determineWinner(long long n);

int main() {
    // Basic odd/even tests
    assert(determineWinner(1) == "Ehab");
    assert(determineWinner(2) == "Mahmoud");
    assert(determineWinner(3) == "Ehab");
    assert(determineWinner(4) == "Mahmoud");
    
    // Larger values
    assert(determineWinner(999999999) == "Ehab");
    assert(determineWinner(1000000000) == "Mahmoud");
    
    // Extremely large values within 64-bit range
    assert(determineWinner(9223372036854775807LL) == "Ehab"); // largest odd 64-bit
    assert(determineWinner(9223372036854775806LL) == "Mahmoud"); // even just below
    
    // Edge case: very small positive integer
    assert(determineWinner(0) == "Mahmoud"); // 0 is even, though problem says positive, still works
}
