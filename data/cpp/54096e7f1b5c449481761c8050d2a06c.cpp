// Write a C++ function `bool areSentencesSimilar(const std::string& sentence1, const std::string& sentence2)` that determines whether two sentences are "similar" under the following rule: two sentences are considered similar if one can be obtained from the other by inserting a (possibly empty) contiguous block of words anywhere in the other sentence. Words are maximal sequences of non-whitespace characters, and sentences may contain leading, trailing, or multiple consecutive spaces. The function should return `true` if the sentences are similar (i.e., one is a subsequence of the other formed by removing a single contiguous block from the middle, prefix, or suffix), and `false` otherwise. For example, `"Hello Jane"` and `"Hello my name is Jane"` are similar because the second can be obtained by inserting `"my name is"` in the middle; `"Hello Jane"` and `"Hello my name"` are not similar. Empty sentences are allowed and considered similar to any sentence (since inserting the whole sentence is a valid operation). The solution must handle duplicate words and varying whitespace.
#include <cassert>

int main() {
    // Both empty
    assert(areSentencesSimilar("", "") == true);
    // One empty
    assert(areSentencesSimilar("", "hello world") == true);
    assert(areSentencesSimilar("hello world", "") == true);
    // Identical sentences
    assert(areSentencesSimilar("hello world", "hello world") == true);
    // Insertion in the middle
    assert(areSentencesSimilar("Hello Jane", "Hello my name is Jane") == true);
    // Insertion at the beginning (prefix)
    assert(areSentencesSimilar("world", "hello world") == true);
    // Insertion at the end (suffix)
    assert(areSentencesSimilar("hello", "hello world") == true);
    // Not similar because missing words in the middle
    assert(areSentencesSimilar("Hello Jane", "Hello my name") == false);
    // Different words
    assert(areSentencesSimilar("apple pie", "banana bread") == false);
    // Extra whitespace handling
    assert(areSentencesSimilar("  one   two  ", "one extra two") == true);
    // Duplicate words at boundaries
    assert(areSentencesSimilar("a a b", "a a a b") == true);
    // Reversed order not similar
    assert(areSentencesSimilar("a b c", "c b a") == false);
    return 0;
}
#include <string>
#include <sstream>
#include <deque>
#include <iterator>

bool areSentencesSimilar(const std::string& sentence1, const std::string& sentence2) {
    // Split sentence1 into words.
    std::istringstream iss1(sentence1);
    std::deque<std::string> words1(
        (std::istream_iterator<std::string>(iss1)),
        std::istream_iterator<std::string>());

    // Split sentence2 into words.
    std::istringstream iss2(sentence2);
    std::deque<std::string> words2(
        (std::istream_iterator<std::string>(iss2)),
        std::istream_iterator<std::string>());

    // Match the common prefix from the front.
    while (!words1.empty() && !words2.empty() && words1.front() == words2.front()) {
        words1.pop_front();
        words2.pop_front();
    }

    // Match the common suffix from the back.
    while (!words1.empty() && !words2.empty() && words1.back() == words2.back()) {
        words1.pop_back();
        words2.pop_back();
    }

    // The sentences are similar if one of the remaining word lists is empty.
    return words1.empty() || words2.empty();
}
// The core idea is to split both strings into deques of words using `std::istringstream` and `std::istream_iterator`. Then, compare words from the front (prefix) while they match, and pop matched words from both deques. After that, compare words from the back (suffix) while they match, again popping matched words. At the end, the sentences are similar if at least one of the deques becomes empty. This works because any valid insertion of a contiguous block into one sentence to obtain the other leaves an unchanged common prefix and suffix; the block being inserted is exactly the middle portion that remains in the longer deque after removing the matched prefix and suffix. Edge cases include: (1) both sentences empty → both deques empty → return true; (2) one empty, other non‑empty → during prefix loop, the empty deque stops immediately; suffix loop also stops; empty deque remains empty → return true; (3) both same words → prefix loop empties both → return true; (4) sentences with extra whitespace → `istringstream` handles it naturally; (5) duplicate words at boundaries — the algorithm is correct because it greedily matches from both ends, which is sufficient for a single contiguous insertion (a known property for this problem). Time complexity is O(n + m) where n and m are the numbers of words in the sentences, since we traverse each word at most once from the front and once from the back. Space complexity is O(n + m) for storing the deques.
