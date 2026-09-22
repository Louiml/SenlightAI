Write a C++ function named `compute_porofluid_coupling_penalty` that takes a vector of nearby element pair IDs (each pair represented as a `std::pair<int,int>`) and a reference to a `std::unordered_map<int, double>` mapping element ID to its porosity value. The function must compute and return a `std::map<int, double>` where each unique element ID appearing in any pair is mapped to a penalty factor equal to the sum of the inverse porosities of all its neighboring elements (i.e., for each pair (a,b), add 1/porosity[b] to the penalty of a, and 1/porosity[a] to the penalty of b). If a neighbor's porosity is zero or missing from the map, treat that contribution as 1.0 (not an error). The function must handle duplicate pairs, self-pairs (where a==b), and ensure the result is sorted ascending by element ID. Use `const` correctness appropriately.

// The solution iterates over all pairs once, and for each pair, we update two entries in a map: one for the first element (adding the inverse porosity of the second element) and one for the second element (adding the inverse porosity of the first element). To handle missing or zero porosity, we use a helper lambda that looks up the neighbor's porosity in the unordered_map; if the value is missing or equal to zero, we substitute 1.0. Because we use a `std::map`, insertion and updates are logarithmic in the number of unique element IDs, and the final iteration is naturally sorted. For duplicate pairs and self-pairs, the logic works automatically: self-pairs add the inverse of own porosity to itself (twice if duplicated, as each occurrence is processed). The total time complexity is \(O(P \log N)\) where \(P\) is the number of pairs and \(N\) is the number of unique elements, and space complexity is \(O(N)\).

#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * Compute per-element coupling penalty factors based on neighboring element pairs.
 *
 * For each pair (a,b), contribution to element a is 1/porosity[b] and to element b
 * is 1/porosity[a]. Missing or zero porosity of a neighbor yields 1.0.
 *
 * @param pairs vector of element ID pairs (each pair unordered, may duplicate, may be self)
 * @param porosity map from element ID to its porosity value (positive expected)
 * @return sorted map from element ID to total penalty factor for that element
 */
std::map<int, double> compute_porofluid_coupling_penalty(
    const std::vector<std::pair<int, int>>& pairs,
    const std::unordered_map<int, double>& porosity) {
    // Helper to get neighbor weight: 1/porosity if valid, else 1.0
    auto neighbor_weight = [&porosity](int neighbor_id) -> double {
        auto it = porosity.find(neighbor_id);
        if (it == porosity.end() || it->second == 0.0) {
            return 1.0;
        }
        return 1.0 / it->second;
    };

    std::map<int, double> result;
    for (const auto& pr : pairs) {
        const int first = pr.first;
        const int second = pr.second;

        // Contribution to first from second
        double w1 = neighbor_weight(second);
        result[first] += w1;

        // Contribution to second from first
        double w2 = neighbor_weight(first);
        result[second] += w2;
    }
    return result;
}

#include <cassert>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

// The function under test (declared or included from the solution header)
std::map<int, double> compute_porofluid_coupling_penalty(
    const std::vector<std::pair<int, int>>& pairs,
    const std::unordered_map<int, double>& porosity);

int main() {
    // Basic case with valid porosities
    {
        std::vector<std::pair<int,int>> pairs = {{1,2}, {2,3}, {1,3}};
        std::unordered_map<int,double> porosity = {{1,2.0}, {2,4.0}, {3,2.0}};
        auto result = compute_porofluid_coupling_penalty(pairs, porosity);
        // Expected penalties:
        // 1 gets from 2 (1/4) and from 3 (1/2) = 0.75
        // 2 gets from 1 (1/2) and from 3 (1/2) = 1.0
        // 3 gets from 2 (1/4) and from 1 (1/2) = 0.75
        assert(result.size() == 3);
        assert(result[1] > 0.749 && result[1] < 0.751);
        assert(result[2] > 0.999 && result[2] < 1.001);
        assert(result[3] > 0.749 && result[3] < 0.751);
    }

    // Missing porosity in map -> use 1.0 as inverse
    {
        std::vector<std::pair<int,int>> pairs = {{10,20}};
        std::unordered_map<int,double> porosity = {{10,5.0}}; // 20 missing
        auto result = compute_porofluid_coupling_penalty(pairs, porosity);
        assert(result.size() == 2);
        // 10 gets 1/1 (from missing 20) = 1.0
        // 20 gets 1/5.0 = 0.2
        assert(result[10] == 1.0);
        assert(result[20] > 0.199 && result[20] < 0.201);
    }

    // Zero porosity -> treat as 1.0 inverse
    {
        std::vector<std::pair<int,int>> pairs = {{1,2}};
        std::unordered_map<int,double> porosity = {{1,0.0}, {2,3.0}};
        auto result = compute_porofluid_coupling_penalty(pairs, porosity);
        // 1 gets inverse of 2 = 1/3 ≈ 0.333...
        // 2 gets inverse of 1 (zero -> 1.0) = 1.0
        assert(result[1] > 0.333 && result[1] < 0.334);
        assert(result[2] == 1.0);
    }

    // Dupicate pairs and self-pair
    {
        std::vector<std::pair<int,int>> pairs = {{1,1}, {1,1}, {1,2}};
        std::unordered_map<int,double> porosity = {{1,2.0}, {2,4.0}};
        auto result = compute_porofluid_coupling_penalty(pairs, porosity);
        // 1: from self twice: 1/2 + 1/2 = 1.0, plus from 2: 1/4 = 0.25 -> total 1.25
        // 2: from 1 once: 1/2 = 0.5
        assert(result[1] > 1.249 && result[1] < 1.251);
        assert(result[2] > 0.499 && result[2] < 0.501);
    }

    // Empty pairs -> empty map
    {
        std::vector<std::pair<int,int>> pairs;
        std::unordered_map<int,double> porosity; // map empty
        auto result = compute_porofluid_coupling_penalty(pairs, porosity);
        assert(result.empty());
    }

    // Sorted order check (even though map naturally sorts)
    {
        std::vector<std::pair<int,int>> pairs = {{5,3}, {1,5}};
        std::unordered_map<int,double> porosity = {{1,1.0}, {3,2.0}, {5,4.0}};
        auto result = compute_porofluid_coupling_penalty(pairs, porosity);
        auto it = result.begin();
        assert(it->first == 1); ++it;
        assert(it->first == 3); ++it;
        assert(it->first == 5);
    }

    return 0;
}
