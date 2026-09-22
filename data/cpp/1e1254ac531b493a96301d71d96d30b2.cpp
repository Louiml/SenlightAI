// Write a C++ function that simulates a simplified version of the goal-scoring logic from the provided code snippet. Given a vector of integers representing the current scores of two players (index 0 = left player, index 1 = right player) and an integer `goalSide` that is either `0` (left goal scored on) or `1` (right goal scored on), the function should increment the score of the *opposite* player (because scoring into the left goal gives a point to the right player, and vice versa), then return the updated vector. The function must handle edge cases such as invalid `goalSide` values (e.g., `-1` or `2`) by returning the original vector unchanged, and must not modify the input vector (i.e., return a new vector). The function signature should be `std::vector<int> updateScore(const std::vector<int>& scores, int goalSide)`.
// The solution mimics the core score-update logic from the snippet: when a ball enters the left goal (tag `0`), the right player's score is incremented; when it enters the right goal (tag `1`), the left player's score is incremented. The main algorithm is straightforward: first, check if `goalSide` is valid (must be exactly `0` or `1`); if not, return a copy of the input unchanged. Otherwise, create a copy of the input vector, then increment the element at index `(1 - goalSide)` (since `1 - 0 = 1` and `1 - 1 = 0`). This works because the vector size is assumed to be exactly 2 (as in the original game where there are exactly two scores). Edge cases include an empty vector or a vector with fewer than 2 elements—the function should handle these gracefully by returning a copy unchanged, since there is no valid score to update. For a vector of size exactly 2, the time complexity is O(1) (copying is O(2)), and space complexity is O(1) for the returned vector (constant size). If the vector size were arbitrary, copying would be O(n), but the problem specifies two scores, so it's constant.
#include <vector>
#include <cstddef>

// Increment the score of the player opposite to the goal side.
// goalSide: 0 = left goal (right player scores), 1 = right goal (left player scores).
// Returns a new vector with updated scores; returns a copy unchanged if input is invalid.
std::vector<int> updateScore(const std::vector<int>& scores, int goalSide) {
    // Return a copy if the input is malformed or goalSide is not 0 or 1.
    if (scores.size() != 2 || (goalSide != 0 && goalSide != 1)) {
        return scores;
    }

    // Create a copy to modify.
    std::vector<int> result = scores;
    // Opposite index: if goalSide==0, update index 1; if goalSide==1, update index 0.
    result[1 - goalSide] += 1;
    return result;
}
#include <cassert>
#include <vector>

int main() {
    // Normal cases
    assert(updateScore({0, 0}, 0) == std::vector<int>({0, 1})); // left goal -> right scores
    assert(updateScore({0, 0}, 1) == std::vector<int>({1, 0})); // right goal -> left scores
    assert(updateScore({5, 3}, 0) == std::vector<int>({5, 4}));
    assert(updateScore({5, 3}, 1) == std::vector<int>({6, 3}));

    // Edge cases: invalid goalSide
    assert(updateScore({2, 2}, -1) == std::vector<int>({2, 2}));
    assert(updateScore({2, 2}, 2) == std::vector<int>({2, 2}));

    // Edge cases: wrong vector size
    assert(updateScore({}, 0) == std::vector<int>({}));
    assert(updateScore({1}, 0) == std::vector<int>({1}));
    assert(updateScore({1, 2, 3}, 1) == std::vector<int>({1, 2, 3}));

    // Input vector is not modified (copy semantics)
    std::vector<int> original = {0, 0};
    std::vector<int> result = updateScore(original, 0);
    assert(original == std::vector<int>({0, 0}));
    assert(result == std::vector<int>({0, 1}));
}
