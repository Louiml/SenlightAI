Write a C++ function that implements a disjoint-set (union-find) data structure supporting four operations: `make_set(x)` to create a new singleton set containing integer `x`, `find_id(x)` to return the representative (root) of the set containing `x` (creating `x` first if it does not exist), `make_union(x, y)` to merge the sets containing `x` and `y` and return the representative of the merged set (again, creating missing elements first), and `set_size(x)` to return the number of elements in the set containing `x`. The function should accept a vector of pairs of operations, where each pair consists of an integer operation code (1 = make_set, 2 = find_id, 3 = make_union, 4 = set_size) and an integer argument (for find_id and set_size, the argument is `x`; for make_union, the two integers are provided in the same pair—you may design the input format as a vector of tuples if simpler). The function should return a `std::vector<long long>` containing the results of only `find_id` and `set_size` operations, in the order they appear. The implementation must use path compression for `find_id` and union by rank for `make_union`. All values are non-negative integers up to 10^5, and at most 10^5 operations are performed. The function signature is: `std::vector<long long> processOperations(const std::vector<std::vector<long long>>& ops)` where each inner vector encodes an operation: `[code, x]` for codes 1, 2, 4, and `[code, x, y]` for code 3.
#include <cassert>
#include <vector>

// Declare the solution function (assumed to be in same translation unit or included)
std::vector<long long> processOperations(const std::vector<std::vector<long long>>& ops);

int main() {
    // Basic make_set and find
    assert(processOperations({{1, 5}, {2, 5}}) == std::vector<long long>({5}));
    // find on non-existing element creates it
    assert(processOperations({{2, 10}}) == std::vector<long long>({10}));
    // size of single element set
    assert(processOperations({{4, 3}}) == std::vector<long long>({1}));
    // union merging two sets and size check
    assert(processOperations({{3, 1, 2}, {4, 1}}) == std::vector<long long>({2}));
    // union with repeated elements and find
    assert(processOperations({{3, 1, 2}, {3, 2, 1}, {2, 1}}) == std::vector<long long>({1}));
    // larger chain: union 1-2, 3-4, then 1-3 -> size of 1 is 4
    assert(processOperations({{3, 1, 2}, {3, 3, 4}, {3, 1, 3}, {4, 1}}) == std::vector<long long>({4}));
    // non-union sets remain separate
    assert(processOperations({{3, 1, 2}, {3, 3, 4}, {4, 1}, {4, 3}}) == std::vector<long long>({2, 2}));
    // find after union returns same representative for all members
    auto res = processOperations({{3, 7, 8}, {2, 7}, {2, 8}});
    assert(res.size() == 2 && res[0] == res[1]);
    // multiple unions with overlapping sets
    assert(processOperations({{3, 1, 2}, {3, 2, 3}, {3, 4, 5}, {4, 1}, {4, 4}}) == std::vector<long long>({3, 2}));
    // set_size of a representative after merging with self
    assert(processOperations({{3, 1, 1}, {4, 1}}) == std::vector<long long>({1}));
    // large union chain to test correctness
    auto largeOps = std::vector<std::vector<long long>>{};
    for (long long i = 1; i <= 100; ++i) largeOps.push_back({1, i});
    for (long long i = 1; i < 100; ++i) largeOps.push_back({3, i, i+1});
    largeOps.push_back({4, 1});
    assert(processOperations(largeOps) == std::vector<long long>({100}));
    return 0;
}
#include <vector>
#include <unordered_map>
#include <algorithm>

// Process a sequence of disjoint-set operations and return the results of find and size queries.
// ops: each inner vector is [code, x] for codes 1 (make_set), 2 (find), 4 (size),
//      or [code, x, y] for code 3 (union).
// Returns a vector of results from codes 2 and 4, in order.
std::vector<long long> processOperations(const std::vector<std::vector<long long>>& ops) {
    std::unordered_map<long long, long long> parent;   // element -> its parent (root points to itself)
    std::unordered_map<long long, long long> rank;     // only meaningful for roots
    std::unordered_map<long long, long long> size;     // only meaningful for roots

    auto make_set = [&](long long x) {
        if (parent.find(x) == parent.end()) {
            parent[x] = x;
            rank[x] = 0;
            size[x] = 1;
        }
    };

    // Iterative find with path compression (returns root of the set containing x)
    auto find = [&](long long x) -> long long {
        make_set(x); // ensure existence
        long long root = x;
        while (parent[root] != root) {
            root = parent[root];
        }
        // Path compression: make every node on the path point directly to root
        long long cur = x;
        while (cur != root) {
            long long next = parent[cur];
            parent[cur] = root;
            cur = next;
        }
        return root;
    };

    auto union_sets = [&](long long a, long long b) -> long long {
        long long ra = find(a);
        long long rb = find(b);
        if (ra == rb) return ra;
        // Union by rank
        if (rank[ra] < rank[rb]) {
            parent[ra] = rb;
            size[rb] += size[ra];
            return rb;
        } else if (rank[ra] > rank[rb]) {
            parent[rb] = ra;
            size[ra] += size[rb];
            return ra;
        } else {
            parent[rb] = ra;
            size[ra] += size[rb];
            rank[ra]++;
            return ra;
        }
    };

    std::vector<long long> results;
    for (const auto& op : ops) {
        long long code = op[0];
        long long x = op[1];
        if (code == 1) {
            make_set(x);
        } else if (code == 2) {
            results.push_back(find(x));
        } else if (code == 3) {
            long long y = op[2];
            union_sets(x, y);
        } else if (code == 4) {
            long long r = find(x);
            results.push_back(size[r]);
        }
    }
    return results;
}
// The core algorithm is the classic disjoint-set union (DSU) with two optimizations: path compression and union by rank. Path compression flattens the tree structure during `find` so that subsequent finds are nearly O(1). Union by rank attaches the tree with lower rank under the root of the tree with higher rank, keeping trees shallow. For `make_set`, we create a new node if not already present, initializing its parent to itself and rank to 0. For `find_id`, we first call `make_set(x)` to ensure the node exists (per the task spec), then recursively or iteratively find the parent, compressing the path. For `make_union`, we ensure both nodes exist, find their roots, and if different, merge by rank: if ranks equal, increment the new root's rank; attach the lower-rank root under the higher-rank one. `set_size` needs to know the size of the set; we maintain a separate `size` array per root (only meaningful for roots), updating it during union. Alternatively, we can compute size by traversing all elements each time, but that would be O(N) per query; better to maintain sizes. Edge cases: repeated `make_set` on same value should be idempotent; `find_id` and `set_size` on a non-created element must create it first (resulting in size 1); union with elements already in the same set must not change anything. Time complexity: each operation is nearly O(α(N)) amortized, where α is the inverse Ackermann function, practically constant. Space complexity: O(N) for storing parent, rank, and size, plus the per-member mapping from value to node index.
