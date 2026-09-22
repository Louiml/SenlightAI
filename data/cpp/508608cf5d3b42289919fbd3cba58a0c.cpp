Given a string `blocks` consisting only of characters `'W'` and `'B'`, and a positive integer `k` where `k <= blocks.length()`, write a C++ function that returns the minimum number of white blocks (`'W'`) that must be recolored to black (`'B'`) so that there exists a contiguous substring of length exactly `k` containing only black blocks. If it is already possible (i.e., there is already such a substring), return 0. The function should handle cases where `k` equals the length of the string, and must work for strings of length up to 100 (small enough to allow a simple sliding window approach).
The problem reduces to finding, over all contiguous windows of length `k` in the string, the window with the fewest number of `'W'` characters. Each white in that chosen window must be recolored, so the answer is exactly that minimum count. A brute-force approach would check every possible starting index from 0 to `n - k`, and for each window count the whites in `O(k)` time, leading to `O(n*k)` time. However, a more efficient sliding window technique counts whites in the first window, then for each subsequent window subtracts the character that leaves and adds the character that enters, maintaining the count in `O(1)` per step. This yields `O(n)` time overall and `O(1)` auxiliary space. Edge cases: if `k == n`, there is only one window, so just count all whites. If the string already contains a window of all `'B'`, the answer is 0 because the minimum count will be 0. The algorithm correctly handles empty whites? The input is non-empty per constraints, but the function can safely handle an empty string by returning 0 (though it's not expected).
#include <string>
#include <algorithm>
#include <climits>

// Returns the minimum number of 'W' characters that must be recolored
// so that there is a contiguous substring of length k containing only 'B'.
// The input string blocks consists of 'W' and 'B' only, and k is positive and <= blocks.size().
int minimumRecolors(const std::string& blocks, int k) {
    if (blocks.empty() || k <= 0) {
        return 0; // Edge-case: no blocks or invalid k; by problem constraints not expected.
    }
    
    int n = static_cast<int>(blocks.size());
    if (k > n) {
        return 0; // Not possible to have a window larger than the string; safe fallback.
    }
    
    // Count whites in the first window of length k
    int currentWhites = 0;
    for (int i = 0; i < k; ++i) {
        if (blocks[i] == 'W') {
            ++currentWhites;
        }
    }
    
    int minWhites = currentWhites;
    
    // Slide the window from left to right
    for (int start = 1; start <= n - k; ++start) {
        // Remove the character leaving the window (start-1)
        if (blocks[start - 1] == 'W') {
            --currentWhites;
        }
        // Add the character entering the window (start+k-1)
        if (blocks[start + k - 1] == 'W') {
            ++currentWhites;
        }
        minWhites = std::min(minWhites, currentWhites);
    }
    
    return minWhites;
}
#include <cassert>

int main() {
    // Example from the given snippet: minimal recolors needed.
    assert(minimumRecolors("WBBWWBBWBW", 7) == 3);
    assert(minimumRecolors("WBWBBBW", 2) == 1);
    assert(minimumRecolors("BBBB", 2) == 0);          // Already all black
    assert(minimumRecolors("WWWW", 4) == 4);          // Entire string must be recolored
    assert(minimumRecolors("B", 1) == 0);             // Single black block
    assert(minimumRecolors("W", 1) == 1);             // Single white block
    assert(minimumRecolors("BWBWBW", 3) == 2);        // Any window has at least 2 whites
    assert(minimumRecolors("WWBWWBWW", 5) == 2);      // Sliding window edge cases
    assert(minimumRecolors("BBWBB", 5) == 1);         // k equals length
    assert(minimumRecolors("WWBBBBWW", 4) == 0);      // Middle substring is all black
    return 0;
}
