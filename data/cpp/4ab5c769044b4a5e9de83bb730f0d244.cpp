/*
You are implementing a caching simulator for a digital audio player. The player has a fixed-size playlist of `k` songs, and the user performs a series of playback requests. Each request gives a song ID `x`. The player uses a "move-to-front" caching policy: when a song is requested, the player finds its position in the playlist (0-indexed), counts the number of previous songs it had to skip (the position index `t`), adds `t+1` to a total "seek cost", then moves that requested song to the front of the playlist by rotating the segment of the playlist from index 0 to that song's position (so the requested song becomes the first element). The user performs requests in a grid: there are `n` rows and `m` columns of requests, given row-major order (i.e., all `m` requests of row 0, then all `m` of row 1, etc.). Given integers `n`, `m`, `k` (where `1 <= n,m,k <= 100`), an initial playlist of `k` distinct song IDs (the array `a`), and the sequence of `n*m` requests, write a function that returns the total seek cost after processing all requests. The input format: first line has three integers `n, m, k`; second line has `k` integers (the initial playlist); then `n` lines each containing `m` integers (the requests). Note that the playlist always contains exactly these `k` distinct IDs; every request is guaranteed to be one of them. The total cost may be up to `100*100*100 = 1,000,000`, which fits in an `int`. In your solution, use a simple array or vector to represent the playlist, and apply the `rotate` operation (either via `std::rotate` or manual shifting) to perform the move-to-front.
*/

#include <vector>
#include <algorithm>

// Simulates a move-to-front cache and returns the total seek cost.
// n: number of rows, m: number of columns of requests, k: playlist size.
// initial: the initial playlist of k distinct IDs (by copy as we modify it).
// requests: a vector of n*m requests in row-major order.
int totalSeekCost(int n, int m, const std::vector<int>& initial, const std::vector<int>& requests) {
    std::vector<int> playlist = initial; // copy to modify
    int total = 0;
    size_t request_count = static_cast<size_t>(n) * m;
    for (size_t i = 0; i < request_count; ++i) {
        int x = requests[i];
        // find index t where playlist[t] == x
        int t = 0;
        while (playlist[t] != x) {
            ++t;
        }
        total += t + 1;
        // move-to-front: rotate first t+1 elements right by one
        // (brings element at index t to front)
        std::rotate(playlist.begin(), playlist.begin() + t, playlist.begin() + t + 1);
    }
    return total;
}

#include <cassert>
#include <vector>

// (Include the above solution function code here in a real test context.)

int main() {
    // Basic case: n=1, m=3, k=3, initial [1,2,3], requests [2,1,3]
    // Step1: request 2 at index1 -> cost2, playlist becomes [2,1,3]
    // Step2: request 1 at index1 -> cost2, playlist becomes [1,2,3]
    // Step3: request 3 at index2 -> cost3, playlist becomes [3,1,2]
    // Total = 2+2+3 = 7
    assert(totalSeekCost(1, 3, {1,2,3}, {2,1,3}) == 7);

    // All requests for the front element: cost always 1
    assert(totalSeekCost(2, 2, {5,6,7}, {5,5,5,5}) == 4);

    // Requests in reverse order: costs 1,2,3 then reset? Let's compute manually.
    // initial [1,2,3], requests [3,2,1]
    // request 3 at index2 cost3 -> playlist [3,1,2]
    // request 2 at index2 cost3 -> playlist [2,3,1]
    // request 1 at index2 cost3 -> playlist [1,2,3] total 9
    assert(totalSeekCost(1, 3, {1,2,3}, {3,2,1}) == 9);

    // Large playlist, single request for last element: cost = k
    assert(totalSeekCost(1, 1, {10,20,30,40}, {40}) == 4);

    // Repeated request alternating first and last: 
    // initial [1,2,3], requests [1,3,1,3]
    // sequence:
    // 1: cost1, playlist unchanged [1,2,3]
    // 3: cost3, playlist [3,1,2]
    // 1: cost2, playlist [1,3,2]
    // 3: cost2, playlist [3,1,2]
    // total = 1+3+2+2 = 8
    assert(totalSeekCost(1, 4, {1,2,3}, {1,3,1,3}) == 8);

    // Grid with multiple rows: n=2, m=2, k=2, initial [7,8], requests [7,8,8,7]
    // Step1: 7 cost1 playlist [7,8]
    // Step2: 8 cost2 playlist [8,7] total 3
    // Step3: 8 cost1 playlist [8,7] total 4
    // Step4: 7 cost2 playlist [7,8] total 6
    assert(totalSeekCost(2, 2, {7,8}, {7,8,8,7}) == 6);

    // Single element playlist: any request cost 1
    assert(totalSeekCost(1, 5, {42}, {42,42,42,42,42}) == 5);

    return 0;
}

// The core operation is to simulate move-to-front caching. The playlist is a vector of `k` integers. For each request `x`, we find its index `t` in the current playlist (guaranteed to exist). The cost contributed is `t+1` because we must skip `t` songs before reaching `x`, and then the actual play operation adds 1. After that, we move that element to the front: the most efficient way is to use `std::rotate` on the range `[playlist.begin(), playlist.begin()+t, playlist.begin()+t+1]`, which circularly shifts the first `t+1` elements right by one, placing the requested song at index 0 and shifting the previous front songs back by one. Since `k` is at most 100 and there are at most `100*100=10000` requests, a total complexity of O(n*m*k) = O(10^6) is trivial. Edge cases: when the requested song is already at the front (`t=0`), cost is 1 and rotation does nothing (or safely rotates a single-element range). The cost sum can be stored as `int` since max is 1,000,000. Time complexity: O(n*m*k) in the worst case due to the linear search and rotation of up to `k` elements per request. Space complexity: O(k) for the playlist plus O(1) auxiliary.
