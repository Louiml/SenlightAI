Write a C++ function named `printInvertedTriangleNumbers` that takes an integer `n` as input (where `n >= 1`) and returns a string representing the pattern produced by the given code snippet, but generalized for any positive `n`. The output string should contain lines separated by `'\n'`, where each line starts with the appropriate number of double spaces (`"  "`) followed by the numbers from `n` down to the current row value, each separated by a single space and with a trailing space after the last number. For example, for `n = 5`, the returned string must exactly match the output of the original code:  
```
       5 
      4 5 
     3 4 5 
    2 3 4 5 
   1 2 3 4 5 
```
Note the indentation: the first row has 4 double-spaces (i.e., 8 spaces), then `5 `; the second row has 3 double-spaces then `4 5 `; etc. The function must work for any positive integer `n` and should not print to the console—it must return the string so it can be tested. Handle edge cases like `n = 1`, where the output is just `"1 \n"`.

The pattern consists of `n` rows. For row `i` (where `i` goes from `n` down to `1`), there are `i-1` leading double-spaces (each `"  "`), followed by the numbers from `n` down to `i` (inclusive), each followed by a space. The main algorithm is to loop `i` from `n` down to `1`. For each `i`, first append `i-1` copies of `"  "` to the result string, then append each number from `j = n` down to `j = i` in the form `std::to_string(j) + " "`. After the inner loop, append a newline `'\n'` (but not an extra trailing newline after the last row—the original code's `endl` adds a newline after each row, so the string ends with a newline after the last row too). Edge case: `n = 1` produces no leading spaces and just `"1 \n"`. Time complexity is `O(n^2)` because the total number of numbers printed is `n + (n-1) + ... + 1 = n(n+1)/2`, and the leading spaces also total `O(n^2)`. Space complexity is `O(n^2)` because the resulting string size is proportional to the total output. The approach uses a `std::string` accumulator for efficiency, avoiding repeated output stream calls.

#include <string>

// Returns a string containing the inverted triangle number pattern for given n.
// Pattern for n=5: row1 has 4 leading "  " then "5 ", row2 has 3 leading "  " then "4 5 ", etc.
// The string ends with a newline after the last row.
std::string printInvertedTriangleNumbers(int n) {
    std::string result;
    // Reserve a rough estimate to avoid reallocation (optional).
    result.reserve(static_cast<size_t>(n * n * 3));
    
    for (int i = n; i >= 1; --i) {
        // Leading spaces: i-1 double spaces
        for (int k = 1; k < i; ++k) {
            result += "  ";
        }
        // Numbers from n down to i
        for (int j = n; j >= i; --j) {
            result += std::to_string(j);
            result += ' ';
        }
        result += '\n';
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the function being tested.
std::string printInvertedTriangleNumbers(int n);

int main() {
    // Test n = 1 (single row, no leading spaces)
    assert(printInvertedTriangleNumbers(1) == "1 \n");
    
    // Test n = 2
    assert(printInvertedTriangleNumbers(2) == "  2 \n1 2 \n");
    
    // Test n = 3
    assert(printInvertedTriangleNumbers(3) == "    3 \n  2 3 \n1 2 3 \n");
    
    // Test n = 4
    assert(printInvertedTriangleNumbers(4) == "      4 \n    3 4 \n  2 3 4 \n1 2 3 4 \n");
    
    // Test n = 5 (original snippet output)
    assert(printInvertedTriangleNumbers(5) ==
           "        5 \n"
           "      4 5 \n"
           "    3 4 5 \n"
           "  2 3 4 5 \n"
           "1 2 3 4 5 \n");
    
    // Test a larger n to ensure no off-by-one, e.g., n=6 first/last line
    std::string result6 = printInvertedTriangleNumbers(6);
    // First line: 5 leading double-spaces then "6 \n"
    assert(result6.compare(0, 2*5 + 2, "          6 \n") == 0);
    // Last line: no leading spaces then "1 2 3 4 5 6 \n"
    size_t pos = result6.find_last_of('\n', result6.size() - 2);
    std::string lastLine = result6.substr(pos + 1);
    assert(lastLine == "1 2 3 4 5 6 \n");
    
    // Test that the total number of newline characters equals n
    int newlineCount = 0;
    for (char c : result6) if (c == '\n') ++newlineCount;
    assert(newlineCount == 6);
    
    return 0;
}
