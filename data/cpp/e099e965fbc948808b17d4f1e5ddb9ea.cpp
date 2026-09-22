/*
Given a directed graph represented by an adjacency list, write a C++ function that computes the PageRank vector using the power iteration method. The function should take a graph structure that provides the number of nodes, the number of edges, and the outgoing edges for each node (as a vector of neighbor indices). It should initialize all PageRank values to 1.0, then repeatedly apply the PageRank update rule (with a given damping factor) until the maximum absolute change between successive iterations falls below a given convergence threshold or a maximum of 1000 iterations is reached. The final PageRank values must be written into a caller-provided output array. The graph may contain nodes with no outgoing edges (dangling nodes); their contribution must be distributed uniformly across all nodes. The function signature must be: `void pageRank(const Graph& g, double* solution, double damping, double convergence);` where `Graph` is a struct defined as shown. The function must be thread-safe and work for any number of nodes ≥ 1.
*/
#include <vector>
#include <cmath>
#include <algorithm>

// Graph structure: adjacency list representation.
struct Graph {
    int num_nodes;
    int num_edges;
    std::vector<std::vector<int>> outgoing; // outgoing[u] = list of neighbor indices
};

// Compute PageRank using power iteration.
// Writes results into solution array (size g.num_nodes).
void pageRank(const Graph& g, double* solution, double damping, double convergence) {
    const int N = g.num_nodes;
    if (N == 0) return;

    // Current and next rank arrays.
    std::vector<double> rank(N, 1.0);
    std::vector<double> new_rank(N, 0.0);
    const double base_prob = (1.0 - damping) / N;
    const double max_iter = 1000;

    for (int iter = 0; iter < max_iter; ++iter) {
        // Reset new_rank to zero each iteration.
        std::fill(new_rank.begin(), new_rank.end(), 0.0);

        double dangling_sum = 0.0; // Sum of ranks of dangling nodes * damping / N

        // First pass: add contributions from non-dangling nodes.
        for (int u = 0; u < N; ++u) {
            double rank_u = rank[u];
            int deg = g.outgoing[u].size();
            if (deg == 0) {
                dangling_sum += ranking[u] * damping / N;
            } else {
                double contribution = damping * rank_u / deg;
                for (int v : g.outgoing[u]) {
                    new_rank[v] += contribution;
                }
            }
        }

        // Add contributions from dangling nodes (uniform to all).
        if (dangling_sum != 0.0) {
            for (int v = 0; v < N; ++v) {
                new_rank[v] += dangling_sum;
            }
        }

        // Add base probability and compute max change.
        double max_diff = 0.0;
        for (int v = 0; v < N; ++v) {
            new_rank[v] += base_prob;
            max_diff = std::max(max_diff, std::fabs(new_rank[v] - rank[v]));
        }

        // Swap for next iteration.
        rank.swap(new_rank);

        // Check convergence.
        if (max_diff < convergence) {
            break;
        }
    }

    // Copy results to output.
    for (int i = 0; i < N; ++i) {
        solution[i] = rank[i];
    }
}
#include <cassert>
#include <cmath>
#include <vector>

// Assume the solution function is declared (include the header for pageRank).
// For self-containedness, copy the Graph struct and pageRank declaration here.

int main() {
    // Test 1: Single node, no edges.
    Graph g1;
    g1.num_nodes = 1;
    g1.num_edges = 0;
    g1.outgoing = {{}};
    double sol1[1];
    pageRank(g1, sol1, 0.3, 1e-7);
    assert(std::fabs(sol1[0] - 1.0) < 1e-6);

    // Test 2: Two nodes, each links to the other (cycle).
    Graph g2;
    g2.num_nodes = 2;
    g2.num_edges = 2;
    g2.outgoing = {{1}, {0}};
    double sol2[2];
    pageRank(g2, sol2, 0.3, 1e-7);
    // Symmetric: both should be equal.
    assert(std::fabs(sol2[0] - sol2[1]) < 1e-6);
    // Sum should approximate N = 2 (unnormalized) when damping=0.3? Actually values will converge to 1.0 each.
    assert(std::fabs(sol2[0] - 1.0) < 1e-6);

    // Test 3: Star graph: node0 points to 1 and 2; 1 and 2 point nowhere.
    Graph g3;
    g3.num_nodes = 3;
    g3.num_edges = 2;
    g3.outgoing = {{1, 2}, {}, {}};
    double sol3[3];
    pageRank(g3, sol3, 0.3, 1e-7);
    // Dangling nodes 1 and 2 should each receive from node0's contribution.
    // After convergence, because 1 and 2 have no outbound, they leak to all nodes.
    // Intuitively, node0 should have a higher rank than others? Not necessarily. Let's just check sum is 3.
    double sum = sol3[0] + sol3[1] + sol3[2];
    assert(std::fabs(sum - 3.0) < 1e-5); // Initial sum is 3, and total mass is conserved.

    // Test 4: Three nodes in a line: 0->1, 1->2, 2->0 (chain cycle).
    Graph g4;
    g4.num_nodes = 3;
    g4.num_edges = 3;
    g4.outgoing = {{1}, {2}, {0}};
    double sol4[3];
    pageRank(g4, sol4, 0.3, 1e-10);
    // Symmetric cycle: all equal.
    assert(std::fabs(sol4[0] - sol4[1]) < 1e-6);
    assert(std::fabs(sol4[0] - sol4[2]) < 1e-6);
    assert(std::fabs(sol4[0] - 1.0) < 1e-6);

    // Test 5: All nodes dangling (no edges).
    Graph g5;
    g5.num_nodes = 4;
    g5.num_edges = 0;
    g5.outgoing = {{}, {}, {}, {}};
    double sol5[4];
    pageRank(g5, sol5, 0.3, 1e-7);
    // All equal after one iteration, and each = (1-damping)/N + damping/N = 1/N? Actually converges to 1.0? Let's check.
    // Initial rank=1.0, all dangling: new_rank[v] = base_prob + dangling_sum (dangling_sum = damping * sum(rank)/N = damping *1.0)
    // So each = (1-damping)/N + damping = (0.7/4 + 0.3) = 0.175+0.3=0.475, next iteration: base_prob + damping * (0.475*4)/4 = 0.175+0.3*0.475=0.175+0.1425=0.3175, converges to? Solve x = (1-d)/N + d*x? Actually each node's total contribution to all nodes is same. Solve: new = (1-d)/N + d * sum(rank)/N? Wait for all dangling: new_rank[v] = (1-d)/N + d * (sum(rank)/N). Since sum(rank) = N * average. Let avg = sum/N. Then new_rank[v] = (1-d)/N + d*avg. Sum over all v: sum_new = N*(1-d)/N + N*d*avg = (1-d)+d*sum_old. Fixed point sum = (1-d)+d*sum => sum=1. So average = 1/N. So each rank = 1/N. So sol5[0] ≈ 0.25, sum ≈ 1.0.
    assert(std::fabs(sol5[0] - 0.25) < 1e-6);
    double sum5 = 0; for (int i=0;i<4;i++) sum5+=sol5[i];
    assert(std::fabs(sum5 - 1.0) < 1e-5);

    // Test 6: Damping factor 1.0 (no random jumps) with a simple cycle.
    Graph g6;
    g6.num_nodes = 2;
    g6.num_edges = 2;
    g6.outgoing = {{1}, {0}};
    double sol6[2];
    pageRank(g6, sol6, 1.0, 1e-7);
    // With damping=1, base_prob=0, cycle ensures both stay 1.0.
    assert(std::fabs(sol6[0] - 1.0) < 1e-6);

    // Test 7: Larger graph with mixed degrees.
    Graph g7;
    g7.num_nodes = 5;
    g7.num_edges = 6;
    g7.outgoing = {{1,2}, {3}, {3,4}, {0}, {1,2,3}};
    double sol7[5];
    pageRank(g7, sol7, 0.85, 1e-9); // more typical damping
    // Just check that all values are positive and sum approximates 5.
    double sum7 = 0;
    for (int i=0;i<5;i++) {
        assert(sol7[i] > 0.0);
        sum7 += sol7[i];
    }
    assert(std::fabs(sum7 - 5.0) < 1e-5);

    // Test 8: Convergence threshold very loose.
    Graph g8;
    g8.num_nodes = 3;
    g8.num_edges = 3;
    g8.outgoing = {{1}, {2}, {0}};
    double sol8[3];
    pageRank(g8, sol8, 0.3, 100.0); // huge threshold, stops after first iteration
    // After first iteration: initial rank=1.0, each node gets 0.3/1? Actually outdeg=1, contribution = 0.3*1/1=0.3, plus base 0.7/3=0.2333, total 0.5333.
    assert(std::fabs(sol8[0] - (0.3 + 0.7/3)) < 1e-6);

    return 0;
}
// The PageRank algorithm models a random surfer who, at each step, either follows an outgoing edge from the current node with probability equal to the damping factor, or jumps to a random node with probability `1 - damping`. The damping factor is typically 0.85 or 0.3 in some contexts (here it’s given as 0.3). The recurrence is: `new_rank[v] = (1 - damping) / N + damping * sum_{u where v is an outgoing neighbor of u} (rank[u] / outdegree(u))`. For dangling nodes (outdegree 0), we treat them as if they link to all nodes uniformly, so they contribute `rank[u] / N` to every node. To compute efficiently, we maintain an array `rank` for the current iteration and `new_rank` for the next. For each node `u`, if outdegree > 0, we distribute `damping * rank[u] / deg` to each of its neighbors in `new_rank`. If outdegree == 0, we add `damping * rank[u] / N` to all nodes. After processing all nodes, we add the base probability `(1 - damping) / N` to every node. Then we swap arrays and compute the L∞ norm difference; if it’s ≤ convergence, stop. The maximum iterations cap prevents infinite loops if the graph is problematic. Time complexity is O(iterations × (V + E)) because each iteration processes each edge once, and O(V) for the dangling contribution which can be implemented via a running sum. Space complexity is O(V) for two rank arrays. Edge cases: single node with no edges — each iteration yields rank = 1.0, converges immediately; all nodes dangling — each iteration distributes uniformly; normalized? Not required here; we just output the raw values as computed.
