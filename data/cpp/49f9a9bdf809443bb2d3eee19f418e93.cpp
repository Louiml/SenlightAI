You are given an array of `n` positive integers and a sequence of `m` update operations. Initially, you must build a data structure that tracks all indices `i` where the value at index `i` is strictly smaller than every value at indices `0..i-1` (i.e., a "prefix minimum candidate"). After each update, which decreases the value at a given index `k` by a positive amount `d`, you must output the number of remaining prefix minimum candidates after that update. A prefix minimum candidate is defined as an index `j` such that for all `0 <= i < j`, `a[i] > a[j]` (strictly). When a value at index `k` is decreased, it may become a new candidate, and some existing candidates with index `>= k` may cease to be candidates if their value is no longer smaller than all earlier values (specifically, if the decreased value at `k` becomes <= their value). Note that updates can only decrease values, never increase them. You need to process all updates in order and print the candidate count after each update.

We maintain a set of pairs `(index, value)` that stores exactly the current prefix minimum candidates. Initially, we scan the array from left to right, and whenever we see a value strictly smaller than the smallest value seen so far, we insert `(i, a[i])` into the set and update the running minimum.

For each update `(k, d)`: decrease `a[k]` by `d`. Now we need to see if this new value at index `k` changes the candidate set. First, check the candidate with the largest index that is `< k` (since the set is ordered by index). If the value of that previous candidate is greater than the new `a[k]`, then the new value at `k` is a new prefix minimum candidate. If so, we must remove all existing candidates with index `>= k` whose value is `>=` the new value at `k` (because those are no longer strictly smaller than the new value at `k`). We do this by repeatedly finding the smallest-index candidate `>= k`, checking its value, and if that value is `>= a[k]`, erase it; stop when we encounter a candidate with value `< a[k]` (since the set is sorted by index, but not by value, we must check each candidate in order from `k` upward until we either clear all or hit one that is strictly smaller). Then we insert the new candidate `(k, a[k])`.

If the new value is not smaller than the previous candidate's value, then no change occurs (except the value itself might be smaller than some later candidates, but since it’s not a new prefix minimum, it cannot invalidate later candidates because those later candidates were already smaller than all earlier values, including this one if this one is not smaller than the immediate previous candidate).

We output `st.size()` after each update.

Edge cases: The initial array may have equal values; only strictly smaller counts. Updates may make a value negative? The problem states positive integers, but decrease may still keep positive if d is small; but we do not need to worry about sign for correctness. The set uses `pl` = `pair<ll,ll>` with first=index, second=value, and we use `st.upper_bound({k, M})` to find the candidate with largest index less than `k` (since M is very large). Use `st.lower_bound({k, -1})` to find the first candidate with index `>= k`. Complexity: Each index is inserted at most once and erased at most once? Actually an index can be re-inserted after being erased? Because after erasing, a later update might decrease that same index again and make it a candidate again. However, each update might re-insert a previously erased index, so worst-case each update could cause O(n) erasures, but amortized analysis is tricky. In practice, the number of erasures per update is limited by the number of candidates that become invalid; since values only decrease, once a value is erased because a new smaller value appears to its left, any later update cannot make that erased candidate valid again unless it decreases further, but the new smaller value to its left will still be smaller unless that left value is also decreased? Actually left value can be decreased further, so it stays smaller, so the erased index will never become a candidate again because there is a smaller value to its left forever. Thus each index is inserted at most once total and erased at most once total. So total work is O(n + m log n) time. Space is O(n).

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Process updates on an array and return the count of prefix minimum candidates after each update.
vector<ll> processPrefixMinimumCandidates(const vector<ll>& initial, const vector<pair<ll, ll>>& updates) {
    ll n = initial.size();
    vector<ll> arr = initial;
    set<pair<ll, ll>> candidates; // (index, value)

    // Build initial set of prefix minimum candidates
    ll mn = LLONG_MAX;
    for (ll i = 0; i < n; ++i) {
        if (arr[i] < mn) {
            candidates.insert({i, arr[i]});
            mn = arr[i];
        }
    }

    vector<ll> result;
    for (const auto& upd : updates) {
        ll k = upd.first;   // 0-based index
        ll d = upd.second;
        arr[k] -= d;

        // Find the candidate with largest index < k
        auto itPrev = candidates.upper_bound({k, LLONG_MAX});
        bool isNewCandidate = false;
        if (itPrev == candidates.begin()) {
            // No previous candidate, so index 0 is always a candidate
            if (k == 0) {
                isNewCandidate = true;
            } else {
                // If k > 0 and no previous candidate, impossible because index 0 is always a candidate
                // So this branch never happens, but just in case:
                isNewCandidate = true;
            }
        } else {
            --itPrev;
            if (itPrev->second > arr[k]) {
                isNewCandidate = true;
            }
        }

        if (isNewCandidate) {
            // Remove all candidates with index >= k whose value >= arr[k]
            auto it = candidates.lower_bound({k, -1});
            while (it != candidates.end()) {
                if (it->second >= arr[k]) {
                    it = candidates.erase(it);
                } else {
                    break;
                }
            }
            candidates.insert({k, arr[k]});
        }

        result.push_back(static_cast<ll>(candidates.size()));
    }
    return result;
}

#include <bits/stdc++.h>
using namespace std;
using ll = long long;

// Solution function declared here (or include the above solution code)

int main() {
    // Test 1: Simple case
    vector<ll> arr1 = {5, 4, 3, 2, 1};
    vector<pair<ll, ll>> upd1 = {{2, 2}, {0, 1}, {4, 1}};
    vector<ll> res1 = processPrefixMinimumCandidates(arr1, upd1);
    vector<ll> expected1 = {2, 2, 2}; // Let's compute manually? Actually just check consistency with logic
    // After init: candidates: (0,5)? No, 5 is not prefix min (nothing before). Actually index0 always candidate. index1 value4 <5 yes. index2 3<4 yes. index3 2<3 yes. index4 1<2 yes. So size=5.
    // Update1: k=2, d=2 -> arr[2]=1. Previous candidate before index2 is (1,4). 4>1 true. So erase candidates with index>=2 value>=1: (2,3) value>=1 erase, (3,2) erase, (4,1) erase. Then insert (2,1). Now set: (0,5),(1,4),(2,1) size=3. So res1[0]=3.
    // Update2: k=0,d=1 -> arr[0]=4. Previous candidate before 0? none (begin). Since k=0, it's new candidate? Actually if k=0, we should always consider it as candidate because no prior index. But our logic: itPrev==begin, k==0 => isNewCandidate true. Then erase candidates with index>=0 value>=4: (0,5) erase, (1,4) erase? 4>=4 yes erase, (2,1) value 1<4 stop. insert (0,4). Now set: (0,4),(2,1) size=2. So res1[1]=2.
    // Update3: k=4,d=1 -> arr[4]=0. Previous candidate before 4 is (2,1) (since candidates indices 0 and 2). 1>0 true. Erase candidates index>=4 value>=0: none (candidates are 0 and 2). insert (4,0). Now set: (0,4),(2,1),(4,0) size=3. So res1[2]=3.
    assert(res1 == vector<ll>({3,2,3}));

    // Test 2: No changes due to updates not qualifying
    vector<ll> arr2 = {10, 20, 30};
    vector<pair<ll, ll>> upd2 = {{1, 5}, {2, 10}};
    // Init candidates: (0,10) only (since 20<10? no, 30<10? no) size=1
    // Upd1: k=1,d=5 -> arr[1]=15. Prev candidate before 1 is (0,10). 10>15? false. No change. size=1
    // Upd2: k=2,d=10 -> arr[2]=20. Prev candidate before 2 is (0,10). 10>20? false. size=1
    assert(processPrefixMinimumCandidates(arr2, upd2) == vector<ll>({1,1}));

    // Test 3: Equal values strictly smaller only
    vector<ll> arr3 = {3, 3, 2, 2};
    // Init: index0=3 candidate; index1=3 not smaller; index2=2<3 candidate; index3=2 not smaller. size=2
    vector<pair<ll, ll>> upd3 = {{3, 1}, {1, 2}};
    // Upd1: k=3,d=1 -> arr[3]=1. Prev candidate before 3 is (2,2). 2>1 true. Erase index>=3 value>=1: (3,2) erase. insert (3,1). Now set: (0,3),(2,2),(3,1) size=3
    // Upd2: k=1,d=2 -> arr[1]=1. Prev candidate before 1 is (0,3). 3>1 true. Erase index>=1 value>=1: (2,2) erase, (3,1) value>=1? 1>=1 true erase. insert (1,1). Now set: (0,3),(1,1) size=2
    assert(processPrefixMinimumCandidates(arr3, upd3) == vector<ll>({3,2}));

    // Test 4: Large decrease making earlier index candidate
    vector<ll> arr4 = {5, 6, 7};
    vector<pair<ll, ll>> upd4 = {{0, 10}}; // arr[0] becomes -5
    // Init: (0,5) only
    // Upd: k=0, d=10 -> arr[0] = -5. Since k=0, always new candidate. Erase index>=0 value>=-5: (0,5) erase. insert (0,-5). size=1
    assert(processPrefixMinimumCandidates(arr4, upd4) == vector<ll>({1}));

    // Test 5: Many updates, no new candidates
    vector<ll> arr5 = {100, 90, 80};
    vector<pair<ll, ll>> upd5 = {{2, 1}, {1, 1}, {0, 1}, {2, 1}};
    // Init: (0,100),(1,90),(2,80) size=3
    // Upd1: k=2,d=1 -> arr[2]=79. Prev candidate before 2 is (1,90). 90>79 true. Erase index>=2 value>=79: (2,80) erase. insert (2,79). set (0,100),(1,90),(2,79) size=3
    // Upd2: k=1,d=1 -> arr[1]=89. Prev candidate before 1 is (0,100). 100>89 true. Erase index>=1 value>=89: (1,90) erase, (2,79) value 79<89 stop. insert (1,89). set (0,100),(1,89),(2,79) size=3
    // Upd3: k=0,d=1 -> arr[0]=99. k=0 new candidate. Erase index>=0 value>=99: (0,100) erase, (1,89) value<99 stop. insert (0,99). set (0,99),(1,89),(2,79) size=3
    // Upd4: k=2,d=1 -> arr[2]=78. Prev candidate before 2 is (1,89). 89>78 true. Erase index>=2 value>=78: (2,79) erase. insert (2,78). set (0,99),(1,89),(2,78) size=3
    assert(processPrefixMinimumCandidates(arr5, upd5) == vector<ll>({3,3,3,3}));

    // Test 6: Empty updates
    vector<ll> arr6 = {1, 2, 3};
    vector<pair<ll, ll>> upd6 = {};
    assert(processPrefixMinimumCandidates(arr6, upd6).empty());

    cout << "All tests passed!\n";
    return 0;
}
