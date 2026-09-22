/*
You are given a string `s` of length `n` consisting only of the characters `'.'` and `'*'`. In one operation, you may move a `'*'` to an adjacent position (left or right by one index) if that target position is currently empty (i.e., contains `'.'`). You may perform any number of such moves. Your goal is to make all occurrences of `'*'` contiguous (i.e., form a single block of consecutive `'*'` characters with no `'.'` between them). Write a C++ function `long long minMovesToGroup(const std::string& s)` that returns the minimum total number of moves required to achieve this. The moves can be in any order, and the final block of `'*'` can be placed anywhere within the string. The input length `n` satisfies `1 <= n <= 10^5`. The function should return the minimal total move count, which can be up to about \(10^{10}\) (fit in a 64-bit integer). Note that there is no need to preserve the order of `'*'` characters; you may interleave them as needed since each move is independent.
*/

#include <string>
#include <vector>
#include <algorithm>

// Return the minimum total moves to make all '*' contiguous in s.
long long minMovesToGroup(const std::string& s) {
    const long long INF = 4e18;
    int n = static_cast<int>(s.size());

    std::vector<long long> pre(n + 1, 0);
    std::vector<long long> suf(n + 1, 0);

    // Left to right: pre[i] = cost to move all stars strictly before i to be contiguous ending at i-1.
    long long starsSeen = 0;
    for (int i = 0; i < n; ++i) {
        pre[i + 1] = pre[i];
        if (s[i] == '*') {
            ++starsSeen;
        } else {
            pre[i + 1] += starsSeen;
        }
    }

    // Right to left: suf[i] = cost to move all stars strictly after i to be contiguous starting at i+1.
    starsSeen = 0;
    for (int i = n - 1; i >= 0; --i) {
        suf[i] = suf[i + 1];
        if (s[i] == '*') {
            ++starsSeen;
        } else {
            suf[i] += starsSeen;
        }
    }

    long long answer = INF;
    for (int i = 0; i <= n; ++i) {
        answer = std::min(answer, pre[i] + suf[i]);
    }
    return answer;
}

#include <cassert>
#include <string>

// Declaration from the solution
long long minMovesToGroup(const std::string& s);

int main() {
    // No stars -> no moves
    assert(minMovesToGroup("....") == 0);
    // All stars -> already contiguous
    assert(minMovesToGroup("****") == 0);
    // One star -> no moves
    assert(minMovesToGroup("..*..") == 0);
    // Two stars with one gap -> move one star by one
    assert(minMovesToGroup("*.*") == 1);
    // Two stars with two gaps -> move inner star? actually minimal is 1? Let's compute: total positions 5, stars at 0 and 4. Move left star right one and right star left one => 2 moves. But better keep block at positions 1-2? Actually minimal is move star at 0 right 2 and star at 4 left 2 => 4? Wait let's think: string " *...*" (index 0: '.', 1:'*', 2:'.',3:'.',4:'*') moves: move star from index1 to index2 (1 move), then we have "..**."? Actually star at index4 stays, block at 2 and 4 not adjacent. Need one more move: move star from 2 to 3? But star at 2 came from 1. So total 2 moves. Let's just give a known case: "*.*" -> 1, "**.*" -> 1 (move last star left by 1), ".*.*" -> 1, "*..*" -> 2 (move first star right 2 or last left 2). So assert that.
    assert(minMovesToGroup("*..*") == 2);
    // Mixed case: ".*.*." -> stars at 1 and 3, move either one left or right by 1, total 1
    assert(minMovesToGroup(".*.*.") == 1);
    // Larger test: "**...**" -> stars at 0,1,4,5. Output? Group at positions 3-6? Actually minimal is move two right stars left 2 each? Let's compute: pre/suf approach will give correct. For sanity, we can compute manually: optimal final block positions 1-4 (indices 1,2,3,4). Move star at 0 to 1 (1 move), star at 1 stays, star at 4 stays, star at 5 to 4 (1 move) total 2. So assert 2.
    assert(minMovesToGroup("**...**") == 2);
    // Edge: many stars separated: "*.*.*" -> stars at 0,2,4. Move middle star to 1 or 3? Actually group all at positions 1-3: move left star to 1 (1), middle stays at 2, right star to 3 (1) total 2. Or group at 0-2: move middle to 1 (1), right to 2 (2) total 3. So min is 2.
    assert(minMovesToGroup("*.*.*") == 2);
    // Another: "..*..*.." -> stars at 2 and 5. Move one to other side: distance 3 => 3 moves.
    assert(minMovesToGroup("..*..*..") == 3);
    // Single star in long string
    assert(minMovesToGroup(".........*") == 0);
    return 0;
}

// The optimal strategy is to choose a final position for the contiguous block of stars and move all stars into that block with minimal total cost. This is a classic problem where we can consider prefix and suffix costs. Suppose we fix a point `i` (from 0 to n) as a boundary: all stars originally to the left of `i` will be moved to the left side of the final block, and all stars to the right of `i` will be moved to the right side. The cost for the left side is obtained by scanning from left to right: maintain a counter `starsSeenLeft` that increments each time we encounter a `'*'`. For each `'.'` cell, the cost of moving all previously seen stars past this cell is exactly `starsSeenLeft` (since each star must cross this cell). Accumulate this into `pre[i]` which represents the minimum cost to move all stars strictly to the left of position `i` and group them immediately to the left of `i` (i.e., they need to be contiguous ending at position `i-1`). Similarly, compute `suf[i]` by scanning from right to left: maintain `starsSeenRight`, and for each `'.'` encountered, add `starsSeenRight` to accumulate the cost of moving all stars to the right of `i` to be contiguous immediately to the right of `i`. Then the answer is the minimum over all `i` of `pre[i] + suf[i]`. This works because when we split at `i`, the stars on the left will form a block ending at `i-1`, and stars on the right form a block starting at `i`; their combined cost is exactly `pre[i] + suf[i]` and they are adjacent (no gap) so the whole set is contiguous. Edge cases: if there are no stars, the answer is 0. If all are stars, no moves needed, also 0. If there is one star, cost is 0. The algorithm runs in O(n) time and O(n) auxiliary space for the two arrays.
