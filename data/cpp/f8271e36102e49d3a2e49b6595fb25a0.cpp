/*
Write a C++ function that takes two std::vector<double> inputs representing feature-descriptor vectors (each inner vector has the same fixed length, e.g., 64 or 128) and returns a std::vector<std::pair<int,int>> of matched index pairs, where each descriptor from the first list is matched to its nearest neighbor in the second list using Euclidean distance. Use a brute-force nearest neighbor search (no KD-tree), and return one match per first-list descriptor, selecting the second-list descriptor with the smallest squared Euclidean distance. Handle empty or unequal-length inputs gracefully: return an empty match vector if either input is empty, and if a first descriptor has no valid candidate (e.g., second list empty), skip it. The function must be `const`-correct, take inputs by const reference, and not modify the inputs.
*/

#include <vector>
#include <cstddef>
#include <limits>
#include <cmath>

// Match each descriptor in first array to its nearest neighbor in second array by Euclidean distance.
// Returns pairs (index_in_first, index_in_second). Skips a first descriptor if second array is empty
// or if descriptor lengths are incompatible. Uses squared Euclidean distance for efficiency.
std::vector<std::pair<int,int>> matchNearestNeighbors(
    const std::vector<std::vector<double>>& descriptors1,
    const std::vector<std::vector<double>>& descriptors2)
{
    std::vector<std::pair<int,int>> matches;
    if (descriptors1.empty() || descriptors2.empty()) {
        return matches;
    }

    // Precompute the length of the first descriptor of the second list to ensure compatibility.
    const size_t descriptorLength = descriptors2[0].size();

    for (size_t i = 0; i < descriptors1.size(); ++i) {
        const std::vector<double>& query = descriptors1[i];
        if (query.size() != descriptorLength) {
            continue; // skip incompatible descriptor
        }

        double bestDistSq = std::numeric_limits<double>::max();
        int bestIndex = -1;

        for (size_t j = 0; j < descriptors2.size(); ++j) {
            const std::vector<double>& candidate = descriptors2[j];
            if (candidate.size() != descriptorLength) {
                continue; // skip incompatible candidate
            }

            double distSq = 0.0;
            for (size_t k = 0; k < descriptorLength; ++k) {
                double diff = query[k] - candidate[k];
                distSq += diff * diff;
                // Early exit if distance already exceeds current best
                if (distSq >= bestDistSq) {
                    break;
                }
            }

            if (distSq < bestDistSq) {
                bestDistSq = distSq;
                bestIndex = static_cast<int>(j);
            }
        }

        if (bestIndex != -1) {
            matches.emplace_back(static_cast<int>(i), bestIndex);
        }
    }

    return matches;
}

#include <cassert>
#include <vector>

// The solution function is declared above; include or paste it here.

int main() {
    // Basic matching with distinct distances.
    std::vector<std::vector<double>> desc1 = {{1.0, 0.0}, {0.0, 1.0}};
    std::vector<std::vector<double>> desc2 = {{0.0, 0.0}, {1.0, 1.0}};
    auto matches = matchNearestNeighbors(desc1, desc2);
    assert(matches.size() == 2);
    assert(matches[0] == std::make_pair(0, 0)); // (1,0) closer to (0,0) than (1,1)
    assert(matches[1] == std::make_pair(1, 0)); // (0,1) also closer to (0,0) because distance sqrt(1) vs sqrt(2)

    // Empty first list.
    assert(matchNearestNeighbors({}, desc2).empty());

    // Empty second list.
    assert(matchNearestNeighbors(desc1, {}).empty());

    // Incompatible lengths: first descriptor length differs from second list's length.
    std::vector<std::vector<double>> badDesc1 = {{1.0, 0.0, 1.0}};
    std::vector<std::vector<double>> desc2len2 = {{0.0, 0.0}};
    assert(matchNearestNeighbors(badDesc1, desc2len2).empty());

    // Single descriptor matching.
    std::vector<std::vector<double>> single1 = {{3.0, 4.0}};
    std::vector<std::vector<double>> single2 = {{0.0, 0.0}, {3.0, 4.0}};
    auto singleMatches = matchNearestNeighbors(single1, single2);
    assert(singleMatches.size() == 1);
    assert(singleMatches[0] == std::make_pair(0, 1)); // exact match

    // Ties: identical distances, function picks first encountered candidate.
    std::vector<std::vector<double>> tieDesc1 = {{0.0, 1.0}};
    std::vector<std::vector<double>> tieDesc2 = {{1.0, 0.0}, {0.0, 1.0}};
    auto tieMatches = matchNearestNeighbors(tieDesc1, tieDesc2);
    assert(tieMatches.size() == 1);
    // Both distances are 2, first candidate (index 0) should be chosen.
    assert(tieMatches[0] == std::make_pair(0, 0));

    // Larger test: 3 vs 2 descriptors, ensure all first descriptors get matches.
    std::vector<std::vector<double>> descA = {{0.0, 0.0}, {2.0, 2.0}, {5.0, 5.0}};
    std::vector<std::vector<double>> descB = {{1.0, 1.0}, {4.0, 4.0}};
    auto multiple = matchNearestNeighbors(descA, descB);
    assert(multiple.size() == 3);
    assert(multiple[0].first == 0 && multiple[0].second == 0); // (0,0) nearest (1,1)
    assert(multiple[1].first == 1 && multiple[1].second == 0); // (2,2) nearest (1,1) vs (4,4)
    assert(multiple[2].first == 2 && multiple[2].second == 1); // (5,5) nearest (4,4)

    return 0;
}

// The core algorithm is brute-force nearest neighbor matching: For each descriptor vector `u` in the first list, iterate over every descriptor `v` in the second list, compute the squared Euclidean distance `d² = Σ (u[i]-v[i])²`, and keep track of the minimal distance and its index. Because the descriptors have fixed length, the inner loop runs in O(L) per comparison, where L is descriptor length. The overall time complexity is O(N*M*L), where N is the number of first descriptors, M is the number of second descriptors, and L is the vector length. Space complexity is O(N) for the result vector, plus O(1) temporary variables. Edge cases: if `descriptors1` is empty or `descriptors2` is empty, return an empty result. If a first descriptor's length differs from the second list's descriptors, treat each mismatched comparison as invalid (skip) or assume the input is well-formed; the safe approach is to check that both descriptors have the same length before computing the distance. Also, prefer squaring distances (avoiding sqrt) for efficiency; ties can be broken arbitrarily (e.g., keep the first encountered minimal index). The function should be a free function named `matchNearestNeighbors` that returns `std::vector<std::pair<int,int>>`.
