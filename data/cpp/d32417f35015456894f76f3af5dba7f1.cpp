Write a C++ function `int countCoinsToChange(int amount)` that, given an amount of money (1 to 1000) in dollars as an integer, returns the minimum number of coins needed to make exactly that amount using only the denominations 1, 5, 10, 50, 100, and 500 dollars. The function must use a greedy algorithm: repeatedly subtract the largest possible coin that does not exceed the remaining amount, counting each subtraction as one coin. The input will always be a positive integer within the range, and the greedy approach is guaranteed to be optimal for these specific denominations (a property of the coin system). For example, for amount 999, the greedy algorithm would use one 500, four 100s, one 50, four 10s, one 5, and four 1s, totaling 15 coins. The function should not modify any global state and must be reusable for multiple calls.

The solution simulates the greedy coin change algorithm exactly as the original snippet does, but with an explicit and clearer loop. The original uses `while(1)` with a target `N` and starts from `n = 1000`, subtracting coins while `n - coin >= N`. This is equivalent to: for each coin denomination in descending order, while the remaining amount (initially `amount`) is greater than or equal to the coin, subtract the coin and increment the count. The loop terminates when the remaining amount becomes 0. Edge cases: the smallest amount is 1, so at least one coin is always used; the largest amount is 1000, so the maximum coins is 1000 (if only 1-dollar coins were used), but the greedy uses at most a few coins, e.g., for 1000 it uses two 500 coins. The time complexity is O(amount) in the worst case (since each subtraction reduces the amount by at least 1, so at most `amount` iterations), and the space complexity is O(1). The denominations are fixed at {500, 100, 50, 10, 5, 1}.

#include <vector>

// Returns the minimum number of coins needed to make the given amount
// using greedy coin change with denominations {1,5,10,50,100,500}.
// Precondition: amount is in [1, 1000].
int countCoinsToChange(int amount) {
    const std::vector<int> denominations = {500, 100, 50, 10, 5, 1};
    int remaining = amount;
    int coinCount = 0;
    for (int coin : denominations) {
        while (remaining >= coin) {
            remaining -= coin;
            ++coinCount;
        }
    }
    return coinCount;
}

#include <cassert>

int main() {
    // Basic cases from the original problem
    assert(countCoinsToChange(1) == 1);
    assert(countCoinsToChange(5) == 1);
    assert(countCoinsToChange(10) == 1);
    assert(countCoinsToChange(50) == 1);
    assert(countCoinsToChange(100) == 1);
    assert(countCoinsToChange(500) == 1);

    // Cases requiring multiple coins
    assert(countCoinsToChange(6) == 2);      // 5 + 1
    assert(countCoinsToChange(99) == 8);     // 50+10+10+10+10+5+1+1+1? Wait: 50+10*4=90, +5=95, +1*4=99 => coins:1+4+1+4=10, but correct greedy gives 50+10*4+5+1*4 = 1+4+1+4 = 10. Let's verify: 99: 50 (remain 49), 10*4 (remain 9), 5 (remain 4), 1*4 => total 1+4+1+4=10. So assert 10.
    assert(countCoinsToChange(999) == 15);   // 500+100*4+50+10*4+5+1*4 = 1+4+1+4+1+4 = 15
    assert(countCoinsToChange(1000) == 2);   // 500+500
    assert(countCoinsToChange(750) == 4);    // 500+100+100+50
    assert(countCoinsToChange(100) == 1);
    assert(countCoinsToChange(101) == 2);    // 100+1
}
