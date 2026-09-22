/*
Write a C++ function named `printInvertedRightTriangle` that takes a positive integer `n` as input and returns a `std::string` containing an inverted right-angled triangle pattern made of asterisks (`*`), where the first line has exactly `n` asterisks, the second line has `n-1` asterisks, and so on, until the last line has exactly 1 asterisk. Each line must be terminated by a newline character (`\n`), and there must be no extra spaces or trailing characters. The function should handle the edge case where `n` is 1, producing a single `*` followed by a newline. If `n` is 0 or negative, the function should return an empty string (since no rows are needed). The function must not print anything to the console; it must only build and return the string.
*/

#include <string>

// Build an inverted right triangle of asterisks with n rows.
// Returns an empty string for n <= 0.
std::string printInvertedRightTriangle(int n) {
    if (n <= 0) {
        return "";
    }

    std::string result;
    for (int row = 0; row < n; ++row) {
        const int asteriskCount = n - row;
        result.append(asteriskCount, '*');
        result.push_back('\n');
    }
    return result;
}

#include <cassert>
#include <string>

std::string printInvertedRightTriangle(int n); // declaration for testing

int main() {
    // n=1: single star
    assert(printInvertedRightTriangle(1) == "*\n");
    
    // n=2: two rows: "**\n*\n"
    assert(printInvertedRightTriangle(2) == "**\n*\n");
    
    // n=3: three rows: "***\n**\n*\n"
    assert(printInvertedRightTriangle(3) == "***\n**\n*\n");
    
    // n=4: original problem pattern
    assert(printInvertedRightTriangle(4) == "****\n***\n**\n*\n");
    
    // n=5: five rows
    assert(printInvertedRightTriangle(5) == "*****\n****\n***\n**\n*\n");
    
    // n=0: empty
    assert(printInvertedRightTriangle(0) == "");
    
    // n=-3: empty
    assert(printInvertedRightTriangle(-3) == "");
    
    // Check length: for n=4, total characters = 4+3+2+1 + 4 newlines = 14
    assert(printInvertedRightTriangle(4).length() == 14);
    
    // Check first character is '*'
    assert(printInvertedRightTriangle(4).front() == '*');
    
    // Check last character is newline
    assert(printInvertedRightTriangle(4).back() == '\n');
}

// The solution builds the pattern line by line. For each row index `i` (starting from 0 to `n-1`), the number of asterisks is `n - i`. We append that many `'*'` characters to the result string using a loop or by using `std::string(count, '*')`, then append a `'\n'` character. This is straightforward; the main edge cases are `n == 1` (produces just `"*\n"`) and `n <= 0` (returns empty string). Time complexity is \(O(n^2)\) because the total number of characters produced is the sum of the first `n` positive integers, which is \(n(n+1)/2\). Space complexity is also \(O(n^2)\) due to the returned string size. The function uses constant extra auxiliary space beyond the output string.
