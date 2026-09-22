Write a C++ function `assignNearest` that takes a list of existing leaf nodes (each with a unique numeric ID and a numeric priority) and a sequence of new leaf nodes (each with a unique numeric ID and a priority). For each new node, the function must output the ID of the existing leaf node (in the current set) whose priority is closest to the new node's priority, and then insert the new node into the set (so it becomes available for subsequent queries). If there is a tie between two existing nodes (one with priority lower, one with higher), choose the one with the lower priority. The initial set always contains a single node with ID 1 and priority 1,000,000,000. The function should return a vector of integers representing, in order, the chosen existing ID for each new node. Priorities can be any integers, including negatives, and all IDs are positive integers. The order of insertion into the set is the order of the input sequence.

#include <cassert>
#include <vector>
#include <utility>

// Forward declaration
std::vector<int> assignNearest(const std::vector<std::pair<int,int>>&);

int main() {
    // Test 1: Basic case from original snippet behavior
    // Initial: (1e9, 1). Add (2, 5) -> only successor? Actually 5 < 1e9, so predecessor is sentinel, no successor -> choose sentinel ID 1.
    // Then add (3, 7) -> neighbors: 5 (ID 2) and 1e9 (ID 1) -> diff 2 vs massive -> choose ID 2.
    assert((assignNearest({{2,5},{3,7}}) == std::vector<int>({1,2})));

    // Test 2: Tie should pick lower priority (predecessor)
    // Initial sentinel 1e9. Add (10, 100) -> only predecessor sentinel -> choice 1.
    // Then add (20, 200) -> neighbors 100 (ID 10) and 1e9 (ID 1) -> diff 100 vs 800k -> choose 10.
    // Then add (30, 150) -> neighbors 100 (ID 10) and 200 (ID 20) -> diff 50 vs 50 -> tie -> choose lower priority 100 (ID 10).
    assert((assignNearest({{10,100},{20,200},{30,150}}) == std::vector<int>({1,10,10})));

    // Test 3: Negative priorities and exact match
    // Start sentinel 1e9. Add A (-5, 1) -> only predecessor sentinel -> choose 1.
    // Add B (-5, 2) -> same priority as A, duplicates allowed; neighbors: A (priority -5) and sentinel (1e9). diff 0 vs large -> choose A (ID 1, priority -5).
    assert((assignNearest({{1,-5},{2,-5}}) == std::vector<int>({1,1})));

    // Test 4: Only one new node
    // Add (7, 500) -> only predecessor sentinel -> choose 1.
    assert((assignNearest({{7,500}}) == std::vector<int>({1})));

    // Test 5: Separate chains to test both sides
    // Start sentinel 1e9. Add X (1, 0) -> predecessor sentinel -> choose 1.
    // Add Y (2, 2000000000) -> now set has 0, 1e9, 2e9. New node 2e9 has predecessor 1e9 and no successor (since it's largest) -> choose predecessor ID 1? Actually predecessor is sentinel ID 1. So choose 1.
    // Then add Z (3, 1000000000) -> exact match with sentinel priority? Using multimap, sentinel priority 1e9, new node priority 1e9, insert after sentinel (since multimap preserves insertion order for equal keys). Neighbors: predecessor sentinel (ID 1), successor 2e9 (ID 2). diff 0 vs 1e9 -> choose predecessor ID 1.
    assert((assignNearest({{1,0},{2,2000000000},{3,1000000000}}) == std::vector<int>({1,1,1})));

    // Test 6: Multiple ties and higher/lower picks
    // Start sentinel 1e9. Add A (1, 10), B (2, 20), C (3, 15). 
    // For A: predecessor sentinel -> choose 1.
    // For B: neighbors 10 (ID1) and 1e9 (ID2? no ID2 is sentinel? Wait sentinel ID is 1, but we have two IDs? Actually sentinel ID is 1, but we also have node ID 1? That's a conflict. To avoid confusion, use distinct IDs.)
    // Let's redo: sentinel has ID 100. Add A (1,10), B (2,20), C (3,15).
    // For A: predecessor sentinel -> choose ID 100.
    // For B: neighbors 10 (ID1) and 1e9 (ID100) -> diff 10 vs ~1e9 -> choose ID1.
    // For C (15): neighbors 10 (ID1) and 20 (ID2) -> diff 5 vs 5 -> tie -> lower priority 10 (ID1) -> choose ID1.
    assert((assignNearest({{1,10},{2,20},{3,15}}) == std::vector<int>({100,1,1})));

    return 0;
}

#include <map>
#include <vector>
#include <utility>

// Assign each new leaf to the existing leaf with closest priority.
// Input: initial_node (priority, ID) is given inside function as (1e9, 1).
// parameters: newNodes is a vector of pairs (ID, priority) in order of insertion.
// Returns vector of chosen existing IDs for each new node.
std::vector<int> assignNearest(const std::vector<std::pair<int,int>>& newNodes) {
    std::multimap<int,int> lst; // key: priority, value: ID
    lst.insert({1000000000, 1}); // sentinel node

    std::vector<int> result;
    result.reserve(newNodes.size());

    for (const auto& node : newNodes) {
        int id = node.first;
        int p = node.second;

        // Insert the new node to get its position
        auto it = lst.insert({p, id});

        int chosenID;
        if (it != lst.begin() && it != std::prev(lst.end())) {
            // Has both predecessor and successor
            auto pred = std::prev(it);
            auto succ = std::next(it);
            int diffPred = std::abs(pred->first - p);
            int diffSucc = std::abs(succ->first - p);
            if (diffPred <= diffSucc) {
                chosenID = pred->second;
            } else {
                chosenID = succ->second;
            }
        } else if (it == lst.begin()) {
            // No predecessor, only successor exists (but check for only one element)
            if (std::next(it) == lst.end()) {
                chosenID = it->second; // Only sentinel exists before insertion? Actually not possible because we insert after sentinel, but safe
            } else {
                chosenID = std::next(it)->second;
            }
        } else {
            // it == prev(lst.end()) and not begin, so only predecessor exists
            chosenID = std::prev(it)->second;
        }

        result.push_back(chosenID);
        // The new node is already in lst, so don't erase
    }

    return result;
}

// The problem is directly modeled by the given code: we maintain an ordered associative container (e.g., `std::multimap<int,int>` where key is priority, value is ID). We start with one sentinel node `(1,000,000,000, 1)`. For each new node, we insert it temporarily to get an iterator to its position. Then we look at its immediate predecessor and successor in the map (if they exist). The candidate with the smallest absolute difference from the new priority is chosen; on a tie, the predecessor (lower priority) wins. If only one side exists, that side is chosen. After outputting the choice, the new node stays inserted for future queries. Edge cases: a new node may have the same priority as an existing one—since we use `multimap`, duplicates are allowed; the iterator positions are still valid. The first and last elements handle boundary conditions. Time complexity is O(n log n) overall because each insertion and neighbor lookup is O(log n) in a balanced BST (like `std::multimap`). Space complexity is O(n) for storing all nodes.
