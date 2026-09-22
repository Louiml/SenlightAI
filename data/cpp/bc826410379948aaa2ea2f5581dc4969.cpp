/*
Write a C++ function `shiftNodeLabels` that takes two integers `n` (number of original vertices) and `m` (number of edges), followed by a list of `m` undirected edges given as pairs `(from, to)` where both endpoints are in the range `[1, n]`. The function must output each edge with its second endpoint shifted by `+n`, i.e., for each input pair `(from, to)`, print `from` followed by `to + n`. However, because this is a pure function, it should collect the results into a `std::vector<std::pair<int,int>>` and return that vector instead of printing. The task is to implement this transformation exactly, preserving the order of edges as given. The function must handle edge cases where `n` and `m` are up to \(10^5\) and where edges may contain duplicate pairs or self‑loops. The returned vector must contain pairs where the first element is the original `from` and the second element is `to + n`.
*/
#include <vector>
#include <utility>

// Shift the second endpoint of each edge by adding n.
// Input: n = number of original vertices, m = number of edges,
//        edges = vector of pairs (from, to) with 1 <= from,to <= n.
// Return: vector of pairs where each pair is (from, to + n) in the same order.
std::vector<std::pair<int,int>> shiftNodeLabels(int n, int m,
                                                const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::pair<int,int>> result;
    result.reserve(m);   // avoid reallocation for known size
    for (const auto& e : edges) {
        int from = e.first;
        int to   = e.second;
        result.emplace_back(from, to + n);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <utility>

// The solution function (copy here or include from above)
std::vector<std::pair<int,int>> shiftNodeLabels(int n, int m,
                                                const std::vector<std::pair<int,int>>& edges) {
    std::vector<std::pair<int,int>> result;
    result.reserve(m);
    for (const auto& e : edges) {
        result.emplace_back(e.first, e.second + n);
    }
    return result;
}

int main() {
    // Case 1: simple example from snippet
    {
        int n = 3, m = 2;
        std::vector<std::pair<int,int>> edges = {{1,2}, {2,3}};
        auto result = shiftNodeLabels(n, m, edges);
        assert(result.size() == 2);
        assert(result[0] == std::make_pair(1, 5)); // 2+3
        assert(result[1] == std::make_pair(2, 6)); // 3+3
    }
    // Case 2: m = 0
    {
        int n = 5, m = 0;
        std::vector<std::pair<int,int>> edges;
        auto result = shiftNodeLabels(n, m, edges);
        assert(result.empty());
    }
    // Case 3: self-loop and duplicate
    {
        int n = 2, m = 3;
        std::vector<std::pair<int,int>> edges = {{1,1}, {2,2}, {1,2}};
        auto result = shiftNodeLabels(n, m, edges);
        assert(result.size() == 3);
        assert(result[0] == std::make_pair(1, 3)); // 1+2
        assert(result[1] == std::make_pair(2, 4)); // 2+2
        assert(result[2] == std::make_pair(1, 4)); // 2+2
    }
    // Case 4: maximum label shift
    {
        int n = 100000, m = 1;
        std::vector<std::pair<int,int>> edges = {{100000, 100000}};
        auto result = shiftNodeLabels(n, m, edges);
        assert(result[0] == std::make_pair(100000, 200000));
    }
    // Case 5: all from=1, to varies
    {
        int n = 4, m = 3;
        std::vector<std::pair<int,int>> edges = {{1,1}, {1,2}, {1,4}};
        auto result = shiftNodeLabels(n, m, edges);
        assert(result[0] == std::make_pair(1, 5));
        assert(result[1] == std::make_pair(1, 6));
        assert(result[2] == std::make_pair(1, 8));
    }
    return 0;
}
// The solution is straightforward: read the two integers `n` and `m`, then for each of the `m` lines, read `from` and `to`, and append `{from, to + n}` to a result vector. No sorting, deduplication, or graph construction is required. The transformation is a simple label shift on the second endpoint. The main edge cases are:  
// - When `m = 0`, the function returns an empty vector.  
// - When `to = n`, the shifted value becomes `2n`, which may exceed the original label range but is correct per the specification.  
// - Duplicate edges and self‑loops (`from == to`) are preserved as given.  
// Time complexity is \(O(m)\) because each edge is processed once. Space complexity is \(O(m)\) to store the result, or \(O(1)\) auxiliary if the result is produced on the fly (but here we return a vector, so it’s \(O(m)\)). The implementation must use appropriate constant references for inputs and avoid any unintended modifications.
