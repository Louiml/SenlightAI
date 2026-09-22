Write a C++ function named `kruskalMST` that reads a weighted undirected graph from a file named "source.txt" and returns a `std::vector<Edge>` representing the edges of a Minimum Spanning Tree (MST) found using Kruskal's algorithm. The input file format is: the first integer is the number of vertices (order), the second integer is the number of edges, followed by triples of integers each representing an edge as `u v weight` (vertices are 0-indexed). The function should use a disjoint-set (union-find) data structure with path compression and union by rank to efficiently determine whether adding an edge creates a cycle. The returned vector should contain the edges of the MST in the order they were selected, where each edge stores its two endpoints and weight. If the graph is disconnected, return the MST of the connected components (a spanning forest). The function must handle edge cases like empty files, invalid vertex indices, and duplicate edges, but assume the input is well-formed for the given format. The function should not modify any global state and must be self-contained.

// The solution involves reading the file, storing all edges in a vector, sorting them by weight in ascending order, and then applying Kruskal's greedy algorithm. For each edge in sorted order, check if its endpoints belong to different components using a union-find data structure. If they are in different components, include the edge in the MST and union the components. This guarantees that the included edges form a forest with no cycles and minimum total weight. The union-find structure uses parent arrays and rank arrays; path compression flattens the tree, and union by rank keeps trees shallow, achieving near-constant amortized time per operation. After processing all edges, the vector of selected edges is returned. Edge cases: if the file cannot be opened, return an empty vector. If the graph has fewer than 2 vertices, the MST is empty. If the graph is disconnected, the algorithm naturally produces a minimum spanning forest. Time complexity is O(E log E) due to sorting (where E is the number of edges), and the union-find operations are O(α(V)) amortized, where α is the inverse Ackermann function. Space complexity is O(V + E) for storing the union-find structures and edges.

#include <fstream>
#include <vector>
#include <algorithm>
#include <stdexcept>

struct Edge {
    int u;
    int v;
    int weight;
    Edge(int from, int to, int w) : u(from), v(to), weight(w) {}
};

// Disjoint-set (union-find) with path compression and union by rank
class DisjointSet {
public:
    explicit DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) parent[rx] = ry;
        else if (rank[rx] > rank[ry]) parent[ry] = rx;
        else { parent[ry] = rx; rank[rx]++; }
    }

private:
    std::vector<int> parent;
    std::vector<int> rank;
};

// Reads graph from "source.txt" and returns MST edges via Kruskal's algorithm
std::vector<Edge> kruskalMST() {
    std::ifstream file("source.txt");
    if (!file.is_open()) return {};

    int order, numEdges;
    if (!(file >> order >> numEdges)) return {};
    if (order < 2 || numEdges < 0) return {};

    std::vector<Edge> edges;
    int u, v, w;
    for (int i = 0; i < numEdges; ++i) {
        if (!(file >> u >> v >> w)) break;
        if (u < 0 || u >= order || v < 0 || v >= order) continue;
        edges.emplace_back(u, v, w);
    }

    std::sort(edges.begin(), edges.end(), 
              [](const Edge& a, const Edge& b) { return a.weight < b.weight; });

    DisjointSet ds(order);
    std::vector<Edge> mst;
    for (const auto& e : edges) {
        if (ds.find(e.u) != ds.find(e.v)) {
            ds.unite(e.u, e.v);
            mst.push_back(e);
        }
    }
    return mst;
}

#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>

// The solution function is assumed to be included above.
// Test harness writes temporary files and checks correctness.

bool areEdgesEqual(const std::vector<Edge>& a, const std::vector<Edge>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i) {
        if (a[i].u != b[i].u || a[i].v != b[i].v || a[i].weight != b[i].weight) return false;
    }
    return true;
}

int main() {
    // Test 1: Basic connected graph (3 vertices, 3 edges)
    {
        std::ofstream f("source.txt");
        f << "3 3\n0 1 1\n1 2 2\n0 2 3\n";
        f.close();
        std::vector<Edge> result = kruskalMST();
        std::vector<Edge> expected = {Edge(0,1,1), Edge(1,2,2)};
        assert(areEdgesEqual(result, expected));
    }

    // Test 2: Disconnected graph (forest)
    {
        std::ofstream f("source.txt");
        f << "4 2\n0 1 5\n2 3 7\n";
        f.close();
        std::vector<Edge> result = kruskalMST();
        std::vector<Edge> expected = {Edge(0,1,5), Edge(2,3,7)};
        assert(areEdgesEqual(result, expected));
    }

    // Test 3: Duplicate weights, ensure correct ordering
    {
        std::ofstream f("source.txt");
        f << "4 4\n0 1 10\n1 2 10\n2 3 10\n0 3 10\n";
        f.close();
        std::vector<Edge> result = kruskalMST();
        // Any 3 edges form an MST; we only check size and total weight
        assert(result.size() == 3);
        int total = 0;
        for (const auto& e : result) total += e.weight;
        assert(total == 30);
    }

    // Test 4: Empty graph (single vertex, no edges)
    {
        std::ofstream f("source.txt");
        f << "1 0\n";
        f.close();
        std::vector<Edge> result = kruskalMST();
        assert(result.empty());
    }

    // Test 5: File missing
    {
        remove("source.txt");
        std::vector<Edge> result = kruskalMST();
        assert(result.empty());
    }

    // Test 6: Graph with 5 vertices, cycle avoidance check
    {
        std::ofstream f("source.txt");
        f << "5 5\n0 1 1\n1 2 2\n2 0 3\n2 3 4\n3 4 5\n";
        f.close();
        std::vector<Edge> result = kruskalMST();
        assert(result.size() == 4); // 5 vertices => 4 edges
        int total = 0;
        for (const auto& e : result) total += e.weight;
        assert(total == 1+2+4+5); // skip the 3-weight edge creating a cycle
    }

    return 0;
}
