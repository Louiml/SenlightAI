Write a C++ function that simulates a simplified version of the problem from the code snippet: given a vector of non-negative integers representing the number of mushrooms eaten per minute (over `n` minutes), compute two possible answers for a mushroom-eating contest. The first answer (`ans1`) is the total number of mushrooms eaten if the eater always eats at a constant speed equal to the maximum rate at which mushrooms disappear between any two consecutive minutes (i.e., the maximum `a[i-1] - a[i]` if positive, else 0). The second answer (`ans2`) is the total number of mushrooms eaten if the eater eats at a constant speed equal to the same maximum rate, but only from the start until the last minute, where in each minute they eat at that constant rate (or less if fewer mushrooms are available), and then the final minute they eat all remaining mushrooms. Note: The problem ensures the vector has at least 2 elements. Return a pair of integers `{ans1, ans2}`.
#include <cassert>
#include <vector>
#include <utility>

// forward declaration of the function (already defined above)
std::pair<int, int> mushroomEating(const std::vector<int>& a);

int main() {
    // Example from typical problem: 10 5 15 5
    assert(mushroomEating({10, 5, 15, 5}) == std::make_pair(15, 15));
    // Constant increasing sequence: no decrease, zero rate
    assert(mushroomEating({1, 2, 3, 4}) == std::make_pair(0, 0));
    // Constant sequence: no decrease, zero rate
    assert(mushroomEating({5, 5, 5}) == std::make_pair(0, 0));
    // Single decrease: max diff = 3, ans1 = 3, ans2 = min(10,3)+min(7,3)=6
    assert(mushroomEating({10, 7, 7}) == std::make_pair(3, 6));
    // Multiple decreases: 100 80 60 -> diffs 20+20=40, mx=20, ans2 = min(100,20)+min(80,20)=40
    assert(mushroomEating({100, 80, 60}) == std::make_pair(40, 40));
    // Edge with zero values: 0 10 0 -> diffs 10, mx=10, ans2 = min(0,10)+min(10,10)=10
    assert(mushroomEating({0, 10, 0}) == std::make_pair(10, 10));
    // Edge with all zeros: zero rate, both zero
    assert(mushroomEating({0, 0, 0}) == std::make_pair(0, 0));
    // Large drop: 200 100 -> mx=100, ans1=100, ans2 = min(200,100)=100
    assert(mushroomEating({200, 100}) == std::make_pair(100, 100));
    // Alternating: 10 1 10 1 -> diffs 9+9=18, mx=9, ans2 = min(10,9)+min(1,9)+min(10,9)=9+1+9=19
    assert(mushroomEating({10, 1, 10, 1}) == std::make_pair(18, 19));
    // Minimum size two: 3 0 -> mx=3, ans1=3, ans2 = min(3,3)=3
    assert(mushroomEating({3, 0}) == std::make_pair(3, 3));
    return 0;
}
#include <vector>
#include <algorithm>
#include <utility>

// Compute two possible total mushrooms eaten given per-minute amounts.
// Returns a pair: first = ans1 (sum of decreases), second = ans2 (constant-rate eating).
std::pair<int, int> mushroomEating(const std::vector<int>& a) {
    int n = static_cast<int>(a.size());
    int mx = 0;
    // Find maximum decrease between consecutive minutes.
    for (int i = n - 1; i > 0; --i) {
        mx = std::max(mx, a[i - 1] - a[i]);
    }
    
    int ans1 = 0;
    int ans2 = 0;
    
    // ans1: sum of positive differences (eating when mushrooms decrease).
    for (int i = 0; i < n - 1; ++i) {
        ans1 += std::max(0, a[i] - a[i + 1]);
    }
    
    // ans2: simulate eating at constant rate mx per minute, capped by available.
    for (int i = 0; i < n - 1; ++i) {
        ans2 += std::min(a[i], mx);
    }
    
    return {ans1, ans2};
}
// The solution first computes the maximum `mx` of `a[i-1] - a[i]` for all `i` from 1 to n-1 (or 0 if all are negative, but since the values are non-negative, the subtraction can be negative, so we take the maximum, which might be positive or zero). `ans1` is simply the sum of all positive differences `a[i] - a[i+1]` (where `a[i] > a[i+1]`) over consecutive pairs; this represents the total mushrooms eaten when eating only when mushrooms decrease. `ans2` is computed by simulating each minute from 0 to n-2: for each minute, if the current mushrooms `a[i]` are less than or equal to `mx`, the eater eats all `a[i]` mushrooms; otherwise, eats exactly `mx` mushrooms. The final minute (index n-1) is ignored because after the last minute no more mushrooms are eaten? Actually, in the original problem, `ans2` sums over `i = 0` to `n-2` (the last minute isn't counted because the eater stops before the last minute? Wait, the code sums for i=0 to n-2, so it counts the eating in the first n-1 minutes, not including the last minute. This is typical of the "mushroom" problem where you only eat between minutes, not after the last. So `ans2` is the total eaten if the eater eats at rate `mx` per minute for the first n-1 minutes, but capped by available mushrooms in each minute. Edge cases: if `mx` is 0 (no decreases), then `ans2` is 0 because the eater eats nothing (since `a[i] <= 0` is false unless zero, but mushrooms are non-negative, so if `mx=0`, the condition `a[i] <= 0` holds only if `a[i]==0`, so they eat 0 in each minute; so `ans2=0`). The algorithm runs in O(n) time and O(1) extra space.
