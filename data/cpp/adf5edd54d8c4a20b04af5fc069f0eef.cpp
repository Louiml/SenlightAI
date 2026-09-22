Write a C++ function named `canPerformAllHits` that takes two integers `requiredHp` and `attackCount` (both non-negative), followed by a vector of non-negative integers representing the damage dealt by each attack, and returns a boolean indicating whether the total damage across all attacks is at least `requiredHp`. The function must handle the case where `attackCount` is zero (total damage is 0) and where some damage values are zero. The function should be const-correct and not accept a size parameter separately; instead, the size is derived from the vector.
The solution simply sums all elements of the damage vector and compares the total to `requiredHp`. Since all values are non-negative, there are no negative overflow or sign issues, but we must consider that the sum could exceed the range of `int` if many large values are used; for this task, we assume inputs fit within a 64-bit signed integer, so use `long long` for the accumulator. Edge cases: an empty vector (attackCount = 0) yields total damage 0; if `requiredHp` is 0, any attack set (including empty) returns true because 0 >= 0. Time complexity is O(n) where n is the number of attacks, and space complexity is O(1) beyond the input vector.
#include <vector>
#include <numeric>

// Return true if the total damage from all attacks is at least requiredHp.
bool canPerformAllHits(int requiredHp, const std::vector<int>& damages) {
    long long totalDamage = 0;
    for (int damage : damages) {
        totalDamage += damage;
    }
    return totalDamage >= requiredHp;
}
#include <cassert>
#include <vector>

// The function declaration is provided separately; here we include it for completeness.
bool canPerformAllHits(int requiredHp, const std::vector<int>& damages);

int main() {
    // Basic case: sum meets requirement
    assert(canPerformAllHits(10, {3, 4, 3}) == true);
    // Exact match
    assert(canPerformAllHits(7, {2, 5}) == true);
    // Not enough damage
    assert(canPerformAllHits(100, {10, 20}) == false);
    // Zero attacks
    assert(canPerformAllHits(0, {}) == true);
    assert(canPerformAllHits(1, {}) == false);
    // Zero required HP always true
    assert(canPerformAllHits(0, {0, 0}) == true);
    // Large values, sum exceeds int range
    assert(canPerformAllHits(2000000000, {1000000000, 1000000000}) == true);
    // Zero damage attacks
    assert(canPerformAllHits(5, {0, 5}) == true);
    assert(canPerformAllHits(6, {0, 5}) == false);
    return 0;
}
