Write a C++ function `int minimumRemovalMoves(vector<int>& heights)` that takes a vector of positive integers representing student heights and returns the minimum number of moves needed to make the sequence non-decreasing. In each move, you simultaneously remove every student whose height is strictly less than the height of the student immediately in front of them (i.e., the student at index `i` is removed if `heights[i] < heights[i-1]`). After each move, the remaining students keep their original relative order, and the process repeats until the sequence is non-decreasing. The function must handle arrays of any length from 1 upward, including already sorted arrays (returning 0) and arrays with duplicate adjacent values (which are never removed). For example, given `{5, 6, 3, 3, 7, 1}`, the function should return 2, as described in the original problem.

// The straightforward approach is to simulate the process exactly as described. We repeatedly scan the current array, building a new array that contains only the elements that satisfy the non-decreasing condition relative to their immediately preceding element (the first element is always kept). When any element is excluded, we set a flag indicating that a removal happened. If no removals occur in a full pass, the array is already non-decreasing and we stop. Otherwise, we replace the original array with the new one and increment the move counter. This works because each move removes all offending elements simultaneously, and after removal the relative order of survivors is preserved, so the process is deterministic. Edge cases: a single-element array requires 0 moves; an already sorted array requires 0 moves; all equal elements require 0 moves; a strictly decreasing array like `{3,2,1}` requires 2 moves (first removes index 1 and 2, leaving `{3}`; then no more removals). The time complexity is \(O(n \cdot m)\) where `m` is the number of moves, and in the worst case (e.g., strictly decreasing) `m` is proportional to `n`, giving \(O(n^2)\). Space complexity is \(O(n)\) for the temporary vector during each move, but we can reuse vectors to reduce overhead; the asymptotic auxiliary space is \(O(n)\).

#include <vector>

// Returns the minimum number of moves to make heights non-decreasing.
// In each move, all elements strictly less than their predecessor are removed simultaneously.
int minimumRemovalMoves(std::vector<int>& heights) {
    int moves = 0;
    bool changed = true;

    while (changed) {
        changed = false;
        std::vector<int> next;

        if (heights.empty()) break;

        next.push_back(heights[0]);

        for (std::size_t i = 1; i < heights.size(); ++i) {
            if (heights[i] >= heights[i - 1]) {
                next.push_back(heights[i]);
            } else {
                changed = true;
            }
        }

        if (changed) {
            heights = std::move(next);
            ++moves;
        }
    }

    return moves;
}

#include <cassert>
#include <vector>

int minimumRemovalMoves(std::vector<int>& heights);

int main() {
    // Example from the problem statement
    std::vector<int> h1 = {5, 6, 3, 3, 7, 1};
    assert(minimumRemovalMoves(h1) == 2);

    // Already non-decreasing
    std::vector<int> h2 = {1, 2, 3, 4};
    assert(minimumRemovalMoves(h2) == 0);

    // Single element
    std::vector<int> h3 = {10};
    assert(minimumRemovalMoves(h3) == 0);

    // All equal
    std::vector<int> h4 = {4, 4, 4, 4};
    assert(minimumRemovalMoves(h4) == 0);

    // Strictly decreasing
    std::vector<int> h5 = {5, 4, 3, 2, 1};
    assert(minimumRemovalMoves(h5) == 4);

    // Two elements: decreasing
    std::vector<int> h6 = {7, 3};
    assert(minimumRemovalMoves(h6) == 1);

    // Two elements: equal
    std::vector<int> h7 = {3, 3};
    assert(minimumRemovalMoves(h7) == 0);

    // A case where only some are removed
    std::vector<int> h8 = {2, 1, 2, 3, 1, 5};
    assert(minimumRemovalMoves(h8) == 2);

    // Empty vector (though not specified, handle gracefully)
    std::vector<int> h9 = {};
    assert(minimumRemovalMoves(h9) == 0);

    return 0;
}
