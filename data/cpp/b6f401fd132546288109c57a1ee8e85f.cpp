/*
Write a C++ function named `printDescendingTriangle` that takes a positive integer `n` representing the number of lines and returns a `std::string` containing a triangular pattern. For each line `i` from 1 to `n`, the line must contain exactly `i` numbers separated by a tab character, where the numbers on that line are the descending sequence starting from `n` down to `n - i + 1`. Each line ends with a newline character. The function must not print to the console; it must build and return the complete string. For example, if `n = 3`, the returned string is `"3\n3\t2\n3\t2\t1\n"`. The function should handle edge cases gracefully: if `n` is 0 or negative, return an empty string. The function should be `const`-correct and use only standard library facilities.
*/

#include <string>
#include <sstream>

// Build a descending triangle pattern for given positive n.
// Returns an empty string for n <= 0.
// For n = 3, returns "3\n3\t2\n3\t2\t1\n".
std::string printDescendingTriangle(int n) {
    if (n <= 0) {
        return "";
    }
    std::ostringstream out;
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            out << (n - j + 1);
            if (j < i) {
                out << '\t';
            } else {
                out << '\n';
            }
        }
    }
    return out.str();
}

#include <cassert>
#include <string>

// Function declaration
std::string printDescendingTriangle(int n);

int main() {
    assert(printDescendingTriangle(1) == "1\n");
    assert(printDescendingTriangle(2) == "2\n2\t1\n");
    assert(printDescendingTriangle(3) == "3\n3\t2\n3\t2\t1\n");
    assert(printDescendingTriangle(4) == "4\n4\t3\n4\t3\t2\n4\t3\t2\t1\n");
    assert(printDescendingTriangle(0) == "");
    assert(printDescendingTriangle(-5) == "");
    assert(printDescendingTriangle(5) == "5\n5\t4\n5\t4\t3\n5\t4\t3\t2\n5\t4\t3\t2\t1\n");
    return 0;
}

// The solution builds the output string line by line. The main algorithm uses nested loops: an outer loop iterates from 1 to `n` (line number), and for each line, an inner loop iterates from 1 to `i` (number of elements on that line). For each inner iteration, we append `n - j + 1` to the string, where `j` is the inner loop index, followed by a tab character (`\t`) except after the last element of the line where we append a newline (`\n`) instead. Edge cases: if `n <= 0`, return an empty string immediately. Positive integers are handled naturally. The time complexity is O(n^2) because the total number of printed numbers is the sum of integers from 1 to n (n(n+1)/2), and each operation is constant time. The space complexity is O(n^2) in the worst case due to the string size, but the auxiliary space is O(1) if we exclude the output string itself.
