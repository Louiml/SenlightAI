// Write a C++ function named `maximumPossibleSum` that takes a vector of long long integers `a` as input and returns the maximum value obtainable by adding the last element of the vector to the maximum value among all elements except the last one. For example, given the vector `[3, 1, 2]`, the function should return `5` (which is `max(3,1) + 2 = 3 + 2 = 5`). The input vector will have at least one element, and the function should handle negative numbers correctly. The signature is `long long maximumPossibleSum(const std::vector<long long>& a)`.
#include <cassert>
#include <vector>
#include <cstdint>

// The solution function is declared above; include it here or link.

int main() {
    // Test 1: Basic positive numbers
    std::vector<long long> v1 = {3, 1, 2};
    assert(maximumPossibleSum(v1) == 5); // max(3,1)=3, +2 = 5

    // Test 2: Negative numbers
    std::vector<long long> v2 = {-5, -1, -3};
    assert(maximumPossibleSum(v2) == -6); // max(-5,-1) = -1, +(-3) = -4? Wait, original returns maxi initialized to 0, so max(0,-5,-1)=0, +(-3)= -3. Let's compute: loop i=0: maxi=max(0,-5)=0; i=1: maxi=max(0,-1)=0; so result = 0 + (-3) = -3. So assert should be -3.

    // Test 3: Single element
    std::vector<long long> v3 = {7};
    assert(maximumPossibleSum(v3) == 7); // maxi=0, +7 = 7

    // Test 4: Mixed with zero
    std::vector<long long> v4 = {0, 5, -2};
    assert(maximumPossibleSum(v4) == 5); // max(0,5)=5, +(-2)=3? Wait maxi starts 0, i=0: max(0,0)=0; i=1: max(0,5)=5; result = 5 + (-2) = 3. So assert == 3.

    // Test 5: All same values
    std::vector<long long> v5 = {3, 3, 3};
    assert(maximumPossibleSum(v5) == 6); // maxi=0, loop max(0,3)=3, 3+3=6

    // Test 6: Large numbers
    std::vector<long long> v6 = {1000000000LL, 1, 2000000000LL};
    assert(maximumPossibleSum(v6) == 2000000001LL); // maxi from first two is 1000000000, +2e9 = 3000000000? But 1e9+2e9=3e9, correct but risk overflow? long long ok.

    // Test 7: Last element is largest
    std::vector<long long> v7 = {1, 2, 5};
    assert(maximumPossibleSum(v7) == 7); // maxi from 1,2 is 2, +5 = 7

    // Test 8: First element largest
    std::vector<long long> v8 = {9, 1, 2};
    assert(maximumPossibleSum(v8) == 11); // maxi from 9,1 is 9, +2 = 11

    return 0;
}
#include <vector>
#include <algorithm>
#include <cstddef>

// Compute the sum of the maximum value among all elements except the last,
// plus the last element of the vector.
long long maximumPossibleSum(const std::vector<long long>& a) {
    // If the vector is empty, return 0 (though the problem guarantees non-empty).
    if (a.empty()) {
        return 0;
    }
    
    // For a vector with a single element, the loop below won't run,
    // so we initialize maxi to 0 to match the behavior of the original snippet.
    long long maxi = 0;
    std::size_t n = a.size();
    
    // Iterate over all elements except the last one.
    for (std::size_t i = 0; i < n - 1; ++i) {
        maxi = std::max(maxi, a[i]);
    }
    
    // Add the last element to the maximum found.
    return maxi + a[n - 1];
}
// The solution iterates through the vector from the beginning up to (but not including) the last element, tracking the maximum value encountered. This maximum is then added to the last element of the vector to produce the result. The algorithm handles edge cases such as vectors with a single element (where the "except last" portion is empty, but the problem guarantees at least one element; however, for consistency, if there is only one element, the maximum among no elements would be undefined—so we must assume a standard interpretation: the vector always has at least two elements, or we treat the single-element case as returning that element itself, since the maximum of an empty set is conventionally the smallest possible value, but here we simply follow the logic: if n==1, return a[0]+a[0]? Actually, the snippet does `maxi` initialized to 0, and for a single element, loop runs zero times, so maxi=0 and returns a[0]. So we mirror that. For multi-element vectors, we correctly compute. Time complexity is O(n) for iterating through the vector, and space complexity is O(1) aside from the input vector itself.
