// Write a C++ function named `printEvenNumbersDescending` that takes a single `unsigned int` parameter and recursively prints all even numbers from that value (rounded down to the nearest even number if it is odd) down to and including 0, each on its own line using `std::cout`. The function must handle the base case of 0 gracefully, should not print any odd numbers, and must return `void`. The task is to implement this recursive function exactly as described, without using loops or global variables, and ensure it works correctly for any non-negative input, including 0 and large values such as `UINT_MAX`.
// The core of the solution is a recursive function that processes a number and reduces it by 2 each call. The main steps are:
// 1. If the input is odd, subtract 1 to make it even (so we always print even numbers).
// 2. The base case occurs when `termo == 0`: print "0" and return.
// 3. Otherwise, print the current even number, then recursively call the function with `termo - 2`.
// This recursion effectively prints all even numbers from the adjusted starting value down to 0. Edge cases: if the input is 0, it prints 0 and stops; if the input is odd, it first adjusts to an even number, e.g., 5 becomes 4, then prints 4, 2, 0. If the input is very large (e.g., `UINT_MAX`), the odd adjustment subtracts 1, resulting in a valid even number. The time complexity is O(n) where n is the number of even numbers printed (approx. `termo/2 + 1`), and space complexity is O(n) due to the recursion call stack depth (one frame per printed number). The function uses `std::cout` directly, not returning anything.
#include <iostream>

// Recursively prints all even numbers from the given term (rounded down to an even number) down to 0, one per line.
void printEvenNumbersDescending(unsigned int termo) {
    // If termo is odd, bring it down to the nearest even number.
    if (termo % 2 == 1) {
        termo -= 1;
    }

    // Base case: print 0 and stop.
    if (termo == 0) {
        std::cout << termo << '\n';
        return;
    }

    // Print current even number and recurse with the next even number (2 less).
    std::cout << termo << '\n';
    printEvenNumbersDescending(termo - 2);
}
#include <cassert>
#include <sstream>
#include <iostream>

// Forward declaration of the function under test (already defined above).
void printEvenNumbersDescending(unsigned int termo);

// Helper to capture output into a string.
std::string captureOutput(unsigned int input) {
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());
    printEvenNumbersDescending(input);
    std::cout.rdbuf(oldCout);
    return buffer.str();
}

int main() {
    // Test small even input.
    assert(captureOutput(6) == "6\n4\n2\n0\n");
    // Test small odd input (should start from 4, not 5).
    assert(captureOutput(5) == "4\n2\n0\n");
    // Test zero input.
    assert(captureOutput(0) == "0\n");
    // Test even input 2.
    assert(captureOutput(2) == "2\n0\n");
    // Test odd input 1.
    assert(captureOutput(1) == "0\n");
    // Test moderate even input.
    assert(captureOutput(10) == "10\n8\n6\n4\n2\n0\n");
    // Test moderate odd input.
    assert(captureOutput(9) == "8\n6\n4\n2\n0\n");
    // Test large even input (e.g., 100).
    assert(captureOutput(100) == "100\n98\n96\n94\n92\n90\n88\n86\n84\n82\n80\n78\n76\n74\n72\n70\n68\n66\n64\n62\n60\n58\n56\n54\n52\n50\n48\n46\n44\n42\n40\n38\n36\n34\n32\n30\n28\n26\n24\n22\n20\n18\n16\n14\n12\n10\n8\n6\n4\n2\n0\n");
    // Edge case: UINT_MAX (odd) should be adjusted to UINT_MAX-1, but we only check the first few lines and total line count.
    {
        std::istringstream out(captureOutput(UINT_MAX));
        std::string firstLine;
        std::getline(out, firstLine);
        assert(firstLine == std::to_string(UINT_MAX - 1));
        int count = 0;
        std::string line;
        while (std::getline(out, line)) count++;
        // For UINT_MAX, the adjusted even is UINT_MAX-1 (which is 4294967294), number of values = (4294967294/2)+1 = 2147483648
        assert(count == 2147483647); // because we already read first line, remaining count is total-1
        // Actually total lines = (UINT_MAX-1)/2 + 1 = 2147483647 + 1 = 2147483648, so count after first line = 2147483647.
        assert(count == 2147483647);
    }
    return 0;
}
