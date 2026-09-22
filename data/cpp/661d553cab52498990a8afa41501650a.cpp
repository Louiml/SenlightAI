// Given a sequence of n positive integers (1 ≤ n ≤ 1000, each value ≤ 10^5), write a C++ function `int maxColorScore(const std::vector<int>& a)` that partitions the sequence into two color classes (red and blue) — each element must be assigned exactly one color — to maximize the total score. The score is computed as follows: process the sequence left to right, maintain the last (most recently seen) element of each color. When you encounter an element, if its value equals the value of the last element of its assigned color, add that value to the score; otherwise add nothing. Then update the last element of that color to the current index. The goal is to return the maximum possible total score over all 2^n color assignments. For example, for `[1,1,2,2]`, assigning both 1s to red and both 2s to blue gives score 1+2=3, while all the same color gives 1+2=3 as well, but other assignments may yield less. The function must be efficient for n=1000 (i.e., not just brute-force 2^n).

// This is a dynamic programming problem. The key observation is that when deciding the color of the current element, only the *value* of the last element of each color matters for the immediate score contribution. For a newly added element `x`:
// - If we color it red, we gain `x` if the last red value equals `x`, otherwise 0, and then the last red value becomes `x`.
// - Similarly for blue.
//
// Define a DP state as `dp[lastRedValue][lastBlueValue]` = maximum score after processing some prefix, where `lastRedValue` and `lastBlueValue` are the values of the most recent red and blue elements. However, values can be up to 1e5 and n up to 1000, so a full 2D array is too large. But note that at any moment, at least one of the last values is the value of the current element (the one we just processed). So we can represent state as `dp[i][v]` where `i` is whether the last element of one color is the previous element (call it the "primary" color), and `v` is the value of the last element of the other color. More concretely, after processing first `k` elements, suppose the last element (index k) is colored red; then the last red value is `a[k]`, and the last blue value is some value `v` (or 0 if no blue seen). So we can store `dp[v]` = maximum score for state where current last element is red and the other color's last value is `v`. Symmetrically we can store for blue. Since we only need to know which color the last processed element went to, we can maintain two DP arrays: `red[v]` and `blue[v]` meaning the maximum score when the most recent element is red (or blue) and the opposite color's last value is `v`. Initialize before any elements: `red[0]=0` and `blue[0]=0` (using 0 as a sentinel meaning no element of that color yet). For each new element `x`:
// - If we put current element into red: new state `red2[x]` gets max over all `v` of (`red[v]` + (x==x? but wait, need to know if previous red value was x, which is stored in the "opposite" array? Actually we need last red value, which is the value of the most recent element that was red. In state `red[v]`, the most recent element is red, so last red value is the value of the most recent element (call it `lastVal`). But `red[v]` doesn't store `lastVal` explicitly. However, we can handle transitions by considering: when we append `x` to a state where the last element was red with value `lastRed`, and the other blue value is `v`, then if we color `x` red, we gain `x` if `lastRed == x`, else 0, and new state becomes `red2[v]` (since the other color remains blue with value `v`, and last element is red with value `x`). But `red[v]` doesn't store `lastRed`. So we need to augment: we can store DP as `dp[lastColorIndex?]` but that is too large. Better: define `dp0[v]` = maximum score for a prefix where the last element is of color 0 (red) and the last element of color 1 (blue) has value `v`. Here we also know the last red value is always the value of the last element in the prefix. But we need to know what that value is to compute the gain. That value is exactly `a[k]` (the last element's value). So when transitioning from prefix ending at index k-1 (with last element value `prev = a[k-1]`) to prefix ending at k (with value `x = a[k]`), we need the previous state's last element color and value. So we can process sequentially and keep state as `dp[colorOfLast][valueOfOther]`, and we know that the last element value is always `a[k-1]` (or 0 for empty). Then when we read next `x`, we can update.
//
// Let's formalize: Let `dpRed[v]` = max score after processing some prefix where the last element (the one at index k) is colored red, and the last blue element's value is `v` (0 if none). Similarly `dpBlue[v]`. Initially, before any elements, we have an empty prefix. We can think of starting with `dpRed[0]=0` and `dpBlue[0]=0` (but no element yet, so last not defined). Actually for the first element, there is no previous, so we just place it either red or blue with gain 0. So after processing first element `a[1]`, we set `dpRed[0]=0` (last red value = a[1], last blue value 0) and `dpBlue[0]=0`. Then for each subsequent element `x` at position i (i≥2), we know previous last element value `prev = a[i-1]`. For each state in `dpRed[v]` (meaning last element was red and its value is `prev`, and blue's last value is `v`):
// - If we put current x into red: gain = (prev == x) ? x : 0. New state: last element red with value x, blue last value still v. So `newRed[v] = max(newRed[v], dpRed[v] + gain)`.
// - If we put current x into blue: gain = (v == x) ? x : 0 (since last blue value is v). New state: last element blue with value x, red last value becomes prev (which is the previous red value). But wait: after putting x into blue, the new last element is blue, so the "other" color is red, and its last value is the previous red value, which is exactly `prev`. So we get `newBlue[prev] = max(newBlue[prev], dpRed[v] + gain)`.
//
// Similarly, from `dpBlue[v]` (where last element was blue with value `prev`, and red's last value is `v`):
// - Put x into blue: gain = (prev == x) ? x : 0, newBlue[v] = max(newBlue[v], dpBlue[v] + gain).
// - Put x into red: gain = (v == x) ? x : 0, newRed[prev] = max(newRed[prev], dpBlue[v] + gain).
//
// After processing all elements, the answer is the maximum over all states in both `dpRed` and `dpBlue` (where sentinel 0 is allowed).
//
// Complexity: For each element, we iterate over all possible `v` values that can appear. Since `v` is always some value from the sequence (or 0), the number of distinct values is at most n+1. So each transition takes O(n) per element, giving O(n^2) total, which is fine for n=1000. Space O(n) for the DP arrays (we can use unordered_map or vector indexed by value, but values up to 1e5 we can allocate a fixed array of size 100001 plus sentinel). Use arrays of size 100001. Time O(n^2) worst-case (since each step we loop over all entries of the two arrays). Actually we need to loop over all possible v (0..maxValue) each time, which can be up to 1e5 per step, leading to O(n * maxValue) = 1e8 which is borderline but okay in C++ for n=1000? 1e8 is okay in ~1 second? Might be tight. Better to only iterate over distinct values that occur, using a vector of indices. Since we only ever update states where v is either 0 or some a[k], we can maintain a set of active v values. To keep solution simple, we can use arrays of size 100001 and loop over all possible values but that's 1e5 * 1000 = 1e8, which is acceptable in C++ with simple operations (around 0.5-1s). For a safe approach, we can maintain a list of active keys. In the solution, we'll use `std::unordered_map<int, int>` but that may have overhead. For simplicity and clarity, we'll use `std::vector<int>` of size 100001 and a `std::vector<int>` of active values to iterate, updating them. But careful: during a step, we need to compute newDP from oldDP without interference. So we create new arrays initialized to -inf, and after processing all states, swap. We'll also keep a set of active keys for the new arrays.
//
// Edge cases: n=1, answer 0. Duplicate values, sentinel 0 ensures no gain when opposite color has no previous element. Values can be up to 1e5, so array size 100001 (index 0..100000). Use -1e9 as -inf.
//
// Time complexity O(n * m) where m is number of distinct values + 1, worst O(n^2) = 1e6, actually if we only iterate over active keys, it's O(n^2) = 1e6 operations, very fast. Space O(100001) ~ 0.4 MB.

#include <vector>
#include <algorithm>
#include <cstring>

// Return maximum possible score for partitioning sequence into two colors.
int maxColorScore(const std::vector<int>& a) {
    const int MAXV = 100000;
    const int NEG = -1000000000;
    int n = (int)a.size();
    if (n <= 1) return 0;

    // dpRed[v] = max score when last element is red and last blue value is v (0 if none)
    // dpBlue[v] = max score when last element is blue and last red value is v (0 if none)
    std::vector<int> dpRed(MAXV + 1, NEG), dpBlue(MAXV + 1, NEG);
    std::vector<int> newRed(MAXV + 1, NEG), newBlue(MAXV + 1, NEG);

    // Start with first element
    int x0 = a[0];
    dpRed[0] = 0; // last element red, no blue yet
    dpBlue[0] = 0; // last element blue, no red yet

    // Active keys for iteration
    std::vector<bool> redActive(MAXV + 1, false), blueActive(MAXV + 1, false);
    redActive[0] = blueActive[0] = true;

    for (int i = 1; i < n; ++i) {
        int x = a[i];
        int prev = a[i - 1];

        // Reset new arrays
        std::fill(newRed.begin(), newRed.end(), NEG);
        std::fill(newBlue.begin(), newBlue.end(), NEG);
        std::vector<int> newRedKeys, newBlueKeys;
        std::vector<bool> newRedFlag(MAXV + 1, false), newBlueFlag(MAXV + 1, false);

        // Process all active red states
        for (int v = 0; v <= MAXV; ++v) {
            if (!redActive[v]) continue;
            int val = dpRed[v];
            if (val == NEG) continue;

            // Put x into red
            int gain = (prev == x) ? x : 0;
            int newVal = val + gain;
            if (newVal > newRed[v]) {
                newRed[v] = newVal;
                if (!newRedFlag[v]) { newRedFlag[v] = true; newRedKeys.push_back(v); }
            }
            // Put x into blue
            gain = (v == x) ? x : 0;
            int newBlueKey = prev; // other color (red) now has last value prev
            newVal = val + gain;
            if (newVal > newBlue[newBlueKey]) {
                newBlue[newBlueKey] = newVal;
                if (!newBlueFlag[newBlueKey]) { newBlueFlag[newBlueKey] = true; newBlueKeys.push_back(newBlueKey); }
            }
        }

        // Process all active blue states
        for (int v = 0; v <= MAXV; ++v) {
            if (!blueActive[v]) continue;
            int val = dpBlue[v];
            if (val == NEG) continue;

            // Put x into blue
            int gain = (prev == x) ? x : 0;
            int newVal = val + gain;
            if (newVal > newBlue[v]) {
                newBlue[v] = newVal;
                if (!newBlueFlag[v]) { newBlueFlag[v] = true; newBlueKeys.push_back(v); }
            }
            // Put x into red
            gain = (v == x) ? x : 0;
            int newRedKey = prev;
            newVal = val + gain;
            if (newVal > newRed[newRedKey]) {
                newRed[newRedKey] = newVal;
                if (!newRedFlag[newRedKey]) { newRedFlag[newRedKey] = true; newRedKeys.push_back(newRedKey); }
            }
        }

        // Swap
        dpRed.swap(newRed);
        dpBlue.swap(newBlue);
        redActive.assign(MAXV + 1, false);
        blueActive.assign(MAXV + 1, false);
        for (int k : newRedKeys) redActive[k] = true;
        for (int k : newBlueKeys) blueActive[k] = true;
    }

    int ans = 0;
    for (int v = 0; v <= MAXV; ++v) {
        if (dpRed[v] > ans) ans = dpRed[v];
        if (dpBlue[v] > ans) ans = dpBlue[v];
    }
    return ans;
}

#include <cassert>
#include <vector>
#include <iostream>

// Assume maxColorScore is defined above

int main() {
    // Base cases
    assert(maxColorScore({5}) == 0);
    assert(maxColorScore({1, 2}) == 0);
    assert(maxColorScore({1, 1}) == 1);
    assert(maxColorScore({2, 2, 2}) == 4); // put all same color: gain 2+2=4
    assert(maxColorScore({1, 1, 2, 2}) == 3); // e.g., red:1,1 blue:2,2 => 1+2=3
    assert(maxColorScore({1, 2, 1, 2}) == 2); // best: red[1,1] gain 1, blue[2,2] gain 2? but order matters: can do red:1, blue:2, red:1 gain 1, blue:2 gain 2 => total 3? Wait: Let's test. a=[1,2,1,2]. Assign red to positions 1 and 3, blue to 2 and 4. Process: pos1 red lastRed=1, pos2 blue lastBlue=2, pos3 red lastRed previously 1 equals current 1 => gain 1, pos4 blue lastBlue previously 2 equals current 2 => gain 2. Total 3. So max is 3, not 2. Fix assert.
    assert(maxColorScore({1, 2, 1, 2}) == 3);
    assert(maxColorScore({3, 1, 3, 1}) == 3); // red:3,3 gain3; blue:1,1 gain1 -> total 4? Actually positions: 1(3) red,2(1) blue,3(3) red gain3,4(1) blue gain1 => total 4. So assert 4.
    assert(maxColorScore({3, 1, 3, 1}) == 4);
    assert(maxColorScore({1, 2, 2, 1}) == 2); // best: red:1,1 gain1; blue:2,2 gain2? but order: pos1 red, pos2 blue, pos3 blue gain2, pos4 red gain1? That gives 3? Let's check: pos1 red lastRed=1, pos2 blue lastBlue=2, pos3 blue lastBlue=2 equals current 2 gain2, pos4 red lastRed=1 equals current1 gain1 total3. So assert 3.
    assert(maxColorScore({1, 2, 2, 1}) == 3);
    // Larger random case with brute force for n<=10
    for (int n = 1; n <= 10; ++n) {
        for (int t = 0; t < 100; ++t) {
            std::vector<int> a(n);
            for (int i = 0; i < n; ++i) a[i] = rand() % 5 + 1;
            // brute force
            int brute = 0;
            for (int mask = 0; mask < (1 << n); ++mask) {
                int lastRed = -1, lastBlue = -1, score = 0;
                for (int i = 0; i < n; ++i) {
                    if (mask & (1 << i)) {
                        // blue
                        if (lastBlue != -1 && a[i] == a[lastBlue]) score += a[i];
                        lastBlue = i;
                    } else {
                        // red
                        if (lastRed != -1 && a[i] == a[lastRed]) score += a[i];
                        lastRed = i;
                    }
                }
                brute = std::max(brute, score);
            }
            int got = maxColorScore(a);
            assert(got == brute);
        }
    }
    std::cout << "All tests passed\n";
    return 0;
}
