Write a C++ function that takes a positive integer `n` and returns a `std::string` containing a right-justified triangular number pattern, where each row contains consecutive integers starting from 1, and the total number of values printed is `n*(n+1)/2`. The pattern should have row `i` (1-indexed) containing exactly `i` integers, each followed by a single space if it is not the last value in the row, with each row terminated by a newline character (`\n`). The function must handle `n` up to at least 1000, must not use any output statements, and must be `const`-correct with respect to its parameter.
#include <cassert>
#include <string>

// Declare the function for testing
std::string generateTriangularPattern(int n);

int main() {
    // n=1: single number
    assert(generateTriangularPattern(1) == "1\n");
    
    // n=2: two rows
    assert(generateTriangularPattern(2) == "1\n2 3\n");
    
    // n=3: three rows
    assert(generateTriangularPattern(3) == "1\n2 3\n4 5 6\n");
    
    // n=4: four rows, check consecutive numbering
    assert(generateTriangularPattern(4) == "1\n2 3\n4 5 6\n7 8 9 10\n");
    
    // n=5: five rows, ensure spaces only between numbers in a row
    assert(generateTriangularPattern(5) == "1\n2 3\n4 5 6\n7 8 9 10\n11 12 13 14 15\n");
    
    // Edge case: n=0 (should be an empty string? But function accepts positive n only; if passed 0, loop doesn't run)
    assert(generateTriangularPattern(0) == "");
    
    // Edge case: single row with multiple numbers (n=1 already checked) – also test that no trailing space exists
    assert(generateTriangularPattern(1).find(' ') == std::string::npos);
    
    // Test that each row has the correct length (row count + 1 for newline)
    assert(generateTriangularPattern(3).size() == 8); // "1\n" (2) + "2 3\n" (4) + "4 5 6\n" (7) = total 13? Actually compute: 2+4+7=13? Let's verify: "1\n"=2, "2 3\n"=4, "4 5 6\n"=7, sum=13
    assert(generateTriangularPattern(3).size() == 13);
    
    // Test that the pattern ends with a newline
    assert(generateTriangularPattern(3).back() == '\n');
    
    return 0;
}
#include <string>
#include <to_string>

// Generate a right-justified triangular pattern of consecutive integers.
// Row i contains exactly i numbers, starting from 1, with spaces between them.
// Each row ends with a newline character. For n=3, the output is:
// 1\n2 3\n4 5 6\n
std::string generateTriangularPattern(int n) {
    std::string result;
    int val = 1;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            result += std::to_string(val);
            if (j < i) {
                result += ' ';
            }
            ++val;
        }
        result += '\n';
    }
    return result;
}
// The task is to generate a triangular pattern of consecutive integers. The algorithm initializes a `std::string` result, an integer counter `val=1`, and loops from `i=1` to `n`. For each row `i`, it appends exactly `i` integers: for each position `j` from 1 to `i`, it appends `std::to_string(val)` and, if `j < i`, appends a space after the value (but not after the last value in the row). After finishing a row, it appends a newline character. The counter `val` increments after every number printed, ensuring consecutive numbering across rows. Edge cases include `n=1` (only one row, one number, no trailing space before newline) and large `n` where the total number of digits grows (the string length is roughly `O(n^2 log n)` in terms of digits, but since `n` is limited to 1000, this is fine). Time complexity is `O(n^2)` because we print `1+2+...+n = n(n+1)/2` numbers, and each conversion to string is constant time relative to the number's magnitude. Space complexity is `O(n^2)` for the resulting string, as it stores all characters.
