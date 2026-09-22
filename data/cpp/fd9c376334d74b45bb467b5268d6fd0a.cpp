Write a C++ function `bool isStrictlyIncreasingAfterRemoval(const std::vector<int>& sequence)` that takes a vector of integers and returns `true` if it is possible to obtain a strictly increasing sequence by removing at most one element from the vector without changing the order of the remaining elements. A sequence is considered strictly increasing if each element is strictly less than the next one. The function must handle vectors of any size (including empty or single-element vectors, which are trivially valid because no removals or zero removals are needed). Edge cases include duplicate adjacent values, a single "bad" element that breaks increasing order, and sequences that require removing an element to fix multiple violations (e.g., `[1, 3, 2]` → remove `3` works, but `[1, 3, 2, 4]` → remove `3` works too). The solution must be iterative, avoid modifying the input, and have O(n) time complexity.
#include <cassert>
#include <vector>

// (The solution function is expected to be included above.)

int main() {
    // Already strictly increasing.
    assert(isStrictlyIncreasingAfterRemoval({}) == true);
    assert(isStrictlyIncreasingAfterRemoval({7}) == true);
    assert(isStrictlyIncreasingAfterRemoval({1, 2, 3, 4}) == true);

    // One element can be removed to fix.
    assert(isStrictlyIncreasingAfterRemoval({1, 3, 2}) == true);      // remove 3
    assert(isStrictlyIncreasingAfterRemoval({2, 1, 3}) == true);      // remove 2
    assert(isStrictlyIncreasingAfterRemoval({1, 2, 2, 3}) == true);   // remove one duplicate
    assert(isStrictlyIncreasingAfterRemoval({1, 5, 2, 3}) == true);   // remove 5

    // Cannot be fixed by removing at most one element.
    assert(isStrictlyIncreasingAfterRemoval({1, 3, 2, 4, 3}) == false);
    assert(isStrictlyIncreasingAfterRemoval({3, 2, 1}) == false);
    assert(isStrictlyIncreasingAfterRemoval({1, 2, 2, 2}) == false);
    assert(isStrictlyIncreasingAfterRemoval({1, 2, 1, 2}) == false);

    // Larger vector with a single violation.
    assert(isStrictlyIncreasingAfterRemoval({1, 2, 3, 10, 4, 5}) == true); // remove 10

    // Larger vector with multiple violations.
    assert(isStrictlyIncreasingAfterRemoval({1, 2, 10, 3, 4, 2}) == false);

    return 0;
}
#include <vector>

// Returns true if the sequence can become strictly increasing by removing at most one element.
bool isStrictlyIncreasingAfterRemoval(const std::vector<int>& sequence) {
    const int n = static_cast<int>(sequence.size());
    
    // Empty or single-element sequences are always valid.
    if (n <= 1) {
        return true;
    }

    // Find the first index where the strictly increasing property is violated.
    int violation_index = -1;
    for (int i = 0; i < n - 1; ++i) {
        if (sequence[i] >= sequence[i + 1]) {
            violation_index = i;
            break;
        }
    }

    // If no violation, the sequence is already strictly increasing.
    if (violation_index == -1) {
        return true;
    }

    // Helper lambda: checks whether the sequence is strictly increasing
    // after removing the element at 'skip_index'.
    auto is_valid_after_removal = [&](int skip_index) {
        int previous = -1;
        bool first_element = true;
        for (int i = 0; i < n; ++i) {
            if (i == skip_index) {
                continue;
            }
            if (!first_element && previous >= sequence[i]) {
                return false;
            }
            previous = sequence[i];
            first_element = false;
        }
        return true;
    };

    // Try removing either of the two elements that caused the violation.
    return is_valid_after_removal(violation_index) ||
           is_valid_after_removal(violation_index + 1);
}
// The main idea is to scan the vector to find the first index where the strictly increasing property is violated (i.e., `sequence[i] >= sequence[i+1]`). If no violation exists, the sequence is already strictly increasing, so return `true` immediately. If there is a violation, we may remove either `sequence[i]` or `sequence[i+1]`, because removing any other element would not fix this specific violation. To decide which removal works, we check two possibilities: (1) remove `sequence[i]` and verify that the remaining sequence is strictly increasing from the beginning to the end, but only need to check the neighborhood around `i` because the rest of the sequence was already strictly increasing before the violation; specifically, after removing `i`, the new pair `(sequence[i-1], sequence[i+1])` must be strictly increasing (if `i-1` exists). (2) Similarly, remove `sequence[i+1]` and verify `(sequence[i], sequence[i+2])` is strictly increasing (if `i+2` exists). If either removal yields a valid sequence, return `true`. If both fail, return `false`. However, we must be careful: even if the local check passes, there could be another violation later in the sequence that is unrelated to the first violation. The simplest robust approach: after finding the first violation index `i`, we test both removal scenarios by building a new vector without that element (or by checking the full sequence with a skip flag during one pass). To avoid copying a potentially large vector, we can scan the entire vector once for each candidate removal, but that would be O(n) per candidate, giving O(n) total since we only have two candidates. Alternatively, we can do a single pass that skips one element. The cleanest implementation: write a helper lambda that returns `true` if the vector is strictly increasing when we skip one specific index. For each candidate (either `i` or `i+1`), run a linear scan over the original vector, skipping that index, and verify each adjacent pair is strictly increasing. This is O(n) per candidate, and there are at most 2 candidates, so total O(n) time and O(1) extra space (excluding the input). Edge cases: empty vector (size 0) returns `true`; size 1 returns `true`; if the first violation is at index `i`, both candidates may work (e.g., `[1, 2, 2, 3]` – violation at `i=1` (2>=2), removing either the first 2 or the second 2 works, so return `true`). If there are multiple violations and removing one element cannot fix all, return `false` (e.g., `[1, 3, 2, 4, 3]` – first violation at `i=1` (3>=2), removing 3 gives `[1,2,4,3]` still has violation, removing 2 gives `[1,3,4,3]` still has violation → false). The time complexity is O(n) with a constant factor of 2, and space complexity is O(1).
