Write a C++ function `long long countBalancedTriplets(const std::vector<int>& a)` that takes a vector of integers and returns the number of ordered index pairs `(i, j, k)` satisfying `0 <= i < j < k < n` such that the sum of the three elements is even. The input vector may have up to `n = 2 * 10^5` elements, each within `[-10^9, 10^9]`. You must not use any additional data structures beyond a few scalar variables. The function should count triplets efficiently, not by brute force, and must handle duplicate values correctly.
#include <cassert>
#include <vector>

// Function declaration (definition provided separately)
long long countBalancedTriplets(const std::vector<int>& a);

int main() {
    // n < 3
    assert(countBalancedTriplets({}) == 0);
    assert(countBalancedTriplets({1}) == 0);
    assert(countBalancedTriplets({1, 2}) == 0);
    
    // Simple cases
    assert(countBalancedTriplets({1, 1, 1}) == 0); // all odd: sum odd
    assert(countBalancedTriplets({1, 1, 2}) == 1); // 1+1+2=4 even
    assert(countBalancedTriplets({1, 2, 3}) == 1); // 1+2+3=6 even
    assert(countBalancedTriplets({2, 4, 6}) == 1); // all even
    assert(countBalancedTriplets({2, 2, 2}) == 1); // all even
    assert(countBalancedTriplets({2, 2, 2, 2}) == 4); // C(4,3)=4
    assert(countBalancedTriplets({1, 1, 1, 2}) == 3); // C(3,2)*1 = 3
    
    // Mixed with duplicates
    assert(countBalancedTriplets({1, 1, 2, 2}) == 2); // all even? no: choose 1,1,2 -> 2 combos of odds, 2 choices of even -> 2
    // Actually: even: 2,2 (2 evens). odd:1,1 (2 odds). C(O,2)*E = 1*2=2. Also all even? C(E,3) with E=2 =0. total=2.
    
    // Larger test: 100 evens, 100 odds => all even: C(100,3) = 161700, twoOddOneEven: C(100,2)*100 = 4950*100=495000, total=656700
    std::vector<int> bigTest;
    for (int i = 0; i < 100; ++i) bigTest.push_back(2);
    for (int i = 0; i < 100; ++i) bigTest.push_back(1);
    assert(countBalancedTriplets(bigTest) == 656700LL);
    
    // Test with negative numbers
    assert(countBalancedTriplets({-2, -2, -2}) == 1);
    assert(countBalancedTriplets({-1, -1, 2}) == 1);
    assert(countBalancedTriplets({-1, -3, 4}) == 1);
    
    return 0;
}
#include <vector>
#include <cstdint>

// Returns the number of index triplets (i<j<k) with an even sum.
long long countBalancedTriplets(const std::vector<int>& a) {
    const std::size_t n = a.size();
    if (n < 3) {
        return 0LL;
    }
    long long evenCount = 0;
    long long oddCount = 0;
    for (int value : a) {
        if (value % 2 == 0) {
            ++evenCount;
        } else {
            ++oddCount;
        }
    }
    // All three even: C(E,3)
    // Exactly two odd, one even: C(O,2) * E
    long long allEvenTriplets = evenCount * (evenCount - 1) * (evenCount - 2) / 6;
    long long twoOddOneEven = oddCount * (oddCount - 1) / 2 * evenCount;
    return allEvenTriplets + twoOddOneEven;
}
// The task asks for the number of triplets `(i, j, k)` with `i < j < k` where `a[i] + a[j] + a[k]` is even. Since the sum is even, the parity (odd/even) of the three numbers must have an even number of odd numbers. That means either all three are even (0 odds), or exactly two are odd and one is even (2 odds). Because the sum of two odds gives even, and adding an even keeps it even. So we count:
// - Number of triplets where all three are even: `C(E, 3)` where `E` is the count of even numbers.
// - Number of triplets where exactly two are odd and one is even: `C(O, 2) * E` where `O` is the count of odd numbers.
//
// Since the input size can be large (up to 2e5), brute force O(n^3) is too slow. We just need to count how many even and odd numbers exist. The ordering `i < j < k` does not matter for the count, because for any selection of three distinct indices, there is exactly one way to order them increasingly. So the answer is simply the combinatorial sum. Edge cases: if n < 3, the answer is 0. Also, values can be large, but parity only matters. Use `long long` to avoid overflow because C(2e5, 3) is about 1.33e15, which fits in 64-bit. Time complexity O(n) and space O(1).
