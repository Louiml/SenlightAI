// Write a C++ function `long long flowerGame(int n, int m)` that returns the number of distinct pairs of integers `(x, y)` with `1 ≤ x ≤ n` and `1 ≤ y ≤ m` such that `x + y` is odd. The integers `n` and `m` are positive (≥ 1). The result may exceed the range of a 32-bit integer, so use `long long` for the return type and for any intermediate arithmetic.
The condition `x + y` is odd is equivalent to `x` and `y` having opposite parity (one even, one odd). For a range from 1 to `n`, the counts of odd and even numbers are: `oddCount = (n + 1) / 2`, `evenCount = n / 2`. Similarly for range 1 to `m`, we have `oddM = (m + 1) / 2`, `evenM = m / 2`. The number of valid pairs is the sum of pairs where `x` is odd and `y` is even, plus pairs where `x` is even and `y` is odd: `oddCount * evenM + evenCount * oddM`. This simplifies algebraically to `(n * m) / 2` using integer division truncation. For example, if `n = 3`, `m = 4`: oddCount=2, evenCount=1, oddM=2, evenM=2 → pairs=2*2+1*2=6, and `(3*4)/2=6`. Edge cases include `n = 1`, `m = 1` (no pairs, result 0), and large values up to ~10^9 where `n*m` may overflow 32-bit int, so use `long long` multiplication. Time complexity is O(1), space O(1).
#include <cstdint>

// Return the number of pairs (x, y) with 1 <= x <= n, 1 <= y <= m,
// and x + y is odd. n and m are positive integers.
long long flowerGame(int n, int m) {
    // The count simplifies to floor(n * m / 2).
    return static_cast<long long>(n) * static_cast<long long>(m) / 2LL;
}
#include <cassert>

int main() {
    assert(flowerGame(1, 1) == 0);
    assert(flowerGame(1, 2) == 1);
    assert(flowerGame(2, 1) == 1);
    assert(flowerGame(2, 2) == 2);
    assert(flowerGame(3, 3) == 4);
    assert(flowerGame(10, 10) == 50);
    assert(flowerGame(3, 4) == 6);
    assert(flowerGame(4, 3) == 6);
    assert(flowerGame(1000000000, 1000000000) == 500000000000000000LL);
    assert(flowerGame(999999999, 1000000000) == 499999999500000000LL);
}
