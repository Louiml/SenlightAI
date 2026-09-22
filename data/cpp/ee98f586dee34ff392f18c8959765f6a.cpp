Write a C++ function named `seatingAssignments` that takes a vector of pairs where each pair contains a student roll number (integer) and a desk number (integer). The function should return an `unordered_map<int,int>` where the key is the roll number and the value is the desk number, but with the following rules: if a roll number appears multiple times in the input, only the **last** occurrence's desk number should be kept (i.e., later assignments override earlier ones). The order of keys in the returned map does not matter (since it's unordered). Handle empty input by returning an empty map. The function must be `const`-correct in the sense that it does not modify the input vector.
The solution uses an `unordered_map<int,int>` to store the final mapping. Iterate through the vector of pairs from beginning to end. For each pair, use the bracket operator `map[roll] = desk`, which will either insert a new key-value pair or overwrite the existing value if the roll number is already present. This directly satisfies the "last occurrence wins" requirement because each new assignment replaces the previous one. After processing all pairs, return the map by value. Edge cases: empty input → return an empty map (the default constructor handles this automatically). Duplicate roll numbers with different desk numbers → the final desk number is the one from the last pair in the input order. Time complexity is O(n) average-case for the loop over n pairs, with each insert/lookup being O(1) average due to hash table. Space complexity is O(u) where u is the number of unique roll numbers, since the map only stores one entry per unique key.
#include <unordered_map>
#include <vector>
#include <utility>

// Given a vector of (roll_number, desk_number) pairs, return an unordered_map
// where each roll number maps to the desk number from its LAST occurrence.
std::unordered_map<int, int> seatingAssignments(const std::vector<std::pair<int, int>>& assignments) {
    std::unordered_map<int, int> result;
    // Iterate through all pairs; later assignments overwrite earlier ones.
    for (const auto& entry : assignments) {
        result[entry.first] = entry.second;
    }
    return result;
}
#include <cassert>
#include <unordered_map>
#include <vector>
#include <utility>

// Function declaration (provided by the solution)
std::unordered_map<int, int> seatingAssignments(const std::vector<std::pair<int, int>>& assignments);

int main() {
    // Test 1: Basic insertion
    std::vector<std::pair<int, int>> t1 = {{1, 53}, {2, 54}, {3, 55}};
    auto r1 = seatingAssignments(t1);
    assert(r1.size() == 3);
    assert(r1[1] == 53);
    assert(r1[2] == 54);
    assert(r1[3] == 55);

    // Test 2: Overwrite duplicate roll numbers (last wins)
    std::vector<std::pair<int, int>> t2 = {{1, 53}, {2, 54}, {2, 57}, {1, 60}};
    auto r2 = seatingAssignments(t2);
    assert(r2.size() == 2);
    assert(r2[1] == 60);
    assert(r2[2] == 57);

    // Test 3: All duplicate roll numbers
    std::vector<std::pair<int, int>> t3 = {{5, 10}, {5, 20}, {5, 30}};
    auto r3 = seatingAssignments(t3);
    assert(r3.size() == 1);
    assert(r3[5] == 30);

    // Test 4: Empty input
    std::vector<std::pair<int, int>> t4;
    auto r4 = seatingAssignments(t4);
    assert(r4.empty());

    // Test 5: Multiple unique keys with interleaved duplicates
    std::vector<std::pair<int, int>> t5 = {{7, 1}, {8, 2}, {7, 3}, {9, 4}, {8, 5}};
    auto r5 = seatingAssignments(t5);
    assert(r5.size() == 3);
    assert(r5[7] == 3);
    assert(r5[8] == 5);
    assert(r5[9] == 4);

    // Test 6: Large values and negative roll numbers (unlikely but allowed)
    std::vector<std::pair<int, int>> t6 = {{-1, 100}, {2, 200}, {-1, 300}};
    auto r6 = seatingAssignments(t6);
    assert(r6.size() == 2);
    assert(r6[-1] == 300);
    assert(r6[2] == 200);

    return 0;
}
