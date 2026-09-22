/*
Given a rooted tree with \( n \) nodes (rooted at 1), where each node \( i \) (for \( i \ge 2 \)) has a parent \( fa[i] \), an edge weight \( w_i \) from parent to \( i \), and three parameters: \( p_i \), \( q_i \), and a maximum distance limit \( lim_i \). The distance \( dis[i] \) from root to node \( i \) is the sum of edge weights along the path. For each node \( i \ge 2 \), define a cost function: you may choose any ancestor \( j \) of \( i \) (including itself? no, must be an ancestor strictly above, i.e., \( j \) is on the path from root to \( i \) excluding \( i \), but the original code includes \( j \) as any ancestor including maybe root) that satisfies \( dis[i] - dis[j] \le lim[i] \). The cost to reach \( i \) via \( j \) is \( f[j] + p[i] \cdot (dis[i] - dis[j]) + q[i] \). Compute the minimal possible cost \( f[i] \) for every node \( i \ge 2 \), given \( f[1] = 0 \). Write a C++ function `std::vector<long long> computeMinCosts(const std::vector<std::vector<std::pair<int,long long>>>& tree, const std::vector<long long>& p, const std::vector<long long>& q, const std::vector<long long>& lim)` that returns a vector of size \( n+1 \) (index 1..n) with the minimal costs for all nodes (index 1 has 0). The tree edges are given as adjacency list where `tree[u]` contains pairs `(v, w)` meaning an edge from u to v with weight w. All edge weights are positive. \( n \le 2 \times 10^5 \). The values of \( p, q, lim \) can be any 64-bit integers (positive, zero, or negative) except that lim is non-negative for each node. The solution must be efficient enough for large n.
*/

#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

using ll = long long;
const ll INF = 4e18;

struct Line {
    ll m, b; // y = m*x + b
    Line(ll _m = 0, ll _b = INF) : m(_m), b(_b) {}
    ll eval(ll x) const { return m * x + b; }
};

struct ConvexHull {
    // Lower hull for minimum queries with monotonically decreasing slopes
    std::vector<Line> hull;
    std::vector<long double> breaks; // breakpoints between lines (x-coordinate where next line becomes better)
    
    void clear() { hull.clear(); breaks.clear(); }
    
    // Check if line l3 is unnecessary (intersection(l1,l3) <= intersection(l1,l2))
    bool bad(const Line& l1, const Line& l2, const Line& l3) {
        // (l2.b - l1.b) / (l1.m - l2.m) >= (l3.b - l1.b) / (l1.m - l3.m)
        // Careful with signs: slopes are negative and decreasing => m1 < m2 < m3
        // Using long double to avoid overflow
        long double left = (long double)(l2.b - l1.b) / (l1.m - l2.m);
        long double right = (long double)(l3.b - l1.b) / (l1.m - l3.m);
        return left >= right;
    }
    
    void add(Line l) {
        if (hull.empty()) {
            hull.push_back(l);
            breaks.push_back(-1e18L); // dummy
            return;
        }
        if (hull.back().m == l.m) {
            // same slope: keep the one with smaller intercept (lower y)
            if (l.b >= hull.back().b) return;
            hull.pop_back();
            if (!hull.empty()) breaks.pop_back();
            else { hull.push_back(l); breaks.push_back(-1e18L); return; }
        }
        while (hull.size() >= 2 && bad(hull[hull.size()-2], hull.back(), l)) {
            hull.pop_back();
            breaks.pop_back();
        }
        long double inter = (long double)(l.b - hull.back().b) / (hull.back().m - l.m);
        breaks.push_back(inter);
        hull.push_back(l);
    }
    
    // Query minimum at x
    ll query(ll x) {
        if (hull.empty()) return INF;
        // binary search for the last breakpoint <= x
        int lo = 0, hi = (int)breaks.size() - 1;
        int pos = 0;
        while (lo <= hi) {
            int mid = (lo+hi)/2;
            if (breaks[mid] <= (long double)x) {
                pos = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        return hull[pos].eval(x);
    }
};

std::vector<ll> computeMinCosts(
    const std::vector<std::vector<std::pair<int,ll>>>& tree,
    const std::vector<ll>& p,
    const std::vector<ll>& q,
    const std::vector<ll>& lim
) {
    int n = (int)tree.size() - 1; // nodes 1..n
    std::vector<ll> dis(n+1, 0);
    std::vector<int> parent(n+1, 0);
    // Build parent and distance via DFS
    std::vector<int> order;
    order.reserve(n);
    std::vector<int> stack = {1};
    parent[1] = 0;
    while (!stack.empty()) {
        int u = stack.back(); stack.pop_back();
        order.push_back(u);
        for (auto& e : tree[u]) {
            int v = e.first; ll w = e.second;
            if (v == parent[u]) continue;
            parent[v] = u;
            dis[v] = dis[u] + w;
            stack.push_back(v);
        }
    }
    
    std::vector<ll> f(n+1, INF);
    f[1] = 0;
    
    // We will use centroid decomposition, but to keep code simpler we implement
    // a recursive divide-and-conquer on the tree using a "root processing" approach.
    // For simplicity, we can implement a brute O(n^2) for small n, but for final
    // solution we need efficient. Given this is a teaching example, we provide
    // an O(n log^2 n) solution using centroid decomposition.
    
    // Build adjacency for working copy: we need to iterate children only.
    std::vector<std::vector<int>> children(n+1);
    std::vector<std::vector<ll>> edgeW(n+1);
    for (int u = 1; u <= n; ++u) {
        for (auto& e : tree[u]) {
            int v = e.first; ll w = e.second;
            if (parent[v] == u) {
                children[u].push_back(v);
                edgeW[u].push_back(w);
            }
        }
    }
    
    std::vector<bool> removed(n+1, false);
    std::vector<int> subSize(n+1);
    
    // Compute subtree sizes for a component
    std::function<void(int,int)> calcSize = [&](int u, int p) {
        subSize[u] = 1;
        for (int v : children[u]) {
            if (v == p || removed[v]) continue;
            calcSize(v, u);
            subSize[u] += subSize[v];
        }
    };
    
    // Find centroid of a component rooted at u with parent p
    std::function<int(int,int,int)> findCentroid = [&](int u, int p, int total) {
        for (int v : children[u]) {
            if (v == p || removed[v]) continue;
            if (subSize[v] > total/2) return findCentroid(v, u, total);
        }
        return u;
    };
    
    // Collect nodes in a component (excluding those removed), store in list
    std::vector<int> nodesList;
    std::function<void(int,int)> collectNodes = [&](int u, int p) {
        nodesList.push_back(u);
        for (int v : children[u]) {
            if (v == p || removed[v]) continue;
            collectNodes(v, u);
        }
    };
    
    // Process a centroid: update f for nodes in its child subtrees
    // For each child subtree, we gather nodes, sort by dis - lim descending,
    // and maintain a hull of ancestors (starting from centroid and moving up)
    std::function<void(int)> decompose = [&](int start) {
        calcSize(start, 0);
        int total = subSize[start];
        int cen = findCentroid(start, 0, total);
        
        // Mark centroid as removed temporarily (but we need it in hull)
        // We'll handle without actually removing centroid from children traversal
        // Instead, we decompose recursively after processing its child components.
        
        // Process each child component of centroid
        for (int idx = 0; idx < (int)children[cen].size(); ++idx) {
            int v = children[cen][idx];
            if (removed[v]) continue;
            // Collect nodes in this child subtree
            nodesList.clear();
            collectNodes(v, cen);
            // Sort by dis - lim descending
            std::sort(nodesList.begin(), nodesList.end(), [&](int a, int b) {
                return dis[a] - lim[a] > dis[b] - lim[b];
            });
            
            // Build hull of ancestors from cen upward until root of this component
            // We insert all ancestors of cen that are not removed and within the same component
            // Actually we need ancestors that are in the current component (the path from start to cen)
            // We'll collect them by going up from cen until start (inclusive)
            ConvexHull hull;
            hull.clear();
            int cur = cen;
            std::vector<int> ancPath;
            while (true) {
                ancPath.push_back(cur);
                if (cur == start) break;
                // find parent within component: we need parent that is not removed
                // Since we are in a component that was originally a subtree, parent is the original parent
                cur = parent[cur];
            }
            // Iterate nodes in sorted order, move j upward inserting ancestors
            int j = 0; // index in ancPath from top (start) to cen
            // ancPath is from cen up to start; we want to advance from cen upward
            // Actually j should start at cen (bottom) and move up
            int top = (int)ancPath.size() - 1; // index of start
            int ptr = 0; // pointing to current ancestor index (starting at 0 = cen)
            for (int node : nodesList) {
                // Advance j upward while dis[ancPath[ptr]] >= dis[node] - lim[node]
                while (ptr <= top && dis[ancPath[ptr]] >= dis[node] - lim[node]) {
                    // insert this ancestor
                    hull.add(Line(-dis[ancPath[ptr]], f[ancPath[ptr]]));
                    ptr++;
                }
                if (ptr > 0) {
                    ll best = hull.query(p[node]);
                    if (best != INF) {
                        ll candidate = best + p[node]*dis[node] + q[node];
                        if (candidate < f[node]) f[node] = candidate;
                    }
                }
            }
        }
        
        // Mark centroid as removed for further decomposition
        removed[cen] = true;
        
        // Recurse on each child component
        for (int v : children[cen]) {
            if (!removed[v]) {
                decompose(v);
            }
        }
    };
    
    decompose(1);
    
    return f;
}

#include <cassert>
#include <vector>
#include <iostream>

// Assume computeMinCosts is already defined above

int main() {
    // Test 1: simple chain with 3 nodes
    // 1 -2-> 2 -3-> 3
    // p2=1, q2=0, lim2=10; p3=1, q3=0, lim3=10
    std::vector<std::vector<std::pair<int,long long>>> tree1(4);
    tree1[1].push_back({2, 2});
    tree1[2].push_back({3, 3});
    std::vector<long long> p1 = {0, 0, 1, 1};
    std::vector<long long> q1 = {0, 0, 0, 0};
    std::vector<long long> lim1 = {0, 10, 10, 10};
    auto res1 = computeMinCosts(tree1, p1, q1, lim1);
    assert(res1[1] == 0);
    assert(res1[2] == 2); // f[1]+p2*2 = 0+2=2
    assert(res1[3] == 5); // f[2]+p3*3 = 2+3=5
    
    // Test 2: limit prevents direct from root
    // 1 -1-> 2 -10-> 3, lim for 3 is 1 so only node2 reachable
    std::vector<std::vector<std::pair<int,long long>>> tree2(4);
    tree2[1].push_back({2, 1});
    tree2[2].push_back({3, 10});
    std::vector<long long> p2 = {0, 0, 2, 3};
    std::vector<long long> q2 = {0, 0, 0, 0};
    std::vector<long long> lim2 = {0, 10, 10, 1};
    auto res2 = computeMinCosts(tree2, p2, q2, lim2);
    assert(res2[1] == 0);
    assert(res2[2] == 2); // f[1]+2*1 = 2
    assert(res2[3] == 32); // f[2]+3*10 = 2+30
    
    // Test 3: negative p causing choice of farther ancestor
    // 1 -1-> 2 -1-> 3, p3 = -10, lim3 big
    std::vector<std::vector<std::pair<int,long long>>> tree3(4);
    tree3[1].push_back({2, 1});
    tree3[2].push_back({3, 1});
    std::vector<long long> p3 = {0,0,-1,-10};
    std::vector<long long> q3 = {0,0,0,0};
    std::vector<long long> lim3 = {0,10,10,10};
    auto res3 = computeMinCosts(tree3, p3, q3, lim3);
    assert(res3[1] == 0);
    assert(res3[2] == -1); // 0 + (-1)*1 = -1
    // For node3: options: via 2: f[2]+(-10)*1 = -11; via 1: 0+(-10)*2 = -20
    assert(res3[3] == -20);
    
    // Test 4: single node tree (only root)
    std::vector<std::vector<std::pair<int,long long>>> tree4(2);
    std::vector<long long> p4 = {0,0};
    std::vector<long long> q4 = {0,0};
    std::vector<long long> lim4 = {0,0};
    auto res4 = computeMinCosts(tree4, p4, q4, lim4);
    assert(res4[1] == 0);
    
    // Test 5: branch tree, ensure all nodes computed
    // 1->2 (w=3), 1->3 (w=4), 2->4 (w=5)
    // p=[0,0,1,2,3], q=[0,0,10,20,30], lim=[0,10,10,10,10]
    std::vector<std::vector<std::pair<int,long long>>> tree5(5);
    tree5[1].push_back({2,3});
    tree5[1].push_back({3,4});
    tree5[2].push_back({4,5});
    std::vector<long long> p5 = {0,0,1,2,3};
    std::vector<long long> q5 = {0,0,10,20,30};
    std::vector<long long> lim5 = {0,10,10,10,10};
    auto res5 = computeMinCosts(tree5, p5, q5, lim5);
    assert(res5[1] == 0);
    assert(res5[2] == 0 + 1*3 + 10 = 13);
    assert(res5[3] == 0 + 2*4 + 20 = 28);
    assert(res5[4] == std::min(13 + 3*5 + 30, 0 + 3*8 + 30) = std::min(58, 54) = 54);
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// This problem is a classic "DP on tree with a convex hull trick" optimization. The recurrence is:
// \( f[i] = \min_{j \in \text{ancestors of } i, \, dis[j] \ge dis[i] - lim[i]} (f[j] - p[i]\cdot dis[j]) + p[i]\cdot dis[i] + q[i] \).
// For a fixed \( i \), the expression inside the min is linear in \( p[i] \) if we view \( f[j] - p[i]\cdot dis[j] \) as a line evaluated at \( x = p[i] \) with slope \(-dis[j]\) and intercept \( f[j] \). Since \( dis[j] \) increases as we go down the tree (positive edge weights), the slopes are monotonically decreasing along the path from root to leaves. We need to query the minimum of a set of lines at a given x, but with a constraint that only ancestors with \( dis[j] \ge dis[i] - lim[i] \) are eligible. This is a "convex hull trick with a moving window" that can be solved using divide-and-conquer on the tree (centroid decomposition) combined with a monotonic convex hull. We decompose the tree recursively: for each centroid, we temporarily cut edges from the centroid to its children, then process all nodes in the subtree of the centroid (excluding the centroid itself) that are reachable via the children. We sort these nodes by \( dis[i] - lim[i] \) descending, and we insert the centroid and its ancestors (within the current decomposition component) into a convex hull as we move upward. We maintain a lower convex hull of lines (slope = -dis[j], intercept = f[j]) and query it using binary search for each node. The divide-and-conquer ensures that for each node we only consider ancestors that are within the same component, and the sorting order guarantees that when we process nodes by decreasing \( dis[i]-lim[i] \), we have already inserted all eligible ancestors. Edge cases: when a node has no eligible ancestor, the cost remains +infinity (but the problem likely guarantees at least one ancestor? Actually root is always eligible if lim is large enough; if not, the answer may be +inf, but typical test cases ensure solution exists). Also careful with overflow: use `long long` and handle multiplication in `__int128` if needed, but since constraints may allow values within 64-bit, we can keep `long long` for final answers but use `long double` for slope comparisons. The algorithm runs in \( O(n \log^2 n) \) time due to centroid decomposition and sorting at each level, and \( O(n) \) space.
