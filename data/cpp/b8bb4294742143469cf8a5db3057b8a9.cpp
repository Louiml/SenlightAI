// Write a C++ function `smallestNumberFromPattern` that accepts a string `pattern` consisting only of the characters `'I'` (increase) and `'D'` (decrease). The function must return the lexicographically smallest string of digits (each digit from `1` to `9`, used exactly once) such that for every consecutive pair of positions, the relationship matches the pattern: an `'I'` means the next digit must be greater than the previous, and a `'D'` means the next digit must be smaller than the previous. The output string will always have length equal to `pattern.size() + 1`. Assume the input is non-empty and contains only `'I'` and `'D'`. If no valid sequence exists, return an empty string (though for the given constraints, a solution always exists because we have 9 digits available and the pattern length is at most 8). The function must be efficient for patterns up to length 8. The solution must not use any additional library beyond the standard C++ headers.

The problem is a classic backtracking search over permutations of digits 1–9. We need to assign a digit to each position (total positions = pattern length + 1) such that the adjacent relationships match the pattern. Since the pattern length is at most 8, there are at most 9 positions and at most 9! = 362,880 permutations, which is small enough for brute force with pruning. The main algorithm starts by trying each possible first digit (1 to 9), then for each subsequent position, only tries digits that satisfy the relationship with the previous chosen digit (greater than for `'I'`, less than for `'D'`). To keep the result lexicographically smallest, we explore candidates in ascending order and stop at the first complete permutation found. We must track which digits have been used using a boolean vector. Edge cases include patterns with all `'I'` (e.g., "III" → "1234") and all `'D'` (e.g., "DD" → "321"), and mixed patterns. Time complexity is O(9!) in the worst case, but pruning drastically reduces it; with the ascending-order exploration, the first solution found is the smallest. Space complexity is O(n) for recursion stack and O(9) for the visited array.

#include <string>
#include <vector>
#include <functional>

// Returns the lexicographically smallest digit string (digits 1-9, each used once)
// that satisfies the increase/decrease pattern. Returns empty string if impossible.
std::string smallestNumberFromPattern(const std::string& pattern) {
    const int n = pattern.size() + 1;  // length of result
    std::string result;
    std::vector<bool> used(10, false);  // digits 1-9

    // Recursive backtracking to build sequence.
    std::function<bool(std::string&, int)> dfs = [&](std::string& current, int pos) -> bool {
        if (pos == n) {
            result = current;
            return true;
        }
        // Try digits in ascending order for lexicographic minimality.
        for (int d = 1; d <= 9; ++d) {
            if (!used[d]) {
                // Check relationship with previous digit if not first position.
                if (pos > 0) {
                    char prev = current.back();
                    if (pattern[pos - 1] == 'I' && d <= (prev - '0')) continue;
                    if (pattern[pos - 1] == 'D' && d >= (prev - '0')) continue;
                }
                used[d] = true;
                current.push_back(static_cast<char>('0' + d));
                if (dfs(current, pos + 1)) return true;
                current.pop_back();
                used[d] = false;
            }
        }
        return false;
    };

    std::string current;
    if (dfs(current, 0)) {
        return result;
    }
    return "";
}

#include <cassert>
#include <string>

// Function declaration from solution
std::string smallestNumberFromPattern(const std::string& pattern);

int main() {
    // Basic increase and decrease patterns.
    assert(smallestNumberFromPattern("I") == "12");
    assert(smallestNumberFromPattern("D") == "21");

    // All increases -> ascending sequence.
    assert(smallestNumberFromPattern("III") == "1234");

    // All decreases -> descending sequence.
    assert(smallestNumberFromPattern("DDD") == "4321");

    // Mixed patterns.
    assert(smallestNumberFromPattern("ID") == "132"); // 1<3>2
    assert(smallestNumberFromPattern("DI") == "213"); // 2>1<3

    // Longer pattern requiring careful selection.
    assert(smallestNumberFromPattern("IIID") == "12354"); // 1<2<3<5>4
    assert(smallestNumberFromPattern("DDI") == "3214"); // 3>2>1<4

    // Edge pattern of length 8 (9 digits).
    assert(smallestNumberFromPattern("IIIIIIII") == "123456789");
    assert(smallestNumberFromPattern("DDDDDDDD") == "987654321");

    // Pattern with alternating signs.
    assert(smallestNumberFromPattern("IDID") == "13254"); // 1<3>2<5>4

    return 0;
}
