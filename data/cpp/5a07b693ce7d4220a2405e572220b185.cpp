Write a C++ function that, given a positive integer `x`, returns the maximum possible number of **pairs of identical positive integers** that can be formed when `x` identical items are divided into pairs, where each pair must contain exactly two items. Since any `x` can be split into pairs of size 2 as long as we have at least 2 items, the answer is simply `x / 2` (integer division truncating toward zero). For example, for `x = 5` we can form 2 pairs and have 1 leftover item; for `x = 4` we can form 2 pairs exactly. Your function should handle any positive `x` up to \(10^9\). The task is to implement this logic in a clean, self‑contained function that can be called repeatedly.

// The core observation is that the number of complete pairs of two items that can be formed from `x` identical items is simply the integer quotient of `x` divided by 2. This holds for all positive integers: if `x` is even, we get exactly `x/2` pairs; if `x` is odd, we get `(x-1)/2` pairs, which is also `x/2` in integer division. There are no special edge cases beyond ensuring `x` is non‑negative (the problem guarantees positive, so we don’t need to handle negative or zero, but the function can still safely handle zero). The time complexity is \(O(1)\) because it’s a single arithmetic operation, and the space complexity is \(O(1)\) as well. The solution is straightforward and does not require any data structures or complex logic.

#include <cstdint>

// Returns the maximum number of pairs that can be formed from x identical items.
// Each pair requires exactly 2 items. Works for any non-negative x.
int64_t maxPairs(int64_t x) {
    return x / 2;
}

#include <cassert>
#include <cstdint>

int main() {
    assert(maxPairs(4) == 2);
    assert(maxPairs(5) == 2);
    assert(maxPairs(1) == 0);
    assert(maxPairs(2) == 1);
    assert(maxPairs(1000000000) == 500000000);
    assert(maxPairs(999999999) == 499999999);
    assert(maxPairs(0) == 0);
    return 0;
}
