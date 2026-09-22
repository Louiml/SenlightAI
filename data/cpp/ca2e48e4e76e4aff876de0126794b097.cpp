/*
Write a standalone C++ function that takes a non-empty string of lowercase English letters, sorts its characters alphabetically, and returns a newline-separated string containing all distinct permutations of the sorted characters in lexicographical order (i.e., the order produced by repeatedly calling `std::next_permutation` on the string). The function must handle strings with duplicate characters correctly, producing only unique permutations. The input string may contain any number of characters from 1 to 10. The returned string must end with a newline after each permutation, including the last one. The output should be exactly as if you sorted the original characters and then printed each permutation on its own line.
*/
#include <string>
#include <algorithm>
#include <cstddef>

// Generate all lexicographical permutations of the sorted characters.
// Returns a string with each permutation on a separate line.
std::string allPermutations(const std::string& input) {
    std::string sorted(input);
    std::sort(sorted.begin(), sorted.end());

    std::string result;
    do {
        result += sorted;
        result += '\n';
    } while (std::next_permutation(sorted.begin(), sorted.end()));

    return result;
}
#include <cassert>
#include <string>

// Declare the solution function.
std::string allPermutations(const std::string& input);

int main() {
    // Single character: only one permutation.
    assert(allPermutations("a") == "a\n");

    // All identical characters: one unique permutation.
    assert(allPermutations("bbb") == "bbb\n");

    // Two distinct characters.
    assert(allPermutations("ba") == "ab\nba\n");

    // Three distinct characters: six permutations.
    assert(allPermutations("cab") == "abc\nacb\nbac\nbca\ncab\ncba\n");

    // Duplicate characters reduce number of permutations.
    assert(allPermutations("aab") == "aab\naba\nbaa\n");

    // Length 4 with duplicates.
    assert(allPermutations("abab") == "aabb\nabab\nabba\nbaab\nbaba\nbbaa\n");

    // Already sorted input gives same result as unsorted.
    assert(allPermutations("abc") == allPermutations("cba"));

    // Longer distinct characters (length 4).
    assert(allPermutations("dcba") == "abcd\nabdc\nacbd\nacdb\nadbc\nadcb\n"
                                      "bacd\nbadc\nbcad\nbcda\nbdac\nbdca\n"
                                      "cabd\ncadb\ncbad\ncbda\ncdab\ncdba\n"
                                      "dabc\ndacb\ndbac\ndbca\ndcab\ndcba\n");

    // Mixed case with all same characters after sorting.
    assert(allPermutations("zzz") == "zzz\n");

    return 0;
}
// The solution first sorts the input string’s characters in ascending order using `std::sort`. This ensures that the initial permutation is the smallest lexicographically. Then, by repeatedly calling `std::next_permutation` on that sorted string, we generate all permutations in lexicographical order. The standard library’s `next_permutation` automatically skips duplicate permutations when the input contains repeated characters, so no extra deduplication logic is needed. We append each permutation to the result string, adding a newline after each. The loop runs as long as `next_permutation` returns `true`, meaning a next permutation exists. Edge cases: (1) a single-character string yields exactly one permutation, (2) strings with all identical characters yield only one permutation, (3) empty input is not allowed per the task. Time complexity is \(O(n! \cdot n)\) because there are up to \(n!\) permutations (fewer with duplicates) and each `next_permutation` takes \(O(n)\) time. Space complexity is \(O(n! \cdot n)\) for the returned string in the worst case, plus \(O(1)\) auxiliary space for the permutation generation (excluding the storage of the output).
