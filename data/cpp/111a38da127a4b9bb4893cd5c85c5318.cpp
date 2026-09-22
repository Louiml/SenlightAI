Write a C++ function `constructNumber` that takes two integers `N` (number of digits, where the first digit cannot be zero when `N > 1`) and `M` (number of constraints), followed by `M` pairs `(s, c)` where `s` is a digit position (1-indexed) that must equal `c` (0-9). The function must return the smallest possible `N`-digit number that satisfies all constraints, as a string. If no such number exists (due to conflicting constraints on the same position, or if the first digit is forced to be 0 when `N > 1`), return the string `"-1"`. Positions not mentioned in any constraint should be filled with the smallest possible digit (1 for the first position when `N > 1`, 0 otherwise) to produce the minimal number. The input is guaranteed to have at least one constraint (M ≥ 1), and N, M, s, c are given in a single line order: `N M` then `M` lines of `s c`. The output must be the constructed number or `-1`.
// The solution uses a fixed-size vector `digit(N, -1)` to store the determined value for each position. For each constraint `(s, c)`, we check if position `s-1` (0-indexed) is either unset (-1) or already equal to `c`; if it already holds a different value, we immediately detect a conflict and return `"-1"`. After processing all constraints, we check the special first-digit rule: if `N > 1` and the first position (index 0) is set to `0`, then no valid N-digit number exists, so return `"-1"`. Finally, we fill any remaining unset positions with the smallest allowed digit: for index 0 when `N > 1`, we use `1`; for all other positions, we use `0`. This guarantees the smallest possible number. The time complexity is O(N + M) because we iterate over M constraints and then N positions, and the space complexity is O(N) for the digit vector.
#include <string>
#include <vector>

// Given N (number of digits), M (constraint count), and M pairs (s, c),
// return the smallest valid N-digit number as a string, or "-1".
std::string constructNumber(int N, int M, const std::vector<std::pair<int, int>>& constraints) {
    std::vector<int> digit(N, -1);
    
    for (const auto& cons : constraints) {
        int pos = cons.first - 1;  // convert to 0-indexed
        int val = cons.second;
        if (digit[pos] == -1 || digit[pos] == val) {
            digit[pos] = val;
        } else {
            return "-1";
        }
    }
    
    // First digit cannot be zero for numbers with more than one digit.
    if (N != 1 && digit[0] == 0) {
        return "-1";
    }
    
    // Fill unspecified positions with minimal possible digits.
    for (int i = 0; i < N; ++i) {
        if (digit[i] == -1) {
            if (i == 0 && N != 1) {
                digit[i] = 1;
            } else {
                digit[i] = 0;
            }
        }
    }
    
    std::string result;
    result.reserve(N);
    for (int d : digit) {
        result.push_back('0' + d);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Declare the function (already included above via solution, but for test completeness we re-declare)
std::string constructNumber(int N, int M, const std::vector<std::pair<int, int>>& constraints);

int main() {
    // Example 1: N=3, constraints: (1,1), (2,2) => Result "120"
    assert(constructNumber(3, 2, {{1,1},{2,2}}) == "120");
    
    // Example 2: N=1, constraint: (1,0) => Result "0" (allowed for 1-digit)
    assert(constructNumber(1, 1, {{1,0}}) == "0");
    
    // Example 3: N=2, constraint (1,0) => impossible => "-1"
    assert(constructNumber(2, 1, {{1,0}}) == "-1");
    
    // Example 4: Conflicting constraints on same position: N=3, (1,1) and (1,2) => "-1"
    assert(constructNumber(3, 2, {{1,1},{1,2}}) == "-1");
    
    // Example 5: N=4, constraints: (2,5), (4,9) => fill 1st with 1, 3rd with 0 => "1509"
    assert(constructNumber(4, 2, {{2,5},{4,9}}) == "1509");
    
    // Example 6: N=3, constraints: (3,7) => fill first with 1, second with 0 => "107"
    assert(constructNumber(3, 1, {{3,7}}) == "107");
    
    // Example 7: N=1, multiple constraints same position: (1,0) and (1,0) => "0"
    assert(constructNumber(1, 2, {{1,0},{1,0}}) == "0");
    
    // Example 8: N=2, constraints: (2,8) => first digit 1, second 8 => "18"
    assert(constructNumber(2, 1, {{2,8}}) == "18");
    
    return 0;
}
