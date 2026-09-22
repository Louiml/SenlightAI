Write a C++ function `generateFibonacciSequence` that takes a positive integer `n` and prints (to standard output) the first `n` Fibonacci numbers as a single line with each number followed by two spaces, starting with `1 1` and then continuing with the sum of the previous two numbers. For `n == 1`, output just `1 ` (one number followed by two spaces). For `n == 0`, output nothing. The function must handle edge cases where `n` is 0, 1, or 2 correctly, and must not print any extra leading or trailing spaces beyond the described format (each printed number is followed by exactly two spaces, including after the last number).
#include <cassert>
#include <sstream>
#include <iostream>

// Forward declaration of the solution function
void generateFibonacciSequence(int n);

// Helper to capture output
std::string captureOutput(int n) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    generateFibonacciSequence(n);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

int main() {
    assert(captureOutput(0) == "");
    assert(captureOutput(1) == "1  ");
    assert(captureOutput(2) == "1  1  ");
    assert(captureOutput(3) == "1  1  2  ");
    assert(captureOutput(5) == "1  1  2  3  5  ");
    assert(captureOutput(7) == "1  1  2  3  5  8  13  ");
    assert(captureOutput(10) == "1  1  2  3  5  8  13  21  34  55  ");
    assert(captureOutput(-1) == "");
    return 0;
}
#include <iostream>

// Prints the first n Fibonacci numbers (starting with 1, 1) to standard output.
// Each number is followed by exactly two spaces, including after the last one.
void generateFibonacciSequence(int n) {
    if (n <= 0) {
        return;
    }

    int a = 1;
    int b = 1;

    std::cout << a << "  ";
    if (n == 1) {
        return;
    }

    std::cout << b << "  ";
    if (n == 2) {
        return;
    }

    int c;
    for (int i = 3; i <= n; ++i) {
        c = a + b;
        std::cout << c << "  ";
        a = b;
        b = c;
    }
}
// The Fibonacci sequence begins with `1, 1, 2, 3, 5, 8, ...`. The simplest iterative approach uses three variables: `a` and `b` to hold the two most recent numbers, and `c` for the next computed sum. Start by setting `a = 1` and `b = 1`. Print `a` (followed by two spaces) when `n >= 1`, then print `b` (followed by two spaces) when `n >= 2`. For `i` from 3 to `n`, compute `c = a + b`, print `c` (with two spaces), then shift `a = b` and `b = c`. Edge cases: `n == 0` prints nothing; `n == 1` prints only `"1  "`; `n == 2` prints `"1  1  "`. Each number is followed by exactly two spaces, and no extra spaces are added at the beginning or between numbers beyond those two. Time complexity is O(n) because each Fibonacci number is computed once. Space complexity is O(1) since only a constant number of variables are used (excluding output stream buffers).
