/*
Given a 0-indexed integer array `nums`, write a C++ function `bool canTraverseAllPairs(const std::vector<int>& nums)` that returns `true` if for every pair of distinct indices `(i, j)` there exists a sequence of allowed traversals connecting them, where a traversal between indices `i` and `j` is permitted if and only if `gcd(nums[i], nums[j]) > 1`. Return `false` otherwise. The length of `nums` is between 1 and 10^5, and each element is between 1 and 10^5. Note that if any element equals `1`, then that index cannot connect to any other, so the result must be `false` (unless the array has length 1, which trivially returns `true`). Your solution must be efficient for the given constraints.
*/
#include <vector>
#include <unordered_map>
#include <numeric>

namespace {
    // DSU with path compression and union by size
    int findRoot(std::vector<int>& parent, int x) {
        if (parent[x] != x) {
            parent[x] = findRoot(parent, parent[x]);
        }
        return parent[x];
    }

    void unionSets(std::vector<int>& parent, std::vector<int>& sz, int a, int b) {
        a = findRoot(parent, a);
        b = findRoot(parent, b);
        if (a == b) return;
        if (sz[a] < sz[b]) {
            std::swap(a, b);
        }
        parent[b] = a;
        sz[a] += sz[b];
    }
}

// Returns true if all pairs of indices are connected via gcd > 1 edges.
bool canTraverseAllPairs(const std::vector<int>& nums) {
    const int n = static_cast<int>(nums.size());
    if (n == 1) {
        return true;
    }

    std::vector<int> parent(n);
    std::vector<int> sz(n, 1);
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }

    // Maps a prime factor to the first index that contains it.
    std::unordered_map<int, int> firstIndexWithPrime;

    for (int i = 0; i < n; ++i) {
        int x = nums[i];
        if (x == 1) {
            return false; // 1 has no prime factors, cannot connect to any other index
        }

        // Factor x by trial division
        for (int d = 2; d * d <= x; ++d) {
            if (x % d == 0) {
                if (firstIndexWithPrime.count(d)) {
                    unionSets(parent, sz, i, firstIndexWithPrime[d]);
                } else {
                    firstIndexWithPrime[d] = i;
                }
                while (x % d == 0) {
                    x /= d;
                }
            }
        }

        // If x > 1 after loop, it is a prime factor
        if (x > 1) {
            if (firstIndexWithPrime.count(x)) {
                unionSets(parent, sz, i, firstIndexWithPrime[x]);
            } else {
                firstIndexWithPrime[x] = i;
            }
        }
    }

    int root0 = findRoot(parent, 0);
    return sz[root0] == n;
}
#include <cassert>
#include <vector>
#include <iostream>

int main() {
    // Example 1
    {
        std::vector<int> nums = {2, 3, 6};
        assert(canTraverseAllPairs(nums) == true);
    }

    // Example 2
    {
        std::vector<int> nums = {3, 9, 5};
        assert(canTraverseAllPairs(nums) == false);
    }

    // Example 3
    {
        std::vector<int> nums = {4, 3, 12, 8};
        assert(canTraverseAllPairs(nums) == true);
    }

    // Single element (always true)
    {
        std::vector<int> nums = {7};
        assert(canTraverseAllPairs(nums) == true);
    }

    // Single element equal to 1 (trivially true)
    {
        std::vector<int> nums = {1};
        assert(canTraverseAllPairs(nums) == true);
    }

    // Contains a 1 with length > 1 (cannot connect)
    {
        std::vector<int> nums = {1, 2};
        assert(canTraverseAllPairs(nums) == false);
    }

    // All elements share a common prime factor (2)
    {
        std::vector<int> nums = {2, 4, 6, 8, 10};
        assert(canTraverseAllPairs(nums) == true);
    }

    // Two isolated components
    {
        std::vector<int> nums = {6, 10, 15, 7, 11};
        // 6(2*3), 10(2*5), 15(3*5) all connect, 7 and 11 are isolated primes
        assert(canTraverseAllPairs(nums) == false);
    }

    // All primes but including 1 (must be false)
    {
        std::vector<int> nums = {2, 3, 5, 7, 11, 1};
        assert(canTraverseAllPairs(nums) == false);
    }

    // Large composite numbers that connect via shared prime factor
    {
        std::vector<int> nums = {99991, 99991 * 2, 2 * 3, 3 * 5, 5 * 7};
        // First two share 99991, then 2 connects to third, 3 to fourth, 5 to fifth, all connected
        assert(canTraverseAllPairs(nums) == true);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
// The problem reduces to checking whether all indices belong to the same connected component in an undirected graph where vertices are indices and edges exist between indices whose values share a common prime factor. A direct pairwise GCD check would be O(n^2) which is too slow. Instead, we use a Union-Find (Disjoint Set Union) data structure. We iterate over each number and factor it completely (trial division up to sqrt(x)). For each distinct prime factor `p` of `nums[i]`, we maintain a map from `p` to the first index that has `p` as a factor. When we encounter a new index with factor `p`, we union that index with the previously stored index for `p`. After processing all numbers, we check if all indices belong to the same set by verifying that the size of the set containing index 0 equals `n`. If any number is `1`, it has no prime factors and thus cannot connect to any other index, so we immediately return `false` (unless `n == 1`, which is handled early). Edge cases: `nums = [1]` returns `true`; `nums = [1,2]` returns `false`; duplicate numbers are handled naturally; numbers that are prime themselves are handled after the trial division loop. Time complexity: For each number, factoring takes O(sqrt(max(nums[i]))) worst-case, but since we divide out factors, the inner loop is O(number of distinct prime factors × log value). Overall about O(n × sqrt(M)) worst-case, but with M ≤ 1e5, this is acceptable. Space complexity: O(n) for the DSU arrays plus O(number of distinct prime factors across all numbers) for the map, which is at most O(n log M). The DSU operations are near-constant with path compression and union by size.
