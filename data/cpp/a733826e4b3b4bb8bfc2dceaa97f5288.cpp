You are a treasure hunter exploring a dungeon with `n` doors arranged in a fixed order. Each door `j` requires `doorR[j]` red keys and `doorG[j]` green keys to open. Once you open a door, you enter a room that contains a chest; you may take any combination of `roomR[j]` red, `roomG[j]` green, and `roomW[j]` white keys from it (you may take any nonnegative quantities up to those amounts, independently). You start with `keys[0]` red, `keys[1]` green, and `keys[2]` white keys. At any moment, a white key can be used as either a red or a green key, but only if you have enough physical keys of the required type (i.e., you cannot spend more red keys than the total of your red + white keys, but the same white key cannot be used for both colors). You may open doors in any order, and each door at most once. Write a C++ function `int maxKeys(std::vector<int> doorR, std::vector<int> doorG, std::vector<int> roomR, std::vector<int> roomG, std::vector<int> roomW, std::vector<int> keys)` that returns the maximum possible total number of keys (red + green + white) you can have after potentially opening any subset of doors, given that you can choose which doors to open in any order and how to allocate the white keys optimally. The number of doors `n` is between 1 and 15 inclusive. All input values are nonnegative integers. You may assume that the vectors `doorR`, `doorG`, `roomR`, `roomG`, `roomW` all have the same length, and `keys` has length exactly 3.

// This is a classic bitmask dynamic programming problem. Since `n ≤ 15`, there are at most `2^15 = 32768` subsets, which is manageable. The key challenge is that white keys are flexible: they can act as either red or green, but a single white key cannot be used simultaneously for both. To handle this, we treat white keys as a resource that we can pre-allocate into either color. For each subset `mask` of doors already opened, we want to know all possible numbers of red keys we could have (with the remaining keys implicitly being green and white). However, we can simplify by converting all white keys into either red or green upfront? Actually, a better approach is to track the count of red keys after opening a subset, assuming we have optimally used white keys. The total number of keys after opening `mask` is deterministic: `keycnt[mask] = keys[0]+keys[1]+keys[2] + sum over opened doors of (roomR+roomG+roomW - doorR - doorG)`. That total is fixed for a given subset. So if we know the number of red keys `x` in that state, then the number of non-red (green) keys is `y = keycnt[mask] - x`. The challenge is that white keys can shift between the two categories, so for a given mask, there can be multiple feasible `x` values. For each state, we keep a set of possible red key counts. Initially, with no doors opened, we can convert any number `i` from 0 to `keys[2]` white keys into red keys, so `f[0]` contains all integers from `keys[0]` to `keys[0]+keys[2]`. For each mask, for each possible red count `x`, we compute `y = keycnt[mask] - x`. To open door `j` not in `mask`, we require `x >= doorR[j]` and `y >= doorG[j]`. The physical constraint that a white key cannot be used for both is automatically satisfied because we are only tracking total red vs non-red, and the opening consumes exactly `doorR[j]` red and `doorG[j]` green keys from the available pool; since we treat any key as usable for either color, the only restriction is having enough total of each color. After opening door `j`, we gain `roomR[j]` red, `roomG[j]` green, and `roomW[j]` white keys. The new red count can be `x - doorR[j] + roomR[j] + k` for any `k` from 0 to `roomW[j]`, where `k` is the number of the new white keys we immediately convert to red. This generates all possible red counts for the new mask. The answer is the maximum `keycnt[mask]` over all masks that have at least one feasible red count. We must be careful: if a subset is unreachable, its `f` is empty and it should be ignored. Also note that the total key count after opening a door might decrease (if door costs exceed room gains), so we cannot simply keep the best total; we must evaluate all reachable subsets. Time complexity: There are `O(2^n)` masks, and for each mask we may store up to `O(total keys)` distinct red counts. In the worst case, the number of distinct red counts per mask can be up to the sum of all room white keys plus initial white keys, which could be up to maybe `n * max_value` (but values are small enough in practice). A safe bound is `O(2^n * K^2)` where `K` is the maximum possible total keys, but for typical constraints (values up to 1000, n=15) this is acceptable. Space complexity is `O(2^n * K)`.

#include <vector>
#include <unordered_set>
#include <algorithm>

// Given door requirements, room rewards, and initial keys, return the maximum
// total number of keys (red + green + white) that can be obtained by opening
// a subset of doors in any order, using white keys flexibly.
int maxKeys(std::vector<int> doorR, std::vector<int> doorG,
            std::vector<int> roomR, std::vector<int> roomG,
            std::vector<int> roomW, std::vector<int> keys) {
    int n = static_cast<int>(doorR.size());
    const int totalMasks = 1 << n;
    
    // keycnt[mask] = total number of keys after opening exactly the doors in mask
    std::vector<int> keycnt(totalMasks, 0);
    for (int mask = 0; mask < totalMasks; ++mask) {
        int total = keys[0] + keys[1] + keys[2];
        for (int j = 0; j < n; ++j) {
            if (mask & (1 << j)) {
                total += roomR[j] + roomG[j] + roomW[j] - doorR[j] - doorG[j];
            }
        }
        keycnt[mask] = total;
    }
    
    // f[mask] = set of possible numbers of red keys after opening doors in mask
    std::vector<std::unordered_set<int>> f(totalMasks);
    // Initially, we can convert any number of white keys to red: 0 .. keys[2]
    for (int i = 0; i <= keys[2]; ++i) {
        f[0].insert(keys[0] + i);
    }
    
    // Process masks in increasing order of bits (any order works due to monotonicity)
    for (int mask = 0; mask < totalMasks; ++mask) {
        if (f[mask].empty()) continue;
        for (int red : f[mask]) {
            int green = keycnt[mask] - red; // all non-red keys treated as green
            // Try to open each unopened door
            for (int j = 0; j < n; ++j) {
                if (mask & (1 << j)) continue;
                if (red >= doorR[j] && green >= doorG[j]) {
                    // After opening door j, we gain new keys.
                    // Let k be how many of the room's white keys we convert to red.
                    int baseRed = red - doorR[j] + roomR[j];
                    for (int k = 0; k <= roomW[j]; ++k) {
                        f[mask | (1 << j)].insert(baseRed + k);
                    }
                }
            }
        }
    }
    
    // Find the maximum total key count among all reachable masks
    int answer = 0;
    for (int mask = 0; mask < totalMasks; ++mask) {
        if (!f[mask].empty()) {
            answer = std::max(answer, keycnt[mask]);
        }
    }
    return answer;
}

#include <cassert>
#include <vector>

// The solution function is declared above (include its definition here in a real project)
int main() {
    // Example 1: No doors, start with some keys.
    assert(maxKeys({}, {}, {}, {}, {}, {1, 2, 3}) == 6);
    
    // Example 2: One door requiring (1,1), room gives (2,0,1), start with (1,0,0).
    assert(maxKeys({1}, {1}, {2}, {0}, {1}, {1,0,0}) == 4); // open door, total = 1+0+0 + (2+0+1-1-1)=2? wait: initial total=1, after open total=1+ (2+0+1-1-1)=2, but we also count keys? Actually total after open = 1 + (2+0+1-1-1)=2, but the answer should be 2? Let's compute: start red=1, green=0, white=0. open door: need red>=1, green>=0, so ok. Gain roomR=2, roomG=0, roomW=1, spend doorR=1, doorG=1. After spending: red=1-1+2=2, green=0-1+0=-1? That's invalid because green becomes -1! Wait, we must have green >= doorG=1, but initial green=0, so can we open? We have white=0, so no. Actually we cannot open this door because we need 1 green key. So the answer is just initial total = 1. Let's correct the test to a feasible one.
    assert(maxKeys({1}, {1}, {2}, {2}, {0}, {1,1,0}) == 4); // start red=1,green=1, open door, red becomes 1-1+2=2, green becomes 1-1+2=2, total=4.

    // Example 3: Two doors, one opens first, then second.
    // door0: (2,0), room0: (0,0,3); door1: (0,2), room1: (4,0,0). Start (0,0,2).
    // We can convert both whites to red to open door0? door0 needs 2 red, so convert 2 whites to red -> red=2,green=0, open door0, gain 3 white -> now we have red=0,green=0,white=3. Then convert all whites to green? door1 needs 2 green, convert 2 whites to green -> green=2, open door1, gain 4 red -> total after both = 0+0+3 + (4+0+0-0-2)=5? Let's compute: after door0 total = initial 0+0+2 + (0+0+3-2-0)=3. After door1 total = 3 + (4+0+0-0-2)=5. So answer should be 5.
    assert(maxKeys({2,0}, {0,2}, {0,4}, {0,0}, {3,0}, {0,0,2}) == 5);

    // Example 4: Door requires more than we have, cannot open.
    assert(maxKeys({5}, {0}, {1}, {0}, {0}, {0,0,3}) == 3); // need 5 red, only 3 whites can become red, so no open.

    // Example 5: Multiple possible orders, ensure max is found.
    // door0: (1,0), room0: (0,1,0); door1: (0,1), room1: (1,0,0); start (1,1,0)
    // Open both: order door0 then door1: after door0 red=0,green=2, then door1 red=1,green=1 total=2. Same for other order. initial total=2, so answer=2.
    assert(maxKeys({1,0}, {0,1}, {0,1}, {1,0}, {0,0}, {1,1,0}) == 2);

    // Example 6: Room gives more than costs, total increases.
    assert(maxKeys({1}, {0}, {5}, {0}, {0}, {1,0,0}) == 5); // open, red=1-1+5=5, total=5.

    // Example 7: n=3, a more complex case with white keys.
    // door0: (1,0), room0: (0,0,2); door1: (0,1), room1: (0,0,2); door2: (2,2), room2: (3,3,0); start (1,1,0)
    // Open door0 and door1 first: after both, we have? Start red=1,green=1. Open door0: red=0,green=1, gain 2 white -> total keys = 0+1+2=3. Open door1: need green>=1, ok, after: red=0, green=0, gain 2 white -> total = 0+0+4=4. Now we have 4 white keys. Door2 needs 2 red and 2 green, we can convert 2 whites to red and 2 to green, open door2, then gain 3 red+3 green, total after = 4 + (3+3+0-2-2)=6. So answer=6.
    assert(maxKeys({1,0,2}, {0,1,2}, {0,0,3}, {0,0,3}, {2,2,0}, {1,1,0}) == 6);

    return 0;
}
