/*
Given an array of `n` integers where each integer is between `0` and `n` inclusive, write a C++ function that computes the smallest non-negative integer not present in the array (the MEX), subject to a special rule involving the count of values that appear exactly once. Specifically, if there are at least two distinct values that each appear exactly once in the array, the answer is the minimum of (a) the MEX of the array and (b) the second smallest value among those that appear exactly once. If there are zero or one values appearing exactly once, the answer is simply the MEX of the array (where if all numbers `0..n` are present, MEX is `n+1`). The function should return this final integer value. The input includes the length `n` and the array elements.
*/

#include <vector>
#include <algorithm>
#include <cstdint>

// Computes the special answer based on the MEX and single-occurrence rule.
// The input array `arr` has length `n`, and each element is in [0, n].
int specialMexAnswer(const std::vector<int>& arr, int n) {
    std::vector<int> cnt(n + 1, 0);
    for (int x : arr) {
        cnt[x]++;
    }

    std::vector<int> single;
    int mex = n + 1;
    bool foundMex = false;

    for (int i = 0; i <= n; ++i) {
        if (cnt[i] == 1) {
            single.push_back(i);
        }
        if (cnt[i] == 0 && !foundMex) {
            mex = i;
            foundMex = true;
        }
    }

    if (single.size() >= 2) {
        return std::min(mex, single[1]);
    } else {
        return mex;
    }
}

#include <cassert>
#include <vector>

int main() {
    // Test 1: n=5, array = [0,1,2,3,4] -> all 0..4 present, cnt[5]=0, mex=5; singles: none -> answer 5
    assert(specialMexAnswer({0,1,2,3,4}, 5) == 5);

    // Test 2: n=5, array = [0,1,2,3,5] -> cnt: 0,1,2,3 each once, 4 missing, 5 once -> mex=4, singles=[0,1,2,3,5] -> second smallest single=1 -> min(4,1)=1
    assert(specialMexAnswer({0,1,2,3,5}, 5) == 1);

    // Test 3: n=4, array = [0,0,1,2] -> cnt[0]=2, cnt[1]=1, cnt[2]=1, cnt[3]=0, cnt[4]=0 -> first zero at 3 -> mex=3; singles=[1,2] -> second smallest=2 -> min(3,2)=2
    assert(specialMexAnswer({0,0,1,2}, 4) == 2);

    // Test 4: n=3, array = [0,0,0] -> cnt[0]=3, cnt[1]=0 -> mex=1; singles empty -> answer 1
    assert(specialMexAnswer({0,0,0}, 3) == 1);

    // Test 5: n=0, array empty -> cnt[0]=0 -> mex=0; singles empty -> answer 0
    assert(specialMexAnswer({}, 0) == 0);

    // Test 6: n=5, array = [0,1,1,2,3] -> cnt[0]=1, cnt[1]=2, cnt[2]=1, cnt[3]=1, cnt[4]=0, cnt[5]=0 -> mex=4; singles=[0,2,3] -> second smallest=2 -> min(4,2)=2
    assert(specialMexAnswer({0,1,1,2,3}, 5) == 2);

    // Test 7: n=2, array = [0,1] -> cnt[0]=1, cnt[1]=1, cnt[2]=0 -> mex=2; singles=[0,1] -> second smallest=1 -> min(2,1)=1
    assert(specialMexAnswer({0,1}, 2) == 1);

    // Test 8: n=3, array = [0,1,2] -> cnt[0]=1, cnt[1]=1, cnt[2]=1, cnt[3]=0 -> mex=3; singles=[0,1,2] -> second smallest=1 -> min(3,1)=1
    assert(specialMexAnswer({0,1,2}, 3) == 1);

    // Test 9: n=4, array = [4,4,4,4] -> cnt[4]=4, zeros at 0,1,2,3 -> mex=0; singles empty -> answer 0
    assert(specialMexAnswer({4,4,4,4}, 4) == 0);

    // Test 10: n=6, array = [0,1,2,3,4,5] -> cnt[0..5] each once, cnt[6]=0 -> mex=6; singles=[0,1,2,3,4,5] -> second smallest=1 -> min(6,1)=1
    assert(specialMexAnswer({0,1,2,3,4,5}, 6) == 1);

    return 0;
}

// The problem translates directly from the snippet. The first step is to count frequencies of each integer from `0` to `n` in the input array. Then we scan from `0` upward:
// - Collect all indices `i` where `cnt[i] == 1` into a vector `single`.
// - Record the first index where `cnt[i] == 0` as `mex` (if none found, `mex = n+1`).
//
// After the scan:
// - If `single.size() >= 2`, the answer is `min(mex, single[1])` (since `single[0]` is the smallest single-occurrence value, `single[1]` is the second smallest).
// - Otherwise, the answer is `mex`.
//
// Edge cases include: when `n=0` with an empty array (then `cnt[0]==0`, `mex=0`, `single` empty → answer `0`); when all numbers `0..n` appear at least once (then `mex=n+1`, and if at least two singles exist, compare with second smallest single); when there is exactly one single-occurrence value (answer is just MEX). Time complexity is O(n) for counting and scanning, space O(n) for the count array.
