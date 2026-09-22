// Given a string consisting only of lowercase English letters, write a C++ function `minRemovalsForNonAlternatingPattern` that processes the string by first compressing consecutive identical characters into runs (e.g., `"aaabbc"` → `('a',3),('b',2),('c',1)`). Then, among all runs that are not the first or last run (i.e., have at least one run on both sides), if the character of the run immediately before it differs from the character of the run immediately after it, the function should consider the number of characters in that middle run plus 2 (representing the cost of removing the two boundary runs' shared characters? Actually interpret as: the answer is the minimum value of (length of middle run + 2) over all such middle runs). If no such middle run exists, return 0. The function should take the input string as a parameter and return an integer representing this minimum value.
//
// **Note:** The original code reads the string from standard input, processes multiple test cases, and prints the result. Your function should be pure and not perform any I/O.
#include <cassert>

int main() {
    // Single run -> no middle run -> 0
    assert(minRemovalsForNonAlternatingPattern("aaaa") == 0);

    // Two runs -> no middle run -> 0
    assert(minRemovalsForNonAlternatingPattern("aabbb") == 0);

    // Three runs, middle run length 3, neighbors differ ('a' vs 'c') -> 3+2=5
    assert(minRemovalsForNonAlternatingPattern("aaabbbccc") == 5);

    // Four runs, middle candidate at index 1: neighbors 'a' and 'c' differ -> 2+2=4
    // also check index 2: neighbors 'b' and 'd' differ -> 1+2=3, min=3
    assert(minRemovalsForNonAlternatingPattern("aabccdd") == 3);

    // Middle run's neighbors same -> skipped, other candidate? 
    // "aaabbbaaa" runs: (a,3),(b,3),(a,3). Middle neighbors are both 'a' -> equal, so 0
    assert(minRemovalsForNonAlternatingPattern("aaabbbaaa") == 0);

    // Alternating pattern with equal neighbors all around -> 0
    assert(minRemovalsForNonAlternatingPattern("ababab") == 0);

    // "abbbbc" runs: (a,1),(b,4),(c,1). Middle neighbors 'a','c' differ -> 4+2=6
    assert(minRemovalsForNonAlternatingPattern("abbbbc") == 6);

    // Empty string should return 0 (no runs)
    assert(minRemovalsForNonAlternatingPattern("") == 0);

    // Multiple candidates: "abbc" runs (a,1),(b,2),(c,1) -> middle b length 2 -> 4
    assert(minRemovalsForNonAlternatingPattern("abbc") == 4);

    return 0;
}
#include <string>
#include <vector>
#include <climits>
#include <utility>

// Returns the minimum value of (run_length + 2) for a middle run whose
// neighboring runs have different characters. Returns 0 if no such run exists.
int minRemovalsForNonAlternatingPattern(const std::string& s) {
    std::vector<std::pair<char, int>> runs;
    for (char c : s) {
        if (runs.empty() || runs.back().first != c) {
            runs.emplace_back(c, 1);
        } else {
            runs.back().second++;
        }
    }

    int num_runs = static_cast<int>(runs.size());
    int result = INT_MAX;

    for (int i = 1; i < num_runs - 1; ++i) {
        if (runs[i - 1].first != runs[i + 1].first) {
            result = std::min(result, runs[i].second + 2);
        }
    }

    return (result == INT_MAX) ? 0 : result;
}
// The algorithm works in two phases. First, compress the string into a vector of pairs `(character, run_length)` by scanning left to right and merging consecutive identical characters. For example, `"aabbbaa"` compresses to `[('a',2),('b',3),('a',2)]`. Second, iterate over all positions `i` from 1 to `num_runs - 2` (i.e., skipping the first and last runs). For each such middle run, check if `vec[i-1].first != vec[i+1].first`. If they differ, compute `vec[i].second + 2` and take the minimum over all such candidates. If no candidate exists, return 0.
//
// Edge cases: 
// - If the string has only 1 or 2 distinct runs, there is no middle run, so return 0.
// - If all middle runs have identical neighboring characters (e.g., `"aabbbaa"` has neighbors 'a' and 'a' equal, so skipped), return 0.
// - The `+2` in the cost represents removing one character from each of the two boundary runs? Actually the original code's intent is that by removing the middle run's entire group plus one from each adjacent group, you can merge the outer groups, but the exact interpretation is not crucial; we just follow the formula.
//
// Time complexity: O(n) where n is the length of the string (we scan it once to compress, then scan the compressed vector). Space complexity: O(n) in worst case if no compression (e.g., all distinct characters), but typically O(number of runs).
