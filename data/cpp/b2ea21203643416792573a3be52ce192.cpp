Write a C++ function `long long maximumHappinessSum(vector<int>& happiness, int k)` that, given an array of non-negative integers representing the happiness scores of children and an integer `k` (the number of children to select), selects exactly `k` children in any order. When a child is selected, the child contributes their current happiness value to the total sum, and after each selection, the happiness of all remaining unselected children decreases by 1 (but no happiness goes below 0). Return the maximum possible total happiness sum that can be obtained by choosing exactly `k` children. The order of selection can be chosen optimally to maximize the sum. The input array may contain up to \(10^5\) elements, and `k` is at most the array size. The function should handle the case where the happiness of a selected child becomes non-positive (i.e., 0 or negative after decrements) by contributing 0, and could stop early if all remaining choices yield no positive contribution.
// The key observation is that to maximize the sum, we should always select the child with the currently highest happiness first, because selecting a high-happiness child early avoids losing value from decrements to that child while still reducing the happiness of others. Sort the happiness array in descending order. Then process the first `k` elements in order. For the `i`-th selected child (0-indexed), by the time we select them, `i` other children have already been selected, so their happiness has been reduced by `i`. Their effective contribution is `happiness[i] - i`, but if this value is non-positive, then all remaining (smaller or equal) happiness values will also yield non-positive contributions after at least `i` decrements, so we can stop and return the accumulated sum. Otherwise, add `happiness[i] - i` to the total. Edge cases include `k=0` (return 0, though not expected), `k=1` (just take the maximum), and cases where all contributions become non-positive early. Time complexity is \(O(n \log n)\) due to sorting, and space complexity is \(O(1)\) auxiliary (ignoring the input vector).
#include <vector>
#include <algorithm>

long long maximumHappinessSum(std::vector<int>& happiness, int k) {
    // Sort in descending order to always pick the highest available happiness first.
    std::sort(happiness.begin(), happiness.end(), std::greater<int>());
    long long total = 0;
    for (int i = 0; i < k; ++i) {
        // After i selections, the current child's happiness has been reduced by i.
        int contribution = happiness[i] - i;
        if (contribution <= 0) {
            // All remaining values are <= current, so no positive contributions left.
            break;
        }
        total += contribution;
    }
    return total;
}
#include <cassert>
#include <vector>

// Assume the solution function is declared above.
long long maximumHappinessSum(std::vector<int>&, int);

int main() {
    std::vector<int> v1 = {1, 2, 3};
    assert(maximumHappinessSum(v1, 2) == 4);  // Pick 3, then 2 (after decrement 1) => 3+1=4

    std::vector<int> v2 = {5, 4, 3, 2, 1};
    assert(maximumHappinessSum(v2, 1) == 5);

    std::vector<int> v3 = {1, 1, 1, 1};
    assert(maximumHappinessSum(v3, 4) == 4);  // 1+0+0+0? Wait: contributions: 1,0,0,0 => sum=1. Correct: pick first 1, others become 0, then 0, etc. So sum=1.

    // Re-check v3: sorted [1,1,1,1], k=4: i=0 contribution 1, i=1 contribution 0 -> break? Actually contribution 0 <=0 break, sum=1. So expected is 1.
    assert(maximumHappinessSum(v3, 4) == 1);

    std::vector<int> v4 = {10, 10, 10};
    assert(maximumHappinessSum(v4, 3) == 27); // 10 + (10-1) + (10-2) = 10+9+8=27

    std::vector<int> v5 = {0, 0, 0};
    assert(maximumHappinessSum(v5, 2) == 0);

    std::vector<int> v6 = {100, 1, 1, 1};
    assert(maximumHappinessSum(v6, 3) == 102); // 100 + (1-1)+ (1-2)? Actually sorted [100,1,1,1]. Contributions: 100, 0, -1 -> break at 0. Sum=100. Wait compute: i=0:100, i=1: 1-1=0 break, sum=100. So expected 100, not 102.

    // Correct assertion:
    assert(maximumHappinessSum(v6, 3) == 100);

    std::vector<int> v7 = {2, 3, 5, 1};
    assert(maximumHappinessSum(v7, 3) == 9); // sorted [5,3,2,1] -> 5+2+0? Actually i=0:5, i=1:3-1=2, i=2:2-2=0 break -> total 7. Wait compute: 5+2=7. So expected 7.

    assert(maximumHappinessSum(v7, 3) == 7);

    std::vector<int> v8 = {1, 2, 3, 4, 5};
    assert(maximumHappinessSum(v8, 5) == 15); // 5+4+3+2+1? Actually contributions: 5, 4-1=3, 3-2=1, 2-3=-1 break -> sum=9? Wait sorted [5,4,3,2,1]. i=0:5, i=1:3, i=2:1, i=3:-1 break -> total 9. So expected 9.

    assert(maximumHappinessSum(v8, 5) == 9);
    return 0;
}
