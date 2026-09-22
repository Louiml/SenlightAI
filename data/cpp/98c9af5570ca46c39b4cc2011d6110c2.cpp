Given an array of `n` positive integers and a cost parameter `p`, you must connect all elements into a single connected component using edges. Initially, each element is its own component. You may add an edge between two adjacent positions `i` and `i+1` with cost `a[i]` (i.e., the value at the left endpoint) only if both `a[i]` and `a[i+1]` are divisible by `a[i]` (note that `a[i]` always divides itself, so the condition reduces to `a[i+1] % a[i] == 0`). After using all such divisible edges optimally (you may choose any subset of them, and they are undirected), you may connect any remaining separate components using direct edges of cost `p` each. Your goal is to minimize the total cost to connect the entire array into one component. Write a C++ function `long long minConnectionCost(const std::vector<int>& a, int p)` that returns this minimal total cost. The input array size `n` satisfies `1 ≤ n ≤ 2·10^5`, each `a[i] ≤ 10^9`, and `p ≤ 10^9`. The array may contain duplicate values.

#include <cassert>
#include <vector>

// The solution function is declared above. Test it with several cases.
int main() {
    // Simple case: one element, no edges needed, cost 0? Actually components-1 = 0, so 0.
    assert(minConnectionCost({5}, 10) == 0);
    // All divisible by 2, p=5, edges cost 2 each
    assert(minConnectionCost({2,2,2,2}, 5) == 6); // 3 edges of cost 2
    // Divisible edges but not all connected: [2,4,3,6], p=5
    // 2 connects 0->1 cost2, 3 connects 2->3 cost3, need one p edge between 1 and2 => +5 => total10
    assert(minConnectionCost({2,4,3,6}, 5) == 10);
    // No cheap edges: [3,7,11], p=2 => all fail, need 2 edges of cost2 =4
    assert(minConnectionCost({3,7,11}, 2) == 4);
    // Duplicates and p smaller than some values
    // [10,5,5,20], p=7: 5 can connect 1-2 cost5, 10 can connect 0-1? 5%10 !=0, 20%10==0 but 0-1? 5%10!=0, so only 2? Let's test: 
    // Actually value 5 at index2 connects to index1 (cost5), value 5 at index1 connects to index0? 10%5==0 so cost5, and to index3? 20%5==0 cost5. Total cost 15, components=1 no p. 
    assert(minConnectionCost({10,5,5,20}, 7) == 15);
    // Large p, but cheap edges
    assert(minConnectionCost({6,6,6}, 100) == 12);
    // Edge case: p=1, all cheap edges cost even less than 1? No edges with value<1, so all p edges
    assert(minConnectionCost({2,3,4}, 1) == 2);
    // Mixed: [4,8,3,6,12], p=10
    // value 3 connects 2-3 cost3, value 6 connects 3-4? 12%6==0 cost6, value4 connects0-1 cost4, now comps: {0,1},{2,3,4} need p edge cost10 => total 23
    assert(minConnectionCost({4,8,3,6,12}, 10) == 23);
    return 0;
}

#include <vector>
#include <map>
#include <numeric>

// Union-Find helper for tracking connected components
class DisjointSet {
    std::vector<int> parent;
public:
    explicit DisjointSet(int n) : parent(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    void merge(int a, int b) {
        parent[find(b)] = find(a);
    }
};

// Minimize total connection cost using divisible edges (cost = left value) and fallback edges (cost = p)
long long minConnectionCost(const std::vector<int>& a, int p) {
    int n = static_cast<int>(a.size());
    std::map<int, std::vector<int>> positions;
    for (int i = 0; i < n; ++i) {
        positions[a[i]].push_back(i);
    }

    DisjointSet ds(n);
    long long totalCost = 0;

    for (const auto& entry : positions) {
        int value = entry.first;
        if (value >= p) {
            break;
        }
        for (int idx : entry.second) {
            // Extend left
            int left = idx - 1;
            while (left >= 0 && ds.find(left) != ds.find(idx) && a[left] % value == 0) {
                ds.merge(idx, left);
                totalCost += value;
                --left;
            }
            // Extend right
            int right = idx + 1;
            while (right < n && ds.find(right) != ds.find(idx) && a[right] % value == 0) {
                ds.merge(idx, right);
                totalCost += value;
                ++right;
            }
        }
    }

    int components = 0;
    for (int i = 0; i < n; ++i) {
        if (ds.find(i) == i) {
            ++components;
        }
    }
    totalCost += static_cast<long long>(components - 1) * p;
    return totalCost;
}

// The key observation is that edges with cost equal to a value `x` can only connect adjacent positions where the right neighbor is divisible by `x`. To minimize total cost, we should use the cheapest possible edges: any edge with cost less than `p` is always beneficial because it is cheaper than the fallback `p` edge. Thus, we process values in increasing order. For each distinct value `x` that is strictly less than `p`, we examine every occurrence of `x` in the array. From each occurrence at index `i`, we greedily extend left and right as long as the neighboring element is divisible by `x` and not already in the same connected component as `i`. We add an edge of cost `x` for each successful merge. Since edges are undirected and we use union-find to track components, we only merge distinct components. After processing all values `< p`, the remaining number of separate components is `k`. To connect these `k` components into one, we need `k-1` edges of cost `p`, so the final cost is the sum of all `x` edges plus `(k-1)*p`. The union-find operations are nearly O(α(n)) each, and each element is visited at most a constant number of times from left/right traversals, giving O(n α(n)) overall. Sorting the distinct values is O(n log n), but since we iterate by value order via a map, it's O(n log n) in the worst case. Space complexity is O(n) for the parent array and the value-to-index mapping.
