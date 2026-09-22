// Write a C++ function that takes a vector of strings and returns a new vector containing only those strings that appear as a substring of at least one other distinct string in the input vector. For example, given `{"mass", "as", "hero", "superhero"}`, the result should be `{"as", "hero"}` because "as" appears inside "mass" and "hero" appears inside "superhero". The order of elements in the output should match the order in which they first appear in the input vector. Strings that are identical to another string but are not substrings of any *different* string should not be included. The input vector may contain duplicates, and each string may be up to 100 characters long; the vector size is at most 1000. Assume all strings are non-empty and contain only lowercase English letters.

The solution iterates over each string in the input vector and checks whether it is a substring of any *other* string in the vector (i.e., not itself). For each candidate string, we loop through all strings in the vector; if we find a different string that contains the candidate as a substring (using `std::string::find`), we add the candidate to the result and break out of the inner loop to avoid duplicates. If no such containing string exists, the candidate is skipped. Edge cases include: duplicate strings—an identical copy does not count as a distinct container, so we must explicitly check `w != word` (or compare indices) to avoid self-matching; a string that is a substring of itself but no other string must be omitted; and empty input vector returns an empty vector. The time complexity is \(O(n^2 \cdot m)\), where \(n\) is the number of strings and \(m\) is the average length of strings, because for each of the \(n\) candidates we scan up to \(n\) strings and perform a substring search that can take \(O(m)\) time in the worst case. The space complexity is \(O(k)\) for the result vector, where \(k\) is the number of matching strings, but no extra auxiliary data structures are used beyond the output.

#include <string>
#include <vector>

// Return all strings in 'words' that appear as a substring of a different string.
std::vector<std::string> findSubstringMatches(const std::vector<std::string>& words) {
    std::vector<std::string> result;
    for (const auto& candidate : words) {
        bool found = false;
        for (const auto& container : words) {
            if (&container != &candidate && container.find(candidate) != std::string::npos) {
                result.push_back(candidate);
                found = true;
                break;
            }
        }
        // 'found' variable is not strictly needed, but included for clarity.
        (void)found;
    }
    return result;
}

#include <cassert>
#include <string>
#include <vector>

int main() {
    // Basic case from description
    std::vector<std::string> words1 = {"mass", "as", "hero", "superhero"};
    assert(findSubstringMatches(words1) == std::vector<std::string>({"as", "hero"}));

    // No matches
    std::vector<std::string> words2 = {"cat", "dog", "bird"};
    assert(findSubstringMatches(words2).empty());

    // Self-reference must not count; "abc" appears in itself, but not in another string
    std::vector<std::string> words3 = {"abc", "def", "abc"};
    assert(findSubstringMatches(words3).empty());

    // Duplicate strings: "aa" appears in a different "aa" (but that's identical, not a different string)
    std::vector<std::string> words4 = {"aa", "aa"};
    assert(findSubstringMatches(words4).empty());

    // One string is a substring of another, but there are duplicates
    std::vector<std::string> words5 = {"ab", "ab", "abc"};
    assert(findSubstringMatches(words5) == std::vector<std::string>({"ab", "ab"}));

    // Empty input
    std::vector<std::string> words6;
    assert(findSubstringMatches(words6).empty());

    // Multiple matches and ordering preserved
    std::vector<std::string> words7 = {"a", "b", "ab", "ba"};
    assert(findSubstringMatches(words7) == std::vector<std::string>({"a", "b"}));

    return 0;
}
