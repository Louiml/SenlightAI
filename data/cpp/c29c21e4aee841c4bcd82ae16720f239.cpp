// Given a fixed-capacity integer list (as modeled by the provided `ArrayList` class, but without needing its actual header), write a standalone C++ free function `int findMostFrequent(const std::vector<int>& arr)` that returns the value that appears most frequently in the input vector. If there is a tie between two or more values for the highest frequency, return the smallest value among those tied. The input vector is non-empty but may contain duplicates, negative numbers, and unsorted values. Your function must not modify the input vector.
// The solution uses an unordered map (hash map) to count occurrences of each distinct integer in the input vector. First, iterate through the vector once and increment counts for each element. Then, iterate through the map to find the key with the highest count; if two keys have the same count, choose the smaller key. This ensures a deterministic tie‑breaker. Edge cases include a vector with all identical values (the answer is that value), a vector with all distinct values (any value could be chosen, but the smallest among ties is returned if counts are tied at 1, so the smallest overall value is returned), and negative values, which are handled naturally as map keys. Time complexity is O(n) for the counting pass plus O(d) for the map scan, where d is the number of distinct elements (≤ n), so total O(n) average. Space complexity is O(d) for the map. The function is const‑correct by taking the vector by const reference.
#include <vector>
#include <unordered_map>
#include <climits>

// Returns the most frequent value in the input vector.
// On ties, returns the smallest value among those with maximum frequency.
int findMostFrequent(const std::vector<int>& arr) {
    if (arr.empty()) {
        // Task guarantees non‑empty input; return a sentinel for safety.
        return 0;
    }

    std::unordered_map<int, int> freq;
    for (int val : arr) {
        ++freq[val];
    }

    int bestVal = INT_MAX;
    int bestCount = 0;

    for (const auto& entry : freq) {
        if (entry.second > bestCount ||
            (entry.second == bestCount && entry.first < bestVal)) {
            bestCount = entry.second;
            bestVal = entry.first;
        }
    }
    return bestVal;
}
#include <cassert>
#include <vector>

int main() {
    // Single element
    assert(findMostFrequent({7}) == 7);

    // All identical
    std::vector<int> v1 = {3, 3, 3};
    assert(findMostFrequent(v1) == 3);

    // Tie: two values appear twice → choose smallest
    std::vector<int> v2 = {1, 2, 2, 1, 3};
    assert(findMostFrequent(v2) == 1);

    // Negative values
    std::vector<int> v3 = {-5, -1, -1, -5, -2};
    assert(findMostFrequent(v3) == -5);  // Both -5 and -1 appear twice, -5 is smaller

    // All distinct → smallest is returned (all have count 1)
    std::vector<int> v4 = {10, 2, 30, 1};
    assert(findMostFrequent(v4) == 1);

    // Larger frequency away from smallest value
    std::vector<int> v5 = {4, 4, 4, 2, 2, 1};
    assert(findMostFrequent(v5) == 4);

    return 0;
}
