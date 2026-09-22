/*
Write a C++ function named `canDefeatMonster` that takes three parameters: an integer `requiredDamage` (representing the monster's HP), an integer `n` (representing the number of available attacks), and a `const std::vector<long long>&` of attack damages. The function should return `true` if the sum of all attack damages is at least `requiredDamage`, and `false` otherwise. The function must handle cases where `n` is zero, where the attack list contains negative or zero values, and where the sum might overflow an `int` — so use `long long` for the sum.
*/
#include <vector>
#include <numeric>

// Returns true if the total attack damage can defeat the monster.
bool canDefeatMonster(long long requiredDamage, int n, const std::vector<long long>& attacks) {
    // Sum all attack damages, starting from 0.
    long long totalDamage = 0;
    for (int i = 0; i < n; ++i) {
        totalDamage += attacks[i];
    }
    // If the sum meets or exceeds required damage, victory is possible.
    return totalDamage >= requiredDamage;
}
#include <cassert>
#include <vector>

// The function under test is declared here (assume it is included from the solution).
bool canDefeatMonster(long long requiredDamage, int n, const std::vector<long long>& attacks);

int main() {
    // Basic victory and defeat cases.
    assert(canDefeatMonster(10, 3, {5, 5, 5}) == true);
    assert(canDefeatMonster(11, 3, {5, 5, 5}) == false);

    // Empty attack list.
    assert(canDefeatMonster(0, 0, {}) == true);
    assert(canDefeatMonster(1, 0, {}) == false);

    // Negative and zero damage values.
    assert(canDefeatMonster(5, 3, {3, -2, 4}) == true); // sum=5
    assert(canDefeatMonster(6, 3, {3, -2, 4}) == false); // sum=5

    // Large values to check overflow safety with long long.
    std::vector<long long> large(100000, 1000000000LL);
    assert(canDefeatMonster(99999999999LL, 100000, large) == true); // sum=1e14
    assert(canDefeatMonster(100000000000LL, 100000, large) == true); // exactly equal

    // Required damage exactly equal to sum.
    assert(canDefeatMonster(7, 3, {2, 3, 2}) == true);
    assert(canDefeatMonster(8, 3, {2, 3, 2}) == false);
}
// The core idea is straightforward: compute the total damage by summing all elements in the attack vector, then compare it against the monster's HP. If the total is greater than or equal to the required damage, return `true`; otherwise `false`. The main edge cases are: (1) empty attack list — the sum is 0, so the function returns `false` unless `requiredDamage` is 0 or negative (in which case `true` since 0 >= requiredDamage); (2) negative damage values — they simply reduce the total, but the logic remains unchanged; (3) potential overflow — using `long long` avoids overflow for typical input sizes up to 10^5 elements each up to 10^9. The algorithm runs in O(n) time and uses O(1) extra space, as we only maintain a running sum.
