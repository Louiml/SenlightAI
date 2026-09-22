Implement a C++ function that performs the standard Dijkstra algorithm on a directed weighted graph represented by an adjacency matrix, where vertices are labeled with characters and a special sentinel value (10000) indicates no edge. The function should accept the adjacency matrix, the vertex labels, the number of vertices, and a source vertex index. It must return two outputs: an array of shortest distances from the source to every vertex, and a predecessor matrix that records the immediate predecessor of each vertex on the shortest path from the source (0 indicating no predecessor). The function must handle graphs with up to 10 vertices, including disconnected vertices (distance 10000) and self-loops (distance 0). Additionally, the solution must be self-contained, with no reliance on global variables, and must be usable in a standalone program.
#include <cassert>
#include <cstring>

int main() {
    const int N = 5;
    char vertices[N] = {'A', 'B', 'C', 'D', 'E'};
    int c[N][N] = {
        {0, 2, INF, 1, INF},
        {INF, 0, 3, INF, 5},
        {INF, INF, 0, 1, INF},
        {INF, INF, INF, 0, 2},
        {INF, INF, INF, INF, 0}
    };
    int D[N];
    int P[N][N];

    // Test from source 0 (A).
    dijkstra(c, D, 0, vertices, P);
    assert(D[0] == 0);
    assert(D[1] == 2);
    assert(D[2] == 5); // via B: 2+3
    assert(D[3] == 1);
    assert(D[4] == 3); // via D: 1+2
    assert(P[0][0] == 0);
    assert(P[0][1] == 0);
    assert(P[0][2] == 1);
    assert(P[0][3] == 0);
    assert(P[0][4] == 3);

    // Test from source 1 (B).
    dijkstra(c, D, 1, vertices, P);
    assert(D[0] == INF);
    assert(D[1] == 0);
    assert(D[2] == 3);
    assert(D[3] == 4); // via C: 3+1
    assert(D[4] == 5);
    assert(P[1][4] == 3); // B->C->D->E? Actually D[4]=5 from B via C? Let's trace: B->C=3, C->D=1, D->E=2 total=6, but B->E=5 direct, so P[1][4]=1.
    assert(P[1][4] == 1);

    // Test a disconnected vertex: add a vertex F isolated.
    int c2[6][6] = {
        {0, 2, INF, 1, INF, INF},
        {INF, 0, 3, INF, 5, INF},
        {INF, INF, 0, 1, INF, INF},
        {INF, INF, INF, 0, 2, INF},
        {INF, INF, INF, INF, 0, INF},
        {INF, INF, INF, INF, INF, 0}
    };
    int D2[6];
    int P2[6][6];
    char v2[6] = {'A','B','C','D','E','F'};
    dijkstra(c2, D2, 0, v2, P2);
    assert(D2[5] == INF);
    assert(P2[0][5] == 0);

    // Test self-loop: set c[0][0]=5, should remain 0 distance from 0.
    int c3[N][N] = {
        {5, 0, INF, 1, INF},
        {INF, 0, 3, INF, 5},
        {INF, INF, 0, 1, INF},
        {INF, INF, INF, 0, 2},
        {INF, INF, INF, INF, 0}
    };
    int D3[N];
    int P3[N][N];
    dijkstra(c3, D3, 0, vertices, P3);
    assert(D3[0] == 0);
    assert(D3[1] == 0); // direct edge weight 0
    assert(D3[3] == 1);

    return 0;
}
#include <vector>
#include <limits>
#include <algorithm>

const int INF = 10000;
const int MAXN = 10;

// Perform Dijkstra's algorithm on a graph given as an adjacency matrix.
// Parameters:
//   c      - N x N adjacency matrix, c[i][j] = weight or INF if no edge.
//   D      - output array: shortest distance from source to each vertex.
//   s      - source vertex index (0-based).
//   v      - array of vertex labels (not used in algorithm, but for clarity).
//   P      - output N x N matrix: P[s][j] = immediate predecessor on shortest path from s to j (0 if none).
void dijkstra(const int c[][MAXN], int D[MAXN], int s, const char v[], int P[][MAXN]) {
    // Initialize predecessors to 0 (none).
    for (int i = 0; i < MAXN; ++i) {
        P[s][i] = 0;
    }
    // Initialize distances directly from source.
    for (int i = 0; i < MAXN; ++i) {
        D[i] = c[s][i];
    }
    // Track visited vertices.
    bool visited[MAXN] = {false};
    // The source is trivially visited with distance 0.
    visited[s] = true;
    D[s] = 0;

    // Repeat N-1 times (each iteration adds one vertex to the visited set).
    for (int iter = 0; iter < MAXN - 1; ++iter) {
        // Find unvisited vertex with minimal distance.
        int u = -1;
        int minDist = INF;
        for (int i = 0; i < MAXN; ++i) {
            if (!visited[i] && D[i] < minDist) {
                minDist = D[i];
                u = i;
            }
        }
        if (u == -1) {
            break; // All remaining vertices are unreachable.
        }
        visited[u] = true;

        // Relax edges from u.
        for (int j = 0; j < MAXN; ++j) {
            if (!visited[j] && c[u][j] < INF) {
                int newDist = D[u] + c[u][j];
                if (newDist < D[j]) {
                    D[j] = newDist;
                    P[s][j] = u;
                }
            }
        }
    }
}
// The problem is a classic single-source shortest path (SSSP) problem. The graph is given as an `N`-by-`N` integer matrix `c` where `c[i][j]` is the weight of the directed edge from vertex `i` to vertex `j`, or 10000 if no edge exists. The number of vertices is `N` (max 10), and vertices are labeled by characters stored in array `v` (e.g., 'A', 'B', etc.). The source is given by an integer index `s` (0-based). The goal is to compute the shortest distances `D[i]` from source `s` to every vertex `i`, and a predecessor matrix `P` where `P[s][j]` stores the immediate predecessor of vertex `j` on the shortest path from `s` to `j` (0 if none). The algorithm is standard Dijkstra with O(N^2) time since N is small and fixed. We initialize `D[i] = c[s][i]` for all `i`, set `D[s] = 0`, and mark all vertices as unvisited. Repeatedly select the unvisited vertex `u` with the smallest `D[u]`, mark it visited, and for each neighbor `j` of `u` (where `c[u][j] < 10000`), if `D[u] + c[u][j] < D[j]`, update `D[j]` and set `P[s][j] = u`. Edge cases include disconnected vertices (distance remains 10000), self-loops (no effect), and negative weights (not handled, as Dijkstra assumes non-negative weights, but the input specification has positive weights as indicated). Time complexity is O(N^2) and space complexity is O(N^2) for the predecessor matrix, which is acceptable for N ≤ 10.
