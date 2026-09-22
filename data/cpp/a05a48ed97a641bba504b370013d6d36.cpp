Write a C++ function that takes a vector of pairs of integers and a single integer value as parameters, and returns a `std::set<std::pair<int, int>>` containing only those pairs from the input vector whose second element (the "value" part) is equal to the given integer. The input vector may contain duplicate pairs, empty pairs are not possible (pairs always have two ints), and the vector may be empty. The output set should automatically remove duplicates and sort the pairs by first element (key) and then by second element (value) in ascending order. The function must be `const`-correct: it should not modify the input vector, and it should return the set by value.

// The solution is straightforward: iterate through each pair in the input vector, check if the pair's second element matches the target value, and if so, insert the pair into a `std::set` of pairs. Since `std::set` stores unique elements in sorted order, duplicates are automatically removed and ordering is handled. Edge cases: if the vector is empty, the function returns an empty set; if no pairs match, the set is empty; if multiple identical pairs match, only one copy is stored. Time complexity is O(n log m), where n is the number of pairs in the vector and m is the number of unique pairs inserted (at most n). Space complexity is O(m) for the returned set, plus O(1) auxiliary space. The `const` reference to the vector ensures no copy and no modification.

#include <set>
#include <vector>
#include <utility>

// Return a set of pairs whose second element equals target_value.
std::set<std::pair<int, int>> filterPairsByValue(
    const std::vector<std::pair<int, int>>& pairs,
    int target_value) {
    
    std::set<std::pair<int, int>> result;
    
    for (const auto& p : pairs) {
        if (p.second == target_value) {
            result.insert(p);
        }
    }
    
    return result;
}

#include <cassert>
#include <set>
#include <vector>
#include <utility>

int main() {
    std::vector<std::pair<int, int>> empty;
    assert(filterPairsByValue(empty, 5).empty());

    std::vector<std::pair<int, int>> v1 = {{1, 2}, {3, 2}, {1, 2}, {5, 0}, {2, 2}};
    std::set<std::pair<int, int>> expected1 = {{1, 2}, {2, 2}, {3, 2}};
    assert(filterPairsByValue(v1, 2) == expected1);

    std::vector<std::pair<int, int>> v2 = {{-1, 10}, {0, -5}, {10, 10}};
    std::set<std::pair<int, int>> expected2 = {{-1, 10}, {10, 10}};
    assert(filterPairsByValue(v2, 10) == expected2);

    std::vector<std::pair<int, int>> v3 = {{0, 0}, {0, 0}, {0, 0}};
    std::set<std::pair<int, int>> expected3 = {{0, 0}};
    assert(filterPairsByValue(v3, 0) == expected3);

    std::vector<std::pair<int, int>> v4 = {{1, 2}, {3, 4}};
    assert(filterPairsByValue(v4, 99).empty());
}
