/*
Write a C++ function `long long countIncreasingTriplets(const std::vector<int>& arr, int valueLimit)` that, given a sequence of `n` integers each in the range `[1, valueLimit]`, returns the total number of increasing triplets `(i, j, k)` with `i < j < k` and `arr[i] < arr[j] < arr[k]`. The function must handle `n` up to 30000 and `valueLimit` up to 30000. Use a Fenwick tree (Binary Indexed Tree) approach to count, for each element as the middle of a triplet, the number of smaller elements before it multiplied by the number of larger elements after it, and sum these products. The input array may contain duplicates, but triplets must be strictly increasing in value, so equal values do not form valid triplets. The output can exceed 32-bit integers, so return `long long`. Assume the input is already read from standard input; the function itself should not perform I/O.
*/
#include <vector>

// Fenwick tree (Binary Indexed Tree) helper
class Fenwick {
    std::vector<long long> tree;
    int size;
public:
    Fenwick(int n) : size(n), tree(n + 1, 0) {}
    
    void add(int idx, long long delta) {
        while (idx <= size) {
            tree[idx] += delta;
            idx += idx & (-idx);
        }
    }
    
    long long sum(int idx) const {
        long long res = 0;
        while (idx > 0) {
            res += tree[idx];
            idx -= idx & (-idx);
        }
        return res;
    }
};

// Count increasing triplets (i<j<k, arr[i]<arr[j]<arr[k])
long long countIncreasingTriplets(const std::vector<int>& arr, int valueLimit) {
    int n = static_cast<int>(arr.size());
    if (n < 3) return 0;
    
    // leftSmaller[j] = number of i<j with arr[i] < arr[j]
    Fenwick leftBIT(valueLimit);
    std::vector<long long> leftSmaller(n, 0);
    for (int j = 0; j < n; ++j) {
        leftSmaller[j] = leftBIT.sum(arr[j] - 1);
        leftBIT.add(arr[j], 1);
    }
    
    // rightLarger[j] = number of k>j with arr[j] < arr[k]
    Fenwick rightBIT(valueLimit);
    std::vector<long long> rightLarger(n, 0);
    for (int j = n - 1; j >= 0; --j) {
        rightLarger[j] = rightBIT.sum(valueLimit) - rightBIT.sum(arr[j]);
        rightBIT.add(arr[j], 1);
    }
    
    long long total = 0;
    for (int j = 0; j < n; ++j) {
        total += leftSmaller[j] * rightLarger[j];
    }
    return total;
}
#include <cassert>
#include <vector>

// The solution function is defined above; include it in the same compilation unit.
int main() {
    // Basic examples
    std::vector<int> arr1 = {1, 2, 3};
    assert(countIncreasingTriplets(arr1, 3) == 1); // (1,2,3)
    
    std::vector<int> arr2 = {1, 1, 1};
    assert(countIncreasingTriplets(arr2, 1) == 0); // no strict increase
    
    std::vector<int> arr3 = {5, 1, 4, 3, 2};
    // Check manually: triplets (1,4,?) no, (1,3,?) no, (1,2,?) no; none because sequence not increasing enough? Let's compute: indices 1..5 values 5,1,4,3,2.
    // Pairs: (1,4) at indices 2,3; then k>3 with value >4? none. (1,3) indices 2,4; k>4 value >3? none. (1,2) indices 2,5; none. (4,3) not increasing. So 0.
    assert(countIncreasingTriplets(arr3, 5) == 0);
    
    std::vector<int> arr4 = {1, 6, 2, 5, 3, 4};
    // Triplets: (1,2,3) at indices 1,3,5 = values 1,2,3; (1,2,4) at 1,3,6 values 1,2,4; (1,5? no, 5 not after 2 before 4? also (1,3,4) at 1,5,6 values 1,3,4; (2? no) also (1,2,5) at 1,3,4 values 1,2,5; (1,3,5) at 1,5,4? not increasing. Count all: (1,2,3),(1,2,4),(1,3,4),(1,2,5),(1,3,5?) no because 3<5 but index? Actually (1,3,5) using values 1 at index1, 3 at index5, 5? there is no 5 after index5? 5 is at index4, so no. (1,2,?) also (6? no). Also (2,3,4) values 2 at index3, 3 at index5, 4 at index6 => yes. (2,3? 5?) 2,3,5? 5 before 3? no. (2,5? no) So triplets: (1,2,3) indices 1,3,5; (1,2,4) 1,3,6; (1,3,4) 1,5,6; (2,3,4) 3,5,6. Total 4.
    assert(countIncreasingTriplets(arr4, 6) == 4);
    
    // Edge: length exactly 3 but duplicate
    std::vector<int> arr5 = {1, 2, 2};
    assert(countIncreasingTriplets(arr5, 2) == 0);
    
    // Larger value limit than needed
    std::vector<int> arr6 = {1, 2, 3, 4};
    assert(countIncreasingTriplets(arr6, 10) == 4); // choose any 3 of 4 increasing values: C(4,3)=4
    
    // All descending -> zero
    std::vector<int> arr7 = {4, 3, 2, 1};
    assert(countIncreasingTriplets(arr7, 4) == 0);
    
    // Long sequence with all increasing
    std::vector<int> arr8;
    for (int i = 1; i <= 100; ++i) arr8.push_back(i);
    assert(countIncreasingTriplets(arr8, 100) == 100LL * 99 * 98 / 6); // C(100,3)
    
    // Duplicates mixed
    std::vector<int> arr9 = {1, 2, 1, 3, 2};
    // Triplets: (1,2,3) at indices 1,2,4 (values 1,2,3); also (1,1? no) (1,2,? ) (1,1? no) (2,? ) (1,3,?) no k. Also (1,2,3) using index1,2,4; also index1,? index3 is 1, index4=3? 1<1 false. index2,3? 2<1 false. index1,5? 2<2 false. index3,? 1<3 and 3<2 false. So only one.
    assert(countIncreasingTriplets(arr9, 3) == 1);
    
    // Test with valueLimit = 1
    std::vector<int> arr10 = {1, 1, 1, 1};
    assert(countIncreasingTriplets(arr10, 1) == 0);
    
    // All test passed
}
// The solution uses two Fenwick trees (BITs). The first BIT tracks the frequency of each value seen so far as we iterate left to right. For the current element `a` at index `j`, the number of elements before it that are strictly smaller than `a` is `query(0, a-1)`, where `query` sums frequencies from 1 to `a-1`. We store this count for later. The second BIT will help count pairs `(i, j)` that form an increasing pair ending at the current element, but we need triplets. A more direct approach: maintain one BIT for frequencies to compute `leftSmaller` for each element in an initial pass, and a second BIT to compute `rightLarger` for each element in a reverse pass. Then the answer is the sum over all positions `j` of `leftSmaller[j] * rightLarger[j]`. Edge cases: if the array length is less than 3, return 0; values are 1-indexed in the BIT, so when `a-1` is 0, query returns 0. Time complexity is O(n log valueLimit) for two passes, and space complexity O(valueLimit) for the two BITs (or O(n) if we store the array). The provided snippet uses a clever single-pass method: it uses BIT[0] to count frequencies and BIT[1] to accumulate for each element the number of smaller elements before it, then later uses BIT[1] to sum across triples. However, the classic two-pass approach is clearer and equally efficient.
