/*
Write a C++ function that simulates a simplified version of the GEOS polygon-edge-ring construction: given a starting directed edge and a collection of directed edges forming a closed ring (each edge has a `next` pointer), the function must traverse the ring, compute the total geometric length of the ring (sum of Euclidean distances between consecutive coordinates), and return the number of edges in the ring. Each directed edge stores a pointer to its originating node, a pointer to its next directed edge in the ring, and a boolean flag indicating whether it has been visited. The function must handle rings that may be traversed multiple times (e.g., due to shared edges) by marking edges as visited and skipping already-visited edges, ensuring each edge is counted exactly once. The input is a `std::vector<DirectedEdge*>` where each edge has `start` and `end` coordinates as `std::pair<double,double>`, and the edges are connected such that `edge->next` points to the next edge. The function should start from the first unvisited edge in the vector and follow `next` pointers until returning to the start, counting and summing lengths. It should then continue scanning the vector for any other unvisited edges (to handle multiple disjoint rings if present) and accumulate the total count and length across all rings. Return a `std::pair<int, double>` where the first element is the total number of edges in all rings and the second is the total length. The function must be robust to empty input (return `{0, 0.0}`) and to self-loops (a single edge whose `next` points to itself). The time complexity should be O(E) where E is the number of edges, and space O(1) beyond the input.
*/
#include <vector>
#include <utility>
#include <cmath>
#include <cstddef>

struct DirectedEdge {
    std::pair<double, double> start;
    std::pair<double, double> end;
    DirectedEdge* next;
    bool visited;
};

// Compute total edge count and total geometric length of all closed rings formed by directed edges.
std::pair<int, double> computeRingStats(std::vector<DirectedEdge*>& edges) {
    int totalEdges = 0;
    double totalLength = 0.0;

    for (DirectedEdge* edge : edges) {
        if (edge == nullptr || edge->visited) {
            continue;
        }

        DirectedEdge* current = edge;
        int ringEdges = 0;
        double ringLength = 0.0;

        do {
            current->visited = true;
            ++ringEdges;
            double dx = current->end.first - current->start.first;
            double dy = current->end.second - current->start.second;
            ringLength += std::sqrt(dx * dx + dy * dy);
            current = current->next;
        } while (current != edge && current != nullptr);

        // If we hit a null pointer (should not happen in valid ring), break to avoid infinite loop.
        if (current == nullptr) {
            break;
        }

        totalEdges += ringEdges;
        totalLength += ringLength;
    }

    return {totalEdges, totalLength};
}
#include <cassert>
#include <cmath>

int main() {
    // Test 1: Empty input
    std::vector<DirectedEdge*> empty;
    auto result = computeRingStats(empty);
    assert(result.first == 0);
    assert(std::fabs(result.second - 0.0) < 1e-9);

    // Test 2: Single self-loop edge (start == end)
    DirectedEdge self;
    self.start = {0.0, 0.0};
    self.end = {0.0, 0.0};
    self.next = &self;
    self.visited = false;
    std::vector<DirectedEdge*> oneEdge = {&self};
    result = computeRingStats(oneEdge);
    assert(result.first == 1);
    assert(std::fabs(result.second - 0.0) < 1e-9);

    // Test 3: Triangle ring of 3 edges
    DirectedEdge e1, e2, e3;
    e1.start = {0.0, 0.0}; e1.end = {1.0, 0.0}; e1.next = &e2; e1.visited = false;
    e2.start = {1.0, 0.0}; e2.end = {0.0, 1.0}; e2.next = &e3; e2.visited = false;
    e3.start = {0.0, 1.0}; e3.end = {0.0, 0.0}; e3.next = &e1; e3.visited = false;
    std::vector<DirectedEdge*> triangle = {&e1, &e2, &e3};
    result = computeRingStats(triangle);
    assert(result.first == 3);
    // Perimeter: 1 + sqrt(2) + 1 = 2 + sqrt(2) ≈ 3.414213562
    assert(std::fabs(result.second - (2.0 + std::sqrt(2.0))) < 1e-9);

    // Test 4: Two disjoint rings (square and a separate edge)
    DirectedEdge s1, s2, s3, s4;
    s1.start = {0.0, 0.0}; s1.end = {0.0, 1.0}; s1.next = &s2; s1.visited = false;
    s2.start = {0.0, 1.0}; s2.end = {1.0, 1.0}; s2.next = &s3; s2.visited = false;
    s3.start = {1.0, 1.0}; s3.end = {1.0, 0.0}; s3.next = &s4; s3.visited = false;
    s4.start = {1.0, 0.0}; s4.end = {0.0, 0.0}; s4.next = &s1; s4.visited = false;

    DirectedEdge lone_a, lone_b;
    lone_a.start = {10.0, 10.0}; lone_a.end = {12.0, 10.0}; lone_a.next = &lone_b; lone_a.visited = false;
    lone_b.start = {12.0, 10.0}; lone_b.end = {10.0, 10.0}; lone_b.next = &lone_a; lone_b.visited = false;

    std::vector<DirectedEdge*> multi = {&s1, &s2, &s3, &s4, &lone_a, &lone_b};
    result = computeRingStats(multi);
    assert(result.first == 6); // 4 + 2
    // Square perimeter 4, lone ring length = 2 + 2 = 4? Actually lone_a length 2, lone_b length 2, total 4.
    assert(std::fabs(result.second - (4.0 + 4.0)) < 1e-9);

    // Test 5: Ring where edges are in vector but start from a different edge
    // Reuse triangle but shuffle vector order
    std::vector<DirectedEdge*> shuffled = {&e2, &e1, &e3};
    result = computeRingStats(shuffled);
    assert(result.first == 3);
    assert(std::fabs(result.second - (2.0 + std::sqrt(2.0))) < 1e-9);

    return 0;
}
// The solution iterates through the vector of directed edges, and for each unvisited edge, it begins a traversal following `next` pointers. Since the edges form closed rings (as per the problem statement), following `next` from any unvisited edge will eventually loop back to the starting edge, and because we mark each visited edge as visited, we avoid counting any edge more than once. For each edge encountered during the traversal, we increment a local edge count and add the Euclidean distance between its `start` and `end` coordinates to a local length sum. We mark each edge as visited as we go. After the traversal returns to the starting edge (the loop condition: `current != start`), we add the local count and length to the global totals. We also need to handle the edge case where the vector contains edges that are part of the same ring but start from different points; since the problem statement guarantees closed rings and each edge has a valid `next`, starting from any unvisited edge will traverse the entire ring. The algorithm is correct because each edge is visited exactly once due to the visited flag, and the `next` pointers maintain the ring structure. Edge cases: empty input returns `{0, 0.0}`; a single self-loop edge counts once and length is the distance from start to end (usually zero if start==end, but could be non-zero if the edge has same endpoints but different coordinates—though typically self-loop has same start and end). If the vector contains multiple disjoint rings, the outer loop over the vector picks up each unvisited edge, and each traversal covers exactly one full ring. Time complexity is O(E) because each edge is processed exactly once (visited flag prevents reprocessing), and space is O(1) since we use only a few local variables and modify the visited flags in the existing edge objects (no additional data structures). Note: we assume the `DirectedEdge` class has public members `next`, `start`, `end`, and `visited`.
