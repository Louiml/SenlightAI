// Write a C++ function `bool canMakeChangeForAll(const std::vector<int>& bills)` that simulates a lemonade stand where each customer buys one lemonade for exactly 5 dollars and pays with either a 5, 10, or 20 dollar bill. The seller starts with no change. The function should return `true` if the seller can give correct change to every customer in the order they appear in the `bills` vector, and `false` otherwise. You must track the available 5-dollar and 10-dollar bills, and handle 20-dollar bills by preferring to give one 10-dollar and one 5-dollar bill as change before falling back to three 5-dollar bills. The input vector may be empty (in which case the function returns `true`), and may contain any sequence of 5, 10, or 20. Use `const` for the input parameter and include necessary headers.
// The key is to always prioritize using the 10-dollar bill when making change for a 20-dollar payment, because 5-dollar bills are more versatile (needed for all change scenarios). Maintain two counters: `fiveCount` and `tenCount`. Iterate through each bill:
// - If it's a 5, increment `fiveCount` (no change needed).
// - If it's a 10, we need exactly one 5-dollar bill as change. If `fiveCount > 0`, decrement `fiveCount`, increment `tenCount`; otherwise return `false`.
// - If it's a 20, we need 15 dollars in change. The best strategy is: if we have at least one 10 and one 5, give those (decrement both). Otherwise, if we have at least three 5s, give three 5s (decrement `fiveCount` by 3). Otherwise, return `false`.
// Edge cases: empty input → `true`; a 20 when only 5s are available → must give three 5s; a 10 when no 5s → `false`. Time complexity is O(n), where n is the number of bills, and space complexity is O(1) (only two counters), excluding the input vector storage.
#include <vector>

// Determine whether the seller can give correct change for every lemonade purchase.
bool canMakeChangeForAll(const std::vector<int>& bills) {
    int fiveCount = 0;
    int tenCount = 0;

    for (int bill : bills) {
        if (bill == 5) {
            ++fiveCount;
        } else if (bill == 10) {
            if (fiveCount > 0) {
                --fiveCount;
                ++tenCount;
            } else {
                return false;
            }
        } else { // bill == 20
            if (fiveCount > 0 && tenCount > 0) {
                --fiveCount;
                --tenCount;
            } else if (fiveCount >= 3) {
                fiveCount -= 3;
            } else {
                return false;
            }
        }
    }
    return true;
}
#include <cassert>
#include <vector>

// Function declaration (already defined above, but included here for context)
bool canMakeChangeForAll(const std::vector<int>&);

int main() {
    // Individual valid scenarios
    assert(canMakeChangeForAll({5, 5, 5, 10, 20}) == true);
    assert(canMakeChangeForAll({5, 10, 5, 20}) == true);
    assert(canMakeChangeForAll({5, 5, 10, 20}) == true);
    assert(canMakeChangeForAll({5, 10, 5, 5, 20}) == true);
    assert(canMakeChangeForAll({}) == true);

    // Invalid scenarios
    assert(canMakeChangeForAll({10, 10}) == false);
    assert(canMakeChangeForAll({5, 20}) == false);
    assert(canMakeChangeForAll({5, 10, 20}) == false);
    assert(canMakeChangeForAll({20}) == false);
    assert(canMakeChangeForAll({5, 5, 5, 5, 10, 20, 20}) == false);

    return 0;
}
