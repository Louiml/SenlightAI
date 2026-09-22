Write a C++ function named `analyzeCollection` that takes a vector of integers (possibly containing duplicates, zeros, negatives, and unordered values) and returns a `std::map<int, int>` where each key is a unique value from the input vector, and its associated mapped value is the number of times that value appears (frequency) in the input. The function must handle an empty input vector by returning an empty map. It must not use any global state or modify the input vector (pass by const reference). Ensure the function is const-correct and uses only standard library facilities.

// The solution approach is to iterate over each element in the input vector and increment a counter stored in a `std::map<int, int>`. Because `std::map` automatically keeps keys sorted and does not allow duplicate keys, using the `operator[]` or `.insert()` with increment will accumulate frequencies correctly. The key insight is that accessing `freqMap[value]` returns a reference to the mapped value for that key, default-initializing it to zero if absent, then incrementing it. For an empty input, the loop does not execute and the map remains empty, which satisfies the edge case. Duplicate values are naturally aggregated because each occurrence increments the same key. Negative values and zeros are handled identically, as `int` keys can be any integer. Time complexity is O(n log n) due to each insertion/lookup in the map costing O(log k), where k is the number of distinct values, and there are n total elements. Space complexity is O(k) for the map, where k ≤ n.

#include <map>
#include <vector>

// Build a frequency map from a vector of integers.
// Keys are unique input values, mapped values are their counts.
std::map<int, int> analyzeCollection(const std::vector<int>& values) {
    std::map<int, int> freq;
    for (int v : values) {
        ++freq[v];  // operator[] inserts default 0 if missing, then increments
    }
    return freq;
}

#include <cassert>
#include <map>
#include <vector>

// Assume the solution function is defined above or included here.

int main() {
    // Test 1: Basic mixed values with duplicates
    std::vector<int> input1 = {1, 2, 2, 3, 3, 3, 0, -1};
    std::map<int, int> expected1 = {{-1, 1}, {0, 1}, {1, 1}, {2, 2}, {3, 3}};
    assert(analyzeCollection(input1) == expected1);

    // Test 2: All identical values
    std::vector<int> input2 = {7, 7, 7};
    std::map<int, int> expected2 = {{7, 3}};
    assert(analyzeCollection(input2) == expected2);

    // Test 3: Empty input returns empty map
    std::vector<int> input3;
    std::map<int, int> expected3;
    assert(analyzeCollection(input3) == expected3);

    // Test 4: Single element
    std::vector<int> input4 = {42};
    std::map<int, int> expected4 = {{42, 1}};
    assert(analyzeCollection(input4) == expected4);

    // Test 5: Negative values and zeros only
    std::vector<int> input5 = {0, -5, -5, 0, -5};
    std::map<int, int> expected5 = {{-5, 3}, {0, 2}};
    assert(analyzeCollection(input5) == expected5);

    // Test 6: Large sequence without duplicates (all values unique)
    std::vector<int> input6 = {10, 20, 30};
    std::map<int, int> expected6 = {{10, 1}, {20, 1}, {30, 1}};
    assert(analyzeCollection(input6) == expected6);

    return 0;
}
