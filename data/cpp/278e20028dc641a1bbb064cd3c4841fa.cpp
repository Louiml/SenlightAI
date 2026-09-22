Write a C++ function that reads a positive integer `n` from standard input and prints a right-aligned number triangle pattern where each row `i` (starting from 1) contains the numbers from 1 to `i` consecutively. For example, if `n = 4`, the output should be:
```
1
1 2
1 2 3
1 2 3 4
```
The function should take the integer `n` as a parameter, validate that `n > 0` (if not, return without printing), and print each row on a new line with numbers separated by single spaces. The pattern must be generated using loops only (no recursion or string manipulation). Your implementation should be robust for any positive integer `n`, including large values (up to, say, 1000), and should not produce trailing spaces or extra newlines after the last row.

// The solution uses two nested loops. The outer loop iterates `i` from 1 to `n`, representing the current row number. The inner loop iterates `j` from 1 to `i`, printing each number followed by a space, except after the last number in the row. To avoid trailing spaces, we print `j` and then a space only if `j < i`; alternatively, we can print all numbers separated by spaces and then print a newline after the inner loop. The edge case is when `n <= 0` – we simply return without printing anything. The time complexity is \(O(n^2)\) because the total number of printed numbers is \(1 + 2 + ... + n = n(n+1)/2\). The space complexity is \(O(1)\) apart from the integer variable and loop counters because we are only using a few primitive variables. No special data structures are needed.

#include <iostream>

// Prints a right-aligned number triangle for a given positive integer n.
// If n <= 0, the function does nothing.
void printNumberTriangle(int n) {
    if (n <= 0) {
        return;
    }

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            std::cout << j;
            if (j < i) {
                std::cout << ' ';
            }
        }
        std::cout << '\n';
    }
}

#include <cassert>
#include <sstream>
#include <iostream>

// Include the solution function here (or link it).

int main() {
    // Test 1: n = 3
    {
        std::ostringstream oss;
        std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
        printNumberTriangle(3);
        std::cout.rdbuf(old_cout);
        assert(oss.str() == "1\n1 2\n1 2 3\n");
    }

    // Test 2: n = 1
    {
        std::ostringstream oss;
        std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
        printNumberTriangle(1);
        std::cout.rdbuf(old_cout);
        assert(oss.str() == "1\n");
    }

    // Test 3: n = 0 (should do nothing)
    {
        std::ostringstream oss;
        std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
        printNumberTriangle(0);
        std::cout.rdbuf(old_cout);
        assert(oss.str().empty());
    }

    // Test 4: n = -5 (should do nothing)
    {
        std::ostringstream oss;
        std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
        printNumberTriangle(-5);
        std::cout.rdbuf(old_cout);
        assert(oss.str().empty());
    }

    // Test 5: n = 5 (verify first three and last row)
    {
        std::ostringstream oss;
        std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
        printNumberTriangle(5);
        std::cout.rdbuf(old_cout);
        std::string expected = "1\n1 2\n1 2 3\n1 2 3 4\n1 2 3 4 5\n";
        assert(oss.str() == expected);
    }

    // Test 6: n = 2 (verify no trailing spaces on the row)
    {
        std::ostringstream oss;
        std::streambuf* old_cout = std::cout.rdbuf(oss.rdbuf());
        printNumberTriangle(2);
        std::cout.rdbuf(old_cout);
        assert(oss.str() == "1\n1 2\n");
    }

    return 0;
}
