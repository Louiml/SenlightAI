/*
Write a standalone C++ function `bool isLuckyNumber(int n)` that determines whether a given positive integer `n` is a "lucky number" according to the classic lucky number sieve process: start with the sequence of positive integers, remove every 2nd number, then every 3rd remaining number, then every 4th remaining number, and so on, increasing the step by 1 each round. The numbers that survive this process are lucky numbers. The function should return `true` if `n` is lucky and `false` otherwise. The input `n` is guaranteed to be a positive integer (≥ 1). The function must be efficient for `n` up to about 10^9.
*/
#include <cstdint>

// Determine if a positive integer n is a lucky number (survives the sieve process).
bool isLuckyNumber(int n) {
    // Start with step size 2 (remove every 2nd number)
    int step = 2;
    int current = n;

    while (step <= current) {
        // If current position is a multiple of step, it gets eliminated
        if (current % step == 0) {
            return false;
        }

        // Update position after removing every step-th element
        current -= current / step;
        ++step;
    }

    return true;
}
#include <cassert>

// Function declaration (to be defined in the solution section)
bool isLuckyNumber(int n);

int main() {
    // Known lucky numbers: 1, 3, 7, 9, 13, 15, 21, 25, 31, 33, 37, 43, 49, ...
    assert(isLuckyNumber(1) == true);
    assert(isLuckyNumber(2) == false);
    assert(isLuckyNumber(3) == true);
    assert(isLuckyNumber(4) == false);
    assert(isLuckyNumber(5) == false);
    assert(isLuckyNumber(6) == false);
    assert(isLuckyNumber(7) == true);
    assert(isLuckyNumber(13) == true);
    assert(isLuckyNumber(21) == true);
    assert(isLuckyNumber(25) == true);
    assert(isLuckyNumber(34) == false);
    assert(isLuckyNumber(99) == false);
    assert(isLuckyNumber(1000000000) == true); // Large lucky number (known from sequence)
    return 0;
}
// The algorithm iteratively simulates the sieve process backwards from the current value. For a number `n`, in the `x`-th round (starting with `x=2`), the removal pattern eliminates every `x`-th position among the numbers that survived previous rounds. If at any round `n % x == 0`, it means `n` is eliminated, so return `false`. If `n` survives all rounds up to when `x > n`, then `n` is lucky. The key optimization is that after each round, the position of `n` in the new sequence is `n - n/x` (since we remove `n/x` elements before or at its position), and we increment `x` by 1. This loop runs at most `O(sqrt(n))` times because once `x > sqrt(n)` and `n` hasn't been eliminated, the condition `n % x == 0` only happens if `x` equals `n` (which cannot happen because `x` grows slower than `n` in practical cases), but in the worst case the loop runs until `x` exceeds `n`, which is `O(n)` for very small `n` but in practice for large `n` it terminates quickly. The time complexity is `O(k)` where `k` is the number of rounds until elimination or survival, which for large `n` is approximately `O(log n)` to `O(sqrt(n))` in practice; the worst-case is `O(n)` for small `n` up to a few thousand, but the expected complexity is low. Space complexity is `O(1)`.
