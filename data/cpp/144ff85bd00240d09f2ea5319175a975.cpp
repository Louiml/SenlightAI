Write a C++ function named `graphKernelMean` that takes two graphs represented as an adjacency list (using `std::map<int, std::vector<std::pair<int, char>>>` where each node has an integer ID and a character label, and each edge stores the neighbor ID and an edge label character), and two bags of trails represented as `std::vector<std::vector<int>>` (each trail is a sequence of node IDs where consecutive pairs are connected by an edge, and the sequence alternates node, edge, node, edge, ..., node). The function should compute the normalized kernel between the two bags by counting the number of pairs of trails (one from each bag) that are "structurally identical" — meaning they have the same length, the same starting node label, the same edge labels in order, and the same node labels in order — and dividing this count by the product of the bag sizes. If the two bags are the same object (i.e., the same bag pointer is passed for both), compute the kernel value for a single bag by counting unordered pairs of equal trails (including each trail compared with itself exactly once), multiplying by 2 for each unordered pair and adding the bag size, then dividing by the square of the bag size. The function should return a `double`, be `const`-correct with respect to graph data, and assume all node IDs referenced in the trails exist in the graph.
#include <cassert>

int main() {
    // Build a simple graph: nodes 0('A'),1('B'),2('A')
    Graph g;
    g[0] = {{1,'x'}};
    g[1] = {{0,'x'},{2,'y'}};
    g[2] = {{1,'y'}};
    std::map<int,char> labels = {{0,'A'},{1,'B'},{2,'A'}};

    // Bag1: two trails: [0,1] and [1,2]
    BagOfTrails bag1 = {{0,1},{1,2}};
    // Bag2: same trails in different order: [1,2] and [0,1]
    BagOfTrails bag2 = {{1,2},{0,1}};

    // Compare bag1 and bag2: each pair? [0,1] vs [1,2]: lengths match, start labels A vs B -> not equal; [0,1] vs [0,1] equal; [1,2] vs [1,2] equal; [1,2] vs [0,1] not equal. So sum=2, P1*P2=4, result=0.5
    double result = graphKernelMean(g, labels, bag1, bag2);
    assert(std::abs(result - 0.5) < 1e-9);

    // Self comparison: bag1 contains two distinct trails, no equal pairs, so sum=0*2 + 2 = 2, divided by 4 = 0.5
    double self = graphKernelMeanSelf(g, labels, bag1);
    assert(std::abs(self - 0.5) < 1e-9);

    // Empty bag: should return 0.0
    BagOfTrails empty;
    assert(graphKernelMean(g, labels, empty, bag1) == 0.0);
    assert(graphKernelMeanSelf(g, labels, empty) == 0.0);

    // Bag with identical trails: two copies of [0,1]
    BagOfTrails bag_dup = {{0,1},{0,1}};
    // self: one unordered pair => sum=1*2+2=4, /4 = 1.0
    assert(std::abs(graphKernelMeanSelf(g, labels, bag_dup) - 1.0) < 1e-9);
    // cross with itself: each pair matches? 4 matches, /4 = 1.0
    assert(std::abs(graphKernelMean(g, labels, bag_dup, bag_dup) - 1.0) < 1e-9);

    // Different lengths never match
    BagOfTrails bag_long = {{0,1,2}}; // trail of length 3
    assert(graphKernelMean(g, labels, bag_long, bag1) == 0.0);

    // Single-node trails: compare two single nodes with same label
    BagOfTrails bag_single = {{0},{2}};
    BagOfTrails bag_single2 = {{2},{0}};
    // both trails are single nodes with label 'A', all four pairs match, sum=4, /4=1.0
    assert(std::abs(graphKernelMean(g, labels, bag_single, bag_single2) - 1.0) < 1e-9);
}
#include <vector>
#include <map>
#include <algorithm>
#include <cstddef>

// Graph: node ID -> vector of (neighbor, edge_label)
using Graph = std::map<int, std::vector<std::pair<int, char>>>;
using Trail = std::vector<int>;
using BagOfTrails = std::vector<Trail>;

// Helper: check if two trails are structurally identical given the graph.
bool trailsEqual(const Trail& t1, const Trail& t2, const Graph& graph) {
    if (t1.size() != t2.size()) return false;
    if (t1.empty()) return true; // both empty

    // Compare starting node labels
    auto it1 = graph.find(t1[0]);
    auto it2 = graph.find(t2[0]);
    if (it1 == graph.end() || it2 == graph.end()) return false;
    if (it1->second.empty() || it2->second.empty()) return false;
    // The graph structure must store the node label; but for simplicity we assume nodes are labeled by their ID? 
    // To be faithful, we need node labels. Since the task only mentions edge labels and node IDs, we will treat node labels as the node ID itself? 
    // The original snippet used GETVALUE("atom", Char) for node labels. To keep it self-contained, we assume node label is a char stored elsewhere.
    // To avoid overcomplicating, we assume the trail's first node is always valid and node labels are not compared? But the task says compare starting node labels.
    // We will adapt by storing node labels in a separate map: node ID -> char label.
    // For simplicity, the function signature will include a separate nodeLabels map.
    // Redesign: function takes graph, nodeLabels, bags.
    // I'll provide a revised solution below.
    return false; // placeholder
}

// Correct solution with node labels:
double graphKernelMean(const Graph& graph, const std::map<int,char>& nodeLabels,
                       const BagOfTrails& bag1, const BagOfTrails& bag2) {
    if (bag1.empty() || bag2.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& t1 : bag1) {
        for (const auto& t2 : bag2) {
            if (t1.size() != t2.size()) continue;
            bool equal = true;
            if (t1.empty()) {
                // empty trails: both same
                // but we still need to compare starting label? empty has no nodes
                // ignore for now
            } else {
                auto lab1 = nodeLabels.find(t1[0]);
                auto lab2 = nodeLabels.find(t2[0]);
                if (lab1 == nodeLabels.end() || lab2 == nodeLabels.end() || lab1->second != lab2->second) continue;
                for (std::size_t k = 0; k + 1 < t1.size(); ++k) {
                    // find edge label between t1[k] and t1[k+1] in graph
                    char e1 = '\0', e2 = '\0';
                    bool found1 = false, found2 = false;
                    auto node1 = graph.find(t1[k]);
                    if (node1 != graph.end()) {
                        for (const auto& p : node1->second) {
                            if (p.first == t1[k+1]) { e1 = p.second; found1 = true; break; }
                        }
                    }
                    auto node2 = graph.find(t2[k]);
                    if (node2 != graph.end()) {
                        for (const auto& p : node2->second) {
                            if (p.first == t2[k+1]) { e2 = p.second; found2 = true; break; }
                        }
                    }
                    if (!found1 || !found2 || e1 != e2) { equal = false; break; }
                    // also compare node label of t1[k+1] and t2[k+1]
                    auto nl1 = nodeLabels.find(t1[k+1]);
                    auto nl2 = nodeLabels.find(t2[k+1]);
                    if (nl1 == nodeLabels.end() || nl2 == nodeLabels.end() || nl1->second != nl2->second) { equal = false; break; }
                }
            }
            if (equal) sum += 1.0;
        }
    }
    return sum / (double)(bag1.size() * bag2.size());
}

// Single-bag version:
double graphKernelMeanSelf(const Graph& graph, const std::map<int,char>& nodeLabels, const BagOfTrails& bag) {
    if (bag.empty()) return 0.0;
    double sum = 0.0;
    for (std::size_t i = 0; i < bag.size(); ++i) {
        for (std::size_t j = i+1; j < bag.size(); ++j) {
            if (bag[i].size() != bag[j].size()) continue;
            bool equal = true;
            if (bag[i].empty()) {
                // empty trails count as equal
            } else {
                auto lab1 = nodeLabels.find(bag[i][0]);
                auto lab2 = nodeLabels.find(bag[j][0]);
                if (lab1 == nodeLabels.end() || lab2 == nodeLabels.end() || lab1->second != lab2->second) continue;
                for (std::size_t k = 0; k + 1 < bag[i].size(); ++k) {
                    char e1 = '\0', e2 = '\0';
                    bool found1 = false, found2 = false;
                    auto node1 = graph.find(bag[i][k]);
                    if (node1 != graph.end()) {
                        for (const auto& p : node1->second) {
                            if (p.first == bag[i][k+1]) { e1 = p.second; found1 = true; break; }
                        }
                    }
                    auto node2 = graph.find(bag[j][k]);
                    if (node2 != graph.end()) {
                        for (const auto& p : node2->second) {
                            if (p.first == bag[j][k+1]) { e2 = p.second; found2 = true; break; }
                        }
                    }
                    if (!found1 || !found2 || e1 != e2) { equal = false; break; }
                    auto nl1 = nodeLabels.find(bag[i][k+1]);
                    auto nl2 = nodeLabels.find(bag[j][k+1]);
                    if (nl1 == nodeLabels.end() || nl2 == nodeLabels.end() || nl1->second != nl2->second) { equal = false; break; }
                }
            }
            if (equal) sum += 1.0;
        }
    }
    sum *= 2.0;
    sum += static_cast<double>(bag.size());
    return sum / (double)(bag.size() * bag.size());
}
// The core algorithm compares each trail from bag1 with each trail from bag2 (or within the same bag for the single-bag case). For each pair, it first checks if the lengths match; if not, skip. Then it compares the label of the first node, then iterates through the trail with step 2, treating indices 1,3,5,... as edge IDs and indices 2,4,6,... as subsequent node IDs. Because the trail stores node IDs and edge indices indirectly via negative values (as in the original snippet), but in this simplified task we directly store node IDs and rely on the graph's adjacency to look up edge labels between consecutive nodes. However, to faithfully replicate the original logic without negative indexing, we interpret the trail as alternating node, edge, node, edge, ... but here we simplify: since the input trail is just a sequence of node IDs, we must assume that the graph stores the edge label between any two adjacent nodes in a lookup structure. To match the task description, the trail is given as `std::vector<int>` containing just node IDs, and the graph's adjacency list provides the edge label when moving from node A to node B. So for each consecutive pair in the trail, we check if the edge label between them matches the corresponding pair in the other trail. If any mismatch occurs, break. Edge cases: empty bags (return 0.0 to avoid division by zero), different lengths, missing edges (treat as mismatch), and single-node trails (no edges to compare). Time complexity is O(P1 * P2 * L) where L is the average trail length; space is O(1) extra beyond input.
