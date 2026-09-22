/*
Write a C++ function named `printEvenSequence` that takes no parameters and returns nothing, but instead prints to standard output all even integers from 2 through 20 inclusive, one per line, in ascending order. The function should not read any input from the user and must produce exactly ten lines of output. This task is derived from a pattern where a loop increments by 2 starting from 2, and the goal is to encapsulate this printing logic in a reusable, standalone function that can be tested independently.
*/

#include <iostream>

// Prints all even integers from 2 to 20 inclusive, one per line.
void printEvenSequence() {
    for (int i = 2; i <= 20; i += 2) {
        std::cout << i << '\n';
    }
}

#include <cassert>
#include <sstream>
#include <iostream>

// Assuming the solution is included above, define a helper to capture output.
void testPrintEvenSequence() {
    // Redirect std::cout to a stringstream for testing.
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    // Call the function.
    printEvenSequence();

    // Restore original std::cout.
    std::cout.rdbuf(old);

    // Expected output as a string.
    std::string expected = "2\n4\n6\n8\n10\n12\n14\n16\n18\n20\n";
    assert(buffer.str() == expected);
}

int main() {
    testPrintEvenSequence();
    // Also run the function once without asserting to ensure no crashes.
    printEvenSequence();
    return 0;
}

// The solution is straightforward: a simple `for` loop that initializes a loop counter to 2, continues while the counter is less than or equal to 20, and increments the counter by 2 in each iteration. Inside the loop, print the current value followed by a newline (using `std::cout << i << '\n'` or `printf("%d\n", i)`). The loop will execute exactly 10 times, producing the values: 2, 4, 6, 8, 10, 12, 14, 16, 18, 20. There are no edge cases to consider because the bounds and step are fixed constants. The time complexity is O(1) in terms of problem size (since the number of iterations is constant), and the space complexity is O(1) because only a single integer variable is used. The function does not need to return a value, as its purpose is to print output; testing will capture the printed output or verify the function runs without error.
