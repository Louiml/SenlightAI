Write a C++ function `std::vector<int> movieQueue(int N, int M, const std::vector<int>& queries)` that simulates a DVD rental queue initially stacked with movies numbered `1` to `N`, placed at positions `M+1` through `M+N` on a long shelf (with positions `1` to `M` initially empty above the main stack). For each query (a movie number `n`), the function must output (in order) how many movies are currently above that movie (i.e., the number of movies whose current shelf position is smaller than the queried movie's position). After each query, the movie `n` is removed from its current position and placed at the top of the stack, which is the smallest available position; the top positions are filled from `M` downward (i.e., first query places at position `M`, second at `M-1`, etc., up to position `1`). The function should return a vector of integers containing the answers for each query in the same order as the input. You may assume all queries are valid movie numbers between `1` and `N`, and that `N` and `M` are positive integers. Implement the solution efficiently for large `N` and `M` (up to 10^5) and up to `M` queries.

#include <cassert>
#include <vector>

// The solution function is defined above (in Solution section).
// Assume the code from Solution is pasted here.

int main() {
    // Test 1: Basic from original snippet
    {
        int N = 5, M = 3;
        std::vector<int> queries = {3, 2, 5, 1, 4};
        std::vector<int> expected = {2, 1, 3, 0, 2};
        assert(movieQueue(N, M, queries) == expected);
    }
    // Test 2: Single movie, multiple queries
    {
        int N = 1, M = 2;
        std::vector<int> queries = {1, 1, 1};
        std::vector<int> expected = {0, 0, 0};
        assert(movieQueue(N, M, queries) == expected);
    }
    // Test 3: No queries
    {
        int N = 4, M = 2;
        std::vector<int> queries = {};
        std::vector<int> expected = {};
        assert(movieQueue(N, M, queries) == expected);
    }
    // Test 4: Queries in reverse order of original positions
    {
        int N = 3, M = 1;
        std::vector<int> queries = {3, 2, 1};
        std::vector<int> expected = {2, 1, 0};
        assert(movieQueue(N, M, queries) == expected);
    }
    // Test 5: All queries are the same movie
    {
        int N = 3, M = 3;
        std::vector<int> queries = {2, 2, 2};
        std::vector<int> expected = {1, 0, 0};
        assert(movieQueue(N, M, queries) == expected);
    }
    // Test 6: M larger than N
    {
        int N = 2, M = 5;
        std::vector<int> queries = {1, 2};
        std::vector<int> expected = {0, 0};
        assert(movieQueue(N, M, queries) == expected);
    }
    // Test 7: Larger random-like sequence (small scale)
    {
        int N = 4, M = 2;
        std::vector<int> queries = {4, 3, 2, 1, 4};
        std::vector<int> expected = {3, 2, 1, 0, 1};
        assert(movieQueue(N, M, queries) == expected);
    }
    
    return 0;
}

#include <vector>
#include <cstddef>

class FenwickTree {
public:
    FenwickTree(int size) : tree(size + 1, 0) {}
    
    void add(int idx, int value) {
        for (; idx < (int)tree.size(); idx += idx & -idx) {
            tree[idx] += value;
        }
    }
    
    int sum(int idx) const {
        int result = 0;
        for (; idx > 0; idx -= idx & -idx) {
            result += tree[idx];
        }
        return result;
    }
    
private:
    std::vector<int> tree;
};

// Simulates the movie queue and returns the count of movies above each queried movie.
std::vector<int> movieQueue(int N, int M, const std::vector<int>& queries) {
    std::vector<int> result;
    result.reserve(queries.size());
    
    FenwickTree bit(N + M);
    std::vector<int> position(N + 1, 0);
    
    // Initially place movie i at position M+i
    for (int i = 1; i <= N; ++i) {
        position[i] = M + i;
        bit.add(M + i, 1);
    }
    
    int frontPos = M; // next front position (decrements)
    
    for (int movie : queries) {
        int current = position[movie];
        int above = bit.sum(current - 1);
        result.push_back(above);
        
        bit.add(current, -1); // remove from old position
        bit.add(frontPos, 1); // place at front
        position[movie] = frontPos;
        --frontPos;
    }
    
    return result;
}

// The problem is a classic application of a Fenwick tree (Binary Indexed Tree) to track counts of movies at each shelf position, while dynamically relocating queried movies to the front. Initially, the shelf has positions `1` to `M+N`. For each movie `i` (1-indexed), we place it at position `M+i`, and set the BIT to have a `1` at that position (meaning a movie occupies it). For each query `n`, we need the number of movies currently above it, which is the prefix sum of positions less than its current position. We query `sum(currentPos - 1)`. Then we remove the movie from its current position by `update(currentPos, -1)`, and place it at the new front position. The front positions are assigned in decreasing order: the first query uses position `M`, the second `M-1`, … down to `1`. So we maintain a counter `frontPos` starting at `M`, and after each query, we set the movie's new position to `frontPos`, update the BIT by `+1` at that new position, and decrement `frontPos`. This ensures all positions `1` to `M` become occupied after exactly `M` queries; since there are at most `M` queries, this is valid. Edge cases: when the queried movie is already at the top, the sum is zero (correct). When `M` is large relative to `N`, the BIT size is `N+M`, which is at most 2*10^5, fine. Time complexity per query is O(log(N+M)) for each sum and two updates, so total O(M log(N+M)). Space complexity O(N+M) for the BIT and position map.
