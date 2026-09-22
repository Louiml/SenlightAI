Write a standalone C++ function named `recamanSequence` that takes a positive integer `n` and returns a `std::vector<int>` containing the first `n` terms of the Recamán's sequence (starting with 0). The sequence is defined as: `a(0) = 0`, and for each subsequent term `a(i)`, compute `candidate = a(i-1) - i`. If `candidate` is positive and has not appeared earlier in the sequence, use it; otherwise, use `a(i-1) + i`. The function must handle the case `n = 0` by returning an empty vector, and must produce the correct sequence for any non-negative `n` up to at least 10^5. Use a hash set (e.g., `std::unordered_set<int>`) to track previously seen values for fast lookups. Ensure the function is efficient in both time and space, and apply appropriate `const` correctness (e.g., parameters passed by value or const reference as needed).

// The main algorithm iterates from index 1 to `n-1`, maintaining the previous term `prev`. For each index `i`, it computes `next = prev - i`. If `next` is non-negative and not already present in the set of seen terms, it accepts `next`; otherwise, it sets `next = prev + i`. The new term is added to the result vector and inserted into the set. The initial term `0` is always added first (if `n > 0`), and the set is initialized with `0`. Edge cases: when `n = 0`, return an empty vector immediately; when `n = 1`, return `{0}`. The candidate `prev - i` can be negative (e.g., `i > prev`), so it must be checked against zero. Duplicate prevention is handled by the set. Time complexity is `O(n)` because each term is computed in constant time (set insert and find are `O(1)` average). Space complexity is `O(n)` for the result vector and the set, since we store all terms.

#include <vector>
#include <unordered_set>

// Return the first n terms of the Recamán's sequence.
// n is a non-negative integer; the sequence starts with a(0) = 0.
std::vector<int> recamanSequence(int n) {
    std::vector<int> result;
    if (n == 0) return result; // empty vector for n = 0
    
    result.reserve(n);
    std::unordered_set<int> seen;
    
    int prev = 0;
    result.push_back(prev);
    seen.insert(prev);
    
    for (int i = 1; i < n; ++i) {
        int candidate = prev - i;
        // Use candidate if it is non-negative and not already seen.
        // Otherwise, use prev + i.
        if (candidate < 0 || seen.count(candidate) > 0) {
            candidate = prev + i;
        }
        result.push_back(candidate);
        seen.insert(candidate);
        prev = candidate;
    }
    return result;
}

#include <cassert>
#include <vector>

// (The solution function is assumed to be defined above.)

int main() {
    // n = 0 -> empty
    assert(recamanSequence(0) == std::vector<int>{});
    
    // n = 1 -> {0}
    assert(recamanSequence(1) == std::vector<int>{0});
    
    // n = 2 -> {0, 1}
    assert(recamanSequence(2) == std::vector<int>({0, 1}));
    
    // n = 5 -> {0, 1, 3, 6, 2}
    assert(recamanSequence(5) == std::vector<int>({0, 1, 3, 6, 2}));
    
    // n = 10 -> {0, 1, 3, 6, 2, 7, 13, 20, 12, 21}
    assert(recamanSequence(10) == std::vector<int>({0, 1, 3, 6, 2, 7, 13, 20, 12, 21}));
    
    // n = 15 -> verify first 15 terms manually computed
    assert(recamanSequence(15) == std::vector<int>({0, 1, 3, 6, 2, 7, 13, 20, 12, 21, 11, 22, 10, 23, 9}));
    
    // Check that no adjacent duplication and all values are non-negative for a larger n
    std::vector<int> seq = recamanSequence(100);
    assert(seq.size() == 100);
    for (int i = 0; i < 100; ++i) {
        assert(seq[i] >= 0);
        if (i > 0) assert(seq[i] != seq[i-1]); // not strictly required but true for Recamán
    }
    
    // Check that all elements in first 100 are distinct (property of Recamán)
    std::unordered_set<int> unique(seq.begin(), seq.end());
    assert(unique.size() == 100);
    
    return 0;
}
