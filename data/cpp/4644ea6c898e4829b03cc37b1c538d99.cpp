/*
Write a C++ function named `printMultiplicationTable` that takes a positive integer `n` and returns a `std::string` containing the multiplication table of `n` from 1 to 10. Each line should contain the result of `n * i` on its own line, separated by newline characters (`'\n'`). The function must return the table as a single string (no trailing newline at the end). The input `n` is guaranteed to be a positive integer (>= 1). Do not print anything to the console; only return the string.
*/

#include <string>

// Return the multiplication table of a positive integer n from 1 to 10
// as a string with each result on a new line (no trailing newline).
std::string printMultiplicationTable(int n) {
    std::string table;
    for (int i = 1; i <= 10; ++i) {
        int product = n * i;  // Ensure const-correctness for the product
        table += std::to_string(product);
        if (i < 10) {
            table += "\n";
        }
    }
    return table;
}

#include <cassert>
#include <string>

// Function declaration (must match the solution above)
std::string printMultiplicationTable(int n);

int main() {
    assert(printMultiplicationTable(1) == "1\n2\n3\n4\n5\n6\n7\n8\n9\n10");
    assert(printMultiplicationTable(2) == "2\n4\n6\n8\n10\n12\n14\n16\n18\n20");
    assert(printMultiplicationTable(5) == "5\n10\n15\n20\n25\n30\n35\n40\n45\n50");
    assert(printMultiplicationTable(10) == "10\n20\n30\n40\n50\n60\n70\n80\n90\n100");
    assert(printMultiplicationTable(7) == "7\n14\n21\n28\n35\n42\n49\n56\n63\n70");
    assert(printMultiplicationTable(3) == "3\n6\n9\n12\n15\n18\n21\n24\n27\n30");
    // Check no trailing newline
    std::string result = printMultiplicationTable(4);
    assert(result.back() != '\n');
    // Verify length and first/last lines
    assert(result.find("4") == 0);
    assert(result.substr(result.size() - 2) == "40");
    
    return 0;
}

// The solution must generate a string that represents the multiplication table for a given integer `n`. The algorithm iterates from 1 to 10 inclusive, computing `n * i` for each value of `i`. Each result is appended to a `std::string` followed by a newline character, except after the last iteration where no trailing newline is added. Since `n` is positive, no special handling is needed for zero or negative values. The main complexity is using `std::to_string` to convert the integer product to a string for concatenation. Time complexity is O(10) = O(1) since the loop always runs exactly 10 times. Space complexity is O(1) apart from the output string, which is O(number of characters) — minimal for small integers.
