Given a string containing only lowercase English letters, write a C++ function that returns the maximum number of disjoint pairs of identical characters that can be formed from the characters in the string. Each character can be used at most once, and a pair consists of two occurrences of the same letter. For example, from the string "aabbcc", three disjoint pairs can be formed (one for each distinct letter), while from "aaabbb", two disjoint pairs can be formed (one pair of 'a' and one pair of 'b', leaving one unused 'a' and one unused 'b'). The function should take a `std::string` as input and return an `int`.
The solution counts the frequency of each character in the input string using a hash map (or an array of size 26 since the input is restricted to lowercase English letters). For each character, the maximum number of disjoint pairs that can be formed from its occurrences is `floor(frequency / 2)`, which is equivalent to `frequency / 2` in integer arithmetic. The total number of pairs is the sum of these values across all distinct characters. This approach directly addresses the requirement that each character can be used at most once and pairs must consist of identical letters. Edge cases include strings with all unique characters (result 0), strings with odd frequencies (the leftover character is simply ignored), and empty strings (result 0). The time complexity is O(n) where n is the length of the string, and the space complexity is O(1) if using a fixed-size array, or O(k) where k is the number of distinct characters if using a hash map.
#include <string>
#include <array>

// Returns the maximum number of disjoint pairs of identical characters that can be formed.
int maxIdenticalPairs(const std::string& s) {
    std::array<int, 26> freq{};
    for (char c : s) {
        ++freq[c - 'a'];
    }
    
    int totalPairs = 0;
    for (int count : freq) {
        totalPairs += count / 2;
    }
    return totalPairs;
}
#include <cassert>
#include <string>

int maxIdenticalPairs(const std::string& s);

int main() {
    // Basic cases
    assert(maxIdenticalPairs("aabbcc") == 3);
    assert(maxIdenticalPairs("aaabbb") == 2);
    assert(maxIdenticalPairs("abc") == 0);
    assert(maxIdenticalPairs("") == 0);
    
    // Odd frequency leftovers
    assert(maxIdenticalPairs("aaa") == 1);
    assert(maxIdenticalPairs("aabbb") == 2); // 'a' gives 1 pair, 'b' gives 1 pair
    
    // All same characters
    assert(maxIdenticalPairs("aaaaaa") == 3);
    assert(maxIdenticalPairs("a") == 0);
    
    // Mixed with large counts
    assert(maxIdenticalPairs("aaabbbccc") == 3);
    assert(maxIdenticalPairs("zzzzzzzz") == 4);
}
