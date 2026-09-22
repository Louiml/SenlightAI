Given a list of `regions` where each inner vector represents a region hierarchy (the first element is the parent region, and the remaining elements are its direct sub-regions), and two region names `a` and `b`, write a C++ function `smallestCommonRegion` that returns the smallest region that contains both `a` and `b` (i.e., their lowest common ancestor in the region tree). The region names are non-empty strings, and every region except the global root appears exactly once as a child somewhere in the input. You may assume that `a` and `b` are always valid region names existing in the tree, and that the root region is always present as the first element of exactly one inner vector. The function should return the region name of the lowest common ancestor. For example, if regions are `[["Earth","North America","South America"],["North America","USA","Canada"],["USA","California","Texas"]]`, then `smallestCommonRegion("California","Texas")` returns `"USA"`, and `smallestCommonRegion("California","Canada")` returns `"North America"`.
// The problem reduces to finding the lowest common ancestor (LCA) in a tree where each node (region) has a unique parent. The given data represents a rooted tree with the global root as the only node without a parent. The main algorithm uses a hash map `parent` to store each region’s direct parent. Build this map by iterating through all inner vectors: for each inner vector, the first element is the parent and every subsequent element is a child; set `parent[child] = parentRegion`. After constructing the map, we compute the path from `a` up to the root by repeatedly following parent pointers and storing each visited node in a vector `pathA`. Similarly, compute `pathB` from `b`. Both paths end at the root. Now, compare the two paths from the end (root side) backwards: while the last elements of both paths are equal, pop them and record the last common node. When they diverge, the recorded node is the LCA. Edge cases: if `a` and `b` are the same, the while loop will pop all elements and the recorded node will be `a` itself. If one is an ancestor of the other, the common path includes the ancestor, and the algorithm correctly returns it. Time complexity is O(N + H) where N is total number of regions (to build the map) and H is the depth of the tree (for path construction). Space complexity is O(N) for the map plus O(H) for the two path vectors.
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

// Returns the lowest common ancestor region of two given regions in a region hierarchy.
std::string smallestCommonRegion(const std::vector<std::vector<std::string>>& regions,
                                 const std::string& a, const std::string& b) {
    // Build parent map: for each region, store its direct parent.
    std::unordered_map<std::string, std::string> parent;
    for (const auto& group : regions) {
        for (size_t i = 1; i < group.size(); ++i) {
            parent[group[i]] = group[0];
        }
    }

    // Build path from a up to root.
    std::vector<std::string> pathA;
    std::string curA = a;
    while (!curA.empty()) {
        pathA.push_back(curA);
        curA = parent[curA]; // root's parent will be "" due to default construction.
    }

    // Build path from b up to root.
    std::vector<std::string> pathB;
    std::string curB = b;
    while (!curB.empty()) {
        pathB.push_back(curB);
        curB = parent[curB];
    }

    // Compare from the end (closest to root) and find last common node.
    std::string result = pathA.back();
    while (!pathA.empty() && !pathB.empty() && pathA.back() == pathB.back()) {
        result = pathA.back();
        pathA.pop_back();
        pathB.pop_back();
    }
    return result;
}
#include <cassert>
#include <string>
#include <vector>

// The solution function is assumed to be declared above.
// No need to re-include headers here, but we do for completeness.
#include <unordered_map>

int main() {
    std::vector<std::vector<std::string>> regions1 = {
        {"Earth", "North America", "South America"},
        {"North America", "USA", "Canada"},
        {"USA", "California", "Texas"}
    };

    assert(smallestCommonRegion(regions1, "California", "Texas") == "USA");
    assert(smallestCommonRegion(regions1, "California", "Canada") == "North America");
    assert(smallestCommonRegion(regions1, "California", "California") == "California");
    assert(smallestCommonRegion(regions1, "USA", "Canada") == "North America");
    assert(smallestCommonRegion(regions1, "Earth", "Texas") == "Earth");

    // Single region tree
    std::vector<std::vector<std::string>> regions2 = {{"Root"}};
    assert(smallestCommonRegion(regions2, "Root", "Root") == "Root");

    // Two children under root
    std::vector<std::vector<std::string>> regions3 = {
        {"R", "A", "B"},
        {"A", "A1"},
        {"B", "B1"}
    };
    assert(smallestCommonRegion(regions3, "A1", "B1") == "R");
    assert(smallestCommonRegion(regions3, "A", "A1") == "A");

    // Deep tree with multiple levels
    std::vector<std::vector<std::string>> regions4 = {
        {"Z", "Y", "X"},
        {"Y", "W"},
        {"W", "V"},
        {"V", "U"}
    };
    assert(smallestCommonRegion(regions4, "U", "X") == "Z");
    assert(smallestCommonRegion(regions4, "U", "V") == "V");

    return 0;
}
