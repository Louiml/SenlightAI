// Write a C++ function named `minimumBills` that takes a single non-negative integer `n` representing the amount of money in dollars, and returns the minimum number of banknotes needed to make that amount using only denominations of 100, 20, 10, 5, and 1. The function should greedily use the largest possible denomination first. The input `n` can be as large as \(10^9\), and the result is guaranteed to fit in an `int`. For example, `minimumBills(125)` returns 3 (one 100, one 20, one 5), and `minimumBills(0)` returns 0.

The optimal strategy is a greedy algorithm: repeatedly take the largest denomination that does not exceed the remaining amount. Since each larger denomination is a multiple of the smaller ones (100 is a multiple of 20, 20 of 10, 10 of 5, and 5 of 1), the greedy approach always yields the minimum number of bills. The algorithm processes the denominations in descending order: for each denomination `d`, compute `n / d` (integer division) to get the number of bills of that value, add it to a running count, and subtract `n / d * d` from `n`. This continues until the remaining amount is zero. Edge cases: `n = 0` returns 0; `n = 5` returns 1; `n = 1` returns 1; very large values like \(10^9\) work fine because the count fits in an `int` (the maximum count is \(10^9\) when using only 1‑dollar bills, which fits in a 32‑bit signed int). Time complexity is \(O(1)\) (only five fixed denominations), and space complexity is \(O(1)\).

#include <vector>

// Returns the minimum number of banknotes needed to make the amount n
// using denominations 100, 20, 10, 5, and 1 (greedy approach).
int minimumBills(int n) {
    const std::vector<int> denominations = {100, 20, 10, 5, 1};
    int count = 0;
    int remaining = n;
    for (int d : denominations) {
        if (remaining >= d) {
            count += remaining / d;
            remaining %= d;
        }
        if (remaining == 0) break;
    }
    return count;
}

#include <cassert>

int main() {
    // Basic cases
    assert(minimumBills(0) == 0);
    assert(minimumBills(1) == 1);
    assert(minimumBills(5) == 1);
    assert(minimumBills(10) == 1);
    assert(minimumBills(20) == 1);
    assert(minimumBills(100) == 1);
    // Mixed values
    assert(minimumBills(125) == 3);   // 100+20+5
    assert(minimumBills(136) == 5);   // 100+20+10+5+1
    assert(minimumBills(999) == 14);  // 9*100 + 4*20 + 1*10 + 1*5 + 4*1
    // Large input
    assert(minimumBills(1000000000) == 10000000); // 10^7 bills of 100
    // All denominations
    assert(minimumBills(136) == 5);
    assert(minimumBills(93) == 8);    // 4*20 + 1*10 + 3*1
}
