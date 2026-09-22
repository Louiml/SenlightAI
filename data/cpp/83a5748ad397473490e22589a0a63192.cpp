Write a C++ function that accepts a single positive integer `n` and prints (via `std::cout`) an inverted right-aligned triangle of asterisks with `n` rows. The first row contains `n` asterisk–space pairs, and each subsequent row has one fewer pair, with the entire triangle shifted right by one additional leading space per row. After the last row, the function should not print an extra trailing newline beyond the final `endl`. The function must be named `printInvertedTriangle` and take an `int` parameter; it should assume the input is at least 1, but if given 0 or a negative value, it should simply do nothing without printing any output. Use nested `do-while` loops in your implementation (no `for` or `while` loops are allowed).

#include <cassert>
#include <sstream>
#include <iostream>

// Declare the function under test (since it's defined above in the solution section, we just call it here)
// For testing, we capture stdout using a stringstream redirection helper.

// Helper to capture stdout
std::string captureOutput(int n) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    printInvertedTriangle(n);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // n=1: single row, no leading spaces, one " *" pair
    assert(captureOutput(1) == " *\n");
    
    // n=3: pattern with 3 rows
    assert(captureOutput(3) == " * * *\n   * *\n     *\n");
    
    // n=4: larger pattern
    assert(captureOutput(4) == " * * * *\n   * * *\n     * *\n       *\n");
    
    // n=5: verify right-alignment and row count
    assert(captureOutput(5) == " * * * * *\n   * * * *\n     * * *\n       * *\n         *\n");
    
    // n=0 and negative: should produce empty output
    assert(captureOutput(0) == "");
    assert(captureOutput(-5) == "");
    
    // n=2: small pattern
    assert(captureOutput(2) == " * *\n   *\n");
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <iostream>

// Prints an inverted right-aligned triangle of asterisks with n rows.
// For n <= 0, does nothing.
// Each row i (from n down to 1) has (n - i) leading spaces,
// followed by i " *" pairs, then a newline.
void printInvertedTriangle(int n) {
    if (n >= 1) {
        int row = n; // current row number (also number of asterisk pairs)
        do {
            // Print leading spaces: (n - row) spaces
            int spaces = n - row;
            do {
                std::cout << " ";
                --spaces;
            } while (spaces > 0);

            // Print asterisk-space pairs: row times
            int pairs = row;
            do {
                std::cout << " *";
                --pairs;
            } while (pairs > 0);

            std::cout << std::endl;
            --row;
        } while (row >= 1);
    }
}

// The core idea is to model the pattern row by row. Let `i` start at `n` and decrement until 1 (inclusive). For a given row `i`, we need to print exactly `(n - i)` leading spaces (to right-align the triangle), then print `i` asterisk–space pairs (i.e., `" *"` repeated `i` times), then a newline. The nested `do-while` structure works well: the outer loop controls rows and decrements `i`, the first inner loop prints spaces using a counter that starts at `(n - i)` and decrements to 0, and the second inner loop prints asterisks using a counter that starts at `i` and decrements to 0. Edge cases: if `n <= 0`, the outer loop condition must be checked before any execution; because `do-while` executes at least once, we must guard with an `if (n >= 1)` around the entire loop structure. Also, careful with decrement order—each inner loop should decrement its counter after printing, and the loop condition checks if the counter is still > 0. Time complexity is O(n²) because the total number of printed characters is proportional to the sum of 1..n for spaces plus 2*sum(1..n) for asterisk–space pairs, which is O(n²). Space complexity is O(1) as only integer counters are used.
