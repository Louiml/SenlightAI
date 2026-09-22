Write a C++ function that takes a single string containing words separated by single underscores (e.g., "hello_world_test") and returns a new string where each word is reversed individually, but the order of words and the underscores between them remain unchanged. The input string will contain only lowercase English letters and underscores, and will always start and end with a word (no leading or trailing underscores), with at least one character total. For example, given "abc_def_gh", the function should return "cba_fed_hg". If the input string has no underscores (a single word), it should simply return the reversed word. The function must be `const` correct (accept a `const std::string&` and return `std::string`).
// The solution requires scanning the input string and detecting segments separated by underscores. A straightforward approach is to iterate through the string character by character, accumulating characters of the current word into a temporary string. When an underscore is encountered, reverse that temporary word, append it to the output, then append the underscore, and clear the temporary to start the next word. After the loop ends, there will be one remaining word (since the input ends with a word), so reverse that and append it. This processes each character exactly once, giving O(n) time complexity where n is the length of the input string. The auxiliary space is O(k) where k is the length of the longest word (for the temporary string), but in the worst case (a single word) this is O(n); however, since the output itself is O(n), this is acceptable. Edge cases include a single word (no underscores) and consecutive underscores, but the problem guarantees no leading/trailing underscores and only single underscores between words, so we don't need to handle multiple underscores. Still, our algorithm naturally handles those cases too because it treats any underscore as a separator.
#include <string>
#include <algorithm>

// Reverse each word in a string where words are separated by single underscores.
// Preserves the order of words and underscores. Expects input with no leading/trailing underscores.
std::string reverseWordsInUnderscoreString(const std::string& input) {
    std::string result;
    std::string currentWord;
    
    for (char ch : input) {
        if (ch == '_') {
            // Reverse the completed word and add it plus the underscore.
            std::reverse(currentWord.begin(), currentWord.end());
            result += currentWord;
            result += '_';
            currentWord.clear();
        } else {
            currentWord += ch;
        }
    }
    
    // Handle the last word (input never ends with underscore per constraints).
    std::reverse(currentWord.begin(), currentWord.end());
    result += currentWord;
    
    return result;
}
#include <cassert>
#include <string>

// The solution function is declared above or included here.
// For testing, we include the function definition above.

int main() {
    // Test basic case with multiple words.
    assert(reverseWordsInUnderscoreString("abc_def_gh") == "cba_fed_hg");
    
    // Test single word (no underscore).
    assert(reverseWordsInUnderscoreString("hello") == "olleh");
    
    // Test two words.
    assert(reverseWordsInUnderscoreString("ab_c") == "ba_c");
    
    // Test single-letter words.
    assert(reverseWordsInUnderscoreString("a_b_c") == "a_b_c");
    
    // Test longer words and underscores.
    assert(reverseWordsInUnderscoreString("abc_xyzw") == "cba_wzyx");
    
    // Test all same letters.
    assert(reverseWordsInUnderscoreString("aa_bb_cc") == "aa_bb_cc");
    
    // Test single character.
    assert(reverseWordsInUnderscoreString("z") == "z");
    
    // Test various lengths.
    assert(reverseWordsInUnderscoreString("one_two_three") == "eno_owt_eerht");
    
    // Test with underscore between same-length words.
    assert(reverseWordsInUnderscoreString("abcd_efgh") == "dcba_hgfe");
    
    // Test with a long repeated pattern.
    assert(reverseWordsInUnderscoreString("test_input_here") == "tset_tupni_ereh");
    
    return 0;
}
