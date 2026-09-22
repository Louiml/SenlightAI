// Write a C++ function `bool isValidBracketSequence(const std::array<int, 4>& counts)` that takes the counts of four types of brackets (index 0 = round brackets `(` `)`, index 1 = square brackets `[` `]`, index 2 = curly brackets `{` `}`, index 3 = angle brackets `<` `>`) and returns `true` if the bracket sequence is valid under the given rules. A sequence is valid if and only if: (1) The counts of opening and closing brackets are equal for each type, i.e., `counts[0] == counts[3]`? Actually, the rule from the snippet is: the number of round-opening brackets (index 0) must equal the number of round-closing brackets (index 3) — note the snippet uses `Num[0]` and `Num[3]` as the pair. Similarly, for consistency, the problem states that: for a valid sequence, the counts of `(` (index 0) and `)` (index 3) must be equal. Additionally, if there is any curly bracket (index 2 > 0), then there must be at least one round-opening and one round-closing bracket (`Num[0] >= 1` and `Num[3] >= 1`). Return `true` if both conditions hold, otherwise `false`. The input array always has exactly 4 non-negative integers. Do not include a `main` function; only the free function.

// The solution directly implements the two validity rules from the snippet. First, check if the count of curly brackets (index 2) is non-zero; if so, ensure both `counts[0]` and `counts[3]` are at least 1. If this condition fails, return `false`. Second, check if `counts[0]` equals `counts[3]`; if not, return `false`. If both checks pass, return `true`. Edge cases: all counts zero is valid (empty sequence); only curly brackets with no round brackets is invalid; equal but zero counts for round brackets with no curly brackets is valid; curly brackets present with at least one round pair is valid. Time complexity is O(1) since only four array elements are examined; space complexity O(1).

#include <array>

// Returns true if the bracket counts satisfy the validity rules:
// 1) Counts of round opening (index 0) and round closing (index 3) must be equal.
// 2) If curly brackets (index 2) exist, then at least one round opening and one round closing must exist.
bool isValidBracketSequence(const std::array<int, 4>& counts) {
    // If curly brackets are present, require at least one round-opening and one round-closing bracket.
    if (counts[2] != 0) {
        if (counts[0] < 1 || counts[3] < 1) {
            return false;
        }
    }
    // Opening and closing round bracket counts must match.
    return counts[0] == counts[3];
}

#include <array>
#include <cassert>

bool isValidBracketSequence(const std::array<int, 4>& counts);

int main() {
    // All zeros (empty sequence) is valid.
    assert(isValidBracketSequence({0, 0, 0, 0}) == true);
    // Equal round brackets, no curly brackets, any square/angle counts: valid.
    assert(isValidBracketSequence({2, 5, 0, 2}) == true);
    // Unequal round brackets: invalid.
    assert(isValidBracketSequence({1, 0, 0, 2}) == false);
    // Curly brackets present but no round openings: invalid.
    assert(isValidBracketSequence({0, 0, 1, 0}) == false);
    // Curly brackets present but no round closings: invalid.
    assert(isValidBracketSequence({1, 0, 1, 0}) == false);
    // Curly brackets present with at least one round pair and equal round counts: valid.
    assert(isValidBracketSequence({1, 0, 1, 1}) == true);
    // Curly brackets present, round counts equal and non-zero: valid.
    assert(isValidBracketSequence({3, 2, 2, 3}) == true);
    // Unequal round counts with curly brackets present: invalid.
    assert(isValidBracketSequence({2, 0, 1, 3}) == false);
    // Equal round counts non-zero, no curly brackets: valid even with other brackets.
    assert(isValidBracketSequence({0, 9, 0, 0}) == true);
    // Single round pair, zero curly: valid.
    assert(isValidBracketSequence({1, 0, 0, 1}) == true);
    return 0;
}
