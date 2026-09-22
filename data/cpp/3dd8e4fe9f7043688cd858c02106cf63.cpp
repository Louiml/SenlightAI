A group of `n` stones is placed on a line at integer coordinates, sorted in strictly increasing order. The first stone is at position 0 and the last stone is at position `d`. Each stone is either fragile (type `'S'`) or sturdy (type `'B'`). You need to cross from position 0 to position `d` along the line by jumping between stones. You can jump from any stone to another stone if the distance between their positions does not exceed a given maximum jump length `L`. However, fragile stones may be used at most once in each direction (so at most one jump landing on it and one jump leaving it total), while sturdy stones can be used any number of times (including repeated jumps onto and off of them). You must make exactly two complete journeys from 0 to `d` (i.e., you must traverse the line twice, each time from start to end), but you may reuse stones and jumps as long as capacity constraints are satisfied. Determine the minimum possible maximum jump length `L` such that two such journeys are possible. The input consists of: an integer `n`, an integer `d` (the destination coordinate), and for each stone (in increasing coordinate order) a string of the form `"X-value"` where `X` is `'B'` for sturdy or `'S'` for fragile, and `value` is the stone's coordinate (with the first stone always at coordinate 0 and the last at coordinate `d`). Write a C++ function `int minimumMaxJump(int n, int d, const std::vector<std::string>& stones)` that returns the minimal possible `L` (an integer, as all coordinates are integers) that allows two complete crossings. You may assume that at least two crossings are always possible for `L = d` (since the first stone is at 0 and the last at `d`), and the stone list is valid with the first coordinate 0 and last coordinate `d`.

#include <cassert>
#include <vector>
#include <string>

// Declare the function from the solution
int minimumMaxJump(int n, int d, const std::vector<std::string>& stones);

int main() {
    // Test 1: Basic example with two sturdy stones.
    std::vector<std::string> stones1 = {"B-0", "B-5", "B-10"};
    assert(minimumMaxJump(3, 10, stones1) == 5);

    // Test 2: All fragile, need L=10 because fragile can only be used once.
    std::vector<std::string> stones2 = {"S-0", "S-5", "S-10"};
    assert(minimumMaxJump(3, 10, stones2) == 10);

    // Test 3: Only two stones (start and end), both sturdy.
    std::vector<std::string> stones3 = {"B-0", "B-10"};
    assert(minimumMaxJump(2, 10, stones3) == 10);

    // Test 4: Stones spaced such that L=2 works with sturdy middle stone.
    std::vector<std::string> stones4 = {"B-0", "B-2", "B-4", "B-6", "B-8", "B-10"};
    assert(minimumMaxJump(6, 10, stones4) == 2);

    // Test 5: Fragile middle stone forces L=4.
    std::vector<std::string> stones5 = {"B-0", "S-2", "B-4", "B-6", "B-8", "B-10"};
    assert(minimumMaxJump(6, 10, stones5) == 4);

    // Test 6: Fragile stones at both ends of a gap.
    std::vector<std::string> stones6 = {"B-0", "S-1", "S-2", "S-3", "B-10"};
    assert(minimumMaxJump(5, 10, stones6) == 8);

    // Test 7: Very small d with two stones.
    std::vector<std::string> stones7 = {"B-0", "B-3"};
    assert(minimumMaxJump(2, 3, stones7) == 3);

    // Test 8: Large number of stones, L=1.
    std::vector<std::string> stones8;
    for (int i = 0; i <= 10; ++i) {
        stones8.push_back("B-" + std::to_string(i));
    }
    assert(minimumMaxJump(11, 10, stones8) == 1);

    // Test 9: One fragile stone at position 5, all others sturdy.
    std::vector<std::string> stones9 = {"B-0", "B-2", "B-4", "S-5", "B-7", "B-9", "B-10"};
    assert(minimumMaxJump(7, 10, stones9) == 3);

    // Test 10: All fragile, equally spaced, need L = distance between consecutive stones.
    std::vector<std::string> stones10 = {"S-0", "S-2", "S-4", "S-6", "S-8", "S-10"};
    assert(minimumMaxJump(6, 10, stones10) == 10);

    return 0;
}

#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include <climits>

// Dinic's max flow implementation for the network.
class Dinic {
public:
    Dinic(int n) : N(n), graph(n), cap(n, std::vector<int>(n, 0)) {}
    
    void addEdge(int u, int v, int c) {
        graph[u].push_back(v);
        graph[v].push_back(u);
        cap[u][v] += c;
    }
    
    int maxFlow(int s, int t) {
        int flow = 0;
        while (bfs(s, t)) {
            std::vector<int> ptr(N, 0);
            while (int pushed = dfs(s, t, INT_MAX, ptr)) {
                flow += pushed;
            }
        }
        return flow;
    }

private:
    int N;
    std::vector<std::vector<int>> graph;
    std::vector<std::vector<int>> cap;
    std::vector<int> level;
    
    bool bfs(int s, int t) {
        level.assign(N, -1);
        std::vector<int> q;
        q.push_back(s);
        level[s] = 0;
        for (int i = 0; i < (int)q.size(); ++i) {
            int u = q[i];
            for (int v : graph[u]) {
                if (level[v] == -1 && cap[u][v] > 0) {
                    level[v] = level[u] + 1;
                    q.push_back(v);
                }
            }
        }
        return level[t] != -1;
    }
    
    int dfs(int u, int t, int f, std::vector<int>& ptr) {
        if (u == t) return f;
        for (int& i = ptr[u]; i < (int)graph[u].size(); ++i) {
            int v = graph[u][i];
            if (level[v] == level[u] + 1 && cap[u][v] > 0) {
                int pushed = dfs(v, t, std::min(f, cap[u][v]), ptr);
                if (pushed > 0) {
                    cap[u][v] -= pushed;
                    cap[v][u] += pushed;
                    return pushed;
                }
            }
        }
        return 0;
    }
};

// Returns the minimal maximum jump length L that allows two crossings.
int minimumMaxJump(int n, int d, const std::vector<std::string>& stones) {
    std::vector<char> type(n);
    std::vector<int> pos(n);
    for (int i = 0; i < n; ++i) {
        size_t pos_hyphen = stones[i].find('-');
        type[i] = stones[i][0];
        pos[i] = std::stoi(stones[i].substr(pos_hyphen + 1));
    }
    
    int low = 0, high = d, ans = d;
    while (low <= high) {
        int mid = (low + high) / 2;
        
        // Build flow network for current L = mid
        int source = 0;
        int sink = 2 * n + 1;
        Dinic dinic(2 * n + 2);
        
        // Stone split edges: capacity = 2 for sturdy, 1 for fragile
        for (int i = 0; i < n; ++i) {
            int in_node = i + 1;
            int out_node = i + n + 1;
            int cap_stone = (type[i] == 'B') ? 2 : 1;
            dinic.addEdge(in_node, out_node, cap_stone);
        }
        
        // Edges between stones in increasing coordinate order
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                if (pos[j] - pos[i] <= mid) {
                    int out_i = i + n + 1;
                    int in_j = j + 1;
                    dinic.addEdge(out_i, in_j, 2);
                }
            }
        }
        
        // Source to stones that are reachable from start
        for (int i = 0; i < n; ++i) {
            if (pos[i] <= mid) {
                dinic.addEdge(source, i + 1, 2);
            }
        }
        
        // Stones to sink if they can reach the destination
        for (int i = 0; i < n; ++i) {
            if (d - pos[i] <= mid) {
                int out_i = i + n + 1;
                dinic.addEdge(out_i, sink, 2);
            }
        }
        
        int flow = dinic.maxFlow(source, sink);
        if (flow >= 2) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return ans;
}

// This is a classic maximum-flow with binary search problem. Build a flow network where each stone is split into an "in" node and an "out" node connected by an edge with capacity equal to the number of times that stone can be used as an intermediate vertex: sturdy stones (`'B'`) have capacity 2 (because you can pass through them twice, once per journey), fragile stones (`'S'`) have capacity 1 (because you can pass through them only once total across both journeys). The source is a virtual node representing position 0, and the sink is a virtual node representing position `d`. For a given maximum jump length `L`, we add edges: from source to every stone whose coordinate ≤ `L` (with capacity 2, since the source can be used repeatedly), from every stone to sink if `d - coordinate ≤ L` (capacity 2), and between every pair of stones `i < j` such that `coordinate[j] - coordinate[i] ≤ L` (capacity 2, because you may jump between them multiple times across the two journeys; edges are directed from the lower-coordinate stone's out node to the higher-coordinate stone's in node, with reverse edges added for the residual graph). The capacity of each directed edge is 2, allowing up to two uses per journey pair. Then run a max-flow algorithm (e.g., Dinic's) to compute the maximum number of paths from source to sink. If the max flow is at least 2, then two journeys are possible for that `L`. Since `L` is monotonic (if `L` works, any larger `L` works), we binary search the minimal `L` in `[0, d]`. Complexity: The number of nodes is `2n + 2`, and edges are `O(n^2)` per build (since we connect every pair of stones). For each binary search step (about `log d` steps), running Dinic's takes `O(E * V^2)` in the worst case, but for such small constraints (n typically ≤ 200) it is fast. Edge cases: The first stone is at 0 and the last at `d`, so there is always a path for `L = d`. Also, ensure that the source and sink edges are correctly set so that the flow can start and finish. The stones are given in increasing coordinate order, so we can directly compare adjacent and non-adjacent pairs without sorting.
