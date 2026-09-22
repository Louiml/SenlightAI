// Write a C++ function `determineWinner` that takes a vector of positive integers (length at least 1) and returns a string `"Alice"` or `"Bob"` based on the following game rule: The player who takes the last turn wins. The game is played by repeatedly subtracting the greatest common divisor (GCD) of all current numbers from one of the numbers, reducing that number, and continuing until all numbers become zero. However, instead of simulating the game, determine the winner using this mathematical shortcut: Let `g` be the GCD of all numbers, and `m` be the maximum number. If `((m / g) - n) % 2 == 1` (where `n` is the number of elements), Alice wins; otherwise Bob wins. Edge case: all numbers are divisible by `g`, so `m / g` is an integer. The function must be `const`-correct and must not modify the input.

The solution computes the GCD of all numbers using the Euclidean algorithm iteratively, starting with the first element. Then it finds the maximum element. The key insight is that the game is equivalent to a Nim-like game where the total number of moves is `(m / g) - n` because each move reduces the sum of all numbers by `g`, and the final state has all zeros. The total moves determine the winner: if odd, Alice moves first and wins; if even, Bob wins. Edge cases: a single element → compute `m/g - 1`; if that is 0, Bob wins (since Alice cannot move). All elements equal → `m/g = 1`, so `1 - n` may be negative; however, GCD of all would be that value, so `m/g = 1` and `1 - n <= 0`, but since `(negative % 2)` in C++ is implementation-defined, we must handle by taking absolute or ensuring positive. Actually, since `m/g >= 1` and `n >= 1`, `m/g - n` can be negative, but modulo on negative numbers in C++ yields a negative remainder, so we need to normalize: use `((m/g - n) % 2 + 2) % 2 == 1`. Alternatively, note that `m/g - n` has the same parity as `(m/g - n)` regardless of sign, but to be safe, compute `((m/g - n) % 2 != 0)` using the absolute value or just check `((m/g - n) & 1)` which works for negative two's complement (since -3 & 1 = 1). Simplest: use `((m/g - n) % 2 != 0)` but in C++ negative modulo yields negative, so `-3 % 2` is -1, not 1, so `!= 0` works. Time complexity: O(n log max) for GCD computation, O(n) for max, so O(n log max). Space: O(1) auxiliary.

#include <vector>
#include <numeric>
#include <algorithm>

// Compute the winner based on the GCD and maximum.
// Returns "Alice" if ((max/gcd - n) % 2) is odd, otherwise "Bob".
std::string determineWinner(const std::vector<int>& numbers) {
    int n = static_cast<int>(numbers.size());
    int g = numbers[0];
    for (int i = 1; i < n; ++i) {
        g = std::gcd(g, numbers[i]);
    }
    int m = *std::max_element(numbers.begin(), numbers.end());
    int moves = m / g - n;  // may be negative
    // In C++, negative modulo yields negative remainder; check parity via bitwise AND.
    if ((moves & 1) != 0) {
        return "Alice";
    } else {
        return "Bob";
    }
}

#include <cassert>
#include <string>
#include <vector>

// Function declaration (from solution)
std::string determineWinner(const std::vector<int>& numbers);

int main() {
    // Single element: m/g=1, moves=1-1=0 -> Bob
    assert(determineWinner({5}) == "Bob");
    // Two elements: [2,4] gcd=2, m/g=2, moves=2-2=0 -> Bob
    assert(determineWinner({2, 4}) == "Bob");
    // [3,6,9] gcd=3, m/g=3, moves=3-3=0 -> Bob
    assert(determineWinner({3, 6, 9}) == "Bob");
    // [4,8] gcd=4, m/g=2, moves=2-2=0 -> Bob
    assert(determineWinner({4, 8}) == "Bob");
    // [6,10] gcd=2, m/g=5, moves=5-2=3 -> Alice
    assert(determineWinner({6, 10}) == "Alice");
    // [2,2] gcd=2, m/g=1, moves=1-2=-1 -> odd -> Alice
    assert(determineWinner({2, 2}) == "Alice");
    // [7,7,7] gcd=7, m/g=1, moves=1-3=-2 -> even -> Bob
    assert(determineWinner({7, 7, 7}) == "Bob");
    // [1,2] gcd=1, m/g=2, moves=2-2=0 -> Bob
    assert(determineWinner({1, 2}) == "Bob");
    // [1,3] gcd=1, m/g=3, moves=3-2=1 -> Alice
    assert(determineWinner({1, 3}) == "Alice");
    // [12,18,24] gcd=6, m/g=4, moves=4-3=1 -> Alice
    assert(determineWinner({12, 18, 24}) == "Alice");
    return 0;
}
