/*
Write a C++ function that takes a single integer `n` and a vector of integers (with possible duplicates), and returns the smallest integer `x` such that `x` appears exactly once in the vector and `10 <= x < 3979`. If no such integer exists, return `-1`. The function must handle an empty vector gracefully (return `-1`). The input vector may be unsorted and contain negative numbers or numbers outside the range; only numbers within `[10, 3978]` that occur exactly once are considered.
*/
#include <unordered_map>
#include <vector>

// Return the smallest integer in [10, 3978] that appears exactly once in the input.
// If no such number exists, return -1.
int findSingleOccurrence(const std::vector<int>& numbers) {
    std::unordered_map<int, int> freq;
    for (int num : numbers) {
        ++freq[num];
    }
    for (int candidate = 10; candidate < 3979; ++candidate) {
        auto it = freq.find(candidate);
        if (it != freq.end() && it->second == 1) {
            return candidate;
        }
    }
    return -1;
}
#include <cassert>
#include <vector>

// The solution function is declared above.

int main() {
    std::vector<int> test1 = {15, 15, 20, 10, 10};
    assert(findSingleOccurrence(test1) == 20);

    std::vector<int> test2 = {10, 11, 12};
    assert(findSingleOccurrence(test2) == 10);

    std::vector<int> test3 = {};
    assert(findSingleOccurrence(test3) == -1);

    std::vector<int> test4 = {5, 6, 7, 8, 9};
    assert(findSingleOccurrence(test4) == -1);

    std::vector<int> test5 = {3978, 3978, 3979, 10};
    assert(findSingleOccurrence(test5) == 10);

    std::vector<int> test6 = {10, 11, 12, 11, 10};
    assert(findSingleOccurrence(test6) == 12);

    std::vector<int> test7 = {10, 10, 11, 11, 12, 12};
    assert(findSingleOccurrence(test7) == -1);

    std::vector<int> test8 = {-1, 0, 1, 2};
    assert(findSingleOccurrence(test8) == -1);

    std::vector<int> test9 = {100, 200, 100, 200, 150};
    assert(findSingleOccurrence(test9) == 150);

    std::vector<int> test10 = {10, 10, 10, 11};
    assert(findSingleOccurrence(test10) == 11);
}
// The solution uses a hash map (unordered_map) to count occurrences of each integer in the vector. After counting, iterate `i` from `10` to `3978` inclusive and check if `i` appears in the map and its count is exactly `1`. The first such `i` found is returned because we iterate in ascending order, guaranteeing the smallest. If the loop completes without finding any match, return `-1`. Edge cases: empty vector → immediately return -1; numbers outside range are ignored; duplicates of a value outside range are irrelevant; if multiple candidates exist, the smallest index is returned. Time complexity: O(n + 3970) ≈ O(n) since the loop bound is constant. Space complexity: O(n) for the hash map storing up to n distinct values. The function is `const`-correct: the input vector is passed by `const` reference.
