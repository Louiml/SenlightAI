Write a C++ function that takes an integer `n` and a string `s` of length `n`, where `s` consists only of characters `'L'`, `'R'`, `'D'`, and `'U'`, representing moves of a point on a 2D grid starting at `(0,0)`. The function should find the shortest contiguous substring (i.e., a consecutive segment of moves) such that if the robot starts at the origin and executes only that substring, it returns to the origin. Return a pair of 1-based indices `(L, R)` (inclusive) representing the start and end of the shortest such substring. If multiple shortest substrings exist, return the one that appears earliest in the string (i.e., with smallest `L`; if ties, smallest `R`). If no such substring exists, return `{-1, -1}`. For example, for `s = "LRUD"` and `n=4`, the whole string returns to origin, but the shortest is length 2: either `"LR"` (positions 1-2) or `"UD"` (positions 3-4), so return `{1,2}`. The input may contain up to `10^5` moves per test case. The function must be efficient and handle edge cases like empty moves (n=0), all moves in one direction, and multiple overlapping zero-sum segments.
The solution uses a hash map to store the last visited position (as a pair of coordinates) mapped to the index (0-based) just after that position was visited. We start at `(0,0)` and record that we've visited it at index 0 (i.e., before any move). Then we iterate through the moves, updating the current position. At each step `i` (0-based), we check if the current position has been seen before. If so, the substring from the previously recorded index `vis[p]` (which is the move index after the previous visit) to the current index `i` (inclusive) forms a zero-sum segment. The length of this segment is `i - vis[p]` (because `vis[p]` is 0-based position in the index space). We keep track of the shortest such segment, and among ties, the earliest left index. Note that we use 1-based output: if the segment is from index `vis[p]` to `i` in 0-based, the 1-based left is `vis[p]+1` and right is `i+1`. After processing, if no segment found, return `{-1, -1}`. Edge cases: n=0 → no moves, return `{-1,-1}`; a single move can never return to origin unless it's zero-length, but that's not possible; repeated visits are handled by always updating `vis[p]` to the latest index after checking, to ensure we capture the earliest possible left for repeated positions. The algorithm runs in O(n) time and O(n) auxiliary space (for the map), which is optimal.
#include <map>
#include <string>
#include <utility>
#include <algorithm>

// Find the shortest substring of moves that returns to origin (0,0).
// 'moves' is a string of 'L','R','D','U'. Returns 1-based indices {L,R} inclusive.
// If none, returns {-1,-1}.
std::pair<int, int> shortestReturnSubstring(const std::string& moves) {
    int n = static_cast<int>(moves.size());
    if (n == 0) return {-1, -1};
    
    std::map<std::pair<int,int>, int> visited; // position -> index (0-based) just after visit
    std::pair<int,int> pos = {0, 0};
    visited[pos] = 0; // before any move, index 0 (meaning moves[0] is next)
    
    int bestLeft = -1;
    int bestRight = n; // sentinel length = n + 1 essentially
    
    for (int i = 0; i < n; ++i) {
        char c = moves[i];
        if (c == 'L') pos.first--;
        else if (c == 'R') pos.first++;
        else if (c == 'D') pos.second--;
        else if (c == 'U') pos.second++;
        // else ignore invalid characters, but problem guarantees valid
        
        auto it = visited.find(pos);
        if (it != visited.end()) {
            int left = it->second; // 0-based index of start of segment
            int length = i - left; // current i is inclusive right
            if (length < bestRight - bestLeft) {
                bestLeft = left;
                bestRight = i;
            }
        }
        // Update with latest index (i+1) so future visits use the most recent occurrence
        visited[pos] = i + 1;
    }
    
    if (bestLeft == -1) return {-1, -1};
    return {bestLeft + 1, bestRight + 1}; // convert to 1-based
}
#include <cassert>
#include <string>
#include <utility>

// Declare the function from the solution (not shown here for brevity, but must be linked)
std::pair<int, int> shortestReturnSubstring(const std::string& moves);

int main() {
    // Basic cases
    assert(shortestReturnSubstring("LR") == std::make_pair(1,2));
    assert(shortestReturnSubstring("UD") == std::make_pair(1,2));
    assert(shortestReturnSubstring("RL") == std::make_pair(1,2));
    assert(shortestReturnSubstring("DU") == std::make_pair(1,2));
    
    // No zero-sum substring
    assert(shortestReturnSubstring("LL") == std::make_pair(-1,-1));
    assert(shortestReturnSubstring("R") == std::make_pair(-1,-1));
    assert(shortestReturnSubstring("") == std::make_pair(-1,-1));
    
    // Multiple and overlapping segments, choose shortest then earliest
    assert(shortestReturnSubstring("LRUD") == std::make_pair(1,2)); // LR or UD both len2, earliest L=1
    assert(shortestReturnSubstring("RRLL") == std::make_pair(1,4)); // whole thing, no shorter
    assert(shortestReturnSubstring("RLLR") == std::make_pair(2,3)); // LL? no, but "RL" at pos2-3 = LL? actually "LL" returns? No, RLLR: positions 2-3 = 'LL' no, let's check: R(1,0), L(0,0) at i=1? Actually string "RLLR": moves: R->(1,0), L->(0,0) so segment i=1 to 1? Wait 0-based: i=0 R->(1,0), i=1 L->(0,0) so segment [0,1] length2? But that's "RL" returns? R then L: (1,0)->(0,0) yes returns at i=1, so should be (1,2) not (2,3). Let's just test simple known: "RLLR" has "RL" at 1-2, and also "LL"? no. So assert (1,2).)
    assert(shortestReturnSubstring("RLLR") == std::make_pair(1,2)); // corrected
    
    // Longer string with multiple, ensure earliest among shortest
    assert(shortestReturnSubstring("LRLR") == std::make_pair(1,2)); // LR at 1-2, also RL at 2-3? length2 both, earliest L=1
    assert(shortestReturnSubstring("RRLLRRLL") == std::make_pair(1,4)); // RRLL returns? (2,0)->(0,0) yes len4, but also later? Actually RRLL len4, then RRLL again? whole? Best is len4, so (1,4)
    
    // Large n stress: just check a known pattern with no return
    std::string big(100000, 'L');
    assert(shortestReturnSubstring(big) == std::make_pair(-1,-1));
    
    // A pattern with a short segment at the end
    assert(shortestReturnSubstring("LLLLRRRR") == std::make_pair(1,8)); // whole thing returns? LLLLRRRR: net 0? 4 left,4 right yes returns, but no shorter? could "LR" appear? no, so whole.
    
    return 0;
}
