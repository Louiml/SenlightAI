Given a connected undirected weighted graph where vertices are numbered 1..n and edges may contain duplicate entries between the same pair of vertices, write a C++ function `long long minimumSpanningTreeCost(int n, int multiplier)` that computes the minimum total cost of a spanning tree, then multiplies that MST cost by a given integer `multiplier` and returns the result. The graph is provided through a global constant-size adjacency matrix `COST[1005][1005]` initialized to -1 and adjacency lists `GRAPH[1005]`, both of which are pre-populated before the function is called. If several edges exist between the same two vertices, only the smallest weight should be considered. Vertices are numbered from 1 to n, and the graph is guaranteed connected, so an MST always exists. The function must use Prim's algorithm starting from vertex 1 and return `(sum_of_MST_edges * multiplier)` as a `long long`.

Prim's algorithm grows a spanning tree from an arbitrary starting vertex by repeatedly adding the cheapest edge that connects a visited vertex to an unvisited one. A min-heap priority queue stores candidate edges as `(weight, vertex)` pairs, with the weight as the first element for correct ordering. Starting with vertex 1 and weight 0, we pop the smallest-weight entry. If that vertex is already visited, we skip it; otherwise we add its weight to the cumulative sum, mark it visited, and push all its unvisited neighbors using the minimum edge cost stored in the matrix. Because the input may contain duplicate edges, when building the graph we must keep only the minimum weight per unordered pair; this is already done by the pre-population code. Edge cases include a single-vertex graph (MST cost 0) and negative weights (allowed, but the algorithm still works). The visited bitset prevents cycles and redundant processing. Time complexity is O(E log V) where E is the number of unique edges (after deduplication) because each edge may be pushed to the heap at most twice, and each pop is O(log V). Space complexity is O(V + E) for the adjacency lists and heap, plus O(V) for the visited bitset and O(V^2) for the cost matrix (which is fixed constant size 1005×1005). The multiplication by `multiplier` is done after computing the MST sum, and all arithmetic uses `long long` to avoid overflow.

#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1005;
int COST[MAXN][MAXN];
vector<int> GRAPH[MAXN];

// Computes the total weight of the Minimum Spanning Tree using Prim's algorithm
// from vertex 1, then multiplies that sum by the given multiplier.
// Preconditions: GRAPH and COST are fully populated, graph is connected,
// vertices are numbered 1..n, COST[u][v] = minimum edge weight between u and v,
// and COST[u][v] is -1 if no edge exists.
long long minimumSpanningTreeCost(int n, int multiplier) {
    long long mst_sum = 0;
    bitset<MAXN> visited;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> min_heap;
    min_heap.push({0, 1});

    while (!min_heap.empty()) {
        auto [weight, vertex] = min_heap.top();
        min_heap.pop();

        if (visited[vertex]) {
            continue;
        }

        mst_sum += weight;
        visited[vertex] = true;

        for (int neighbor : GRAPH[vertex]) {
            if (!visited[neighbor]) {
                min_heap.push({COST[vertex][neighbor], neighbor});
            }
        }
    }

    return mst_sum * multiplier;
}

#include<bits/stdc++.h>
using namespace std;

const int MAXN = 1005;
int COST[MAXN][MAXN];
vector<int> GRAPH[MAXN];

long long minimumSpanningTreeCost(int n, int multiplier);

int main() {
    // Reset global structures
    for (int i = 0; i < MAXN; i++) {
        for (int j = 0; j < MAXN; j++) {
            COST[i][j] = -1;
        }
        GRAPH[i].clear();
    }

    // Test 1: Simple triangle, weights 1,2,3. MST = edges 1+2 = 3. Multiplier 2 => 6
    COST[1][2] = 1; COST[2][1] = 1;
    COST[2][3] = 2; COST[3][2] = 2;
    COST[1][3] = 3; COST[3][1] = 3;
    GRAPH[1] = {2,3};
    GRAPH[2] = {1,3};
    GRAPH[3] = {1,2};
    assert(minimumSpanningTreeCost(3, 2) == 6);

    // Reset for next test
    for (int i = 0; i < MAXN; i++) {
        for (int j = 0; j < MAXN; j++) COST[i][j] = -1;
        GRAPH[i].clear();
    }

    // Test 2: Single vertex, no edges. MST cost = 0, multiplier 5 => 0
    assert(minimumSpanningTreeCost(1, 5) == 0);

    // Reset
    for (int i = 0; i < MAXN; i++) {
        for (int j = 0; j < MAXN; j++) COST[i][j] = -1;
        GRAPH[i].clear();
    }

    // Test 3: Two vertices with duplicate edges - should pick min weight
    COST[1][2] = 5; COST[2][1] = 5;
    // Simulate a duplicate with higher weight being added earlier (but we keep min)
    // GRAPH has only one edge
    GRAPH[1] = {2};
    GRAPH[2] = {1};
    // Add a duplicate higher weight directly (should be ignored due to pre-processing)
    // Actually function uses COST, so it picks 5. Multiplier 1 => 5
    assert(minimumSpanningTreeCost(2, 1) == 5);

    // Reset
    for (int i = 0; i < MAXN; i++) {
        for (int j = 0; j < MAXN; j++) COST[i][j] = -1;
        GRAPH[i].clear();
    }

    // Test 4: Line of 4 vertices with weights 1,2,3. MST = 1+2+3=6. Multiplier 1 => 6
    COST[1][2] = 1; COST[2][1] = 1;
    COST[2][3] = 2; COST[3][2] = 2;
    COST[3][4] = 3; COST[4][3] = 3;
    GRAPH[1] = {2};
    GRAPH[2] = {1,3};
    GRAPH[3] = {2,4};
    GRAPH[4] = {3};
    assert(minimumSpanningTreeCost(4, 1) == 6);

    // Reset
    for (int i = 0; i < MAXN; i++) {
        for (int j = 0; j < MAXN; j++) COST[i][j] = -1;
        GRAPH[i].clear();
    }

    // Test 5: Negative weights - graph with edges -5 and -2, MST = -7. Multiplier 3 => -21
    COST[1][2] = -5; COST[2][1] = -5;
    COST[2][3] = -2; COST[3][2] = -2;
    GRAPH[1] = {2};
    GRAPH[2] = {1,3};
    GRAPH[3] = {2};
    assert(minimumSpanningTreeCost(3, 3) == -21);

    // Reset
    for (int i = 0; i < MAXN; i++) {
        for (int j = 0; j < MAXN; j++) COST[i][j] = -1;
        GRAPH[i].clear();
    }

    // Test 6: Larger complete graph of 4 vertices with equal weights 2. MST = 3*2=6. Multiplier 10 => 60
    for (int i = 1; i <= 4; i++) {
        for (int j = i+1; j <= 4; j++) {
            COST[i][j] = 2; COST[j][i] = 2;
            GRAPH[i].push_back(j);
            GRAPH[j].push_back(i);
        }
    }
    assert(minimumSpanningTreeCost(4, 10) == 60);

    cout << "All tests passed!" << endl;
    return 0;
}
