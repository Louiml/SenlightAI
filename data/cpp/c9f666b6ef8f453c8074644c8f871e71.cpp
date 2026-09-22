// Write a C++ function `bool isChronologicalFamilyTree(const std::vector<std::vector<int>>& tree)` that takes a tree represented as an adjacency list where each node stores a date encoded as three consecutive integers (day, month, year) at the start of its list, followed by the indices of its children. The tree is rooted at node 0. The function returns `true` if the tree is "chronologically valid", meaning that for every parent node, all of its children have dates strictly greater than the parent's date, and this property holds recursively for all subtrees. The input is guaranteed to be a valid tree with at least one node, and node indices are in the range `[0, n-1]`. Dates are valid calendar dates (day 1-31, month 1-12, year normalized). Return `false` if any child's date is less than or equal to its parent's date, or if any recursive check fails.

// The solution performs a depth-first search (DFS) starting from the root (node 0). For each node, its date is extracted from the first three integers of its adjacency list. Then we compare each child's date to the parent's date: if any child's date is not strictly greater than the parent's (i.e., `child <= parent`), we return `false`. Otherwise, we recursively validate each child's subtree. The base case is a leaf node (no children) which is trivially valid. Important edge cases: a single-node tree is valid; dates with equal day/month/year between parent and child must return `false`; the recursion must not visit nodes twice (not needed since it's a tree, but we assume valid input). Time complexity is O(N) where N is the total number of nodes, since each node is visited once and we check all children edges. Space complexity is O(H) for the recursion stack, where H is the height of the tree (worst case O(N) for a skewed tree).

#include <vector>
#include <tuple>

// Compare two dates encoded as (day, month, year) triples.
// Returns true if date1 < date2, false otherwise.
bool dateLess(const std::vector<int>& date1, const std::vector<int>& date2) {
    // date1 and date2 must have at least 3 elements
    if (date1[2] != date2[2]) return date1[2] < date2[2];
    if (date1[1] != date2[1]) return date1[1] < date2[1];
    return date1[0] < date2[0];
}

// Recursive helper that checks the subtree rooted at 'node'.
bool validateSubtree(const std::vector<std::vector<int>>& tree, int node) {
    const auto& nodeData = tree[node];
    // The date is the first three elements
    std::vector<int> parentDate = {nodeData[0], nodeData[1], nodeData[2]};
    
    // Check all children
    for (size_t i = 3; i < nodeData.size(); ++i) {
        int child = nodeData[i];
        const auto& childData = tree[child];
        std::vector<int> childDate = {childData[0], childData[1], childData[2]};
        // Child must be strictly greater than parent
        if (!dateLess(parentDate, childDate)) {
            return false;
        }
        // Recurse into child's subtree
        if (!validateSubtree(tree, child)) {
            return false;
        }
    }
    return true;
}

// Main function: returns true if the tree rooted at 0 is chronologically valid.
bool isChronologicalFamilyTree(const std::vector<std::vector<int>>& tree) {
    if (tree.empty()) return true; // edge case, but not expected
    return validateSubtree(tree, 0);
}

#include <cassert>
#include <vector>

// Function under test is declared above; include the solution code before this.

int main() {
    // Single node (root only)
    std::vector<std::vector<int>> tree1 = {{1, 1, 2000}};
    assert(isChronologicalFamilyTree(tree1) == true);

    // Valid simple two-level tree
    // Root: 2000-01-01, children: 2001-02-03 (node1), 2005-12-31 (node2)
    std::vector<std::vector<int>> tree2 = {
        {1, 1, 2000, 1, 2},   // node 0
        {3, 2, 2001},         // node 1
        {31, 12, 2005}        // node 2
    };
    assert(isChronologicalFamilyTree(tree2) == true);

    // Invalid: child has same date as parent
    std::vector<std::vector<int>> tree3 = {
        {1, 1, 2000, 1},
        {1, 1, 2000}
    };
    assert(isChronologicalFamilyTree(tree3) == false);

    // Invalid: child has earlier date than parent
    std::vector<std::vector<int>> tree4 = {
        {5, 5, 2010, 1},
        {4, 5, 2010}
    };
    assert(isChronologicalFamilyTree(tree4) == false);

    // Valid deeper tree with multiple levels
    std::vector<std::vector<int>> tree5 = {
        {1, 1, 2000, 1, 2},   // node 0
        {2, 2, 2001, 3},      // node 1
        {3, 3, 2002},         // node 2
        {4, 4, 2005}          // node 3
    };
    assert(isChronologicalFamilyTree(tree5) == true);

    // Invalid at deeper level: parent 2001, child 2000
    std::vector<std::vector<int>> tree6 = {
        {1, 1, 2000, 1},      // node 0
        {2, 2, 2001, 2},      // node 1
        {1, 1, 2000}          // node 2
    };
    assert(isChronologicalFamilyTree(tree6) == false);

    // Valid: leaves have no children (only date information)
    std::vector<std::vector<int>> tree7 = {
        {10, 10, 1990, 1, 2},
        {11, 11, 1991},
        {12, 12, 1992}
    };
    assert(isChronologicalFamilyTree(tree7) == true);

    // Empty tree (edge case, but should not crash)
    std::vector<std::vector<int>> tree8 = {};
    assert(isChronologicalFamilyTree(tree8) == true);

    // Complex valid case with multiple branches
    std::vector<std::vector<int>> tree9 = {
        {1, 1, 1900, 1, 2},   // node 0
        {2, 2, 1920, 3, 4},   // node 1
        {3, 3, 1930},         // node 2
        {4, 4, 1950},         // node 3
        {5, 5, 1960, 5},      // node 4
        {6, 6, 1980}          // node 5
    };
    assert(isChronologicalFamilyTree(tree9) == true);

    // Complex invalid: one branch violates
    std::vector<std::vector<int>> tree10 = {
        {1, 1, 1900, 1, 2},   // node 0
        {2, 2, 1920, 3, 4},   // node 1
        {3, 3, 1930},         // node 2
        {4, 4, 1950},         // node 3
        {10, 5, 1960, 5},     // node 4
        {6, 6, 1940}          // node 5 (child has earlier date than node 4)
    };
    assert(isChronologicalFamilyTree(tree10) == false);

    return 0;
}
