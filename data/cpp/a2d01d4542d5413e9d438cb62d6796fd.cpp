Write a C++ function `classifyNumber(int n)` that returns a `std::string` containing `"even"` if the input integer is even, and `"odd"` otherwise. The function must handle negative integers, zero, and all representable `int` values correctly. Do not use any I/O inside the function; simply return the classification string. The function should be `const`‑correct and avoid unnecessary copies.
#include <cassert>
#include <string>

// Assume classifyNumber is declared above.
int main() {
    assert(classifyNumber(0) == "even");
    assert(classifyNumber(2) == "even");
    assert(classifyNumber(-4) == "even");
    assert(classifyNumber(1000000) == "even");
    assert(classifyNumber(1) == "odd");
    assert(classifyNumber(-3) == "odd");
    assert(classifyNumber(999) == "odd");
    assert(classifyNumber(-2147483647) == "odd"); // INT_MIN + 1
    assert(classifyNumber(2147483646) == "even"); // INT_MAX - 1
    return 0;
}
#include <string>

// Returns "even" if the given integer is even, "odd" otherwise.
// Works for negative, zero, and positive integers.
std::string classifyNumber(int n) {
    if (n % 2 == 0) {
        return "even";
    } else {
        return "odd";
    }
}
// The core of the solution is to test divisibility by 2 using the modulo operator `n % 2`. In C++, for any integer (including negative and zero), the remainder has the same sign as the dividend; however, `0 % 2` is `0` and `(-4) % 2` is `0`, so checking `n % 2 == 0` is safe for all values. Edge cases: `n = 0` → even, negative odd numbers (e.g., `-3 % 2` yields `-1`, which is not equal to `0`, so correctly classified as odd). No special handling is needed for overflow because the modulo operation is defined for all `int` values. Time complexity is O(1) and space complexity is O(1) (the returned string is constructed each time). The function only compares an integer and returns a constant string, so it is trivially efficient.
