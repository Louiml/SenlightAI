/*
Write a C++ function that takes an integer `n` as input and returns a string containing the numbers from 0 to `n` inclusive, separated by a single space, with no trailing space. The function must handle non-negative integers, including `n = 0` (which returns just "0"). The output format should match the behavior of the given snippet: for each line, it prints numbers 0 through `n-1` each followed by a space, then prints `n` without a trailing space (the snippet has a bug with `else if` missing a condition, but the intended behavior is to print 0..n with no trailing space).
*/
#include <string>
#include <sstream>

// Build a string containing numbers from 0 to n inclusive, separated by single spaces,
// with no trailing space after the last number.
std::string buildSequence(int n) {
    std::ostringstream out;
    for (int i = 0; i <= n; ++i) {
        if (i < n) {
            out << i << ' ';
        } else {
            out << i;
        }
    }
    return out.str();
}
#include <cassert>
#include <string>

// (include the solution function here or link to it)

int main() {
    // The solution function is called directly; asserting output strings.
    assert(buildSequence(0) == "0");
    assert(buildSequence(1) == "0 1");
    assert(buildSequence(2) == "0 1 2");
    assert(buildSequence(5) == "0 1 2 3 4 5");
    assert(buildSequence(10) == "0 1 2 3 4 5 6 7 8 9 10");
    // Additional edge: large n to ensure no trailing space (check last char)
    std::string resultLarge = buildSequence(100);
    assert(resultLarge.size() > 0);
    assert(resultLarge.back() != ' ');
    // Check that first number is '0' and last number is "100" (with a space before it except when n=0)
    assert(resultLarge.find(" 100") != std::string::npos);
}
// The core algorithm is straightforward: loop from 0 to `n` inclusive. For each number from 0 to `n-1`, append the number followed by a single space to the result string. For the last number `n`, append just the number without a space. This directly matches the intended output of the original code. Edge cases: when `n = 0`, the loop runs once (i=0) and since 0 is not less than 0, it appends just "0" — no space. The function should return the string, and because it's a standalone function, the caller is responsible for printing. Time complexity is O(n) because we append each number once. Space complexity is O(n) because the result string holds all characters (approximately 2n characters for typical digit lengths plus spaces). The function signature should take `int n` and return `std::string`, and we should use a `std::ostringstream` or simple string concatenation (though the former is more efficient).
