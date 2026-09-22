Write a C++ function `hasSuddenJump` that takes a vector of positive integers and returns `true` if, after sorting the numbers in non-decreasing order, there exists an adjacent pair `(a[i-1], a[i])` where `a[i]` is strictly greater than `a[i-1]` (i.e., the two values are distinct) and `a[i]` is strictly less than twice `a[i-1]`. In other words, the function checks whether the sorted array contains two neighboring elements whose values are different and where the larger value is neither equal to nor at least twice the smaller one (so the larger value is strictly between one and two times the smaller). If such a pair exists, return `true`; otherwise return `false`. The input vector may contain duplicates, and you may assume it is non-empty. For example, for `{5, 3, 10}`, after sorting to `{3,5,10}`, the pair `(3,5)` satisfies the condition (5>3 and 5<6), so the function returns `true`; for `{2,4,8}`, after sorting to `{2,4,8}`, the pair `(4,8)` fails (8 is not <8), and `(2,4)` also fails (4 is not <4), so it returns `false`. Your function must be efficient and handle large inputs gracefully.
#include <cassert>
#include <vector>

int main() {
    // Basic true case: (3,5) satisfies 5<6.
    assert(hasSuddenJump({5, 3, 10}) == true);

    // Basic false case: (2,4) fails because 4 is not <4.
    assert(hasSuddenJump({2, 4, 8}) == false);

    // All duplicates: no distinct pair.
    assert(hasSuddenJump({7, 7, 7}) == false);

    // Single element: no adjacent pair.
    assert(hasSuddenJump({1}) == false);

    // Two elements where condition holds: (1,3) because 3<2? Actually 3<2 is false, so test (1,2) fails (2 not <2), (1,1) fails, but (2,3) holds: 3<4.
    assert(hasSuddenJump({3, 2}) == true); // sorted {2,3}, 3<4.

    // Case where only a non-adjacent pair would seem to hold, but adjacent catches it.
    // {1,5,9} sorted: (1,5) fails (5<2 false), (5,9) fails (9<10 true? 9<10 yes, so true). Actually this is true.
    // For false check: {1,4,8} -> (1,4) fails, (4,8) fails because 8<8 false.
    assert(hasSuddenJump({1, 4, 8}) == false);

    // Larger array with mixed values.
    assert(hasSuddenJump({10, 20, 15, 5}) == true); // sorted {5,10,15,20}: (5,10) fails, (10,15) holds (15<20) -> true.

    // Edge case with large numbers to avoid overflow: use long long? Not needed for int, but check 1000000000 and 1500000000 product 2*1e9=2e9 fits in int? 2e9 < 2.147e9 so ok.
    assert(hasSuddenJump({1000000000, 1500000000}) == true); // 1.5e9 < 2e9.

    // Case where the condition fails for all due to equal spacing.
    assert(hasSuddenJump({3, 6, 12}) == false); // (3,6):6<6 false, (6,12):12<12 false.

    return 0;
}
#include <vector>
#include <algorithm>

// Returns true if, after sorting, there exists a distinct adjacent pair
// where the larger value is strictly less than twice the smaller.
bool hasSuddenJump(const std::vector<int>& numbers) {
    if (numbers.size() < 2) {
        return false;
    }

    // Work on a copy to keep the input unmodified (const-correctness).
    std::vector<int> sorted = numbers;
    std::sort(sorted.begin(), sorted.end());

    for (std::size_t i = 1; i < sorted.size(); ++i) {
        // Check distinct values and the strict inequality condition.
        if (sorted[i] != sorted[i - 1] && sorted[i] < sorted[i - 1] * 2) {
            return true;
        }
    }

    return false;
}
// The solution sorts the input vector in non-decreasing order. Once sorted, only adjacent elements need to be checked because the condition depends on the immediate predecessor in sorted order. For each `i` from 1 to `N-1`, we check if `a[i] != a[i-1]` (to ensure distinct values) and `a[i] < a[i-1] * 2` (the strict upper bound). If both conditions hold, return `true` immediately; otherwise continue. If no pair satisfies the condition, return `false`. This works because sorting guarantees that any pair of elements that could satisfy the condition must be adjacent in sorted order: if there were a non-adjacent pair `(x, y)` with `x < y` and `y < 2x`, then any element `z` between them in sorted order would either be equal to `x` (so the adjacent pair `(x, z)` might also work if `z` is distinct, but if all duplicates, then `y` would be adjacent to the last duplicate) or `z` would be between `x` and `y`, and then the adjacent pair `(z, y)` would also satisfy the condition because `y < 2x <= 2z` (since `z >= x`) and `y > z`, so the check would catch it. Thus scanning adjacent pairs is sufficient. Edge cases include all elements equal (then no distinct pair, returns false) and small arrays (length 1 or 2). Time complexity is `O(N log N)` due to sorting, and space complexity is `O(1)` auxiliary if we sort in place (but we will copy the input to avoid modifying it, so `O(N)` extra). The reference solution will take a `const std::vector<int>&` and make a local copy for sorting, or sort a copy to preserve const-correctness.
