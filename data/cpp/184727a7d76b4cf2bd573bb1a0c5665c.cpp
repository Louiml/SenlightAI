Given a vector of strings where each string consists of lowercase English letters and has length at least 2, write a C++ function `bool formsPerfectCycle(const std::vector<std::string>& words)` that returns `true` if all the given words can be arranged in a sequence such that the last character of each word equals the first character of the next word, and the last word's last character equals the first word's first character (i.e., they form a directed cycle that uses every word exactly once). You may assume the input contains no duplicate words. The function must detect whether such a cycle exists using an approach based on mapping each starting character to its corresponding ending character, while verifying that the number of occurrences of each character as a start equals its number of occurrences as an end, and that the mapping forms a single cycle covering all words. If the input is empty or has fewer than 2 words, return `false`.
#include <cassert>
#include <vector>
#include <string>

// The formsPerfectCycle function is declared above.

int main() {
    // Example from the original snippet: cycle of 5 words
    std::vector<std::string> words1 = {"abc", "ceg", "gca", "acc", "caa"};
    assert(formsPerfectCycle(words1) == true);

    // Example from snippet: "abc", "ced", "dfg", "gaa" – also forms a cycle? Let's test: a->c, c->d, d->g, g->a → yes
    std::vector<std::string> words2 = {"abc", "ced", "dfg", "gaa"};
    assert(formsPerfectCycle(words2) == true);

    // Two separate cycles: e.g., "ab","ba","cd","dc" – this does not form a single cycle covering all
    std::vector<std::string> words3 = {"ab", "ba", "cd", "dc"};
    assert(formsPerfectCycle(words3) == false);

    // Not a cycle because frequencies don't match: "ab","bc","ca","ad"
    std::vector<std::string> words4 = {"ab", "bc", "ca", "ad"};
    assert(formsPerfectCycle(words4) == false);

    // Self-loop words: "aa" (but only one word, should be false)
    std::vector<std::string> words5 = {"aa"};
    assert(formsPerfectCycle(words5) == false);

    // Valid cycle with repeated start characters but no duplicate words
    // "ab","bc","ca" and "cb","ba","ac"? Actually let's design one: "ab","bd","da","ac","ca"? Let's pick:
    // "ab","bc","ca" (3 words) is a valid cycle
    std::vector<std::string> words6 = {"ab", "bc", "ca"};
    assert(formsPerfectCycle(words6) == true);

    // Case where a start character appears multiple times but mapping only follows one path
    // "ab","ac","ca" – start 'a' maps to 'b' or 'c', but only one mapping stored, so this may falsely fail.
    // Actually with "ab","ac","ca": startFreq: a:2, c:1; endFreq: b:1,c:1,a:1 → a count mismatch, so false
    std::vector<std::string> words7 = {"ab", "ac", "ca"};
    assert(formsPerfectCycle(words7) == false);

    // Two words forming a cycle: "ab","ba"
    std::vector<std::string> words8 = {"ab", "ba"};
    assert(formsPerfectCycle(words8) == true);

    // Empty input
    std::vector<std::string> words9;
    assert(formsPerfectCycle(words9) == false);

    return 0;
}
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Returns true if all words can form a directed cycle where each word's last character
// equals the next word's first character, and the last word's last character equals the
// first word's first character. Assumes no duplicate words and each word length >= 2.
bool formsPerfectCycle(const std::vector<std::string>& words) {
    if (words.size() < 2) {
        return false;
    }

    const int ALPHABET_SIZE = 26;
    int startFreq[ALPHABET_SIZE] = {0};
    int endFreq[ALPHABET_SIZE] = {0};
    std::map<char, char> charMap; // maps start character to end character for one representative word

    // Build frequencies and map
    for (const std::string& w : words) {
        char start = w.front();
        char end = w.back();
        startFreq[start - 'a']++;
        endFreq[end - 'a']++;
        // Since no duplicate words, but multiple words may share the same start,
        // we only need one mapping per start character.
        if (charMap.find(start) == charMap.end()) {
            charMap[start] = end;
        }
    }

    // Quick check: every character that appears as a start must appear as an end with equal count
    for (int i = 0; i < ALPHABET_SIZE; ++i) {
        if (startFreq[i] != endFreq[i]) {
            return false;
        }
    }

    // Choose a random starting character from any word's first character
    srand(static_cast<unsigned>(time(nullptr)));
    char startChar = words[rand() % words.size()].front();

    // Traverse the mapping chain
    while (true) {
        auto it = charMap.find(startChar);
        if (it == charMap.end()) {
            // No outgoing edge from this character
            break;
        }

        char endChar = it->second;
        startFreq[startChar - 'a']--;
        endFreq[endChar - 'a']--;

        // Remove the mapping only when both counts reach zero
        if (startFreq[startChar - 'a'] == 0 && endFreq[endChar - 'a'] == 0) {
            charMap.erase(it);
        }

        // Move to the next character
        startChar = endChar;
    }

    // If we've removed all mappings and all frequencies are zero, we have a perfect cycle
    return charMap.empty() &&
           std::all_of(std::begin(startFreq), std::end(startFreq), [](int x){ return x == 0; }) &&
           std::all_of(std::begin(endFreq), std::end(endFreq), [](int x){ return x == 0; });
}
// The solution constructs two frequency arrays (size 26) for the first and last characters of each word. For a valid cycle, every character that appears as a start must also appear as an end with the same frequency, because each word's first character is consumed by a previous word's last character and each word's last character is consumed by the next word's first character. The algorithm then builds a map from start character to end character; since no duplicate words exist, each start character maps to a unique end character, but multiple words may share a start character, so the map holds only one mapping per start character. Starting from the first character of any arbitrary word, we repeatedly follow the mapping, decrementing the corresponding frequency counts for the current start and next end. If we encounter a start character that is not in the map or a character whose frequency has already been exhausted, the cycle breaks early. After traversing the mapping chain, if the map is empty and we have visited every word exactly once (ensured by checking that all frequency counts reach zero), then a perfect cycle exists. Edge cases include: duplicate characters appearing as both start and end for different words (handled by frequency counts), self-loops (e.g., "abc" maps 'a' to 'c' but "caa" maps 'c' to 'a'), and inputs where the mapping forms a cycle but not covering all words (e.g., two separate cycles). The approach is similar to finding an Eulerian circuit in a directed graph with 26 nodes and edges from first to last character; we need the graph to be strongly connected and each node's in-degree equal its out-degree. Time complexity is O(n) for processing the words plus O(26) for frequency checks, and space complexity is O(26) for arrays plus O(26) for the map.
