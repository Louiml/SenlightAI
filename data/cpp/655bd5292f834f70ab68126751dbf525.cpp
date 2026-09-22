/*
Write a C++ function `bool decryptMessage(const std::vector<std::string>& encryptedWords, const std::vector<std::string>& dictionary, std::vector<std::string>& result)` that attempts to decrypt a sequence of encrypted words using a given dictionary. The function treats the encryption as a simple substitution cipher where each encrypted character maps to exactly one plain character (and vice versa) globally across all words in the input. The function must return `true` and fill `result` with the decrypted plain words (in the same order as the encrypted input) if a consistent mapping exists such that every encrypted word can be transformed into a dictionary word of the same length using that mapping. If no such mapping exists, the function returns `false` and leaves `result` empty. The mapping must be consistent across all words (i.e., no conflicting assignments for any cipher or plain character). If multiple solutions exist, any one is acceptable. The encrypted words and dictionary consist only of lowercase English letters, and the dictionary contains no duplicate words. The function must handle cases where words of different lengths make certain candidates impossible, and it must not modify the input parameters.
*/

#include <vector>
#include <string>
#include <unordered_map>

// Attempt to decrypt encryptedWords using dictionary. Returns true and fills
// result with decrypted words if a consistent substitution cipher exists.
bool decryptMessage(const std::vector<std::string>& encryptedWords,
                    const std::vector<std::string>& dictionary,
                    std::vector<std::string>& result)
{
    // Helper function for backtracking
    std::function<bool(size_t, std::unordered_map<char,char>&,
                       std::unordered_map<char,char>&)> backtrack;

    backtrack = [&](size_t wordIndex,
                    std::unordered_map<char,char>& cipherToPlain,
                    std::unordered_map<char,char>& plainToCipher) -> bool {
        if (wordIndex == encryptedWords.size()) {
            return true; // All words decrypted
        }

        const std::string& encWord = encryptedWords[wordIndex];
        size_t len = encWord.length();

        // Try each dictionary word of matching length
        for (const std::string& candidate : dictionary) {
            if (candidate.length() != len) continue;

            // Tentative copy of maps for this branch
            std::unordered_map<char,char> tempC2P = cipherToPlain;
            std::unordered_map<char,char> tempP2C = plainToCipher;
            bool valid = true;

            for (size_t i = 0; i < len; ++i) {
                char c = encWord[i];
                char p = candidate[i];

                auto itc = tempC2P.find(c);
                auto itp = tempP2C.find(p);

                if (itc != tempC2P.end() && itc->second != p) {
                    valid = false;
                    break;
                }
                if (itp != tempP2C.end() && itp->second != c) {
                    valid = false;
                    break;
                }
                if (itc == tempC2P.end() && itp == tempP2C.end()) {
                    tempC2P[c] = p;
                    tempP2C[p] = c;
                }
            }

            if (valid) {
                result.push_back(candidate);
                if (backtrack(wordIndex + 1, tempC2P, tempP2C)) {
                    return true;
                }
                result.pop_back();
            }
        }
        return false;
    };

    std::unordered_map<char,char> c2p;
    std::unordered_map<char,char> p2c;
    result.clear();
    return backtrack(0, c2p, p2c);
}

#include <cassert>
#include <vector>
#include <string>
#include <iostream>

// include the solution function here (declared above or in a header)

int main() {
    // Test 1: Simple substitution
    {
        std::vector<std::string> enc = {"abc", "def"};
        std::vector<std::string> dict = {"xyz", "uvw", "abc", "def", "ghi"};
        std::vector<std::string> res;
        assert(decryptMessage(enc, dict, res));
        assert(res.size() == 2);
        assert(res[0] == "abc" && res[1] == "def");
    }

    // Test 2: No solution due to conflicting mapping
    {
        std::vector<std::string> enc = {"aa", "bb"};
        std::vector<std::string> dict = {"cc", "ab"};
        std::vector<std::string> res;
        assert(!decryptMessage(enc, dict, res));
        assert(res.empty());
    }

    // Test 3: Empty encrypted words
    {
        std::vector<std::string> enc = {};
        std::vector<std::string> dict = {"hello"};
        std::vector<std::string> res;
        assert(decryptMessage(enc, dict, res));
        assert(res.empty());
    }

    // Test 4: Different lengths force unique choices
    {
        std::vector<std::string> enc = {"a", "bc"};
        std::vector<std::string> dict = {"z", "xy", "ab"};
        std::vector<std::string> res;
        assert(decryptMessage(enc, dict, res));
        assert(res.size() == 2);
        assert(res[0].length() == 1 && res[1].length() == 2);
    }

    // Test 5: Contradictory bijection across words
    {
        std::vector<std::string> enc = {"ab", "ba"};
        std::vector<std::string> dict = {"xy", "yx"};
        std::vector<std::string> res;
        // "a"->"x", "b"->"y" for first word, then second word requires "b"->"x" and "a"->"y" => conflict
        assert(!decryptMessage(enc, dict, res));
    }

    // Test 6: Consistent mapping with duplicate letters in a word
    {
        std::vector<std::string> enc = {"moon", "soon"};
        std::vector<std::string> dict = {"good", "food"};
        std::vector<std::string> res;
        assert(decryptMessage(enc, dict, res));
        assert(res.size() == 2);
        // "m"->"g", "o"->"o", "n"->"d" ; "s"->"f" => "good" and "food"
        assert(res[0] == "good" && res[1] == "food");
    }

    // Test 7: Multiple possible solutions, any valid is fine
    {
        std::vector<std::string> enc = {"cat"};
        std::vector<std::string> dict = {"dog", "sun", "car"};
        std::vector<std::string> res;
        assert(decryptMessage(enc, dict, res));
        assert(res.size() == 1);
        assert(res[0].length() == 3);
    }

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The problem is a classic backtracking search over word‑by‑word constraints with a global character mapping. The key idea is to process encrypted words one at a time and, for each, try all dictionary words of the same length. For each candidate, we check whether the current partial mapping (from cipher to plain characters and its inverse) can be extended consistently. If a new mapping pair is needed, we add it tentatively; if a conflict arises (same cipher char mapping to two different plain chars, or same plain char mapped from two different cipher chars), we reject that candidate. We recurse to the next word with the tentative mapping; if the recursion succeeds, we return `true`; otherwise we undo the candidate and try the next dictionary word. We use temporary copies of the maps for each branch to avoid manual backtracking. Important edge cases: (1) encrypted words or dictionary may be empty, in which case empty input yields true with empty result; (2) words of unique lengths drastically reduce candidates; (3) conflicts must be checked in both directions to preserve bijectivity; (4) multiple valid mappings may exist, but any is fine. Time complexity: in the worst case, if all dictionary words have the same length and all mappings are possible, the search explores up to \(O(D^W)\) combinations where \(D\) is dictionary size and \(W\) is number of encrypted words, but in practice constraints prune heavily due to character‑mapping consistency. Space complexity is \(O(W \cdot L)\) for recursion stack and result, plus the maps which hold at most 26 entries each.
