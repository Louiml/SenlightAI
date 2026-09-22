// Write a C++ function that takes a vector of non-empty strings (each string being a "pattern" made of lowercase letters a-z) and returns a single string representing the best "abstract pattern" learned from these input patterns. The learning process works by repeatedly finding the two most similar patterns (measured by the length of their longest common substring) and merging them into a new abstract pattern: the merged pattern is the longest common substring found, and the two source patterns are replaced by this merger. This process repeats until only one pattern remains, which is returned. If at any point no two patterns share any common letter (longest common substring length 0), the function returns an empty string. If the input contains only one pattern and no merging is possible, that pattern is returned unchanged. Duplicate patterns may occur; they are treated as distinct elements in the merging pool.

The core algorithm is a greedy bottom-up agglomerative clustering of strings. At each step, we examine all pairs of currently remaining patterns. For each pair, we compute the length of the longest common contiguous substring between the two strings (a classic dynamic programming problem, but since strings are short, a simple O(L1*L2) substring check per pair is fine). We select the pair with the highest such length (ties broken arbitrarily). If that maximum length is 0, no meaningful merge exists and we return an empty string. Otherwise, we replace the two chosen patterns with their longest common substring (which is a single string, deterministic given the pair). The total number of merge operations is exactly (n-1) where n is the original input size, so the loop runs that many times. At the end, if exactly one pattern remains, return it. Edge cases: n==0 should probably not happen per spec (non-empty vector), but if n==1 return that single string immediately (no merge possible). If multiple pairs tie, any choice is acceptable as long as it is consistent; the reference solution picks the first encountered pair with the maximum common-substring length. Time complexity: each iteration considers O(k^2) pairs where k is the current number of patterns (k decreases by 1 each iteration), and each pair comparison costs O(m^2) where m is the max length of any pattern in that pair. Overall worst-case O(n^3 * m^2) but with small inputs this is fine. Space complexity is O(n*m) for storing the patterns and O(m) per comparison.

#include <vector>
#include <string>
#include <algorithm>

// Helper: find the longest common substring between two strings.
// Returns the substring itself (empty if none).
static std::string longestCommonSubstring(const std::string& a, const std::string& b) {
    int best_len = 0;
    std::string best;
    // Brute-force all starting positions in a and b.
    for (size_t i = 0; i < a.size(); ++i) {
        for (size_t j = 0; j < b.size(); ++j) {
            size_t k = 0;
            while (i + k < a.size() && j + k < b.size() && a[i + k] == b[j + k]) {
                ++k;
            }
            if (k > best_len) {
                best_len = (int)k;
                best = a.substr(i, k);
            }
        }
    }
    return best;
}

// Learn an abstract pattern from the given list of patterns.
// Returns the final merged pattern, or empty string if merging fails.
std::string learnAbstractPattern(std::vector<std::string> patterns) {
    if (patterns.empty()) return std::string();
    if (patterns.size() == 1) return patterns[0];

    // Repeatedly merge the two most similar patterns.
    while (patterns.size() > 1) {
        int best_i = -1, best_j = -1;
        std::string best_common;
        int best_len = 0;

        // Find pair with longest common substring.
        for (size_t i = 0; i < patterns.size(); ++i) {
            for (size_t j = i + 1; j < patterns.size(); ++j) {
                std::string common = longestCommonSubstring(patterns[i], patterns[j]);
                if (common.size() > best_len) {
                    best_len = (int)common.size();
                    best_i = (int)i;
                    best_j = (int)j;
                    best_common = common;
                }
            }
        }

        // If no common letters, merging fails.
        if (best_len == 0) return std::string();

        // Replace the two chosen patterns with the merged result.
        // Remove the one with larger index first to avoid shifting.
        if (best_j < best_i) std::swap(best_i, best_j);
        patterns[best_i] = best_common;      // overwrite first
        patterns.erase(patterns.begin() + best_j); // remove second
    }

    return patterns[0];
}

#include <cassert>
#include <vector>
#include <string>

// Helper to call the solution function (already declared above).
int main() {
    // Single pattern: returned unchanged.
    assert(learnAbstractPattern({"abc"}) == "abc");

    // Two patterns with common substring "bc".
    assert(learnAbstractPattern({"abc", "bcd"}) == "bc");

    // Three patterns: "apple", "ample" -> "ap"+"le" wait careful.
    // Actually "apple" and "ample" share "ap" (len2), "ample" and "sample" share "ample" (len5) so merge those first -> "ample", then "apple" and "ample" share "apple" (len5) -> "apple"? Let's verify: after first merge we have {"apple", "ample"}; longest common is "apple" (5) so result "apple".
    assert(learnAbstractPattern({"apple", "ample", "sample"}) == "apple");

    // No common letters anywhere: should return empty.
    assert(learnAbstractPattern({"abc", "def", "ghi"}) == "");

    // Duplicate patterns merge to that same pattern.
    assert(learnAbstractPattern({"abc", "abc", "abc"}) == "abc");

    // Mixed: "cat", "car", "dog" -> "cat" and "car" share "ca" (len2), then "ca" and "dog" share nothing -> empty.
    assert(learnAbstractPattern({"cat", "car", "dog"}) == "");

    // More complex: "abcd", "bcde", "cdef" -> merge "abcd"+"bcde" -> "bcd", then "bcd"+"cdef" -> "cd", then "cd"+"cd"? Actually after first merge we have {"bcd","cdef"}; common "cd" -> result "cd".
    assert(learnAbstractPattern({"abcd", "bcde", "cdef"}) == "cd");

    // Empty input (though spec says non-empty, test defensive).
    assert(learnAbstractPattern({}) == "");

    return 0;
}
