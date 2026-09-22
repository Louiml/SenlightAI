// Write a C++ function `std::vector<std::vector<int>> largeGroupPositions(const std::string& s)` that, given a non-empty string `s` composed only of lowercase English letters, returns a vector of intervals `[start, end]` (both inclusive) for every maximal consecutive group of identical characters whose length is at least 3. The intervals must appear in increasing order of their starting index, and the function should return an empty vector if no such group exists or if the string length is less than 3. The input string must not be modified, and the function must be const-correct (i.e., accept by `const std::string&`).
// The solution uses a single linear scan with a sliding-window pointer. Start at index `0` and, for each position `i`, set `start = i`, then advance a second pointer `j = i + 1` while `j < s.length()` and `s[j] == s[start]`. When the inner loop stops, the length of the current run is `j - start`. If it is at least 3, record the interval `{start, j-1}`. Then set `i = j` to skip past the entire run and continue. This works because each character is examined at most twice (once in the inner while, once when jumping `i`), so time complexity is O(n) for a string of length n. Space complexity is O(1) auxiliary, excluding the space needed for the output vector, which in the worst case (e.g., runs of exactly length 3 alternating with different characters) could contain up to n/3 intervals—still O(n) output. Edge cases include: string length < 3 (return empty); a run exactly length 3 (included); a run longer than 3 (single interval for the whole run); and runs at the very end of the string (the while loop must check `j < s.length()` before accessing `s[j]` to avoid out-of-bounds).
#include <string>
#include <vector>

// Returns start and end indices (inclusive) of every maximal group of identical
// characters with length >= 3 in s. The intervals appear in increasing start order.
std::vector<std::vector<int>> largeGroupPositions(const std::string& s) {
    std::vector<std::vector<int>> positions;
    const int n = static_cast<int>(s.length());
    if (n < 3) {
        return positions;
    }

    int i = 0;
    while (i < n - 2) {
        const int start = i;
        int j = i + 1;
        while (j < n && s[j] == s[start]) {
            ++j;
        }
        if (j - start >= 3) {
            positions.push_back({start, j - 1});
        }
        i = j;  // Skip the entire run
    }

    return positions;
}
#include <cassert>
#include <vector>

int main() {
    // Basic cases
    assert(largeGroupPositions("abbxxxxzzy") == std::vector<std::vector<int>>({{3, 6}}));
    assert(largeGroupPositions("abc") == std::vector<std::vector<int>>());
    assert(largeGroupPositions("abcdddeeeeaabbbcd") == std::vector<std::vector<int>>({{3, 5}, {6, 9}, {12, 14}}));

    // Edge cases: length < 3, exactly 3, run at ends, multiple runs
    assert(largeGroupPositions("ab") == std::vector<std::vector<int>>());
    assert(largeGroupPositions("aaab") == std::vector<std::vector<int>>({{0, 2}}));
    assert(largeGroupPositions("baaa") == std::vector<std::vector<int>>({{1, 3}}));
    assert(largeGroupPositions("aaabbb") == std::vector<std::vector<int>>({{0, 2}, {3, 5}}));
    assert(largeGroupPositions("aaaa") == std::vector<std::vector<int>>({{0, 3}}));

    // Single character repeated many times
    assert(largeGroupPositions("cccccccc") == std::vector<std::vector<int>>({{0, 7}}));

    // Runs of length exactly 3, separated by different characters
    assert(largeGroupPositions("aaabcccd") == std::vector<std::vector<int>>({{0, 2}, {4, 6}}));

    // No runs of length >= 3
    assert(largeGroupPositions("aabbcc") == std::vector<std::vector<int>>());

    // Empty string (though problem says non-empty, function handles it)
    assert(largeGroupPositions("") == std::vector<std::vector<int>>());

    // Mixed with runs longer than 3
    assert(largeGroupPositions("aaabbbbbcc") == std::vector<std::vector<int>>({{0, 2}, {3, 7}}));

    return 0;
}
