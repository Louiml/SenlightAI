// Write a C++ function named `reactPolymer` that takes a string of letters (uppercase or lowercase) and returns the length of the fully reduced polymer. A reduction removes adjacent pairs of letters that are the same letter but different cases (e.g., `aA` or `Aa`). After removing a pair, the two neighbors may form a new pair, so the process continues until no such pairs remain. The input may contain non-letter characters which should be ignored. The function should return the size (number of remaining letters) after full reduction. For example, input `"dabAcCaCBAcCcaDA"` should yield length 10.

#include <cassert>
#include <string>

// The solution function is defined above (reactPolymer).
// This test only checks lengths after reduction.

int main() {
    // Given example
    assert(reactPolymer("dabAcCaCBAcCcaDA") == 10);
    // Empty input
    assert(reactPolymer("") == 0);
    // No reactions
    assert(reactPolymer("abc") == 3);
    // Simple pair
    assert(reactPolymer("aA") == 0);
    // Chain reaction
    assert(reactPolymer("abBA") == 0);
    // Non-letters ignored
    assert(reactPolymer("a1b2B3A") == 0); // after removing non-letters: a b B A -> abBA -> 0
    // Non-letters only
    assert(reactPolymer("123!") == 0);
    // Multiple pairs with leftover
    assert(reactPolymer("aA bB cC Xy") == 2); // reduces to Xy
    // Uppercase and lowercase same letter do not react if same case
    assert(reactPolymer("aabb") == 4);
    // Mixed case with no reaction at end
    assert(reactPolymer("aA b") == 1);
    return 0;
}

#include <string>
#include <cctype>

// Returns the length of the fully reduced polymer.
// Letters of the same character but opposite case react and are removed.
// Non-letter characters are ignored.
std::size_t reactPolymer(const std::string& input) {
    std::string reduced;
    reduced.reserve(input.size());

    for (const char c : input) {
        if (!std::isalpha(static_cast<unsigned char>(c)))
            continue;

        if (!reduced.empty()) {
            const char last = reduced.back();
            const bool sameLetter = (std::tolower(static_cast<unsigned char>(last)) ==
                                     std::tolower(static_cast<unsigned char>(c)));
            const bool oppositeCase = (std::isupper(static_cast<unsigned char>(last)) !=
                                       std::isupper(static_cast<unsigned char>(c)));
            if (sameLetter && oppositeCase) {
                reduced.pop_back();
                continue;
            }
        }
        reduced.push_back(c);
    }

    return reduced.size();
}

// The core algorithm is a single-pass reduction using a stack or a doubly linked list. The most straightforward and efficient approach is to iterate through the string, maintaining a result container (e.g., `std::string` or `std::vector<char>`) that acts like a stack. For each character that is a letter, compare it with the last character in the stack: if they are the same letter but opposite cases (i.e., one uppercase and one lowercase), pop the stack (remove the last character). Otherwise, push the current character. This simulates the effect of repeatedly removing adjacent reactive pairs, because when a pair is removed, the characters before and after become adjacent in the next iteration naturally. Non-letter characters are skipped entirely, as per the specification. Edge cases: empty input returns 0; input with no reactive pairs returns the count of letters; input like `"aA"` returns 0; input like `"abBA"` returns 0 because after removing `bB`, `aA` becomes adjacent. Time complexity is O(n) where n is the number of characters in the input, since each character is processed once and stack operations are O(1). Space complexity is O(m) where m is the number of letters in the input, used by the stack.
