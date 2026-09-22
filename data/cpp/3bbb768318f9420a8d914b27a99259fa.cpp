Write a C++ function `bool canFormPairs(int count, const std::vector<std::string>& words)` that takes a positive integer `count` and a vector of `count` strings (each containing only lowercase English letters). The function must return `true` if, by processing the strings in the given order, it is possible to pair each string with an earlier string whose reverse is identical to the current string (ignoring palindromes, which are never paired), and such that each string is used at most once. More precisely, when iterating from the first to the last word, for each non-palindromic word `w`, check if its reverse `rev(w)` has appeared previously and has not already been paired; if so, mark that previous occurrence as paired and continue. If at any point a non-palindromic word's reverse has already been paired, or a non-palindromic word's reverse appears later but a duplicate of the current word is needed, the function should return `false`. The function returns `false` if any non-palindromic word cannot find an available reverse earlier or if any word is forced to pair with itself (which is impossible since palindromes are excluded). Essentially, the behavior must exactly match the original snippet: it outputs `"YES"` if a valid pairing is found in the first come, first served manner, otherwise `"NO"`.
// The core idea is to simulate the original algorithm exactly: maintain a `std::set<std::string>` that stores the original forms of previously seen non-palindromic words that have not yet been used as a match. For each input word in order:  
// - If the word is a palindrome (i.e., equal to its reverse), ignore it entirely because the original code never inserts or checks palindromes.  
// - If not a palindrome, let `rev` be its reverse. Check if `rev` is already present in the set.  
//   - If yes, a pairing is found: remove `rev` from the set (since it is now used) and mark that a pair exists. This matches the original code's `found=true; break;`, but since we must process the entire vector to detect if any later word fails, we can simply return `true` immediately because the original code stops at the first successful pair.  
//   - If no, insert the current word itself into the set for potential future matching.  
// - If we finish the loop without ever finding a pair, return `false`.  
//
// Edge cases:  
// - Single word: no pairing possible → `false`.  
// - All palindromes: no pair → `false`.  
// - A word appears multiple times: each occurrence is independent; the set stores one copy per occurrence, and removal deletes one.  
// - The original code stops at the first successful pair, so our function should return `true` as soon as a pair is found, regardless of the rest of the vector.  
//
// Time complexity: O(n * L) where n is the number of strings and L is the maximum string length (for reversal and set operations). Space complexity: O(n * L) in the worst case for the set storing up to n−1 strings.
#include <string>
#include <vector>
#include <set>
#include <algorithm>

// Return true if a valid pairing can be formed according to the rule
// that for each non-palindrome, its reverse must have appeared earlier
// and not yet been used. Stop at the first successful pair.
bool canFormPairs(int count, const std::vector<std::string>& words) {
    std::set<std::string> available;  // Original forms of unmatched non-palindromes

    for (int i = 0; i < count; ++i) {
        const std::string& current = words[i];
        std::string reversed = current;
        std::reverse(reversed.begin(), reversed.end());

        // Skip palindromes; they are never used for pairing.
        if (current == reversed) {
            continue;
        }

        // Check if the reverse has been seen and is still unmatched.
        if (available.find(reversed) != available.end()) {
            return true;  // Found a valid pair immediately.
        } else {
            available.insert(current);  // Store this word for future matches.
        }
    }

    return false;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be declared above.
int main() {
    // Test 1: Exact match from the snippet with 4 words, pair "abc" and "cba".
    std::vector<std::string> t1 = {"abc", "def", "cba", "xyz"};
    assert(canFormPairs(4, t1) == true);

    // Test 2: No pair exists.
    std::vector<std::string> t2 = {"abc", "def", "ghi"};
    assert(canFormPairs(3, t2) == false);

    // Test 3: Single word, no pair.
    std::vector<std::string> t3 = {"hello"};
    assert(canFormPairs(1, t3) == false);

    // Test 4: Only palindromes, no pair.
    std::vector<std::string> t4 = {"aba", "cc", "xyzzyx"};
    assert(canFormPairs(3, t4) == false);

    // Test 5: Pair appears later, still returns true.
    std::vector<std::string> t5 = {"a", "b", "b", "a"};
    assert(canFormPairs(4, t5) == true);

    // Test 6: Duplicate unmatched words, still no pair if reverses not present.
    std::vector<std::string> t6 = {"ab", "ab", "cd"};
    assert(canFormPairs(3, t6) == false);

    // Test 7: Reversed words not adjacent but order matters.
    std::vector<std::string> t7 = {"dog", "cat", "god"};
    assert(canFormPairs(3, t7) == true);

    // Test 8: Palindromes interspersed, but a real pair exists after.
    std::vector<std::string> t8 = {"aa", "bb", "ab", "ba"};
    assert(canFormPairs(4, t8) == true);

    // Test 9: Empty vector (count = 0) - though spec says positive, edge.
    std::vector<std::string> t9;
    assert(canFormPairs(0, t9) == false);

    // Test 10: Multiple same non-palindrome, first pair found at positions 2 and 3.
    std::vector<std::string> t10 = {"xy", "yx", "xy"};
    assert(canFormPairs(3, t10) == true);
}
