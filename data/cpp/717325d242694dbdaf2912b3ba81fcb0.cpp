// Write a C++ function that, given a positive integer `n` representing the total number of coins, returns the number of complete rows that can be built if each row `k` requires exactly `k` coins, and the rows are built sequentially (row 1 needs 1 coin, row 2 needs 2 coins, and so on). For example, with `n = 5`, you can complete rows 1 and 2 (using 1+2=3 coins) but not row 3 (which would require 3 more coins, total 6), so the function returns 2. The input `n` is guaranteed to be a positive integer, and the function must handle values up to `2^31 - 1` safely without overflow. The function should be named `completeRows` and use only constant extra space.

#include <cassert>

int completeRows(int n); // declaration

int main() {
    assert(completeRows(1) == 1);
    assert(completeRows(2) == 1);
    assert(completeRows(5) == 2);
    assert(completeRows(6) == 3);
    assert(completeRows(8) == 3);
    assert(completeRows(10) == 4);
    assert(completeRows(55) == 10);       // 1+2+...+10 = 55
    assert(completeRows(56) == 10);
    assert(completeRows(2147483647) == 65535); // max int, sqrt(2*2^31) ~ 65536
    return 0;
}

#include <cstdint>

// Returns the number of complete rows that can be formed from exactly n coins,
// where row k requires k coins. n is a positive 32-bit integer.
int completeRows(int n) {
    int row = 0;
    // While there are enough coins for the next row
    while (n > row) {
        ++row;
        n -= row;
    }
    return row;
}

// The problem is a classic simulation: repeatedly subtract increasing row sizes from the total coins until the remaining coins are insufficient for the next row. The main algorithm initializes a counter `row` to 0. While `n` is greater than `row` (i.e., there are enough coins to build the next row), increment `row` by 1 and subtract `row` from `n`. At the end, `row` is the number of complete rows.  
// Edge cases:  
// - `n = 1` → only row 1 fits, returns 1.  
// - `n = 2` → row 1 fits (coins left = 1), row 2 needs 2 > remaining, returns 1.  
// - Large `n` near `INT_MAX` (e.g., 2147483647) → the loop runs about 65536 times, well within limits; no overflow occurs because both `row` and `n` remain within `int` range (the loop stops before `row` exceeds ~65536).  
// Time complexity is `O(sqrt(n))` because the sum of the first `k` rows is `k(k+1)/2`, so the loop runs approximately `sqrt(2n)`. Space complexity is `O(1)`.
