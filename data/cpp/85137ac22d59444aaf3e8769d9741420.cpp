// Write a C++ function `vector<pair<int,int>> computeIntervals(const vector<int>& v, const vector<pair<int,int>>& ranges)` that, given a sorted vector `v` of distinct integers and a vector `ranges` of the same length where `ranges[i] = {a_i, b_i}`, returns a vector of pairs `{L_i, R_i}` such that `L_i` is the number of elements in `v` strictly less than `v[i] - a_i`, and `R_i` is the number of elements in `v` less than or equal to `v[i] + b_i`. For each `i`, these counts are 0-based indices in the sorted vector, interpreted as `[L_i, R_i)` interval bounds (with `R_i` exclusive). Assume `v` is sorted in non-decreasing order, and that all indices are valid (i.e., no out-of-bounds). The function must use binary search for efficiency. Do not sort or modify the input vectors. The returned vector should have the same order as the input ranges.

// The problem requires computing, for each element `v[i]`, the index range of elements in `v` that fall within the inclusive-exclusive bound `[v[i] - a_i, v[i] + b_i]`, but expressed as indices such that `L_i` = count of elements < `v[i] - a_i` and `R_i` = count of elements ≤ `v[i] + b_i`. Since `v` is sorted, we can use `lower_bound` to find the first position where an element is ≥ `(v[i] - a_i)`, which gives exactly the count of elements strictly less than that value. For `R_i`, we need the count of elements less than or equal to `v[i] + b_i`; that is the position returned by `upper_bound` minus 1? Actually, `upper_bound` returns the first position where an element is > `v[i] + b_i`, so the number of elements ≤ `v[i] + b_i` is exactly `upper_bound(v.begin(), v.end(), v[i]+b_i) - v.begin()`. Thus `L_i = lower_bound(v.begin(), v.end(), v[i]-a_i) - v.begin()` and `R_i = upper_bound(v.begin(), v.end(), v[i]+b_i) - v.begin()`. Note that `R_i` is exclusive end, so the interval is `[L_i, R_i)`. Edge cases: if `v[i]-a_i` is smaller than the smallest element, `L_i=0`; if `v[i]+b_i` is larger than the largest, `R_i=n`. Since `v` is sorted, binary search works fine. Complexity: for each `i`, two `O(log n)` binary searches, so total `O(n log n)` time, `O(n)` space for output.

#include <vector>
#include <algorithm>
#include <utility>

// Given a sorted vector v and a vector of {a,b} pairs,
// return for each i the interval [L_i, R_i) of indices where
// L_i = count of elements < v[i]-a_i, R_i = count of elements <= v[i]+b_i.
std::vector<std::pair<int, int>> computeIntervals(
    const std::vector<int>& v,
    const std::vector<std::pair<int, int>>& ranges) {
    
    int n = static_cast<int>(v.size());
    std::vector<std::pair<int, int>> result;
    result.reserve(n);
    
    for (int i = 0; i < n; ++i) {
        int a = ranges[i].first;
        int b = ranges[i].second;
        
        // L: first index where v[index] >= v[i]-a
        int L = static_cast<int>(
            std::lower_bound(v.begin(), v.end(), v[i] - a) - v.begin());
        
        // R: first index where v[index] > v[i]+b
        int R = static_cast<int>(
            std::upper_bound(v.begin(), v.end(), v[i] + b) - v.begin());
        
        result.emplace_back(L, R);
    }
    
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

// the function is already included above, but for completeness we assume it's declared.
// Test cases
int main() {
    std::vector<int> v = {1, 3, 5, 7, 9};
    std::vector<std::pair<int, int>> ranges = {{1, 2}, {0, 1}, {2, 0}, {3, 1}, {4, 5}};
    
    auto res = computeIntervals(v, ranges);
    
    // For v[0]=1, a=1,b=2 => lower bound of 0 => index 0, upper bound of 3 => index 1 (since 3>3? Actually v[1]=3 is equal to 1+2=3, so <= includes it, so upper_bound(3) returns index 2? Wait: v={1,3,5,7,9}, upper_bound(3) returns iterator to 5 at index 2. So R=2, interval [0,2) includes indices 0,1. That is correct.
    assert(res[0] == std::make_pair(0, 2));
    
    // v[1]=3, a=0,b=1 => lower bound of 3 => index 1, upper bound of 4 => index 2 (since 5>4). Interval [1,2) includes index 1 only.
    assert(res[1] == std::make_pair(1, 2));
    
    // v[2]=5, a=2,b=0 => lower bound of 3 => index 1, upper bound of 5 => index 3 (since 7>5). Interval [1,3) includes indices 1,2.
    assert(res[2] == std::make_pair(1, 3));
    
    // v[3]=7, a=3,b=1 => lower bound of 4 => index 2, upper bound of 8 => index 4 (since 9>8). Interval [2,4) includes indices 2,3.
    assert(res[3] == std::make_pair(2, 4));
    
    // v[4]=9, a=4,b=5 => lower bound of 5 => index 2, upper bound of 14 => index 5 (all). Interval [2,5) includes indices 2,3,4.
    assert(res[4] == std::make_pair(2, 5));
    
    // Edge case: single element
    v = {10};
    ranges = {{5, 5}};
    res = computeIntervals(v, ranges);
    assert(res[0] == std::make_pair(0, 1));
    
    // Edge case: large range covers all
    v = {2, 4, 6, 8};
    ranges = {{100, 100}, {100, 100}, {100, 100}, {100, 100}};
    res = computeIntervals(v, ranges);
    for (const auto& p : res) {
        assert(p == std::make_pair(0, 4));
    }
    
    // Edge case: negative values and zero range
    v = {-5, 0, 3};
    ranges = {{2, 2}, {1, 1}, {0, 0}};
    res = computeIntervals(v, ranges);
    // v[0]=-5, a=2 -> -7 lower bound => 0, b=2 -> -3 upper_bound => 0? Actually upper_bound(-3) returns index 0 because -5 <= -3? No, -5 is less than -3, so upper_bound(-3) gives first > -3, which is 0? Wait v[-5,0,3], upper_bound(-3) returns iterator to 0 at index 1, because -5 <= -3? Actually -5 is less than -3, so upper_bound(-3) returns first element > -3, which is 0 at index 1. So interval [0,1) includes index 0 only. So assert res[0]==make_pair(0,1).
    assert(res[0] == std::make_pair(0, 1));
    // v[1]=0, a=1 -> -1 lower_bound => index 0 (since -5 < -1, 0>= -1?), lower_bound(-1) returns first element >= -1, which is 0 at index 1? Actually lower_bound(-1) returns first element not less than -1, so -5 < -1, 0 >= -1, so returns index 1. b=1 -> 1 upper_bound => first >1, which is 3 at index 2. So interval [1,2) includes index 1 only.
    assert(res[1] == std::make_pair(1, 2));
    // v[2]=3, a=0 -> 3 lower_bound => index 2, b=0 -> 3 upper_bound => first >3, which is end index 3. interval [2,3) includes index 2 only.
    assert(res[2] == std::make_pair(2, 3));
    
    return 0;
}
