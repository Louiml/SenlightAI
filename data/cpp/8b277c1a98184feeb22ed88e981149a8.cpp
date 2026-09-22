/*
Write a C++ function named `printNumberTriangle` that takes an integer parameter `n` (assume `n >= 1`) and prints a right-aligned number triangle to the console where each row `i` (starting from 1) contains the numbers from 1 to `i` separated by a single space. The function should produce output matching this pattern for `n = 5`:  
```
1  
1 2  
1 2 3  
1 2 3 4  
1 2 3 4 5  
```  
Each row ends with a newline, and there are no trailing spaces after the last number on a row. The function must not return a value; it should use `std::cout` for output and include appropriate `#include <iostream>`.
*/
#include <iostream>

// Prints a right-aligned number triangle where row i contains numbers 1..i.
// Each row is terminated by a newline, with no trailing space after the last number.
void printNumberTriangle(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            if (j > 1) {
                std::cout << ' ';
            }
            std::cout << j;
        }
        std::cout << '\n';
    }
}
#include <cassert>
#include <sstream>
#include <iostream>

// Redirect cout to capture output for testing.
void runTest(int n, const std::string& expected) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printNumberTriangle(n);
    std::cout.rdbuf(oldCout);
    assert(buffer.str() == expected);
}

int main() {
    runTest(1, "1\n");
    runTest(2, "1\n1 2\n");
    runTest(3, "1\n1 2\n1 2 3\n");
    runTest(5, "1\n1 2\n1 2 3\n1 2 3 4\n1 2 3 4 5\n");
    runTest(4, "1\n1 2\n1 2 3\n1 2 3 4\n");
    return 0;
}
// The simplest approach uses two nested loops. The outer loop runs from 1 to `n`, controlling the current row number. For each row `i`, an inner loop runs from 1 to `i`, printing each number `j` followed by a space, except for the last element where we print only the number and then break the line after the inner loop completes. This ensures no trailing space on any row. Edge cases: n = 1 produces a single line `1`. For any positive `n`, the pattern is consistent; note that if `n` is large, output grows quadratically. Time complexity is O(n²) because the total number of printed numbers is the sum 1+2+...+n = n(n+1)/2. Space complexity is O(1) since only loop counters are used, and output is streamed directly.
