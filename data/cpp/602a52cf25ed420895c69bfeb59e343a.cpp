// Implement a C++ function `buildCandidateSets` that, given a set of frequent itemsets of size k (represented as `std::set<std::set<std::string>>`), generates the candidate itemsets of size k+1 using the Apriori-gen join step. The function should return a new `std::set<std::set<std::string>>` containing all possible pairs of frequent itemsets that share all but one item, and whose union has size exactly k+1. For example, if frequent itemsets are {A,B} and {A,C}, the candidate {A,B,C} is generated because they share {A} and their union has size 3. However, if two itemsets are identical or share fewer than k-1 items, no candidate is produced from that pair. The input set is non-empty and may contain itemsets of any size, but the function should only consider pairs where both itemsets have size exactly k (you may assume the caller passes itemsets all of size k). The function must be pure (no I/O, no global state) and use only standard library facilities.
#include <cassert>
#include <set>
#include <string>

// The function to test (already defined above, but included here for clarity)
std::set<std::set<std::string>> buildCandidateSets(const std::set<std::set<std::string>>& frequentK);

int main() {
    // Test 1: Basic join of two 2-itemsets sharing one item
    std::set<std::set<std::string>> freq1 = {{"A", "B"}, {"A", "C"}};
    auto cand1 = buildCandidateSets(freq1);
    std::set<std::set<std::string>> expected1 = {{"A", "B", "C"}};
    assert(cand1 == expected1);

    // Test 2: Pair with no common items (difference size > 1) -> no candidate
    std::set<std::set<std::string>> freq2 = {{"A", "B"}, {"C", "D"}};
    auto cand2 = buildCandidateSets(freq2);
    assert(cand2.empty());

    // Test 3: Three itemsets, only two produce a valid candidate
    std::set<std::set<std::string>> freq3 = {{"X", "Y"}, {"X", "Z"}, {"Y", "Z"}};
    auto cand3 = buildCandidateSets(freq3);
    std::set<std::set<std::string>> expected3 = {{"X", "Y", "Z"}};
    assert(cand3 == expected3);

    // Test 4: Identical itemsets should not produce a candidate (difference size 0)
    std::set<std::set<std::string>> freq4 = {{"A", "B"}, {"A", "B"}};
    auto cand4 = buildCandidateSets(freq4);
    assert(cand4.empty());

    // Test 5: Larger sets – join three 3-itemsets
    std::set<std::set<std::string>> freq5 = {{"1", "2", "3"}, {"1", "2", "4"}, {"1", "3", "4"}};
    auto cand5 = buildCandidateSets(freq5);
    // From pair {1,2,3} and {1,2,4} -> {1,2,3,4}
    // From pair {1,2,3} and {1,3,4} -> {1,2,3,4}
    // From pair {1,2,4} and {1,3,4} -> {1,2,3,4}
    std::set<std::set<std::string>> expected5 = {{"1", "2", "3", "4"}};
    assert(cand5 == expected5);

    // Test 6: Single itemset input (no pairs) -> no candidates
    std::set<std::set<std::string>> freq6 = {{"A"}};
    auto cand6 = buildCandidateSets(freq6);
    assert(cand6.empty());

    // Test 7: Itemsets with strings of different lengths
    std::set<std::set<std::string>> freq7 = {{"apple", "banana"}, {"apple", "cherry"}};
    auto cand7 = buildCandidateSets(freq7);
    std::set<std::set<std::string>> expected7 = {{"apple", "banana", "cherry"}};
    assert(cand7 == expected7);

    // Test 8: Empty input set (edge case) -> empty output
    std::set<std::set<std::string>> freq8;
    auto cand8 = buildCandidateSets(freq8);
    assert(cand8.empty());

    return 0;
}
#include <set>
#include <string>
#include <algorithm>

// Given a set of frequent itemsets (all of size k), generate candidate itemsets of size k+1
// by joining pairs that share all but one item. Return the set of valid candidates.
std::set<std::set<std::string>> buildCandidateSets(const std::set<std::set<std::string>>& frequentK) {
    std::set<std::set<std::string>> candidates;
    
    // For each itemset Li, consider all later itemsets Lj
    for (auto it = frequentK.begin(); it != frequentK.end(); ++it) {
        const auto& Li = *it;
        // Advance iterator to the next itemset
        auto jt = it;
        ++jt;
        for (; jt != frequentK.end(); ++jt) {
            const auto& Lj = *jt;
            
            // Compute Li minus Lj: if exactly one element remains, they differ by one item
            std::set<std::string> diff = Li;
            for (const auto& elem : Lj) {
                diff.erase(elem);
            }
            
            // If the difference has size 1, the union has size k+1 and is a valid candidate
            if (diff.size() == 1) {
                // Build the union (Li ∪ Lj)
                std::set<std::string> candidate = Li;
                candidate.insert(Lj.begin(), Lj.end());
                // Safety check: union size must be k+1 (should always be true here)
                if (candidate.size() == Li.size() + 1) {
                    candidates.insert(std::move(candidate));
                }
            }
        }
    }
    
    return candidates;
}
// The solution iterates over all ordered pairs of distinct itemsets in the input set. For each pair (Li, Lj), we need to check whether they differ in exactly one element and have the same remaining elements. This is equivalent to checking if the union of the two sets has size k+1. A direct implementation: for each itemset Li, iterate over all subsequent itemsets Lj (to avoid duplicate candidate generation). Compute the union using `std::set_union` or simply insert all elements of Lj into a copy of Li, then check if the resulting set size equals k+1. If yes, insert the union into the result set. However, this naive union computation may be costly for large sets; an alternative is to compute the set difference between Li and Lj; if the size of `Li - Lj` is exactly 1, then the union has size k+1. Since we are only dealing with sets of strings, computing difference can be done by copying Li and erasing all elements of Lj (using `std::set::erase` for each element of Lj). If the remaining size is 1, the union is valid. Edge cases: empty itemsets (size 0) would not be valid k, but we can guard against that; identical itemsets will have difference size 0, so they are rejected. The algorithm is O(n^2 * k * log k) where n is number of frequent itemsets and k is the size of each set, due to set operations. Memory usage is O(n * k) for the output set, plus temporary copies.
