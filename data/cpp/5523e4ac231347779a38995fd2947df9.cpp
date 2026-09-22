Write a C++ function named `multiplicationTable` that takes a positive integer `N` as input and returns a `std::string` containing a formatted multiplication table for values from 1 to N. The table must have a header line `N\t10 * N\t100 * N\t1000 * N\n\n` followed by N rows, each row containing the current count, 10 times the count, 100 times the count, and 1000 times the count, separated by tabs and ending with a newline. If N is 0 or negative, return an empty string. The output must exactly match the format shown in the snippet (including the blank line after the header).

The solution iterates from 1 to N inclusive, generating each row as a string. For each integer `i`, we append `i`, a tab, `10*i`, a tab, `100*i`, a tab, `1000*i`, and a newline. The header is added first. We must handle the edge case where N ≤ 0 by returning an empty string. Because each row has a fixed structure, we can build the string using `std::to_string` for numbers and `\t` and `\n` literals. Time complexity is O(N) because we perform a constant amount of work per row. Space complexity is O(N * log10(i)) for the output string, but since the output size is proportional to N (with constant-width numbers), it's effectively O(N) for practical input sizes. No additional data structures are needed.

#include <string>

// Returns a formatted multiplication table for values 1 through N.
// Returns an empty string if N is not positive.
std::string multiplicationTable(int N) {
    if (N <= 0) {
        return "";
    }

    std::string result = "N\t10 * N\t100 * N\t1000 * N\n\n";

    for (int i = 1; i <= N; ++i) {
        result += std::to_string(i) + "\t" +
                  std::to_string(10 * i) + "\t" +
                  std::to_string(100 * i) + "\t" +
                  std::to_string(1000 * i) + "\n";
    }

    return result;
}

#include <cassert>
#include <string>

// Function declaration
std::string multiplicationTable(int N);

int main() {
    // N = 1: header + one row
    assert(multiplicationTable(1) == "N\t10 * N\t100 * N\t1000 * N\n\n1\t10\t100\t1000\n");
    
    // N = 2: header + two rows
    assert(multiplicationTable(2) == "N\t10 * N\t100 * N\t1000 * N\n\n1\t10\t100\t1000\n2\t20\t200\t2000\n");
    
    // N = 5: exact match with original snippet
    assert(multiplicationTable(5) == "N\t10 * N\t100 * N\t1000 * N\n\n1\t10\t100\t1000\n2\t20\t200\t2000\n3\t30\t300\t3000\n4\t40\t400\t4000\n5\t50\t500\t5000\n");
    
    // Edge case: N = 0 should return empty string
    assert(multiplicationTable(0) == "");
    
    // Edge case: negative N should return empty string
    assert(multiplicationTable(-3) == "");
    
    // Verify the row content for N = 3 manually
    std::string expected = "N\t10 * N\t100 * N\t1000 * N\n\n";
    expected += "1\t10\t100\t1000\n2\t20\t200\t2000\n3\t30\t300\t3000\n";
    assert(multiplicationTable(3) == expected);
    
    // N = 10: check last row
    std::string output = multiplicationTable(10);
    assert(output.size() > 0);
    // Check that row 10 exists and has the correct values
    assert(output.find("10\t100\t1000\t10000\n") != std::string::npos);
    
    return 0;
}
