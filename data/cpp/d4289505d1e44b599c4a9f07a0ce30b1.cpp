// Write a C++ function `bool isGroupValid(const std::vector<int>& schedule)` that determines whether a binary sequence representing daily presence (1 = present, 0 = absent) satisfies a strict spacing rule: any two consecutive 1s in the sequence must be separated by at least 6 positions (i.e., the distance between their indices must be at least 6, meaning there must be at least 5 zeros between them). The function should return `true` if the rule is satisfied for all pairs of consecutive 1s, and `false` otherwise. The input is a non-empty vector of integers where each element is either 0 or 1. For example, for a vector `{1,0,0,0,0,0,1}`, the distance between the two 1s is 6, so it is valid; for `{1,0,0,0,0,1}`, the distance is 5, which is invalid (violation). The function must treat the sequence as circular? No, treat it as linear only. Consider edge cases such as all zeros, a single 1, or multiple 1s that are far apart. The solution should be efficient and avoid unnecessary checks.
#include <cassert>
#include <vector>

// Forward declaration of the function under test
bool isGroupValid(const std::vector<int>& schedule);

int main() {
    // Valid: single 1
    assert(isGroupValid({0, 1, 0}) == true);
    // Valid: all zeros
    assert(isGroupValid({0, 0, 0, 0}) == true);
    // Valid: exactly 6 apart (indices 0 and 6)
    assert(isGroupValid({1, 0, 0, 0, 0, 0, 1}) == true);
    // Valid: more than 6 apart (indices 2 and 10)
    assert(isGroupValid({0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}) == true);
    // Invalid: exactly 5 apart (indices 0 and 5)
    assert(isGroupValid({1, 0, 0, 0, 0, 1}) == false);
    // Invalid: two 1s adjacent (indices 3 and 4)
    assert(isGroupValid({0, 0, 0, 1, 1, 0}) == false);
    // Invalid: one violation among many (indices 1, 4, 12)
    assert(isGroupValid({0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}) == false);
    // Valid: sparse 1s (indices 0, 8, 16)
    assert(isGroupValid({1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1}) == true);
    return 0;
}
#include <vector>

// Returns true if every pair of consecutive 1s in the binary vector
// are at least 6 indices apart (i.e., distance >= 6).
bool isGroupValid(const std::vector<int>& schedule) {
    int lastOneIndex = -6; // virtual position so first 1 always passes
    for (int i = 0; i < static_cast<int>(schedule.size()); ++i) {
        if (schedule[i] == 1) {
            if (i - lastOneIndex < 6) {
                return false; // violation: too close
            }
            lastOneIndex = i;
        }
    }
    return true;
}
// The algorithm scans the vector from left to right. We maintain an index of the last occurrence of a 1. When we encounter a new 1, we check the distance between this index and the previous 1's index. If the difference is less than 6, the rule is violated and we return `false`. Otherwise, we update the last index to the current position. If we finish scanning without violations, return `true`. Edge cases: if there are fewer than two 1s, the rule is vacuously true. The vector can contain leading/trailing zeros, which do not affect the logic. The time complexity is O(n) where n is the length of the vector, and space complexity is O(1) auxiliary. We only need one integer to store the last seen 1 index. The initial value of last index can be set to `-6` to simplify the first check (since the distance from any first 1 to -6 is at least 6), or we can handle the first 1 separately.
