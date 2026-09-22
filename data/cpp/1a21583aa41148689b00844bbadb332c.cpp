/*
Write a C++ function that simulates C++ namespace merging behavior. Given a vector of vectors of pairs, where each inner vector represents a namespace block and each pair contains a variable name (string) and its type (char, where 'i' = int, 'f' = float, 'd' = double), merge all namespace blocks into a single unified namespace. The function should return a sorted vector of pairs representing the final merged namespace, following C++ rules: if a variable name appears in multiple blocks, keep only the last declaration (most recent block wins); if the same name is declared twice within the same block (which is illegal in C++), the function should throw a `std::invalid_argument`. The input may contain empty blocks, and blocks may reference variables that appear in earlier blocks. The output pairs should be sorted alphabetically by variable name, and for equal names (which shouldn't happen in valid output), by type. The function must be `const`-correct and use only standard library facilities.
*/

#include <string>
#include <vector>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <utility>

// Merge multiple namespace blocks into a single sorted list of (name, type) pairs.
// Later blocks override earlier declarations for the same name.
// Throws std::invalid_argument if a name is declared twice within the same block.
std::vector<std::pair<std::string, char>> mergeNamespaces(
    const std::vector<std::vector<std::pair<std::string, char>>>& blocks) {
    
    // Map from name to type, maintaining the latest seen type.
    std::map<std::string, char> merged;
    
    for (const auto& block : blocks) {
        // Temporary set to detect duplicates within this block.
        std::vector<std::string> declared_in_block;
        for (const auto& entry : block) {
            const std::string& name = entry.first;
            char type = entry.second;
            
            // Check for duplicate declaration in the same block.
            if (std::find(declared_in_block.begin(), declared_in_block.end(), name) != declared_in_block.end()) {
                throw std::invalid_argument("Duplicate declaration in same namespace block: " + name);
            }
            declared_in_block.push_back(name);
            
            // Overwrite with the latest type (since later blocks take precedence).
            merged[name] = type;
        }
    }
    
    // Convert map to vector of pairs.
    std::vector<std::pair<std::string, char>> result(merged.begin(), merged.end());
    
    // Sort by name first, then by type (though names are unique after merging).
    std::sort(result.begin(), result.end());
    
    return result;
}

#include <cassert>
#include <vector>
#include <string>
#include <utility>
#include <stdexcept>

// The solution function is assumed to be defined above.

int main() {
    // Case 1: Single namespace block.
    std::vector<std::vector<std::pair<std::string, char>>> test1 = {
        {{"x", 'i'}, {"y", 'f'}}
    };
    auto res1 = mergeNamespaces(test1);
    assert(res1.size() == 2);
    assert(res1[0] == std::make_pair(std::string("x"), 'i'));
    assert(res1[1] == std::make_pair(std::string("y"), 'f'));

    // Case 2: Multiple blocks, later overrides earlier.
    std::vector<std::vector<std::pair<std::string, char>>> test2 = {
        {{"x", 'i'}, {"y", 'f'}},
        {{"y", 'd'}, {"z", 'i'}}
    };
    auto res2 = mergeNamespaces(test2);
    assert(res2.size() == 3);
    assert(res2[0] == std::make_pair(std::string("x"), 'i'));
    assert(res2[1] == std::make_pair(std::string("y"), 'd'));  // overridden
    assert(res2[2] == std::make_pair(std::string("z"), 'i'));

    // Case 3: Empty blocks and empty input.
    std::vector<std::vector<std::pair<std::string, char>>> test3 = {
        {},
        {{"a", 'i'}},
        {}
    };
    auto res3 = mergeNamespaces(test3);
    assert(res3.size() == 1);
    assert(res3[0] == std::make_pair(std::string("a"), 'i'));

    // Case 4: Duplicate name across blocks is fine.
    std::vector<std::vector<std::pair<std::string, char>>> test4 = {
        {{"var", 'i'}},
        {{"var", 'd'}}
    };
    auto res4 = mergeNamespaces(test4);
    assert(res4.size() == 1);
    assert(res4[0] == std::make_pair(std::string("var"), 'd'));

    // Case 5: Duplicate within same block throws.
    std::vector<std::vector<std::pair<std::string, char>>> test5 = {
        {{"x", 'i'}, {"x", 'f'}}
    };
    bool threw = false;
    try {
        mergeNamespaces(test5);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Case 6: Empty vector returns empty result.
    std::vector<std::vector<std::pair<std::string, char>>> test6 = {};
    auto res6 = mergeNamespaces(test6);
    assert(res6.empty());
}

// The solution processes the input vector of namespace blocks in order, maintaining a map from variable name to type (and also track the block index for error detection). For each block, iterate through its pairs: if a name was already declared in the *same* block, throw an `std::invalid_argument`. Otherwise, update the map entry with the new type (overwriting any previous declaration from earlier blocks, since later blocks take precedence). After processing all blocks, convert the map to a vector of pairs, then sort it by pair's first element (string) as primary key and second element (char) as secondary key. Edge cases: empty input vector → return empty vector; empty inner blocks → skip; duplicate names within a block → throw. Time complexity: O(B + N log N), where B is total number of declarations across all blocks and N is number of unique names. Space complexity: O(N) for the map.
