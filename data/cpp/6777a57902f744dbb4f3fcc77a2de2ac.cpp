/*
Write a C++ function `computeEnergy(const std::vector<int>& labels, const std::vector<int>& unaryCosts, const std::vector<std::tuple<int,int,int>>& pairwiseEdges)` that computes a pairwise energy function for a labeling problem. The function takes:
- `labels`: a vector of size `N` where each entry is a label ID in the range `[0, L-1]`
- `unaryCosts`: a vector of size `N * L` representing per-site unary costs, where `unaryCosts[s * L + l]` is the cost of assigning label `l` to site `s`
- `pairwiseEdges`: a vector of tuples `(site1, site2, weight)` where each tuple defines an undirected edge with a symmetric penalty for any label difference between the two connected sites

The total energy is defined as the sum of unary costs for the current labeling plus the sum over all pairwise edges of `weight * (label[site1] != label[site2])` (i.e., a Potts model). The function should return the total energy as a 64-bit integer to avoid overflow, and it must validate inputs appropriately (throw `std::invalid_argument` on malformed inputs such as mismatched sizes, out-of-range labels, or invalid site indices). This task is inspired by the energy computation methods in the given GCoptimization library, specifically the `compute_energy()` and `giveSmoothEnergy()`/`giveDataEnergy()` functions.
*/
#include <vector>
#include <tuple>
#include <stdexcept>
#include <cstdint>

// Compute the total energy for a labeling given unary costs and pairwise Potts edges.
// unaryCosts is indexed as [site * numLabels + label].
// Each pairwise edge is (site1, site2, weight), symmetric, penalty weight if labels differ.
int64_t computeEnergy(
    const std::vector<int>& labels,
    const std::vector<int>& unaryCosts,
    const std::vector<std::tuple<int, int, int>>& pairwiseEdges)
{
    const std::size_t N = labels.size();
    if (N == 0) {
        throw std::invalid_argument("Labels vector must be non-empty.");
    }

    // Determine number of labels from unaryCosts size.
    if (unaryCosts.empty() || unaryCosts.size() % N != 0) {
        throw std::invalid_argument("Unary costs size must be a multiple of number of sites.");
    }
    const std::size_t L = unaryCosts.size() / N;
    if (L == 0) {
        throw std::invalid_argument("Number of labels must be positive.");
    }

    // Validate all labels are in range.
    for (int label : labels) {
        if (label < 0 || static_cast<std::size_t>(label) >= L) {
            throw std::out_of_range("Label value out of range.");
        }
    }

    int64_t totalEnergy = 0;

    // Add unary costs.
    for (std::size_t s = 0; s < N; ++s) {
        const int label = labels[s];
        totalEnergy += static_cast<int64_t>(unaryCosts[s * L + static_cast<std::size_t>(label)]);
    }

    // Add pairwise costs (Potts model).
    for (const auto& edge : pairwiseEdges) {
        const int site1 = std::get<0>(edge);
        const int site2 = std::get<1>(edge);
        const int weight = std::get<2>(edge);

        if (site1 < 0 || static_cast<std::size_t>(site1) >= N ||
            site2 < 0 || static_cast<std::size_t>(site2) >= N) {
            throw std::out_of_range("Pairwise edge site index out of range.");
        }

        if (labels[static_cast<std::size_t>(site1)] != labels[static_cast<std::size_t>(site2)]) {
            totalEnergy += static_cast<int64_t>(weight);
        }
    }

    return totalEnergy;
}
#include <cassert>
#include <vector>
#include <tuple>

int main() {
    // Basic test: 2 sites, 2 labels, no edges.
    std::vector<int> labels = {0, 1};
    std::vector<int> unary = {5, 10, 20, 30}; // site0: label0=5, label1=10; site1: label0=20, label1=30
    std::vector<std::tuple<int,int,int>> edges;
    assert(computeEnergy(labels, unary, edges) == 5 + 30);

    // With an edge where labels differ.
    edges.push_back({0, 1, 7});
    assert(computeEnergy(labels, unary, edges) == 5 + 30 + 7);

    // Same labels on edge: no penalty.
    labels = {1, 1};
    assert(computeEnergy(labels, unary, edges) == 10 + 30);

    // Self-loop edge should not add cost.
    edges.push_back({1, 1, 100});
    assert(computeEnergy(labels, unary, edges) == 10 + 30);

    // Multiple edges, different weights.
    edges.clear();
    edges.push_back({0, 1, 3});
    edges.push_back({0, 0, 5}); // self-loop, no penalty
    labels = {0, 1};
    assert(computeEnergy(labels, unary, edges) == 5 + 30 + 3);

    // Edge with large weights to test 64-bit overflow handling.
    labels = {0, 1};
    edges.clear();
    edges.push_back({0, 1, 1000000000});
    edges.push_back({0, 1, 1000000000});
    // unary costs are small, total = 35 + 2000000000
    assert(computeEnergy(labels, unary, edges) == static_cast<int64_t>(5) + 30 + 2000000000LL);

    // Validation: out-of-range label.
    bool caught = false;
    try {
        std::vector<int> badLabels = {0, 2};
        computeEnergy(badLabels, unary, edges);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);

    // Validation: mismatched unary cost length.
    caught = false;
    try {
        std::vector<int> badUnary = {1, 2, 3};
        computeEnergy(labels, badUnary, edges);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);

    return 0;
}
// The solution involves straightforward summation with careful validation. First, validate that the labels vector is non-empty and all labels are within `[0, L-1]` (derive `L` from the size of `unaryCosts` divided by the number of sites, ensuring the division is exact). For unary costs, iterate over each site `s`, compute the index `s * L + labels[s]`, and add that cost to a running 64-bit sum. For pairwise edges, iterate over each tuple, validate that both site indices are in range `[0, N-1]`, then if the labels differ, add `weight` to the sum (weights are assumed non-negative, but no negativity check is required). Edge cases include: empty labels (throw), zero or negative labels count (throw), a unary cost array that doesn't match `N * L` exactly (throw), duplicate edges (allow—simply add multiple times), self-loops (site1 == site2—the penalty is zero because the labels are identical, so it adds nothing, which is correct), and possible overflow—use `int64_t` or `long long` for the accumulator. Time complexity is `O(N + E)` where `E` is the number of edges, and space complexity is `O(1)` beyond input storage.
