Write a C++ function named `extractHypreParameterMap` that takes a `std::map<std::string, int>` representing Hypre solver parameter names mapped to their numeric codes (as suggested by `Ifpack2_HypreParameterMap.hpp` in the snippet) and a non-empty `std::vector<std::string>` of parameter names. The function must return a `std::map<std::string, int>` containing only those entries from the input map whose keys appear in the vector, preserving the original numeric codes. If a requested parameter name is not found in the map, skip it silently. The function must be `const`-correct (accepting a `const std::map&` and `const std::vector&`), and it must work correctly for duplicate names in the vector (no duplication in output) and for empty result sets (return an empty map). Use `std::set` or `std::unordered_set` internally to deduplicate the requested names for efficiency.

The core algorithm is straightforward: iterate over the vector of requested parameter names, check if each name exists in the input map, and if so, insert or assign that key-value pair into the result map. Since the input map is sorted by key (`std::map` guarantees ordered iteration), and the result map is also a `std::map`, the output will be automatically sorted in ascending alphabetical order of parameter names. Duplicate names in the vector are harmless: after the first occurrence, subsequent occurrences will find the key already in the result map, and `operator[]` or `insert_or_assign` will simply overwrite the same value. To avoid unnecessary lookups and to handle duplicates cleanly, one can use an `std::unordered_set<std::string>` to track which names have already been added; but since the input size is typically small, this optimization is optional. However, the task asks to use a set for deduplication. Edge cases include: an empty vector (return empty map), vector with names not in the map (skip them), and vector with duplicated names (only include once). Time complexity is O(N log M + N log R) where N is vector size, M is map size (lookup), and R is result size (insertion), but with an unordered_set it becomes O(N) average for dedup and O(N log M) for lookups. Space complexity is O(N) for the set and O(R) for the result.

#include <map>
#include <string>
#include <vector>
#include <unordered_set>

// Extracts entries from the source map whose keys are present in the requested names vector.
// Returns a new map with matching keys and their original integer codes.
// Does not mutate the input; returns an empty map if no matches or no requests.
std::map<std::string, int> extractHypreParameterMap(
    const std::map<std::string, int>& parameterMap,
    const std::vector<std::string>& requestedNames) {
    
    std::map<std::string, int> result;
    std::unordered_set<std::string> seen;  // Deduplicate requested names
    
    for (const auto& name : requestedNames) {
        // Skip duplicates to avoid redundant lookups and insertions
        if (seen.find(name) != seen.end()) {
            continue;
        }
        seen.insert(name);
        
        // Look up in the source map; if found, copy to result
        auto it = parameterMap.find(name);
        if (it != parameterMap.end()) {
            result.insert(*it);
        }
    }
    
    return result;
}

#include <cassert>
#include <map>
#include <string>
#include <vector>

// Function declaration from solution (assumed already included)
std::map<std::string, int> extractHypreParameterMap(
    const std::map<std::string, int>& parameterMap,
    const std::vector<std::string>& requestedNames);

int main() {
    // Base map with sample Hypre parameters (codes are illustrative)
    const std::map<std::string, int> base = {
        {"tolerance", 100},
        {"max_iter", 200},
        {"print_level", 300},
        {"precond_type", 400},
        {"solver_type", 500}
    };

    // Test 1: All requested names exist; output should be sorted alphabetically.
    std::vector<std::string> all_names = {"solver_type", "tolerance", "max_iter"};
    auto result1 = extractHypreParameterMap(base, all_names);
    std::map<std::string, int> expected1 = {
        {"max_iter", 200},
        {"solver_type", 500},
        {"tolerance", 100}
    };
    assert(result1 == expected1);

    // Test 2: Some names missing; missing ones are skipped.
    std::vector<std::string> some_missing = {"tolerance", "non_existent", "print_level"};
    auto result2 = extractHypreParameterMap(base, some_missing);
    std::map<std::string, int> expected2 = {
        {"print_level", 300},
        {"tolerance", 100}
    };
    assert(result2 == expected2);

    // Test 3: Empty request vector returns empty map.
    std::vector<std::string> empty_names;
    auto result3 = extractHypreParameterMap(base, empty_names);
    assert(result3.empty());

    // Test 4: All names missing returns empty map.
    std::vector<std::string> all_missing = {"alpha", "beta"};
    auto result4 = extractHypreParameterMap(base, all_missing);
    assert(result4.empty());

    // Test 5: Duplicate names in request are deduplicated.
    std::vector<std::string> duplicates = {"max_iter", "max_iter", "tolerance", "tolerance"};
    auto result5 = extractHypreParameterMap(base, duplicates);
    std::map<std::string, int> expected5 = {
        {"max_iter", 200},
        {"tolerance", 100}
    };
    assert(result5 == expected5);

    // Test 6: Single valid name.
    std::vector<std::string> single = {"precond_type"};
    auto result6 = extractHypreParameterMap(base, single);
    std::map<std::string, int> expected6 = {{"precond_type", 400}};
    assert(result6 == expected6);

    // Test 7: Input map with zero entries, non-empty request.
    const std::map<std::string, int> empty_map;
    std::vector<std::string> req = {"a"};
    auto result7 = extractHypreParameterMap(empty_map, req);
    assert(result7.empty());

    // Test 8: Case sensitivity — exact match only.
    std::vector<std::string> case_mismatch = {"TOLERANCE", "Tolerance", "tolerance"};
    auto result8 = extractHypreParameterMap(base, case_mismatch);
    std::map<std::string, int> expected8 = {{"tolerance", 100}};
    assert(result8 == expected8);

    return 0;
}
