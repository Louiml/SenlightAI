// Given three groups of items—agencies (each with a cost and a list of availability flags for restaurants), restaurants (each with a cost and a list of availability flags for hotels), and hotels (each with a cost and a list of availability flags for agencies)—write a C++ function that finds a valid triple (agency `i`, restaurant `j`, hotel `k`) such that: (1) agency `i` has a direct connection to restaurant `j` (the flag `da[i][j]` equals 0), (2) restaurant `j` has a direct connection to hotel `k` (the flag `dr[j][k]` equals 0), and (3) hotel `k` has a direct connection back to agency `i` (the flag `dh[k][i]` equals 0). Among all valid triples, the function must return the one with the minimum total cost (`pa[i] + pr[j] + ph[k]`). If multiple triples tie for the minimum cost, the function may return any one of them. If no valid triple exists, the function should return a sentinel indicating failure (e.g., an empty optional or a special struct with a flag). All counts (agencies, restaurants, hotels) are between 1 and 25. The input arrays are provided as vectors of vectors (or references to them) with sizes matching the counts, and all costs are non-negative integers. The function must not modify the input.
// The problem is essentially a constrained search over all combinations. The main algorithm brute‑forces every possible triple `(i, j, k)` with three nested loops over agencies, restaurants, and hotels. For each triple, we check the three connection conditions: `da[i][j] == 0`, `dr[j][k] == 0`, and `dh[k][i] == 0`. If all hold, we compute the total cost and track the minimum. We maintain a boolean flag to indicate whether any valid triple has been found, and store the indices and minimum cost. The worst‑case number of triples is `A * R * H`, each with O(1) checks, so time complexity is O(A·R·H), which is at most 25³ = 15,625 operations—well within limits. Space usage is O(1) auxiliary (excluding input storage). Edge cases include: counts of 1 (loops still work), no valid triple (return failure), and ties (return the first minimum found). Since costs are non‑negative, an initial large sentinel (e.g., INT_MAX) can be used for comparison.
#include <vector>
#include <limits>
#include <optional>
#include <tuple>

struct Answer {
    bool found;
    int agencyIndex;
    int restaurantIndex;
    int hotelIndex;
    int totalCost;
};

// Finds the minimum-cost triple (agency, restaurant, hotel) that forms a cycle,
// where each edge exists (flag == 0). Returns Answer with found=false if none.
Answer findCheapestTrip(
    const std::vector<std::vector<int>>& da, // agency->restaurant flags
    const std::vector<std::vector<int>>& dr, // restaurant->hotel flags
    const std::vector<std::vector<int>>& dh, // hotel->agency flags
    const std::vector<int>& pa,              // agency costs
    const std::vector<int>& pr,              // restaurant costs
    const std::vector<int>& ph               // hotel costs
) {
    Answer best{false, -1, -1, -1, std::numeric_limits<int>::max()};
    const int A = static_cast<int>(pa.size());
    const int R = static_cast<int>(pr.size());
    const int H = static_cast<int>(ph.size());

    for (int i = 0; i < A; ++i) {
        for (int j = 0; j < R; ++j) {
            if (j >= static_cast<int>(da[i].size())) continue; // defensive
            if (da[i][j] != 0) continue; // no direct edge
            for (int k = 0; k < H; ++k) {
                // Check both remaining edges in the cycle
                if (j < static_cast<int>(dr.size()) &&
                    k < static_cast<int>(dr[j].size()) && dr[j][k] == 0 &&
                    k < static_cast<int>(dh.size()) &&
                    i < static_cast<int>(dh[k].size()) && dh[k][i] == 0) {
                    int total = pa[i] + pr[j] + ph[k];
                    if (total < best.totalCost) {
                        best.found = true;
                        best.agencyIndex = i;
                        best.restaurantIndex = j;
                        best.hotelIndex = k;
                        best.totalCost = total;
                    }
                }
            }
        }
    }
    return best;
}
#include <cassert>
#include <vector>

int main() {
    // Test 1: Simple valid case
    {
        std::vector<std::vector<int>> da = {{0, 1}};
        std::vector<std::vector<int>> dr = {{0, 0}};
        std::vector<std::vector<int>> dh = {{0, 0}};
        std::vector<int> pa = {5};
        std::vector<int> pr = {3};
        std::vector<int> ph = {2};
        auto ans = findCheapestTrip(da, dr, dh, pa, pr, ph);
        assert(ans.found);
        assert(ans.agencyIndex == 0 && ans.restaurantIndex == 0 && ans.hotelIndex == 0);
        assert(ans.totalCost == 10);
    }

    // Test 2: No valid triple (all edges blocked)
    {
        std::vector<std::vector<int>> da = {{1}};
        std::vector<std::vector<int>> dr = {{1}};
        std::vector<std::vector<int>> dh = {{1}};
        std::vector<int> pa = {1};
        std::vector<int> pr = {1};
        std::vector<int> ph = {1};
        auto ans = findCheapestTrip(da, dr, dh, pa, pr, ph);
        assert(!ans.found);
    }

    // Test 3: Multiple triples, pick cheapest
    {
        std::vector<std::vector<int>> da = {{0, 1}, {0, 1}};
        std::vector<std::vector<int>> dr = {{0, 1}, {1, 0}};
        std::vector<std::vector<int>> dh = {{0, 1}, {1, 0}};
        std::vector<int> pa = {10, 20};
        std::vector<int> pr = {1, 5};
        std::vector<int> ph = {1, 2};
        auto ans = findCheapestTrip(da, dr, dh, pa, pr, ph);
        assert(ans.found);
        // Valid triples: (0,0,0) cost=12, (1,1,1) cost=27, (0,0,1) fails due dr, etc.
        assert(ans.agencyIndex == 0 && ans.restaurantIndex == 0 && ans.hotelIndex == 0);
        assert(ans.totalCost == 12);
    }

    // Test 4: Larger case with tie (first found is expected)
    {
        std::vector<std::vector<int>> da = {{0, 0}};
        std::vector<std::vector<int>> dr = {{0, 0}, {0, 0}};
        std::vector<std::vector<int>> dh = {{0, 0}, {0, 0}};
        std::vector<int> pa = {1};
        std::vector<int> pr = {1, 1};
        std::vector<int> ph = {1, 1};
        auto ans = findCheapestTrip(da, dr, dh, pa, pr, ph);
        assert(ans.found);
        // Both triples cost 3; first is (0,0,0)
        assert(ans.agencyIndex == 0 && ans.restaurantIndex == 0 && ans.hotelIndex == 0);
        assert(ans.totalCost == 3);
    }

    // Test 5: Single item each with valid connection
    {
        std::vector<std::vector<int>> da = {{0}};
        std::vector<std::vector<int>> dr = {{0}};
        std::vector<std::vector<int>> dh = {{0}};
        std::vector<int> pa = {100};
        std::vector<int> pr = {200};
        std::vector<int> ph = {300};
        auto ans = findCheapestTrip(da, dr, dh, pa, pr, ph);
        assert(ans.found);
        assert(ans.totalCost == 600);
    }

    return 0;
}
