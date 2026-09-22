// You are given an integer `n`. Write a C++ function `std::string destination(int n)` that returns the string `"contest"` if `n` is odd, and `"home"` if `n` is even. The function must be pure, have no side effects, and accept the integer by value. This task is inspired by a simple parity-based decision, but you need to implement it as a standalone reusable function with proper type handling and const-correctness.
#include <cassert>
#include <string>

// Declaration of the function under test (from the solution above)
std::string destination(int n);

int main() {
    assert(destination(1) == "contest");
    assert(destination(2) == "home");
    assert(destination(0) == "home");
    assert(destination(-1) == "contest");
    assert(destination(-2) == "home");
    assert(destination(1000000) == "home");
    assert(destination(999999) == "contest");
    assert(destination(-2147483647) == "contest");
    assert(destination(2147483646) == "home");
    return 0;
}
#include <string>

// Returns "contest" if n is odd, "home" if n is even.
std::string destination(int n) {
    if ((n & 1) != 0) {  // Bitwise AND works for positive and negative odd numbers
        return "contest";
    }
    return "home";
}
// The solution is straightforward: check the parity of `n` using the modulo operator `n % 2`. In C++, `n % 2` returns `1` for odd integers and `0` for even positive integers. For negative odd integers, `n % 2` returns `-1` in most implementations (since the sign is preserved), which is still non-zero and evaluates to `true` in a boolean context. To be safe and portable, use `n % 2 != 0` or `(n & 1)` (bitwise AND) which works for all integers, including negatives, because the least significant bit is `1` for odd numbers regardless of sign. Edge cases include `n = 0` (even → "home"), negative odd numbers (→ "contest"), and large values within `int` range (no overflow risk). Time complexity is O(1), space complexity is O(1).
