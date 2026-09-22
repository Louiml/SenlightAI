// Given a string `s` consisting of lowercase English letters and an integer `k` (1 ≤ k ≤ s.length), write a C++ function `orderlyQueue` that returns the lexicographically smallest string obtainable by repeatedly applying this operation any number of times: take one of the first `k` characters of the current string and move it to the end. For example, if `k=2` and the current string is `"abc"`, you may move `'a'` or `'b'` to the end, producing `"bca"` or `"cab"`. The function must handle two distinct cases: when `k == 1`, only the first character can be moved each time, so the set of reachable strings is exactly all cyclic rotations of the original string; when `k > 1`, it can be proven that any permutation of the original characters is reachable, so the answer is simply the sorted string. The solution should not modify the input string and should return a new string. Assume the input is always valid and non-empty.
// For `k == 1`, the allowed operation is: move the first character to the end. Repeating this operation produces every cyclic rotation of `s`. To find the lexicographically smallest among all rotations, generate each rotation by taking `s.substr(i) + s.substr(0, i)` for each index `i` from `0` to `s.size()-1`, and track the minimum using standard string comparison (`<`). Edge case: when the string has length 1, only one rotation exists, and the loop finds it trivially. For `k > 1`, because you can move any of the first `k` characters to the end, and with `k ≥ 2` you can swap adjacent characters (by moving the second character to the end, then moving the first character, etc.), you can achieve any permutation of the original characters. The lexicographically smallest permutation is the characters sorted in ascending order. Thus, just sort the string and return it. Time complexity: `O(n²)` for the `k == 1` case due to generating `n` strings of length `n` and comparing, but for `k > 1` it is `O(n log n)` for sorting. Space complexity: `O(n)` for the temporary rotated string in the `k == 1` case, and `O(1)` extra for sorting (assuming in-place sort).
#include <string>
#include <algorithm>

// Return the lexicographically smallest string obtainable by moving
// any of the first k characters to the end, repeated any number of times.
std::string orderlyQueue(const std::string& s, int k) {
    if (k == 1) {
        std::string best = s;
        const std::size_t n = s.size();
        for (std::size_t i = 0; i < n; ++i) {
            std::string rotation = s.substr(i) + s.substr(0, i);
            if (rotation < best) {
                best = rotation;
            }
        }
        return best;
    } else {
        std::string result = s;
        std::sort(result.begin(), result.end());
        return result;
    }
}
#include <cassert>
#include <string>

// Declare the function under test (or include the header where it is defined).
std::string orderlyQueue(const std::string& s, int k);

int main() {
    // k == 1: only rotations are reachable.
    assert(orderlyQueue("cba", 1) == "acb");   // rotations: cba, bac, acb -> "acb"
    assert(orderlyQueue("aaaa", 1) == "aaaa"); // all rotations are same
    assert(orderlyQueue("ab", 1) == "ab");     // rotations: ab, ba -> "ab"
    assert(orderlyQueue("b", 1) == "b");       // single character

    // k > 1: any permutation is reachable, so sorted string is answer.
    assert(orderlyQueue("cba", 2) == "abc");
    assert(orderlyQueue("baaca", 3) == "aaabc");
    assert(orderlyQueue("zxy", 5) == "xyz");   // k may equal length
    assert(orderlyQueue("edcba", 2) == "abcde");

    // Mixed edge case: already sorted string with k=1 and k>1.
    assert(orderlyQueue("abc", 1) == "abc");
    assert(orderlyQueue("abc", 2) == "abc");

    return 0;
}
