You are given a list of XOR-based queries. Initially, you have an empty list of integers (conceptually containing a single element `0`). Each query is either of type `0 x` or type `1 x`. For a type `0 x` query, the integer `x` is appended to the current list. For a type `1 x` query, all existing elements in the list (including any previously appended values and the initial `0`) are XORed with `x`. However, the operations must be processed in reverse order of the given queries. Specifically, when processing from the last query to the first, a type `1` query applies a global XOR `totalXor` to all elements that were present before that query (in the original forward direction, meaning elements appended earlier). When you encounter a type `0` query (which originally appended `v`), you must compute the actual value that `v` would have had at the final state by XORing `v` with the current accumulated `totalXor` (the XORs from all later type-1 queries). After processing all queries in reverse, the initial `0` element must also be included, XORed with the final accumulated `totalXor`. Finally, return the sorted list of all resulting integers. Implement a C++ function that takes an integer `q` (number of queries) and a 2D vector `queries` (each inner vector has exactly two integers) and returns the sorted vector of final list values.

The key insight is to process the queries from the end to the beginning to avoid repeatedly XORing all existing elements. We maintain a running `totalXor` that represents the XOR of all type-1 XOR operations that occur chronologically *after* the current position in the reverse scan. Initially, `totalXor = 0`. When we encounter a type-1 query (u == 1) while scanning backward, we update `totalXor ^= v` because this XOR applies to all elements appended before this query in the forward direction (i.e., those that are encountered later in the reverse scan). When we encounter a type-0 query (u == 0), the original value `v` was appended, but later type-1 XORs would have modified it in the final list. In the reverse scan, the current `totalXor` accumulates exactly those later XORs, so the final value for that element is `totalXor ^ v`. We push this into the answer vector. After scanning all queries, we also need to include the initial element `0`; its final value is `totalXor ^ 0 = totalXor`. Finally, we sort the answer vector. This works correctly because XOR is commutative and associative, and an element appended at position i is only affected by type-1 queries that come after i in the forward order. Edge cases: there may be no type-0 queries at all, in which case only the initial zero remains (after applying all XORs). Also, if there are no queries at all (`q=0`), the result is `{0}`. Time complexity is O(n + m log m) where n is the number of queries and m is the number of elements in the final list (which equals number of type-0 queries + 1), dominated by the sorting step. Space complexity is O(m) for the answer vector.

#include <vector>
#include <algorithm>

// Given q XOR-based queries, return the sorted final list of integers.
// Queries are pairs: {type, value}. type 1 = XOR all current elements by value.
// type 0 = append value to the list. Initial list contains a single 0.
std::vector<int> constructList(int q, std::vector<std::vector<int>>& queries) {
    std::vector<int> result;
    int totalXor = 0;
    
    // Process queries in reverse order
    for (int i = q - 1; i >= 0; --i) {
        int type = queries[i][0];
        int value = queries[i][1];
        if (type == 1) {
            totalXor ^= value;
        } else { // type == 0
            result.push_back(totalXor ^ value);
        }
    }
    
    // Include the initial zero element
    result.push_back(totalXor ^ 0);
    
    // Sort the final list
    std::sort(result.begin(), result.end());
    
    return result;
}

#include <cassert>
#include <vector>

// Function declaration (from solution)
std::vector<int> constructList(int q, std::vector<std::vector<int>>& queries);

int main() {
    // Test 1: simple append then XOR
    {
        std::vector<std::vector<int>> queries = {{0, 5}, {1, 3}, {0, 2}};
        std::vector<int> result = constructList(3, queries);
        std::vector<int> expected = {2, 6}; // Explanation: forward: start [0], append 5 -> [0,5], XOR all by 3 -> [3,6], append 2 -> [3,6,2], sorted [2,3,6]? Wait check: actually XOR all by 3 gives [0^3,5^3]=[3,6], then append 2 -> [3,6,2], sorted [2,3,6]. But our reverse algorithm: totalXor=0; i=2 type0 v=2 -> push 2; i=1 type1 totalXor=3; i=0 type0 v=5 -> push 3^5=6; final push 3^0=3; result {2,6,3} sorted {2,3,6}. Expected {2,3,6}.
        assert(result == std::vector<int>({2, 3, 6}));
    }
    // Test 2: only XOR queries
    {
        std::vector<std::vector<int>> queries = {{1, 4}, {1, 2}};
        std::vector<int> result = constructList(2, queries);
        // Forward: start [0], XOR by 4 -> [4], XOR by 2 -> [6]. Final [6].
        assert(result == std::vector<int>({6}));
    }
    // Test 3: no queries
    {
        std::vector<std::vector<int>> queries;
        std::vector<int> result = constructList(0, queries);
        assert(result == std::vector<int>({0}));
    }
    // Test 4: only append queries
    {
        std::vector<std::vector<int>> queries = {{0, 10}, {0, -3}, {0, 7}};
        std::vector<int> result = constructList(3, queries);
        // Forward: never XOR, just append to initial 0: [0,10,-3,7] sorted [-3,0,7,10]
        assert(result == std::vector<int>({-3, 0, 7, 10}));
    }
    // Test 5: XOR before append (later XOR does not affect future append)
    {
        std::vector<std::vector<int>> queries = {{1, 5}, {0, 2}};
        std::vector<int> result = constructList(2, queries);
        // Forward: start [0], XOR by 5 -> [5], append 2 -> [5,2] sorted [2,5]
        assert(result == std::vector<int>({2, 5}));
    }
    // Test 6: multiple XORs and appends
    {
        std::vector<std::vector<int>> queries = {{0, 1}, {1, 2}, {0, 3}, {1, 4}};
        std::vector<int> result = constructList(4, queries);
        // Forward: [0] -> append1 [0,1] -> XOR2 [2,3] -> append3 [2,3,3] -> XOR4 [6,7,7] sorted [6,7,7]
        assert(result == std::vector<int>({6, 7, 7}));
    }
    return 0;
}
