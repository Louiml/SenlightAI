/*
You are given a complete weighted graph on `n` vertices (numbered `1` to `n`) plus a special source vertex `0`. The graph has `m` undirected edges given by user input, with possible multiple edges between the same pair of vertices (keep only the minimum weight). You need to compute the minimum total cost of a set of `k` vertex-disjoint paths that start at vertex `0` and end at distinct vertices from `{1,...,n}`, such that every vertex from `{1,...,n}` is used exactly once as an endpoint, and each path is a simple path that may pass through intermediate vertices (including possibly vertex `0` again? Actually, in the original problem, paths start at vertex `0` and go to distinct destination vertices, and all vertices except `0` must be used exactly once as an endpoint; the paths are vertex-disjoint except for sharing vertex `0`, and they may visit vertex `0` multiple times? The original code uses the Floyd-Warshall shortest path distances to set edge weights, and then solves a maximum-weight bipartite matching on a complete bipartite graph between the `n` destinations and `k` "path slots". The answer is the negative of the maximum matching weight. So the task is: Given `n`, `m`, `k` (with `k` ≤ `n`), and graph edges with positive lengths, compute the minimum possible total length of `k` vertex-disjoint (except at `0`) paths from `0` to cover all `n` vertices? Actually, the code creates `k` extra "slots" and matches each slot to a destination vertex, with edge weight `-dist(0, v)`. Then it computes maximum weight matching between the `n` original vertices and the `k` slots plus the `n` original vertices? Let's analyze carefully: It sets `num = n` initially, creates a complete bipartite graph on the `n` original vertices (indices 0..n-1) with weights `-dist(i+1, j+1)` for `i<j`. Then for each of the `k` extra slots (indices `num` to `num+k-1`), it adds edges from that slot to every original vertex with weight `-dist(0, v)`. Then it runs the KM algorithm for maximum weight matching on a complete bipartite graph of size `num+k` (but wait, it only adds `k` vertices, making the graph `n+k` by `n+k`? Actually, it increments `num` by 1 for each of `k` iterations, but inside the loop it sets edges from the new vertex to all `n` original vertices, but it also sets edges from original vertices to the new vertex? The code does `mat[num-1][i-1] = mat[i-1][num-1] = -c[0][i]`, so it creates a symmetric edge. But the KM function expects a square matrix of size `num` by `num`. So after `k` iterations, `num = n+k`. The matrix has entries for all pairs, including between two extra slots (which remain -0x80808080, i.e., very negative), and between an original vertex and an extra slot. The matching will match each extra slot to some original vertex, and the remaining original vertices will match among themselves (since the graph is complete but weights are negative distances, it will try to maximize the sum, which means minimize negative distances). The final answer is `-maxWeight`, which equals the minimum total length of `k` paths. This is essentially solving a minimum-weight perfect matching on a complete graph of size `n+k` where each extra vertex has weight `-dist(0, v)` to original vertices and `-inf` (so they must match to original vertices) and original vertices have weight `-dist(i,j)` between each other. The result corresponds to partitioning the `n` original vertices into `k` groups (each group assigned to one extra slot) and then within each group forming a path (or actually just connecting to the source). The minimum total length is the sum of the shortest distances from 0 to the chosen "endpoints"? Actually the original problem seems to be a known problem: "The Traveling Salesman Problem with multiple salesmen" or "k-route" problem, where you want to cover all cities using `k` routes starting from a depot. The solution here uses a combination of Floyd-Warshall to get all-pairs shortest paths, then constructs a complete bipartite graph where the edge weight is the negative shortest path distance, and finds a maximum weight perfect matching. This yields the minimum total length of `k` vertex-disjoint Hamiltonian paths starting from the depot. So the task: implement a function that takes `n`, `m`, `k`, and the list of edges, computes all-pairs shortest paths (using Floyd-Warshall), then builds a matrix for KM and returns the minimum total cost as described. The function should handle the case where the graph may be disconnected? But we assume it's strongly connected? The original code uses `inf = 0x3fffffff` and initializes `c` with that, then runs Floyd-Warshall; if there is no path, the distance remains `inf`. But then the matching weights become `-inf`, which might cause issues. We'll assume the graph is connected enough that all-pairs distances are finite. For the independent task, we'll specify that the graph is guaranteed to have a path between every pair of vertices (i.e., connected). Also note that the original code has `k` from input, and it uses `k` extra slots, so `k` must be ≤ `n`. The output is the minimum total length of `k` paths. So the task: Write a C++ function `int minimumPathCoverCost(int n, int m, int k, const vector<tuple<int,int,int>>& edges)` that returns the minimum total length.
*/
#include <bits/stdc++.h>
using namespace std;

// Hungarian / KM for maximum weight perfect matching.
// Returns the maximum total weight. Matrix w is N x N, with negative weights allowed.
struct HungarianMax {
    int n;
    vector<vector<int>> w;
    vector<int> matchL, matchR, lx, ly;
    vector<int> slack, slackx, pre;
    vector<bool> visL, visR;

    HungarianMax(const vector<vector<int>>& mat) : n(mat.size()), w(mat) {
        matchL.assign(n, -1);
        matchR.assign(n, -1);
        lx.assign(n, 0);
        ly.assign(n, 0);
        slack.assign(n, 0);
        slackx.assign(n, 0);
        pre.assign(n, -1);
        visL.assign(n, false);
        visR.assign(n, false);
        // Initialize labels
        for (int i = 0; i < n; ++i) {
            lx[i] = w[i][0];
            for (int j = 1; j < n; ++j)
                lx[i] = max(lx[i], w[i][j]);
        }
    }

    void bfs(int root) {
        fill(visL.begin(), visL.end(), false);
        fill(visR.begin(), visR.end(), false);
        fill(slack.begin(), slack.end(), INT_MAX);
        fill(slackx.begin(), slackx.end(), -1);
        fill(pre.begin(), pre.end(), -1);
        queue<int> q;
        q.push(root);
        visL[root] = true;
        bool found = false;
        while (!q.empty() && !found) {
            int u = q.front(); q.pop();
            for (int v = 0; v < n && !found; ++v) {
                if (visR[v]) continue;
                int diff = lx[u] + ly[v] - w[u][v];
                if (diff < slack[v]) {
                    slack[v] = diff;
                    slackx[v] = u;
                }
                if (slack[v] == 0) {
                    visR[v] = true;
                    if (matchR[v] == -1) {
                        // Augment
                        int cur = v;
                        while (cur != -1) {
                            int pu = slackx[cur];
                            int pv = matchL[pu];
                            matchL[pu] = cur;
                            matchR[cur] = pu;
                            cur = pv;
                        }
                        found = true;
                        break;
                    } else {
                        int nx = matchR[v];
                        if (!visL[nx]) {
                            visL[nx] = true;
                            q.push(nx);
                        }
                    }
                }
            }
            if (!found) {
                int delta = INT_MAX;
                for (int v = 0; v < n; ++v)
                    if (!visR[v]) delta = min(delta, slack[v]);
                for (int i = 0; i < n; ++i) {
                    if (visL[i]) lx[i] -= delta;
                    if (visR[i]) ly[i] += delta;
                    else slack[i] -= delta;
                }
            }
        }
    }

    int solve() {
        for (int i = 0; i < n; ++i) {
            // Initial DFS from i
            fill(visL.begin(), visL.end(), false);
            fill(visR.begin(), visR.end(), false);
            // We need to augment from row i. Use the BFS above.
            bfs(i);
        }
        int total = 0;
        for (int i = 0; i < n; ++i)
            if (matchL[i] != -1)
                total += w[i][matchL[i]];
        return total;
    }
};

// Compute the minimum total length of k vertex-disjoint paths
// starting from vertex 0 and covering all vertices 1..n exactly once.
int minimumPathCoverCost(int n, int m, int k, const vector<tuple<int,int,int>>& edges) {
    const int N_VERT = n + 1;
    const int INF = 1e9;

    vector<vector<int>> dist(N_VERT, vector<int>(N_VERT, INF));
    for (int i = 0; i < N_VERT; ++i) dist[i][i] = 0;
    for (const auto& [u, v, len] : edges) {
        dist[u][v] = min(dist[u][v], len);
        dist[v][u] = min(dist[v][u], len);
    }
    for (int mid = 0; mid < N_VERT; ++mid)
        for (int i = 0; i < N_VERT; ++i)
            for (int j = 0; j < N_VERT; ++j)
                if (dist[i][mid] < INF && dist[mid][j] < INF)
                    dist[i][j] = min(dist[i][j], dist[i][mid] + dist[mid][j]);

    int N = n + k;
    vector<vector<int>> w(N, vector<int>(N, -INF));
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            if (i != j) w[i-1][j-1] = -dist[i][j];
    for (int s = n; s < N; ++s)
        for (int v = 1; v <= n; ++v) {
            w[s][v-1] = -dist[0][v];
            w[v-1][s] = -dist[0][v];
        }

    HungarianMax km(w);
    int maxWeight = km.solve();
    return -maxWeight;
}
#include <cassert>
#include <vector>
#include <tuple>
using namespace std;

int minimumPathCoverCost(int n, int m, int k, const vector<tuple<int,int,int>>& edges);

int main() {
    // Simple triangle: 0-1 length 2, 1-2 length 3, 0-2 length 5.
    // n=2, k=1: need one path covering both vertices, min total = 0->1->2 = 5? Actually shortest path 0-2 is 5, but via 1 is 2+3=5, so total 5.
    {
        vector<tuple<int,int,int>> edges = { {0,1,2}, {1,2,3}, {0,2,5} };
        assert(minimumPathCoverCost(2, 3, 1, edges) == 5);
    }

    // n=4, k=2: two paths, all vertices covered.
    // Edges form a line 0-1-2-3-4, all lengths 1.
    // Best: two paths: 0-1-2 and 0-3-4 (dist 0-3=3, but 0-3 via 1,2 is 3, total 1+1 + 3=5? Actually path 0-1-2 length 2, path 0-3-4 length 3, total 5.
    {
        vector<tuple<int,int,int>> edges = { {0,1,1}, {1,2,1}, {2,3,1}, {3,4,1} };
        assert(minimumPathCoverCost(4, 4, 2, edges) == 5);
    }

    // n=3, k=3: each vertex gets its own path, total = dist(0,v) sum.
    // Distances: 0-1:1, 0-2:2, 0-3:3 -> total 6.
    {
        vector<tuple<int,int,int>> edges = { {0,1,1}, {0,2,2}, {0,3,3} };
        assert(minimumPathCoverCost(3, 3, 3, edges) == 6);
    }

    // n=2, k=2: two separate paths.
    // Dist 0-1:10, 0-2:20 -> total 30.
    {
        vector<tuple<int,int,int>> edges = { {0,1,10}, {0,2,20} };
        assert(minimumPathCoverCost(2, 2, 2, edges) == 30);
    }

    // Duplicate edges: keep min.
    // n=1, k=1: single vertex, only need path 0-1. Edges: 0-1 length 5 and 3, use 3.
    {
        vector<tuple<int,int,int>> edges = { {0,1,5}, {0,1,3} };
        assert(minimumPathCoverCost(1, 2, 1, edges) == 3);
    }

    // Disconnected graph? We assume connected. But test a small one where n=2, k=1, only path via direct edge length 7.
    {
        vector<tuple<int,int,int>> edges = { {0,1,7}, {0,2,0}, {1,2,0} }; // zero length edges? Not allowed, but we'll use positive.
    }
    // Use a proper test: n=2, k=1, triangle with edges 0-1=1, 1-2=2, 0-2=10. Min = 1+2=3.
    {
        vector<tuple<int,int,int>> edges = { {0,1,1}, {1,2,2}, {0,2,10} };
        assert(minimumPathCoverCost(2, 3, 1, edges) == 3);
    }

    // n=4, k=2, star from 0 to all vertices: 0-1=1,0-2=2,0-3=3,0-4=4. No edges between others.
    // Two paths: best pair 1,2 (1+2=3) and 3,4 (3+4=7) total 10. Or 1,3 (4) + 2,4 (6) =10. So min 10.
    {
        vector<tuple<int,int,int>> edges = { {0,1,1}, {0,2,2}, {0,3,3}, {0,4,4} };
        assert(minimumPathCoverCost(4, 4, 2, edges) == 10);
    }

    return 0;
}
// The problem can be reduced to a minimum-weight perfect matching on a complete graph of size `n + k`. First, compute the all-pairs shortest path distances `dist[u][v]` for vertices `0..n` using Floyd-Warshall on `n+1` vertices (including the source `0`), with edge weights being the given lengths, keeping minimum for parallel edges. Then we create a weight matrix `w` of size `N = n + k`, where:
// - For `1 <= i <= n` and `1 <= j <= n`, `i != j`, set `w[i-1][j-1] = -dist[i][j]` (shortest distance between vertices `i` and `j`).
// - For each extra slot `s = n, n+1, ..., n+k-1` (0-indexed row/col index), and each original vertex `v` (1..n), set `w[s][v-1] = w[v-1][s] = -dist[0][v]`.
// - All other entries (between two extra slots) are set to a very negative number (e.g., `-1e9`) so that they will never be matched to each other in a maximum weight perfect matching.
//
// Then we compute a maximum weight perfect matching on this complete undirected graph (or equivalently, maximum weight matching on a complete bipartite graph with identical left and right copies, which is what the KM minimum-cost maximum bipartite matching does). The maximum sum of weights is `-minimum total length`. So the answer is the negative of that maximum matching weight. This works because a perfect matching partitions the `N` vertices into `N/2` pairs. Each extra slot must be paired with some original vertex, meaning that vertex is an endpoint of a path starting at the depot. The remaining original vertices are paired among themselves, representing connections between cities that lie on the same path (the path goes from depot to a vertex, then along matched edges, but careful: each matched edge between two original vertices represents the path segment covering those two vertices consecutively. Since we have `k` extra slots, we get `k` path segments starting at the depot; each path is a chain of original vertices. The total length of a path is the sum of the distances along the chain plus the distance from depot to the first vertex. Because the matching is perfect, the original vertices form a disjoint union of chains ending at the depot slots. The total weight sum is `- (sum of all edge lengths used)`, and maximizing the weight sum minimizes the total length. Edge cases: `k` can be up to `n`. If `k = n`, then each extra slot pairs with a unique original vertex, and the total cost is just the sum of `dist[0][v]` over all vertices (since no original-original edges are used). The algorithm handles that. Also note that the matching weight matrix is symmetric, and the KM algorithm for bipartite matching requires two copies of the same set; here we treat the matrix as the cost between left part and right part, both of size `N`. Since the graph is complete (with some very negative entries for forbidden pairs), a perfect matching always exists as long as `N` is even. But `N = n + k` could be odd if `n` and `k` have opposite parity. The original code does not check parity, but for a perfect matching to exist, `N` must be even. However, the original problem likely always has `n` even? Actually, the original code does not handle odd `N`, and the KM implementation assumes a square matrix and always matches all vertices. If `N` is odd, it will still compute something, but it may be invalid. We must assume that `n + k` is even or we handle it by adding a dummy? But the original problem presumably always has `n + k` even because the number of vertices in the matching is `n` (original) plus `k` (slots) and we pair them all, so `n + k` must be even for a perfect matching. Since `k` is the number of paths, and the number of endpoints is `k` (from source) plus `n` (the original vertices), but actually each path has two ends: one at source (shared) and one at some original vertex? Wait, the matching model creates a perfect matching between `n+k` nodes, meaning each node is paired with exactly one other. The extra slots represent the "source" side of each path. There are `k` source ends, and `n` city ends. So total ends = `n + k`. In a set of undirected paths, each path has 2 ends. But here we treat each path as starting at the source and ending at a city, so we have one source end and one city end per path, but the source is shared across all paths, so we count it `k` times (once per path). So total ends = `n + k`. For a perfect matching to make sense, we need `n + k` even, because we pair ends. However, each path has 2 endpoints, so the total number of endpoints must be even. So `n + k` must be even. The original code does not enforce this; if it's odd, the KM will still run but the matching is not a perfect matching of a valid set of paths. The problem statement should guarantee that `n + k` is even. In practice, the original problem likely has `n` even and `k` arbitrary? Or perhaps the problem is for a different interpretation. To be safe, we assume the input is given such that `n + k` is even (e.g., this holds when `n` and `k` have the same parity). But the task can simply say that the input guarantees that a valid solution exists, and we can ignore the parity issue. In our reference solution, we will implement a standard Kuhn-Munkres (Hungarian) algorithm for maximum weight perfect matching on a square matrix. We'll handle the case `n + k` even. If it's odd, we could add a dummy vertex with zero weight edges to make it even, but the original problem doesn't do that. We'll keep it simple: assume `n + k` is even (which is mathematically required for a perfect matching of ends). Time complexity: Floyd-Warshall takes O((n+1)^3), and the Hungarian algorithm takes O(N^3) where N = n + k. So overall O(n^3) (since k ≤ n, N ≤ 2n). Space O(n^2). Edge cases: multiple edges between same pair (take minimum), possibly self-loops (ignore). The graph is connected so all-pairs shortest paths exist. Also note that the original code uses `mat` initialized with `0x80` (very negative) and then sets symmetric edges. We'll replicate that with a large negative constant (e.g., `-1e9`). The answer is the negative of the maximum matching sum, which is the minimum total path length.
