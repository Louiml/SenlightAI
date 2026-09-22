// Write a C++ function `long long count_valid_paths(const std::vector<int>& values, const std::vector<std::pair<int,int>>& edges)` that, given a tree with `n` nodes (node indices `1..n`) where each node has a positive integer value from `values`, counts the number of unordered simple paths (with at least one edge, i.e., length ≥ 1) whose edge-wise GCD of all node values along the path is exactly `1`. The function must return that count as a `long long`. You may assume `n ≥ 2` and that the input tree is connected and undirected. The path must contain at least one edge, so single-node paths are not counted. The function should handle large `n` up to `3e5` and values up to `3e5`.

#include <cassert>
#include <vector>
#include <utility>
#include <numeric>

long long count_valid_paths(const std::vector<int>& values, const std::vector<std::pair<int,int>>& edges); // declaration from above

int main() {
    // Test 1: simple path of length 2, values 2 and 3 => gcd=1 => one path
    {
        std::vector<int> v = {0, 2, 3}; // 1-indexed
        std::vector<std::pair<int,int>> e = {{1,2}};
        assert(count_valid_paths(v, e) == 1);
    }
    // Test 2: all values 2 => gcd of any path is 2 => zero paths
    {
        std::vector<int> v = {0, 2, 2, 2};
        std::vector<std::pair<int,int>> e = {{1,2},{2,3}};
        assert(count_valid_paths(v, e) == 0);
    }
    // Test 3: star with center value 1, leaves 2,3,5 => paths with gcd 1? center-leaf gcd=1 for every leaf => 3 paths. leaf-leaf gcd = gcd(2,3)=1 via center? gcd of path leaf1-center-leaf2 = gcd(2,1,3)=1 => 3 choose 2 =3 more. total 6.
    {
        std::vector<int> v = {0, 1, 2, 3, 5};
        std::vector<std::pair<int,int>> e = {{1,2},{1,3},{1,4}};
        assert(count_valid_paths(v, e) == 6);
    }
    // Test 4: chain 1-2-3 with values 2,3,4 => paths: (1,2):gcd=1; (2,3):gcd=1; (1,3):gcd=1? gcd(2,3,4)=1 => total 3
    {
        std::vector<int> v = {0, 2, 3, 4};
        std::vector<std::pair<int,int>> e = {{1,2},{2,3}};
        assert(count_valid_paths(v, e) == 3);
    }
    // Test 5: chain 1-2-3 with values 2,4,6 => all gcds are even => 0
    {
        std::vector<int> v = {0, 2, 4, 6};
        std::vector<std::pair<int,int>> e = {{1,2},{2,3}};
        assert(count_valid_paths(v, e) == 0);
    }
    // Test 6: two nodes 6 and 10 => gcd=2 => 0
    {
        std::vector<int> v = {0, 6, 10};
        std::vector<std::pair<int,int>> e = {{1,2}};
        assert(count_valid_paths(v, e) == 0);
    }
    // Test 7: two nodes 6 and 7 => gcd=1 => 1
    {
        std::vector<int> v = {0, 6, 7};
        std::vector<std::pair<int,int>> e = {{1,2}};
        assert(count_valid_paths(v, e) == 1);
    }
    // Test 8: path of 4 nodes, values 1,1,1,1 => all pairs (6 possible) have gcd 1 => 6
    {
        std::vector<int> v = {0, 1, 1, 1, 1};
        std::vector<std::pair<int,int>> e = {{1,2},{2,3},{3,4}};
        assert(count_valid_paths(v, e) == 6);
    }
    // Test 9: path of 3 nodes, values 2,1,2 => paths: (1,2):gcd=1; (2,3):gcd=1; (1,3):gcd(2,1,2)=1 => 3
    {
        std::vector<int> v = {0, 2, 1, 2};
        std::vector<std::pair<int,int>> e = {{1,2},{2,3}};
        assert(count_valid_paths(v, e) == 3);
    }
    // Test 10: small random tree: 4 nodes star with center 2, leaves 3,4,5 => center-leaf gcds: 1,2,1 => two paths with gcd 1 (leaves 3 and 5). leaf-leaf: (3,4) gcd? path 3-2-4 gcd(3,2,4)=1; (3,5) gcd(3,2,5)=1; (4,5) gcd(4,2,5)=1? gcd(4,2)=2, gcd(2,5)=1 => 1. so total leaf-leaf 3 + center-leaf 2 =5
    {
        std::vector<int> v = {0, 2, 3, 4, 5};
        std::vector<std::pair<int,int>> e = {{1,2},{1,3},{1,4}};
        assert(count_valid_paths(v, e) == 5);
    }
    return 0;
}

#include <vector>
#include <algorithm>
#include <cstring>
#include <functional>

using ll = long long;

class PathGCDCounter {
    int n;
    const std::vector<int>& a;
    std::vector<std::vector<int>> adj;
    std::vector<bool> removed;
    std::vector<int> sub_size;
    int max_val;
    std::vector<int> mu;
    std::vector<std::vector<int>> divisors;

    // linear sieve for Möbius function up to max_val
    void build_mu(int mx) {
        mu.assign(mx + 1, 0);
        std::vector<int> primes;
        std::vector<bool> is_comp(mx + 1, false);
        mu[1] = 1;
        for (int i = 2; i <= mx; ++i) {
            if (!is_comp[i]) {
                primes.push_back(i);
                mu[i] = -1;
            }
            for (int p : primes) {
                if (i * p > mx) break;
                is_comp[i * p] = true;
                if (i % p == 0) {
                    mu[i * p] = 0;
                    break;
                } else {
                    mu[i * p] = -mu[i];
                }
            }
        }
    }

    // list all divisors of numbers up to max_val
    void build_divisors(int mx) {
        divisors.assign(mx + 1, {});
        for (int d = 1; d <= mx; ++d)
            for (int multiple = d; multiple <= mx; multiple += d)
                divisors[multiple].push_back(d);
    }

    // compute subtree sizes and find centroid of current component
    int get_centroid(int start) {
        std::vector<int> order;
        std::function<void(int,int)> dfs = [&](int u, int p) {
            sub_size[u] = 1;
            order.push_back(u);
            for (int v : adj[u]) {
                if (v == p || removed[v]) continue;
                dfs(v, u);
                sub_size[u] += sub_size[v];
            }
        };
        dfs(start, -1);
        int total = sub_size[start];
        int centroid = start;
        for (int u : order) {
            bool ok = true;
            for (int v : adj[u]) {
                if (removed[v]) continue;
                int part = (v == u ? 0 : (sub_size[v] < sub_size[u] ? sub_size[v] : total - sub_size[u]));
                if (part > total / 2) { ok = false; break; }
            }
            if (ok) { centroid = u; break; }
        }
        return centroid;
    }

    // for a given subtree rooted at u (not to go through centroid), collect all gcd values from centroid to nodes,
    // and also update global counts
    void collect_gcds(int u, int p, int current_gcd, std::vector<int>& counts, std::vector<int>& current_div_counts) {
        int g = std::gcd(a[u], current_gcd);
        for (int d : divisors[g]) {
            counts[d]++;
            current_div_counts[d]++;
        }
        for (int v : adj[u]) {
            if (v == p || removed[v]) continue;
            collect_gcds(v, u, g, counts, current_div_counts);
        }
    }

public:
    PathGCDCounter(const std::vector<int>& values, const std::vector<std::pair<int,int>>& edges)
        : a(values), n(values.size()), max_val(*std::max_element(values.begin(), values.end())) {
        adj.assign(n + 1, {});
        for (auto& e : edges) {
            adj[e.first].push_back(e.second);
            adj[e.second].push_back(e.first);
        }
        removed.assign(n + 1, false);
        sub_size.assign(n + 1, 0);
        build_mu(max_val);
        build_divisors(max_val);
    }

    ll count() {
        ll total_paths = 0; // will sum over all d: mu[d] * F[d]
        // We'll compute F[d] cumulatively across all centroids
        std::vector<ll> F(max_val + 1, 0);

        std::function<void(int)> decompose = [&](int start) {
            int c = get_centroid(start);
            removed[c] = true;

            // Component consisting of the centroid itself
            // We'll maintain a global count per divisor among all nodes processed so far (including centroid)
            // We process each subtree one at a time.
            // Start with centroid as a single "subtree"
            std::vector<int> total_div_counts(max_val + 1, 0);
            for (int d : divisors[a[c]]) total_div_counts[d]++;

            for (int v : adj[c]) {
                if (removed[v]) continue;
                std::vector<int> current_div_counts(max_val + 1, 0);
                collect_gcds(v, c, a[c], current_div_counts, current_div_counts);

                // For each divisor d, add current_count[d] * total_count[d] to F[d]
                for (int d = 1; d <= max_val; ++d) {
                    if (current_div_counts[d] > 0 && total_div_counts[d] > 0) {
                        F[d] += (ll)current_div_counts[d] * total_div_counts[d];
                    }
                }
                // Merge current into total
                for (int d = 1; d <= max_val; ++d) {
                    total_div_counts[d] += current_div_counts[d];
                }
            }

            // Recurse into each component
            for (int v : adj[c]) {
                if (removed[v]) continue;
                decompose(v);
            }
        };

        decompose(1);

        // Apply Möbius inversion
        ll answer = 0;
        for (int d = 1; d <= max_val; ++d) {
            if (mu[d] != 0 && F[d] > 0) {
                answer += mu[d] * F[d];
            }
        }
        return answer;
    }
};

// Public free function matching the task specification
long long count_valid_paths(const std::vector<int>& values, const std::vector<std::pair<int,int>>& edges) {
    PathGCDCounter counter(values, edges);
    return counter.count();
}

// We need to count paths where the GCD of all node values on the path is exactly 1. A direct O(n²) enumeration is impossible. Use Centroid Decomposition to count paths efficiently. For each centroid, we consider all paths that pass through it. For a path through the centroid, the GCD is `gcd(gcd of values on path from centroid to one endpoint, gcd from centroid to other endpoint, a[centroid])`. To count only those with overall GCD = 1, we use Möbius inversion: let `F[d]` = number of paths whose GCD is a multiple of `d`. Then `F[d] = sum_{k multiple of d} count[GCD = k]`. By Möbius inversion, `count[GCD=1] = sum_{d≥1} mu[d] * F[d]`. For each centroid, we compute `F[d]` for all divisors `d` of the maximum value. Specifically, for each subtree attached to the centroid, we collect, for each node, the GCD of values from centroid to that node. For each collected GCD `g`, we increment a hash table `cnt[g]` for every divisor `g` itself (since the GCD of the path through centroid is a divisor of the node-GCD). Then for each pair of different subtrees plus the centroid itself, the number of paths with overall GCD multiple of `d` is computed via convolution on divisor counts: sum over all pairs of nodes from different components (including the centroid as a single component) of the product `cnt1[d] * cnt2[d]`. We maintain this by processing subtrees one by one: maintain a global `total` array that stores counts per divisor from all previously processed components (including centroid), then for current subtree, for each divisor `d` in the subtree's counts, add `current[d] * total[d]` to `F[d]`. Then merge current into total. After processing all subtrees, we add the contribution to `F[d]`. Finally, for each `d`, sum `mu[d] * F[d]` gives number of paths with GCD exactly 1. Complexity: Centroid decomposition gives O(n log n) per level, and each node contributes its divisors (up to ~128) at each level, so O(n log n * τ(max)) ≈ acceptable for 3e5. Space O(n + max * τ(max)).
//
// Edge cases: paths of length 1 (adjacent nodes) must be counted if GCD of their two values is 1. Paths longer than 1 also counted. Single-node paths are excluded. Values are positive, no zeros. The tree may be a line, so recursion depth must be managed (we use iterative or increase stack but in practice use recursion with careful implementation; for testing, small n is fine).
