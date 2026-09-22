// Write a standalone C++ function `findSequence` that takes a `std::vector<bool>` representing a binary input sequence and returns an `int` specifying the index (0-based) of the first occurrence of the pattern `1-0-1` in the sequence. The function should return `-1` if the pattern does not occur. The pattern must be contiguous, meaning the three bits appear consecutively in the input in the exact order `1, 0, 1`. The input sequence may be empty or contain fewer than three elements, in which case the function should return `-1`. The function must handle both short and long sequences efficiently and must not modify the input.

// The core problem is to search for a fixed-length contiguous pattern (length 3) in a vector of booleans. The simplest approach is a linear scan from index 0 to `size - 3`, checking at each position whether `vec[i] == true`, `vec[i+1] == false`, and `vec[i+2] == true`. If found, return `i` immediately. If the loop completes without a match, return `-1`. Edge cases include empty vector, vector with fewer than 3 elements, or when the pattern appears multiple times (we only need the first). No special handling is needed for overlapping occurrences since the loop naturally checks every possible start index. Time complexity is O(n) where n is the size of the vector, because each element is examined at most once (technically up to 3 times but constant factor). Space complexity is O(1) since we only use a few integer variables. This approach is optimal for a single pattern search because any algorithm must at least inspect the input once in the worst case.

#include <vector>

// Find the first occurrence of the pattern 1-0-1 in a binary sequence.
// Returns the starting index (0-based) of the first occurrence, or -1 if not found.
int findSequence(const std::vector<bool>& sequence) {
    const std::size_t n = sequence.size();
    if (n < 3) {
        return -1;
    }
    for (std::size_t i = 0; i + 2 < n; ++i) {
        if (sequence[i] == true && sequence[i + 1] == false && sequence[i + 2] == true) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

#include <cassert>
#include <vector>

int findSequence(const std::vector<bool>& sequence); // declaration from solution

int main() {
    // Basic pattern at start
    std::vector<bool> seq1 = {true, false, true};
    assert(findSequence(seq1) == 0);

    // Pattern in the middle
    std::vector<bool> seq2 = {false, true, false, true, false};
    assert(findSequence(seq2) == 1);

    // Pattern at the end
    std::vector<bool> seq3 = {true, true, true, false, true};
    assert(findSequence(seq3) == 2);

    // No pattern present
    std::vector<bool> seq4 = {true, true, false, false};
    assert(findSequence(seq4) == -1);

    // Empty vector
    std::vector<bool> seq5 = {};
    assert(findSequence(seq5) == -1);

    // Fewer than three elements
    std::vector<bool> seq6 = {true, false};
    assert(findSequence(seq6) == -1);

    // Multiple patterns, first one is returned
    std::vector<bool> seq7 = {false, true, false, true, true, false, true};
    assert(findSequence(seq7) == 1);

    // Pattern with false false false inside
    std::vector<bool> seq8 = {true, false, true, false, false, true};
    assert(findSequence(seq8) == 0);

    // All true
    std::vector<bool> seq9 = {true, true, true, true};
    assert(findSequence(seq9) == -1);

    // Long sequence with pattern near the end
    std::vector<bool> seq10(100, true);
    seq10[98] = true;
    seq10[99] = false;
    assert(findSequence(seq10) == -1); // since no 1-0-1
    std::vector<bool> seq10b = {true};  // trivial

    return 0;
}
