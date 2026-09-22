// Write a C++ function `std::vector<int> trigramPruneCounts(const std::vector<std::string>& rules, const std::string& query)` that, given a list of regex-like patterns and a query string, simulates a simplified trigram index to determine whether the query is "definitely out" for each pattern. A pattern is treated as a sequence of characters where: `\` escapes the next character (so `\a` counts `a` as a literal), `.` and `*` reset the current trigram accumulation and do not contribute to any trigram, advanced metacharacters `()^$|+?[]\{}` cause the pattern to be "defeated" (meaning it can never be pruned), and backslash followed by a digit `1`-`9` also defeats the pattern. For each non-defeated pattern, collect all distinct trigrams (3 consecutive literal characters, where escaped characters count as literals and `.`/`*` break the sequence) that appear at most 4 times across the index. If a pattern has fewer than one such trigram, it is defeated. For each pattern, record the required count of its distinct trigrams (i.e., how many of its own trigrams it has). Then, scan the query string and for each position where a trigram appears that is in the index, increment a per-pattern counter. If any pattern's counter reaches the required count, that pattern is not definitely out; otherwise it is definitely out. Return a vector of booleans (as `int` 0/1) for each pattern in input order, where 1 means the query is definitely out (i.e., no match possible) and 0 means it is not definitely out. If a pattern is defeated, always return 0 for it (cannot prune). The query string can contain any ASCII characters, and trigram computation uses the same escaping and `.`/`*` reset rules as the patterns, but advanced metacharacters in the query are treated as ordinary literal characters (no defeat).
The core idea is to build an index of trigrams from the patterns. For each pattern, we process it character by character. We maintain a buffer of the last up to three literal characters. When we encounter a backslash, we set a flag so the next character is treated literally (and if the next character is a digit 1-9, the pattern is defeated). If we encounter `.` or `*` (and not escaped), we reset the trigram buffer. If we encounter any advanced metacharacter (and not escaped), the pattern is defeated and we stop processing it. For every position where we have at least three accumulated characters, we compute a 24-bit trigram value (e.g., by shifting and masking). We maintain a global map from trigram value to a list of pattern indices. We only add a pattern to a trigram's list if the list currently has fewer than 4 patterns (to limit the index size). For each pattern, we also count how many distinct trigrams it contributed (using a `std::set` to avoid duplicates). If this count is zero, the pattern is defeated. Otherwise, we store this required count for the pattern. After processing all patterns, we scan the query string, again maintaining a trigram buffer with the same reset rules (but no defeat conditions). For each trigram present in the index, we iterate through the associated pattern indices and increment their current count. If any pattern's current count reaches its required count, we mark it as not definitely out (return 0). At the end, patterns that never reached the required count are definitely out (return 1). Important edge cases: patterns shorter than 3 literals, escaped metacharacters like `\.` (treated as literal), patterns with multiple distinct trigrams, patterns that share trigrams, and the index limit of 4 patterns per trigram (additional patterns are simply not added and are treated as having fewer trigrams, which could potentially cause them to be considered definitely out incorrectly if they rely on a popular trigram; but per the spec, that is acceptable). Time complexity is O(T + Q*I) where T is total pattern literal characters, Q is query length, and I is the average number of patterns per trigram (bounded by 4 per trigram). Space complexity is O(T) for the index and pattern storage.
#include <vector>
#include <string>
#include <unordered_map>
#include <set>

// Simplified trigram-based pruning: returns 1 if query is definitely out for each pattern, else 0.
std::vector<int> trigramPruneCounts(const std::vector<std::string>& rules, const std::string& query) {
    // Helper to check advanced metacharacters (same as in the snippet).
    auto isAdvancedMetachar = [](char c) {
        return c == '(' || c == ')' || c == '^' || c == '$' || c == '|' ||
               c == '+' || c == '?' || c == '[' || c == ']' || c == '\\' ||
               c == '{' || c == '}';
    };

    // Index from trigram value to list of rule indices.
    std::unordered_map<unsigned, std::vector<size_t>> index;
    // Required distinct trigram count for each rule.
    std::vector<int> requiredCounts;
    // Whether a rule is defeated (cannot be pruned).
    std::vector<bool> defeated;

    for (const auto& rule : rules) {
        if (defeated.size() < requiredCounts.size() + 1) {
            // First time; initialize structures.
            requiredCounts.push_back(0);
            defeated.push_back(false);
        }
        size_t ruleIdx = defeated.size() - 1;
        // Reset state for this rule.
        bool esc = false;
        unsigned tri = 0;
        int len = 0;
        std::set<unsigned> seenTrigrams;
        int distinctTrigrams = 0;

        for (char ch : rule) {
            if (!esc) {
                if (ch == '\\') {
                    esc = true;
                    continue;
                }
                if (isAdvancedMetachar(ch)) {
                    defeated[ruleIdx] = true;
                    break;
                }
                if (ch == '.' || ch == '*') {
                    tri = 0;
                    len = 0;
                    continue;
                }
            }
            if (esc && ch >= '1' && ch <= '9') {
                defeated[ruleIdx] = true;
                break;
            }
            esc = false;
            tri = ((tri << 8) + static_cast<unsigned char>(ch)) & 0xFFFFFF;
            len++;
            if (len < 3) continue;
            // Limit index size: only add if trigram has fewer than 4 rules.
            if (index[tri].size() >= 4) continue;
            // Count distinct trigram per rule.
            if (seenTrigrams.insert(tri).second) {
                distinctTrigrams++;
                index[tri].push_back(ruleIdx);
            }
        }

        if (defeated[ruleIdx]) {
            requiredCounts[ruleIdx] = 0; // Not used, but keep consistent.
            continue;
        }
        if (distinctTrigrams == 0) {
            defeated[ruleIdx] = true;
            requiredCounts[ruleIdx] = 0;
        } else {
            requiredCounts[ruleIdx] = distinctTrigrams;
        }
    }

    // Now process the query.
    std::vector<int> result(rules.size(), 1); // Default: definitely out.
    // For each rule that is not defeated, we may change result to 0.
    std::vector<int> currentCounts(requiredCounts.size(), 0);

    // Scan query with same trigram logic.
    unsigned qtri = 0;
    int qlen = 0;
    bool qesc = false;
    for (char ch : query) {
        if (!qesc && ch == '\\') {
            qesc = true;
            continue;
        }
        if (!qesc && (ch == '.' || ch == '*')) {
            qtri = 0;
            qlen = 0;
            qesc = false;
            continue;
        }
        qesc = false;
        qtri = ((qtri << 8) + static_cast<unsigned char>(ch)) & 0xFFFFFF;
        qlen++;
        if (qlen < 3) continue;
        auto it = index.find(qtri);
        if (it == index.end()) continue;
        for (size_t rIdx : it->second) {
            if (defeated[rIdx]) continue; // Defeated rules are never pruned.
            currentCounts[rIdx]++;
            if (currentCounts[rIdx] >= requiredCounts[rIdx]) {
                // Not definitely out.
                result[rIdx] = 0;
            }
        }
    }

    // For defeated rules, result should be 0.
    for (size_t i = 0; i < rules.size(); ++i) {
        if (defeated[i]) result[i] = 0;
    }

    return result;
}
#include <cassert>
#include <vector>
#include <string>

// The solution function is declared above (or in a header).
// Provide a main that runs assertions.

int main() {
    // Basic case: two patterns, one matches trigram in query.
    std::vector<std::string> rules1 = {"abc", "abd"};
    // "abc" has trigram 0x616263, "abd" has 0x616264. Query "xabc" contains "abc".
    // For "abc": required=1, query contains trigram => not out (0).
    // For "abd": query does not contain its trigram => out (1).
    auto res1 = trigramPruneCounts(rules1, "xabc");
    assert(res1.size() == 2);
    assert(res1[0] == 0); // abc is present
    assert(res1[1] == 1); // abd absent

    // Pattern with escaped metacharacter: "a\\.c" (literal 'a', '.', 'c')
    // Trigram: 'a', '.', 'c'. Query "a.c" contains that.
    std::vector<std::string> rules2 = {"a\\.c"};
    auto res2 = trigramPruneCounts(rules2, "a.c");
    assert(res2.size() == 1);
    assert(res2[0] == 0); // not out

    // Pattern with '.' reset: "ab.de" gives trigrams "ab?" none because '.' resets.
    // Actually "ab" then '.' resets, then "de" len<3, so no trigram => defeated => 0.
    std::vector<std::string> rules3 = {"ab.de"};
    auto res3 = trigramPruneCounts(rules3, "zabdex");
    assert(res3.size() == 1);
    assert(res3[0] == 0); // defeated, always 0

    // Pattern with repeated trigram: "abcabc" has distinct trigrams "abc","bca","cab"
    // Query "abcabc" contains all three, so not out.
    std::vector<std::string> rules4 = {"abcabc"};
    auto res4 = trigramPruneCounts(rules4, "abcabc");
    assert(res4.size() == 1);
    assert(res4[0] == 0);

    // Query does not contain required trigrams: pattern "xyz", query "abc" -> out (1).
    std::vector<std::string> rules5 = {"xyz"};
    auto res5 = trigramPruneCounts(rules5, "abc");
    assert(res5.size() == 1);
    assert(res5[0] == 1);

    // Empty rules: result empty.
    std::vector<std::string> rules6 = {};
    auto res6 = trigramPruneCounts(rules6, "abc");
    assert(res6.empty());

    // Multiple rules sharing a trigram, index limit not hit (<=4).
    std::vector<std::string> rules7 = {"abc", "abcd", "abce"};
    // Trigram "abc" shared by first, second? Actually "abcd" has "abc","bcd"; "abce" has "abc","bce".
    // Query "abc" contains "abc": first needs 1, second needs 2 (but query only has "abc" once, so not all), third needs 2.
    // So first -> 0, second -> 1 (since only 1/2), third -> 1.
    auto res7 = trigramPruneCounts(rules7, "abc");
    assert(res7.size() == 3);
    assert(res7[0] == 0);
    assert(res7[1] == 1);
    assert(res7[2] == 1);

    // Query with escaped sequences: pattern "a\\b" (literal 'a','\','b'? Actually '\\' is backslash, so pattern has 'a', then '\' escaped? Let's see: string "a\\b" means a, backslash, b. The backslash is escaped? In the pattern parser, a backslash escapes the next char. So "a\\b" has 'a', then backslash which escapes the next 'b'? Actually the string "a\\b" has characters: 'a','\\','b'. The backslash is not followed by a digit, so it escapes the 'b'? Wait, the pattern parsing: when we see backslash, we set esc flag, next char 'b' becomes literal. So the trigram is 'a','b'? No, because the backslash is not counted. So pattern "a\\b" yields literals 'a' and 'b'? But that's only two characters, no trigram. So defeated. Query "ab" doesn't matter.
    std::vector<std::string> rules8 = {"a\\b"};
    auto res8 = trigramPruneCounts(rules8, "ab");
    assert(res8.size() == 1);
    assert(res8[0] == 0); // defeated due to no trigram

    // Pattern with advanced metachar: "a(b" defeats.
    std::vector<std::string> rules9 = {"a(b"};
    auto res9 = trigramPruneCounts(rules9, "ab");
    assert(res9.size() == 1);
    assert(res9[0] == 0);

    // Pattern with backslash+digit: "\\1" defeats.
    std::vector<std::string> rules10 = {"\\1"};
    auto res10 = trigramPruneCounts(rules10, "1");
    assert(res10.size() == 1);
    assert(res10[0] == 0);

    return 0;
}
