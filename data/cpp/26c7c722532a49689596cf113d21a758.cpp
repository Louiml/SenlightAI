Write a C++ function named `findRepeatedDnaSequences` that takes a string `s` consisting only of uppercase letters from the set {A, C, G, T} and returns a vector of strings containing all 10-letter-long substrings that occur more than once in the input. The returned substrings should appear in ascending order of their first occurrence index in the original string, and each repeated substring should appear exactly once in the result. If there are no repeated 10-letter substrings, the function should return an empty vector. The function signature must be `std::vector<std::string> findRepeatedDnaSequences(const std::string& s)`.

#include <cassert>
#include <string>
#include <vector>

// Assume findRepeatedDnaSequences is declared above.

int main() {
    // Basic case with one repeated substring
    std::vector<std::string> r1 = findRepeatedDnaSequences("AAAAACCCCCAAAAACCCCCC");
    std::vector<std::string> expected1 = {"AAAAACCCCC", "CCCCCAAAAA"};
    assert(r1 == expected1);
    
    // Overlapping repeats
    std::vector<std::string> r2 = findRepeatedDnaSequences("AAAAAAAAAAA");
    std::vector<std::string> expected2 = {"AAAAAAAAAA"};
    assert(r2 == expected2);
    
    // No repeats
    std::vector<std::string> r3 = findRepeatedDnaSequences("ACGTACGTAC");
    assert(r3.empty());
    
    // Short string (less than 10)
    std::vector<std::string> r4 = findRepeatedDnaSequences("ACGT");
    assert(r4.empty());
    
    // Exactly 10 chars, no repeat
    std::vector<std::string> r5 = findRepeatedDnaSequences("ACGTACGTAC");
    assert(r5.empty());
    
    // Exactly 10 chars, single occurrence cannot repeat
    std::vector<std::string> r6 = findRepeatedDnaSequences("AAAAAAAAAA");
    assert(r6.empty());
    
    // String with one distinct repeated pattern
    std::vector<std::string> r7 = findRepeatedDnaSequences("CCCCCCCCCCCCCCCC");
    assert(r7 == std::vector<std::string>({"CCCCCCCCCC"}));
    
    // More than two occurrences, still listed once
    std::vector<std::string> r8 = findRepeatedDnaSequences("AAAAAAAAAAAAAA");
    assert(r8 == std::vector<std::string>({"AAAAAAAAAA"}));
    
    // Case with multiple repeats in non-overlapping positions
    std::vector<std::string> r9 = findRepeatedDnaSequences("TTTTTCCCCCCTTTTTCCCCC");
    std::vector<std::string> expected9 = {"TTTTTCCCCC", "CCCCCTTTTT"};
    assert(r9 == expected9);
    
    // Empty string
    std::vector<std::string> r10 = findRepeatedDnaSequences("");
    assert(r10.empty());
    
    return 0;
}

#include <string>
#include <vector>
#include <unordered_map>

// Returns all 10-letter substrings that occur more than once in s.
// The result is ordered by the first occurrence index of each repeated substring.
std::vector<std::string> findRepeatedDnaSequences(const std::string& s) {
    std::vector<std::string> result;
    int n = static_cast<int>(s.length());
    if (n < 10) {
        return result;
    }
    
    std::unordered_map<std::string, int> count;
    for (int i = 0; i <= n - 10; ++i) {
        std::string sub = s.substr(i, 10);
        ++count[sub];
        if (count[sub] == 2) {
            result.push_back(sub);
        }
    }
    return result;
}

// The solution uses an unordered map to count occurrences of every 10-character substring in the input. Starting from each index `i` from 0 to `s.length() - 10`, extract the substring of length 10. Increment its count in the map. If the count becomes exactly 2, this is the first time we observe a duplicate, so push the substring into the result vector. This approach ensures each repeated substring is added only once because we only add when the count transitions from 1 to 2; subsequent occurrences (count > 2) are ignored. Edge cases include: if the input length is less than 10, return an empty vector immediately. Also, the input may contain overlapping repeats; the loop covers all starting positions, so overlapping substrings are handled correctly. Time complexity is O(n * 10) = O(n) because each substring extraction takes O(10) time. Space complexity is O(n * 10) in the worst case (all substrings are distinct) for storing the map, but in practice it’s bounded by the number of possible distinct 10‑letter DNA sequences, which is 4^10 ≈ 1,048,576.
