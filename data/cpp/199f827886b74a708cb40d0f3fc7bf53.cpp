Write a C++ function `decideWinner(int a, int b)` that takes two non-negative integers `a` and `b`, representing the number of stones in two piles, and returns a string `"Alice"` if the total number of stones `(a + b)` is odd, otherwise returns `"Bob"` if the total is even. The function should handle any non-negative integer values, including zero, and must not use any input or output operations. This task is inspired by a simple game where players alternate turns and the winner is determined solely by the parity of the total stone count.
// The problem reduces to checking whether the sum of two integers is even or odd. If the sum is even, Bob wins; if odd, Alice wins. The main algorithm is straightforward: compute `sum = a + b`, then check `sum % 2 == 0`. Since the input constraints are non-negative integers, the sum fits comfortably within a 64-bit signed integer (use `long long` or `int64_t` to avoid overflow with large inputs). Edge cases: both `a` and `b` being zero yields an even sum (0) → "Bob". Also, very large values near the maximum of `long long` must be handled safely; using `long long` for the sum avoids overflow because adding two non-negative `long long` values could overflow if both are near `LLONG_MAX`, but typical constraints keep values within 32-bit range. Time complexity is O(1) and space complexity is O(1).
#include <string>

// Returns "Alice" if the total number of stones is odd, otherwise "Bob".
std::string decideWinner(long long a, long long b) {
    const long long total = a + b;
    if (total % 2 == 0) {
        return "Bob";
    }
    return "Alice";
}
#include <cassert>
#include <string>

// Function under test
std::string decideWinner(long long a, long long b);

int main() {
    // Test basic odd and even sums
    assert(decideWinner(1, 2) == "Alice");   // 3 is odd
    assert(decideWinner(2, 2) == "Bob");     // 4 is even
    assert(decideWinner(0, 0) == "Bob");     // 0 is even
    assert(decideWinner(0, 1) == "Alice");   // 1 is odd

    // Test larger values
    assert(decideWinner(1000000000LL, 1000000000LL) == "Bob"); // 2e9 even
    assert(decideWinner(999999999LL, 1LL) == "Bob");           // 1e9 even
    assert(decideWinner(123456789LL, 987654321LL) == "Bob");   // sum even

    // Test edge with maximum 32-bit ints
    assert(decideWinner(2147483647LL, 0LL) == "Alice"); // odd
    assert(decideWinner(2147483647LL, 1LL) == "Bob");   // even

    return 0;
}
