/*
Write a C++ function `int countTripletsWithSumInSet(const std::vector<int>& arr)` that, given a vector of distinct integers, returns the number of ordered pairs `(i, j)` with `i < j` such that the sum of the two elements at indices `i` and `j` is also present anywhere in the array. The input vector will contain only distinct integers and may be empty or contain negative numbers. Your solution must not modify the input array.
*/
#include <vector>
#include <unordered_set>

// Counts pairs (i,j) with i<j such that arr[i]+arr[j] exists in arr.
int countTripletsWithSumInSet(const std::vector<int>& arr) {
    // Use long long for sums to avoid overflow.
    std::unordered_set<long long> values;
    for (const int& x : arr) {
        values.insert(static_cast<long long>(x));
    }
    int count = 0;
    const std::size_t n = arr.size();
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = i + 1; j < n; ++j) {
            long long sum = static_cast<long long>(arr[i]) + static_cast<long long>(arr[j]);
            if (values.find(sum) != values.end()) {
                ++count;
            }
        }
    }
    return count;
}
#include <cassert>
#include <vector>

// Assume the solution function is declared above.
int main() {
    std::vector<int> arr1 = {1, 5, 3, 2};
    assert(countTripletsWithSumInSet(arr1) == 2); // (1+2=3, 2+3=5)

    std::vector<int> arr2 = {1, 2, 3};
    assert(countTripletsWithSumInSet(arr2) == 1); // (1+2=3)

    std::vector<int> arr3 = {1, 2};
    assert(countTripletsWithSumInSet(arr3) == 0); // fewer than 3 elements

    std::vector<int> arr4 = {};
    assert(countTripletsWithSumInSet(arr4) == 0); // empty

    std::vector<int> arr5 = {-1, 0, 1, 2};
    assert(countTripletsWithSumInSet(arr5) == 2); // (-1+1=0, -1+2=1, 0+1=1? but 1 is in set, so (-1+2=1) and (0+1=1), that's 2? actually check: pairs: (-1,0)=-1? no. (-1,1)=0 yes. (-1,2)=1 yes. (0,1)=1 yes. (0,2)=2 yes. (1,2)=3 no. Wait that's 4? Let's recount carefully: i<j: (-1,0): -1 not in set. (-1,1): 0 in set -> count1. (-1,2): 1 in set -> count2. (0,1): 1 in set -> count3. (0,2): 2 in set -> count4. (1,2): 3 not in set. So answer is 4, not 2. Let's fix the assertion.
    assert(countTripletsWithSumInSet(arr5) == 4);

    std::vector<int> arr6 = {10, 20, 30, 40, 50};
    assert(countTripletsWithSumInSet(arr6) == 2); // (10+20=30, 10+30=40, 20+30=50? actually: (10,20)=30 yes, (10,30)=40 yes, (10,40)=50 yes, (20,30)=50 yes, others exceed. So that's 4? Let's check: (10,20)=30, (10,30)=40, (10,40)=50, (20,30)=50, total 4. So assert 4.
    assert(countTripletsWithSumInSet(arr6) == 4);
    return 0;
}
// The key idea is to first insert all array elements into a hash set (e.g., `std::unordered_set`) for O(1) average lookups. Then, iterate over all unordered pairs `(i, j)` with `i < j` using nested loops, compute `sum = arr[i] + arr[j]`, and check if `sum` exists in the set. If it does, increment a counter. Since all elements are distinct, each pair is unique. Edge cases include an empty array (return 0), an array with fewer than three elements (cannot form a triplet, return 0), negative numbers (sums may be negative but are still valid lookups), and large values that could overflow `int`—although the problem assumes typical 32-bit `int`, we can use `long long` for the sum to be safe. Time complexity is O(n^2) due to the nested loops, but with a hash set the lookup per pair is O(1) average. Space complexity is O(n) for the set.
