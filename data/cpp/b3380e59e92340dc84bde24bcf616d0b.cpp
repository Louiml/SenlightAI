// Given a non-empty string `s` consisting of lowercase English letters, write a C++ function that returns a rearranged version of the string such that no two adjacent characters are the same. If such a rearrangement is impossible, return an empty string. The output need not be unique; any valid rearrangement is acceptable. For example, `"aab"` can become `"aba"`, but `"aaab"` has no valid rearrangement because the most frequent character appears more than `(length+1)/2` times.
// The core observation is that a valid rearrangement exists if and only if no character appears more than `ceil(n/2)` times, where `n` is the string length. If the maximum frequency exceeds this threshold, adjacent duplicates are unavoidable. To construct a valid arrangement, use a max-heap (priority queue) that always yields the character with the highest remaining frequency. Pop the most frequent character, append it to the result, decrement its count, and then push back the character that was popped in the previous iteration (if any). This "cool-down" strategy ensures that the same character is not placed consecutively. At each step, the previously used character is held aside and reinserted only after one other character has been written. If at any point the heap is empty but the result is incomplete, the task is impossible and an empty string is returned (though the frequency precondition already prevents this). Time complexity is O(n log k) where k is the number of distinct characters (at most 26), and space complexity is O(k) for the heap and frequency map.
#include <string>
#include <unordered_map>
#include <queue>
#include <utility>

// Rearrange the string so no two adjacent characters are equal.
// Return an empty string if impossible.
std::string reorganizeString(const std::string& s) {
    const int n = static_cast<int>(s.size());
    std::unordered_map<char, int> freq;
    
    // Count frequencies and immediately check feasibility.
    for (char c : s) {
        ++freq[c];
        if (freq[c] > (n + 1) / 2) {
            return "";
        }
    }
    
    // Max-heap ordered by frequency, then character for deterministic order.
    std::priority_queue<std::pair<int, char>> maxHeap;
    for (const auto& [ch, count] : freq) {
        maxHeap.push({count, ch});
    }
    
    std::string result;
    std::pair<int, char> previous = {0, '\0'};  // holds the last used character
    
    while (!maxHeap.empty()) {
        auto [count, ch] = maxHeap.top();
        maxHeap.pop();
        
        result.push_back(ch);
        --count;
        
        // If a previous character has remaining count, put it back now.
        if (previous.first > 0) {
            maxHeap.push(previous);
        }
        
        // Set this character as the new previous (with updated count).
        previous = {count, ch};
    }
    
    // Since the feasibility check ensures success, result length must be n.
    return (result.size() == static_cast<std::size_t>(n)) ? result : "";
}
#include <cassert>
#include <string>

// Function prototype; include the solution code above before this test.

int main() {
    // Basic examples from the prompt
    assert(reorganizeString("aab") == "aba");
    assert(reorganizeString("aaab") == "");
    
    // Single character
    assert(reorganizeString("a") == "a");
    assert(reorganizeString("aa") == "");
    
    // Already valid
    assert(reorganizeString("abc") == "abc");
    
    // All same character impossible beyond length 1
    assert(reorganizeString("aaaa") == "");
    
    // Two distinct with one more than the other
    assert(reorganizeString("aaabb") == "ababa");
    
    // Longer string with valid arrangement
    std::string result = reorganizeString("vvvlo");
    assert(result.size() == 5);
    for (std::size_t i = 1; i < result.size(); ++i) {
        assert(result[i] != result[i - 1]);
    }
    
    // Edge case with just two characters alternating
    assert(reorganizeString("abab") == "abab");
    
    // Many duplicates but still feasible
    std::string result2 = reorganizeString("aaabbb");
    assert(result2.size() == 6);
    for (std::size_t i = 1; i < result2.size(); ++i) {
        assert(result2[i] != result2[i - 1]);
    }
    
    return 0;
}
