// You are given two vectors of strings, `first` and `second`, each of the same length `n` (with `n ≥ 1`). Each position `i` represents a person who is interested in exactly two topics: `first[i]` and `second[i]`. Topics are case-sensitive and may repeat across the vectors. Write a C++ function named `bestInvitation` that takes the two vectors as inputs (by const reference) and returns the maximum number of people who share at least one common topic. In other words, find the topic that appears most frequently across all `first` and `second` entries, counting each person only once per topic (even if a person lists the same topic twice, which is guaranteed not to happen because the two topics for each person are distinct in the original problem, but your code should still handle duplicates gracefully by counting the topic once per occurrence). The return value is an integer representing the largest count of people who have that topic in their list of two interests.

The problem reduces to counting, for each distinct topic, how many persons list it in either of their two positions. A natural approach is to use a hash map (e.g., `std::map` or `std::unordered_map`) that maps each topic string to an integer count. First, iterate over all positions `i` from 0 to n-1, and for each topic in `first[i]` and `second[i]`, increment the count in the map. Because each person contributes to the count for each of their two distinct topics, and a topic appearing in multiple people gets the sum of contributions. Then, scan the map to find the maximum count. Edge cases: if both vectors are empty (though specification says n ≥ 1, handle gracefully by returning 0), if a topic appears only in one position, or if the same topic appears as both first and second for different people, the counting still works. Complexity: time is O(n * average string length) for hash operations, and O(n) for the map traversal, so overall O(n * L) where L is the average string length; space is O(number of distinct topics) which is at most 2n. Using `std::map` gives O(n log n) time, while `std::unordered_map` gives average O(n). For simplicity and determinism, use `std::map` or `unordered_map`. The function should be `const`-correct by taking const references and not modifying inputs.

#include <string>
#include <vector>
#include <map>
#include <algorithm>

// Given two parallel vectors of topics (each person has exactly two interests),
// return the maximum number of people who share at least one common topic.
int bestInvitation(const std::vector<std::string>& first, const std::vector<std::string>& second) {
    // Count how many people list each topic (each person lists two distinct topics).
    std::map<std::string, int> topicCount;
    const size_t n = first.size();
    for (size_t i = 0; i < n; ++i) {
        ++topicCount[first[i]];
        ++topicCount[second[i]];
    }
    
    // Find the highest frequency among all topics.
    int maxInvited = 0;
    for (const auto& entry : topicCount) {
        maxInvited = std::max(maxInvited, entry.second);
    }
    return maxInvited;
}

#include <cassert>
#include <string>
#include <vector>

int bestInvitation(const std::vector<std::string>& first, const std::vector<std::string>& second);

int main() {
    // Example from the original snippet: 4 people, each with two interests.
    std::vector<std::string> f1 = {"fishing", "gardening", "swimming", "fishing"};
    std::vector<std::string> s1 = {"gardening", "reading", "fishing", "travel"};
    assert(bestInvitation(f1, s1) == 3); // "fishing" appears 3 times, "gardening" 2 times.

    // Single person, two distinct topics.
    std::vector<std::string> f2 = {"a"};
    std::vector<std::string> s2 = {"b"};
    assert(bestInvitation(f2, s2) == 1);

    // All people share the same first topic.
    std::vector<std::string> f3 = {"x", "x", "x"};
    std::vector<std::string> s3 = {"y", "z", "w"};
    assert(bestInvitation(f3, s3) == 3);

    // No repeated topics, each person unique.
    std::vector<std::string> f4 = {"a", "b"};
    std::vector<std::string> s4 = {"c", "d"};
    assert(bestInvitation(f4, s4) == 1);

    // Mixed duplicates, topic appears in both first and second positions.
    std::vector<std::string> f5 = {"p", "q", "r"};
    std::vector<std::string> s5 = {"q", "r", "p"};
    assert(bestInvitation(f5, s5) == 2); // each topic appears twice.

    // Empty vectors (edge case, though problem says n ≥ 1).
    std::vector<std::string> f6;
    std::vector<std::string> s6;
    assert(bestInvitation(f6, s6) == 0);

    // Long strings with case sensitivity.
    std::vector<std::string> f7 = {"Alpha", "beta", "Alpha"};
    std::vector<std::string> s7 = {"gamma", "Alpha", "DELTA"};
    assert(bestInvitation(f7, s7) == 2); // "Alpha" appears in f7[0], f7[2], and s7[1] = 3? Let's count: f7 has Alpha at index 0 and 2, s7 has Alpha at index 1 → total 3.
    assert(bestInvitation(f7, s7) == 3); // Corrected: "Alpha" appears 3 times.

    return 0;
}
