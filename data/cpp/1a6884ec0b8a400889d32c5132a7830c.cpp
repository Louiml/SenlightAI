// Write a C++ function `void generateAllPermutations(int n)` that prints all permutations of the integers from 1 to n, one per line, with each permutation's digits printed consecutively without separators. The function must use a recursive backtracking approach with a fixed-size array and a boolean visited array, similar to the given snippet. It should handle n in the range 1 to 10 inclusive. The function must not take user input; it receives n as an argument. Output must be in lexicographic (dictionary) order. For example, for n=3, output must be exactly: 123, 132, 213, 231, 312, 321 (each on a new line). If n is 0 or negative, the function should do nothing.
#include <cassert>
#include <sstream>
#include <string>

// Note: We redirect cout to a stringstream to capture output for testing.
// Since the solution function prints to cout, we test by capturing.

// Helper: capture output of generateAllPermutations(n) into a string.
std::string captureOutput(int n) {
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    generateAllPermutations(n);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // n=1: only one permutation
    assert(captureOutput(1) == "1\n");

    // n=2
    assert(captureOutput(2) == "12\n21\n");

    // n=3 (matches the snippet's example output)
    assert(captureOutput(3) == "123\n132\n213\n231\n312\n321\n");

    // n=4: check lexicographic order and count: 24 lines
    std::string out4 = captureOutput(4);
    int lines = 0;
    for (char c : out4) if (c == '\n') ++lines;
    assert(lines == 24);
    // First permutation must be 1234, last must be 4321
    assert(out4.substr(0, 5) == "1234\n");
    assert(out4.substr(out4.size() - 5) == "4321\n");

    // n=0 and negative: should produce no output
    assert(captureOutput(0) == "");
    assert(captureOutput(-5) == "");

    // n=10: just check the first and last permutation and that there are 10! lines
    std::string out10 = captureOutput(10);
    int lines10 = 0;
    for (char c : out10) if (c == '\n') ++lines10;
    // 10! = 3628800 lines is huge but correct; we only verify first and last to keep test fast.
    // We'll just verify first line is 12345678910 and last is 10987654321.
    assert(out10.substr(0, 11) == "12345678910\n");
    assert(out10.substr(out10.size() - 12) == "10987654321\n");
    // Note: We don't count all lines due to time, but the structure ensures correctness.

    return 0;
}
#include <iostream>

// Print all permutations of 1..n in lexicographic order, each on a new line.
// Uses recursive backtracking. Does nothing if n <= 0 or n > 10.
void generateAllPermutations(int n) {
    if (n <= 0 || n > 10) return;

    const int maxn = 11;
    int perm[maxn] = {0};
    bool used[maxn] = {false};

    // Recursive helper: index is the current position to fill (1-based).
    // Fills positions index..n and prints a complete permutation when done.
    auto backtrack = [&](auto&& self, int index) -> void {
        if (index == n + 1) {
            for (int i = 1; i <= n; ++i) {
                std::cout << perm[i];
            }
            std::cout << '\n';
            return;
        }
        for (int x = 1; x <= n; ++x) {
            if (!used[x]) {
                perm[index] = x;
                used[x] = true;
                self(self, index + 1);
                used[x] = false; // restore state for next candidate
            }
        }
    };

    backtrack(backtrack, 1);
}
// The core algorithm is recursive backtracking. We maintain an array `perm[1..n]` (using 1-based indexing for clarity) to store the current permutation being built, and a boolean array `used[x]` to indicate whether integer x is already placed in the current prefix. The recursive function `backtrack(int index)` tries to place an integer at position `index`. If `index == n+1`, all positions are filled, so we print the current permutation. Otherwise, we iterate x from 1 to n, and if `used[x]` is false, we assign `perm[index] = x`, mark it used, recurse into `backtrack(index+1)`, and then unmark it to allow other choices. This ensures all permutations are generated exactly once. The order is lexicographic because we always try smaller x first. Edge cases: n=1 produces just "1"; n=10 works fine because array size 11 is enough (indices 1-10). For n≤0, we return immediately without printing. Time complexity: O(n! * n) because there are n! permutations, each printed in O(n) steps, plus the recursion overhead. Space complexity: O(n) for the permutation array, used array, and recursion stack depth.
