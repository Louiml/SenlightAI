Write a C++ function `bool containsSpecialWord(const vector<string>& words)` that returns `true` if any word in the input vector exactly equals one of the five predefined words: `"and"`, `"not"`, `"that"`, `"the"`, or `"you"`. The function must be case-sensitive (so `"And"` or `"THE"` do not count) and must handle an empty vector gracefully by returning `false`. The function should not modify the input vector, and it should work for any number of words.
// The solution is straightforward: create a `std::set<std::string>` containing the five target words, then iterate over every string in the input vector and check membership using the set's `count` or `find` method. Since the set provides O(log k) lookup where k=5, this is effectively constant time per word. Edge cases include: an empty vector (loop never executes, return `false`), exact case matching (set lookup is case-sensitive, which matches the requirement), and duplicates in the input (first match triggers early return, so duplicates do not affect correctness). Time complexity is O(n) where n is the number of words, because each lookup is O(1) practically (or O(log 5) theoretically). Space complexity is O(1) because the set has a fixed size of five elements, and we do not copy the input.
#include <string>
#include <vector>
#include <set>
#include <algorithm> // for std::any_of (optional, but used for clarity)

// Returns true if any word in the input vector exactly matches one of:
// "and", "not", "that", "the", "you". Case-sensitive.
bool containsSpecialWord(const std::vector<std::string>& words) {
    static const std::set<std::string> predefined = {"and", "not", "that", "the", "you"};
    for (const std::string& w : words) {
        if (predefined.count(w) > 0) {
            return true;
        }
    }
    return false;
}
#include <cassert>
#include <vector>
#include <string>

// (The solution function is assumed to be included above.)
int main() {
    assert(containsSpecialWord({"hello", "world"}) == false);
    assert(containsSpecialWord({"this", "is", "the", "test"}) == true);
    assert(containsSpecialWord({"and"}) == true);
    assert(containsSpecialWord({"And", "THE"}) == false); // case-sensitive
    assert(containsSpecialWord({}) == false); // empty vector
    assert(containsSpecialWord({"not", "that"}) == true);
    assert(containsSpecialWord({"you", "are", "kind"}) == true);
    assert(containsSpecialWord({"cat", "dog", "and", "bird"}) == true);
    assert(containsSpecialWord({"apple", "banana", "cherry"}) == false);
    assert(containsSpecialWord({"that", "that", "that"}) == true); // duplicates
    return 0;
}
