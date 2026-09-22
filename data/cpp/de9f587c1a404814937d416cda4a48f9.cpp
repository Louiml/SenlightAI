// Write a C++ function `int kthSmallestProduct(const std::vector<int>& arr, int k)` that, given a sorted (non-decreasing) array `arr` of integers and a positive integer `k`, returns the k-th smallest value among all possible products `arr[i] * arr[j]` for `0 ≤ i ≤ j < n`. The array may contain negative numbers, zeros, and duplicates, and the input array is guaranteed to be sorted in non-decreasing order. The function must handle `k` up to `n*(n+1)/2` (where `n` is the array size), and you should assume the array has at least one element. The products are to be considered with duplicates counted separately (i.e., if two index pairs yield the same product, they each count toward the k-th position).

#include <cassert>
#include <vector>

int main() {
    // Test 1: Basic positive numbers
    std::vector<int> a = {1, 2, 3};
    assert(kthSmallestProduct(a, 1) == 1); // 1*1
    assert(kthSmallestProduct(a, 2) == 2); // 1*2
    assert(kthSmallestProduct(a, 3) == 3); // 1*3 or 2*2? Actually 1*3=3, 2*2=4, so 3
    assert(kthSmallestProduct(a, 4) == 4); // 2*2
    assert(kthSmallestProduct(a, 5) == 6); // 2*3
    assert(kthSmallestProduct(a, 6) == 9); // 3*3

    // Test 2: Negative numbers
    std::vector<int> b = {-3, -2, -1};
    assert(kthSmallestProduct(b, 1) == 1); // (-1)*(-1)=1
    assert(kthSmallestProduct(b, 2) == 2); // (-1)*(-2)=2
    assert(kthSmallestProduct(b, 3) == 3); // (-1)*(-3)=3 or (-2)*(-2)=4, so 3
    assert(kthSmallestProduct(b, 4) == 4); // (-2)*(-2)=4
    assert(kthSmallestProduct(b, 5) == 6); // (-2)*(-3)=6
    assert(kthSmallestProduct(b, 6) == 9); // (-3)*(-3)=9

    // Test 3: Mixed signs
    std::vector<int> c = {-5, -2, 0, 3, 4};
    // All products:
    // -5*-5=25, -5*-2=10, -5*0=0, -5*3=-15, -5*4=-20
    // -2*-2=4, -2*0=0, -2*3=-6, -2*4=-8
    // 0*0=0, 0*3=0, 0*4=0
    // 3*3=9, 3*4=12, 4*4=16
    // Sorted: -20, -15, -8, -6, 0,0,0,0, 4,9,10,12,16,25
    assert(kthSmallestProduct(c, 1) == -20);
    assert(kthSmallestProduct(c, 2) == -15);
    assert(kthSmallestProduct(c, 3) == -8);
    assert(kthSmallestProduct(c, 4) == -6);
    assert(kthSmallestProduct(c, 5) == 0); // first zero
    assert(kthSmallestProduct(c, 9) == 4); // first positive

    // Test 4: Duplicates and zeros
    std::vector<int> d = {0, 0, 2};
    assert(kthSmallestProduct(d, 1) == 0);
    assert(kthSmallestProduct(d, 2) == 0);
    assert(kthSmallestProduct(d, 3) == 0); // 0*2 also 0
    assert(kthSmallestProduct(d, 4) == 0); // pairs: (0,0), (0,0), (0,2), (0,2), (0,0), (2,2)
    assert(kthSmallestProduct(d, 5) == 0);
    assert(kthSmallestProduct(d, 6) == 4); // 2*2

    // Test 5: Single element
    std::vector<int> e = {5};
    assert(kthSmallestProduct(e, 1) == 25);

    // Test 6: Large negatives and positives
    std::vector<int> f = {-10, -1, 1, 10};
    assert(kthSmallestProduct(f, 1) == -100); // -10*10
    assert(kthSmallestProduct(f, 2) == -10); // -10*1 or -1*10
    assert(kthSmallestProduct(f, 5) == 1); // -1*-1, or 1*1
    assert(kthSmallestProduct(f, 8) == 100); // 10*10
}

#include <vector>
#include <algorithm>

// Count how many pairs (i, j) with i <= j have product <= mid.
long long countPairs(const std::vector<int>& arr, long long mid) {
    int n = arr.size();
    long long count = 0;
    int neg = 0, pos = n - 1;
    
    for (int i = 0; i < n; ++i) {
        if (arr[i] < 0) {
            // For negative arr[i], arr[i]*arr[j] <= mid means arr[j] >= ceil(mid / arr[i]).
            // Since arr[i] is negative, divide and round up toward zero (actually toward negative infinity for quotient).
            long long need = mid / arr[i];
            // For negative divisor, integer division truncates toward zero, so adjust for ceil toward negative infinity.
            while (neg < n && arr[neg] * arr[i] <= mid) neg++;
            count += (neg - i); // only j >= i
        } else if (arr[i] == 0) {
            count += (n - i); // all j >= i give product 0
        } else {
            // For positive arr[i], need arr[j] <= mid / arr[i] (floor division).
            long long maxJ = mid / arr[i];
            while (pos >= i && arr[pos] > maxJ) pos--;
            if (pos >= i) count += (pos - i + 1);
        }
    }
    return count;
}

// Return the k-th smallest product (with k starting from 1).
int kthSmallestProduct(const std::vector<int>& arr, int k) {
    int n = arr.size();
    long long low = (long long)arr[0] * arr[0];
    long long high = (long long)arr[n-1] * arr[n-1];
    // Ensure low is the actual minimum product, high is max.
    low = std::min(low, (long long)arr[0] * arr[n-1]);
    high = std::max(high, (long long)arr[0] * arr[n-1]);
    
    while (low < high) {
        long long mid = low + (high - low) / 2;
        long long cnt = countPairs(arr, mid);
        if (cnt >= k) high = mid;
        else low = mid + 1;
    }
    return (int)low;
}

// The main challenge is efficiently finding the k-th smallest product without generating all products explicitly (which would be O(n²) in time and memory). Since the array is sorted, we can use a binary search over the possible product values. The search range is from `arr[0] * arr[0]` (smallest possible product, which occurs when both factors are the most negative or most positive, depending on signs) to `arr[n-1] * arr[n-1]` (largest possible product). For a given candidate product `mid`, we count how many products are ≤ `mid` using a two-pointer technique that exploits the sorted array. For each element `arr[i]`, we count how many `j ≥ i` such that `arr[i] * arr[j] ≤ mid`. Since both indices are from the same sorted array, we can handle negative, zero, and positive elements by considering sign cases. Specifically, for positive `arr[i]`, we need the number of `j` where `arr[j] ≤ mid / arr[i]` (using integer division carefully with truncation toward zero). For negative `arr[i]`, the inequality flips, so we need `arr[j] ≥ ceil(mid / arr[i])`. For zero, any product is zero, so we count all `j ≥ i`. The counting uses two pointers: one from the left for negative, one from the right for positive. After counting, if the count is ≥ `k`, we move the high bound down; otherwise, we move the low bound up. This binary search takes O(log(range)) iterations, each counting O(n), giving O(n log(range)) time and O(1) auxiliary space. Edge cases include arrays with all negative numbers, all positive, mixed signs, and zeros, as well as `k` equal to the total number of pairs.
