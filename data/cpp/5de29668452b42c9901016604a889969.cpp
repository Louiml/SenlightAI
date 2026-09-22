Create a C++ function that, given a positive integer `n`, prints (via `std::cout`) a specific pattern of numbers: for each `i` from 1 to `n`, output the number `i` followed by all numbers from `n` down to 1 in descending order, except that the current `i` is skipped (i.e., omitted from the descending sequence). Each line ends with a newline. The function should be `void printPattern(long long n)` and must handle all positive integer values within the `long long` range, including `n = 1` (which yields a single line "1 "). The output format must match exactly: each number followed by a single space, and a newline after the last number on each line. No extra spaces at line start, and the line ends after the final number’s space (i.e., there is a trailing space before the newline, as in the original snippet).

The solution directly simulates the nested loops from the provided snippet. For each `i` from 1 to `n`, print `i` and a space, then iterate `j` from `n` down to 1, but skip printing when `j == i`. This ensures that each line contains exactly `n` numbers: `i` plus the `n-1` descending values excluding `i`. Edge cases: when `n=1`, the inner loop runs once but skips because `j==i`, so the line becomes "1 " (one number plus trailing space). No integer overflow occurs because `i` and `j` are both within `[1, n]`. Complexity: The nested loops run `n` times externally and `n` times internally, giving O(n^2) time. Space usage is O(1) beyond the loop counters.

#include <iostream>

// Prints the described pattern for a given positive integer n.
void printPattern(long long n) {
    for (long long i = 1; i <= n; ++i) {
        std::cout << i << " ";
        for (long long j = n; j >= 1; --j) {
            if (j != i) {
                std::cout << j << " ";
            }
        }
        std::cout << "\n";
    }
}

#include <cassert>
#include <sstream>
#include <iostream>

// Forward declaration of the function to test.
void printPattern(long long n);

int main() {
    // Redirect cout to capture output for testing.
    std::ostringstream buffer;
    std::streambuf* oldCout = std::cout.rdbuf(buffer.rdbuf());

    // Test n = 1
    printPattern(1);
    assert(buffer.str() == "1 \n");
    buffer.str(""); buffer.clear();

    // Test n = 2
    printPattern(2);
    assert(buffer.str() == "1 2 \n2 1 \n");
    buffer.str(""); buffer.clear();

    // Test n = 3
    printPattern(3);
    assert(buffer.str() == "1 3 2 \n2 3 1 \n3 2 1 \n");
    buffer.str(""); buffer.clear();

    // Test n = 4
    printPattern(4);
    assert(buffer.str() == "1 4 3 2 \n2 4 3 1 \n3 4 2 1 \n4 3 2 1 \n");
    buffer.str(""); buffer.clear();

    // Test n = 5 (first and last lines)
    printPattern(5);
    assert(buffer.str() == "1 5 4 3 2 \n2 5 4 3 1 \n3 5 4 2 1 \n4 5 3 2 1 \n5 4 3 2 1 \n");
    buffer.str(""); buffer.clear();

    // Restore original cout
    std::cout.rdbuf(oldCout);
    std::cout << "All tests passed.\n";
    return 0;
}
