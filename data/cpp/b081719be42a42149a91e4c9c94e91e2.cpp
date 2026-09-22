/*
Write a C++ function `bool isB2Sequence(const std::vector<int>& sequence)` that determines whether a given sequence of positive integers is a "B2-Sequence". A sequence is a B2-Sequence if it is strictly increasing (each element is greater than the previous one) AND all pairwise sums of elements (including when i equals j, i.e., twice an element) are distinct. That is, for any two indices i ≤ j and k ≤ l, if the pairs (i,j) and (k,l) are not identical, then a[i]+a[j] ≠ a[k]+a[l]. The input sequence will have at least one element. If either the strictly increasing condition or the distinct-sum condition fails, the function should return `false`; otherwise, return `true`.
*/

#include <vector>
#include <unordered_set>

// Determine if a sequence is a B2-Sequence: strictly increasing and all pairwise sums distinct.
bool isB2Sequence(const std::vector<int>& sequence) {
    const std::size_t n = sequence.size();
    
    // Strictly increasing check.
    for (std::size_t i = 1; i < n; ++i) {
        if (sequence[i - 1] >= sequence[i]) {
            return false;
        }
    }
    
    // Track all pairwise sums (including self-pairs).
    std::unordered_set<int> seenSums;
    
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = i; j < n; ++j) {
            const int sum = sequence[i] + sequence[j];
            if (seenSums.find(sum) != seenSums.end()) {
                return false; // Duplicate sum found.
            }
            seenSums.insert(sum);
        }
    }
    
    return true;
}

#include <cassert>
#include <vector>

// Free function declaration (from solution).
bool isB2Sequence(const std::vector<int>& sequence);

int main() {
    // Basic valid B2-Sequence examples.
    assert(isB2Sequence({1}) == true);
    assert(isB2Sequence({1, 3}) == true);
    assert(isB2Sequence({1, 2, 4}) == true);
    assert(isB2Sequence({1, 3, 4}) == true); // sums: 2,4,5,6,7,8 all distinct.
    
    // Strictly increasing violation.
    assert(isB2Sequence({1, 1}) == false);
    assert(isB2Sequence({3, 2, 1}) == false);
    assert(isB2Sequence({1, 2, 2}) == false);
    
    // Duplicate sum violation.
    assert(isB2Sequence({1, 2, 3}) == false); // 1+3 = 2+2 = 4, duplicate.
    assert(isB2Sequence({2, 3, 4}) == false); // 2+4 = 3+3 = 6.
    assert(isB2Sequence({1, 3, 5}) == false); // 1+5 = 3+3 = 6.
    
    // Larger valid sequence (Sidon sequence).
    assert(isB2Sequence({1, 2, 4, 8}) == true);
    
    // Larger invalid sequence.
    assert(isB2Sequence({1, 2, 4, 7}) == false); // 1+7 = 4+4 = 8, duplicate.
    
    return 0;
}

// The solution processes the input in two phases. First, verify the strictly increasing property: iterate through the sequence once, and for any i>0, if the previous element is greater than or equal to the current element, the sequence is not strictly increasing, so return `false`. Second, compute all pairwise sums (including self-sums) using nested loops where the inner loop starts at the outer index to avoid duplicate pairs (since addition is commutative). Use a `std::unordered_set<int>` (or a boolean array if the maximum possible sum is known) to track seen sums. For each pair (i,j) with i≤j, compute `sum = a[i]+a[j]`; if the sum is already in the set, the sequence is not a B2-Sequence, return `false`; otherwise, insert it. After processing all pairs, return `true`. Edge cases: an empty sequence (but specification says at least one element) — handle gracefully by returning `true` or `false`? For at least one element, a single-element sequence is strictly increasing (vacuously) and has only one sum (twice the element), so it’s valid. The time complexity is O(N²) because of the double loop, and the space complexity is O(N²) in the worst case for the set, though typically there are at most N(N+1)/2 distinct sums.
