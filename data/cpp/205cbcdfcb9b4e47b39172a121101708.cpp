// Write a C++ function that takes a string `s` and a character `c`, and returns a `std::vector<int>` where each element at index `i` is the shortest distance from `s[i]` to any occurrence of `c` in the string. The distance is measured as the absolute difference between indices. The input string is non-empty, may contain any printable ASCII characters, and is guaranteed to contain at least one occurrence of `c`. The function should handle cases where `c` appears at the very beginning or end of the string, and must work efficiently for large inputs (up to 10^5 characters). Do not use any standard library functions like `find` or `distance` for the main logic; implement the algorithm manually with loops.
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic case with multiple occurrences.
    std::vector<int> result1 = shortestDistanceToChar("loveleetcode", 'e');
    assert(result1 == std::vector<int>({3, 2, 1, 0, 1, 0, 0, 1, 2, 2, 1, 0}));

    // Character at the very first position.
    std::vector<int> result2 = shortestDistanceToChar("abc", 'a');
    assert(result2 == std::vector<int>({0, 1, 2}));

    // Character at the very last position.
    std::vector<int> result3 = shortestDistanceToChar("abc", 'c');
    assert(result3 == std::vector<int>({2, 1, 0}));

    // Single character string with that character.
    std::vector<int> result4 = shortestDistanceToChar("z", 'z');
    assert(result4 == std::vector<int>({0}));

    // All same characters.
    std::vector<int> result5 = shortestDistanceToChar("aaaa", 'a');
    assert(result5 == std::vector<int>({0, 0, 0, 0}));

    // Characters repeated with gaps.
    std::vector<int> result6 = shortestDistanceToChar("a  b  a", 'a');
    assert(result6 == std::vector<int>({0, 1, 2, 2, 1, 0, 1, 2}));

    // Verify with a longer random pattern manually calculated.
    std::vector<int> result7 = shortestDistanceToChar("xabcx", 'x');
    assert(result7 == std::vector<int>({0, 1, 2, 1, 0}));

    // Edge case where c appears only once in the middle.
    std::vector<int> result8 = shortestDistanceToChar("abcdef", 'd');
    assert(result8 == std::vector<int>({3, 2, 1, 0, 1, 2}));

    return 0;
}
#include <vector>
#include <string>
#include <algorithm>

// Returns a vector where result[i] is the shortest distance from s[i] to any occurrence of c.
std::vector<int> shortestDistanceToChar(const std::string& s, char c) {
    const std::size_t n = s.size();
    std::vector<int> distances(n);

    // First pass: left to right, compute distance to nearest c on the left (or current).
    int prev = -static_cast<int>(n); // Initialize to -n so that i - prev is large when no c seen yet.
    for (std::size_t i = 0; i < n; ++i) {
        if (s[i] == c) {
            prev = static_cast<int>(i);
        }
        distances[i] = static_cast<int>(i) - prev;
    }

    // Second pass: right to left, correct distances by considering nearest c on the right.
    // Start from the last known prev (from first pass) and move leftward.
    for (int i = prev - 1; i >= 0; --i) {
        if (s[i] == c) {
            prev = i;
        }
        distances[i] = std::min(distances[i], prev - i);
    }

    return distances;
}
// The solution uses a two-pass dynamic programming approach. 
// - **First pass (left to right)**: Initialize `prev` to a very negative value (e.g., `-n`) so that the first distance is large if `c` hasn't appeared yet. Iterate through the string, updating `prev` whenever `s[i] == c`. For each index, store `i - prev` as the distance to the nearest occurrence of `c` that is to the left (or at the current position).
// - **Second pass (right to left)**: Starting from the last position, iterate backward. Whenever we encounter `c`, update `prev` to that index. For each index, compute `prev - i` (distance to the nearest occurrence to the right) and take the minimum with the already stored value from the first pass.
// - **Edge cases**: 
//   - If `c` appears only once at position `0`, the first pass sets distances to `i` for all indices; the second pass starts from `prev` (which is `0`) and goes to index `-1` (loop condition `i >= 0`, so it doesn't execute), leaving correct distances.
//   - If `c` appears only once at the last position, the first pass computes distances as `i - (-n)` which are large (like `n + i`), then the second pass corrects all of them to `n-1-i`.
//   - When `c` appears consecutively, the distances will be small and updated correctly.
// - **Time complexity**: \(O(n)\) because each pass iterates over the string exactly once.
// - **Space complexity**: \(O(n)\) for the output vector, plus \(O(1)\) extra space (just a few integer variables).
