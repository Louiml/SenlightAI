/*
Given an undirected weighted graph with \(n\) vertices and \(m\) edges, three special vertices \(a\), \(b\), and \(c\), and a connectedness guarantee (all three vertices are in the same connected component), write a C++ function `long long shortestTrip(int n, const std::vector<std::vector<Edge>>& adj, int a, int b, int c)` that returns the minimum total edge weight of a walk that starts at one of the three special vertices and visits the other two (in any order, possibly revisiting vertices and edges). The graph is undirected, edge weights are positive, and the input adjacency list is 0-indexed.
*/
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>

struct Edge {
    int to;
    long long weight;
    Edge() = default;
    Edge(int t, long long w) : to(t), weight(w) {}
};

// Compute shortest distances from source using Dijkstra.
std::vector<long long> dijkstraDist(int n, const std::vector<std::vector<Edge>>& adj, int source) {
    const long long INF = std::numeric_limits<long long>::max();
    std::vector<long long> dist(n, INF);
    dist[source] = 0;
    using P = std::pair<long long, int>; // (distance, vertex)
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;
    pq.push({0, source});
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        for (const auto& e : adj[u]) {
            long long nd = d + e.weight;
            if (nd < dist[e.to]) {
                dist[e.to] = nd;
                pq.push({nd, e.to});
            }
        }
    }
    return dist;
}

// Returns the minimum total edge weight of a walk starting from one of {a,b,c}
// and visiting the other two, given an undirected graph with positive weights.
long long shortestTrip(int n, const std::vector<std::vector<Edge>>& adj, int a, int b, int c) {
    std::vector<long long> dA = dijkstraDist(n, adj, a);
    std::vector<long long> dB = dijkstraDist(n, adj, b);
    // dC = dijkstraDist(n, adj, c); but we can derive d(b,c) from dB[c] and d(a,c) from dA[c].
    long long ab = dA[b];
    long long ac = dA[c];
    long long bc = dB[c];

    long long ans = std::min({ab + bc, ac + bc, ab + ac});
    return ans;
}
#include <cassert>

int main() {
    // Example 1: triangle graph 0-1 (2), 1-2 (3), 0-2 (5)
    {
        std::vector<std::vector<Edge>> adj(3);
        adj[0].push_back(Edge(1, 2));
        adj[1].push_back(Edge(0, 2));
        adj[1].push_back(Edge(2, 3));
        adj[2].push_back(Edge(1, 3));
        adj[0].push_back(Edge(2, 5));
        adj[2].push_back(Edge(0, 5));
        // start at 0: 0->1->2 = 2+3=5, start at 1: 1->0->2=2+5=7, start at 2: 2->1->0=3+2=5 => min=5
        assert(shortestTrip(3, adj, 0, 1, 2) == 5);
    }

    // Example 2: larger graph with shortcut
    {
        std::vector<std::vector<Edge>> adj(4);
        adj[0].push_back(Edge(1, 1));
        adj[1].push_back(Edge(0, 1));
        adj[1].push_back(Edge(2, 1));
        adj[2].push_back(Edge(1, 1));
        adj[2].push_back(Edge(3, 1));
        adj[3].push_back(Edge(2, 1));
        adj[0].push_back(Edge(3, 10));
        adj[3].push_back(Edge(0, 10));
        // a=0,b=2,c=3: distances: d(0,2)=2 via 0-1-2, d(0,3)=3 via 0-1-2-3, d(2,3)=1.
        // Options: 0->2->3 = 2+1=3, 0->3->2 = 3+1=4, 2->0->3 = 2+3=5 => min=3
        assert(shortestTrip(4, adj, 0, 2, 3) == 3);
    }

    // Example 3: same vertex appears twice (a == b)
    {
        std::vector<std::vector<Edge>> adj(2);
        adj[0].push_back(Edge(1, 7));
        adj[1].push_back(Edge(0, 7));
        // a=0,b=0,c=1 => need visit 0 and 1, starting from 0, just go to 1: cost 7
        assert(shortestTrip(2, adj, 0, 0, 1) == 7);
    }

    // Example 4: all three connected in a line
    {
        std::vector<std::vector<Edge>> adj(3);
        adj[0].push_back(Edge(1, 4));
        adj[1].push_back(Edge(0, 4));
        adj[1].push_back(Edge(2, 6));
        adj[2].push_back(Edge(1, 6));
        // distances: d(0,1)=4, d(1,2)=6, d(0,2)=10
        // min(4+6=10, 10+6=16, 4+10=14) = 10
        assert(shortestTrip(3, adj, 0, 1, 2) == 10);
    }

    // Example 5: disconnected? The snippet checks connectedness, but our function assumes same component.
    // Still test a valid disconnected scenario where vertices are in different components, 
    // but for safety we just test with all connected.

    return 0;
}
// We need to find the minimum cost path that starts at one of the three vertices \(a,b,c\) and visits the other two. For three vertices, the optimal walk is the sum of the shortest paths between two pairs: the walk will be a "Y" shape where the traveler goes from the start to one intermediate vertex, then to the other. Specifically, if we start at \(a\), the best route is either \(a\to b\to c\) or \(a\to c\to b\), both having cost \(d(a,b)+d(b,c)\) (or \(d(a,c)+d(b,c)\)), but since the graph is undirected, both equal \(d(a,b)+d(b,c)\). Similarly for other starting points. Therefore the answer is the minimum over the three possible pairs of distances:
// - Start at \(a\): cost = \(d(a,b)+d(a,c)\)? Wait, careful: If start at \(a\), visit \(b\) then \(c\), cost = \(d(a,b)+d(b,c)\). If start at \(a\), visit \(c\) then \(b\), cost = \(d(a,c)+d(c,b)=d(a,c)+d(b,c)\). Both are the same sum because \(d(a,c)+d(b,c)\). Actually the two possible sums are: starting at \(a\): \(d(a,b)+d(b,c)\) or \(d(a,c)+d(c,b)\). Since \(d(c,b)=d(b,c)\), both are the same: \(d(a,b)+d(b,c)\)? No, one is \(d(a,b)+d(b,c)\), the other is \(d(a,c)+d(b,c)\). So they differ. The minimum is \(\min(d(a,b)+d(b,c), d(a,c)+d(b,c))\). But we also have starting at \(b\) and \(c\). In total, the possible sums are:
// - Start at \(a\): either \(d(a,b)+d(b,c)\) or \(d(a,c)+d(b,c)\).
// - Start at \(b\): either \(d(b,a)+d(a,c)\) or \(d(b,c)+d(a,c)\).
// - Start at \(c\): either \(d(c,a)+d(a,b)\) or \(d(c,b)+d(b,a)\).
// But since distances are symmetric, these collapse to three distinct sums: \(d(a,b)+d(a,c)\)? Wait, \(d(a,c)+d(b,c)\) appears in start at \(a\) and also start at \(b\) (since \(d(b,c)+d(a,c)\)). Similarly \(d(a,b)+d(a,c)\) appears? Let's list all ordered pairs: The walk is a sequence of three vertices with no repetition (but can pass through others). The total cost is the sum of two shortest-path distances: from start to first, and from first to second. The three possible pairs of edges in the "covered path" among the three vertices are: (a,b) and (b,c), (a,c) and (c,b), (b,a) and (a,c). Since graph is undirected, the cost is one of: \(d(a,b)+d(b,c)\), \(d(a,c)+d(b,c)\), \(d(a,b)+d(a,c)\). So the answer is the minimum of these three sums. Thus we compute all-pairs shortest distances among the three vertices using Dijkstra from each of the three starting points (or from just two, since \(d(b,c)\) can be derived from one of them). Since the graph is undirected and positive weights, Dijkstra works. If the graph is not fully connected, the problem guarantees they are in the same component, so all distances are finite. Edge case: if one of the vertices is the same as another? The problem doesn't specify, but if \(a=b\), then the walk only needs to visit \(c\) and \(a\) (same). But typical problems assume distinct. We'll handle by allowing zero distances; the min formula still works: if \(a=b\), \(d(a,b)=0\), then the min of \(0+d(b,c)\), \(d(a,c)+d(b,c)\), \(0+d(a,c)\) gives \(d(a,c)\) which is correct (start at a, go to c). Time complexity: Running Dijkstra from 3 sources takes \(O(3 \cdot (m \log n)) = O(m \log n)\) time, with \(O(n)\) space. If we want to be safe, we run from two sources and compute \(d(b,c)\) from one of them, but running from three is fine.
