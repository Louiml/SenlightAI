You are given a complete binary tree of infinite depth, where each node is identified by a positive integer (root = 1, left child = 2*i, right child = 2*i+1). Each node initially has a value of 0. Process a sequence of operations: an "add" operation takes a node index `v` and a positive integer `e`, and adds `e` to the value of every node on the path from `v` up to the root (inclusive). A "query" operation asks: starting from the root, you can move down the tree (left or right) one level at a time, but you may only move to a child if the child's current value is strictly less than the current accumulated "budget" (which starts at 0 and is non-decreasing). More precisely, define a recursive function `f(node, budget)` that returns the minimum additional budget needed at the node to ensure you can reach at least one leaf (infinite depth, but in practice the recursion will always terminate because values are finite). The rule: if `budget >= value(node)`, you can stop and need no more budget. Otherwise, you must move to both children; let `need_left = f(left, max(budget, value(node) - value(left)))` and similarly for right, then `f(node, budget) = 0.5 * (need_left + need_right)`. After each query, output the value of `f(1, 0)` with 8 decimal places. Note: the tree depth is effectively bounded by the number of distinct nodes that have ever been touched by an add operation, but the recursion may go arbitrarily deep if you keep adding to new nodes. Implement a class/function that processes a list of operations (as strings "add v e" or "query") and returns the results for each query as a vector of doubles.
// The core is to simulate the recursive function efficiently. Direct recursion would be exponential because each query could visit both children repeatedly. However, observe that the recursion only ever visits nodes that have been modified by an add, plus their ancestors. For an unmodified node, its value is 0, and the recursion will immediately terminate at that node because `budget >= 0` (since budget starts at 0 and never decreases). Therefore, we only need to store values for nodes that have been touched, and the recursion depth is bounded by the maximum depth of any touched node (which is at most the height of the tree, but in practice the number of distinct nodes is limited by the input). The key optimization: when we compute `f(node, budget)`, if `budget >= value(node)`, we return `budget` (not `value(node)`). This is important because the caller might pass a budget larger than the node's value, and the result must be that budget, not just the value. Also, for the children, we pass `max(ss, mp[i] - mp[l])` – that is, the budget for the child is either the current budget or the difference between the parent's value and the child's value, whichever is larger. This ensures that if the parent has a large value, we must "spend" that difference to enter the child. Since values are added to ancestors, each add operation updates O(depth) nodes (path to root). The number of distinct nodes ever updated is at most the total number of add operations times depth, but depth is at most the maximum node index's binary length, which is at most ~31 for typical int. So in practice, the map size is manageable. For each query, the recursion visits each touched node at most once per parent? Actually it can revisit nodes if the tree is not a DAG? No, it's a tree, so no cycles. However, the recursion can still be exponential if the tree is deep and both children are touched? But each node appears in at most one call per level? Actually, the recursion from root explores both subtrees, but within each subtree it may visit many nodes. In the worst case, if all touched nodes form a complete binary tree of depth d, the recursion visits all 2^d nodes? But wait, the function `dfs` for a node returns a value based on its children's results, and each child is called exactly once. So the total number of calls per query is exactly the number of touched nodes in the subtree (plus possibly some untouched nodes that are leaves of the touched subtree, but those terminate immediately). So it's O(N) per query where N is the number of touched nodes. Over Q queries, worst-case O(Q*N) could be large, but the problem constraints (implied by the original snippet) are small enough. The original snippet uses a map and recursion, so we do the same. Edge cases: if the query is called when no add has happened, the root value is 0, budget 0 >= 0, return 0. Also, when computing `mp[i] - mp[l]`, note that `mp[l]` might not exist (value 0). The map's `operator[]` inserts a 0, which is fine. But we must be careful: after inserting children values, they become touched nodes, but they have value 0, so the recursion will terminate at them. That's acceptable. Time complexity: each add is O(log V) where V is the node index (max ~2^31), but each step is just a map update. Each query is O(number of touched nodes in the recursion tree), which is at most the total number of distinct nodes ever created. Space complexity: O(number of distinct nodes touched), which is at most 31 * number of adds. For the reference solution, we'll write a function `processOperations(int h, vector<pair<string, vector<int>>> ops)` but the original snippet reads h and q but doesn't use h except in input? Actually h is not used except being read. So we can ignore h. We'll define a class `TreeBudget` with methods `add(int v, int e)` and `query()`. But to match the task specification, we'll write a free function that takes a vector of strings (operations) and returns a vector of doubles. For clarity, we'll parse each operation string. We'll use `map<int, double> values`. The recursion function will be a member of a struct or a lambda capturing the map. Since the task requires a free function, we'll implement it inside a struct or use a helper function with a global map? But the solution must be self-contained with a descriptively named free function. We'll write `std::vector<double> evaluateOperations(const std::vector<std::string>& operations)`. Inside, we'll define a lambda `dfs` that captures `values` by reference. However, C++ lambdas cannot easily capture a local map and be recursive; we need `std::function` or use a struct. We'll use a struct with a method.
//
// The recursion base case: if `budget >= values[node]` return `budget`. else compute left and right child indices, recursively call with `max(budget, values[node] - values[left])` and similarly for right. Use `values[left]` which defaults to 0 if not present. Important: we must not use `values[node]` in a way that inserts new nodes inadvertently during query? In the recursion, when we call `values[left]`, `operator[]` will insert a 0, which is fine but increases the map size. That's acceptable. But to be efficient, we can use `find` to check existence and use 0 if absent. We'll do that to avoid unnecessary insertions. However, the recursion will eventually stop because we only go deeper when `budget < values[node]`, and if the child has value 0, then `max(budget, values[node] - 0)` is at least `values[node]` which is > budget, so we call recursively on a child with value 0, and the next call will have `budget >= 0`? Let's see: if child value is 0, the condition `budget >= 0` is always true (since budget is non-negative), so it returns budget immediately. So it terminates after one extra level. Good. So no infinite recursion.
//
// Edge case: negative e? The original says positive e but the snippet doesn't enforce. We'll assume positive. Also, h is read but unused.
//
// Time complexity: each add O(log V) with V up to 2^31, but since we use map, each step is O(log N) where N is number of distinct nodes. Overall O(Q * (number of touched nodes)) for queries. Space O(N).
//
// Now produce the solution.
#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <functional>

// Evaluates add/query operations on an infinite complete binary tree.
// Each node holds a double value, initially 0.
// "add v e" adds e to all ancestors of v (including v).
// "query" returns the result of dfs(1, 0) as defined in the statement.
std::vector<double> evaluateOperations(const std::vector<std::string>& operations) {
    std::map<int, double> values; // node -> current value (default 0)
    
    // Recursive helper. Since it needs to be recursive, we use a std::function.
    std::function<double(int, double)> dfs = [&](int node, double budget) -> double {
        double nodeVal = 0.0;
        auto itNode = values.find(node);
        if (itNode != values.end()) nodeVal = itNode->second;
        if (budget >= nodeVal) return budget;
        
        int left = node << 1;
        int right = left + 1;
        
        double leftVal = 0.0;
        auto itLeft = values.find(left);
        if (itLeft != values.end()) leftVal = itLeft->second;
        
        double rightVal = 0.0;
        auto itRight = values.find(right);
        if (itRight != values.end()) rightVal = itRight->second;
        
        double needLeft = dfs(left, std::max(budget, nodeVal - leftVal));
        double needRight = dfs(right, std::max(budget, nodeVal - rightVal));
        return 0.5 * (needLeft + needRight);
    };
    
    std::vector<double> results;
    for (const std::string& op : operations) {
        std::istringstream iss(op);
        std::string cmd;
        iss >> cmd;
        if (cmd == "add") {
            int v, e;
            iss >> v >> e;
            int cur = v;
            while (cur >= 1) {
                values[cur] += e;
                cur >>= 1;
            }
        } else if (cmd == "query") {
            results.push_back(dfs(1, 0.0));
        }
    }
    return results;
}
#include <cassert>
#include <cmath>
#include <vector>
#include <string>

int main() {
    // Test 1: No operations -> no queries
    std::vector<std::string> ops1 = {};
    assert(evaluateOperations(ops1).empty());
    
    // Test 2: Single query with no adds -> root value 0, budget 0 >= 0 -> returns 0
    std::vector<std::string> ops2 = {"query"};
    auto res2 = evaluateOperations(ops2);
    assert(res2.size() == 1);
    assert(std::abs(res2[0] - 0.0) < 1e-9);
    
    // Test 3: Add to root, query
    // add 1 10 -> root becomes 10
    // query: dfs(1,0): budget 0 < 10, children have value 0.
    //   left: max(0, 10-0)=10 -> child value 0, budget >=0 -> return 10
    //   right: same -> return 10
    //   result = 0.5*(10+10)=10
    std::vector<std::string> ops3 = {"add 1 10", "query"};
    auto res3 = evaluateOperations(ops3);
    assert(std::abs(res3[0] - 10.0) < 1e-9);
    
    // Test 4: Add to left child and right child
    // add 2 5 -> path from 2 to root: node 2 gets 5, node 1 gets 5 (total 5? root already 0)
    // add 3 7 -> node 3 gets 7, node 1 gets 7 (now root = 5+7=12)
    // query: root=12, budget=0
    //   left child node2 value=5, right child node3 value=7
    //   left: max(0,12-5)=7 -> child value 5, budget 7 >= 5 -> return 7
    //   right: max(0,12-7)=5 -> child value 7, budget 5 < 7 -> need children of node3
    //     node3's children value 0, so need left = max(5,7-0)=7, return 7 (since budget 7>=0)
    //     similarly right = 7
    //     node3 returns 0.5*(7+7)=7
    //   right returns 7
    //   root result = 0.5*(7+7)=7
    std::vector<std::string> ops4 = {"add 2 5", "add 3 7", "query"};
    auto res4 = evaluateOperations(ops4);
    assert(std::abs(res4[0] - 7.0) < 1e-9);
    
    // Test 5: Multiple adds to same node accumulate
    // add 2 3, add 2 4 -> node2=7, root=7
    // query: root=7, budget=0, children node2=7, node3=0
    //   left: max(0,7-7)=0 -> budget 0, node2 value 7 -> need children of node2
    //     node2's children value 0, left: max(0,7-0)=7 -> return 7, right similarly 7
    //     node2 returns 7
    //   right: max(0,7-0)=7 -> return 7
    //   result = 0.5*(7+7)=7
    std::vector<std::string> ops5 = {"add 2 3", "add 2 4", "query"};
    auto res5 = evaluateOperations(ops5);
    assert(std::abs(res5[0] - 7.0) < 1e-9);
    
    // Test 6: Deep tree, verify recursion terminates and produces finite result
    // add 100 1 (path: 100->50->25->12->6->3->1) increases several nodes
    // Then query should not crash and returns some value
    std::vector<std::string> ops6 = {"add 100 1", "query"};
    auto res6 = evaluateOperations(ops6);
    assert(res6.size() == 1);
    assert(std::isfinite(res6[0]));
    
    // Test 7: Multiple queries after adds
    std::vector<std::string> ops7 = {"add 1 2", "query", "add 2 3", "query"};
    auto res7 = evaluateOperations(ops7);
    // First query: root=2, children 0 -> result = 0.5*(2+2)=2
    assert(std::abs(res7[0] - 2.0) < 1e-9);
    // After add 2 3: node2=3, root=5 (2+3)
    // Query: root=5, budget=0, left child node2=3, right child node3=0
    //   left: max(0,5-3)=2 -> budget 2 < node2 value 3 -> need children of node2 (value 0)
    //     left child of node2: max(2,3-0)=3 -> return 3, right also 3 -> node2 returns 3
    //   right: max(0,5-0)=5 -> return 5
    //   result = 0.5*(3+5)=4
    assert(std::abs(res7[1] - 4.0) < 1e-9);
    
    return 0;
}
