Write a C++ function `printNumberTriangle` that takes a positive integer `n` and outputs to standard output a triangle pattern of numbers. The triangle has `n` rows. In the `i`-th row (1-indexed), the number `i` is printed `i` times, with each occurrence separated by a single space, followed by a newline. The function should not read any input and should return `void`. Assume `n` is always a positive integer (at least 1). The output should match exactly the following format for `n = 4`:  
```
1 
2 2 
3 3 3 
4 4 4 4 
```  
Each line ends with a newline character, and there are no leading or trailing spaces on any line.
#include <cassert>
#include <sstream>
#include <iostream>

// Declaration of the function to test.
void printNumberTriangle(int n);

int main() {
    // Helper lambda to capture output
    auto capture = [](int n) {
        std::ostringstream oss;
        std::streambuf* old = std::cout.rdbuf(oss.rdbuf());
        printNumberTriangle(n);
        std::cout.rdbuf(old);
        return oss.str();
    };

    assert(capture(1) == "1 \n");
    assert(capture(2) == "1 \n2 2 \n");
    assert(capture(3) == "1 \n2 2 \n3 3 3 \n");
    assert(capture(4) == "1 \n2 2 \n3 3 3 \n4 4 4 4 \n");
    assert(capture(5) == "1 \n2 2 \n3 3 3 \n4 4 4 4 \n5 5 5 5 5 \n");
    assert(capture(0) == ""); // Not required by task but ensures function handles edge gracefully
    return 0;
}
#include <iostream>

// Prints a triangle where row i contains the number i repeated i times.
void printNumberTriangle(int n) {
    for (int i = 1; i <= n; ++i) {
        for (int j = 0; j < i; ++j) {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }
}
// The solution uses two nested loops. The outer loop iterates from `1` to `n` inclusive, representing the row number. For each row `i`, the inner loop runs `i` times, printing the current value `i` followed by a space. After the inner loop finishes, a newline is printed to move to the next row. This directly constructs the required pattern. Edge cases: for `n = 1`, the output is a single line `1 ` (the trailing space before the newline is required as per the pattern; note the example shows a space after each number in a row, including the last one, then a newline). No special handling is needed because the loops naturally handle the smallest valid input. Time complexity is \(O(n^2)\) because the total number of printed numbers is the sum of the first `n` integers, which is \(n(n+1)/2\). Space complexity is \(O(1)\) as only constant extra space is used for loop variables.
