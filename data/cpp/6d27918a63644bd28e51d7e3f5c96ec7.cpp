// Write a standalone C++ function that, given two vectors of strings (where each string consists only of lowercase letters 'a'–'z'), returns the maximum absolute difference in length between any string from the first vector and any string from the second vector. If either vector is empty, the function must return -1. For example, given `a1 = {"aa", "b"}` and `a2 = {"cccc", "dd"}`, the maximum difference is `|1 - 4| = 3`. The function must follow the signature `int maxLengthDiff(const std::vector<std::string>& a1, const std::vector<std::string>& a2)` and must not modify the input vectors.
#include <cassert>
#include <vector>
#include <string>

// The solution function is assumed to be defined above (or included here).
// For completeness, include the declaration again (or the full definition from the solution section).
// In a proper setup, the function definition from the Solution section is available.
// Here we assume it is already included.

int main() {
    // Example from the problem statement
    std::vector<std::string> s1 = {"hoqq", "bbllkw", "oox", "ejjuyyy", "plmiis", "xxxzgpsssa", "xxwwkktt", "znnnnfqknaz", "qqquuhii", "dvvvwz"};
    std::vector<std::string> s2 = {"cccooommaaqqoxii", "gggqaffhhh", "tttoowwwmmww"};
    assert(maxLengthDiff(s1, s2) == 13);

    // Basic case
    std::vector<std::string> a1 = {"aa", "b"};
    std::vector<std::string> a2 = {"cccc", "dd"};
    assert(maxLengthDiff(a1, a2) == 3); // |1-4|=3

    // Same lengths
    std::vector<std::string> b1 = {"x", "yy"};
    std::vector<std::string> b2 = {"z", "ww"};
    assert(maxLengthDiff(b1, b2) == 1); // |1-2|=1

    // Single string in each
    std::vector<std::string> c1 = {"hello"};
    std::vector<std::string> c2 = {"world"};
    assert(maxLengthDiff(c1, c2) == 0);

    // Empty first vector
    std::vector<std::string> d1;
    std::vector<std::string> d2 = {"abc"};
    assert(maxLengthDiff(d1, d2) == -1);

    // Empty second vector
    std::vector<std::string> e1 = {"a"};
    std::vector<std::string> e2;
    assert(maxLengthDiff(e1, e2) == -1);

    // Large difference
    std::vector<std::string> f1 = {"a"}; // length 1
    std::vector<std::string> f2 = {"abcdefghij"}; // length 10
    assert(maxLengthDiff(f1, f2) == 9);

    // Multiple strings, all same length in one vector
    std::vector<std::string> g1 = {"cat", "dog", "bat"};
    std::vector<std::string> g2 = {"hi", "no"};
    assert(maxLengthDiff(g1, g2) == 1); // |3-2|=1

    return 0;
}
#include <vector>
#include <string>
#include <algorithm>
#include <limits>

// Return the maximum absolute length difference between any string in a1 and any string in a2.
// If either vector is empty, return -1.
int maxLengthDiff(const std::vector<std::string>& a1, const std::vector<std::string>& a2) {
    if (a1.empty() || a2.empty()) {
        return -1;
    }

    // Find min and max lengths in a1
    size_t min1 = a1[0].size();
    size_t max1 = a1[0].size();
    for (const auto& s : a1) {
        min1 = std::min(min1, s.size());
        max1 = std::max(max1, s.size());
    }

    // Find min and max lengths in a2
    size_t min2 = a2[0].size();
    size_t max2 = a2[0].size();
    for (const auto& s : a2) {
        min2 = std::min(min2, s.size());
        max2 = std::max(max2, s.size());
    }

    // The maximum difference is the larger of (max1 - min2) and (max2 - min1)
    // Since lengths are non-negative, these are always non-negative.
    int diff1 = static_cast<int>(max1 - min2);
    int diff2 = static_cast<int>(max2 - min1);

    return std::max(diff1, diff2);
}
// The solution reduces the problem to finding the extreme string lengths in each vector. The maximum absolute difference between any pair of elements from two sets of numbers is achieved by comparing the maxima and minima of the two sets. Specifically, the maximum difference is `max(max1 - min2, max2 - min1)`, where `max1` and `min1` are the longest and shortest lengths in `a1`, and `max2` and `min2` correspond to `a2`. To handle this: first check if either vector is empty, returning -1 if so. Otherwise, iterate over each vector once to find its minimum and maximum string length. Then compute the two possible differences and take the larger one. This approach is far more efficient than checking all pairs. Time complexity is O(n + m) where n and m are the sizes of the two vectors, since we scan each once. Space complexity is O(1) additional memory beyond the input. Edge cases: empty vectors return -1; vectors with a single string have min == max, so the difference is the absolute difference between that length and the extreme of the other vector; and the lengths themselves are non-negative integers, so the result is always non-negative when inputs are non-empty.
