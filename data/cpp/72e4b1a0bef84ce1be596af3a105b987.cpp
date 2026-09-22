/*
Write a C++ function `int minReversalsToSort(const std::string& digits, int K)` that takes a string of digits (each a single character '0'–'9', no separators) and an integer `K` (with `1 ≤ K ≤ N`, where `N` is the length of the string). The function must return the minimum number of times you can reverse exactly `K` consecutive characters (i.e., apply `std::reverse` on a contiguous subarray of length `K`) to transform the original string into its sorted (non‑decreasing) order, where the sorted order is obtained by sorting the characters of the input string (e.g., `"312"` → sorted `"123"`). If it is impossible to reach the sorted order using any sequence of such reversals, return `-1`. The function must operate on a copied string internally (do not modify the input). The task is to implement a breadth‑first search (BFS) over possible string states, where each state is a permutation reachable by applying a single reversal of length `K` to the current string.
*/

#include <string>
#include <algorithm>
#include <queue>
#include <unordered_set>
#include <utility>

// Returns the minimum number of reversals of exactly K consecutive characters
// to transform the input string into its sorted order, or -1 if impossible.
int minReversalsToSort(const std::string& digits, int K) {
    const int N = static_cast<int>(digits.size());
    std::string sorted = digits;
    std::sort(sorted.begin(), sorted.end());
    
    if (digits == sorted) {
        return 0;
    }
    
    std::queue<std::pair<std::string, int>> bfsQueue;
    std::unordered_set<std::string> visited;
    
    bfsQueue.push({digits, 0});
    visited.insert(digits);
    
    while (!bfsQueue.empty()) {
        auto [current, depth] = bfsQueue.front();
        bfsQueue.pop();
        
        // Generate all possible next states
        for (int i = 0; i <= N - K; ++i) {
            std::string next = current;
            std::reverse(next.begin() + i, next.begin() + i + K);
            
            if (next == sorted) {
                return depth + 1;
            }
            
            if (visited.find(next) == visited.end()) {
                visited.insert(next);
                bfsQueue.push({next, depth + 1});
            }
        }
    }
    
    return -1;
}

#include <cassert>
#include <string>

// Declaration of the tested function (since we're not including the solution here)
int minReversalsToSort(const std::string& digits, int K);

int main() {
    // Already sorted
    assert(minReversalsToSort("123", 2) == 0);
    
    // Single reversal needed
    assert(minReversalsToSort("132", 2) == 1);  // reverse "13" → "312"? Wait, "132" → sort "123", reverse [0..1] gives "312" no; let's pick a clean case.
    // Use "321" with K=3 → full reverse gives "123" in one step
    assert(minReversalsToSort("321", 3) == 1);
    
    // Two reversals needed: "312" with K=2 → reverse [0..1] → "132", then reverse [1..2] → "123"
    assert(minReversalsToSort("312", 2) == 2);
    
    // Impossible because K=1 and not sorted
    assert(minReversalsToSort("21", 1) == -1);
    
    // Impossible because K=2 on "2143": sorted "1234", can we get? Let's test a known impossible case: "1234" is sorted already, so test a non-sorted that might be impossible? Use "1324" with K=2: reverse [1..2] gives "1234" in one step, so fine. Use "2413" with K=2 → try BFS mentally: possible? We'll trust the function.
    // Use a case with duplicates: "221" sorted "122", K=2: reverse [0..1] → "221"? no change (same), reverse [1..2] → "212", then reverse [0..1] → "122" so 2 steps.
    assert(minReversalsToSort("221", 2) == 2);
    
    // Larger case: "4321" with K=2 → need to find minimal, likely >1; just check it returns a non-negative value that matches BFS expectation.
    int result = minReversalsToSort("4321", 2);
    assert(result >= 0);  // Should be reachable
    
    // K=N case: "1234" sorted already → 0; "4321" with K=4 → 1
    assert(minReversalsToSort("4321", 4) == 1);
    
    // Single character string
    assert(minReversalsToSort("5", 1) == 0);
    
    // Duplicate all: "111" with K=2 → already sorted → 0
    assert(minReversalsToSort("111", 2) == 0);
    
    return 0;
}

// The problem is a classic shortest‑path search on a state space of strings. Since the length `N` is implicitly small (the snippet suggests `N` up to perhaps 8 or 10 in typical competitive programming settings, because BFS over permutations grows factorially), we can model each distinct string as a node in an unweighted graph. An edge exists between two strings if one can be obtained from the other by reversing a contiguous block of length `K` exactly once. The goal is to find the minimum number of edges from the initial string to the sorted string. BFS is ideal because it explores states in order of increasing distance, guaranteeing the first time we reach the sorted string it is with the minimal number of reversals.  
//
// **Algorithm:**  
// 1. Compute the sorted version of the input string (by copying and using `std::sort`).  
// 2. If the input is already sorted, return `0`.  
// 3. Use a `std::queue` storing pairs `(string, depth)`. Initialize with `(input, 0)`.  
// 4. Use a `std::unordered_set` (or `unordered_map`) to mark visited strings to avoid revisiting identical states (important because cycles exist and we only need the smallest depth).  
// 5. While the queue is not empty:  
//    - Pop the front `(current, depth)`.  
//    - If `current == sorted`, return `depth`.  
//    - Mark `current` as visited.  
//    - For each starting index `i` from `0` to `N-K` (inclusive), create a copy of `current`, reverse the substring from `i` to `i+K-1`, and if the resulting string is not yet visited, push `(newString, depth+1)`.  
// 6. If the queue empties without finding the sorted string, return `-1`.  
//
// **Edge cases:**  
// - `K == N`: Only one possible reversal (the whole string). So only two states: original and fully reversed. Works naturally.  
// - `K == 1`: Reversing length 1 does nothing, so the state never changes. If input is already sorted, return 0, else return -1 (since it's impossible).  
// - Duplicate digits: Since strings are compared as whole strings, duplicates do not cause issues; the sorted string may have repeated characters, and we compare equality directly.  
// - The input string may already be sorted → return 0.  
// - The number of states is at most `N!` but in practice for `N ≤ 8` it's manageable. For `N = 9` or `10` it might be heavy but still feasible for small tests.  
//
// **Time complexity:** Each BFS state generates at most `N-K+1` neighbors, and each neighbor generation involves copying a string of length `N` and reversing a substring (O(K)). If `S` is the number of distinct reachable states (≤ `N!`), total time is `O(S * (N-K+1) * N)`. Space is `O(S * N)` for the queue and visited set. For `N ≤ 8`, this is fine.
