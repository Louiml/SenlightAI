// Write a C++ function `lexicographicSort` that takes a `std::vector<std::string>` by reference and sorts it lexicographically by character values (using standard ASCII ordering), but with a special rule: when comparing two strings, the comparison must be performed character-by-character from left to right, and if one string is a prefix of the other, the shorter string should be considered smaller. However, to make the task more interesting, the sort must be implemented using a custom recursive function that partitions the vector exactly like the provided `sort` function (which is a quicksort variant that groups equal elements by the current digit and recurses with an incremented digit for the equal group). Your function must not use `std::sort`, `std::stable_sort`, or any other standard sorting algorithm directly; only `std::swap` is allowed for element movement. The input vector may contain empty strings and duplicate strings. After sorting, the vector should be in non-decreasing lexicographic order (standard `std::string` comparison). The function must be deterministic (no random pivots—use the middle element as pivot) and must handle arbitrary-length strings. The function signature: `void lexicographicSort(std::vector<std::string>& vec);`. You may define helper functions and use const-correctness where appropriate.
The solution mimics the provided `sort` function but adapts it to work with `std::string` and character access. The core algorithm is a recursive quicksort that, at each level, considers a specific character position (starting at index 0). The pivot is chosen as the middle element of the current range. The vector is partitioned into three groups: elements whose character at the current position is less than the pivot's character, equal, and greater. The equal group is collected by swapping equal elements to the end of the range, then moved to the middle. After partitioning: (1) recursively sort the "less" group at the same character position; (2) if not all strings in the equal group are prefix-terminated at this position (i.e., at least one string has a non-null character here), recursively sort the equal group at the next character position; (3) recursively sort the "greater" group at the same position. A string that has no character at the current position (i.e., its length is <= digit) is treated as having a "null" character (value 0). This ensures empty strings and prefixes sort correctly: a shorter string that ends early will be considered smaller than any longer string that has more characters at that position. The base case is when the range size is 0 or 1. Edge cases include empty strings, strings of different lengths, and duplicates. The time complexity is O(N * L) on average, where N is the number of strings and L is the average length, because each level of recursion processes the entire vector once per digit, and the recursion depth is bounded by the maximum string length. In the worst case (already sorted or many repeated prefixes), it could be O(N*L^2) due to unbalanced partitions, but the middle-pivot choice mitigates typical cases. Space complexity is O(L) for the recursion stack depth plus O(1) auxiliary per call (excluding the vector itself).
#include <vector>
#include <string>
#include <cstddef>

// Helper to get character at a given digit, returning '\0' if out of range.
static char charAt(const std::string& s, std::size_t digit) {
    return (digit < s.size()) ? s[digit] : '\0';
}

// Recursive 3-way quicksort on a range of strings, comparing character at 'digit'.
static void sortRange(std::vector<std::string>& vec,
                      std::vector<std::string>::iterator begin,
                      std::vector<std::string>::iterator end,
                      std::size_t digit) {
    if (end - begin <= 1) return;

    // Choose middle element as pivot to avoid randomness.
    auto pivotIter = begin + (end - begin) / 2;
    char pivotChar = charAt(*pivotIter, digit);

    // Move pivot to the end temporarily.
    std::swap(*pivotIter, *(end - 1));

    auto lessIter = begin;          // next position for elements < pivot
    auto greaterIter = begin;       // next position for elements > pivot
    auto equalEnd = end - 1;        // where equal elements are moved to initially

    bool hasNonTerminated = false;

    auto iter = begin;
    while (iter != equalEnd) {
        char c = charAt(*iter, digit);
        if (c != '\0') hasNonTerminated = true;

        if (c < pivotChar) {
            std::swap(*iter, *lessIter);
            ++lessIter;
            ++iter;
        } else if (c == pivotChar) {
            --equalEnd;
            std::swap(*iter, *equalEnd);
        } else {
            ++iter;
        }
    }

    // Now move equal elements from the tail to just after the less group.
    auto equalStart = lessIter;
    auto tailIter = equalEnd;
    while (tailIter != end) {
        std::swap(*tailIter, *lessIter);
        ++tailIter;
        ++lessIter;
    }
    auto equalEndFinal = lessIter;

    // Recursively sort less group, equal group (if needed), and greater group.
    sortRange(vec, begin, equalStart, digit);
    if (hasNonTerminated || (equalStart != equalEndFinal && charAt(*(equalStart), digit) != '\0')) {
        sortRange(vec, equalStart, equalEndFinal, digit + 1);
    }
    sortRange(vec, equalEndFinal, end, digit);
}

// Public function: sorts strings lexicographically using a custom quicksort.
void lexicographicSort(std::vector<std::string>& vec) {
    sortRange(vec, vec.begin(), vec.end(), 0);
}
#include <cassert>
#include <string>
#include <vector>

// The solution function is declared above; include its definition here.
// For this test, we assume the solution code is already included.

int main() {
    // Basic lexicographic ordering
    std::vector<std::string> v1 = {"cat", "apple", "bat", "apple"};
    lexicographicSort(v1);
    assert(v1 == std::vector<std::string>({"apple", "apple", "bat", "cat"}));

    // Prefix handling: "a" < "aa", "ab" < "b"
    std::vector<std::string> v2 = {"b", "a", "aa", "ab"};
    lexicographicSort(v2);
    assert(v2 == std::vector<std::string>({"a", "aa", "ab", "b"}));

    // Empty strings first
    std::vector<std::string> v3 = {"", "x", "", "y"};
    lexicographicSort(v3);
    assert(v3 == std::vector<std::string>({"", "", "x", "y"}));

    // Duplicates and longer strings
    std::vector<std::string> v4 = {"zzz", "zz", "z", "zzz", "zz"};
    lexicographicSort(v4);
    assert(v4 == std::vector<std::string>({"z", "zz", "zz", "zzz", "zzz"}));

    // Mixed lengths and repeated prefixes
    std::vector<std::string> v5 = {"ab", "a", "abc", "ab", "a", "abcd"};
    lexicographicSort(v5);
    assert(v5 == std::vector<std::string>({"a", "a", "ab", "ab", "abc", "abcd"}));

    // Single element
    std::vector<std::string> v6 = {"hello"};
    lexicographicSort(v6);
    assert(v6 == std::vector<std::string>({"hello"}));

    // Large case to test stability not required, but correctness
    std::vector<std::string> v7 = {"banana", "apple", "cherry", "apple", "banana", "cherry"};
    lexicographicSort(v7);
    assert(v7 == std::vector<std::string>({"apple", "apple", "banana", "banana", "cherry", "cherry"}));

    return 0;
}
