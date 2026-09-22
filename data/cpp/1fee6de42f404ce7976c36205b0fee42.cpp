Given an array of `n` positive integers, write a C++ function `bool canMakeAllDivisors(int n, const std::vector<int>& a)` that returns `true` if there exists a value `g` equal to the GCD (greatest common divisor) of all elements in the array that are not divisible by the minimum element `a[0]` (the smallest value in the array), and `g` itself appears as an element in the original array. More precisely: let `m` be the minimum element in `a`. Consider the set `S = {x in a | x % m != 0}`. If `S` is empty, return `false`. Otherwise compute `g` as the GCD of all elements in `S`. If `g` appears in the original array, return `true`; otherwise return `false`. The array is guaranteed to have at least one element and all elements are positive. The order of elements in the input does not matter except for determining the minimum.
#include <cassert>
#include <vector>

// Assuming canMakeAllDivisors is defined above
int main() {
    // Example from typical use: a = {6, 10, 15} -> min=6, non-divisible: 10,15 -> gcd=5, 5 not in array -> false
    assert(canMakeAllDivisors(3, {6, 10, 15}) == false);
    // a = {2, 4, 6} -> min=2, all divisible -> no non-divisible -> false
    assert(canMakeAllDivisors(3, {2, 4, 6}) == false);
    // a = {8, 12, 18} -> min=8, non-divisible: 12,18 -> gcd=6, 6 not in array -> false
    assert(canMakeAllDivisors(3, {8, 12, 18}) == false);
    // a = {5, 10, 15} -> min=5, all divisible -> false
    assert(canMakeAllDivisors(3, {5, 10, 15}) == false);
    // a = {3, 6, 9} -> min=3, all divisible -> false
    assert(canMakeAllDivisors(3, {3, 6, 9}) == false);
    // a = {4, 6, 8} -> min=4, non-divisible: 6 -> gcd=6, 6 not in array -> false
    assert(canMakeAllDivisors(3, {4, 6, 8}) == false);
    // a = {6, 9, 12} -> min=6, non-divisible: 9 -> gcd=9, 9 in array -> true
    assert(canMakeAllDivisors(3, {6, 9, 12}) == true);
    // a = {10, 15, 20} -> min=10, non-divisible: 15 -> gcd=15, 15 in array -> true
    assert(canMakeAllDivisors(3, {10, 15, 20}) == true);
    // Single element
    assert(canMakeAllDivisors(1, {7}) == false);
    // Two elements: {2, 3} -> min=2, non-divisible: 3 -> gcd=3, 3 in array -> true
    assert(canMakeAllDivisors(2, {2, 3}) == true);
    return 0;
}
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_set>

// Determines whether the GCD of all elements not divisible by the minimum element
// exists in the original array.
bool canMakeAllDivisors(int n, const std::vector<int>& a) {
    if (n <= 0) return false;

    std::vector<int> sorted = a;
    std::sort(sorted.begin(), sorted.end());
    int minimum = sorted[0];

    std::unordered_set<int> present(a.begin(), a.end());

    int gcd_val = -1;
    for (int x : sorted) {
        if (x % minimum != 0) {
            if (gcd_val == -1) {
                gcd_val = x;
            } else {
                gcd_val = std::gcd(gcd_val, x);
            }
        }
    }

    if (gcd_val == -1) return false;
    return present.find(gcd_val) != present.end();
}
// The solution first sorts the array to easily identify the minimum element `a[0]`. Then it iterates through the sorted array; for any element that is not divisible by `a[0]`, we update a running GCD (`g`). Initially `g = -1` to indicate "not set yet". When we encounter the first such element, we set `g` to that element; thereafter we set `g = std::gcd(g, current)`. After the loop, if no such element was found (`g == -1`), the answer is `false`. Otherwise, we use a frequency map (or unordered_set) built from the original array to check if `g` exists in the array. If it does, answer is `true`; else `false`. Edge cases: only one element (then no non‑divisible elements exist → false); all elements equal (same); all elements divisible by the minimum (false). Time complexity is O(n log n) due to sorting, plus O(n) for map building and GCD computation. Space complexity is O(n) for the frequency structure.
