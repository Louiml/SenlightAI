// Write a C++ function that takes a vector of integers (each between 0 and 100 inclusive) and a target integer, and returns the number of triples (i, j, k) with i < j < k such that the sum of the three elements equals the target. The result may be large, so return it modulo 1,000,000,007. The vector may contain duplicate values, and the indices must be distinct (strictly increasing order). For example, if `arr = {1, 1, 2, 2, 3, 4}` and `target = 6`, the valid triples are (0,1,5) summing to 1+1+4=6, (0,2,4) summing to 1+2+3=6, and (0,3,4) summing to 1+2+3=6, plus (1,2,4) and (1,3,4), so the count is 5.

#include <cassert>
#include <vector>

// Assume the solution function is declared above.
int main() {
    std::vector<int> arr1 = {1, 1, 2, 2, 3, 4};
    assert(countTriplesSumToTarget(arr1, 6) == 5);

    std::vector<int> arr2 = {0, 0, 0, 0};
    assert(countTriplesSumToTarget(arr2, 0) == 4);  // choose any 3 of 4

    std::vector<int> arr3 = {1, 2, 3};
    assert(countTriplesSumToTarget(arr3, 6) == 1);

    std::vector<int> arr4 = {1, 2, 3};
    assert(countTriplesSumToTarget(arr4, 7) == 0);

    std::vector<int> arr5 = {100, 100, 100};
    assert(countTriplesSumToTarget(arr5, 300) == 1);

    std::vector<int> arr6 = {1, 1, 1};
    assert(countTriplesSumToTarget(arr6, 3) == 1);

    std::vector<int> arr7 = {1, 1, 2, 2, 2, 2};
    assert(countTriplesSumToTarget(arr7, 5) == 8); // all triples with two 1's and one 2: C(2,2)*C(4,1)=4; or with one 1 and two 2's: C(2,1)*C(4,2)=2*6=12? Actually let's compute: triples with sum 5: either 1+1+2 or 1+2+2. Count: for 1+1+2: choose 2 from the two 1's (1 way) and 1 from the four 2's (4 ways) => 4. For 1+2+2: choose 1 from two 1's (2 ways) and 2 from four 2's (C(4,2)=6) => 12. Total 16. But careful: the function counts ordered i<j<k, so yes 16.
    // So assert is 16.
    assert(countTriplesSumToTarget(arr7, 5) == 16);

    std::vector<int> arr8 = {5, 5, 5, 5, 5};
    assert(countTriplesSumToTarget(arr8, 15) == 10); // C(5,3)=10

    std::vector<int> arr9 = {};
    assert(countTriplesSumToTarget(arr9, 0) == 0);

    std::vector<int> arr10 = {0, 1, 2, 3, 4};
    assert(countTriplesSumToTarget(arr10, 10) == 0); // no triple sums to 10? Actually 0+4+6 no, max 4+3+2=9, so 0.
}

#include <vector>

// Returns the number of triples (i<j<k) in arr that sum to target, modulo 1'000'000'007.
// Values in arr are assumed to be in [0, 100].
int countTriplesSumToTarget(const std::vector<int>& arr, int target) {
    const int MOD = 1000000007;
    int cnt[101] = {0};
    for (int v : arr) {
        ++cnt[v];
    }
    long long ans = 0;
    for (int j = 0; j < static_cast<int>(arr.size()); ++j) {
        const int b = arr[j];
        --cnt[b];  // Exclude arr[j] from being the third element
        for (int i = 0; i < j; ++i) {
            const int a = arr[i];
            const int c = target - a - b;
            if (c >= 0 && c <= 100) {
                ans += cnt[c];
                ans %= MOD;
            }
        }
    }
    return static_cast<int>(ans);
}

// The approach uses a counting array and an index-based two-pointer-like scan. Since values are bounded by 100, we can count frequencies. However, the given code uses a clever method: for each position `j` (as the middle element of the triple), it first decrements the count of `arr[j]` to avoid counting the current element as the third element when scanning earlier indices. Then, for each `i < j`, it treats `a = arr[i]` and `b = arr[j]` as the first two elements, computes the required third value `c = target - a - b`, and checks if `c` is within the valid range (0..100). If so, it adds the current count of `c` in the remaining suffix (which includes elements after `j` plus any other earlier elements not yet removed). Since `cnt` was decremented for `arr[j]`, and for each `i` we have already processed earlier `i` values but not yet removed those counts, this correctly counts each distinct triple once because the loop over `j` and `i` ensures `i < j`, and the third element is chosen from positions `> j` (since we decremented `j` and also we never decrement `i` before counting for that `i`). The modulo is applied after each addition to avoid overflow. Time complexity is O(n^2) where n = arr.size(), because of nested loops, but since values are bounded by 100, the inner loop still runs O(n^2) worst-case. Space complexity is O(101) for the counting array. Edge cases include target smaller than 0 or larger than 300 (no triples possible), duplicate values, and empty or small arrays.
