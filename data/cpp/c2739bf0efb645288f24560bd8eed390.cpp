/*
Write a C++ function `blackjackResult` that takes three integers `a`, `b`, and `c` representing the point values of three cards in a game of Blackjack, and returns a string `"win"` if the sum of the three integers is strictly less than 22, otherwise returns `"bust"`. The function should handle any integer inputs, including negative numbers, zero, and large values (up to `INT_MAX`), and should not perform any input/output operations itself—only return the appropriate string based on the sum. The function must be reusable and side-effect free, and the returned string must be exactly `"win"` or `"bust"`.
*/
#include <string>

// Returns "win" if the sum of a, b, c is strictly less than 22, otherwise "bust".
// Uses long long to avoid integer overflow when summing large int values.
std::string blackjackResult(int a, int b, int c) {
    long long sum = static_cast<long long>(a) + b + c;
    return (sum < 22) ? "win" : "bust";
}
#include <cassert>
#include <string>

// Include the solution function here (or link against it)
std::string blackjackResult(int a, int b, int c) {
    long long sum = static_cast<long long>(a) + b + c;
    return (sum < 22) ? "win" : "bust";
}

int main() {
    // Basic winning case
    assert(blackjackResult(1, 2, 3) == "win");
    // Sum exactly 21 -> win
    assert(blackjackResult(10, 5, 6) == "win");
    // Sum exactly 22 -> bust
    assert(blackjackResult(10, 5, 7) == "bust");
    // Sum greater than 22 -> bust
    assert(blackjackResult(20, 10, 5) == "bust");
    // With zero and negative numbers
    assert(blackjackResult(0, -5, 10) == "win");        // sum = 5
    assert(blackjackResult(25, -3, -1) == "win");       // sum = 21
    assert(blackjackResult(30, -5, 0) == "bust");       // sum = 25
    // Large values that would overflow int if summed directly
    assert(blackjackResult(2000000000, 2000000000, 2000000000) == "bust"); // sum = 6000000000
    assert(blackjackResult(1000000000, 1000000000, 1000000000) == "bust"); // sum = 3000000000
    // All values zero
    assert(blackjackResult(0, 0, 0) == "win");          // sum = 0
    // Negative sum
    assert(blackjackResult(-10, -10, -10) == "win");    // sum = -30
    return 0;
}
// The problem is straightforward: compute the sum of three integers and compare it against the threshold of 22. The key conditions are: if `a + b + c < 22` return `"win"`; otherwise (i.e., if the sum is 22 or greater) return `"bust"`. There are no edge cases that require special handling beyond integer overflow—since `a`, `b`, and `c` are read as `int`, their sum may overflow. To avoid undefined behavior, the sum should be computed using a wider type such as `long long` (or `int64_t`). Since the inputs are each at most `INT_MAX` (~2.1e9), the sum of three such values could be up to ~6.4e9, which fits in a 64-bit signed integer. The time complexity is O(1) as we perform a constant number of arithmetic and comparison operations. Space complexity is O(1) since we only use a few local variables and return a string of constant size.
