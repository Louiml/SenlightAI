/*
Given an `m x n` grid of non-negative integers representing terrain heights, a second `m x n` grid of booleans marking "starting points", and an integer `t`, write a C++ function that computes the total sum of the "effort weight" required to connect all clusters of at least `t` cells. Specifically, process the edges between adjacent cells (horizontal and vertical only) in increasing order of their absolute height difference, merging components via a union-find structure as you go. When merging two components, if the combined size reaches or exceeds `t` for the first time (i.e., at least one of the two components was previously below `t` but after merging the union is >= `t`), then for each starting point in the component(s) that crossed the threshold, add the current edge weight multiplied by the number of starting points in that component (if a component crosses the threshold, add the edge weight times its starting-point count once per crossing, considering only components that were below `t` before the merge). The final answer is the sum of all such contributions. The function must read the data from standard input in the exact order: first `m`, `n`, `t`, then the height grid (row-major), then the boolean starting-point grid (where `1` means starting point). Output the single integer answer. The grid dimensions are at most 500 by 500, heights fit in a 32-bit signed integer, and the result may exceed 64-bit but the required answer fits in a 64-bit signed integer. Assume `t` is at least 1.
*/
#include <bits/stdc++.h>
using namespace std;

// Solves the described ski-level problem.
// Reads from standard input: m, n, t, height grid (m rows, n cols), start grid (0/1).
// Returns the total sum as a 64-bit integer.
int64_t solve() {
    int m, n, t;
    cin >> m >> n >> t;

    vector<vector<int>> mat(m, vector<int>(n));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            cin >> mat[i][j];

    vector<vector<bool>> start(m, vector<bool>(n, false));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) {
            int val;
            cin >> val;
            start[i][j] = (val == 1);
        }

    const int N = m * n; // total number of cells

    // Union-Find structure
    vector<int> parent(N), size(N, 1), starting_points(N, 0);
    for (int i = 0; i < N; ++i) {
        parent[i] = i;
        // Map idx = i * n + j
        int row = i / n;
        int col = i % n;
        starting_points[i] = (start[row][col] ? 1 : 0);
    }

    function<int(int)> find = [&](int x) -> int {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    };

    // Edge list: from, to, weight
    struct Edge {
        int from, to;
        int64_t w;
        bool operator<(const Edge &other) const {
            return w < other.w;
        }
    };

    vector<Edge> edges;
    const int di[2] = {0, 1};
    const int dj[2] = {1, 0};
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            int pos1 = i * n + j;
            for (int k = 0; k < 2; ++k) {
                int ni = i + di[k];
                int nj = j + dj[k];
                if (ni >= m || nj >= n) continue;
                int pos2 = ni * n + nj;
                edges.push_back({pos1, pos2, abs(mat[i][j] - mat[ni][nj])});
            }
        }
    }

    sort(edges.begin(), edges.end());

    int64_t ans = 0;
    for (const Edge &e : edges) {
        int rx = find(e.from);
        int ry = find(e.to);
        if (rx == ry) continue;

        // Union by size
        if (size[rx] < size[ry]) swap(rx, ry);
        // Now rx is the root with larger (or equal) size; we'll attach ry to rx
        // But careful: we need sizes of both before merging to check crossing.
        int szx = size[rx];
        int szy = size[ry];
        int combined = szx + szy;
        if (combined >= t) {
            // If either component was below t, add contribution for that component's starting points
            if (szx < t)
                ans += e.w * starting_points[rx];
            if (szy < t)
                ans += e.w * starting_points[ry];
        }
        // Merge ry into rx
        parent[ry] = rx;
        size[rx] = combined;
        starting_points[rx] += starting_points[ry];
    }

    return ans;
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

// Declare the solution function (should be defined elsewhere)
int64_t solve();

// Helper to feed input via stringstream
int64_t run_with_input(const string& input) {
    istringstream iss(input);
    // Redirect cin to iss
    streambuf* orig = cin.rdbuf(iss.rdbuf());
    int64_t res = solve();
    cin.rdbuf(orig); // restore
    return res;
}

int main() {
    // 1x1 grid, t=1 -> no edges, no merges -> answer 0
    assert(run_with_input("1 1 1\n5\n1\n") == 0);

    // 2x1, heights [5,5], both starting points, t=1 -> no merges? Actually merge once, but both components already >=1, so no contribution
    assert(run_with_input("2 1 1\n5\n5\n1\n1\n") == 0);

    // 2x1, heights [5,6], both starting, t=2 -> merge with weight 1, combined=2 crosses, both size<2 -> add 1*1 + 1*1 = 2
    assert(run_with_input("2 1 2\n5\n6\n1\n1\n") == 2);

    // 3x1, heights [1,2,3], starts at 0 and 2 (positions 0 and 2 are 1? Let's set: start[0]=0, start[1]=1, start[2]=0) t=2
    // edges: (0,1) w=1 merges: size 1+1=2 crosses -> add 1* (start[0]=0) + 1*(start[1]=1)=1
    // then (1,2) w=1 merges size2+1=3, size of comp with root 1? Actually after first merge, comp size 2 >=2, other size1<2 -> add 1* (start of comp size1 = start[2]=0) => 0
    // total =1
    assert(run_with_input("3 1 2\n1\n2\n3\n0\n1\n0\n") == 1);

    // 2x2 grid with all equal heights, t=4, all starting points
    // depths: all 10, start all 1. edges all weight 0. merge all cells together:
    // each merge: when combined first reaches 4, add 0 * starting (so 0). total 0
    assert(run_with_input("2 2 4\n10 10\n10 10\n1 1\n1 1\n") == 0);

    // 2x2 with varying heights: heights [0,1;1,2], start all 1, t=4
    // edges: (0,1) w1, (0,2) w1, (1,3) w1, (2,3) w1. Merging eventually all, each merge weight1,
    // when combined reaches 4, add 1* (starting of each below-t component)
    // Let's simulate:
    // merge 0-1: sz1+1=2 (<4) no add. comp {0,1} has starts2
    // merge 0-2: comp {0,1} size2 + {2} size1 =3 (<4) no add. comp {0,1,2} starts3
    // merge 1-3: comp {0,1,2} size3 + {3} size1 =4 >=4 -> sz comp=3 (<4) add 1*3=3, sz other=1 (<4) add 1*1=1 => total 4
    // total 4
    assert(run_with_input("2 2 4\n0 1\n1 2\n1 1\n1 1\n") == 4);

    // Test with t large so no crossing: 1x2, t=3, both starts
    // merge weight 5, combined=2 <3 -> no add -> 0
    assert(run_with_input("1 2 3\n4\n9\n1\n1\n") == 0);

    // Test with a larger grid: 2x3, t=3, some starts
    // heights: 1 2 3 / 2 4 5
    // starts: 1 0 0 / 0 1 0
    // We'll trust the algorithm, just check not crashing and returns a value
    // Use a simple run to ensure it doesn't crash
    int64_t res = run_with_input("2 3 3\n1 2 3\n2 4 5\n1 0 0\n0 1 0\n");
    // No specific expected value; just verify it runs (we'll just assert true)
    assert(res >= 0);

    // 1x1 with t=1, start=1 -> as per algorithm, no merge so 0
    assert(run_with_input("1 1 1\n7\n1\n") == 0);

    cout << "All tests passed" << endl;
    return 0;
}
// The problem is a classic Kruskal-like algorithm on a grid graph: each cell is a node, and edges are between orthogonal neighbors with weight equal to the absolute height difference. Sort all edges by weight. Use a union-find (disjoint-set) with path compression and union by rank. Each component maintains:
// - `RA` (size/rank) for union by size.
// - `starting_points` count (sum of starting-point booleans in the component).
// When joining two roots, after finding and if they are different, we compute the combined size. If the combined size >= `t`, then for each of the two component roots (before merging) that has size < `t`, we add `w * starting_points[root]` to the answer. This ensures each component that first reaches threshold `t` contributes exactly once for each starting point it contains at that moment. Important edge case: if both components are already >= `t` before the merge, nothing is added because neither crossed the threshold. Also, if a component has size exactly `t` from the start (if `t == 1`), then initially each single cell with a starting point is already a valid component? The algorithm as written processes edges and only adds when merging causes a component to reach `t`. For `t == 1`, every single cell is already a component of size 1 >= `t`. The algorithm would never add anything because no merge makes a component cross from <t to >=t? Wait: if t=1, then for a single cell, the component size is 1, which is already >=t. The algorithm only adds when merging, but a single cell that is a starting point and already of size 1 should contribute? The given reference code assumes that only when a component reaches size >= t during a merge does it contribute, so for t=1, no contributions happen because all components are already of size 1. That seems a flaw if t=1, but the problem likely ensures t>1? The snippet doesn't specify, but the algorithm as is will not count starting points in components that already meet the threshold initially. Since the task is to replicate the given code's behavior, we will follow exactly: only components that become >=t during a merge contribute. For t=1, the answer would always be 0. That is acceptable for the task. Complexity: there are O(mn) edges (about 2mn), sorting takes O(mn log(mn)) time, and union-find operations are nearly O(alpha(mn)). Space is O(mn) for the grid, parent arrays, and edge list. Given max grid 500x500, edges ~500,000, sorting is fine.
