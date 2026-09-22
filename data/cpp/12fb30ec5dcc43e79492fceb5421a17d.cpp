Write a C++ function `std::string nextTenQuadrangularOrMessage(int n)` that takes a positive integer `n` and returns a string. If `n` is a quadrangular number (also called a perfect square: 1, 4, 9, 16, ...), the function must return a string listing the next 10 quadrangular numbers after `n`, separated by commas, with a period after the last one, all in the exact format: `"Proximos: 25, 36, 49, 64, 81, 100, 121, 144, 169, 196."` (adjust numbers accordingly). If `n` is not a quadrangular number, return the exact message `"Nao eh quadrangular"`. The function must handle edge cases like `n = 1` (which has a square root of 1) and very large numbers (use `long long` internally to avoid overflow when computing squares of values larger than `sqrt(INT_MAX)`). Do not output any prompt or extra text; return exactly the described string.

#include <cassert>
#include <string>
#include <iostream>

// Declare the function under test (the solution is provided separately)
std::string nextTenQuadrangularOrMessage(int n);

int main() {
    // Test non-quadrangular numbers
    assert(nextTenQuadrangularOrMessage(2) == "Nao eh quadrangular");
    assert(nextTenQuadrangularOrMessage(3) == "Nao eh quadrangular");
    assert(nextTenQuadrangularOrMessage(0) == "Nao eh quadrangular");
    assert(nextTenQuadrangularOrMessage(-5) == "Nao eh quadrangular");

    // Test n=1 (smallest quadrangular)
    assert(nextTenQuadrangularOrMessage(1) == "Proximos: 4, 9, 16, 25, 36, 49, 64, 81, 100, 121.");

    // Test n=4
    assert(nextTenQuadrangularOrMessage(4) == "Proximos: 9, 16, 25, 36, 49, 64, 81, 100, 121, 144.");

    // Test n=25
    assert(nextTenQuadrangularOrMessage(25) == "Proximos: 36, 49, 64, 81, 100, 121, 144, 169, 196, 225.");

    // Test n=100
    assert(nextTenQuadrangularOrMessage(100) == "Proximos: 121, 144, 169, 196, 225, 256, 289, 324, 361, 400.");

    // Test a large perfect square (e.g., 2147395600 = 46340^2, near INT_MAX)
    assert(nextTenQuadrangularOrMessage(2147395600) == "Proximos: 2147483649, 2147490884, 2147498121, 2147505360, 2147512601, 2147519844, 2147527089, 2147534336, 2147541585, 2147548836.");

    // Test a large non-square near INT_MAX
    assert(nextTenQuadrangularOrMessage(2147483647) == "Nao eh quadrangular");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

#include <string>
#include <sstream>
#include <cmath>

// Returns a string describing the next 10 quadrangular numbers after n,
// or a message if n is not quadrangular.
std::string nextTenQuadrangularOrMessage(int n) {
    if (n < 1) {
        return "Nao eh quadrangular";
    }

    long long root = static_cast<long long>(std::sqrt(static_cast<double>(n)));
    // Adjust for potential floating-point inaccuracies
    while ((root + 1) * (root + 1) <= n) ++root;
    while (root * root > n) --root;

    if (root * root != static_cast<long long>(n)) {
        return "Nao eh quadrangular";
    }

    std::ostringstream oss;
    oss << "Proximos: ";
    for (int i = 1; i <= 10; ++i) {
        long long nextRoot = root + i;
        long long square = nextRoot * nextRoot;
        oss << square;
        if (i < 10) {
            oss << ", ";
        } else {
            oss << ".";
        }
    }
    return oss.str();
}

// The main algorithm: To check if `n` is a perfect square, compute the integer square root using `static_cast<long long>(std::sqrt(n))` (or iterate from 1 up to `sqrt(n)`). If `root * root == n`, it's quadrangular. Then to find the next 10 squares, start from `root + 1` and square each value, appending to a string. Use `std::ostringstream` for efficient concatenation. Edge cases: `n = 1` works because root = 1; `n = 0` is not quadrangular per the problem definition (since quadrangular numbers start at 1); negative numbers are not quadrangular. For very large `n` up to `INT_MAX`, the next square after it might exceed `INT_MAX`, so use `long long` for multiplication. Time complexity: O(1) for the square root check (using `sqrt` which is constant time) and O(10) for generating the next 10 squares, so overall O(1). Space complexity: O(1) auxiliary (excluding the returned string length which is fixed).
