Write a standalone C++ function `classifyLetter` that takes a single lowercase English letter as a `char` and returns a `std::string` equal to `"vowel"` if the letter is one of `a, e, i, o, u` (case-sensitive), and `"consonant"` otherwise (including if the character is not a letter at all, treat it as a consonant). The function must be `const`-correct (take the parameter by value, since it's a primitive type) and must not read from standard input or write to standard output; it should only return the classification string. The task is to implement this function precisely according to this behavior, with no extra logic such as checking for uppercase vowels or handling whitespace. The expected output for each call is exactly `"vowel"` or `"consonant"` (no extra spaces or punctuation). Provide a reference implementation that uses a simple loop over the five vowels, or an equivalent set-based check, and ensure it works for any `char` input from the caller.
The solution is straightforward: given a character `ch`, compare it against the five lowercase vowels `'a'`, `'e'`, `'i'`, `'o'`, `'u'`. If any match, return `"vowel"`; otherwise return `"consonant"`. The main algorithm is a linear scan over a constant-size set (5 characters), so it runs in O(1) time and uses O(1) auxiliary space. Edge cases: the input is a single `char`, so there is no empty string or length issue. If the character is uppercase (e.g., `'A'`) or a non-letter (e.g., `'1'` or `'!'`), it must be treated as a consonant because the specification only recognizes lowercase vowels. Since we compare directly to lowercase characters, all other inputs naturally fall into the consonant branch. The function returns a `std::string`; we can use `std::string` literals directly without any dynamic allocation concerns (though returning a string literal may involve a copy, but that is fine for this simple task). The implementation should be self-contained and include only necessary headers like `<string>`.
#include <string>

// Classify a single character as "vowel" if it is a lowercase vowel,
// otherwise return "consonant".
std::string classifyLetter(char ch) {
    // The set of lowercase vowels.
    const std::string vowels = "aeiou";
    
    // Check if the character is one of the vowels.
    for (char v : vowels) {
        if (ch == v) {
            return "vowel";
        }
    }
    
    // Any other character (including uppercase, digits, punctuation) is consonant.
    return "consonant";
}
#include <cassert>
#include <string>

// Assume the function is defined above.

int main() {
    // Basic lowercase vowels.
    assert(classifyLetter('a') == "vowel");
    assert(classifyLetter('e') == "vowel");
    assert(classifyLetter('i') == "vowel");
    assert(classifyLetter('o') == "vowel");
    assert(classifyLetter('u') == "vowel");
    
    // Some lowercase consonants.
    assert(classifyLetter('b') == "consonant");
    assert(classifyLetter('z') == "consonant");
    assert(classifyLetter('y') == "consonant");
    
    // Uppercase vowels are treated as consonants per specification.
    assert(classifyLetter('A') == "consonant");
    assert(classifyLetter('E') == "consonant");
    
    // Non-letter characters are also treated as consonants.
    assert(classifyLetter('1') == "consonant");
    assert(classifyLetter('!') == "consonant");
    assert(classifyLetter(' ') == "consonant");
    
    return 0;
}
