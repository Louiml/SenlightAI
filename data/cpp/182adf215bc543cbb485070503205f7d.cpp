Write a C++ function that reads a single integer `n` from standard input, then on the next line reads `n` space-separated integers, and returns the sum of all integers that are divisible by both 2 and 3 (i.e., divisible by 6). If no such integers exist, return 0. The function must handle `n = 0` gracefully (returning 0) and must not read anything else from standard input beyond exactly `n` numbers. The function signature should be `int sumDivisibleBy6()`, and it should perform all input reading itself from `std::cin` (which is already synchronized with C I/O and untied from `cout` in the driver). The input is guaranteed to contain at least the line with `n`, and if `n > 0`, exactly `n` integers on the next line, but the integers can be any 32-bit signed values including negatives. The function must return the sum, which fits in a 32-bit signed integer.
The algorithm is straightforward: read `n` from standard input using `std::cin`. If `n` is less than or equal to 0, return 0 without attempting further reads (this avoids issues with empty input or negative counts). Otherwise, loop exactly `n` times, reading one integer at a time. For each integer, check if it is divisible by 6 using the modulo operator (`value % 6 == 0`). Since division by 6 is only possible if the number is divisible by both 2 and 3, this condition is equivalent. Accumulate the sum of qualifying numbers. Edge cases include: `n=0` (return 0 immediately), negative values (e.g., `-6` is divisible by 6 and should be added), and all numbers not divisible by 6 (sum remains 0). For `n` integers, time complexity is O(n) and space complexity is O(1) because we only store the running sum and the current value. No extra containers are needed.
#include <iostream>

// Reads n and then n integers from standard input.
// Returns the sum of all integers divisible by 6.
int sumDivisibleBy6() {
    int n;
    if (!(std::cin >> n)) {
        // If reading fails, treat as no input.
        return 0;
    }

    int sum = 0;
    for (int i = 0; i < n; ++i) {
        int value;
        std::cin >> value;
        if (value % 6 == 0) {
            sum += value;
        }
    }
    return sum;
}
#include <cassert>
#include <sstream>
#include <iostream>

// Forward declaration of the function under test.
int sumDivisibleBy6();

int main() {
    // Test 1: n=0, nothing to sum.
    {
        std::istringstream input("0\n");
        std::cin.rdbuf(input.rdbuf());
        assert(sumDivisibleBy6() == 0);
    }

    // Test 2: n=1, one divisible number.
    {
        std::istringstream input("1\n6\n");
        std::cin.rdbuf(input.rdbuf());
        assert(sumDivisibleBy6() == 6);
    }

    // Test 3: n=3, mixed numbers.
    {
        std::istringstream input("3\n12 5 -6\n");
        std::cin.rdbuf(input.rdbuf());
        assert(sumDivisibleBy6() == 6); // 12 + (-6) = 6
    }

    // Test 4: n=4, no divisible numbers.
    {
        std::istringstream input("4\n1 2 3 4\n");
        std::cin.rdbuf(input.rdbuf());
        assert(sumDivisibleBy6() == 0);
    }

    // Test 5: n=2, negative and zero (zero is divisible by 6).
    {
        std::istringstream input("2\n-12 0\n");
        std::cin.rdbuf(input.rdbuf());
        assert(sumDivisibleBy6() == -12);
    }

    // Test 6: n=1, large value.
    {
        std::istringstream input("1\n1000000\n");
        std::cin.rdbuf(input.rdbuf());
        assert(sumDivisibleBy6() == 0); // 1000000 not divisible by 6
    }

    // Test 7: n=2, both divisible.
    {
        std::istringstream input("2\n18 24\n");
        std::cin.rdbuf(input.rdbuf());
        assert(sumDivisibleBy6() == 42);
    }

    // Test 8: n=5, include 6 and 12, ignore others.
    {
        std::istringstream input("5\n6 7 12 13 18\n");
        std::cin.rdbuf(input.rdbuf());
        assert(sumDivisibleBy6() == 36); // 6+12+18
    }

    // Test 9: n=1, negative divisible.
    {
        std::istringstream input("1\n-6\n");
        std::cin.rdbuf(input.rdbuf());
        assert(sumDivisibleBy6() == -6);
    }

    // Test 10: n=3, all zero.
    {
        std::istringstream input("3\n0 0 0\n");
        std::cin.rdbuf(input.rdbuf());
        assert(sumDivisibleBy6() == 0); // zero is divisible but sum is 0
    }

    return 0;
}
