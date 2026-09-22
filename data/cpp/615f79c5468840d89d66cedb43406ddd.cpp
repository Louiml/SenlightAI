Write a C++ function `int maxOddSum(const std::vector<int>& numbers)` that, given a list of `N` positive integers (1 ≤ N ≤ 100, each integer between 1 and 1000), returns the maximum sum of a subset (any selection of elements, not necessarily contiguous) whose total sum is odd. If no such subset exists (which can only happen if all elements are even and the sum of all is even, but since all are positive, the only impossible case is when all numbers are even — then the answer is 0 because the empty subset has sum 0, which is even and not allowed, and any non-empty subset of evens is even), return 0. The function should correctly handle cases with all odd numbers, mixed parity, and all even numbers.

#include <cassert>
#include <vector>

int maxOddSum(const std::vector<int>& numbers);

int main() {
    // All odd numbers: total is odd → return total.
    assert(maxOddSum({1, 3, 5}) == 9);
    // Mixed: total = 1+2+3+4 = 10 (even), smallest odd = 1 → 10-1=9.
    assert(maxOddSum({1, 2, 3, 4}) == 9);
    // All even: no odd sum possible → 0.
    assert(maxOddSum({2, 4, 6}) == 0);
    // Single odd: returns that odd.
    assert(maxOddSum({7}) == 7);
    // Single even: returns 0.
    assert(maxOddSum({8}) == 0);
    // Larger mixed: total = 100 (even), smallest odd = 3 → 97.
    assert(maxOddSum({3, 5, 12, 20, 60}) == 97);
    // Odd total with evens mixed: total = 1+2+4+6=13 (odd) → 13.
    assert(maxOddSum({1, 2, 4, 6}) == 13);
    // All odds but smallest removal not needed because total odd.
    assert(maxOddSum({1, 1, 1}) == 3);
    // Edge: empty vector (though spec says N≥1) → 0.
    assert(maxOddSum({}) == 0);
    // Maximum possible values: 1000 each, 100 elements, all odd → total 100000 (even) but smallest odd 1 → 99999.
    std::vector<int> big(100, 1000);
    big[0] = 1; // make at least one odd
    assert(maxOddSum(big) == 99999);
}

#include <vector>
#include <algorithm>
#include <limits>

// Returns the maximum sum of a subset (any selection) whose total is odd.
// If no odd sum is possible (only possible when all numbers are even), returns 0.
int maxOddSum(const std::vector<int>& numbers) {
    if (numbers.empty()) return 0;

    int total = 0;
    int minOdd = std::numeric_limits<int>::max();

    for (int value : numbers) {
        total += value;
        if (value % 2 != 0) {
            minOdd = std::min(minOdd, value);
        }
    }

    if (total % 2 != 0) {
        return total;               // total is already odd
    } else if (minOdd != std::numeric_limits<int>::max()) {
        return total - minOdd;      // remove smallest odd -> odd sum
    } else {
        return 0;                   // no odd numbers at all
    }
}

// The key observation is that the maximum odd sum is simply the sum of all numbers, unless that total sum is even. If the total sum is even, we need to remove the smallest odd number (if any) to make the sum odd, because removing an even number would keep the sum even, and removing a larger odd number would produce a smaller odd sum than removing the smallest odd. If there are no odd numbers, then every subset sum is even (since sum of evens is even), so the answer is 0. The algorithm: compute `total = sum(numbers)`. Find `minOdd` = minimum odd number among the numbers (if any). If `total` is odd, return `total`. Else if `minOdd` exists (i.e., there is at least one odd), return `total - minOdd` (which will be odd because even minus odd is odd). Else return 0. Edge cases: empty vector? The problem states N ≥ 1, but for safety, if the vector is empty, we can return 0. Also note that all numbers are positive, so no zeros or negatives. Time complexity: O(N) for summing and scanning for min odd. Space: O(1).
