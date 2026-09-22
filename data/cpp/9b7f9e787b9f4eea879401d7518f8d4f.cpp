Write a C++ function named `bacteriasAfterHours` that takes two integer parameters: the initial number of bacteria (`inicial`, a positive integer) and the number of hours (`horas`, a non-negative integer). The function must recursively compute and return the total number of bacteria after the given hours, assuming that every hour the bacterial population exactly doubles. For `horas == 0`, the function should return the initial count unchanged. The function must be defined without using loops, must respect `const` correctness for its parameters (use pass-by-value with `const` qualifiers), and must be self-contained with only the necessary headers. The function should handle large numbers that may exceed the range of `int`, so use `long long` for return type and intermediate computations.

The solution uses a simple recursion that mirrors the growth process: if `horas` is 0, the population is just the initial amount. Otherwise, one hour passes and the population doubles, so we call the function with `horas-1` and multiply the result by 2. This is a straightforward recursion that reduces the hour count by 1 each call, leading to a linear chain of calls. The time complexity is O(horas) because each hour requires one recursive call. The space complexity is O(horas) as well, due to the recursion stack that grows to depth `horas`. Edge cases include `horas == 0` (returns initial) and negative `horas`, which the function does not need to handle per specification, but if passed it would lead to infinite recursion; thus, we assume input is valid. For very large `horas` (e.g., 60 or more), the result may overflow even `long long`, but for reasonable inputs (like 24), it fits comfortably. The function is implemented with `const` parameters to indicate they are not modified.

#include <cstddef> // not needed, but include for completeness if needed

// Recursively compute the number of bacteria after a given number of hours.
// Population doubles every hour.
// Returns the initial count when horas == 0.
long long bacteriasAfterHours(const long long inicial, const int horas) {
    if (horas == 0) {
        return inicial;
    }
    return 2 * bacteriasAfterHours(inicial, horas - 1);
}

#include <cassert>

// The function is declared above (or included from header).
int main() {
    // Basic case: 0 hours returns initial count.
    assert(bacteriasAfterHours(1000, 0) == 1000);
    // Doubling for 1 hour.
    assert(bacteriasAfterHours(5, 1) == 10);
    // 24 hours starting from 1000 (same as snippet).
    assert(bacteriasAfterHours(1000, 24) == 16777216000LL);
    // Larger initial and hours.
    assert(bacteriasAfterHours(1, 10) == 1024);
    assert(bacteriasAfterHours(3, 3) == 24);
    // Ensure non-negative result for larger values.
    assert(bacteriasAfterHours(2, 30) == 2147483648LL);
    // Test with a large initial count.
    assert(bacteriasAfterHours(1000000, 5) == 32000000LL);
    // Test with 1 hour and odd initial.
    assert(bacteriasAfterHours(7, 1) == 14);
    // Test with multiple hours.
    assert(bacteriasAfterHours(10, 4) == 160);
    return 0;
}
