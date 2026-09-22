/*
Write a C++ function `minimumCoinsGreedy` that accepts an array of coin denominations (sorted in non-increasing order, guaranteed to include a coin of denomination 1), the number of denominations, and a target value `V`. The function must return the minimum number of coins needed to make exactly `V` using the given denominations, but only under the assumption that the denominations are canonical (i.e., the greedy strategy of always taking the largest possible coin first is optimal). The function should compute the result using the greedy method, handling cases where `V` is 0 (return 0) and where the denominations include 1 so that any positive `V` can always be formed. Do not validate the canonical property; just implement the greedy computation.
*/

#include <vector>
#include <cstddef>

// Computes the greedy minimum coin count for a canonical coin system.
// coins must be sorted in non-increasing order and include 1 for all positive V.
// Returns the total number of coins used.
int minimumCoinsGreedy(const std::vector<int>& coins, int target) {
    int count = 0;
    int remaining = target;
    for (const int coin : coins) {
        if (remaining == 0) break;
        count += remaining / coin;
        remaining %= coin;
    }
    return count;
}

#include <cassert>
#include <vector>

int main() {
    // Classic US coin denominations: 25, 10, 5, 1 (non-increasing, canonical).
    std::vector<int> us_coins = {25, 10, 5, 1};
    assert(minimumCoinsGreedy(us_coins, 0) == 0);
    assert(minimumCoinsGreedy(us_coins, 1) == 1);
    assert(minimumCoinsGreedy(us_coins, 5) == 1);
    assert(minimumCoinsGreedy(us_coins, 10) == 1);
    assert(minimumCoinsGreedy(us_coins, 25) == 1);
    assert(minimumCoinsGreedy(us_coins, 30) == 2);      // 25+5
    assert(minimumCoinsGreedy(us_coins, 41) == 4);      // 25+10+5+1
    assert(minimumCoinsGreedy(us_coins, 99) == 9);      // 25*3 + 10*2 + 1*4
    assert(minimumCoinsGreedy(us_coins, 100) == 4);     // 25*4

    // Another canonical set: denominations {20, 10, 5, 1}.
    std::vector<int> coins2 = {20, 10, 5, 1};
    assert(minimumCoinsGreedy(coins2, 55) == 4);       // 20+20+10+5
    assert(minimumCoinsGreedy(coins2, 19) == 6);       // 10+5+1*4

    // Only 1 coin denomination (just {1}).
    std::vector<int> only_one = {1};
    assert(minimumCoinsGreedy(only_one, 7) == 7);
    assert(minimumCoinsGreedy(only_one, 0) == 0);

    return 0;
}

// The solution iterates through the coin denominations from largest (index 0, since sorted non-increasing) to smallest. For each denomination `coins[i]`, we take as many of that coin as possible: `V / coins[i]`, add that count to the total answer, and reduce `V` by `coins[i] * (V / coins[i])` using the modulo operator `V %= coins[i]`. After processing all denominations, if the denominations are canonical and include 1, `V` will be reduced to 0, and the accumulated count is the minimal number of coins. Edge cases: if `V` is 0, the loop adds 0 and returns 0. If a denomination is larger than `V`, the division yields 0 and modulo leaves `V` unchanged. The algorithm runs in O(m) time where `m` is the number of denominations, and uses O(1) auxiliary space.
