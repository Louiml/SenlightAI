/*
Write a C++ function named `canPlaceShips` that takes two integers `n` and `k`, and returns a `bool` indicating whether it is possible to arrange ships according to the following rule: the ships can be placed if the number of cells `n` is even, OR if the number of ships `k` is exactly 1. Otherwise, placement is impossible. The function must handle large values (up to `10^18`) and correctly return `true` for all even `n` regardless of `k`, return `true` for `n` odd and `k == 1`, and return `false` for all other cases (odd `n` with `k > 1`). There is no input validation required; assume both values are non-negative integers.
*/
#include <cstdint>

// Returns true if ships can be placed given n cells and k ships.
// Rule: possible if n is even OR k == 1; otherwise impossible.
bool canPlaceShips(long long n, long long k) {
    return (n % 2 == 0) || (k == 1);
}
#include <cassert>

int main() {
    // Even n always true
    assert(canPlaceShips(0, 5) == true);
    assert(canPlaceShips(2, 0) == true);
    assert(canPlaceShips(1000000000000000000LL, 999999999999999999LL) == true);

    // Odd n with k == 1 true
    assert(canPlaceShips(1, 1) == true);
    assert(canPlaceShips(7, 1) == true);
    assert(canPlaceShips(999999999999999999LL, 1) == true);

    // Odd n with k > 1 false
    assert(canPlaceShips(1, 2) == false);
    assert(canPlaceShips(3, 0) == false);
    assert(canPlaceShips(5, 3) == false);
    assert(canPlaceShips(999999999999999999LL, 2) == false);

    return 0;
}
// The logic is a straightforward conditional: the condition for success is `(n % 2 == 0) || (k == 1)`. This directly follows the original problem's pattern. Edge cases include `n == 0` (even, so true), `k == 0` (if n odd, false; if n even, true), and large values up to `10^18` which fit in `long long`. The algorithm uses only constant-time arithmetic operations and no extra data structures, so time complexity is O(1) and space complexity is O(1). The main subtlety is ensuring the function is `const`-correct and uses appropriate types to avoid overflow; since `n` and `k` are non-negative, `long long` suffices for the given range.
