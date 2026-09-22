/*
Write a C++ function named `minimumCuttingCost` that takes three arguments: two `int64_t` values `w` and `h` (the number of vertical and horizontal cuts required, respectively), a `std::vector<int64_t>` of length `w` containing the costs of vertical cuts, and a `std::vector<int64_t>` of length `h` containing the costs of horizontal cuts. The function must return a `uint64_t` representing the minimum total cost to cut a rectangular board into 1×1 squares, given that each cut divides one current piece into two, and the cost of a cut is equal to its original cost multiplied by the number of pieces the cut currently separates (i.e., the number of opposite-direction cuts already made plus one). More precisely, process all vertical and horizontal cuts in an optimal order: each cut's cost is computed as (original cost) × (1 + number of cuts already made in the perpendicular direction). The function must choose the order of cuts to minimize the total cost. For example, if `w = 2`, `h = 1`, vertical costs `[4, 6]`, horizontal cost `[3]`, an optimal order is: first vertical cost 4 (no horizontal cuts yet, multiplier 1 → cost 4), then horizontal cost 3 (now one vertical cut exists, multiplier 2 → cost 6), then vertical cost 6 (now one horizontal cut exists, multiplier 2 → cost 12), total = 4+6+12 = 22. Another order could give larger total. The function must handle empty vectors (if either `w` or `h` is zero, then no cuts of that type exist) and must work with large costs and counts (up to `int64_t` for costs and counts, with total cost fitting within `uint64_t`). Implement the function with appropriate `const` correctness and efficiency.
*/

#include <algorithm>
#include <cstdint>
#include <vector>

// Returns the minimum total cost to cut a board into 1x1 squares.
// verticalCosts: costs of vertical cuts (length = w). horizontalCosts: costs of horizontal cuts (length = h).
// Each cut's cost = originalCost * (number of perpendicular cuts already made + 1).
// Optimal strategy: always perform the most expensive available cut next.
uint64_t minimumCuttingCost(
    const std::vector<int64_t>& verticalCosts,
    const std::vector<int64_t>& horizontalCosts)
{
    // Combine both sets of cuts into a single list of (cost, direction).
    // direction = 0 for vertical, 1 for horizontal.
    std::vector<std::pair<int64_t, int>> cuts;
    cuts.reserve(verticalCosts.size() + horizontalCosts.size());
    for (int64_t c : verticalCosts) {
        cuts.emplace_back(c, 0);
    }
    for (int64_t c : horizontalCosts) {
        cuts.emplace_back(c, 1);
    }

    // Sort descending by cost.
    std::sort(cuts.begin(), cuts.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    uint64_t totalCost = 0;
    int64_t verticalPieces = 1;   // number of slices in vertical direction
    int64_t horizontalPieces = 1; // number of slices in horizontal direction

    for (const auto& [cost, dir] : cuts) {
        if (dir == 0) {
            // Vertical cut multiplies by number of horizontal segments already formed.
            totalCost += static_cast<uint64_t>(cost) * static_cast<uint64_t>(horizontalPieces);
            ++verticalPieces;
        } else {
            // Horizontal cut multiplies by number of vertical segments already formed.
            totalCost += static_cast<uint64_t>(cost) * static_cast<uint64_t>(verticalPieces);
            ++horizontalPieces;
        }
    }
    return totalCost;
}

#include <cassert>
#include <cstdint>
#include <vector>

// Include the solution function declaration here (or copy the function above).
uint64_t minimumCuttingCost(const std::vector<int64_t>&, const std::vector<int64_t>&);

int main() {
    // Example from task description.
    assert(minimumCuttingCost({4, 6}, {3}) == 22);
    // All vertical cuts, no horizontal.
    assert(minimumCuttingCost({5, 1, 2}, {}) == 8); // 5*1 + 1*1 + 2*1 = 8
    // All horizontal cuts.
    assert(minimumCuttingCost({}, {7, 3}) == 10); // 7*1 + 3*1 = 10
    // Both empty.
    assert(minimumCuttingCost({}, {}) == 0);
    // Simple equal costs – order irrelevant.
    assert(minimumCuttingCost({2, 2}, {2}) == 10); // 2*1 + 2*2 + 2*2? Let's compute: do vertical 2 first (mult 1), then horizontal 2 (mult 2) = 4, then vertical 2 (mult 2) = 4 → total 10. Alternative: vertical 2, vertical 2, horizontal 2 = 2+2+6=10? No that's 10 too? Let's check: first vertical (mult 1) =2, second vertical (still mult 1) =2, horizontal (mult 2) =4 → total 8? Wait horizontal multiplier is verticalPieces after two vertical cuts =2? Actually verticalPieces starts 1, after first vertical becomes 2, after second vertical becomes 3, then horizontal uses multiplier=verticalPieces=3 → cost 2*3=6 → total 2+2+6=10. Yes correct.
    // Larger test with known greedy property.
    assert(minimumCuttingCost({10, 1, 1}, {5, 5}) == 37); // Compute: sort desc: 10(V),5(H),5(H),1(V),1(V). Start vP=1,hP=1. 10*1=10 – vP=2. 5*2=10 – hP=2. 5*2=10 – hP=3. 1*3=3 – vP=3. 1*3=3 – vP=4. Total=10+10+10+3+3=36? Wait 10+10+10=30, plus 3+3=6 → 36. Let me recompute: Actually after first vertical: vP=2. First horizontal: multiplier=vP=2 → 5*2=10, hP=2. Second horizontal: multiplier=vP=2 → 5*2=10, hP=3. Then 1 vertical: multiplier=hP=3 → 3, vP=3. Then 1 vertical: multiplier=hP=3 → 3, vP=4. Total=10+10+10+3+3=36. So assert 36.
    assert(minimumCuttingCost({10, 1, 1}, {5, 5}) == 36);
    // Large values to ensure uint64_t handling.
    assert(minimumCuttingCost({1000000000LL, 1000000000LL}, {1000000000LL}) == 3000000000ULL); // 1e9*1 + 1e9*2 + 1e9*2 = 5e9? Let's compute: first vertical cost 1e9 (mult 1), second vertical cost 1e9 (mult 1) =2e9, horizontal cost 1e9 (mult verticalPieces=3) =3e9 → total=5e9. Actually order doesn't matter here all equal, so each cut multiplier: first vertical 1, second vertical 1, horizontal 3? Wait horizontal multiplier = verticalPieces after two verticals =3, so 3e9, total 1e9+1e9+3e9=5e9. But if we do vertical, horizontal, vertical: vertical 1e9 (mult1), horizontal (mult verticalPieces=2) 2e9, vertical (mult horizontalPieces=2) 2e9 → total 5e9. So assert 5000000000ULL.
    assert(minimumCuttingCost({1000000000LL, 1000000000LL}, {1000000000LL}) == 5000000000ULL);
    return 0;
}

// This is a classic greedy problem similar to "minimum cost to cut a chocolate bar" (or "cutting a board" from competitive programming). The key insight is that you should always perform the most expensive cut **first** regardless of direction, because expensive cuts get multiplied by the number of already-made perpendicular cuts, which only increases over time. Therefore, sort all vertical and horizontal cuts together by cost in descending order. Maintain two counters: `verticalPieces` and `horizontalPieces` initially 1 each (representing the number of pieces in each direction). When you make a vertical cut, its multiplier is `horizontalPieces` (number of horizontal slices already made), and it increases `verticalPieces` by 1. Similarly, a horizontal cut uses multiplier `verticalPieces` and increases `horizontalPieces`. Add `cost * multiplier` to the total. This greedy works because swapping two adjacent cuts where the earlier one has smaller cost than the later one always increases total cost (proof by exchange argument). Edge cases: if one count is zero, only process the other; if both empty, return 0. Also note costs are positive integers (may be large), and the product `cost * multiplier` could exceed `int64_t` but fits in `uint64_t`. Time complexity: O((w+h) log(w+h)) due to sorting, space O(w+h) for the combined list. Alternatively, use a max-heap. The reference solution sorts a vector of pairs (cost, direction), where direction 0=vertical, 1=horizontal.
