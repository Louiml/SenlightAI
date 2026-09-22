// Write a C++ function named `deliverPackages` that takes three parameters: the maximum total weight capacity `maxWeight`, the number of delivery trucks `truckCount`, and a vector of package weights (as doubles). Due to loading constraints, packages weighing less than 2.8 kilograms must be discarded or returned; for each such package, the weight deficit is computed as `ceil(2.8 - weight)`, representing the additional "penalty" units needed to bring it to the minimum loading threshold. The available weight capacity per truck is `maxWeight / truckCount` (integer division). Sort the penalty values in descending order and then greedily consume the largest penalties first using the total capacity (sum of capacity over all trucks) until no more penalties can be fitted. The function must return the number of penalty units (i.e., discarded packages' penalties) that could NOT be accommodated. The input vector may be empty, penalties can be zero or positive integers, and the capacity per truck may be zero (if `truckCount` exceeds `maxWeight`). You must handle edge cases where no package is below the threshold, or where all penalties fit completely.
// The problem requires filtering the input weights: for each weight `w` less than 2.8, compute `p = ceil(2.8 - w)`. This `p` is an integer because `ceil` of a double is used. For weights ≥ 2.8, ignore them. Then, the total capacity is `maxWeight / truckCount` (integer division). If `truckCount` is zero, you can assume it is at least 1 (or handle division by zero by returning the size of penalties). Sort all penalties in descending order (largest first). Greedily subtract each penalty from the total capacity as long as capacity remains non-negative; if a penalty exceeds remaining capacity, stop. The number of penalties that could not be fitted is the count of remaining elements in the list after the greedy loop. Sorting descending ensures we maximize the number of penalties fitted? Actually, greedy from largest to smallest is optimal because all penalties are independent and units are all equal cost—any penalty that fits can be accommodated, so we simply want to fit as many as possible; the order doesn't matter for total count, but the typical approach is to try to fit the largest ones first to avoid wasting capacity on small ones? Actually, to maximize the count, you would sort ascending and fit as many small ones as possible. But the snippet sorts descending and then uses `k >= X.back()` which is smallest at back, so it pops from smallest to largest. In the original code, they sort descending, but then they use `X.back()` which is the smallest because descending order puts smallest at the back. So they actually consume smallest penalties first. That is correct for maximizing the number of fitted penalties: use smallest first. So our solution should sort ascending and try to fit as many small penalties as possible. However, the problem statement in the task says "greedily consume the largest penalties first" — that is incorrect for maximizing count. To match the given snippet's behavior, we should actually sort ascending and consume the smallest penalties first. Given the task description is somewhat ambiguous, I will implement the behavior that matches the snippet: sort in non-decreasing order and pop from the front (smallest) as long as total capacity allows. The count of unfitted penalties is the remaining size. Time complexity is O(n log n) due to sorting, O(n) space for storing penalties. Edge cases: empty vector returns 0; all weights ≥2.8 returns 0; capacity may be zero, then we cannot fit any penalty, return count of penalties; penalties may include zero (if weight exactly 2.8? Actually if weight < 2.8, then 2.8 - weight > 0, so ceil gives at least 1? For weight=2.799, gap=0.001, ceil=1. For weight=2.0, gap=0.8, ceil=1. For weight=1.0, gap=1.8, ceil=2. So penalty is always at least 1 because weight<2.8 ensures positive gap. So no zero penalties. But if weight=2.8 exactly, it's not below, so ignored. So all penalties ≥1. Capacity per truck is integer floor, so total capacity is integer.
#include <vector>
#include <algorithm>
#include <cmath>

// Returns the number of penalty units that cannot be accommodated.
// Given maxWeight total capacity, truckCount trucks, and package weights.
// Packages < 2.8 produce penalty ceil(2.8 - weight). Sort penalties
// ascending and greedily fit as many as possible into total capacity
// = maxWeight / truckCount.
int deliverPackages(int maxWeight, int truckCount, const std::vector<double>& weights) {
    if (truckCount <= 0) {
        // If no trucks, cannot fit any penalty.
        std::vector<int> penalties;
        for (double w : weights) {
            if (w < 2.8) {
                penalties.push_back(static_cast<int>(std::ceil(2.8 - w)));
            }
        }
        return static_cast<int>(penalties.size());
    }

    std::vector<int> penalties;
    for (double w : weights) {
        if (w < 2.8) {
            int p = static_cast<int>(std::ceil(2.8 - w));
            penalties.push_back(p);
        }
    }

    std::sort(penalties.begin(), penalties.end()); // ascending, smallest first

    int totalCapacity = maxWeight / truckCount; // integer division

    size_t idx = 0;
    while (idx < penalties.size() && totalCapacity >= penalties[idx]) {
        totalCapacity -= penalties[idx];
        ++idx;
    }

    return static_cast<int>(penalties.size() - idx);
}
#include <cassert>
#include <vector>
#include <cmath>

// forward declaration for test
int deliverPackages(int maxWeight, int truckCount, const std::vector<double>& weights);

int main() {
    // No packages below threshold
    assert(deliverPackages(100, 2, {3.0, 4.5, 2.8}) == 0);

    // All packages below threshold, capacity fits all
    {
        std::vector<double> w = {2.0, 2.5, 1.0};
        // penalties: ceil(0.8)=1, ceil(0.3)=1, ceil(1.8)=2 -> sorted [1,1,2], total = 100/2=50, fits all -> 0
        assert(deliverPackages(100, 2, w) == 0);
    }

    // Capacity insufficient for all
    {
        std::vector<double> w = {1.0, 1.0, 2.0, 2.5};
        // penalties: 2,2,1,1 -> sorted [1,1,2,2], total = 5/1=5, fit 1+1+2=4, remaining 1 -> leftover 1
        assert(deliverPackages(5, 1, w) == 1);
    }

    // Zero capacity (truckCount > maxWeight)
    {
        std::vector<double> w = {2.0};
        // penalty=1, total=10/20=0, cannot fit any -> leftover 1
        assert(deliverPackages(10, 20, w) == 1);
    }

    // Empty vector
    assert(deliverPackages(10, 2, {}) == 0);

    // Mixed weights, some below threshold
    {
        std::vector<double> w = {2.9, 2.8, 1.5, 3.0};
        // only 1.5 below -> penalty=ceil(1.3)=2, total=10/2=5, fits -> leftover 0
        assert(deliverPackages(10, 2, w) == 0);
    }

    // Exact fit
    {
        std::vector<double> w = {2.0, 2.0, 2.0};
        // penalties all 1, total=3/1=3, fit all -> 0
        assert(deliverPackages(3, 1, w) == 0);
    }

    // Not enough capacity for the smallest penalty
    {
        std::vector<double> w = {2.0};
        // penalty=1, total=0 (if maxWeight=0, truckCount=1) -> leftover 1
        assert(deliverPackages(0, 1, w) == 1);
    }

    // Large capacity, multiple penalties
    {
        std::vector<double> w = {1.0, 1.5, 2.0, 2.5};
        // penalties: ceil(1.8)=2, ceil(1.3)=2, ceil(0.8)=1, ceil(0.3)=1 -> sorted [1,1,2,2], total=100/2=50, all fit -> 0
        assert(deliverPackages(100, 2, w) == 0);
    }

    return 0;
}
