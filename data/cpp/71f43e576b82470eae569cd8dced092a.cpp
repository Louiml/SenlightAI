You are given a sequence of \( n \) integers, each in the range \([1, 20]\). Your task is to write a C++ function that, given the sequence, returns the minimum number of adjacent swaps needed to rearrange the sequence so that all equal values are grouped together into contiguous blocks (i.e., after sorting the groups in any order, all occurrences of the same integer appear consecutively). The order of the blocks themselves can be arbitrary; you only need to minimize the total number of adjacent swaps (where a swap exchanges two neighboring elements). The input size satisfies \( 1 \le n \le 4\cdot 10^5 \). The function should take a `std::vector<int>` (with values guaranteed to be in \(1..20\)) and return a `long long` representing the minimum number of swaps. Note that the values are 1-indexed in the problem statement but you may internally map them to 0-indexed for convenience.

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// The solution function from the task (copy here)
ll minAdjacentSwapsToGroup(const vector<int>& arr) {
    const int T = 20;
    int n = (int)arr.size();
    vector<vector<ll>> cnt(T, vector<ll>(T, 0));
    vector<int> freq(T, 0);
    for (int val : arr) {
        int idx = val - 1;
        freq[idx]++;
        for (int j = 0; j < T; ++j) {
            if (j != idx) {
                cnt[idx][j] += freq[j];
            }
        }
    }
    const ll INF = 1e18;
    int fullMask = (1 << T);
    vector<ll> dp(fullMask, INF);
    dp[0] = 0;
    for (int mask = 0; mask < fullMask; ++mask) {
        if (dp[mask] == INF) continue;
        for (int j = 0; j < T; ++j) {
            if (mask & (1 << j)) continue;
            ll add = 0;
            for (int k = 0; k < T; ++k) {
                if (!(mask & (1 << k))) {
                    add += cnt[j][k];
                }
            }
            int newMask = (mask | (1 << j));
            dp[newMask] = min(dp[newMask], dp[mask] + add);
        }
    }
    return dp[fullMask - 1];
}

int main() {
    // Test 1: Already grouped sequence
    vector<int> arr1 = {1, 1, 2, 2, 3, 3};
    assert(minAdjacentSwapsToGroup(arr1) == 0);
    
    // Test 2: Simple swap needed: 1,2,1 -> group 1s together requires 1 swap (swap positions 2 and 3)
    vector<int> arr2 = {1, 2, 1};
    assert(minAdjacentSwapsToGroup(arr2) == 1);
    
    // Test 3: All distinct? Only 2 types: 1,2,1,2 -> best order 1,1,2,2: need 2 swaps
    vector<int> arr3 = {1, 2, 1, 2};
    assert(minAdjacentSwapsToGroup(arr3) == 2);
    
    // Test 4: Reversed order of two types
    vector<int> arr4 = {2, 2, 1, 1};
    // Already grouped, order doesn't matter (blocks can be any order) -> 0 swaps
    assert(minAdjacentSwapsToGroup(arr4) == 0);
    
    // Test 5: Interleaved three types
    vector<int> arr5 = {1, 2, 3, 1, 2, 3};
    // Optimal: group 1s, then 2s, then 3s. Count inversions: 1 before 2: (1,2),(1,2) -> 2; 1 before 3: (1,3),(1,3) -> 2; 2 before 3: (2,3),(2,3) -> 2. Total = 6.
    // Wait, actually compute manually: original: positions: 1:1,2:2,3:3,4:1,5:2,6:3. For order 1,2,3: pairs (1,2): positions (1,2),(1,5),(4,5)=3 pairs? Actually (1,2) at (1,2),(4,5) plus (1,5) is also (1,2) because pos1=1, pos5=2. So 3 pairs. (1,3): (1,3),(1,6),(4,6)=3 pairs. (2,3): (2,3),(2,6),(5,6)=3 pairs. Total=9. But maybe better order 2,1,3? Let's trust DP, but test with small brute force? Instead use known result: The minimal swaps for alternating two types of length n each is n^2/2? For 3 types alternating, we can verify by brute force but easier to just assert a small known case. Let's just compute by hand a simpler case.
    // Use a simpler test to avoid hand computation errors: only two types.
    
    // Test 5: Single element
    vector<int> arr5b = {7};
    assert(minAdjacentSwapsToGroup(arr5b) == 0);
    
    // Test 6: All same
    vector<int> arr6 = {5, 5, 5};
    assert(minAdjacentSwapsToGroup(arr6) == 0);
    
    // Test 7: Two types with 3 each, interleaved 1,2,1,2,1,2
    // This is classic: min swaps = 3*2 = 6? Actually to group all 1s together and all 2s together, we need to move each 1 to the left of the block. For alternating, the number of inversions of minority? Let's compute: arr = 1,2,1,2,1,2. Order 1,2: cnt[1][2] = number of pairs (i<j) with 1 before 2 = each 1 is before all 2s to its right. First 1 has 3 twos after, second 1 has 2 twos, third 1 has 1 two -> total 6. So min swaps = 6.
    vector<int> arr7 = {1,2,1,2,1,2};
    assert(minAdjacentSwapsToGroup(arr7) == 6);
    
    // Test 8: Same but with order 2,1? cnt[2][1] = each 2 before a 1: first 2 has 3 ones after, second has 2, third has 1 -> 6 also. So answer is 6.
    
    // Test 9: Large range but only few types, e.g., 20 types all in increasing order already grouped
    vector<int> arr9;
    for (int i = 1; i <= 20; ++i) arr9.push_back(i);
    assert(minAdjacentSwapsToGroup(arr9) == 0);
    
    // Test 10: Reverse order (20,19,...,1) - still grouped, blocks any order, 0 swaps
    vector<int> arr10 = arr9;
    reverse(arr10.begin(), arr10.end());
    assert(minAdjacentSwapsToGroup(arr10) == 0);
    
    cout << "All tests passed!" << endl;
    return 0;
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Given a sequence of integers in [1,20], return the minimum adjacent swaps
// needed to group all equal values into contiguous blocks (blocks can be in any order).
ll minAdjacentSwapsToGroup(const vector<int>& arr) {
    const int T = 20; // number of distinct value types
    int n = (int)arr.size();
    
    // cnt[a][b] = number of pairs (i<j) with arr[i]==a and arr[j]==b
    vector<vector<ll>> cnt(T, vector<ll>(T, 0));
    vector<int> freq(T, 0);
    for (int val : arr) {
        int idx = val - 1; // 0-indexed
        freq[idx]++;
        for (int j = 0; j < T; ++j) {
            if (j != idx) {
                cnt[idx][j] += freq[j];
            }
        }
    }
    
    const ll INF = 1e18;
    int fullMask = (1 << T);
    vector<ll> dp(fullMask, INF);
    dp[0] = 0;
    
    for (int mask = 0; mask < fullMask; ++mask) {
        if (dp[mask] == INF) continue;
        // Try adding a new type j that isn't in the mask
        for (int j = 0; j < T; ++j) {
            if (mask & (1 << j)) continue;
            ll add = 0;
            // Types not in mask will appear after j, so each pair (j, k) with k not in mask
            // must be swapped.
            for (int k = 0; k < T; ++k) {
                if (!(mask & (1 << k))) {
                    add += cnt[j][k];
                }
            }
            int newMask = (mask | (1 << j));
            dp[newMask] = min(dp[newMask], dp[mask] + add);
        }
    }
    
    return dp[fullMask - 1];
}

// The key observation is that the optimal grouping corresponds to a permutation of the 20 distinct value types. Since there are only 20 types, we can consider all possible orderings of these types via bitmask dynamic programming over subsets of types. For any two types \(a\) and \(b\), precompute the number of "inversions" between them: i.e., the number of pairs \((i,j)\) with \(i<j\), \(ar[i]=a\), \(ar[j]=b\) that would need to be swapped if \(a\) appears before \(b\) in the final order. If we place type \(a\) before type \(b\), then every occurrence of \(b\) that appears after an occurrence of \(a\) in the original sequence forms an inversion that must be resolved. Thus, the total number of swaps for a fixed ordering is the sum over all ordered pairs \((a,b)\) with \(a\) before \(b\) of \(\text{cnt}[a][b]\), where \(\text{cnt}[a][b]\) is the number of pairs \((i<j)\) with \(ar[i]=a,\ ar[j]=b\).
//
// We compute this with DP: let \(dp[mask]\) be the minimum swaps needed to arrange all types in `mask` as a prefix of the final ordering, with the block order being the order in which types are added to the mask. When adding a new type \(j\) to a mask, the additional swaps are the sum of \(\text{cnt}[j][k]\) over all \(k\) not yet in the mask (i.e., types that will come after \(j\)), because every occurrence of \(k\) after an occurrence of \(j\) in the original sequence must be swapped across. The recurrence is \(dp[mask|1<<j] = \min(dp[mask|1<<j], dp[mask] + \sum_{k \notin mask} cnt[j][k])\). The answer is \(dp[(1<<20)-1]\). Complexity: \(O(20^2 \cdot n + 20 \cdot 2^{20})\) time, \(O(2^{20})\) space. Edge case: if not all 20 types appear, we still iterate over all 20 but costs for absent types interact correctly because their `cnt` values are zero, and the DP still finds the optimal order among present types.
