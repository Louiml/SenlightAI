You are given the number of songs `n` and the number of queries `m`. Each song `i` has a duration described by two integers `c_i` (number of times it repeats a short melody) and `t_i` (length of that melody in seconds), so the total length of song `i` is `c_i * t_i`. Songs are played one after another in order (song 1, then song 2, …), and the entire playlist is continuous. For each of the `m` queries, you are given a time `v` in seconds (where `v` is strictly between 0 and the total playlist length, and queries are given in strictly increasing order). You must output the index (1-based) of the song that is playing at exactly time `v` seconds from the start of the playlist. Write a C++ function that takes `n`, a vector of `(c, t)` pairs, and a vector of query times `v` (all in increasing order), and returns a vector of integers containing the song index for each query.
#include <cassert>
#include <vector>
#include <utility>

// The function from the solution (declared here for the test)
std::vector<int> findSongs(
    const std::vector<std::pair<int, int>>& songs,
    const std::vector<int>& queries
);

int main() {
    // Example: 3 songs: (3,5)=15s, (2,10)=20s, (4,5)=20s. Total 55s.
    std::vector<std::pair<int, int>> songs1 = {{3,5}, {2,10}, {4,5}};
    std::vector<int> queries1 = {7, 15, 16, 35, 55};
    assert(findSongs(songs1, queries1) == std::vector<int>({1, 1, 2, 3, 3}));

    // Single song
    std::vector<std::pair<int, int>> songs2 = {{10, 10}}; // 100s
    std::vector<int> queries2 = {1, 50, 100};
    assert(findSongs(songs2, queries2) == std::vector<int>({1, 1, 1}));

    // Two songs, first very short
    std::vector<std::pair<int, int>> songs3 = {{1, 1}, {100, 1}}; // 1s, then 100s
    std::vector<int> queries3 = {1, 2, 101};
    assert(findSongs(songs3, queries3) == std::vector<int>({1, 2, 2}));

    // Edge: query at exact boundary
    std::vector<std::pair<int, int>> songs4 = {{2, 5}, {3, 3}}; // 10s, then 9s
    std::vector<int> queries4 = {10, 11, 19};
    assert(findSongs(songs4, queries4) == std::vector<int>({1, 2, 2}));

    // All songs are 1 second each
    std::vector<std::pair<int, int>> songs5 = {{1,1}, {1,1}, {1,1}};
    std::vector<int> queries5 = {1, 2, 3};
    assert(findSongs(songs5, queries5) == std::vector<int>({1, 2, 3}));

    // Larger test: 5 songs, varying lengths
    std::vector<std::pair<int, int>> songs6 = {{2,3}, {1,5}, {4,1}, {3,2}, {2,4}};
    // lengths: 6, 5, 4, 6, 8 => pref: 6, 11, 15, 21, 29
    std::vector<int> queries6 = {1, 6, 7, 11, 12, 15, 16, 21, 22, 29};
    std::vector<int> expected6 = {1,1,2,2,3,3,4,4,5,5};
    assert(findSongs(songs6, queries6) == expected6);

    return 0;
}
#include <vector>
#include <utility>
#include <cstddef>

// Return the 1-based song index for each query time.
// songs: vector of (c, t) pairs, queries: strictly increasing times.
std::vector<int> findSongs(
    const std::vector<std::pair<int, int>>& songs,
    const std::vector<int>& queries
) {
    const int n = static_cast<int>(songs.size());
    // Prefix sums: pref[i] = total duration of songs 0..i-1
    std::vector<long long> pref(n + 1, 0);
    for (int i = 0; i < n; ++i) {
        pref[i + 1] = pref[i] + 
            static_cast<long long>(songs[i].first) * songs[i].second;
    }

    std::vector<int> result;
    result.reserve(queries.size());

    int songIndex = 1;  // 1-based song index (first song)
    for (int v : queries) {
        // Advance songIndex until the current query fits in that song
        while (songIndex <= n && static_cast<long long>(v) > pref[songIndex]) {
            ++songIndex;
        }
        result.push_back(songIndex);
    }
    return result;
}
// The key observation is that the playlist is a sequence of non‑overlapping intervals: song 1 occupies seconds `(0, c1*t1]`, song 2 occupies `(c1*t1, c1*t1+c2*t2]`, etc. So we first compute prefix sums `pref[i]` = total duration of songs 1 through `i`. For each query `v`, we need to find the smallest index `song` such that `pref[song] >= v`. Since the queries are given in strictly increasing order, we can use a two‑pointer approach: maintain a current song index and advance it as necessary for each query. Because both queries and the `pref` array are sorted, the pointer only moves forward, giving O(n + m) time. Edge cases: `v` is always positive and never exceeds the total duration, so we don't need to handle `v == 0`; also queries are strictly increasing so no duplicate handling is needed. Space complexity is O(n) for the prefix sums. If queries were not sorted, we could use binary search (lower_bound) on `pref` for each query, giving O(m log n) time, but the two‑pointer version is optimal given the problem constraint.
