Write a C++ function `int countWays(int n)` that returns the number of distinct ways a frog can climb a staircase with `n` steps if it can jump either 1 or 2 steps at a time. The order of jumps matters (e.g., 1+2 and 2+1 are different). Assume `n >= 0`. For `n = 0`, there is exactly one way (doing nothing). The function must avoid recursion to prevent exponential time, and must handle `n` up to at least 90 without overflow (the result fits in a 64–bit unsigned integer). Provide a clean iterative solution using dynamic programming with only O(1) extra space.
// This problem reduces to computing the (n+1)–th Fibonacci number (with the sequence starting F0=1, F1=1). Let `ways(i)` be the number of ways to reach step `i`. For `i=0` there is 1 way (stand still). For `i=1` there is 1 way (one 1‑step). For `i≥2`, the last jump can be either a 1‑step from `i‑1` or a 2‑step from `i‑2`, so `ways(i) = ways(i‑1) + ways(i‑2)`. This recurrence is identical to Fibonacci but shifted. We can compute iteratively using two variables to store the previous two values, updating them in a loop from `2` to `n`. Edge cases: `n=0` returns 1, `n=1` returns 1, `n=2` returns 2 (1+1, 2). Time complexity is O(n), space complexity is O(1) aside from the loop variable.
#include <cstdint>

// Returns the number of distinct ways a frog can climb n steps
// when each jump is either 1 or 2 steps. Order matters.
// The result is a Fibonacci-like number (F_{n+1} with F0=1, F1=1).
// Works for n up to at least 90 without overflow.
std::uint64_t countWays(int n) {
    if (n < 0) return 0;          // invalid input, return 0
    if (n == 0) return 1;         // one way: do nothing
    if (n == 1) return 1;         // one way: single 1-step

    std::uint64_t prev = 1;       // ways(0)
    std::uint64_t curr = 1;       // ways(1)
    for (int i = 2; i <= n; ++i) {
        std::uint64_t next = prev + curr;  // ways(i) = ways(i-1)+ways(i-2)
        prev = curr;
        curr = next;
    }
    return curr;
}
#include <cassert>
#include <cstdint>

int main() {
    // Base cases
    assert(countWays(0) == 1);
    assert(countWays(1) == 1);
    assert(countWays(2) == 2);
    // Small values: manually verified
    assert(countWays(3) == 3);  // 111, 12, 21
    assert(countWays(4) == 5);  // 1111, 112, 121, 211, 22
    assert(countWays(5) == 8);  // Fibonacci pattern
    // Larger values (Fibonacci sequence F_{n+1})
    assert(countWays(10) == 89);
    assert(countWays(20) == 10946);
    // Check overflow safety for a large n (F_91 = 4660046610375530309)
    assert(countWays(90) == 4660046610375530309ULL);
    return 0;
}
