// Write a C++ function named `countAndLocatePattern` that, given a `std::string` containing only uppercase letters 'A', 'C', 'G', and 'T' (representing a DNA sequence) and two separate patterns `leftPattern` and `rightPattern` (also consisting only of these characters), returns a `std::pair<size_t, std::vector<size_t>>` where the first member is the number of occurrences of the concatenated pattern (i.e., `leftPattern` immediately followed by `rightPattern`) in the genome, and the second member is a vector of the 0-based starting positions of all such occurrences. The function must handle overlapping occurrences correctly (e.g., in "AAAA", pattern "AA"+"AA" occurs at positions 0 and 1). If the left or right pattern is empty, treat that side as a zero-length pattern, meaning the search is for the non-empty side alone. The genome length can be up to 10,000 characters, and pattern lengths up to 1,000 each. The function should be efficient enough to handle this size, and must be implemented using a suffix-automaton-like or FM-index-like approach? No—implement a straightforward sliding window or string search approach that is correct and efficient. Use the C++ standard library only.
#include <cassert>
#include <string>
#include <vector>
#include <utility>

// Forward declaration (solution function is defined above in the actual file)
std::pair<size_t, std::vector<size_t>> countAndLocatePattern(const std::string&, const std::string&, const std::string&);

int main() {
    // Basic test from the original snippet: genome "ATCGATCGAAGGCTAGCTAGCTAAGGGA"
    // Combined "AAGG" occurs at positions 8 and 22.
    std::string genome = "ATCGATCGAAGGCTAGCTAGCTAAGGGA";
    auto result1 = countAndLocatePattern(genome, "AA", "GG");
    assert(result1.first == 2);
    assert(result1.second == std::vector<size_t>({8, 22}));

    // Overlapping occurrences: genome "AAAAA", pattern "AA" + "AA" = "AAAA"
    // Occurs at positions 0 and 1.
    auto result2 = countAndLocatePattern("AAAAA", "AA", "AA");
    assert(result2.first == 2);
    assert(result2.second == std::vector<size_t>({0, 1}));

    // No occurrences
    auto result3 = countAndLocatePattern("ACGTACGT", "CC", "TT");
    assert(result3.first == 0);
    assert(result3.second.empty());

    // One pattern empty: left empty, right "GT" in "AGTGTA" -> positions 1 and 4
    auto result4 = countAndLocatePattern("AGTGTA", "", "GT");
    assert(result4.first == 2);
    assert(result4.second == std::vector<size_t>({1, 4}));

    // Both patterns empty -> count 0
    auto result5 = countAndLocatePattern("ACGT", "", "");
    assert(result5.first == 0);
    assert(result5.second.empty());

    // Exact full match
    auto result6 = countAndLocatePattern("ATGC", "ATG", "C");
    assert(result6.first == 1);
    assert(result6.second == std::vector<size_t>({0}));

    // Pattern longer than genome -> no matches
    auto result7 = countAndLocatePattern("AC", "ACG", "T");
    assert(result7.first == 0);
    assert(result7.second.empty());

    // Multiple overlapping at start
    auto result8 = countAndLocatePattern("GGGG", "G", "GG");
    // Combined "GGG" occurs at 0 and 1? Check: genome "GGGG", pattern "GGG" -> positions 0 and 1
    assert(result8.first == 2);
    assert(result8.second == std::vector<size_t>({0, 1}));

    return 0;
}
#include <string>
#include <vector>
#include <utility>
#include <algorithm>

// Count and locate all occurrences of leftPattern+rightPattern in genome.
// Returns a pair: first = count, second = vector of starting positions.
std::pair<size_t, std::vector<size_t>> countAndLocatePattern(
    const std::string& genome,
    const std::string& leftPattern,
    const std::string& rightPattern)
{
    std::string combined = leftPattern + rightPattern;
    std::vector<size_t> positions;
    size_t count = 0;

    if (combined.empty()) {
        return {0, {}};
    }

    size_t pos = 0;
    while (true) {
        // Find next occurrence starting from pos
        size_t found = genome.find(combined, pos);
        if (found == std::string::npos) {
            break;
        }
        positions.push_back(found);
        ++count;
        // Continue from found+1 to allow overlapping matches
        pos = found + 1;
    }

    return {count, std::move(positions)};
}
// The task reduces to finding all occurrences of a combined pattern `P = leftPattern + rightPattern` in the genome string `S`. The naive approach is to build the combined string and scan through the genome using `std::string::find` in a loop, or use a sliding window with `std::string_view` (C++17) to compare substrings. To handle overlapping occurrences, after finding a match at position `pos`, the next search should start at `pos + 1`, not `pos + P.length()`. Edge cases: if both patterns are empty, the result should be a count of 0 and an empty vector (since zero-length pattern matches are undefined—but to be safe, we can return count 0). If one pattern is empty, just search for the other. The solution uses `std::search` or manual `find` loop. Time complexity is O(n * m) in the worst case, where n is genome length and m is combined pattern length, but with typical DNA sequences it's much faster. Space complexity is O(m) for the combined pattern string, and O(k) for the result vector where k is number of occurrences. Since the genome is at most 10,000 and pattern at most 2,000, this is acceptable. We ensure `const` correctness by taking genome and patterns as `const std::string&` and returning by value. We'll use `std::pair<size_t, std::vector<size_t>>` as the return type.
