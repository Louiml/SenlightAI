// Write a C++ function `sortNamesById` that takes a vector of pairs, where each pair consists of an integer ID and a string name, and returns a new vector of strings containing only the names, sorted by their corresponding ID in ascending order. If two or more entries have the same ID, preserve their original relative order (stable sorting). The function must not modify the input vector, and it should handle empty input by returning an empty vector. IDs are unique in the original problem, but your implementation must still be deterministic and correct when duplicates appear.
The task is to create a stable sort of names based on their integer IDs. The main algorithm is straightforward: copy the input vector into a local mutable vector, sort it using `std::stable_sort` with a lambda that compares only the first element (the ID) of each pair. Unlike `std::sort`, `stable_sort` guarantees that equal elements (same ID) keep their original order. After sorting, extract the names (the second element of each pair) into a new vector of strings. Edge cases: empty input returns an empty vector; duplicate IDs must not reorder names; negative IDs must work naturally; ties in ID are handled by stability. Time complexity is O(N log N) due to sorting, and space complexity is O(N) for the copies (the local vector and the result vector).
#include <vector>
#include <string>
#include <algorithm>
#include <utility>

// Returns the names from `entries` sorted by their integer IDs (stable).
// The input vector is not modified. Duplicate IDs preserve original order.
std::vector<std::string> sortNamesById(const std::vector<std::pair<int, std::string>>& entries) {
    // Create a mutable copy of the input to sort.
    auto sorted = entries;
    
    // Stable sort by the ID only (the first element of the pair).
    std::stable_sort(sorted.begin(), sorted.end(),
                     [](const std::pair<int, std::string>& a,
                        const std::pair<int, std::string>& b) {
                         return a.first < b.first;
                     });
    
    // Extract only the names in the sorted order.
    std::vector<std::string> result;
    result.reserve(sorted.size());
    for (const auto& entry : sorted) {
        result.push_back(entry.second);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>

// Function under test is declared above or included here.
// (For full compilation, the function definition must be visible.)

int main() {
    // Test 1: Simple sorted input
    std::vector<std::pair<int, std::string>> data1 = {{1, "A"}, {2, "B"}, {3, "C"}};
    assert(sortNamesById(data1) == std::vector<std::string>({"A", "B", "C"}));

    // Test 2: Reverse order
    std::vector<std::pair<int, std::string>> data2 = {{3, "C"}, {1, "A"}, {2, "B"}};
    assert(sortNamesById(data2) == std::vector<std::string>({"A", "B", "C"}));

    // Test 3: Duplicate IDs must remain stable
    std::vector<std::pair<int, std::string>> data3 = {{2, "B"}, {1, "X"}, {2, "A"}};
    assert(sortNamesById(data3) == std::vector<std::string>({"X", "B", "A"}));

    // Test 4: Empty vector
    std::vector<std::pair<int, std::string>> data4 = {};
    assert(sortNamesById(data4) == std::vector<std::string>());

    // Test 5: Negative IDs and ties
    std::vector<std::pair<int, std::string>> data5 = {{-1, "neg"}, {0, "zero"}, {-1, "neg2"}};
    assert(sortNamesById(data5) == std::vector<std::string>({"neg", "neg2", "zero"}));

    // Test 6: Input not modified (const correctness)
    auto original = data2;
    sortNamesById(data2);
    assert(data2 == original);

    return 0;
}
