// Write a C++ function `int minimalWeightSelection(const std::string& letters, int k)` that takes a string `letters` consisting of lowercase English letters and an integer `k`. The function must select exactly `k` distinct characters from the string such that the absolute difference between the ASCII values of any two consecutively selected characters (when sorted alphabetically) is at least 2. If no such selection of size `k` is possible, return -1. Otherwise, return the sum of the positions of the selected letters in the alphabet (where 'a'=1, 'b'=2, ..., 'z'=26). The input string may contain duplicate letters, and you must use each character at most once. The selection does not need to preserve the original order; you may sort the string first.
#include <cassert>
#include <string>

// Function prototype (from solution)
int minimalWeightSelection(const std::string& letters, int k);

int main() {
    // Basic feasible case: "abcde", k=3 -> choose a, c, e (differences: 2 and 2) sum = 1+3+5 = 9
    assert(minimalWeightSelection("abcde", 3) == 9);

    // Impossible case: "aaa", k=2 -> only one distinct letter, cannot pick two
    assert(minimalWeightSelection("aaa", 2) == -1);

    // Duplicates but enough distinct: "abab", k=2 -> possible? a and b differ by 1, so no. Only 'a' and 'b' exist, diff=1 <2 -> -1
    assert(minimalWeightSelection("abab", 2) == -1);

    // Case with gap: "aceg", k=2 -> any two work, but greedy picks a and c, sum=1+3=4
    assert(minimalWeightSelection("aceg", 2) == 4);

    // Case with duplicates and needs skip close ones: "aabcde", k=3 -> sorted "aabcde" -> pick a, c, e (skip second a, b; c, e) sum=9
    assert(minimalWeightSelection("aabcde", 3) == 9);

    // k larger than distinct letters: "z", k=2 -> -1
    assert(minimalWeightSelection("z", 2) == -1);

    // Single letter: "m", k=1 -> sum = 13
    assert(minimalWeightSelection("m", 1) == 13);

    // All distinct but impossible due to adjacency: "abc", k=3 -> cannot pick a,b,c (adjacent) -> only a and c possible, so -1
    assert(minimalWeightSelection("abc", 3) == -1);

    // Larger gap: "xyz", k=2 -> x and z (diff=2) sum = 24+26=50
    assert(minimalWeightSelection("xyz", 2) == 50);

    // Mixed duplicates and gap: "aabbccddeeff", k=3 -> sorted "aabbccddeeff" -> pick a, c, e? diff 2,2, sum=1+3+5=9, but c and d are too close, so after a pick a, skip b, pick c, skip d, pick e => sum 9
    assert(minimalWeightSelection("aabbccddeeff", 3) == 9);

    return 0;
}
#include <string>
#include <algorithm>
#include <vector>

// Select exactly k distinct letters such that consecutive selected letters (after sorting)
// have ASCII difference at least 2. Return sum of alphabet positions (a=1...z=26), or -1 if impossible.
int minimalWeightSelection(const std::string& letters, int k) {
    if (k <= 0) return -1; // k must be positive

    std::string sortedLetters = letters;
    std::sort(sortedLetters.begin(), sortedLetters.end());

    std::string selected;
    selected.reserve(k);

    for (char c : sortedLetters) {
        // If we already have k letters, stop
        if (selected.size() == static_cast<size_t>(k)) break;

        // If this is the first selection, or the gap is at least 2, select it
        if (selected.empty() || (c - selected.back()) >= 2) {
            selected.push_back(c);
        }
    }

    if (selected.size() != static_cast<size_t>(k)) {
        return -1;
    }

    int totalWeight = 0;
    for (char c : selected) {
        totalWeight += (c - 'a' + 1);
    }
    return totalWeight;
}
// The problem requires selecting `k` distinct characters from the input string such that, after sorting them alphabetically, any two adjacent selected characters differ by at least 2 in ASCII value (i.e., no adjacent letters like 'a' and 'b' can both be selected, but 'a' and 'c' can). The goal is to check feasibility and, if feasible, compute the sum of alphabet indices (1‑based). The optimal strategy is to sort the string in ascending order and then greedily build the selection: always take the first available character, then for each subsequent position, pick the smallest character that is at least 2 positions away from the last selected character (i.e., ASCII difference ≥ 2). This greedy works because taking the smallest possible character at each step leaves maximum room for future selections, and the sorted order ensures we never miss a valid candidate earlier that could enable a larger set. We also need to handle duplicates: once a character is used, we cannot reuse it, but duplicates of the same letter are indistinguishable for selection purposes—we simply skip any occurrence that is too close to the last selected. Edge cases include: `k` greater than the number of distinct letters, `k`=0 (but the problem implies positive k), and cases where no valid combination exists (return -1). After building the selection, sum the 1‑based positions (`letter - 'a' + 1`). Time complexity is O(n log n) due to sorting, plus O(n) for the scan, so overall O(n log n). Space complexity is O(n) for the sorted copy or O(1) extra if sorting in place, but we can use a copy to keep the input unchanged.
