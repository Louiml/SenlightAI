Write a C++ function that simulates the behavior of the given code snippet. The function should take a circular doubly linked list of nodes numbered `1` through `N` (initially where node `i` is linked to node `i+1` for `i < N`, and node `N` links to node `1`, with the reverse links also set accordingly), and process a sequence of operations. Each operation is either:  
- `'A' a b`: move node `a` from its current position and insert it immediately before node `b` (so that in the resulting circular list, `a`'s left neighbor becomes `b`'s old left neighbor, and `a`'s right neighbor becomes `b`).  
- `'B' a b`: move node `a` from its current position and insert it immediately after node `b` (so that `a`'s right neighbor becomes `b`'s old right neighbor, and `a`'s left neighbor becomes `b`).  
- `'Q' a b`: query and return the following: if `a == 1`, return the node immediately to the right of `b`; otherwise, return the node immediately to the left of `b`.  
All operations are guaranteed valid: nodes `a` and `b` are distinct, and they exist in the current list. After all operations, the final list state is not needed—only the results of queries, in order, which should be collected into a `std::vector<int>` and returned. The function must not modify the global state and must be self-contained.

#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Test 1: Basic A and B moves with queries
    {
        int N = 4;
        std::vector<std::tuple<char,int,int>> ops = {
            {'Q', 1, 2},   // right of 2 is 3
            {'A', 3, 1},   // move 3 before 1: list becomes 3,1,2,4
            {'Q', 1, 1},   // right of 1 is 2
            {'B', 4, 2},   // move 4 after 2: list becomes 3,1,2,4
            {'Q', 0, 4}    // left of 4 is 2
        };
        auto res = simulateList(N, ops);
        assert(res.size() == 3);
        assert(res[0] == 3);
        assert(res[1] == 2);
        assert(res[2] == 2);
    }

    // Test 2: N=1 (no valid operations, but must not crash)
    {
        int N = 1;
        std::vector<std::tuple<char,int,int>> ops = {};
        auto res = simulateList(N, ops);
        assert(res.empty());
    }

    // Test 3: Queries only, no moves
    {
        int N = 5;
        std::vector<std::tuple<char,int,int>> ops = {
            {'Q', 1, 1},  // right of 1 = 2
            {'Q', 0, 5},  // left of 5 = 4
            {'Q', 1, 5},  // right of 5 = 1
            {'Q', 0, 1}   // left of 1 = 5
        };
        auto res = simulateList(N, ops);
        assert(res.size() == 4);
        assert(res[0] == 2);
        assert(res[1] == 4);
        assert(res[2] == 1);
        assert(res[3] == 5);
    }

    // Test 4: Moving a node multiple times
    {
        int N = 3;
        std::vector<std::tuple<char,int,int>> ops = {
            {'A', 2, 1},  // list: 2,1,3
            {'B', 3, 2},  // list: 2,3,1
            {'Q', 1, 1},  // right of 1 = 2
            {'Q', 0, 2}   // left of 2 = 3
        };
        auto res = simulateList(N, ops);
        assert(res.size() == 2);
        assert(res[0] == 2);
        assert(res[1] == 3);
    }

    // Test 5: Large N and many operations (stress check efficiency)
    {
        int N = 100000;
        std::vector<std::tuple<char,int,int>> ops;
        for (int i = 0; i < 1000; ++i) {
            ops.emplace_back('A', (i % N) + 1, ((i*7) % N) + 1);
            ops.emplace_back('Q', 1, (i % N) + 1);
        }
        auto res = simulateList(N, ops);
        assert(res.size() == 1000);
        for (int r : res) {
            assert(r >= 1 && r <= N);
        }
    }

    return 0;
}

#include <vector>
#include <cstddef>

// Simulates a circular doubly linked list of nodes 1..N and processes operations.
// Returns the results of all 'Q' queries in order.
std::vector<int> simulateList(int N, const std::vector<std::tuple<char,int,int>>& operations) {
    // Use 1-indexed arrays for nodes 1..N
    std::vector<int> left(N + 2, 0);
    std::vector<int> right(N + 2, 0);

    // Initialize the circular doubly linked list
    for (int i = 1; i <= N; ++i) {
        left[i] = (i == 1) ? N : i - 1;
        right[i] = (i == N) ? 1 : i + 1;
    }

    std::vector<int> queryResults;
    queryResults.reserve(operations.size());

    for (const auto& op : operations) {
        char c = std::get<0>(op);
        int a = std::get<1>(op);
        int b = std::get<2>(op);

        if (c == 'A' || c == 'B') {
            // Step 1: Remove node 'a' from its current position
            right[left[a]] = right[a];
            left[right[a]] = left[a];

            // Step 2: Insert 'a' at the new position
            if (c == 'A') {
                // Insert 'a' before 'b'
                right[a] = b;
                left[a] = left[b];
                right[left[a]] = a;  // left[a] is the old left of b
                left[b] = a;
            } else { // 'B'
                // Insert 'a' after 'b'
                left[a] = b;
                right[a] = right[b];
                left[right[a]] = a;  // right[a] is the old right of b
                right[b] = a;
            }
        } else { // 'Q' query
            if (a == 1) {
                queryResults.push_back(right[b]);
            } else {
                queryResults.push_back(left[b]);
            }
        }
    }

    return queryResults;
}

// The problem is a classic circular doubly linked list manipulation. The key is to correctly maintain the `left` and `right` pointers for each node. Initially, for nodes `1..N`, set `left[i] = i-1` (with `left[1] = N`) and `right[i] = i+1` (with `right[N] = 1`). For each operation:  
// - For `A` or `B`, first remove node `a` from its current position by linking its left and right neighbors together: `right[left[a]] = right[a]` and `left[right[a]] = left[a]`. Then insert `a` at the desired place. For `A`, `a` goes before `b`: set `right[a] = b`, `left[a] = left[b]`, then update `right[left[b]] = a` and `left[b] = a`. For `B`, `a` goes after `b`: set `left[a] = b`, `right[a] = right[b]`, then update `left[right[b]] = a` and `right[b] = a`.  
// - For `Q`, simply check if `a == 1`; if so, output `right[b]`, else `left[b]`.  
// Edge cases: `N` can be as small as 1, though operations guarantee distinct `a` and `b`; if `N=1`, no valid operations exist, but the function must handle it by returning an empty vector. When moving a node, it is essential to first remove it completely before re-inserting, otherwise cross-links may cause cycles. Time complexity: each operation is O(1), so for `M` operations it is O(N + M) to initialize and process. Space complexity: O(N) for the two pointer arrays.
