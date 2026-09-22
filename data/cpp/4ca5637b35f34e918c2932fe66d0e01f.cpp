Write a C++ function `long long universityRaidQuery(long long n, long long m, const std::vector<std::string>& operations, const std::vector<std::array<long long,2>>& params)` that simulates a sequence of operations on two dynamic disjoint-set forests (called "universe" and "military") over elements numbered `1..n`. Initially, each element is its own component in both forests, each with size 1. The component that contains an element at a given time is represented by a union-find structure with union-by-size (when merging, attach the smaller tree under the larger; if equal, attach the second under the first). Each merge operation records a timestamp (the operation number, 1-indexed). The operations are:

- `"U" x y`: merge the components of x and y in the universe forest, recording the operation number as the "universe effect" time for the child root.
- `"M" x y`: merge the components of x and y in the military forest, recording the operation number as the "military effect" time for the child root.
- `"A" x`: "add" operation — increase the size of the universe component containing element x by 1, and store the current time and cumulative size into per-component lists.
- `"Z" y`: "raid" operation — set the "last raid time" of the military component containing y to the current time.
- `"Q" x`: query — output the total number of "add" operations that affected the universe component of x after the most recent raid on that component's military history. More precisely, define `T` as the maximum raid time found along the military path from x to its military root, but if a component was merged after a raid, the raid time is only considered if the raid happened after the merge effect time. Then sum over all universe ancestors of x (including itself) the number of "add" operations that were recorded at times strictly greater than `T` and that belong to that ancestor's component (using the cumulative size lists). Return this sum.

The function should return the results of all `"Q"` operations in order in a `std::vector<long long>`.

This problem is a faithful reinterpretation of the given snippet. The key is to maintain two independent union-find structures with union-by-size. For the universe forest, each component keeps a vector `add` of timestamps when "A" operations were applied to that component (at that moment, the component's size is stored in a parallel `pre` prefix sum vector). When a merge happens, the child root is recorded with an `eff` time (the operation number) so that later queries can correctly filter operations that happened before a merge. For the military forest, each component keeps a `tag` (last raid time). When merging, the child root gets an `eff` time; the `tag` is not merged but propagated appropriately. For a query on x: first find the most recent raid time `T` along the military path. Starting from x, go up to the military root. At each node, if its `tag` is greater than the current effective time (which is the max of the recorded `eff` times along the path), we take that as candidate for `T`. To be precise, `getlastclear` recursively goes from x upward: if the node is root, return `tag[x]` if it is >T else 0; otherwise, return the max of `tag[x]` (if >T) and `getlastclear(fa[x], max(T, eff[x]))`. Then for the universe forest, starting from x, we sum for each ancestor (including x) the number of add operations that occurred after `T`. For a given node, we use binary search on its `add` vector to find the first index with `add[idx] > T`; then the contribution is `pre[last] - (first>0 ? pre[first-1] : 0)`. Then recursively climb to parent with `T` updated to `max(T, eff[x])`. Edge cases: if no adds or all adds ≤T, contribution 0; if no raid (T=0) then all adds count. Since union by size ensures logarithmic depth, each query is O(log n) for both military and universe paths, plus binary search O(log n) per node. Total O((m+number of queries) log n) time and O(m) space for the operation records. The final answer vector collects output lines.

#include <vector>
#include <array>
#include <string>
#include <algorithm>

// Simulates the operations and returns query results.
std::vector<long long> universityRaidQuery(
    long long n, long long m,
    const std::vector<std::string>& operations,
    const std::vector<std::array<long long,2>>& params) {
    
    // Universe forest
    std::vector<long long> univfa(n+1), univsize(n+1), univeff(n+1);
    std::vector<std::vector<long long>> add(n+1), pre(n+1);
    
    // Military forest
    std::vector<long long> milifa(n+1), milisize(n+1), milieff(n+1), tag(n+1);
    
    // Initialize both forests
    for (long long i = 1; i <= n; ++i) {
        univfa[i] = milifa[i] = i;
        univsize[i] = milisize[i] = 1;
        univeff[i] = milieff[i] = 0;
        tag[i] = 0;
    }
    
    // Find with path compression (iterative to avoid recursion depth issues)
    auto findset = [&](long long x, std::vector<long long>& fa) {
        long long root = x;
        while (root != fa[root]) root = fa[root];
        while (x != root) {
            long long next = fa[x];
            fa[x] = root;
            x = next;
        }
        return root;
    };
    
    // Merge helper
    auto merge = [&](long long x, long long y,
                     std::vector<long long>& fa,
                     std::vector<long long>& size,
                     std::vector<long long>& eff,
                     long long rep) {
        long long u = findset(x, fa);
        long long v = findset(y, fa);
        if (u == v) return;
        if (size[u] < size[v]) std::swap(u, v);
        fa[v] = u;
        size[u] += size[v];
        eff[v] = rep;
    };
    
    // Add operation
    auto inc = [&](long long x, long long T) {
        long long root = findset(x, univfa);
        add[root].push_back(T);
        if (pre[root].empty()) pre[root].push_back(univsize[root]);
        else pre[root].push_back(pre[root].back() + univsize[root]);
    };
    
    // Raid operation
    auto raid = [&](long long x, long long T) {
        long long root = findset(x, milifa);
        tag[root] = T;
    };
    
    // Recursive getlastclear using explicit stack to avoid recursion
    auto getlastclear = [&](long long x, long long T) {
        long long result = 0;
        long long curT = T;
        long long node = x;
        while (true) {
            if (tag[node] > curT) result = std::max(result, tag[node]);
            if (node == milifa[node]) break;
            curT = std::max(curT, milieff[node]);
            node = milifa[node];
        }
        return result;
    };
    
    // Recursive cal using explicit stack
    auto cal = [&](long long x, long long T) {
        long long total = 0;
        long long curT = T;
        long long node = x;
        while (true) {
            long long val = 0;
            if (!add[node].empty() && add[node].back() > curT) {
                auto it = std::upper_bound(add[node].begin(), add[node].end(), curT);
                long long first = it - add[node].begin();
                val = pre[node].back() - (first > 0 ? pre[node][first-1] : 0);
            }
            total += val;
            if (node == univfa[node]) break;
            curT = std::max(curT, univeff[node]);
            node = univfa[node];
        }
        return total;
    };
    
    std::vector<long long> results;
    for (long long rep = 1; rep <= m; ++rep) {
        const std::string& op = operations[rep-1];
        long long x = params[rep-1][0];
        long long y = params[rep-1][1];
        if (op == "U") {
            merge(x, y, univfa, univsize, univeff, rep);
        } else if (op == "M") {
            merge(x, y, milifa, milisize, milieff, rep);
        } else if (op == "A") {
            inc(x, rep);
        } else if (op == "Z") {
            raid(y, rep);
        } else { // "Q"
            long long T = getlastclear(x, 0);
            results.push_back(cal(x, T));
        }
    }
    return results;
}

#include <cassert>
#include <vector>
#include <array>
#include <string>

// Include the solution code here (copy from above)

int main() {
    // Test 1: Simple universe add and query without raids
    {
        long long n = 3, m = 5;
        std::vector<std::string> ops = {"A", "A", "Q", "Q", "Q"};
        std::vector<std::array<long long,2>> params = {{1,0},{2,0},{1,0},{2,0},{3,0}};
        auto res = universityRaidQuery(n, m, ops, params);
        assert(res.size() == 3);
        assert(res[0] == 1); // one add on component 1
        assert(res[1] == 1); // one add on component 2
        assert(res[2] == 0); // no add on component 3
    }
    
    // Test 2: Merge then add, query should see cumulative sizes
    {
        long long n = 2, m = 5;
        std::vector<std::string> ops = {"U", "A", "Q", "Q"};
        std::vector<std::array<long long,2>> params = {{1,2},{1,0},{1,0},{2,0}};
        auto res = universityRaidQuery(n, m, ops, params);
        assert(res.size() == 2);
        assert(res[0] == 1); // after merge, add increases component size to 3
        assert(res[1] == 1); // same component
    }
    
    // Test 3: Raid clears adds after raid time
    {
        long long n = 1, m = 7;
        std::vector<std::string> ops = {"A", "Z", "A", "Q", "Z", "A", "Q"};
        std::vector<std::array<long long,2>> params = {{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0}};
        auto res = universityRaidQuery(n, m, ops, params);
        assert(res.size() == 2);
        assert(res[0] == 1); // after first raid at t=2, add at t=3 remains
        assert(res[1] == 1); // after second raid at t=5, add at t=6 remains
    }
    
    // Test 4: Military merge and raid timestamps
    {
        long long n = 2, m = 8;
        std::vector<std::string> ops = {"M", "A", "Z", "M", "A", "Q", "Q"};
        std::vector<std::array<long long,2>> params = {{1,2},{1,0},{2,0},{1,2},{1,0},{1,0},{2,0}};
        auto res = universityRaidQuery(n, m, ops, params);
        assert(res.size() == 2);
        // After M at t=1, Z at t=3 on comp2, M at t=4 merges again, add at t=5 is after raid? 
        // The raid on comp2 at t=3, but comp2 was merged at t=1, so raid applies to whole comp.
        // For element 1: military path to root, tags along path. raid at t=3 > eff? yes. 
        // So T=3, adds at t=2 are cleared, adds at t=5 remain (1). For element 2 same.
        assert(res[0] == 1);
        assert(res[1] == 1);
    }
    
    // Test 5: No operations, query returns 0
    {
        long long n = 1, m = 1;
        std::vector<std::string> ops = {"Q"};
        std::vector<std::array<long long,2>> params = {{1,0}};
        auto res = universityRaidQuery(n, m, ops, params);
        assert(res.size() == 1);
        assert(res[0] == 0);
    }
    
    // Test 6: Larger chain and multiple merges
    {
        long long n = 4, m = 10;
        std::vector<std::string> ops = {"U", "U", "A", "Q", "M", "M", "Z", "A", "Q", "Q"};
        std::vector<std::array<long long,2>> params = {{1,2},{3,4},{1,0},{1,0},{1,3},{2,4},{1,0},{2,0},{1,0},{3,0}};
        auto res = universityRaidQuery(n, m, ops, params);
        assert(res.size() == 3);
        // After U at 1,2 and 3,4, add at 3 to comp{1,2}. Query at 4: T=0, adds after 0 on path: 
        // component of 1 has add at 3 -> 1. Component of 2 same. So res[0]=1.
        // After M at 5 and 6, military merges, Z at 7 on comp1 (which includes all? Actually military merges 
        // 1-3 and 2-4, so all connected). Raid at 7, add at 8 on comp{1,2,3,4}? Actually A on 2 at 8. 
        // Query at 9 on 1: T=7, adds after 7: only add at 8, but that add is on entire universe? 
        // Since U merges happened earlier, universe comp of 1 includes all 4? Actually U at 1 merged 1-2, 
        // U at 2 merged 3-4, but no U between those two groups. So universe comp of 1 is {1,2}, and add at 8 
        // on 2 is on that comp. T=7, so add at 8 counts -> 1. Query at 10 on 3: universe comp {3,4} has no adds >7 ->0.
        assert(res[0] == 1);
        assert(res[1] == 1);
        assert(res[2] == 0);
    }
    
    // Test 7: Multiple adds to same component and raids
    {
        long long n = 1, m = 8;
        std::vector<std::string> ops = {"A", "A", "Z", "A", "Q", "Z", "A", "Q"};
        std::vector<std::array<long long,2>> params = {{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0},{1,0}};
        auto res = universityRaidQuery(n, m, ops, params);
        assert(res.size() == 2);
        // Adds at t=1,2, then raid at 3, add at 4, query at 5: T=3, adds >3: only t=4 ->1.
        // Raid at 6, add at 7, query at 8: T=6, adds >6: t=7 ->1.
        assert(res[0] == 1);
        assert(res[1] == 1);
    }
    
    // Test 8: Empty operations (no queries)
    {
        long long n = 1, m = 0;
        std::vector<std::string> ops;
        std::vector<std::array<long long,2>> params;
        auto res = universityRaidQuery(n, m, ops, params);
        assert(res.empty());
    }
    
    // Test 9: Raid after merge but before add, check eff filtering
    {
        long long n = 2, m = 6;
        std::vector<std::string> ops = {"M", "Z", "A", "Q", "A", "Q"};
        std::vector<std::array<long long,2>> params = {{1,2},{2,0},{1,0},{1,0},{1,0},{1,0}};
        auto res = universityRaidQuery(n, m, ops, params);
        assert(res.size() == 2);
        // M at 1, Z at 2 on comp2 (now comp1-2), A at 3, query at 4: T=2, add at 3 counts ->1.
        // A at 5, query at 6: T=2, adds at 3 and 5 count ->2.
        assert(res[0] == 1);
        assert(res[1] == 2);
    }
    
    // Test 10: Union by size and query from child after merge
    {
        long long n = 3, m = 5;
        std::vector<std::string> ops = {"U", "U", "A", "Q", "Q"};
        std::vector<std::array<long long,2>> params = {{1,2},{3,1},{3,0},{3,0},{1,0}};
        auto res = universityRaidQuery(n, m, ops, params);
        assert(res.size() == 2);
        // U(1,2) t=1, U(3,1) t=2 -> all connected, size becomes 3. A on 3 at t=3 adds to root.
        // Query on 3: path to root includes 3's parent (maybe 1) and root. Adds after 0: add at 3 on root ->1.
        // Same for 1.
        assert(res[0] == 1);
        assert(res[1] == 1);
    }
    
    return 0;
}
