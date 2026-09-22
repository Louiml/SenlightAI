// Write a C++ function that takes a string `s` of exactly 3 lowercase letters (you may assume the input is always 3 characters, all lowercase English letters) and returns a string `"Yes"` if the string contains exactly one pair of identical adjacent letters (i.e., either positions 0-1 are equal while position 2 is different, or positions 1-2 are equal while position 0 is different, or positions 0-2 are equal while position 1 is different), and returns `"No"` otherwise. The function must handle all possible 3-letter strings, including cases where all three are the same (which should be "No") and cases where all three are different (also "No"). The function should not read from standard input or write to standard output; it should only work with the passed string.
// The problem is a straightforward string pattern check on exactly 3 characters. The condition for "Yes" is that exactly two characters are the same, but they must be adjacent? Actually reading the original snippet, the condition is: either `s[0]==s[1]` and `s[1]!=s[2]` (so first two equal, third different), or `s[1]==s[2]` and `s[0]!=s[1]` (last two equal, first different), or `s[0]==s[2]` and `s[0]!=s[1]` (first and third equal, middle different). In all these cases, there is exactly one pair of equal letters, and the third is distinct. So the function can simply check whether exactly two characters are the same and the third is different. That is equivalent to counting each character's occurrences: if the multiset has counts (2,1) — i.e., exactly one character appears twice and one appears once — then return "Yes", else "No". This covers all three patterns and excludes cases like "aaa" (count 3), "abc" (counts all 1), and "aba" (counts 2 for 'a', 1 for 'b') which is indeed "Yes". The algorithm: count occurrences of each of the 26 letters, then check if any count equals 2 and the remaining letters sum to 1 (since exactly 3 total). Time complexity O(1) since string length is 3, space O(1) for the count array. Edge cases: all same, all different, and the three possible arrangements of a pair plus a singleton.
#include <string>
#include <array>

// Determine if a 3-letter string contains exactly one pair of identical letters
// and the third letter is different from that pair.
std::string hasExactlyOnePair(const std::string& s) {
    std::array<int, 26> counts{};
    for (char c : s) {
        counts[c - 'a']++;
    }

    int pairCount = 0;
    int singletonCount = 0;
    for (int count : counts) {
        if (count == 2) pairCount++;
        else if (count == 1) singletonCount++;
        // counts >2 would make pairCount>1 and we don't need to track that separately
    }

    // Exactly one pair and exactly one singleton means two distinct letters
    // with counts (2,1). That yields "Yes".
    if (pairCount == 1 && singletonCount == 1) {
        return "Yes";
    }
    return "No";
}
#include <cassert>
#include <string>

// Declare the function from the solution (normally would be in a header)
std::string hasExactlyOnePair(const std::string& s);

int main() {
    // All three same -> No
    assert(hasExactlyOnePair("aaa") == "No");
    // All three different -> No
    assert(hasExactlyOnePair("abc") == "No");
    // First two equal, third different -> Yes
    assert(hasExactlyOnePair("aab") == "Yes");
    // Last two equal, first different -> Yes
    assert(hasExactlyOnePair("abb") == "Yes");
    // First and third equal, middle different -> Yes
    assert(hasExactlyOnePair("aba") == "Yes");
    // Different pair pattern using other letters
    assert(hasExactlyOnePair("zzx") == "Yes");
    assert(hasExactlyOnePair("xyy") == "Yes");
    // No pair, only one letter repeated twice? No, that's already covered. 
    // Test a case where two letters repeat? Impossible with length 3.
    // Test a case with letters that are not adjacent alphabetically
    assert(hasExactlyOnePair("qpq") == "Yes");
    // Edge: string with non-'a' letters, all same
    assert(hasExactlyOnePair("zzz") == "No");
}
