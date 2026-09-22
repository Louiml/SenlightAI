// Write a C++ function `findHiddenCountry` that takes a non-empty string containing a concatenated sequence of country names (all uppercase letters, no spaces) and a target country name (also uppercase), and returns the zero-based index of the first occurrence of the target country within the combined string. If the target is not found, return `-1`. The input will contain only uppercase English letters. For example, given `s = "BRITISHEGYPTGHANA"` and target `"EGYPT"`, the function must return `7`. The function must not use any string search library functions (like `find`, `search`, `strstr`) — you must implement the substring search manually.

The simplest correct approach is to implement a manual substring search: iterate over every possible starting index `i` in the main string such that `i + target.length() <= s.length()`. For each starting index, check if the substring of length `target.length()` matches the target character-by-character. If a match is found, return `i`. If the loop finishes without a match, return `-1`.  

Edge cases:  
- The target may be longer than the main string → immediately return `-1`.  
- The target may appear multiple times; return the first occurrence.  
- The target may be the same as the entire string → return `0`.  
- The main string may be shorter than the target but non-empty.  
- All inputs are uppercase letters, so case is not an issue.  

Time complexity: In the worst case (e.g., `s = "AAAA...A"`, `target = "AAA...B"`), we perform `O(n * m)` character comparisons, where `n = s.length()` and `m = target.length()`. For typical inputs, it is fast enough. Space complexity is `O(1)` extra space (only a few integer variables), not counting the input strings themselves.

#include <string>

// Returns the zero-based index of the first occurrence of target in s,
// or -1 if target is not found. Implements a manual substring search.
int findHiddenCountry(const std::string& s, const std::string& target) {
    const int n = static_cast<int>(s.size());
    const int m = static_cast<int>(target.size());

    // If target is longer than s, it cannot be found.
    if (m > n) {
        return -1;
    }

    // Iterate over every possible starting position.
    for (int i = 0; i <= n - m; ++i) {
        bool match = true;
        // Check all characters of target against s starting at i.
        for (int j = 0; j < m; ++j) {
            if (s[i + j] != target[j]) {
                match = false;
                break;
            }
        }
        if (match) {
            return i;
        }
    }

    return -1;
}

#include <cassert>

int main() {
    // Basic case from the snippet
    assert(findHiddenCountry("BRITISHEGYPTGHANA", "EGYPT") == 7);

    // Target at the very beginning
    assert(findHiddenCountry("EGYPTGHANA", "EGYPT") == 0);

    // Target at the very end
    assert(findHiddenCountry("GHANAEGYPT", "EGYPT") == 5);

    // Target not present
    assert(findHiddenCountry("BRITISHGHANA", "EGYPT") == -1);

    // Target longer than the main string
    assert(findHiddenCountry("EGY", "EGYPT") == -1);

    // Single-character main string and target
    assert(findHiddenCountry("A", "A") == 0);
    assert(findHiddenCountry("A", "B") == -1);

    // Repeated pattern – first occurrence wins
    assert(findHiddenCountry("ABABAB", "ABA") == 0);

    // Non-overlapping occurrence in the middle
    assert(findHiddenCountry("XXEGYPTXX", "EGYPT") == 2);

    // Empty target? Not allowed per problem statement, but test returns 0 if it were allowed
    // Here we assume target is non-empty, so this is not tested.

    // Very short main string, target equal to substring
    assert(findHiddenCountry("EGY", "EGY") == 0);

    return 0;
}
