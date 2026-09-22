Write a C++ function `findPath(const std::string& start, const std::string& goal)` that performs a **greedy best-first search** on the given graph (mapping cities to neighboring cities with edge costs) using only the provided heuristic function (mapping each city to an estimated cost to reach the goal). The function must return the path (as a `std::vector<std::string>` of city names in order from `start` to `goal`) found by always expanding the node with the lowest heuristic value, without considering actual edge costs, and using a simple "first time visited" rule (no re‑opening). If no path exists, return an empty vector. The graph and heuristic are fixed as follows (cities A–G):  
- Edges (with costs): A–B (2), A–C (4), B–A (4), B–D (3), B–E (1), C–A (2), C–F (5), D–B (3), D–G (7), E–B (1), E–G (2), F–C (4), F–G (1), G–D (7), G–E (1), G–F (1).  
- Heuristic values (estimated distance to G): A=6, B=4, C=3, D=4, E=2, F=1, G=0.  
You may assume the input start and goal are always valid city letters from the set {A,B,C,D,E,F,G} and that `start != goal`. Your function must be self‑contained; you may define the graph and heuristic as local static data inside the function.
#include <bits/stdc++.h>
// The solution function is assumed to be defined above.

int main() {
    // Test 1: Basic path from A to G using the given heuristic.
    // Greedy best-first expands A (h=6) -> C (h=3) because C is lower than B (h=4),
    // then C -> F (h=1), then F -> G (h=0). So expected path: A -> C -> F -> G.
    std::vector<std::string> path1 = findPath("A", "G");
    std::vector<std::string> expected1 = {"A", "C", "F", "G"};
    assert(path1 == expected1);

    // Test 2: Path from B to G. B (h=4) expands neighbors: A(6), D(4), E(2).
    // The lowest heuristic is E(2), then E expands G(0). So path: B -> E -> G.
    std::vector<std::string> path2 = findPath("B", "G");
    std::vector<std::string> expected2 = {"B", "E", "G"};
    assert(path2 == expected2);

    // Test 3: Path from D to G. D (h=4) expands B(4) and G(0). Since G has lower priority,
    // but B is visited first? Actually both are pushed with priorities 4 (B) and 0 (G).
    // The min-heap pops G first, so path is just D -> G.
    std::vector<std::string> path3 = findPath("D", "G");
    std::vector<std::string> expected3 = {"D", "G"};
    assert(path3 == expected3);

    // Test 4: Path from E to G. E (h=2) expands B(4) and G(0). G is popped immediately,
    // so path is E -> G.
    std::vector<std::string> path4 = findPath("E", "G");
    std::vector<std::string> expected4 = {"E", "G"};
    assert(path4 == expected4);

    // Test 5: Path from F to G. F (h=1) expands C(3) and G(0). G is popped immediately.
    std::vector<std::string> path5 = findPath("F", "G");
    std::vector<std::string> expected5 = {"F", "G"};
    assert(path5 == expected5);

    // Test 6: Path from C to G. C (h=3) expands A(6) and F(1). Then F expands G(0). Path: C -> F -> G.
    std::vector<std::string> path6 = findPath("C", "G");
    std::vector<std::string> expected6 = {"C", "F", "G"};
    assert(path6 == expected6);

    // Test 7: Path from G to itself is not allowed by task, but we can test another goal, say A.
    // From G (h=0) to A (h=6). G expands D(4), E(2), F(1). The lowest is F(1), then F expands C(3),
    // then C expands A(6) (since A hasn't been visited yet). Path: G -> F -> C -> A.
    std::vector<std::string> path7 = findPath("G", "A");
    std::vector<std::string> expected7 = {"G", "F", "C", "A"};
    assert(path7 == expected7);

    // Test 8: Ensure the path starts and ends correctly for a multi-step path.
    assert(path1.front() == "A" && path1.back() == "G");
    assert(path2.front() == "B" && path2.back() == "G");
    assert(path3.front() == "D" && path3.back() == "G");

    // Test 9: Ensure the path contains unique nodes (no cycles) because we never revisit.
    auto check_unique = [](const std::vector<std::string>& p) {
        std::set<std::string> s(p.begin(), p.end());
        return s.size() == p.size();
    };
    assert(check_unique(path1));
    assert(check_unique(path7));

    // Test 10: Test a longer path, e.g., from A to D. 
    // A expands B(4) and C(3). Lowest is C(3), then C expands F(1), F expands G(0), G expands D(4).
    // But note: D may have been visited earlier? No, because we haven't visited D yet. So path: A -> C -> F -> G -> D.
    std::vector<std::string> path10 = findPath("A", "D");
    std::vector<std::string> expected10 = {"A", "C", "F", "G", "D"};
    assert(path10 == expected10);

    return 0;
}
#include <bits/stdc++.h>

// Perform greedy best-first search using a fixed graph and heuristic.
// Returns a vector of city names representing the path from start to goal,
// or an empty vector if no path exists.
std::vector<std::string> findPath(const std::string& start, const std::string& goal) {
    // Fixed graph: first string = current city, inner map: neighbor -> edge cost (cost unused in this search).
    static const std::map<std::string, std::map<std::string, int>> graph = {
        {"A", {{"B", 2}, {"C", 4}}},
        {"B", {{"A", 4}, {"D", 3}, {"E", 1}}},
        {"C", {{"A", 2}, {"F", 5}}},
        {"D", {{"B", 3}, {"G", 7}}},
        {"E", {{"B", 1}, {"G", 2}}},
        {"F", {{"C", 4}, {"G", 1}}},
        {"G", {{"D", 7}, {"E", 1}, {"F", 1}}}
    };

    // Heuristic: estimated distance from each city to goal 'G'.
    static const std::map<std::string, int> heuristic = {
        {"A", 6}, {"B", 4}, {"C", 3}, {"D", 4}, {"E", 2}, {"F", 1}, {"G", 0}
    };

    // Min-heap: (priority = heuristic value, city name). Use greater to make it a min-heap.
    using QueueElement = std::pair<int, std::string>;
    std::priority_queue<QueueElement, std::vector<QueueElement>, std::greater<QueueElement>> pq;

    // Store the predecessor of each visited node for path reconstruction.
    std::map<std::string, std::string> cameFrom;

    pq.push({heuristic.at(start), start});
    cameFrom[start] = "";

    while (!pq.empty()) {
        std::string current = pq.top().second;
        pq.pop();

        if (current == goal) {
            // Reconstruct path from start to goal.
            std::vector<std::string> path;
            while (current != "") {
                path.push_back(current);
                current = cameFrom[current];
            }
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Explore neighbors that have not been visited yet.
        for (const auto& neighbour : graph.at(current)) {
            const std::string& next = neighbour.first;
            if (cameFrom.find(next) == cameFrom.end()) {
                cameFrom[next] = current;
                int priority = heuristic.at(next);
                pq.push({priority, next});
            }
        }
    }

    return {}; // No path found.
}
// The main algorithm is a best‑first search using a priority queue (min‑heap) that orders nodes by their heuristic value. Start by pushing the start node with its heuristic and record it as visited (via a `cameFrom` map). Repeatedly pop the node with the smallest heuristic; if it is the goal, reconstruct the path by backtracking through `cameFrom`. Otherwise, for each neighbor of the current node, if the neighbor has not been visited yet (not in `cameFrom`), record its predecessor and push it with its heuristic value. This greedy approach may not find the shortest path but will find some path if one exists, provided the heuristic is consistent enough (which is not required for this task—just following the snippet’s logic). Edge cases: if the goal is unreachable, return an empty vector; if the start equals the goal, the snippet’s logic would return a path containing only the start (but we assume they differ). Time complexity is \(O(V \log V + E)\) in the worst case (each node pushed at most once), and space is \(O(V)\) for the maps and priority queue. The heuristic is only used as priority; actual edge costs are ignored.
