// You are given a non-decreasing array of `n` integers (already sorted in non-decreasing order). In one operation, you can choose any index `i` and decrease `arr[i]` by 1 (but you cannot make it negative). Your task is to determine whether it is possible to make the array strictly increasing by applying any number of operations. If it is not possible (i.e., the array is already not non-decreasing to begin with, or cannot be made strictly increasing because duplicates are too close together), the result is 0. If it is possible, return the minimum number of operations required to achieve a strictly increasing array. The input array is guaranteed to be sorted non-decreasing, but may contain duplicates and may have length at least 1. Write a function `int minOpsToMakeStrictlyIncreasing(const std::vector<int>& arr)` that returns the required number of operations as described.

// Since the array is already sorted non-decreasing, to make it strictly increasing we only need to fix consecutive equal or too-close values. The key observation is that the final strictly increasing array must have `arr[i] >= arr[i-1] + 1` for every `i > 0`. Because we can only decrease elements, the best strategy is to keep the leftmost elements as high as possible and decrease later elements minimally. However, since we can only decrement, if two adjacent elements are equal, we must decrease the right one by at least 1 to separate them. But decreasing the right one may then force further decreases for the next element, etc. Actually, the greedy approach from left to right works: if `arr[i] <= arr[i-1]`, we must reduce `arr[i]` to at most `arr[i-1]-1`. The minimal number of decrements needed to make `arr[i]` strictly greater than `arr[i-1]` is `arr[i] - (arr[i-1]-1)` if that is positive, else we need to reduce `arr[i-1]`? Wait, but we cannot increase, only decrease. So if `arr[i]` is already less than `arr[i-1]` (which cannot happen because array is sorted non-decreasing), it would be impossible. Since input is sorted non-decreasing, we have `arr[i] >= arr[i-1]`. If `arr[i] == arr[i-1]`, we must decrease `arr[i]` by at least 1 to make it `arr[i-1]-1`, requiring exactly 1 operation. But then after decreasing `arr[i]`, the next element `arr[i+1]` might be equal to the original `arr[i]`, and since we decreased `arr[i]`, the gap may shrink, but since we are processing left to right, we adjust each element to be exactly `arr[i-1]-1` if needed. Actually, the minimal total operations is found by making each element as large as possible while still satisfying `arr[i] > arr[i-1]` after previous modifications. The optimal is to keep the first element unchanged, then for each next element, if it is already `> previous final value`, we don't decrease it. If it is `<= previous final value`, we must decrease it to `previous final value - 1`, costing `(current value) - (previous final value - 1)` operations. But wait, we can only decrease, so if current value is already less than or equal to previous final, we must lower it. However, since array is non-decreasing originally, after we lower a previous element, the next original element might be large enough. Let's simulate: suppose array [1,1,1]. We keep first =1. Next original=1, previous final=1, so we must make it at most 0? But cannot go negative, so impossible => return 0. Indeed, if any element needs to become negative, it's impossible. So the condition for possibility is that after processing, the final value for the last element is >=0. The minimal ops is sum over i of max(0, (previous_final - (current_original - (some)))? Actually a simpler way: the minimal number of operations to make a non-decreasing array strictly increasing by only decreasing elements is to compute the maximum allowed final value for each index from right to left, and then sum the differences if current exceeds that maximum. But since we can only decrease, the final array must satisfy `final[i] <= original[i]` and `final[i] < final[i+1]`. The maximum possible final value for the last element is `original[n-1]`. For the second last, the maximum is `min(original[n-2], final[n-1]-1)`. Continuing backwards, we can compute the maximum feasible final values. Then the number of operations needed is sum of `original[i] - final[i]`. If any final value becomes negative, return 0. This is O(n) time and O(n) space (or O(1) if we process backwards and keep track). Edge cases: single element array is already strictly increasing, return 0. Already strictly increasing array (all adjacent differences >=1) requires 0 operations. Time complexity O(n), space O(1) auxiliary.

#include <vector>
#include <algorithm>
#include <climits>

// Returns the minimum number of decrement operations to make a non-decreasing
// array strictly increasing, or 0 if impossible.
int minOpsToMakeStrictlyIncreasing(const std::vector<int>& arr) {
    int n = static_cast<int>(arr.size());
    if (n <= 1) return 0;

    // Process from right to left, compute the maximum allowed final value.
    long long maxAllowed = arr[n-1];
    long long ops = 0;

    for (int i = n-2; i >= 0; --i) {
        // The current element must be strictly less than the next final value.
        long long currentMax = std::min<long long>(arr[i], maxAllowed - 1);
        if (currentMax < 0) {
            return 0; // Impossible to make strictly increasing without negatives.
        }
        ops += static_cast<long long>(arr[i]) - currentMax;
        maxAllowed = currentMax;
    }
    return static_cast<int>(ops);
}

#include <cassert>
#include <vector>

int main() {
    // Already strictly increasing
    assert(minOpsToMakeStrictlyIncreasing({1, 2, 3}) == 0);
    // Single element
    assert(minOpsToMakeStrictlyIncreasing({5}) == 0);
    // Duplicate adjacent requiring one decrement
    assert(minOpsToMakeStrictlyIncreasing({1, 1, 2}) == 1);
    // All equal, but possible: e.g., {2,2,2} -> final {2,1,0} costs 3
    assert(minOpsToMakeStrictlyIncreasing({2, 2, 2}) == 3);
    // Impossible because would need negative: {0,0,0}
    assert(minOpsToMakeStrictlyIncreasing({0, 0, 0}) == 0);
    // Larger gap, no ops needed if already increasing
    assert(minOpsToMakeStrictlyIncreasing({10, 20, 30}) == 0);
    // Mixed: {3,3,4} -> must make second to 2, third to 3? Actually final {3,2,?} but third original 4 must be >2, so final {3,2,3} but 3 not >2? Wait final must be strictly increasing, so {3,2,3} is not. Let's compute: from right, maxAllowed=4, i=1: currentMax=min(3,3)=3, ops+=3-3=0, maxAllowed=3; i=0: currentMax=min(3,2)=2, ops+=3-2=1, maxAllowed=2. Final array {2,3,4}? No, we are using original values but we only decrease, so final could be {2,3,4} but we cannot increase 3 to 4. Actually our computation gives final {2,3,4}? That would require increasing the middle from 3 to 3? Wait, final[i] is the value we keep after operations; we subtract ops from original. For arr={3,3,4}, we computed currentMax for i=1 as 3 (since min(3,4-1)=min(3,3)=3), so arr[1] stays 3. For i=0, currentMax = min(3, 3-1)=2, so arr[0] becomes 2. Final array {2,3,4} is strictly increasing and valid (we decreased 3 to 2 and kept 3 and 4). So ops=1. Check: original {3,3,4}, decrease first by 1 gives {2,3,4} -> strictly increasing. So assert 1 is correct.
    assert(minOpsToMakeStrictlyIncreasing({3, 3, 4}) == 1);
    // Long case
    assert(minOpsToMakeStrictlyIncreasing({5, 5, 5, 5}) == 6); // final {2,3,4,5}? Actually compute: from right maxAllowed=5, i=2: min(5,4)=4 ops+=1, maxAllowed=4; i=1: min(5,3)=3 ops+=2, maxAllowed=3; i=0: min(5,2)=2 ops+=3, total=6. final {2,3,4,5} strictly increasing.
    return 0;
}
