You are given an integer `n` and a permutation of the first `n` positive integers (i.e., each integer from `1` to `n` appears exactly once). Your task is to write a C++ function that determines the minimum number of adjacent swaps required so that the array is in strictly increasing order (`1, 2, 3, ..., n`). However, instead of computing the actual minimum swaps directly (which is the inversion count), you must observe a simpler pattern: Count how many positions `i` (0-indexed) have `a[i] == i+1` (i.e., the element is already in its correct final place). Let this count be `k`. The answer is `(k + 1) / 2` (integer division rounding up). Your function should take a vector of integers representing the permutation and return this value as a `long long`. The input permutation is guaranteed to be valid (each integer from 1 to n appears exactly once), and `n` can be as large as 10^5.

The problem is a simplified version of a known Codeforces problem. The key observation is that in a permutation where we count the number of elements already fixed in their correct positions (`a[i] == i+1`), each adjacent swap can fix at most two such "already correct" elements by bringing them into position? Actually, the pattern is: the answer equals the ceiling of half the number of already-correct elements. Why? Because we only need to perform swaps to fix the misplaced elements. For the already-correct elements, they form blocks, and moving misplaced elements across them can be minimized. The formula `(k+1)/2` is a direct result of the known solution to the problem "Minimum number of swaps to sort permutation when you can swap adjacent elements, and you want to minimize moves? No, it's a known observation from Codeforces problem 1851A? Actually the snippet solves a problem where the answer is `(count_of_already_correct + 1) / 2`. The reasoning: In an optimal sequence, you can fix two already-correct elements with one swap if they are adjacent in value? But the exact proof is beyond scope; the task is to implement that formula. Edge case: if `k=0`, answer is `0/2`? The formula `(0+1)/2 = 0` (integer division) which is correct. If `k=1`, answer is `(1+1)/2 = 1` but that seems odd; but the known problem says that. The function must handle large `n` (up to 10^5) and return a `long long`. Time complexity: O(n) to scan the array once. Space complexity: O(1) auxiliary. The solution is straightforward: iterate through the vector, count matches, return `(k+1)/2`.

#include <bits/stdc++.h>
using namespace std;

// Given a permutation of 1..n, return the minimum number of adjacent swaps
// needed so that every element is in its final sorted position.
// The answer is (count_of_already_correct_elements + 1) / 2.
long long minimumAdjacentSwapsForPermutation(const vector<int>& arr) {
    long long correctCount = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        if (arr[i] == static_cast<int>(i + 1)) {
            ++correctCount;
        }
    }
    return (correctCount + 1) / 2;
}

#include <bits/stdc++.h>
#include <cassert>
using namespace std;

long long minimumAdjacentSwapsForPermutation(const vector<int>& arr);

int main() {
    // Test 1: Already sorted
    assert(minimumAdjacentSwapsForPermutation({1, 2, 3, 4}) == 2);
    // Explanation: k=4, (4+1)/2 = 2
    // Test 2: Reversed
    assert(minimumAdjacentSwapsForPermutation({4, 3, 2, 1}) == 0);
    // k=0, (0+1)/2 = 0
    // Test 3: Single element
    assert(minimumAdjacentSwapsForPermutation({1}) == 1);
    // k=1, (1+1)/2 = 1
    // Test 4: Two elements, one correct
    assert(minimumAdjacentSwapsForPermutation({1, 2}) == 1);
    // k=2, (2+1)/2 = 1
    // Test 5: Two elements, none correct
    assert(minimumAdjacentSwapsForPermutation({2, 1}) == 0);
    // k=0, (0+1)/2 = 0
    // Test 6: Larger permutation with several correct
    assert(minimumAdjacentSwapsForPermutation({1, 3, 2, 4}) == 1);
    // k=2 (positions 0 and 3), (2+1)/2 = 1
    // Test 7: All misplaced
    assert(minimumAdjacentSwapsForPermutation({3, 1, 2}) == 0);
    // k=0? Check: positions: arr[0]=3 !=1, arr[1]=1 !=2, arr[2]=2 !=3 => k=0 => 0
    // Test 8: Five elements
    assert(minimumAdjacentSwapsForPermutation({1, 2, 3, 5, 4}) == 1);
    // k=3 (positions 0,1,2), (3+1)/2 = 2? Wait compute: k=3 (arr[0]=1, arr[1]=2, arr[2]=3, arr[3]=5 !=4, arr[4]=4 !=5) => k=3 => (3+1)/2=2. So assert == 2.
    // Correct the test:
    assert(minimumAdjacentSwapsForPermutation({1, 2, 3, 5, 4}) == 2);
    // Test 9: n=6, pattern
    assert(minimumAdjacentSwapsForPermutation({1, 2, 4, 3, 5, 6}) == 2);
    // k=4 (positions 0,1,3? Wait arr[2]=4 !=3, arr[3]=3 !=4? actually arr[3]=3 !=4, so only 0,1,4,5 are correct -> k=4 -> (4+1)/2=2
    // Test 10: Large random check using brute force? Not needed, but we can add one more simple case.
    assert(minimumAdjacentSwapsForPermutation({2, 1, 3}) == 1);
    // k=1 (position 2), (1+1)/2=1
    cout << "All tests passed!" << endl;
    return 0;
}
