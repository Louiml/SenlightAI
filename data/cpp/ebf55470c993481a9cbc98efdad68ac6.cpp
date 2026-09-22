Write a C++ function `sumMiddlePages` that takes a vector of ordering rules (pairs of page numbers where the first must appear before the second) and a vector of updates (each update is a vector of page numbers). The function returns a `std::pair<unsigned int, unsigned int>` where the first value is the sum of the middle page numbers from all updates that are already in a correct order according to the rules, and the second value is the sum of the middle page numbers of the incorrectly ordered updates after reordering them into a valid order. A valid order is one where for every rule (A,B), A appears before B in the update. If a rule involves a page not present in the update, it can be ignored. The input rules may contain duplicate pairs and page numbers are positive integers.
// The solution proceeds in three main steps. First, build a directed graph from the rules: each rule (A,B) is stored as a set of successors for A (i.e., pages that must come after A). To make lookup efficient, use `std::map<uint32_t, std::set<uint32_t>>` so that `rules[A]` gives all pages that must appear after A. Second, for each update, check if it is correctly ordered: iterate through consecutive pairs in the update and verify that for every adjacent pair (X,Y), either there is no rule requiring Y to come before X, or if such a rule exists, then X must be allowed to come before Y (i.e., the rule (X,Y) exists). More precisely, for any adjacent pair (X,Y), if there is a rule (Y,X) that would be violated, the update is incorrect. If no such violation is found, the update is correct. Third, for incorrectly ordered updates, reorder them using a topological sort via depth-first search (DFS). Since the rules only contain pages present in the update, we restrict the graph to the nodes in the update. Perform DFS starting from each update page that hasn't been visited, adding pages to a list in post-order, then reverse the list. Because the problem guarantees a valid ordering exists (as in Advent of Code day 5), the algorithm is safe. Edge cases: updates of odd length always have a middle element; rules with pages not in the update are ignored; duplicate rules are handled by the set. Time complexity: building the graph takes O(R) where R is number of rules; checking each update takes O(U * L) where L is the length of the update, because for each adjacent pair we do a constant-time lookup in the map; reordering with DFS visits each node and edge once, so O(V+E) per update, but since each update is small, overall it's linear in the total input size. Space complexity is O(R + total update size) for the graph and temporary structures.
#include <cstdint>
#include <map>
#include <set>
#include <vector>
#include <utility>

// Sum middle pages of correctly and incorrectly ordered updates.
std::pair<std::uint32_t, std::uint32_t> sumMiddlePages(
    const std::vector<std::pair<std::uint32_t, std::uint32_t>>& rules,
    const std::vector<std::vector<std::uint32_t>>& updates) {
    
    // Build a map from page -> set of pages that must come after it.
    std::map<std::uint32_t, std::set<std::uint32_t>> graph;
    for (const auto& rule : rules) {
        graph[rule.first].insert(rule.second);
        // Ensure the key exists even if it has no outgoing edges.
        graph[rule.second];
    }

    // Helper lambda to check if an update is correctly ordered.
    auto is_ordered = [&graph](const std::vector<std::uint32_t>& update) {
        for (std::size_t i = 0; i + 1 < update.size(); ++i) {
            auto it = graph.find(update[i + 1]);
            if (it != graph.end() && it->second.count(update[i]) > 0) {
                return false; // Rule (update[i+1], update[i]) violated.
            }
        }
        return true;
    };

    // DFS helper to produce a topological order.
    std::vector<std::uint32_t> order;
    std::set<std::uint32_t> visited;
    std::function<void(std::uint32_t)> dfs = [&](std::uint32_t node) {
        visited.insert(node);
        auto it = graph.find(node);
        if (it != graph.end()) {
            for (std::uint32_t neighbor : it->second) {
                if (visited.find(neighbor) == visited.end()) {
                    dfs(neighbor);
                }
            }
        }
        order.push_back(node);
    };

    std::uint32_t correct_sum = 0;
    std::uint32_t incorrect_sum = 0;

    for (const auto& update : updates) {
        if (is_ordered(update)) {
            correct_sum += update[update.size() / 2];
        } else {
            // Topological sort restricted to nodes in this update.
            order.clear();
            visited.clear();
            for (std::uint32_t page : update) {
                if (visited.find(page) == visited.end()) {
                    dfs(page);
                }
            }
            std::reverse(order.begin(), order.end()); // Correct order.
            incorrect_sum += order[order.size() / 2];
        }
    }

    return {correct_sum, incorrect_sum};
}
#include <cassert>
#include <vector>
#include <utility>
#include <cstdint>

// The solution function is declared above (assumed included).

int main() {
    using Rules = std::vector<std::pair<std::uint32_t, std::uint32_t>>;
    using Updates = std::vector<std::vector<std::uint32_t>>;

    // Sample from problem statement (Advent of Code day 5).
    Rules rules = {
        {47, 53}, {47, 13}, {47, 61}, {47, 29},
        {97, 13}, {97, 61}, {97, 47}, {97, 29}, {97, 53}, {97, 75},
        {75, 29}, {75, 53}, {75, 47}, {75, 61}, {75, 13},
        {61, 13}, {61, 53}, {61, 29},
        {29, 13},
        {53, 29}, {53, 13}
    };
    Updates updates = {
        {75, 47, 61, 53, 29},
        {97, 61, 53, 29, 13},
        {75, 29, 13},
        {75, 97, 47, 61, 53},
        {61, 13, 29},
        {97, 13, 75, 29, 47}
    };
    auto result = sumMiddlePages(rules, updates);
    assert(result.first == 143);  // Correct updates: middle pages 61+53+29 = 143
    assert(result.second == 123); // Incorrect updates reordered: 47+29+47 = 123

    // Edge case: no rules, single-element updates.
    Rules empty_rules;
    Updates single = {{42}, {7}};
    result = sumMiddlePages(empty_rules, single);
    assert(result.first == 42 + 7);
    assert(result.second == 0);

    // Edge case: rules with pages not in update should not affect order.
    Rules extra_rules = {{1, 2}, {3, 4}};
    Updates upd = {{2, 1}};  // Violates rule (1,2)
    result = sumMiddlePages(extra_rules, upd);
    assert(result.first == 0);
    assert(result.second == 1);  // reordered becomes {1,2}, middle is 1

    // Duplicate rules should not break.
    Rules dup_rules = {{5, 9}, {5, 9}};
    Updates dup_upd = {{5, 9}};
    result = sumMiddlePages(dup_rules, dup_upd);
    assert(result.first == 9);   // 5 before 9 is valid, middle is 9? Wait size 2, middle index 1 = 9
    assert(result.second == 0);

    // Correct order with multiple rules.
    Rules multi = {{2, 3}, {1, 2}};
    Updates ok = {{1, 2, 3}};
    result = sumMiddlePages(multi, ok);
    assert(result.first == 2);
    assert(result.second == 0);

    // Incorrect order must be reordered.
    Updates bad = {{3, 2, 1}}; // violates (1,2) and (2,3)
    result = sumMiddlePages(multi, bad);
    assert(result.first == 0);
    assert(result.second == 2); // reordered to {1,2,3}, middle is 2

    // Test with an update that has a rule partially relevant.
    Rules partial = {{10, 20}};
    Updates part_upd = {{30, 10, 20}}; // rule (10,20) applies, 30 unrelated
    result = sumMiddlePages(partial, part_upd);
    assert(result.first == 10); // middle of {30,10,20} is 10 and correct
    assert(result.second == 0);

    return 0;
}
