Write a C++ function `maxMedianAfterIncrements` that takes a vector of `long long` integers, an integer `n` representing the number of elements, and an integer `k` representing the maximum total increment allowed. The function must return the maximum possible value of the median of the array after applying at most `k` total increments (each increment adds 1 to any single element). You can increment elements arbitrarily, but the total number of increments applied must not exceed `k`. The median is defined as the element at index `n/2` (0-based) in the sorted array. If the array has only one element, the median is that element. The array may contain duplicates. The function should handle `n` up to 100,000 and `k` up to 2e9, and element values up to 2e9. You may assume that all input values fit in a `long long`.
// The solution sorts the array first, so the median position is fixed at index `n/2`. To maximize the median, we only need to consider elements from index `n/2` to `n-1` because elements to the left of the median cannot affect the median value (they are always less than or equal to the median). We binary search on the possible median value `m` in the range from the current median value to a high upper bound (e.g., `2e9 + k`). For a candidate `m`, we compute the total increments required to raise all elements from index `n/2` to `n-1` to at least `m` (i.e., sum of `max(0, m - ar[i])` for i >= n/2). If this required sum is <= `k`, then `m` is achievable and we try higher values; otherwise, we lower `m`. The binary search finds the maximum `m` that is achievable. Special case: if `n == 1`, the median is the single element, and we can simply return `ar[0] + k`. Time complexity is `O(n log n)` for sorting plus `O(n log(2e9))` for binary search, and space complexity is `O(n)` for the vector.
#include <vector>
#include <algorithm>
#include <cstdint>

// Returns the maximum possible median value after at most k total increments.
long long maxMedianAfterIncrements(std::vector<long long>& arr, int n, long long k) {
    if (n == 1) {
        return arr[0] + k;
    }
    std::sort(arr.begin(), arr.end());
    long long low = arr[n / 2];
    long long high = static_cast<long long>(2e9) + k;
    long long answer = low;
    
    while (low <= high) {
        long long mid = low + (high - low) / 2;
        long long required = 0;
        for (int i = n / 2; i < n; ++i) {
            if (arr[i] < mid) {
                required += mid - arr[i];
                if (required > k) break; // early exit to avoid overflow
            }
        }
        if (required <= k) {
            answer = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return answer;
}
#include <cassert>
#include <vector>
#include <iostream>

// The solution function is declared above (include it here or in a header).

int main() {
    // Test 1: Basic case from snippet
    std::vector<long long> a1 = {1, 2, 3, 4, 5};
    assert(maxMedianAfterIncrements(a1, 5, 5) == 5);

    // Test 2: Single element
    std::vector<long long> a2 = {10};
    assert(maxMedianAfterIncrements(a2, 1, 3) == 13);

    // Test 3: All elements equal
    std::vector<long long> a3 = {5, 5, 5};
    assert(maxMedianAfterIncrements(a3, 3, 10) == 10);

    // Test 4: Unsorted input, median must increase
    std::vector<long long> a4 = {7, 1, 3, 2, 6};
    // Sorted: 1,2,3,6,7 -> median=3, can raise to 4? Need to raise 3->4 (1), also 6 and 7 can stay. So max = 4 with k=1.
    assert(maxMedianAfterIncrements(a4, 5, 1) == 4);

    // Test 5: Large k, all elements can be raised
    std::vector<long long> a5 = {1, 2, 3, 4, 5};
    assert(maxMedianAfterIncrements(a5, 5, 100) == 23); // Sorted: can raise all to 23, need (22+21+20+19+18)=100, median=23

    // Test 6: k=0, median unchanged
    std::vector<long long> a6 = {9, 8, 7, 6, 5};
    // Sorted: 5,6,7,8,9 -> median=7
    assert(maxMedianAfterIncrements(a6, 5, 0) == 7);

    // Test 7: Even number of elements (n=4) median index = 2
    std::vector<long long> a7 = {1, 2, 3, 4};
    // Sorted: 1,2,3,4 -> median=3 (index 2). k=2: raise 3->4 (1), then 4->5 (1) => median becomes 5? Wait: after raising to 4, array = 1,2,4,4 median index2=4. Then raise that 4 to 5 requires increment of 1 for the element at index2 (which is now 4? Actually careful: we can raise any elements; to get median=5, need index2>=5, so raise both 3 and 4 to 5: cost 2+1=3 >2. So median=4.
    assert(maxMedianAfterIncrements(a7, 4, 2) == 4);

    // Test 8: Negative numbers
    std::vector<long long> a8 = {-5, -1, 0, 3};
    // Sorted: -5,-1,0,3 -> median index2=0. k=4: raise 0->4 (4) => median=4
    assert(maxMedianAfterIncrements(a8, 4, 4) == 4);

    // Test 9: Duplicates
    std::vector<long long> a9 = {2, 2, 2, 2};
    // Sorted: 2,2,2,2 -> median index2=2. k=4: raise all four to 3 cost 4 => median=3
    assert(maxMedianAfterIncrements(a9, 4, 4) == 3);

    // Test 10: Large values and k
    std::vector<long long> a10 = {2000000000LL, 2000000000LL};
    assert(maxMedianAfterIncrements(a10, 2, 100LL) == 2000000100LL);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
