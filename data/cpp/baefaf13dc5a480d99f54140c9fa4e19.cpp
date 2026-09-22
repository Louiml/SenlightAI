// Given two integers `x` and `y` representing the total number of legs and the total number of animals on a farm, write a C++ function `bool isPossible(ll x, ll y)` that returns `true` if it is possible for the farm to contain only chickens (2 legs each) and cows (4 legs each) such that the total number of animals is exactly `y` and the total number of legs is exactly `x`. The function must return `false` otherwise. Note that `x` and `y` are non-negative integers, and the number of chickens and cows must both be non-negative integers. The function should handle all edge cases, including zero animals, zero legs, and unbalanced leg/animal counts.

Let `c` be the number of chickens and `w` be the number of cows. We have two equations:  
`c + w = y` (total animals)  
`2c + 4w = x` (total legs)  
From the first equation, `c = y - w`. Substitute into the second: `2(y - w) + 4w = x` → `2y + 2w = x` → `2w = x - 2y` → `w = (x - 2y)/2`. For `w` to be a non-negative integer, we need `(x - 2y)` to be non-negative and even. Also, `c = y - w` must be non-negative, so `w ≤ y`. Equivalent conditions are: `x` must be even (since total legs is always even when chickens and cows have even legs), `2y ≤ x` (enough legs for all animals to be chickens), and `4y ≥ x` (not too many legs for all animals to be cows). Thus the condition reduces to: `x` is even, `2*y ≤ x`, and `4*y ≥ x`. If any of these fails, return `false`. Edge cases: `x=0, y=0` → true (zero animals, zero legs); `x=0, y>0` → false; `x=2, y=1` → true (one chicken); `x=4, y=1` → true (one cow); `x=3, y=1` → false (odd legs). Time complexity is O(1) and space complexity is O(1).

#include <cstdint>

using ll = long long;

// Determine if it's possible to have y animals (chickens and cows) with x total legs.
bool isPossible(ll x, ll y) {
    // Total legs must be even, and total legs must be between 2*y (all chickens) and 4*y (all cows).
    if (x % 2 != 0) return false;
    if (2 * y > x) return false;
    if (4 * y < x) return false;
    return true;
}

#include <cassert>

int main() {
    assert(isPossible(0, 0) == true);          // no animals, no legs
    assert(isPossible(2, 1) == true);          // one chicken
    assert(isPossible(4, 1) == true);          // one cow
    assert(isPossible(4, 2) == true);          // two chickens
    assert(isPossible(8, 2) == true);          // two cows
    assert(isPossible(6, 2) == true);          // one chicken and one cow
    assert(isPossible(3, 1) == false);         // odd legs impossible
    assert(isPossible(1, 1) == false);         // too few legs for one animal
    assert(isPossible(10, 1) == false);        // too many legs for one animal
    assert(isPossible(7, 3) == false);         // odd legs and also impossible combination
}
