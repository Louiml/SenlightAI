Write a C++ function that converts a given English word to its plural form according to the following rules, in order of priority:  
1. If the word is in a provided map of irregular plurals, return the mapped plural.  
2. If the word ends with a consonant followed by `y`, change the ending to `ies` (e.g., city → cities).  
3. If the word ends with `o`, `s`, `ch`, `sh`, or `x`, append `es` (e.g., box → boxes).  
4. Otherwise, just append `s` (e.g., cat → cats).  
The function receives: a word (lowercase English letters), a `std::map<std::string, std::string>` of irregular singular→plural pairs, and returns the plural as a `std::string`. You may assume inputs are non‑empty and that vowels are `a, e, i, o, u` only; `y` is considered a consonant for this rule. The supplied map may contain entries that are not used; you must not modify the map.

#include <cassert>
#include <map>
#include <string>

// The solution function is assumed to be declared above (or in the same file).
int main() {
    std::map<std::string, std::string> irr = {
        {"child", "children"},
        {"goose", "geese"},
        {"man", "men"},
        {"mouse", "mice"},
        {"woman", "women"}
    };

    // Irregular
    assert(pluralize("child", irr) == "children");
    assert(pluralize("goose", irr) == "geese");
    assert(pluralize("man", irr) == "men");

    // Consonant + y
    assert(pluralize("city", irr) == "cities");
    assert(pluralize("baby", irr) == "babies");
    assert(pluralize("day", irr) == "days");   // 'y' preceded by vowel
    assert(pluralize("boy", irr) == "boys");   // 'y' preceded by vowel

    // Special endings
    assert(pluralize("box", irr) == "boxes");
    assert(pluralize("bus", irr) == "buses");
    assert(pluralize("church", irr) == "churches");
    assert(pluralize("brush", irr) == "brushes");
    assert(pluralize("hero", irr) == "heroes");

    // Default
    assert(pluralize("cat", irr) == "cats");
    assert(pluralize("dog", irr) == "dogs");

    // Map entries that are not used remain unchanged (no effect)
    assert(pluralize("mouse", irr) == "mice");

    return 0;
}

#include <map>
#include <string>

// Check if 'str' ends with the exact suffix 'ending'.
bool ends_with(const std::string& str, const std::string& ending) {
    if (str.size() < ending.size())
        return false;
    return str.compare(str.size() - ending.size(), ending.size(), ending) == 0;
}

// Check if a character is a lowercase vowel.
bool is_vowel(char ch) {
    return ch == 'a' || ch == 'e' || ch == 'i' || ch == 'o' || ch == 'u';
}

// Convert a word to its plural according to the specified rules.
std::string pluralize(const std::string& word,
                      const std::map<std::string, std::string>& irregular) {
    // Rule 1: irregular plural
    auto it = irregular.find(word);
    if (it != irregular.end())
        return it->second;

    // Rule 2: consonant + y
    if (word.size() >= 2 && word.back() == 'y' && !is_vowel(word[word.size() - 2]))
        return word.substr(0, word.size() - 1) + "ies";

    // Rule 3: special endings
    if (ends_with(word, "o") || ends_with(word, "s") ||
        ends_with(word, "ch") || ends_with(word, "sh") || ends_with(word, "x"))
        return word + "es";

    // Rule 4: default
    return word + "s";
}

// The algorithm is a step‑wise decision process. First, check if the word exists in the irregular map using a constant‑time `find()` lookup; if found, return the mapped value. Otherwise, inspect the word’s suffixes: to apply the `y`→`ies` rule, the word must have length at least 2, end with `y`, and the character immediately before `y` must not be a vowel (so “boy” becomes “boys” but “city” becomes “cities”). Then check if the word ends with any of the five special suffixes (`o`, `s`, `ch`, `sh`, `x`) using a helper `ends_with` that compares the tail of the string; note that `ch` and `sh` are two‑character endings, so the helper must handle those correctly. If none of the special cases apply, append `s`. Edge cases: the irregular map may contain an empty string? The problem guarantees provided plurals are non‑empty, but a safe implementation can treat an empty mapped value as “not found” if needed; here we rely on exact presence in the map. The helper `is_vowel` checks a single character. Time complexity: O(L) per word, where L is the word length, because all suffix checks require scanning at most 2 characters (for `ch`/`sh`) and the map lookup is O(log M) for M irregular entries. Space complexity: O(1) auxiliary, not counting the input map or the returned string.
