Write a C++ function named `longestWord` that reads text from standard input using `cin >>` (which extracts whitespace-separated tokens) and returns the longest token found in the input. If multiple tokens share the same maximum length, return the first one encountered. Your function should not store all tokens; it should process the stream incrementally. Assume the input may contain any non-whitespace characters (including punctuation and numbers) and may be empty (in which case return an empty string). Do not include a `main` function in your solution—just the free function.
// The solution uses a simple streaming approach: repeatedly read a token via `cin >> text`. Initialize `longestWord` as an empty string. For each token read, compare its length (`text.size()`) with the length of the current `longestWord`; if the new token is strictly longer, replace `longestWord` with it. This ensures the first token of maximum length is kept (since we only replace on strictly greater length). Edge cases: empty input yields no tokens and the function returns an empty string; a single token becomes the longest; tokens with equal length don’t replace the existing longest. Time complexity is O(n) where n is the total number of characters in all tokens, since we scan each character once when extracting. Space complexity is O(m) where m is the length of the longest token (or the current token being read), but we do not store all tokens—only the current one and the longest so far.
#include <iostream>
#include <string>

// Reads whitespace-separated tokens from standard input and returns the longest one.
// If multiple tokens share the maximum length, returns the first encountered.
// Returns an empty string if no tokens are read.
std::string longestWord() {
    std::string current;
    std::string longest;
    while (std::cin >> current) {
        if (current.size() > longest.size()) {
            longest = current;
        }
    }
    return longest;
}
#include <cassert>
#include <sstream>
#include <string>

// Forward declaration of the solution function (assumed defined above in actual test).
std::string longestWord();

int main() {
    // Test with various inputs by temporarily redirecting std::cin.
    {
        std::istringstream input("the quick brown fox jumps");
        auto old_buf = std::cin.rdbuf(input.rdbuf());
        assert(longestWord() == "quick");
        std::cin.rdbuf(old_buf);
    }
    {
        std::istringstream input("");
        auto old_buf = std::cin.rdbuf(input.rdbuf());
        assert(longestWord() == "");
        std::cin.rdbuf(old_buf);
    }
    {
        std::istringstream input("a bb ccc dddd eeeee");
        auto old_buf = std::cin.rdbuf(input.rdbuf());
        assert(longestWord() == "eeeee");
        std::cin.rdbuf(old_buf);
    }
    {
        std::istringstream input("same same same");
        auto old_buf = std::cin.rdbuf(input.rdbuf());
        assert(longestWord() == "same"); // first occurrence
        std::cin.rdbuf(old_buf);
    }
    {
        std::istringstream input("one  two   three   four");
        auto old_buf = std::cin.rdbuf(input.rdbuf());
        assert(longestWord() == "three");
        std::cin.rdbuf(old_buf);
    }
    {
        std::istringstream input("a bb ccc dddd");
        auto old_buf = std::cin.rdbuf(input.rdbuf());
        assert(longestWord() == "dddd");
        std::cin.rdbuf(old_buf);
    }
    return 0;
}
