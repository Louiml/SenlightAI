/*
Write a C++ function named `calcEquation` that takes three parameters: a vector of pairs of strings `equations`, where each pair represents a division equation `a / b`; a vector of doubles `values`, where `values[i]` is the result of `equations[i].first / equations[i].second`; and a vector of pairs of strings `queries`, where each pair `{x, y}` asks for the value of `x / y`. The function must return a vector of doubles, where each answer corresponds to the query in order. If a query involves a variable that never appears in any equation, or if the path between the two variables is disconnected (i.e., no sequence of divisions can be chained from `x` to `y`), return `-1.0` for that query. You may assume all given values are positive and non-zero, and no contradictory equations exist. For example, if equations = { {"a","b"}, {"b","c"} }, values = {2.0, 3.0}, then queries = { {"a","c"}, {"c","a"}, {"b","a"}, {"x","y"} } should produce {6.0, 1/6.0, 1/2.0, -1.0}. Implement the solution using a graph representation and a depth-first search for each query.
*/
#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include <functional>

// Given division equations and their values, evaluate each query x/y.
std::vector<double> calcEquation(
    const std::vector<std::pair<std::string, std::string>>& equations,
    const std::vector<double>& values,
    const std::vector<std::pair<std::string, std::string>>& queries) {
    
    // Build the graph: variable -> list of (neighbor, weight)
    std::unordered_map<std::string, std::vector<std::pair<std::string, double>>> graph;
    for (size_t i = 0; i < equations.size(); ++i) {
        const auto& a = equations[i].first;
        const auto& b = equations[i].second;
        double v = values[i];
        graph[a].push_back({b, v});
        graph[b].push_back({a, 1.0 / v});
    }
    
    // Helper DFS function to find product from current to target.
    std::function<bool(const std::string&, const std::string&, double, 
                       std::unordered_map<std::string, bool>&, double&)> dfs =
        [&](const std::string& node, const std::string& target, double acc,
            std::unordered_map<std::string, bool>& visited, double& result) -> bool {
        visited[node] = true;
        if (node == target) {
            result = acc;
            return true;
        }
        for (const auto& neighbor_pair : graph.at(node)) {
            const std::string& next = neighbor_pair.first;
            double weight = neighbor_pair.second;
            if (!visited[next]) {
                if (dfs(next, target, acc * weight, visited, result)) {
                    return true;
                }
            }
        }
        return false;
    };
    
    std::vector<double> answers;
    answers.reserve(queries.size());
    
    for (const auto& query : queries) {
        const std::string& x = query.first;
        const std::string& y = query.second;
        
        // If either variable is unknown, answer is -1.0
        if (graph.find(x) == graph.end() || graph.find(y) == graph.end()) {
            answers.push_back(-1.0);
            continue;
        }
        
        // DFS from x to y
        std::unordered_map<std::string, bool> visited;
        for (const auto& entry : graph) {
            visited[entry.first] = false;
        }
        
        double result = -1.0;
        if (dfs(x, y, 1.0, visited, result)) {
            answers.push_back(result);
        } else {
            answers.push_back(-1.0);
        }
    }
    
    return answers;
}
#include <cassert>
#include <vector>
#include <string>
#include <utility>
#include <cmath>

int main() {
    // Test case 1: Simple chain
    std::vector<std::pair<std::string, std::string>> eq1 = {{"a","b"}, {"b","c"}};
    std::vector<double> val1 = {2.0, 3.0};
    std::vector<std::pair<std::string, std::string>> q1 = {{"a","c"}, {"c","a"}, {"b","a"}, {"a","a"}, {"x","y"}};
    std::vector<double> ans1 = calcEquation(eq1, val1, q1);
    assert(std::abs(ans1[0] - 6.0) < 1e-9);
    assert(std::abs(ans1[1] - (1.0/6.0)) < 1e-9);
    assert(std::abs(ans1[2] - 0.5) < 1e-9);
    assert(std::abs(ans1[3] - 1.0) < 1e-9);
    assert(std::abs(ans1[4] + 1.0) < 1e-9); // -1.0

    // Test case 2: Disconnected components
    std::vector<std::pair<std::string, std::string>> eq2 = {{"a","b"}};
    std::vector<double> val2 = {4.0};
    std::vector<std::pair<std::string, std::string>> q2 = {{"a","b"}, {"b","a"}, {"a","c"}};
    std::vector<double> ans2 = calcEquation(eq2, val2, q2);
    assert(std::abs(ans2[0] - 4.0) < 1e-9);
    assert(std::abs(ans2[1] - 0.25) < 1e-9);
    assert(std::abs(ans2[2] + 1.0) < 1e-9);

    // Test case 3: Self-loop (start equals end, variable exists)
    std::vector<std::pair<std::string, std::string>> eq3 = {{"x","y"}};
    std::vector<double> val3 = {2.5};
    std::vector<std::pair<std::string, std::string>> q3 = {{"x","x"}};
    std::vector<double> ans3 = calcEquation(eq3, val3, q3);
    assert(std::abs(ans3[0] - 1.0) < 1e-9);

    // Test case 4: Multiple paths (cycle) but still consistent
    std::vector<std::pair<std::string, std::string>> eq4 = {{"a","b"}, {"b","c"}, {"c","a"}};
    std::vector<double> val4 = {2.0, 3.0, 1.0/6.0};
    std::vector<std::pair<std::string, std::string>> q4 = {{"a","c"}, {"b","a"}};
    std::vector<double> ans4 = calcEquation(eq4, val4, q4);
    assert(std::abs(ans4[0] - 6.0) < 1e-9);
    assert(std::abs(ans4[1] - 0.5) < 1e-9);

    // Test case 5: Empty equations and queries
    std::vector<std::pair<std::string, std::string>> eq5;
    std::vector<double> val5;
    std::vector<std::pair<std::string, std::string>> q5 = {{"p","q"}};
    std::vector<double> ans5 = calcEquation(eq5, val5, q5);
    assert(std::abs(ans5[0] + 1.0) < 1e-9);

    return 0;
}
// The problem can be modeled as a directed weighted graph where each variable is a node, and an equation `a / b = v` introduces two directed edges: `a -> b` with weight `v` (meaning a/b = v) and `b -> a` with weight `1/v` (meaning b/a = 1/v). A query `x / y` asks for the product of edge weights along any path from `x` to `y`; if no path exists, the answer is `-1.0`. Use an unordered_map to store adjacency lists: each key is a variable name, and the value is a vector of pairs {neighbor, weight}. For each query, first check if both variables exist in the graph; if not, immediately return -1.0. Otherwise, perform a DFS from the start variable with a current accumulator value (starting at 1.0). Mark nodes as visited to avoid cycles. When the target is reached, return the accumulated value. If the DFS exhausts all neighbors without finding the target, return -1.0. Edge cases include queries where the start equals the end (answer is 1.0 if the variable exists), variables that appear only in one direction but still connected via a longer path, and disconnected components. Since each query runs an independent DFS on a graph with V nodes and E edges, the worst-case time per query is O(V + E), and with Q queries the total is O(Q * (V + E)). Space complexity is O(V + E) for the graph plus O(V) for the visited map per query.
