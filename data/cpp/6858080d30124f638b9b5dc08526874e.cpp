Write a C++ function `minimumCoinsGreedy` that takes a non-negative integer amount `V` (representing a value in Indian Rupees) and uses a greedy algorithm with the standard Indian coin denominations `{1, 2, 5, 10, 20, 50, 100, 200, 500, 1000}` (with unlimited supply of each) to return the minimum number of coins required to make exactly `V`. The greedy strategy must always pick the largest possible denomination first. Assume the input is always exactly representable with the given denominations (so `V` is a non-negative integer). The function should return `0` when `V` is `0`. Do not include a `main` function in the solution; only provide the free function.
#include <cassert>

int main() {
    // Edge case: zero amount.
    assert(minimumCoinsGreedy(0) == 0);
    // Single coin denominations.
    assert(minimumCoinsGreedy(1) == 1);
    assert(minimumCoinsGreedy(2) == 1);
    assert(minimumCoinsGreedy(5) == 1);
    assert(minimumCoinsGreedy(10) == 1);
    assert(minimumCoinsGreedy(20) == 1);
    assert(minimumCoinsGreedy(100) == 1);
    // Composite amounts.
    assert(minimumCoinsGreedy(49) == 5); // 20+20+5+2+2
    assert(minimumCoinsGreedy(93) == 5); // 50+20+20+2+1
    assert(minimumCoinsGreedy(2890) == 7); // 1000+1000+500+200+100+50+40? Wait: 1000+1000+500+200+100+50+40? Actually 40 not a coin, so 1000+1000+500+200+100+50+20+10+5+2+1? Let's recalc: 2890 = 2*1000 + 1*500 + 1*200 + 1*100 + 1*50 + 1*20 + 1*10 + 1*5 + 1*2 + 1*1? That's 2+1+1+1+1+1+1+1+1+1 = 11. So better test: 2890 = 2*1000 + 1*500 + 1*200 + 1*100 + 1*50 + 1*20 + 1*10 + 1*5 + 1*2 + 1*1 = 11 coins. Let's just test 2890 == 11.
    assert(minimumCoinsGreedy(2890) == 11);
    // Large amount.
    assert(minimumCoinsGreedy(10000) == 10); // 10 * 1000
    // Mixed: 137 = 100+20+10+5+2 => 5 coins.
    assert(minimumCoinsGreedy(137) == 5);
    // Check for 199 = 100+50+20+20+5+2+2 = 7 coins.
    assert(minimumCoinsGreedy(199) == 7);
    // Check for 303 = 200+100+2+1 = 4 coins.
    assert(minimumCoinsGreedy(303) == 4);
    return 0;
}
#include <vector>
#include <cstddef>

// Returns the minimum number of Indian coins needed to make exactly `amount`
// using the greedy algorithm (largest denomination first).
// Denominations: 1, 2, 5, 10, 20, 50, 100, 200, 500, 1000 (unlimited supply).
// Assumes `amount` is non-negative and exactly representable.
int minimumCoinsGreedy(int amount) {
    // Standard Indian coin denominations in ascending order.
    const int denominations[] = {1, 2, 5, 10, 20, 50, 100, 200, 500, 1000};
    const std::size_t n = sizeof(denominations) / sizeof(denominations[0]);

    int remaining = amount;
    int coinCount = 0;

    // Iterate from largest to smallest.
    for (std::size_t i = n; i > 0; --i) {
        int coin = denominations[i - 1];
        while (remaining >= coin) {
            remaining -= coin;
            ++coinCount;
        }
        // Early exit if no more amount left.
        if (remaining == 0) {
            break;
        }
    }

    return coinCount;
}
// The solution iterates through the denominations from largest to smallest. For each denomination, while the remaining amount is greater than or equal to that denomination, subtract it and increment a coin counter. This greedy choice works optimally for the Indian coin system because each coin is a multiple of the smaller denominations (specifically, the denominations are canonical for this coin set). The loop continues until the amount becomes zero or all denominations are exhausted. Edge cases: `V = 0` returns `0` immediately; `V` exactly equal to a denomination uses one coin; a large `V` like `2890` uses as many `1000`, `500`, etc. as possible. Time complexity is `O(n)` where `n` is the number of denominations (here constant 10), and space complexity is `O(1)` since no extra data structures are used. The function uses a static array of denominations and avoids mutating the input parameter by using a local working copy.
