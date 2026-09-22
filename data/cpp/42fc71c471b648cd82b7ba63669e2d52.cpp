// Write a C++ function `rearrangeNoAdjacentDuplicates` that takes a string `S` consisting of lowercase English letters and returns a rearranged string such that no two adjacent characters are the same. If such a rearrangement is impossible, return an empty string. The function should prioritize producing a valid arrangement; any valid arrangement is acceptable. For example, for input `"aab"`, possible outputs include `"aba"`; for `"aaab"`, the output must be `""` because no rearrangement avoids adjacent identical characters.
The problem is a classic "rearrange string to avoid adjacent duplicates" task. The main algorithm uses a max-heap (priority queue) keyed by character frequency. First, count frequencies of all characters using an unordered map. Then push each character and its count into a max-heap, where the character with the highest frequency is popped first. While the heap has more than one element, pop the top two entries (the two most frequent remaining characters), append their characters to the result string, decrement their counts, and push any entry with count > 0 back. This greedy approach ensures that we always place the most frequent characters as far apart as possible. After the loop, if one entry remains in the heap, its count must be exactly 1 to append it without creating adjacency; otherwise, return `""`. If the heap is empty, the rearrangement succeeded. Edge cases: empty string returns empty; single character with count 1 returns that character; single character with count > 1 returns empty; string where the most frequent character count exceeds half the string length (ceil) is impossible. Time complexity is O(n log k) where n is string length and k is the number of distinct characters (at most 26), so effectively O(n). Space complexity is O(k) for the heap and map.
#include <string>
#include <unordered_map>
#include <queue>
#include <utility>

// Reorders a lowercase string so no two adjacent characters are equal.
// Returns an empty string if impossible.
std::string rearrangeNoAdjacentDuplicates(const std::string& S) {
    if (S.empty()) return "";

    // Count frequencies
    std::unordered_map<char, int> freq;
    for (char c : S) {
        freq[c]++;
    }

    // Max-heap by frequency, then by character for deterministic ordering
    std::priority_queue<std::pair<int, char>> pq;
    for (const auto& entry : freq) {
        pq.push({entry.second, entry.first});
    }

    std::string result;
    while (pq.size() > 1) {
        auto first = pq.top(); pq.pop();
        auto second = pq.top(); pq.pop();

        result += first.second;
        result += second.second;

        first.first--;
        second.first--;

        if (first.first > 0) pq.push(first);
        if (second.first > 0) pq.push(second);
    }

    // At most one character remains with count possibly > 1
    if (!pq.empty()) {
        if (pq.top().first > 1) {
            return ""; // impossible
        }
        result += pq.top().second;
    }

    return result;
}
#include <cassert>
#include <string>

// Function declaration (included for clarity)
std::string rearrangeNoAdjacentDuplicates(const std::string& S);

bool hasNoAdjacentDuplicates(const std::string& s) {
    for (size_t i = 1; i < s.size(); ++i) {
        if (s[i] == s[i-1]) return false;
    }
    return true;
}

int main() {
    // Test 1: Basic rearrangement
    std::string out1 = rearrangeNoAdjacentDuplicates("aab");
    assert(out1 == "aba" || out1 == "baa" || out1 == "aab" && !hasNoAdjacentDuplicates(out1));
    assert(hasNoAdjacentDuplicates(out1));

    // Test 2: Impossible
    assert(rearrangeNoAdjacentDuplicates("aaab") == "");

    // Test 3: Single character
    assert(rearrangeNoAdjacentDuplicates("a") == "a");

    // Test 4: Empty
    assert(rearrangeNoAdjacentDuplicates("") == "");

    // Test 5: All distinct
    std::string out5 = rearrangeNoAdjacentDuplicates("abc");
    assert(out5.size() == 3);
    assert(hasNoAdjacentDuplicates(out5));

    // Test 6: Long test with frequent character
    std::string out6 = rearrangeNoAdjacentDuplicates("aaabbb");
    assert(hasNoAdjacentDuplicates(out6));
    assert(out6.size() == 6);

    // Test 7: Frequency exactly half
    std::string out7 = rearrangeNoAdjacentDuplicates("aaabb");
    assert(out7.size() == 5);
    assert(hasNoAdjacentDuplicates(out7));

    // Test 8: One char repeated more than half
    assert(rearrangeNoAdjacentDuplicates("aaaaabc") == "");

    // Test 9: Two distinct with equal high counts
    std::string out9 = rearrangeNoAdjacentDuplicates("aabb");
    assert(out9.size() == 4);
    assert(hasNoAdjacentDuplicates(out9));

    // Test 10: Random small string
    std::string out10 = rearrangeNoAdjacentDuplicates("xxy");
    assert(out10.size() == 3);
    assert(hasNoAdjacentDuplicates(out10));

    return 0;
}
