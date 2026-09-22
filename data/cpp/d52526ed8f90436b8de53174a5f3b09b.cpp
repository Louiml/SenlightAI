Write a C++ function `generateSimpleBipartite` that creates a simple bipartite graph (no self-loops or multi-edges) with a given number of left nodes, right nodes, and edges. The function should use a seeded random number generator (provided by the caller via a `std::mt19937` reference) to randomly select pairs of left and right nodes until the required number of distinct edges has been added. The function should return a `std::pair<size_t, size_t>` representing the number of successfully added edges and the number of attempted selections (collisions/skips) that did not result in a new edge. The function must validate that the requested edge count does not exceed the maximum possible distinct edges (leftNodes * rightNodes), and if it does, throw an `std::invalid_argument` exception. The algorithm should efficiently track existing edges using a hash set to ensure no duplicates are added, and it should handle the edge case where the requested edges are zero or the number of nodes is zero gracefully (returning zero attempts and zero edges). The time complexity should be O(edges) on average, and the space complexity O(edges) to store the edge set.
#include <cassert>
#include <random>
#include <utility>

// Declare the function (implementation above is assumed included)
std::pair<size_t, size_t> generateSimpleBipartite(
    size_t LeftNodes, size_t RightNodes, size_t Edges, std::mt19937& rng);

int main() {
    // Fixed seed for reproducibility
    std::mt19937 rng(42);

    // Basic valid cases
    {
        auto [e, c] = generateSimpleBipartite(3, 4, 5, rng);
        assert(e == 5);
        assert(c >= 0); // collisions can be any non-negative number
    }

    // Zero edges
    {
        auto [e, c] = generateSimpleBipartite(10, 10, 0, rng);
        assert(e == 0 && c == 0);
    }

    // Max possible edges (full bipartite graph)
    {
        auto [e, c] = generateSimpleBipartite(4, 3, 12, rng);
        assert(e == 12);
    }

    // Edge case: one side zero, but Edges zero is fine
    {
        auto [e, c] = generateSimpleBipartite(0, 5, 0, rng);
        assert(e == 0 && c == 0);
    }

    // Invalid: too many edges throws
    {
        bool threw = false;
        try {
            generateSimpleBipartite(2, 2, 5, rng);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Invalid: zero nodes but positive edges throws
    {
        bool threw = false;
        try {
            generateSimpleBipartite(0, 3, 1, rng);
        } catch (const std::invalid_argument&) {
            threw = true;
        }
        assert(threw);
    }

    // Verify that all added edges are distinct by re-generating with a small
    // graph and checking the second return (collisions) is plausible.
    // With 100 edges out of 10*10=100 possible, collisions must be >=0.
    {
        auto [e, c] = generateSimpleBipartite(10, 10, 100, rng);
        assert(e == 100);
    }

    return 0;
}
#include <cstddef>
#include <stdexcept>
#include <unordered_set>
#include <random>
#include <utility>

// Generate a simple bipartite graph (no multi-edges or self-loops) with given
// numbers of left/right nodes and total edges. Returns a pair:
//   first  = number of edges successfully added (should equal Edges)
//   second = number of attempted selections that were duplicates (collisions)
// Throws std::invalid_argument if Edges > LeftNodes * RightNodes.
// The caller-provided random generator is used for reproducibility.
std::pair<size_t, size_t> generateSimpleBipartite(
    size_t LeftNodes,
    size_t RightNodes,
    size_t Edges,
    std::mt19937& rng) {

    if (LeftNodes == 0 || RightNodes == 0) {
        if (Edges == 0) return {0, 0};
        throw std::invalid_argument("Cannot add edges if either side is empty");
    }
    if (Edges > LeftNodes * RightNodes) {
        throw std::invalid_argument("Too many edges requested");
    }

    // We store edges as a 64-bit key: leftIdx * RightNodes + rightIdx
    std::unordered_set<size_t> edgeSet;
    edgeSet.reserve(Edges);

    std::uniform_int_distribution<size_t> leftDist(0, LeftNodes - 1);
    std::uniform_int_distribution<size_t> rightDist(0, RightNodes - 1);

    size_t added = 0;
    size_t collisions = 0;

    while (added < Edges) {
        size_t l = leftDist(rng);
        size_t r = rightDist(rng);
        size_t key = l * RightNodes + r; // unique for given (l,r)

        if (edgeSet.insert(key).second) {
            ++added;
        } else {
            ++collisions;
        }
    }

    return {added, collisions};
}
// The solution approach is straightforward: initialize an empty unordered set of pairs to store the distinct edges. Loop until we have added the required number of edges. In each iteration, generate a random left index in the range [0, LeftNodes) and a random right index in the range [0, RightNodes), but to avoid confusion with the right nodes’ local indices, we treat them as separate ranges. Create a pair (leftIdx, rightIdx) and check if it is already in the set. If not, insert it and increment the edge count; if it is already present, increment a collision count. Continue until the edge count reaches the target. If at any point the required edges exceed the maximum possible distinct edges (LeftNodes * RightNodes), throw an exception. Edge cases include: when either LeftNodes or RightNodes is zero, the maximum possible edges is zero, so if Edges > 0, throw an exception; if Edges == 0, return (0,0) immediately. Also, when the random generation might loop indefinitely if Edges is close to the maximum and the random generator repeatedly picks the same pair, but since the collision rate is manageable for typical inputs, we accept this. The time complexity is O(Edges) average because each successful insertion is a constant-time hash set operation, plus the collisions are proportional to the probability of duplicate picks; the space complexity is O(Edges) for the set. We use a `std::unordered_set` with a custom hash for pairs, or we can encode the pair into a single 64-bit integer key as `leftIdx * RightNodes + rightIdx` to use a `std::unordered_set<size_t>` for efficiency.
