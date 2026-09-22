// Given a vector of distinct integers representing customer IDs and a vector of tour variables where each tour variable is a vector of customer IDs (each customer appears in exactly one tour), write a C++ function `computeTourCoverage` that takes a vector of tours and returns a vector of vectors of customer IDs representing the minimal set of tours needed to cover all customers exactly once. The function should identify tours that are redundant (fully contained within the union of other selected tours) and return only the essential tours that form a partition of all customers. If input is invalid (empty tour list, duplicate customers across tours, or missing customers), the function should return an empty vector. The solution must use a greedy approach: iteratively select tours that cover the most uncovered customers, then validate that the selected tours exactly cover all customers without overlap.

#include <cassert>
#include <vector>

int main() {
    // Basic case: two tours covering four customers.
    std::vector<std::vector<int>> tours1 = {{1,2}, {3,4}};
    auto result1 = computeTourCoverage(tours1);
    assert(result1.size() == 2);

    // Redundant tour: first tour covers all, others redundant.
    std::vector<std::vector<int>> tours2 = {{1,2,3}, {1}, {2,3}};
    auto result2 = computeTourCoverage(tours2);
    assert(result2.size() == 1);
    assert(result2[0] == std::vector<int>({1,2,3}));

    // Single tour.
    std::vector<std::vector<int>> tours3 = {{5,6,7}};
    auto result3 = computeTourCoverage(tours3);
    assert(result3.size() == 1);
    assert(result3[0] == std::vector<int>({5,6,7}));

    // Invalid: duplicate customer.
    std::vector<std::vector<int>> tours4 = {{1,2}, {2,3}};
    auto result4 = computeTourCoverage(tours4);
    assert(result4.empty());

    // Invalid: empty tour list.
    std::vector<std::vector<int>> tours5 = {};
    auto result5 = computeTourCoverage(tours5);
    assert(result5.empty());

    // Multiple tours with no redundancy.
    std::vector<std::vector<int>> tours6 = {{1}, {2}, {3}};
    auto result6 = computeTourCoverage(tours6);
    assert(result6.size() == 3);

    // Greedy picks largest first, then remaining.
    std::vector<std::vector<int>> tours7 = {{1,2,3}, {4,5}, {6}};
    auto result7 = computeTourCoverage(tours7);
    assert(result7.size() == 3);
    assert(result7[0] == std::vector<int>({1,2,3}));
    assert(result7[1] == std::vector<int>({4,5}));
    assert(result7[2] == std::vector<int>({6}));

    return 0;
}

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

// Compute a minimal set of tours (by greedy selection) that covers all customers exactly once.
// Returns empty vector if input is invalid (empty tour list, duplicate customers, or missing customers).
std::vector<std::vector<int>> computeTourCoverage(const std::vector<std::vector<int>>& tours) {
    if (tours.empty()) return {};

    // Validate: no duplicate customers across tours.
    std::unordered_set<int> all_customers;
    for (const auto& tour : tours) {
        for (int c : tour) {
            if (!all_customers.insert(c).second) {
                return {}; // duplicate customer found
            }
        }
    }
    if (all_customers.empty()) return {};

    // Greedy selection: pick tour with most uncovered customers.
    std::vector<std::vector<int>> selected;
    std::unordered_set<int> covered;
    std::vector<bool> tour_used(tours.size(), false);

    while (covered.size() < all_customers.size()) {
        int best_idx = -1;
        int best_count = -1;

        for (size_t i = 0; i < tours.size(); ++i) {
            if (tour_used[i]) continue;
            int count = 0;
            for (int c : tours[i]) {
                if (covered.find(c) == covered.end()) ++count;
            }
            if (count > best_count) {
                best_count = count;
                best_idx = static_cast<int>(i);
            }
        }

        if (best_idx == -1 || best_count == 0) {
            return {}; // cannot cover all customers
        }

        tour_used[best_idx] = true;
        selected.push_back(tours[best_idx]);
        for (int c : tours[best_idx]) covered.insert(c);
    }

    // Validate exact partition: all customers covered, no duplicates (already ensured).
    if (covered.size() != all_customers.size()) return {};

    return selected;
}

// The solution processes tours using a greedy set-cover style heuristic adapted for exact partitioning. First, validate the input: ensure no customer appears more than once across all tours, and ensure every customer ID in the union is present (assuming all integers are customers). If validation fails, return empty. Then repeatedly select the tour that covers the maximum number of uncovered customers; if a tie, prefer the tour with the smallest index. After selecting a tour, mark its customers as covered, and remove them from consideration. Continue until all customers are covered. After the greedy selection, verify that the selected tours form an exact partition: no customer is duplicated and all customers are covered. If the greedy result does not exactly partition (which can happen if input tours overlap despite validation, but validation already prevents overlap, so this is guaranteed), the function would return empty; however, because validation ensures no overlaps, the greedy selection will always cover all customers exactly once (since the union of all tours equals the customer set and tours are disjoint). Edge cases: empty tour list or empty customer set—return empty; single tour covering all customers—return that tour. Time complexity: let \(n\) be number of tours and \(m\) total customers. Validation is \(O(m)\). The greedy selection loops at most \(n\) times, each scanning \(n\) tours and counting uncovered customers by iterating over each tour's customers. Worst-case \(O(n^2 \cdot \text{avg_tour_length})\), but with total customers \(m\), the total work per selection is \(O(m)\), and at most \(n\) selections, so \(O(n \cdot m)\). Space complexity \(O(n + m)\).
