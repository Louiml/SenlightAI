Write a C++ function `long long rangeSum(const std::vector<long long>& arr, int left, int right)` that accepts a 0-indexed vector of integers (which may contain negative values) and two inclusive indices `left` and `right` (with `0 <= left <= right < arr.size()`). The function must return the sum of all elements from index `left` through `right` inclusive. To be efficient for many queries, precompute a prefix-sum array once and then answer each query in constant time. The function must be reusable across multiple independent calls, meaning it should not rely on global state; instead, it should internally compute the prefix sums for the given vector each time it is called. The vector may be empty, but in that case no valid calls will be made with valid indices, so you do not need to handle empty input for actual queries. The function must correctly handle large sums (use `long long`) and negative numbers.

The core idea is to use a prefix-sum array `prefix` of size `n+1`, where `prefix[0] = 0` and `prefix[i] = prefix[i-1] + arr[i-1]` for `i = 1..n`. Then the sum from index `l` to `r` (0-indexed) is `prefix[r+1] - prefix[l]`. This works because `prefix[r+1]` contains the sum of all elements from index 0 through `r`, and `prefix[l]` contains the sum from 0 through `l-1`, so their difference gives exactly the sum from `l` to `r`. For negative numbers this still holds because addition is commutative and subtraction correctly removes the prefix. Edge cases: when `left = 0`, the result is simply `prefix[right+1]`; when `left = right`, the result is `arr[left]`. Time complexity: building the prefix array takes O(n), and each query takes O(1). Space complexity: O(n) for the prefix array. The function must be `const`-correct by taking the vector by `const std::vector<long long>&` and not modifying it.

#include <vector>

// Returns the sum of arr[left..right] inclusive (0-indexed) using a prefix-sum array.
long long rangeSum(const std::vector<long long>& arr, int left, int right) {
    int n = static_cast<int>(arr.size());
    std::vector<long long> prefix(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        prefix[i + 1] = prefix[i] + arr[i];
    }
    return prefix[right + 1] - prefix[left];
}

#include <cassert>
#include <vector>

// The function under test is declared above.

int main() {
    std::vector<long long> arr1 = {1, 2, 3, 4, 5};
    assert(rangeSum(arr1, 0, 4) == 15);
    assert(rangeSum(arr1, 1, 3) == 9);
    assert(rangeSum(arr1, 2, 2) == 3);
    assert(rangeSum(arr1, 0, 0) == 1);

    std::vector<long long> arr2 = {-5, 10, -3, 7};
    assert(rangeSum(arr2, 0, 3) == 9);
    assert(rangeSum(arr2, 1, 2) == 7);
    assert(rangeSum(arr2, 3, 3) == 7);

    std::vector<long long> arr3 = {0, 0, 0};
    assert(rangeSum(arr3, 0, 2) == 0);

    std::vector<long long> arr4 = {1000000000LL, 1000000000LL};
    assert(rangeSum(arr4, 0, 1) == 2000000000LL);

    std::vector<long long> arr5 = {-1000000000LL, 1000000000LL, -1000000000LL};
    assert(rangeSum(arr5, 0, 2) == -1000000000LL);
}
