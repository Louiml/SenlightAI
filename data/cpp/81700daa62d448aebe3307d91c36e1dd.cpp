// Write a C++ function named `canFormParityPairs` that takes two integers `n` and `m` as input and returns a boolean value indicating whether it is possible to form `m` pairs from `n` items, where each pair must contain exactly two items, all items must be used, and the parity (odd/even) of the number of items in each pair must be the same (i.e., all pairs must have either an even or odd count of items, which effectively means each pair must have exactly 2 items since pairs are defined as size 2). More specifically, the function should return `true` if and only if `m` is less than or equal to `n` and both `n` and `m` have the same parity (both even or both odd). The function should handle all integer inputs including negative values, returning `false` for invalid cases where forming such pairs is impossible. Provide the function signature `bool canFormParityPairs(int n, int m)` and ensure it works for large integer inputs without overflow.
The problem reduces to a simple parity and magnitude check. For `m` pairs to be formed from `n` total items, we need at least `2*m` items, but since pairs are just formed by grouping items, the constraint `m <= n` is necessary (though not sufficient alone). Additionally, each pair must have the same parity of item count. Since a pair is defined as exactly 2 items, each pair has an even count. But the original snippet checks if `n % 2 == m % 2`, which suggests the real task is about splitting `n` items into `m` groups where each group has the same parity (odd or even). If `m` groups are formed, the sum of parities must equal `n`'s parity. If all groups have even size, then `n` must be even; if all groups have odd size, then `n` must be even if `m` is even (since sum of `m` odd numbers has parity equal to `m % 2`), and odd if `m` is odd. But the condition `n % 2 == m % 2` works because: if `m` is even, all groups odd ⇒ sum even ⇒ `n` even; if `m` is odd, all groups odd ⇒ sum odd ⇒ `n` odd. So the condition is exactly `n % 2 == m % 2`. Edge cases: `m` negative ⇒ impossible; `m > n` ⇒ impossible; `n` negative ⇒ impossible (but the function should return false). Also handle `m=0` and `n=0` – if `m=0` and `n=0` then true (zero groups, zero items) but `0%2==0` so condition holds; if `m=0` and `n>0` then false because `0<=n` but parities differ unless `n` even? Actually `0%2==0` and if `n` even then condition holds, but is it possible to form 0 pairs from `n` positive items? No, because all items must be used? The original snippet doesn't specify "all items used", but from context it's about whether `m` pairs can be formed from `n` items, which typically means using exactly `2*m` items, leaving `n-2*m` unused, so `m <= n` is enough. But the original condition is just `m <= n && (n%2 == m%2)`. We'll follow that logic. Negative inputs: if either is negative, return false (since counts can't be negative). Time complexity O(1), space O(1).
#include <cstddef>

// Determines if m pairs can be formed from n items such that each pair has the same parity of size.
// Returns true iff m <= n and n and m have the same parity.
bool canFormParityPairs(int n, int m) {
    // Negative counts are invalid.
    if (n < 0 || m < 0) {
        return false;
    }
    // Must have at least as many items as pairs, and parity must match.
    return m <= n && (n % 2 == m % 2);
}
#include <cassert>

int main() {
    // Basic valid cases
    assert(canFormParityPairs(4, 2) == true);   // n even, m even
    assert(canFormParityPairs(5, 3) == true);   // n odd, m odd
    // Invalid parity
    assert(canFormParityPairs(4, 3) == false);  // n even, m odd
    assert(canFormParityPairs(5, 2) == false);  // n odd, m even
    // m > n
    assert(canFormParityPairs(2, 3) == false);
    // Zero cases
    assert(canFormParityPairs(0, 0) == true);
    assert(canFormParityPairs(2, 0) == true);
    assert(canFormParityPairs(0, 2) == false);
    // Negative inputs
    assert(canFormParityPairs(-1, 2) == false);
    assert(canFormParityPairs(3, -1) == false);
    // Equal numbers
    assert(canFormParityPairs(3, 3) == true);
    assert(canFormParityPairs(6, 6) == true);
    return 0;
}
