You are given an integer `n` and two binary strings `a` and `b` of length `n` (containing only characters `'0'` and `'1'`). You may perform operations on the string `a` any number of times. In one operation, you choose a prefix of `a` (from position `1` to some position `k`, where `1 ≤ k ≤ n`) and flip every bit in that prefix (i.e., change `0` to `1` and `1` to `0`). Determine whether it is possible to transform string `a` into string `b` using any sequence of such prefix-flip operations. Write a C++ function `bool canTransform(const std::string& a, const std::string& b)` that returns `true` if transformation is possible, and `false` otherwise. The function should handle strings of length up to \(10^5\) efficiently.
This problem can be solved by simulating the effect of prefix flips from left to right. A key observation is that if we process positions from left to right, the state of each character in `a` is determined by the parity of the number of flips applied to prefixes that include that position. A more direct approach: maintain a variable `flipped` that indicates whether the current prefix has been toggled an odd number of times. For each position `i` from `0` to `n-1`, compute the effective char of `a[i]` after all previous flips: `actual = (a[i] - '0') ^ flipped`. If `actual` does not equal `b[i]`, then we must flip the prefix ending at `i` (i.e., flip positions `0..i`), so we toggle `flipped`. However, it's also allowed to flip a prefix that doesn't change the current position but could affect future ones? Actually, any flip that ends before `i` would have already been accounted. The only way to fix a mismatch at position `i` without affecting earlier positions (which are already correct) is to flip a prefix that ends at `i` (or later), but flipping a prefix ending later would also affect position `i` and all earlier positions, ruining them unless they are also mismatched. Therefore, the greedy left-to-right strategy is correct: when at position `i`, if the current effective char doesn't match `b[i]`, we must flip prefix `0..i` (toggle `flipped`). This works because flipping any longer prefix would break earlier already-corrected prefixes, and we have no other operation that affects only position `i` without affecting earlier ones. After processing, we check if the last flip (if any) leaves the entire string matching. Actually, the greedy automatically ensures all positions match after the loop, because whenever we see a mismatch we fix it, and flipping prefix `0..i` only changes positions `0..i` but all positions `0..i-1` were already correct and flipping them would break them? Wait, that's a problem: flipping prefix `0..i` changes all positions from `0` to `i`, so positions `0..i-1` that were already correct would become incorrect. However, recall that we have already processed those earlier positions and they are correct with respect to the current `flipped` state. When we toggle `flipped`, the effective value of earlier positions changes, so we need to check that they still match. But notice: if at position `i` we see a mismatch, it means the current `flipped` (which has been toggled for all earlier prefixes that we flipped) gives an effective char that is wrong. To fix position `i`, we toggle `flipped`, which changes the effective value of every position `0..i`. For earlier positions, their effective value flips. But since we already ensured they matched before, after flipping they would mismatch. So how can this be valid? The trick is: we are allowed to flip any prefix, and flipping a prefix of length `i+1` changes all positions `0..i` simultaneously. If we need to change position `i`, we must also change positions `0..i-1`. But those earlier positions might already be correct, so flipping a longer prefix would break them. However, it is possible to first flip shorter prefixes to adjust earlier positions, then flip a longer prefix to adjust later ones. The greedy from left to right actually works if we process from right to left instead? Let's think. Standard solution for this type of problem (prefix flips) is to simulate from right to left, because flipping a prefix ending at `i` affects all positions `≤ i`, so if we process from rightmost to leftmost, we can decide flips independently without affecting already-fixed positions to the right. Indeed, if we go from rightmost position `n-1` down to `0`, when we decide to flip a prefix ending at `i`, it affects positions `0..i` but we have already fixed positions `i+1..n-1` (to the right), so they are unaffected (since the prefix ends at `i`). That is the correct greedy. So algorithm: initialize `flipped = 0`. For `i` from `n-1` down to `0`, compute `actual = (a[i] - '0') ^ flipped`. If `actual != (b[i] - '0')`, then set `flipped ^= 1` (flip the prefix ending at `i`). After processing all positions, return `true` (because we always can fix each position from right to left). This works because flipping a prefix ending at `i` never affects positions to the right of `i`. Edge cases: all zeros/ones, single character, already equal. Time complexity O(n), space O(1) auxiliary (ignoring input storage).
#include <string>

// Determines if binary string 'a' can be transformed into 'b'
// by repeatedly flipping any prefix of 'a'.
bool canTransform(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    int n = static_cast<int>(a.size());
    int flipped = 0;  // parity of flips applied to prefixes ending at current or larger index

    // Process from rightmost to leftmost so that flipping a prefix ending at i
    // only affects positions <= i, leaving already-corrected positions to the right unchanged.
    for (int i = n - 1; i >= 0; --i) {
        int currentBit = (a[i] - '0') ^ flipped;
        int targetBit = b[i] - '0';
        if (currentBit != targetBit) {
            // Flip the prefix of length i+1 (positions 0..i).
            flipped ^= 1;
        }
    }
    // By construction, every position matches after the loop.
    return true;
}
#include <cassert>
#include <string>

// Declaration of the solution function (must be defined above or in included file).
bool canTransform(const std::string& a, const std::string& b);

int main() {
    // Already equal
    assert(canTransform("0000", "0000") == true);
    assert(canTransform("1111", "1111") == true);

    // Single character
    assert(canTransform("0", "1") == true);  // flip prefix length 1
    assert(canTransform("1", "0") == true);
    assert(canTransform("1", "1") == true);

    // Simple cases
    assert(canTransform("01", "10") == true);  // flip whole prefix, then flip first character? Actually: rightmost pos 1: a[1]=1, flipped=0, b[1]=0 mismatch → flip prefix length2 → flipped=1. pos0: a[0]=0 ^1 =1 == b[0]=1 → done.
    assert(canTransform("10", "01") == true);  // similar
    assert(canTransform("000", "111") == true); // flip whole prefix once
    assert(canTransform("111", "000") == true);

    // Cases where transformation is impossible (should always return true? Actually any binary string can be transformed to any other? Let's check: With prefix flips, can we turn "01" into "00"? Rightmost: pos1: a[1]=1 mismatch with 0 → flip prefix len2 → flipped=1, now a[1]^1=0 ok, pos0: a[0]^1=1 vs b[0]=0 mismatch → flip prefix len1 → flipped=0, now a[0]^0=0 ok. So yes possible. Actually with prefix flips you can achieve any target because each position can be independently toggled by choosing appropriate prefix lengths? Consider n=2: possible reachable states from "00" via flipping prefixes: {} → 00, flip len1 → 10, flip len2 → 11, flip len1 then len2 → 01. So all 4 states reachable. For any n, all 2^n states reachable? Yes, because you can generate any binary vector by toggling differences from left to right. So the function should always return true for equal lengths. But the problem statement says determine if possible; answer always true. However, let's test the function on random cases and also confirm it returns true for equal length. 
    // Test with random strings of equal length - should all return true.
    assert(canTransform("010101", "101010") == true);
    assert(canTransform("000111", "111000") == true);
    assert(canTransform("1010", "0101") == true);
    assert(canTransform("1", "0") == true);
    assert(canTransform("0", "0") == true);

    // Different lengths should return false (though task might not require it, we test).
    assert(canTransform("0", "00") == false);

    return 0;
}
