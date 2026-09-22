// Write a C++ function that takes a lowercase English sentence and returns the sentence translated according to Google Translate's "magic" substitution cipher, where each letter is replaced by the corresponding letter in the fixed alphabet `"yhesocvxduiglbkrztnwjpfmaq"` (i.e., 'a'→'y', 'b'→'h', 'c'→'e', ..., 'z'→'q'). Only lowercase letters `a`–`z` should be transformed; all other characters (spaces, punctuation, digits, uppercase letters) must remain unchanged. The function should accept a `const std::string&` and return a `std::string`.

#include <cassert>
#include <string>

// Insert the solution function here (content omitted for brevity in this test block, but it would be included above)

int main() {
    // Basic known examples from the original problem (if applicable)
    assert(magicTranslate("a") == "y");
    assert(magicTranslate("b") == "h");
    assert(magicTranslate("z") == "q");
    
    // Whole alphabet
    assert(magicTranslate("abcdefghijklmnopqrstuvwxyz") == "yhesocvxduiglbkrztnwjpfmaq");
    
    // Mixed with spaces and punctuation
    assert(magicTranslate("hello world") == "wggjf xjwjk");
    assert(magicTranslate("abc 123 ABC!") == "yhe 123 ABC!");
    
    // Empty string
    assert(magicTranslate("") == "");
    
    // Only non-lowercase characters
    assert(magicTranslate("HELLO 123 !!!") == "HELLO 123 !!!");
    
    // Digits and newline (if needed)
    assert(magicTranslate("a1b2c3") == "y1h2e3");
    
    // Single uppercase letter
    assert(magicTranslate("A") == "A");
    
    // Long string with repeated letters
    assert(magicTranslate("zzzzz") == "qqqqq");
    
    return 0;
}

#include <string>

// Translate a string using the fixed Google substitution cipher.
// Only lowercase 'a'–'z' are replaced; all other characters are kept.
std::string magicTranslate(const std::string& input) {
    // Fixed mapping: 'a' -> 'y', 'b' -> 'h', ..., 'z' -> 'q'
    static const char map[26] = {'y','h','e','s','o','c','v','x','d','u','i','g','l','b','k','r','z','t','n','w','j','p','f','m','a','q'};
    
    std::string result;
    result.reserve(input.size());
    
    for (char c : input) {
        if (c >= 'a' && c <= 'z') {
            result.push_back(map[c - 'a']);
        } else {
            result.push_back(c);
        }
    }
    return result;
}

// The solution is a straightforward character-by-character substitution. For each character in the input, check if it lies between `'a'` and `'z'`. If so, compute its index (`c - 'a'`) and lookup the replacement from the mapping string. If not, keep the character as is. The mapping string is a fixed constant array of 26 characters, so we can either use a C-style array or `std::string`. The time complexity is O(n), where n is the number of characters in the input, and the auxiliary space is O(1) besides the returned string (which is also O(n) for the output). Edge cases include empty strings, strings with no letters, strings with uppercase letters, digits, and non-ASCII characters—all should be passed through unchanged. No special handling is needed for multi-byte characters since we only touch ASCII lowercase letters.
