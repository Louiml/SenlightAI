Write a C++ function that takes a string `s` and a vector of integers `indices` of the same length as `s`. The function must return a new string where each character from `s` at position `i` is placed at position `indices[i]` in the output. The `indices` vector is a permutation of `0` to `n-1` (where `n` is the length of the string), meaning every index appears exactly once. The original string and indices are not modified. The function must handle empty strings correctly, returning an empty string. For example, given `s = "codeleet"` and `indices = {4,5,6,7,0,1,2,3}`, the result is `"leetcode"`.

The solution is straightforward: create a result string of the same length as the input (using `resize`), then iterate through each index `i` from `0` to `n-1`, assigning `result[indices[i]] = s[i]`. This directly places each character at its target position. Since `indices` is guaranteed to be a valid permutation, no position is overwritten or left unassigned. The main edge case is an empty string: `resize(0)` and the loop run zero times, returning an empty string. No additional checks are needed because the problem guarantees the permutation property. Time complexity is O(n) where `n` is the length of the string, because we perform one assignment per character. Space complexity is O(n) for the result string, not counting the input.

#include <string>
#include <vector>

// Shuffle a string based on the given permutation of indices.
// Each character s[i] is placed at position indices[i] in the result.
std::string restoreString(const std::string& s, const std::vector<int>& indices) {
    std::string result;
    result.resize(s.size());

    for (std::size_t i = 0; i < indices.size(); ++i) {
        result[indices[i]] = s[i];
    }

    return result;
}

#include <cassert>
#include <string>
#include <vector>

int main() {
    assert(restoreString("codeleet", {4,5,6,7,0,1,2,3}) == "leetcode");
    assert(restoreString("abc", {0,1,2}) == "abc");
    assert(restoreString("abc", {2,1,0}) == "cba");
    assert(restoreString("a", {0}) == "a");
    assert(restoreString("", {}) == "");
    assert(restoreString("xyn", {1,0,2}) == "yxn");
    assert(restoreString("abcd", {1,3,0,2}) == "cadb");
    assert(restoreString("hello", {4,3,2,1,0}) == "olleh");
}
