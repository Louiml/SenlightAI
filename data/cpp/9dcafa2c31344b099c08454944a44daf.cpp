/*
Write a C++ function named `fuzzyStringScore` that takes two strings, `word` and `query`, along with a double `fuzziness` parameter (default 0), and returns a similarity score between 0 and 1 (inclusive). The function must implement a fuzzy substring matching algorithm inspired by the provided code: it should search for each character of `query` in `word` sequentially (case-insensitively), advancing through `word` after each match, and award bonus scores for matches at word boundaries (start of word or after a space) and for exact case matches. If a character from `query` is not found in the remaining portion of `word`, and `fuzziness` is greater than 0, the function may skip that character once (increasing a fuzziness penalty) and continue; if no fuzziness is allowed, it must return 0.0. The final score must be normalized by the length of `query`, blended with word-length ratio, divided by the fuzziness penalty, and optionally boosted by 0.1 if the first character of `query` matched the very first character of `word` (and the boosted score is still less than 1). The returned value must never exceed 1.0. You may use the original scoring constants: `MATCH_SCORE = 0.1`, `BOUNDARY_SCORE = 0.7` (for word start), `SPACE_BOUNDARY_SCORE = 0.8` (after space), `EXACT_CASE_BONUS = 0.1`, `FUZZY_PENALTY_ADD = (1.0 - fuzziness)`, and `START_CHAR_BOOST = 0.1`.
*/

#include <string>
#include <cctype>

// Constants for scoring (mirroring the original algorithm)
constexpr double MATCH_SCORE = 0.1;
constexpr double WORD_START_BONUS = 0.7;
constexpr double SPACE_BOUNDARY_BONUS = 0.8;
constexpr double EXACT_CASE_BONUS = 0.1;
constexpr double START_CHAR_BOOST = 0.1;

// Compute a fuzzy similarity score between word and query.
// If fuzziness > 0, allow skipping unmatched query characters.
double fuzzyStringScore(const std::string& word, const std::string& query, double fuzziness = 0.0) {
    const std::size_t wordLen = word.length();
    const std::size_t queryLen = query.length();

    // Immediate edge cases
    if (wordLen == queryLen && word == query)
        return 1.0;
    if (queryLen == 0)
        return 0.0;
    if (wordLen == 0)
        return 0.0;

    double totalScore = 0.0;
    double fuzzies = 1.0;
    bool firstCharMatchedAtStart = false;

    std::size_t wordIdx = 0;  // current position in word

    for (std::size_t qIdx = 0; qIdx < queryLen; ++qIdx) {
        const char queryChar = query[qIdx];
        const char lowerQueryChar = static_cast<char>(std::tolower(queryChar));

        // Search for a match in the remaining portion of word
        std::size_t matchPos = std::string::npos;
        for (std::size_t i = wordIdx; i < wordLen; ++i) {
            if (std::tolower(word[i]) == lowerQueryChar) {
                matchPos = i;
                break;
            }
        }

        if (matchPos == std::string::npos) {
            if (fuzziness > 0.0) {
                // Skip this query character with a penalty
                fuzzies += (1.0 - fuzziness);
                continue;
            } else {
                return 0.0;
            }
        }

        // Base score for the match
        totalScore += MATCH_SCORE;

        // Exact case bonus
        if (word[matchPos] == queryChar) {
            totalScore += EXACT_CASE_BONUS;
        }

        // Boundary bonuses
        if (matchPos == 0) {
            totalScore += WORD_START_BONUS;
            if (qIdx == 0) {
                firstCharMatchedAtStart = true;
            }
        } else if (matchPos > 0 && word[matchPos - 1] == ' ') {
            totalScore += SPACE_BOUNDARY_BONUS;
        }

        // Advance the search position in word
        wordIdx = matchPos + 1;
    }

    // Normalize by query length
    double avgScore = totalScore / static_cast<double>(queryLen);

    // Blend with length ratio
    double blended = (avgScore * (queryLen / static_cast<double>(wordLen)) + avgScore) / 2.0;

    // Apply fuzziness penalty
    blended /= fuzzies;

    // Optional boost for starting character match
    if (firstCharMatchedAtStart && (blended + START_CHAR_BOOST < 1.0)) {
        blended += START_CHAR_BOOST;
    }

    // Clamp to [0,1] (though algorithm naturally stays within)
    if (blended < 0.0) return 0.0;
    if (blended > 1.0) return 1.0;
    return blended;
}

#include <cassert>
#include <cmath>

// Reuse the fuzzyStringScore declaration from the solution (assume it's defined above)
// In a real environment, this would be included or copy-pasted.

int main() {
    // Exact match returns 1
    assert(std::fabs(fuzzyStringScore("hello", "hello") - 1.0) < 1e-9);

    // Empty query returns 0
    assert(fuzzyStringScore("hello", "") == 0.0);

    // Empty word with non-empty query returns 0
    assert(fuzzyStringScore("", "hello") == 0.0);

    // Simple substring without fuzziness should score above 0
    assert(fuzzyStringScore("hello world", "he") > 0.0);
    assert(fuzzyStringScore("hello world", "hello") > 0.5);

    // Case-insensitive matching: "He" matches "hello"
    double caseScore = fuzzyStringScore("hello", "He");
    assert(caseScore > 0.0);

    // Fuzziness allows skipping unmatched characters
    double fuzzyScore = fuzzyStringScore("hello", "hxllo", 0.5);
    assert(fuzzyScore > 0.0);

    // No fuzziness with non-matching character returns 0
    assert(fuzzyStringScore("abc", "axc", 0.0) == 0.0);

    // Boundary bonus: match after a space gives higher score than middle
    double spaceScore = fuzzyStringScore("hello world", "w");
    double middleScore = fuzzyStringScore("hello world", "o");
    assert(spaceScore > middleScore);

    // Fuzziness penalty reduces score compared to perfect substring
    double perfect = fuzzyStringScore("hello world", "hello", 0.0);
    double fuzzy = fuzzyStringScore("hello world", "hello", 1.0);
    assert(fuzzy <= perfect + 1e-9);

    // Score never exceeds 1
    assert(fuzzyStringScore("abc", "a") <= 1.0);
    assert(fuzzyStringScore("abc", "abcde", 1.0) <= 1.0);

    return 0;
}

// The solution must replicate the logic of the given snippet but with clearer naming and robust handling. The main algorithm iterates through each character of `query` (from left to right). For each query character, it searches forward in the current position of `word` (starting from the beginning) for a case-insensitive match. When found, it adds a base score: `MATCH_SCORE` for a case-insensitive match, plus an additional `MATCH_SCORE` if the case matches exactly. It then adds bonus scores: `0.7` if the match is at index 0 of `word` (i.e., the very first character), or `0.8` if the character before the match is a space. After each match, advance the search pointer in `word` to just after the matched character. If no match is found in the remaining portion of `word`, then if `fuzziness > 0`, increment a `fuzzies` counter by `(1.0 - fuzziness)` and skip the query character; otherwise return 0.0. After processing all query characters, compute the raw average score by dividing the accumulated score by `query.length()`. Then blend this average with a length-normalized score: `(raw_average * (query.length() / word.length()) + raw_average) / 2`. Divide by the fuzziness penalty `fuzzies`. If the first query character matched the very first character of `word` (i.e., the first match was at index 0 of `word`), add `0.1` to the score if the result is still less than 1.0. Finally, clamp the score to be between 0 and 1 (though the algorithm naturally stays within). Edge cases: empty `word` with non-empty `query` should return 0; empty `query` should return 0; identical words return 1.0. The algorithm runs in O(query_length × word_length) in the worst case due to the linear search inside the outer loop, and uses O(1) auxiliary space. The `fuzzies` counter starts at 1.0 and increases with each fuzzy skip.
