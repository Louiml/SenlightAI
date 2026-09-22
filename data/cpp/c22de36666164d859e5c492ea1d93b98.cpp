// Write a C++ function called `printSequentialPairs` that takes no parameters and returns a `std::string` containing the exact text output produced by the provided code snippet (the loop over `i` from 0 to 20 in steps of 2). The function must replicate the output format precisely: for each `i`, three lines are generated, where `I` and `J` are computed as `i / 10` and `i / 10 + 1`, `i / 10 + 2`, `i / 10 + 3`, respectively. The formatting rules are: if `i` equals 0, 10, or 20, print the values with no decimal places (e.g., `I=0 J=1`); otherwise, print exactly one decimal place (e.g., `I=0.2 J=1.2`). The returned string should include a trailing newline after every line, exactly as `printf` would produce, and no extra spaces or characters beyond the pattern `I=x J=y\n` per line. The function must not print to the console; it must build and return the output as a `std::string` so that it can be tested programmatically. Ensure the implementation uses appropriate integer types and avoids floating-point comparison pitfalls.

#include <cassert>
#include <string>

// Function under test declared here (assuming it's provided)
std::string printSequentialPairs();

int main() {
    std::string result = printSequentialPairs();
    
    // Expected first few lines (manually verified from original snippet)
    assert(result.find("I=0 J=1\nI=0 J=2\nI=0 J=3\n") == 0);
    // Check a non-integer line
    assert(result.find("I=0.2 J=1.2\n") != std::string::npos);
    // Check middle integer line
    assert(result.find("I=1 J=2\n") != std::string::npos);
    // Check last line
    assert(result.find("I=2 J=5\n") != std::string::npos);
    // Count number of lines: should be 33
    int lineCount = 0;
    for (char c : result) if (c == '\n') ++lineCount;
    assert(lineCount == 33);
    // Verify total length is reasonable (11 iterations * 3 lines each, each ~15 chars)
    assert(result.length() > 300 && result.length() < 500);
    
    // Explicit full check for the first few lines and last line using exact string construction
    std::string expectedPrefix = 
        "I=0 J=1\n"
        "I=0 J=2\n"
        "I=0 J=3\n"
        "I=0.2 J=1.2\n"
        "I=0.2 J=2.2\n"
        "I=0.2 J=3.2\n";
    assert(result.substr(0, expectedPrefix.size()) == expectedPrefix);
    
    // Check that no extra spaces or unexpected characters appear
    for (char c : result) {
        assert(c == 'I' || c == '=' || c == 'J' || c == ' ' || 
               (c >= '0' && c <= '9') || c == '.' || c == '\n');
    }
    
    return 0;
}

#include <string>
#include <sstream>
#include <iomanip>

// Builds the exact output string as specified by the task.
// Replicates the loop over i from 0 to 20 in steps of 2, printing three lines per i.
// For i equal to 0, 10, or 20, uses "%.0f"; otherwise uses "%.1f".
std::string printSequentialPairs() {
    std::ostringstream out;
    
    // Loop over i from 0 to 20 inclusive, step 2
    for (int i = 0; i <= 20; i += 2) {
        // Determine if we need 0 or 1 decimal places
        bool integerFormat = (i == 0 || i == 10 || i == 20);
        
        // Base value i/10 as a double for precision
        double base = static_cast<double>(i) / 10.0;
        
        // Print three lines for offset 1,2,3
        for (int offset = 1; offset <= 3; ++offset) {
            double j = base + offset;
            
            // Set precision based on formatting rule
            out << std::fixed << std::setprecision(integerFormat ? 0 : 1);
            out << "I=" << base << " J=" << j << "\n";
        }
    }
    
    return out.str();
}

// The solution replicates the given code's logic but instead of printing directly, it accumulates the formatted output into a `std::string`. The main challenge is to reproduce the exact formatting: using `%.0f` for `i` equal to 0, 10, or 20, and `%.1f` otherwise. A straightforward approach is to loop over `int i = 0; i <= 20; i += 2` (the original uses `int16_t`, but `int` is fine in C++ for this range). For each `i`, compute the base value `base = i / 10.0f` (as a float). Then for `offset` from 1 to 3, compute `j = base + offset`. For formatting, we can use `std::ostringstream` with stream manipulators: if `i` is 0, 10, or 20, set `std::fixed << std::setprecision(0)` to print no decimals; otherwise `std::fixed << std::setprecision(1)` to print one decimal. However, note that `%.0f` doesn't print a decimal point (e.g., "1" not "1.0"), whereas `std::fixed << std::setprecision(0)` does exactly that (prints "1"). For `%.1f`, it prints "1.0" which matches `fixed << setprecision(1)`. An alternative is to use `char buffer[128]` and `snprintf` to mimic the original exactly, but using `std::ostringstream` is cleaner. Edge cases: when `i = 10`, `base = 1.0`, and `j` values are 2.0, 3.0, 4.0, all printed as `I=1 J=2`, etc., correctly. For `i = 20`, `base = 2.0`, `j` = 3.0, 4.0, 5.0. For other `i`, like `i=2`, `base=0.2`, `j` = 1.2, 2.2, 3.2. The loop produces 11 iterations (0,2,4,...,20), each generating 3 lines, so the output string has 33 lines. Time complexity is O(1) because the loop has a fixed number of iterations (11*3 = 33 operations). Space complexity is O(1) for the output string because its length is bounded (each line is short, total length constant ~ 33*15 bytes). The code must include necessary headers `<string>`, `<sstream>`, `<iomanip>`.
