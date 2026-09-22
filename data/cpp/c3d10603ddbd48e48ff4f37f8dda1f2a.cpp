// Write a C++ function that takes two integers `n` and `m` (with `1 ≤ n ≤ m ≤ 1000`) and returns a `std::string` containing all integers from `n` to `m` inclusive, in increasing order, separated by single spaces, with no trailing space at the end. For example, for `n=3, m=6`, the function must return `"3 4 5 6"`. The function should handle the trivial case where `n == m` by returning just that single number as a string. The function signature is `std::string printRange(int n, int m)`.

// The solution is straightforward: iterate from `n` to `m` and append each number converted to a string to a `std::ostringstream`. To avoid a trailing space, append a space before every number except the first one, or append the numbers with spaces and then remove the final character. The simplest approach is to use a loop that writes each number followed by a space only if it is not the last element. Alternatively, build a vector and use `std::ostringstream` manually. Edge cases include `n == m` (returns just that number) and when `m - n` is large, but within constraints it is safe. Time complexity is `O(m - n)` for the number of integers, and auxiliary space is `O(m - n)` for the output string itself (plus constant overhead). No special handling needed for negative numbers because input is positive per constraints.

#include <string>
#include <sstream>

// Return a string with integers from n to m inclusive, space-separated.
std::string printRange(int n, int m) {
    std::ostringstream out;
    for (int i = n; i <= m; ++i) {
        if (i > n) {
            out << ' ';
        }
        out << i;
    }
    return out.str();
}

#include <cassert>
#include <string>

// Declaration of the function under test
std::string printRange(int n, int m);

int main() {
    assert(printRange(1, 5) == "1 2 3 4 5");
    assert(printRange(5, 5) == "5");
    assert(printRange(10, 12) == "10 11 12");
    assert(printRange(7, 9) == "7 8 9");
    assert(printRange(100, 100) == "100");
    assert(printRange(1, 1) == "1");
    assert(printRange(3, 6) == "3 4 5 6");
    assert(printRange(2, 4) == "2 3 4");
    assert(printRange(8, 10) == "8 9 10");
    assert(printRange(0, 0) == "0"); // edge though constraints say >=1
    return 0;
}
