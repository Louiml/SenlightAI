You are given a string `S` of length `N` consisting only of the characters `'L'`, `'R'`, `'U'`, and `'D'`, representing movements on a 2D grid. Starting from the origin `(0,0)`, each character moves one unit: `'L'` decreases x by 1, `'R'` increases x by 1, `'U'` increases y by 1, and `'D'` decreases y by 1. Write a function `findShortestRepeatedSegment` that, given the string `S` and its length `N`, returns a `pair<int,int>` representing the 1-based indices `[start, end]` of the shortest contiguous substring of the movement sequence such that the robot’s position after executing that substring is the same as before executing it (i.e., the net displacement of that substring is (0,0)). If no such substring exists, return `{-1,-1}`. When there are multiple shortest substrings of equal minimal length, return the one that appears earliest in the string (i.e., the smallest starting index; if tied, also the smallest ending index). The input length `N` satisfies `1 <= N <= 200000`. The function must be efficient enough for large inputs.
#include <cassert>

int main() {
    // Basic cases
    assert(findShortestRepeatedSegment("LR", 2) == std::make_pair(1, 2));
    assert(findShortestRepeatedSegment("RLUD", 4) == std::make_pair(1, 4));
    assert(findShortestRepeatedSegment("UU", 2) == std::make_pair(-1, -1));
    assert(findShortestRepeatedSegment("UD", 2) == std::make_pair(1, 2));

    // Multiple zero segments: shortest is "UD" at positions 2-3
    assert(findShortestRepeatedSegment("RUDL", 4) == std::make_pair(2, 3));

    // Tie-breaking: two segments of length 2, earliest start wins
    assert(findShortestRepeatedSegment("LRRL", 4) == std::make_pair(1, 2));

    // Longer string with internal zero segments
    assert(findShortestRepeatedSegment("UUDRL", 5) == std::make_pair(2, 4));

    // No zero-displacement substring (except single char? no, single char has nonzero displacement)
    assert(findShortestRepeatedSegment("L", 1) == std::make_pair(-1, -1));
    assert(findShortestRepeatedSegment("RRR", 3) == std::make_pair(-1, -1));

    // Full string zero net, but there is a shorter internal segment
    assert(findShortestRepeatedSegment("RLRL", 4) == std::make_pair(1, 2)); // "RL" at start

    // Verification of a case where segment spans multiple moves
    assert(findShortestRepeatedSegment("URDL", 4) == std::make_pair(1, 4));

    return 0;
}
#include <map>
#include <string>
#include <utility>

// Finds the shortest contiguous substring whose net displacement is zero.
// Returns pair {start, end} (1-based). If none exists, returns {-1,-1}.
std::pair<int,int> findShortestRepeatedSegment(const std::string& S, int N) {
    std::map<std::pair<int,int>, int> prefixMap;
    int x = 0, y = 0;
    prefixMap[{0, 0}] = 0;

    int bestLen = N + 1;
    int bestStart = -1, bestEnd = -1;

    for (int i = 0; i < N; ++i) {
        char c = S[i];
        if (c == 'L') --x;
        else if (c == 'R') ++x;
        else if (c == 'U') ++y;
        else if (c == 'D') --y;

        auto it = prefixMap.find({x, y});
        if (it != prefixMap.end()) {
            int prev = it->second;
            int currentLen = i - prev + 1;
            if (currentLen < bestLen ||
                (currentLen == bestLen && (prev + 1) < bestStart)) {
                bestLen = currentLen;
                bestStart = prev + 1;
                bestEnd = i + 1;
            }
        }

        // Store earliest occurrence only
        if (prefixMap.find({x, y}) == prefixMap.end()) {
            prefixMap[{x, y}] = i + 1;
        }
    }

    if (bestStart == -1) {
        return {-1, -1};
    }
    return {bestStart, bestEnd};
}
// The problem is to find the shortest subarray of movements whose net effect is a zero displacement. The approach uses a hash map (or `std::map` for safety) that stores the position after each prefix of the movement sequence, mapping a coordinate pair to the earliest 1-based index after which that position was reached (i.e., index `i+1` if prefix is `S[0..i]`). We iterate over the string, updating the current position. Before updating the map, if the current position has been seen before, then the substring from the previous occurrence (exclusive) to the current index (inclusive) has zero net displacement. The length of that substring is `current_index - previous_index` where `previous_index` is the stored integer (which is the prefix length). We track the minimum such length, and for ties, the earliest starting index. Initially, the map contains `(0,0) -> 0`. For each prefix, we check if the current coordinate exists in the map; if so, we compute the candidate segment length. We update the best answer if the length is smaller, or if equal and the start index is smaller. Then we store the current position with the current prefix length (`i+1`) if it doesn’t already exist or if the existing value is larger (but since we store the earliest, we only insert if not present). Edge cases: the whole string might have zero net displacement, but we want the shortest; if no segment yields zero, return `{-1,-1}`. The time complexity is `O(N log N)` due to `std::map`; using `std::unordered_map` with a custom hash would be `O(N)` average. Space is `O(N)`.
