Write a C++ function named `hashJoin` that performs an inner hash join on two vectors of integer pairs. The function takes two vectors of `std::pair<int, int>` representing left and right tables, where the first integer of each pair is the join key and the second is a payload value. It returns a vector of `std::pair<int, int>` containing all matched pairs from both sides concatenated: for each matching key, the result should contain the left payload followed by the right payload (i.e., a pair `{left_value, right_value}`). Multiple matches must be handled correctly: if a key appears multiple times in the left table and multiple times in the right table, all combinations must appear in the result. The input vectors are non-empty but may contain duplicate keys. The function must be efficient using a hash map and should not modify the input vectors. Assume all payload values are non-negative integers.
// The solution builds a hash table mapping each distinct key from the left table to a list (vector) of all payload values associated with that key. We iterate through the left table once to populate this hash map. Then we iterate through the right table; for each row, we look up its key in the hash map. If found, for every left payload stored in the map for that key, we append a pair `{left_payload, right_payload}` to the result vector. This ensures that all combinations of matching rows are produced. Edge cases include duplicate keys in both sides (handled by storing all left payloads and iterating all combinations), keys that appear only on one side (ignored since it's an inner join), and empty left or right vectors (though spec says non-empty, we still handle gracefully). Time complexity is O(L + R + M) where L and R are sizes of input vectors and M is the number of matched output pairs, because both hash map insertion and lookup average O(1). Space complexity is O(L + M) for storing the hash map and the result.
#include <vector>
#include <unordered_map>
#include <utility>

/**
 * Perform an inner hash join on two vectors of (key, value) pairs.
 * Returns a vector of (left_value, right_value) pairs for all matching keys.
 */
std::vector<std::pair<int, int>> hashJoin(
    const std::vector<std::pair<int, int>>& left,
    const std::vector<std::pair<int, int>>& right) {
    
    // Map each key from the left table to a list of values.
    std::unordered_map<int, std::vector<int>> hashTable;
    for (const auto& [key, value] : left) {
        hashTable[key].push_back(value);
    }
    
    std::vector<std::pair<int, int>> result;
    for (const auto& [key, rightValue] : right) {
        auto it = hashTable.find(key);
        if (it != hashTable.end()) {
            // For each left value with the same key, produce a combined pair.
            for (int leftValue : it->second) {
                result.emplace_back(leftValue, rightValue);
            }
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Basic inner join.
    std::vector<std::pair<int, int>> left1 = {{1, 10}, {2, 20}, {3, 30}};
    std::vector<std::pair<int, int>> right1 = {{2, 200}, {3, 300}, {4, 400}};
    auto res1 = hashJoin(left1, right1);
    assert(res1 == (std::vector<std::pair<int, int>>{{20, 200}, {30, 300}}));

    // Duplicate keys on both sides produce all combinations.
    std::vector<std::pair<int, int>> left2 = {{1, 10}, {1, 11}, {2, 20}};
    std::vector<std::pair<int, int>> right2 = {{1, 100}, {1, 101}};
    auto res2 = hashJoin(left2, right2);
    assert(res2 == (std::vector<std::pair<int, int>>{{10, 100}, {10, 101}, {11, 100}, {11, 101}}));

    // Keys that don't match are omitted.
    std::vector<std::pair<int, int>> left3 = {{5, 50}, {6, 60}};
    std::vector<std::pair<int, int>> right3 = {{7, 70}, {8, 80}};
    auto res3 = hashJoin(left3, right3);
    assert(res3.empty());

    // Single match.
    std::vector<std::pair<int, int>> left4 = {{1, 1}};
    std::vector<std::pair<int, int>> right4 = {{1, 2}};
    auto res4 = hashJoin(left4, right4);
    assert(res4 == (std::vector<std::pair<int, int>>{{1, 2}}));

    // Multiple left values for one key, single right value.
    std::vector<std::pair<int, int>> left5 = {{1, 1}, {1, 2}, {1, 3}};
    std::vector<std::pair<int, int>> right5 = {{1, 10}};
    auto res5 = hashJoin(left5, right5);
    assert(res5 == (std::vector<std::pair<int, int>>{{1, 10}, {2, 10}, {3, 10}}));

    return 0;
}
