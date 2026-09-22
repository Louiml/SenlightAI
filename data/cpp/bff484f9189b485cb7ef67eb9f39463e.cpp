// Write a C++ function that, given two vectors `A` and `B` of equal length `N` (each containing integers, possibly negative, duplicates allowed) and a positive integer `C` (where `C <= N*N`), returns a vector of the `C` largest distinct sum combinations formed by adding one element from `A` and one element from `B`. If `C` exceeds the number of possible distinct sums, return all distinct sums sorted in descending order. For example, if `A = [1, 2]` and `B = [3, 4]`, all possible sums are `4, 5, 5, 6`; distinct sums are `4, 5, 6`, so for `C = 2` return `[6, 5]`. The function must be `const`-correct and handle edge cases like empty vectors (return empty) and `C = 0` (return empty).
#include <cassert>
#include <vector>

// (The solution function is included above; this main tests it.)
int main() {
    // Basic test
    std::vector<int> A1 = {1, 2};
    std::vector<int> B1 = {3, 4};
    auto res1 = largestDistinctSums(A1, B1, 2);
    assert(res1 == std::vector<int>({6, 5}));

    // Duplicate sums: A=[1,1], B=[1,1] gives sums 2,2,2,2 -> distinct sum is {2}
    std::vector<int> A2 = {1, 1};
    std::vector<int> B2 = {1, 1};
    auto res2 = largestDistinctSums(A2, B2, 3);
    assert(res2 == std::vector<int>({2}));

    // C=0 returns empty
    auto res3 = largestDistinctSums({1,2}, {3,4}, 0);
    assert(res3.empty());

    // Empty vectors return empty
    std::vector<int> empty;
    auto res4 = largestDistinctSums(empty, {3,4}, 2);
    assert(res4.empty());

    // Negative numbers
    std::vector<int> A5 = {-1, -2};
    std::vector<int> B5 = {-3, -4};
    auto res5 = largestDistinctSums(A5, B5, 2);
    // All sums: -4,-5,-5,-6 -> distinct descending: -4,-5,-6 -> top 2: -4,-5
    assert(res5 == std::vector<int>({-4, -5}));

    // C larger than distinct sums returns all distinct
    std::vector<int> A6 = {1};
    std::vector<int> B6 = {2, 3};
    auto res6 = largestDistinctSums(A6, B6, 10);
    assert(res6 == std::vector<int>({4, 3}));

    // Test with duplicates and negatives together
    std::vector<int> A7 = {5, 5, 1};
    std::vector<int> B7 = {4, 2, 0};
    auto res7 = largestDistinctSums(A7, B7, 5);
    // Distinct sums: 9 (5+4), 7 (5+2, also 5+2), 5 (5+0, 1+4, 5+0), 3 (1+2), 1 (1+0)
    // Sorted descending: 9,7,5,3,1 -> top 5
    assert(res7 == std::vector<int>({9, 7, 5, 3, 1}));

    return 0;
}
#include <vector>
#include <queue>
#include <set>
#include <algorithm>
#include <functional> // for std::greater

// Return the C largest distinct sum combinations from A and B, sorted descending.
std::vector<int> largestDistinctSums(std::vector<int> A, std::vector<int> B, int C) {
    if (A.empty() || B.empty() || C <= 0) {
        return {};
    }
    // Sort descending to easily access largest sums
    std::sort(A.begin(), A.end(), std::greater<int>());
    std::sort(B.begin(), B.end(), std::greater<int>());

    // Result vector
    std::vector<int> result;
    // Max-heap storing sums; each element: (sum, indexA, indexB)
    using Node = std::tuple<int, int, int>;
    // priority_queue is max-heap by default; we want largest sum first
    std::priority_queue<Node> pq;
    pq.push({A[0] + B[0], 0, 0});

    // Visited pairs to avoid duplicates
    std::set<std::pair<int,int>> visited;
    visited.insert({0,0});

    while (!pq.empty() && result.size() < static_cast<size_t>(C)) {
        auto [sum, i, j] = pq.top();
        pq.pop();
        // Only add if this sum is new (distinct)
        if (result.empty() || result.back() != sum) {
            result.push_back(sum);
        } else {
            // If duplicate sum, we still need to explore its neighbors; but the sum itself is already counted
            // We still continue to next iterations.
        }

        // Generate neighbors: increment i, keeping j
        if (i + 1 < A.size()) {
            int ni = i + 1, nj = j;
            if (visited.find({ni, nj}) == visited.end()) {
                visited.insert({ni, nj});
                pq.push({A[ni] + B[nj], ni, nj});
            }
        }
        // increment j, keeping i
        if (j + 1 < B.size()) {
            int ni = i, nj = j + 1;
            if (visited.find({ni, nj}) == visited.end()) {
                visited.insert({ni, nj});
                pq.push({A[ni] + B[nj], ni, nj});
            }
        }
    }
    return result;
}
// The problem asks for the `C` largest distinct sums from the Cartesian product of two arrays. A naive approach would generate all `N*N` sums, deduplicate, sort descending, and take the first `C`. That is simple but inefficient for large `N` (e.g., `N = 10^5` would require `10^10` operations). A better approach sorts both arrays in descending order. The maximum sum is always `A[0] + B[0]`. The next possible sums come from either decreasing the index in `A` or in `B`. This resembles merging sorted lists, and we can use a max-heap (priority queue) to always extract the current largest sum, then push its two neighbors (by incrementing index in `A` or `B`). To avoid duplicates, we use a set of visited "pairs" (indices in A and B). Each time we pop a sum, we check if it equals the last pushed sum (since we want distinct sums); if it's new, we add it to result. We stop when we have `C` distinct sums or the heap is empty. Edge cases: if `C` is larger than the number of distinct sums, we return all distinct sums. If either vector is empty or `C <= 0`, return empty. Since we always pop the largest sum, and each sum is generated from a unique pair (with visited set), we guarantee correctness. Complexity: sorting takes `O(N log N)`. The heap operations: we push at most `2N` pairs (each pop may push up to 2 new pairs), and each push/pop is `O(log N)`. So total time is `O(N log N)` (dominated by sorting) plus the heap operations which are also `O(N log N)`. Space is `O(N)` for the visited set and heap.
