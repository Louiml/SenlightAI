// Write a C++ function named `drawNumberedTriangle` that takes an integer `n` (where `1 <= n <= 20`) and returns a `std::string` containing a right-angled triangle pattern. The triangle consists of `n` rows, where row `i` (starting from 1) contains exactly `i` asterisks (`*`). However, unlike a simple triangle, each asterisk is followed by a running count starting at 1 and incrementing by 1 for every asterisk printed overall (across all rows). The count number is printed immediately after the asterisk without any space, and each row is terminated with a newline character (`\n`). For example, for `n = 3`, the output should be `"*1\n*2*3\n*4*5*6\n"`. Your function must be efficient and not rely on global variables or external state.

The solution requires a nested loop structure: an outer loop iterating over rows from 1 to `n`, and an inner loop iterating from 1 to the current row index. A running counter (e.g., `counter`) is initialized to 1 and incremented after each asterisk–number pair is appended to the result string. For each cell in the inner loop, the asterisk and the current counter value are appended to the result, then the counter is incremented. After the inner loop completes, a newline character is appended. Key edge cases include `n = 1` (produces just `"*1\n"`), and the maximum `n = 20`, where the counter will reach up to 210 (the 20th triangular number) — so standard `int` is sufficient. The algorithm runs in \(O(n^2)\) time because the total number of asterisks is the sum of the first `n` integers, equal to \(n(n+1)/2\). The space complexity is \(O(n^2)\) for the returned string, with only \(O(1)\) additional auxiliary space for the counter and loop indices. The solution uses `std::to_string` to convert the counter to a string for concatenation.

#include <string>

// Returns a string containing a right-angled triangle of asterisks with a running count after each asterisk.
// Row i (1-indexed) has i asterisks, each followed by the next integer in the sequence starting from 1.
std::string drawNumberedTriangle(int n) {
    std::string result;
    result.reserve(static_cast<size_t>(n * (n + 1) / 2 * 3)); // heuristic pre-allocation
    
    int counter = 1;
    for (int row = 1; row <= n; ++row) {
        for (int col = 1; col <= row; ++col) {
            result += '*';
            result += std::to_string(counter);
            ++counter;
        }
        result += '\n';
    }
    return result;
}

#include <cassert>
#include <string>

// Declaration of the function to test (assumed to be included above)
std::string drawNumberedTriangle(int n);

int main() {
    assert(drawNumberedTriangle(1) == "*1\n");
    assert(drawNumberedTriangle(2) == "*1\n*2*3\n");
    assert(drawNumberedTriangle(3) == "*1\n*2*3\n*4*5*6\n");
    assert(drawNumberedTriangle(4) == "*1\n*2*3\n*4*5*6\n*7*8*9*10\n");
    assert(drawNumberedTriangle(5) == "*1\n*2*3\n*4*5*6\n*7*8*9*10\n*11*12*13*14*15\n");
    // Additional verification: ensure the last counter value for n=20 is 210
    std::string big = drawNumberedTriangle(20);
    // The last two characters before the final newline should be "210"
    size_t len = big.length();
    assert(big.substr(len - 4, 3) == "210");
    // Verify the total number of '*' characters equals 210
    int starCount = 0;
    for (char c : big) {
        if (c == '*') ++starCount;
    }
    assert(starCount == 210);
    return 0;
}
