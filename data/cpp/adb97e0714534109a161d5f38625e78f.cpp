Write a standalone C++ function that, given a vector of strings and two string parameters `target` and `radius`, counts and returns the number of strings in the vector whose Levenshtein edit distance from `target` is less than or equal to `radius`. The Levenshtein distance is defined as the minimum number of single-character insertions, deletions, or substitutions required to transform one string into another. The function must handle empty strings, case-sensitive comparison, and arbitrary string lengths. The function signature should be `int countWithinRadius(const std::vector<std::string>& dictionary, const std::string& target, int radius)`. Edge cases include a radius of zero (only exact matches) and negative radius (which should return zero since no distance can be negative).
#include <cassert>
#include <vector>
#include <string>

// Declare the function from the solution (for testing, include its definition above).
int countWithinRadius(const std::vector<std::string>& dictionary, const std::string& target, int radius);

int main() {
    // Basic cases
    assert(countWithinRadius({"cat", "bat", "cut", "cart"}, "cat", 1) == 3); // cat, bat, cut
    assert(countWithinRadius({"cat", "bat", "cut", "cart"}, "cat", 0) == 1); // exact match only
    assert(countWithinRadius({"cat", "bat", "cut", "cart"}, "cat", 2) == 4); // all match within distance 2
    
    // Empty target
    assert(countWithinRadius({"", "a", "ab"}, "", 1) == 2); // "" and "a"
    assert(countWithinRadius({"", "a", "ab"}, "", 0) == 1); // only ""
    
    // Empty dictionary
    assert(countWithinRadius({}, "anything", 3) == 0);
    
    // Negative radius
    assert(countWithinRadius({"a", "b"}, "a", -1) == 0);
    
    // Case sensitivity
    assert(countWithinRadius({"Cat"}, "cat", 0) == 0); // case-sensitive
    assert(countWithinRadius({"Cat"}, "cat", 1) == 1); // one substitution
    
    // Larger radius covers everything
    assert(countWithinRadius({"hello", "hallo", "help"}, "hello", 2) == 3);
    assert(countWithinRadius({"hello", "hallo", "help"}, "hello", 1) == 2); // hello, hallo (help distance 2)
    
    // Single character strings
    assert(countWithinRadius({"a"}, "a", 0) == 1);
    assert(countWithinRadius({"a"}, "b", 1) == 1);
    
    // Long strings with reasonable radius
    assert(countWithinRadius({"kitten", "sitting", "mittens"}, "kitten", 3) == 3);
    assert(countWithinRadius({"kitten", "sitting", "mittens"}, "kitten", 2) == 1); // only kitten itself
    
    return 0;
}
#include <vector>
#include <string>
#include <algorithm>

// Count dictionary strings whose Levenshtein distance from target is <= radius.
int countWithinRadius(const std::vector<std::string>& dictionary, const std::string& target, int radius) {
    if (radius < 0) return 0;
    
    int count = 0;
    const int m = static_cast<int>(target.size());
    
    for (const auto& word : dictionary) {
        const int n = static_cast<int>(word.size());
        
        // Use two rows for DP; prev represents row i-1, curr represents row i.
        std::vector<int> prev(n + 1), curr(n + 1);
        
        // Initialize first row (distance from empty target to word prefix)
        for (int j = 0; j <= n; ++j) {
            prev[j] = j;
        }
        
        for (int i = 1; i <= m; ++i) {
            curr[0] = i;  // distance from target prefix to empty word
            for (int j = 1; j <= n; ++j) {
                const int cost = (target[i - 1] == word[j - 1]) ? 0 : 1;
                curr[j] = std::min({
                    prev[j] + 1,          // deletion
                    curr[j - 1] + 1,      // insertion
                    prev[j - 1] + cost    // substitution/match
                });
            }
            std::swap(prev, curr);
        }
        
        if (prev[n] <= radius) {
            ++count;
        }
    }
    
    return count;
}
// The solution requires computing the Levenshtein distance between `target` and each string in the dictionary. The classic dynamic programming approach uses a 2D table of size `(m+1) x (n+1)` where `m` and `n` are the lengths of the two strings. Initialize the first row and column with incremental costs, then fill the table by comparing characters. The distance is the value at the bottom-right cell. To optimize memory, we can use two rolling vectors of size `n+1` because only the previous row is needed. For each dictionary word, compute the distance and check if it is `<= radius`. Important edge cases: if `radius` is negative, return 0 immediately (since distance is always non-negative). If both strings are empty, distance is 0. If one is empty, distance is the length of the other. Time complexity is `O(K * L * M)` where `K` is the number of dictionary words, `L` is the length of `target`, and `M` is the maximum length of a dictionary word. Space complexity is `O(min(L, M))` for the rolling arrays, or `O(L*M)` if using full table, but rolling reduces to `O(L)`.
