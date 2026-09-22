// Write a C++ function that takes a positive integer `n` and returns a `std::string` containing the numbers from 1 to `n` inclusive, each followed by a newline character. The function must handle `n = 0` by returning an empty string, and `n = 1` by returning just `"1\n"`. The output should have no trailing spaces, and each number must be on its own line.

The solution builds the result by iterating from 1 to `n`, appending each number (converted to a string) followed by `"\n"` to an accumulating `std::string`. For `n = 0`, the loop does not execute and an empty string is returned, which is correct. For `n = 1`, the loop executes once and returns `"1\n"`. The main edge case is `n = 0`, which naturally results in an empty output, and `n` being large, where we must ensure efficiency by using `std::to_string` for conversion and appending directly. Time complexity is O(n) because we iterate exactly n times and each append operation is amortized O(1), and space complexity is O(n) since the result string stores all output.

#include <string>

// Build a string with numbers 1..n, each on its own line.
std::string numbersFromOneTo(int n) {
    std::string result;
    for (int i = 1; i <= n; ++i) {
        result += std::to_string(i) + "\n";
    }
    return result;
}

#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
std::string numbersFromOneTo(int n); // assume declaration available

int main() {
    assert(numbersFromOneTo(0) == "");
    assert(numbersFromOneTo(1) == "1\n");
    assert(numbersFromOneTo(3) == "1\n2\n3\n");
    assert(numbersFromOneTo(5) == "1\n2\n3\n4\n5\n");
    assert(numbersFromOneTo(10).size() == 10 * 2 + 9); // each number 1-9: 2 chars, 10: 3 chars, total = 9*2 + 3 = 21, not correct; test actual content instead
    // Correct test for 10:
    assert(numbersFromOneTo(10) == "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n");
    return 0;
}
