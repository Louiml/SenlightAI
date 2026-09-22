// Write a C++ function `int wordConversionSteps(std::string begin, std::string target, const std::vector<std::string>& words)` that returns the minimum number of single-letter transformations needed to change `begin` into `target`, where each intermediate word must exist in the given `words` list, and each transformation changes exactly one letter. If `target` is not present in `words` or no valid transformation sequence exists, return 0. The words are all lowercase strings of equal length, and the list may contain duplicate entries (treat duplicates as irrelevant). The transformation path must start from `begin` and end at `target`, using only words from the list as intermediate states. The function should not modify the input vector and must handle empty `words`, empty strings, or a target equal to `begin` (in which case 0 steps are needed only if `target` is not in `words`; if it is in `words`, 0 steps are still correct because no transformation is needed).
#include <cassert>
#include <string>
#include <vector>

int wordConversionSteps(const std::string& begin, const std::string& target, const std::vector<std::string>& words);

int main() {
    // Standard example from the snippet
    std::vector<std::string> words1 = {"hot", "dot", "dog", "lot", "log", "cog"};
    assert(wordConversionSteps("hit", "cog", words1) == 4);
    
    // Target not in words
    std::vector<std::string> words2 = {"hot", "dot", "dog", "lot", "log"};
    assert(wordConversionSteps("hit", "cog", words2) == 0);
    
    // Begin equals target (target in words)
    assert(wordConversionSteps("hot", "hot", words1) == 0);
    
    // No path possible
    std::vector<std::string> words3 = {"cat", "bat", "rat"};
    assert(wordConversionSteps("dog", "cat", words3) == 0);
    
    // Direct one-step conversion
    std::vector<std::string> words4 = {"cat", "bat", "rat"};
    assert(wordConversionSteps("cat", "bat", words4) == 1);
    
    // Duplicate words in list
    std::vector<std::string> words5 = {"hot", "hot", "dot", "dog", "cog"};
    assert(wordConversionSteps("hot", "cog", words5) == 2);
    
    // Single character words
    std::vector<std::string> words6 = {"b", "c", "d"};
    assert(wordConversionSteps("a", "d", words6) == 3);
    
    // Empty words list
    std::vector<std::string> words7;
    assert(wordConversionSteps("a", "b", words7) == 0);
    
    // Longer words with multiple paths, ensure shortest is returned
    std::vector<std::string> words8 = {"aab", "abb", "abc", "bbc", "cbc"};
    assert(wordConversionSteps("aaa", "cbc", words8) == 3);
    
    return 0;
}
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>

// Returns minimum number of single-letter transformations from begin to target 
// using only words from the given list. Returns 0 if target not in words or unreachable.
int wordConversionSteps(const std::string& begin, const std::string& target, const std::vector<std::string>& words) {
    // Build a set of allowed words for fast lookup
    std::unordered_set<std::string> wordSet(words.begin(), words.end());
    
    // If target is not present, impossible
    if (wordSet.find(target) == wordSet.end()) {
        return 0;
    }
    
    // If start equals target, zero steps needed
    if (begin == target) {
        return 0;
    }
    
    // BFS setup: queue of (word, steps)
    std::queue<std::pair<std::string, int>> q;
    std::unordered_set<std::string> visited;
    q.push({begin, 0});
    visited.insert(begin);
    
    int wordLength = static_cast<int>(begin.size());
    
    while (!q.empty()) {
        auto [current, steps] = q.front();
        q.pop();
        
        // Generate all possible one-letter variations
        for (int pos = 0; pos < wordLength; ++pos) {
            char originalChar = current[pos];
            for (char ch = 'a'; ch <= 'z'; ++ch) {
                if (ch == originalChar) continue;
                std::string next = current;
                next[pos] = ch;
                
                // Skip if not in the allowed words or already visited
                if (wordSet.find(next) == wordSet.end() || visited.find(next) != visited.end()) {
                    continue;
                }
                
                // If we reached the target, return steps + 1
                if (next == target) {
                    return steps + 1;
                }
                
                visited.insert(next);
                q.push({next, steps + 1});
            }
        }
    }
    
    // No path found
    return 0;
}
// This is a classic shortest-path problem on an unweighted graph where each node is a word, and an edge exists between two words if they differ in exactly one character. A BFS (breadth-first search) starting from `begin` finds the minimum number of steps to reach `target` because BFS explores all nodes at distance 1, then distance 2, etc. Before BFS, check that `target` exists in `words`; if not, immediately return 0. Also, if `begin == target`, return 0 because no transformation is needed (even if `target` is in `words`). During BFS, use a queue of pair(word, steps) and a set of visited words (or a boolean vector indexed by positions in `words`). For each current word, generate every possible one-letter change (try all 26 letters at each position) and if the new word is in the `words` list and not visited, add it to the queue. If the new word equals `target`, return current steps + 1. Edge cases: duplicate words in `words` do not affect correctness because visited set prevents revisiting; if `target` not reachable, BFS exhausts and returns 0. Complexity: Let `L` be the length of each word and `N` be the number of words. For each visited word, we generate `26*L` candidate words and check membership in a set or hash set, so BFS O(N * 26 * L) time in the worst case if all words are visited, and O(N) space for queue and visited set. This is efficient for typical constraints (e.g., N ≤ 50, L ≤ 10).
