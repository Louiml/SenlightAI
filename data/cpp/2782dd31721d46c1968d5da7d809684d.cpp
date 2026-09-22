You are given a permutation of the integers from 1 to n. Consider a graph where each integer i is a vertex, and there is an undirected edge between i and the value at position i (i.e., if p[i] is the value at index i, then connect i and p[i]; note that self-loops are ignored because if p[i]==i, no edge is added). Write a C++ function `long long maximizeSumOfComponentSquares(int n, const std::vector<int>& perm)` that first finds all connected components of this graph, then combines the two smallest components into one by merging them (this merging step is done exactly once, only if there is at least one component). After this optional merge, compute the sum of the squares of the sizes of all resulting components. Return this sum as a `long long`. For example, if component sizes before merge are `{1,2,3}`, then after merging the two smallest (1 and 2) we get `{3,3}`, and the answer is `3*3 + 3*3 = 18`. If there are 0 or 1 components, the merge step is skipped and the answer is the sum of squares of existing sizes (0 if n=0, otherwise n^2 since the whole graph is one component if n>0). Constraints: n can be up to 10^6, and the input is a valid permutation with indices starting from 1 (so `perm` has size n, with `perm[i]` being the value at position i+1 in 1-based indexing). You must handle large n efficiently (O(n) time, O(n) space).
// We need to build an undirected graph where each vertex i (1..n) connects to perm[i] (if perm[i] != i). Since the input is a permutation, each vertex has degree exactly 2 (counting the self-loop as ignored, so the graph consists of disjoint cycles of length >= 2). Actually, if perm[i] == i, that vertex is isolated (component of size 1). So components are either isolated vertices or cycles. To find all component sizes, we can perform a DFS/BFS over all unvisited vertices. For each unvisited vertex, traverse all vertices reachable via the edges (both directions, but since it's a permutation, each vertex has exactly one outgoing edge to perm[i] and one incoming edge from the index j such that perm[j]==i; however for simplicity, we can just build adjacency lists with both directions, but that would use O(n) memory). A more memory-efficient approach: since each vertex has exactly one outgoing edge to perm[i], we can just follow the cycle by starting from an unvisited vertex and moving to perm[current], marking visited. When we return to the starting vertex, we have a component. For isolated vertices (perm[i]==i), they form a component of size 1. After collecting all component sizes in a vector, we ensure at least two components exist; if so, we find the smallest and second smallest sizes, remove them, and insert their sum. Then compute sum of squares of all sizes. Edge case: n=0 (empty permutation) should return 0. n=1: there is one isolated vertex, so component size 1, sum squares =1. If all vertices are in one big cycle (n>=2), then there is only one component, no merge, answer = n^2. Time complexity: O(n) to traverse all vertices, plus O(c log c) to sort or find two smallest (but we can do it in O(c) with two passes to find smallest and second smallest). Space: O(n) for visited array, and O(c) for storing component sizes. We can also avoid storing all component sizes by keeping track of smallest and second smallest during traversal, but we need to know if there are at least two components. So we can count components and maintain smallest and second smallest sizes. That improves to O(1) extra space besides visited, but O(n) for visited is necessary. Then after traversal, if count>=2, we replace the two smallest sizes with their sum, and compute sum of squares. Since we have only smallest and second smallest, we need to know all sizes to compute sum of squares after merging? No, because we can compute total sum of squares initially, and then adjust: initial sumSq = sum of size^2 for all components. If we merge smallest a and second smallest b into a+b, then new sumSq = oldSumSq - a^2 - b^2 + (a+b)^2. So we only need smallest and second smallest. That is O(n) time and O(n) memory for visited. The solution function should be robust and use `long long` for sizes because squares of up to 10^6 fit in 64-bit.
#include <vector>
#include <algorithm>
#include <cstdint>

// Compute sum of squares of component sizes after merging the two smallest components once.
long long maximizeSumOfComponentSquares(int n, const std::vector<int>& perm) {
    if (n == 0) return 0;

    std::vector<bool> visited(static_cast<size_t>(n) + 1, false); // indices 1..n

    long long smallest = -1;
    long long secondSmallest = -1;
    long long totalSumSq = 0;
    long long componentCount = 0;

    for (int i = 1; i <= n; ++i) {
        if (visited[i]) continue;

        // Traverse the cycle starting at i.
        int current = i;
        long long size = 0;
        while (!visited[current]) {
            visited[current] = true;
            ++size;
            current = perm[current - 1]; // perm is 0-indexed but values are 1..n
        }

        ++componentCount;
        totalSumSq += size * size;

        if (smallest == -1 || size < smallest) {
            secondSmallest = smallest;
            smallest = size;
        } else if (secondSmallest == -1 || size < secondSmallest) {
            secondSmallest = size;
        }
    }

    if (componentCount >= 2) {
        // Merge the two smallest components.
        totalSumSq = totalSumSq - smallest * smallest - secondSmallest * secondSmallest
                     + (smallest + secondSmallest) * (smallest + secondSmallest);
    }

    return totalSumSq;
}
#include <cassert>
#include <vector>

// The solution function is already defined above.
int main() {
    // Empty permutation
    assert(maximizeSumOfComponentSquares(0, {}) == 0);

    // Single element: isolated vertex
    std::vector<int> p1 = {1};
    assert(maximizeSumOfComponentSquares(1, p1) == 1);

    // Two-cycle: [2,1] -> one component of size 2, no merge
    std::vector<int> p2 = {2, 1};
    assert(maximizeSumOfComponentSquares(2, p2) == 4);

    // Three elements forming one 3-cycle: [2,3,1]
    std::vector<int> p3 = {2, 3, 1};
    assert(maximizeSumOfComponentSquares(3, p3) == 9);

    // Mixed: [1,3,2] -> component sizes: {1} (index 1), {2,3} (cycle)
    // Before merge: 1^2 + 2^2 = 5. Merge 1 and 2 -> size 3, sum squares = 9.
    std::vector<int> p4 = {1, 3, 2};
    assert(maximizeSumOfComponentSquares(3, p4) == 9);

    // Four elements: [2,1,4,3] -> two components of size 2 each.
    // Before merge: 4 + 4 = 8. Merge two 2's -> size 4, sum = 16.
    std::vector<int> p5 = {2, 1, 4, 3};
    assert(maximizeSumOfComponentSquares(4, p5) == 16);

    // Four elements: [1,2,3,4] -> all isolated, components {1,1,1,1}
    // Before merge: 4. Merge smallest two: new sizes {2,1,1}, sum = 4+1+1=6.
    std::vector<int> p6 = {1, 2, 3, 4};
    assert(maximizeSumOfComponentSquares(4, p6) == 6);

    // Five elements: [2,3,1,5,4] -> one 3-cycle and one 2-cycle.
    // Sizes {3,2}. Before merge: 9+4=13. Merge 3 and 2 -> size 5, sum=25.
    std::vector<int> p7 = {2, 3, 1, 5, 4};
    assert(maximizeSumOfComponentSquares(5, p7) == 25);

    // Five elements: [2,1,4,5,3] -> components: {2,1} size2, {4,5,3} size3? Let's check.
    // Actually: 1->2, 2->1 => size2. 3->4,4->5,5->3 => size3. So sizes {2,3}.
    // Merge -> size5 => 25.
    std::vector<int> p8 = {2, 1, 4, 5, 3};
    assert(maximizeSumOfComponentSquares(5, p8) == 25);

    // Large n test: identity permutation of size 100000.
    int n = 100000;
    std::vector<int> p9(n);
    for (int i = 0; i < n; ++i) p9[i] = i + 1;
    // All isolated: 100000 components of size 1.
    // Merge two smallest -> new sizes: {2, 1,1,...} (99998 ones).
    // Sum squares = 2^2 + 99998 * 1^2 = 4 + 99998 = 100002.
    assert(maximizeSumOfComponentSquares(n, p9) == 100002LL);

    return 0;
}
