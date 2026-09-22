// Write a C++ function named `isGuessConsistent` that takes four parameters: a `std::string` representing a first guess (a 4-character string consisting of the letters 'R','G','B','Y','O','V', each used exactly once), two integers `firstA` and `firstB` giving the feedback for that first guess (where `firstA` is the number of correct letters in correct positions, and `firstB` is the number of correct letters in wrong positions), a second guess string (again a 4-character string using each of the six letters exactly once), and two integers `secondA` and `secondB` giving the feedback for that second guess. The function should return `true` if there exists at least one 4-character secret code (using each of the six letters exactly once) that would produce both feedback pairs for the two guesses, and `false` otherwise. The feedback rules: for each guess, `A` counts positions where the guess letter matches the secret letter in the same position; `B` counts positions where the guess letter appears in the secret but not in the same position (and each letter in the secret is only counted once for `B`). Note that the secret code can be any permutation of exactly 4 distinct letters chosen from the six possible letters. Both guesses are always valid (4 distinct letters from the six). The function must not rely on any global state and must be deterministic.

// The problem is a constraint-satisfaction search over all possible secret codes. There are only 6 letters and the secret uses 4 distinct letters, so the total number of possible secrets is the number of permutations of 4 letters chosen from 6: \(P(6,4) = 6 \times 5 \times 4 \times 3 = 360\). That is very small, so a brute-force enumeration is trivial. The solution generates all 360 permutations (e.g., using recursion with a set to track used letters or using `std::next_permutation` on a set of available letters and then taking the first 4 characters). For each candidate secret, compute the feedback for the first guess using a helper function `feedback(secret, guess)` that returns a pair `(A,B)`. Then compare that feedback to `(firstA, firstB)`. If it matches, compute the feedback for the second guess and compare to `(secondA, secondB)`. If both match, return `true`. If no candidate matches both constraints, return `false`. Time complexity is \(O(360 \times 4) = O(1440)\) operations, which is constant-time in practice. Space complexity is \(O(1)\) aside from the list of 360 candidate secrets if stored, but we can generate them on the fly or store them in a local vector of size 360. Edge cases include cases where the feedback numbers are impossible (e.g., `firstA + firstB > 4`), but the brute-force search will naturally return `false` because no secret will produce such a combined count. Also, note that `B` counting must exclude same-position matches. Use a set or boolean array to avoid double-counting letters for `B`.

#include <string>
#include <vector>
#include <set>
#include <array>
#include <utility>

// Compute feedback for a guess given a secret.
// Returns (correctPositions, correctLettersWrongPosition).
std::pair<int,int> getFeedback(const std::string& secret, const std::string& guess) {
    int A = 0;
    bool secretUsed[4] = {false};
    bool guessUsed[4] = {false};
    
    // First pass: count exact matches and mark used positions.
    for (int i = 0; i < 4; ++i) {
        if (secret[i] == guess[i]) {
            ++A;
            secretUsed[i] = true;
            guessUsed[i] = true;
        }
    }
    
    // Second pass: count wrong-position matches.
    int B = 0;
    for (int i = 0; i < 4; ++i) {
        if (guessUsed[i]) continue;
        for (int j = 0; j < 4; ++j) {
            if (secretUsed[j]) continue;
            if (guess[i] == secret[j]) {
                ++B;
                secretUsed[j] = true;
                guessUsed[i] = true;
                break;
            }
        }
    }
    return {A, B};
}

// Generate all permutations of length 4 from the letters "RGBYOV".
void generateSecrets(std::string current, std::set<char>& used, std::vector<std::string>& out) {
    if (current.size() == 4) {
        out.push_back(current);
        return;
    }
    const std::string letters = "RGBYOV";
    for (char c : letters) {
        if (used.find(c) == used.end()) {
            used.insert(c);
            generateSecrets(current + c, used, out);
            used.erase(c);
        }
    }
}

// Returns true if there exists a secret consistent with both guesses and their feedback.
bool isGuessConsistent(const std::string& firstGuess, int firstA, int firstB,
                       const std::string& secondGuess, int secondA, int secondB) {
    std::vector<std::string> secrets;
    std::set<char> used;
    generateSecrets("", used, secrets);
    
    for (const auto& secret : secrets) {
        auto fb1 = getFeedback(secret, firstGuess);
        if (fb1.first != firstA || fb1.second != firstB) continue;
        auto fb2 = getFeedback(secret, secondGuess);
        if (fb2.first == secondA && fb2.second == secondB) {
            return true;
        }
    }
    return false;
}

#include <cassert>
#include <string>

// The solution function is declared above; here we test it.
int main() {
    // Example 1: Secret "RGBY" satisfies guess "RGBY" with (4,0) and guess "RGBO" with (3,0).
    assert(isGuessConsistent("RGBY", 4, 0, "RGBO", 3, 0) == true);
    
    // Example 2: No secret can give (4,0) for "RGBY" and (4,0) for "BGRY" because that would imply two different secrets.
    assert(isGuessConsistent("RGBY", 4, 0, "BGRY", 4, 0) == false);
    
    // Example 3: Secret "ROVY" works for guess "RGBY" (1 exact: 'R', 1 wrong: 'Y') and guess "YVOR" (3 exact: 'Y','V','O')? Actually check manual: secret = R O V Y, guess1 = R G B Y -> A=2 (R,Y), B=0? Wait B counts wrong positions: 'O','V' not in guess? So B=0. Not matching. But we expect true? Let's use a known consistent pair: secret "ROVY", guess1 "RGBY" gives A=2 (R,Y), B=0? Actually guess "RGBY" has letters R,G,B,Y; secret has R,O,V,Y; common letters R,Y. R pos0 correct, Y pos3 correct -> A=2; O,V not in guess so B=0. So (2,0). guess2 "OVRB" gives A? guess letters O,V,R,B; secret R,O,V,Y -> positions: 0 vs O wrong, 1 vs V wrong? Wait secret index1=O, guess index1=V -> not match; index2=V vs R -> not; index3=Y vs B -> not. So A=0. Common letters O,V,R,Y but Y not in guess? Actually guess "OVRB" has O,V,R,B. Common with secret R,O,V,Y: O,V,R,Y? Y not in guess, so O,V,R. B counts wrong positions: secret O at index1, guess O at index0 -> wrong position count 1; secret V index2, guess V index1 -> count 1; secret R index0, guess R index2 -> count 1. So B=3. So (0,3). So test: (2,0) and (0,3) should be true.
    assert(isGuessConsistent("RGBY", 2, 0, "OVRB", 0, 3) == true);
    
    // Example 4: Impossible feedback, A+B > 4. Should return false.
    assert(isGuessConsistent("RGBY", 3, 2, "RGBY", 3, 2) == false);
    
    // Example 5: Both guesses identical with same feedback, should be true (e.g., secret "RGBY").
    assert(isGuessConsistent("RGBY", 4, 0, "RGBY", 4, 0) == true);
    
    // Example 6: First guess feedback impossible for any secret, e.g., A=5.
    assert(isGuessConsistent("RGBY", 5, 0, "RGBY", 4, 0) == false);
    
    // Example 7: Secret "GBYR" works? guess "RGBY" gives A? positions: R vs G wrong, G vs B wrong, B vs Y wrong, Y vs R wrong -> A=0. Common letters: all four, but all wrong positions -> B=4. So (0,4). For guess "GBYR" gives (4,0). So test true.
    assert(isGuessConsistent("RGBY", 0, 4, "GBYR", 4, 0) == true);
    
    // Example 8: Contradictory feedback: (0,4) and (4,0) for same guess "RGBY" -> can't both hold, false.
    assert(isGuessConsistent("RGBY", 0, 4, "RGBY", 4, 0) == false);
    
    return 0;
}
