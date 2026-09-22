// Write a C++ function named `isUnlinkedTaxa` that takes a vector of partitions, where each partition is a vector of strings representing unique sequence names within that partition. The function should return `true` if every sequence name across all partitions is unique (i.e., no sequence name appears in more than one partition), and `false` otherwise. A partition is represented as `std::vector<std::string>`, and the input is `std::vector<std::vector<std::string>>`. The function must be `const`-correct and should not modify the input. Handle empty partitions gracefully (they contribute no names). The time complexity should be O(total number of names), and space complexity O(number of distinct names).

// The core task is to detect whether any sequence name appears in two or more partitions. The simplest approach is to insert every name from every partition into an unordered set (or map) and check for duplicates as we go. If we attempt to insert a name that already exists in the set, we have found a collision, meaning at least one name is shared between partitions, so we return `false`. If we finish processing all names without collision, all names are unique, so we return `true`. Edge cases: an empty input vector (no partitions) should trivially return `true` because there are no names to collide. A partition with duplicate names internally? The problem statement says partitions contain "unique sequence names within that partition," so we can assume each partition has no internal duplicates. However, if internal duplicates were allowed, our collision detection would still correctly return `false` because the second occurrence of the same name within the same partition would be detected as a duplicate. The algorithm runs in O(N) time and O(N) space where N is the total number of names across all partitions, because each name is processed once and stored in the set once.

#include <vector>
#include <string>
#include <unordered_set>

// Returns true if no sequence name appears in more than one partition.
// Each partition is a vector of sequence names; all names within a partition are assumed unique.
bool isUnlinkedTaxa(const std::vector<std::vector<std::string>>& partitions) {
    std::unordered_set<std::string> seen;
    for (const auto& partition : partitions) {
        for (const auto& name : partition) {
            // If name already exists in 'seen', it's shared across partitions.
            if (seen.find(name) != seen.end()) {
                return false;
            }
            seen.insert(name);
        }
    }
    return true;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or via header).
#include "solution.h" // Adjust include as needed.

int main() {
    // Case 1: Empty input -> trivially unlinked.
    std::vector<std::vector<std::string>> empty;
    assert(isUnlinkedTaxa(empty) == true);

    // Case 2: Single partition with unique names.
    std::vector<std::vector<std::string>> single = {{"a", "b", "c"}};
    assert(isUnlinkedTaxa(single) == true);

    // Case 3: Multiple partitions with all unique names.
    std::vector<std::vector<std::string>> unique = {{"a", "b"}, {"c", "d"}, {"e"}};
    assert(isUnlinkedTaxa(unique) == true);

    // Case 4: Overlap between two partitions.
    std::vector<std::vector<std::string>> overlap = {{"a", "b"}, {"b", "c"}};
    assert(isUnlinkedTaxa(overlap) == false);

    // Case 5: Overlap with empty partition in between.
    std::vector<std::vector<std::string>> overlap_with_empty = {{"x"}, {}, {"x"}};
    assert(isUnlinkedTaxa(overlap_with_empty) == false);

    // Case 6: Many partitions, no overlap.
    std::vector<std::vector<std::string>> many = {{"s1"}, {"s2", "s3"}, {"s4", "s5", "s6"}};
    assert(isUnlinkedTaxa(many) == true);

    // Case 7: Same name appears three times across partitions.
    std::vector<std::vector<std::string>> triple = {{"z"}, {"z"}, {"z"}};
    assert(isUnlinkedTaxa(triple) == false);

    // Case 8: Partition with duplicate name internally (though spec says unique, still test).
    std::vector<std::vector<std::string>> internal_dup = {{"a", "a"}};
    assert(isUnlinkedTaxa(internal_dup) == false);

    // Case 9: Empty partitions mixed with non-empty, no overlap.
    std::vector<std::vector<std::string>> mixed_empty = {{}, {"a"}, {}, {"b"}};
    assert(isUnlinkedTaxa(mixed_empty) == true);

    // Case 10: Large number of unique names.
    std::vector<std::vector<std::string>> large;
    for (int i = 0; i < 100; ++i) {
        large.push_back({std::to_string(i)});
    }
    assert(isUnlinkedTaxa(large) == true);
}
