/*
You are given a puzzle consisting of a string of length N containing the digits '1' through 'N' exactly once (initially in some arbitrary order). You are allowed to apply a set of M distinct swap operations; each operation is defined by a pair of 1-based indices (a, b), and applying it to the current string swaps the characters at those positions. The goal is to transform a given starting permutation into the sorted identity permutation "123...N" using the fewest number of moves. Write a C++ function `int shortest_swaps(int N, const std::string& source, const std::vector<std::pair<int,int>>& swaps)` that returns the minimum number of moves required, or -1 if it is impossible. The function must handle N up to 12, M up to 144, and multiple calls (the function is pure and stateless, not depending on global variables). Assume all inputs are valid (source contains exactly N distinct digits from '1' to 'N', swap indices are within [1,N]).
*/
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>

// Returns the minimum number of swap moves to reach sorted permutation,
// or -1 if impossible.
int shortest_swaps(int N, const std::string& source,
                   const std::vector<std::pair<int,int>>& swaps) {
    std::string target;
    for (int i = 1; i <= N; ++i) {
        target.push_back(static_cast<char>('0' + i));
    }

    if (source == target) {
        return 0;
    }

    std::queue<std::string> q;
    std::unordered_map<std::string, int> dist;
    q.push(source);
    dist[source] = 0;

    while (!q.empty()) {
        std::string cur = q.front();
        q.pop();
        int d = dist[cur];

        for (const auto& sw : swaps) {
            int a = sw.first - 1;  // Convert to 0-based
            int b = sw.second - 1;
            if (a == b) {
                continue; // self-swap doesn't change anything
            }
            std::string nxt = cur;
            std::swap(nxt[a], nxt[b]);

            if (dist.find(nxt) != dist.end()) {
                continue; // already visited
            }
            if (nxt == target) {
                return d + 1;
            }
            dist[nxt] = d + 1;
            q.push(nxt);
        }
    }

    return -1;
}
#include <cassert>
#include <string>
#include <vector>

// Function from solution block (copied here for self-containment)
int shortest_swaps(int N, const std::string& source,
                   const std::vector<std::pair<int,int>>& swaps);

int main() {
    // Basic case: already sorted
    assert(shortest_swaps(3, "123", {{1,2}}) == 0);

    // Swap adjacent once
    assert(shortest_swaps(3, "213", {{1,2}}) == 1);

    // Two moves: 321 -> swap(1,3) -> 123 (direct), also here
    assert(shortest_swaps(3, "321", {{1,3}}) == 1);

    // Need two moves: 312 -> swap(1,2) -> 132 -> swap(2,3) -> 123
    std::vector<std::pair<int,int>> sw1 = {{1,2}, {2,3}};
    assert(shortest_swaps(3, "312", sw1) == 2);

    // Impossible: only move is (1,2) on N=3, cannot reach all permutations
    assert(shortest_swaps(3, "123", {{1,2}}) == 0); // already sorted
    assert(shortest_swaps(3, "132", {{1,2}}) == -1); // swap(1,2) gives 312, never 123

    // N=4, a few swaps
    std::vector<std::pair<int,int>> sw2 = {{1,4}, {2,3}};
    assert(shortest_swaps(4, "4321", sw2) == 1); // swap(1,4) -> 1324? No, swap positions 1 and 4: becomes 1324, then swap(2,3): 1234? Let's compute: source 4321, swap(1,4)=1234 directly, so 1 move actually. But with those swaps, 4321 swap(1,4) gives 1324? Wait indices: 4,3,2,1. Swap idx1 (val4) and idx4 (val1) -> 1,3,2,4 = 1324, not sorted. Then swap(2,3) on 1324 -> 1234. So 2 moves.
    assert(shortest_swaps(4, "4321", sw2) == 2);

    // Empty swaps
    assert(shortest_swaps(2, "12", {}) == 0);
    assert(shortest_swaps(2, "21", {}) == -1);

    // N=1
    assert(shortest_swaps(1, "1", {{1,1}}) == 0);

    return 0;
}
// This is a shortest-path problem on an unweighted graph where each node is a permutation string of length N, and each edge corresponds to applying one of the M swap operations. Since N ≤ 12, the number of possible permutations is at most 12! ≈ 479 million, which is too large to precompute, but the number of distinct states reachable from a given source is usually much smaller. We perform a breadth-first search (BFS) starting from the source string, moving level by level until we reach the destination string `"123...N"`. We maintain a `std::unordered_map<std::string, int>` for distances (or a visited flag plus depth variables) to avoid revisiting states. For each popped state, we generate all M successors by applying each swap and enqueue if not seen. The first time we encounter the destination, its distance is the answer. If we exhaust the queue without finding it, return -1. Edge cases: (1) If source already equals destination, return 0 (the BFS naturally handles this because the initial node's distance is 0). (2) If M=0 and source != destination, impossible. (3) Swaps that affect the same position pairs or are self-swaps (a==b) may be present; self-swaps produce no change and can be ignored or skipped when generating moves to avoid infinite loops—but BFS with visited set handles that. Complexity: Each state generates M neighbors; BFS visits at most all reachable permutations. In the worst case, the number of reachable states could be up to 12! (about 479M), but in practice with N=12 and typical swap sets it's far smaller; however, in adversarial cases, time and memory can blow up. For N=12, the state space is large but BFS is still feasible for many inputs. Time complexity is O(R * M) where R is the number of visited states, and space is O(R) for the visited set and queue.
