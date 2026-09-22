Write a C++ function `int countCombinations(const std::array<int,6>& quantities)` that, given the available counts of six distinct coin denominations (1, 2, 3, 5, 10, and 20 cents), returns the number of distinct total sums that can be formed using any number (including zero) of each coin type, as long as the number used of each type does not exceed its given quantity. The output should include the sum of zero (i.e., using no coins) but the returned count should exclude that zero sum. For example, if quantities are `{1,0,0,0,0,0}`, the only non-zero sum is 1, so return 1. The input quantities are non-negative and can be zero. The function must handle the case where all quantities are zero, in which case there are no non-zero sums, so return 0. The total number of possible sums will fit in an `int`.

#include <cassert>
#include <array>

// Declaration of the function under test
int countCombinations(const std::array<int, 6>& quantities);

int main() {
    // Case 1: only one 1-cent coin → sums {0,1} → non-zero = 1
    assert(countCombinations({1, 0, 0, 0, 0, 0}) == 1);

    // Case 2: all zero quantities → only sum 0 → non-zero = 0
    assert(countCombinations({0, 0, 0, 0, 0, 0}) == 0);

    // Case 3: one of each denomination (1,2,3,5,10,20) → all sums 0..41? Actually check manually:
    // With one of each, you can form any sum from 0 to 41? Let's verify: possible sums are all subsets.
    // There are 2^6=64 subsets, but sums may duplicate? With distinct coprime-ish values, likely all distinct? 
    // We'll just compute a few known values: 1,2,3,5,10,20 and combinations like 1+2=3 (duplicate with 3),
    // so not all distinct. Let's just test a known small case.
    // For one of {1,2,3}: sums {0,1,2,3,1+2=3,1+3=4,2+3=5,1+2+3=6} → distinct {0,1,2,3,4,5,6} → 6 non-zero sums.
    assert(countCombinations({1, 1, 1, 0, 0, 0}) == 6);  // {1,2,3} → sums 1..6

    // Case 4: two of the 1-cent coin only → sums {0,1,2} → 2 non-zero
    assert(countCombinations({2, 0, 0, 0, 0, 0}) == 2);

    // Case 5: one 2-cent and one 3-cent → sums {0,2,3,5} → non-zero 3
    assert(countCombinations({0, 1, 1, 0, 0, 0}) == 3);

    // Case 6: one 5-cent and one 10-cent → sums {0,5,10,15} → 3 non-zero
    assert(countCombinations({0, 0, 0, 1, 1, 0}) == 3);

    // Case 7: one 20-cent only → sums {0,20} → 1 non-zero
    assert(countCombinations({0, 0, 0, 0, 0, 1}) == 1);

    // Case 8: quantities 1 for all six denominations (1,1,1,1,1,1) – compute distinct sums?
    // Total sum of all coins = 1+2+3+5+10+20=41. Number of reachable sums could be many. 
    // Let's just sanity-check that the result is positive and reasonable: it should be 41? Actually,
    // with one of each, you can make every sum from 0 to 41? Let's test a few: 1,2,3 (using coin 3), 4=1+3, 5 (coin), 6=1+5, 7=2+5, 8=3+5, 9=1+3+5, 10 (coin), 11=1+10, 12=2+10, 13=3+10, 14=1+3+10, 15=5+10, 16=1+5+10, 17=2+5+10, 18=3+5+10, 19=1+3+5+10, 20 (coin), 21=1+20, ... it seems all 0..41 are possible. So result should be 41.
    assert(countCombinations({1, 1, 1, 1, 1, 1}) == 41);

    // Case 9: one 1, one 2, one 5 → sums {0,1,2,3,5,6,7,8} → non-zero 7
    assert(countCombinations({1, 1, 0, 1, 0, 0}) == 7);

    // Case 10: zero 1-cent, one 2-cent, one 3-cent, one 4? but no 4 → use 5: {0,2,3,5,7,8} → non-zero 5
    assert(countCombinations({0, 1, 1, 1, 0, 0}) == 5);

    return 0;
}

#include <array>
#include <set>

// Count the number of distinct non-zero sums that can be formed
// using at most quantities[i] coins of denomination denom[i],
// where denom = {1,2,3,5,10,20}.
int countCombinations(const std::array<int, 6>& quantities) {
    const std::array<int, 6> denominations = {1, 2, 3, 5, 10, 20};
    std::set<int> sums;  // stores all reachable sums including 0

    // Depth-first search over denominations.
    // index: current denomination position (0..5)
    // currentSum: accumulated sum so far
    // coinCount: number of coins of the current denomination already added
    // (not needed for logic, but we emulate the original structure)
    // We use a nested lambda for simplicity.
    std::function<void(int, int)> dfs = [&](int index, int currentSum) {
        sums.insert(currentSum);
        if (index == 6) {
            return;  // all denominations processed
        }
        // Try using 0 to quantities[index] coins of the next denomination
        for (int used = 0; used <= quantities[index]; ++used) {
            dfs(index + 1, currentSum + used * denominations[index]);
        }
    };

    dfs(0, 0);  // start with index 0 and sum 0

    // Subtract 1 for the zero sum
    return static_cast<int>(sums.size()) - 1;
}

// The problem is a classic subset-sum counting problem with bounded multiplicities. We can solve it using a depth-first search (DFS) over the six denominations, similar to the provided snippet. Each DFS state tracks the current denomination index, the accumulated sum, and the count of that denomination used. At each call, we insert the current accumulated sum into a `std::set` to automatically remove duplicates. Then, if we are not at the last denomination, we iterate through all possible counts (from 0 to the available quantity) for the next denomination and recurse. The base case stops when we have processed all six denominations. After the DFS completes, the set contains all possible sums, including 0 (when all counts are zero). We return `set.size() - 1` to exclude the zero sum. This approach correctly handles zero quantities: if a denomination has quantity 0, the loop for it only runs once (i=0), so it contributes nothing new. Edge cases: all quantities zero → set contains only {0} → return 0. Time complexity is \(O(\prod_{i=1}^{6} (q_i+1))\) in the worst case, which is the number of combinations of coin counts; with the `std::set` insertion overhead of \(O(\log S)\) where \(S\) is the number of distinct sums, the total is \(O(C \cdot \log S)\). Auxiliary space is \(O(S)\) for the set plus \(O(6)\) for the recursion stack. For typical small quantities (each <= 20), this is efficient. Using a `std::set` guarantees uniqueness and sorted order, but we only need size.
