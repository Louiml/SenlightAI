/*
Write a C++ function `bool hasThreeSumLastDigits(const std::vector<int>& nums)` that determines whether there exists a group of exactly three distinct indices in `nums` whose element values, when taken modulo 10 and added together, produce a sum ending in digit 3 (i.e., `(a % 10 + b % 10 + c % 10) % 10 == 3`). The function must account for duplicate values in the input: if the same value appears multiple times at different indices, they can be used as separate elements, but the three chosen indices must be pairwise distinct. The function should return `true` if such a triple exists, `false` otherwise. The input vector size `n` satisfies `1 ≤ n ≤ 10^5`, and each element is a non-negative integer. Time limit is strict (approximately 1 second), so avoid O(n³) or O(n²) solutions.
*/
#include <vector>

// Returns true if there exist three distinct indices in nums
// whose last digits sum to a number ending in 3.
bool hasThreeSumLastDigits(const std::vector<int>& nums) {
    // Frequency of each last digit (0-9)
    int freq[10] = {0};
    for (int val : nums) {
        freq[val % 10]++;
    }

    // Enumerate first two chosen digits
    for (int first = 0; first < 10; ++first) {
        if (freq[first] == 0) continue;
        freq[first]--; // use one occurrence

        for (int second = 0; second < 10; ++second) {
            if (freq[second] == 0) continue;
            freq[second]--; // use another occurrence

            // Required third digit to make sum % 10 == 3
            int need = (3 - (first + second) % 10 + 10) % 10;
            if (freq[need] > 0) {
                freq[second]++; // restore
                freq[first]++;  // restore
                return true;
            }

            freq[second]++; // restore for next iteration
        }

        freq[first]++; // restore for next iteration
    }

    return false;
}
#include <cassert>
#include <vector>

// The solution function is defined above (or included before this).
// This is a standalone test main.

bool hasThreeSumLastDigits(const std::vector<int>& nums);

int main() {
    // Basic case: 1+2+0 = 3 → true
    assert(hasThreeSumLastDigits({1, 2, 10}) == true);

    // Need exactly three distinct indices; only two numbers → false
    assert(hasThreeSumLastDigits({3, 0}) == false);

    // Duplicates: three 1s (1+1+1=3) → true
    assert(hasThreeSumLastDigits({1, 1, 1}) == true);

    // Only two duplicates, can't use same index twice → false
    assert(hasThreeSumLastDigits({2, 2, 1}) == false); // 2+2+? need 9 → no 9

    // Large case: many numbers, triple exists
    std::vector<int> large;
    for (int i = 0; i < 100000; ++i) large.push_back(i);
    assert(hasThreeSumLastDigits(large) == true);

    // No triple possible: all digits 0,4,5,6,7,8,9 (no combination sums to 3)
    assert(hasThreeSumLastDigits({0, 4, 5, 6, 7, 8, 9, 0, 4}) == false);

    // Edge: negative numbers? Not allowed per task, but modulo works in C++ (negative mod yields negative), assume non-negative.

    // Three numbers sum to 13 (ends in 3): 3+4+6=13
    assert(hasThreeSumLastDigits({3, 4, 6}) == true);

    // Sum ends in 3 but requires careful handling of same digit: 7+7+9=23
    assert(hasThreeSumLastDigits({7, 7, 9}) == true);

    // Need three same digit: 9+9+5=23
    assert(hasThreeSumLastDigits({9, 9, 5}) == true);

    // Only one triple of same digit: 3 copies of 1
    assert(hasThreeSumLastDigits({1, 1, 1}) == true);
    assert(hasThreeSumLastDigits({1, 1, 2, 2, 2}) == false); // 1+1+? need 1 but only two 1's → false

    return 0;
}
// The solution leverages a frequency array of size 10 to count how many numbers in the input have each last digit `0..9`. We then enumerate all ordered pairs `(i, e)` of distinct last digits (or the same digit if its frequency allows two distinct indices) by decrementing their counts temporarily, to ensure we don't reuse the same index. For each pair, the required third digit `need` is computed as `(3 - (i + e) % 10 + 10) % 10`. If the frequency of `need` is still positive after the two decrements, a valid triple exists. To avoid iterating over all possible pairs (100 pairs) for each test case, we precompute the frequency and then loop `i` from 0 to 9 and `e` from 0 to 9, but only when `freq[i] > 0`. This is O(100) per test case, which is constant time. Edge cases include: when the same last digit appears at least 3 times (we handle by decrementing twice and checking); when the required third digit equals `i` or `e` but we already used one or two occurrences, we ensure we don't overuse by checking the adjusted frequency. Time complexity: O(n) for reading and building frequency, plus O(100) for the triple check, so O(n) overall. Space: O(1) for the frequency array.
