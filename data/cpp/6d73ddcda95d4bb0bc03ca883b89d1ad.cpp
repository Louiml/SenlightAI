Write a C++ function `simplex_spouses` that, given a `std::map<int, std::vector<int>>` representing a simplicial complex—where each key is a simplex ID and each value is the list of child simplex IDs (faces) that it owns—returns a `std::map<int, std::vector<int>>` mapping each simplex ID to the sorted list of its "spouses". A spouse of simplex A is any other simplex B that shares at least one child (face) with A, but is not A itself. Duplicate spouse IDs must appear only once per simplex, and the output lists must be sorted in ascending order. The input is guaranteed non-empty, and every simplex listed as a child must exist as a key in the map. For example, if simplex 0 has children {1,2} and simplex 3 has children {2,4}, then simplex 0 has spouse 3, and simplex 3 has spouse 0, because they share child 2. Self-spousal is excluded (a simplex cannot be its own spouse even if it somehow lists itself as a child). The function must be `const`-correct and not modify the input. If a simplex has no spouses, its output list must be empty.
#include <cassert>
#include <map>
#include <vector>

int main() {
    // Basic shared child
    {
        std::map<int, std::vector<int>> input = {{0, {1,2}}, {3, {2,4}}};
        auto out = simplex_spouses(input);
        assert(out[0] == std::vector<int>({3}));
        assert(out[3] == std::vector<int>({0}));
    }

    // No children -> no spouses
    {
        std::map<int, std::vector<int>> input = {{0, {}}, {1, {0}}};
        auto out = simplex_spouses(input);
        assert(out[0].empty());
        assert(out[1] == std::vector<int>({0})); // 0 is child of 1, but 0 has no children so no spouse for 0
    }

    // Multiple shared children, dedup
    {
        std::map<int, std::vector<int>> input = {{0, {5,5,6}}, {1, {5,6}}, {2, {6}}};
        auto out = simplex_spouses(input);
        assert(out[0] == std::vector<int>({1,2})); // shares 5 with 1, 6 with both 1 and 2
        assert(out[1] == std::vector<int>({0,2}));
        assert(out[2] == std::vector<int>({0,1}));
    }

    // Self in own child list is excluded
    {
        std::map<int, std::vector<int>> input = {{0, {0,1}}, {1, {1,2}}};
        auto out = simplex_spouses(input);
        assert(out[0] == std::vector<int>({1})); // not 0
        assert(out[1] == std::vector<int>({0})); // not 1
    }

    // Triangle all share edges
    {
        std::map<int, std::vector<int>> input = {{0, {1,2}}, {3, {1,4}}, {5, {2,4}}};
        auto out = simplex_spouses(input);
        assert(out[0] == std::vector<int>({3,5}));
        assert(out[3] == std::vector<int>({0,5}));
        assert(out[5] == std::vector<int>({0,3}));
    }

    // Isolated simplex with no children and none referencing it
    {
        std::map<int, std::vector<int>> input = {{7, {}}};
        auto out = simplex_spouses(input);
        assert(out[7].empty());
    }

    // Single simplex with child referencing another existing simplex
    {
        std::map<int, std::vector<int>> input = {{9, {8}}, {10, {8,8}}};
        auto out = simplex_spouses(input);
        assert(out[9] == std::vector<int>({10}));
        assert(out[10] == std::vector<int>({9}));
    }
}
#include <map>
#include <vector>
#include <set>
#include <algorithm>

/**
 * For each simplex ID, return sorted unique spouse IDs.
 * A spouse is another simplex sharing at least one child.
 * Excludes self.
 */
std::map<int, std::vector<int>> simplex_spouses(const std::map<int, std::vector<int>>& simplices) {
    // Build inverse: child -> set of parents
    std::map<int, std::set<int>> child_to_parents;
    for (const auto& entry : simplices) {
        int parent = entry.first;
        for (int child : entry.second) {
            child_to_parents[child].insert(parent);
        }
    }

    std::map<int, std::vector<int>> result;
    for (const auto& entry : simplices) {
        int a = entry.first;
        std::set<int> spouses;
        for (int child : entry.second) {
            const auto it = child_to_parents.find(child);
            if (it != child_to_parents.end()) {
                for (int other_parent : it->second) {
                    if (other_parent != a) {
                        spouses.insert(other_parent);
                    }
                }
            }
        }
        result[a] = std::vector<int>(spouses.begin(), spouses.end());
    }
    return result;
}
// The solution builds an inverse mapping from each child simplex ID to the set of parent simplex IDs that own it, by iterating over the input map. For each simplex `parent`, for each child `childID` in its list, we record `parent` in a bucket keyed by `childID`. After building this inverse map, we reconstruct the spouse relationships: for every simplex `A`, we collect all parents that appear in any child bucket corresponding to `A`’s own children. We insert those parent IDs (excluding `A` itself) into a `std::set<int>` to automatically deduplicate and sort. Then we assign the set’s contents to the output vector. Edge cases include: a simplex with no children (output empty), a child appearing multiple times in a parent’s list (ignored due to set), a child shared by three or more parents (each pair becomes spouses), and the same simplex appearing in its own child list (excluded by the check `parent != A`). Time complexity is O(T + S log S) where T is total number of child entries and S is total number of spouse pairs (with log factor from set insertion per spouse). Space complexity is O(T + total spouses) for the inverse map and output.
