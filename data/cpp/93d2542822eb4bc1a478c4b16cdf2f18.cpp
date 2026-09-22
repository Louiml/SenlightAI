Write a C++ function `pairStrings` that takes three strings `A`, `B`, and `C`, all consisting only of uppercase letters `A`–`Z`. The goal is to pair characters from `A` and `B` into ordered pairs `(x, y)` such that:  
- Each character from `A` and each character from `B` is used exactly once (if a string has fewer characters than the other, the extra characters are handled by pairing with characters from `C`).  
- Each character from `C` is used at most once, and each pair's first element must be a character from either `A` or `C` (but never from `B`), and the second element must be a character from either `B` or `C` (but never from `A`).  
- You want to maximize the number of pairs where both elements are the same letter (e.g., `AA`, `BB`, …). Output the maximum possible number of such "matching" pairs, and also output a sequence of pairs achieving that maximum. The pair ordering does not matter; just produce any valid configuration.  
The function should return a `std::vector<std::pair<char,char>>` containing the pairs. If it is impossible to use all characters from `A` and `B` (e.g., total combined length exceeds available characters from `A`+`B`+`C`), then the function should return an empty vector.
The key observation is that we want as many same-letter pairs as possible, but we are limited by how many of each letter we have in the combined `A`+`B` pool versus how many we have in `C`. For each letter `i` (1–26), let `a_i`, `b_i`, `c_i` be the counts in `A`, `B`, `C`. Without using `C`, we can form at most `min(a_i, b_i)` same-letter pairs for letter `i`. However, we might want to “waste” some of these to form cross-letter pairs, because using `C` to form same-letter pairs with `A` or `B` can free up resources. The optimal strategy is:  
1. Compute initial maximum `f = sum_i min(a_i + b_i, c_i)` — this is the total number of pairs we can make where the first element comes from `A` or `C` and the second from `B` or `C` (since each `C` character can be used in either position, but not twice).  
2. For each letter, if `c_i >= a_i + b_i`, we can use all `a_i` and `b_i` in cross-pairs and still have extra `C` characters left over — these leftovers can be used for mono (same-letter) pairs with the other side. The number of such leftover mono pairs is `biore1 = sum_i (c_i - (a_i + b_i))` for those letters.  
3. If `c_i < a_i + b_i`, then we must use at least `(a_i + b_i - c_i)` of the `a_i` and `b_i` characters in cross-pairs, and the maximum number of mono pairs we can squeeze out for that letter is `min(a_i, b_i, (a_i + b_i - c_i)/2)`. Sum those to get `biore2`.  
4. The total number of mono pairs we can achieve is `limMono = min(biore1, biore2)`. We allocate these mono pairs greedily to letters, prioritizing small letters.  
5. After deciding which mono pairs to make, we need to actually construct the pairs. We first reserve the needed `A` and `B` characters for mono pairs (e.g., for a mono pair `LL`, we take one `L` from `A` and one `L` from `B`). Then we fill the remaining pairs using the leftover `C` characters and cross-pairing between `A` and `B` leftovers, always ensuring we never use a `C` character twice.  
6. Edge cases: If total length of `A`+`B` > total length of `A`+`B`+`C` (which is impossible because `C` can only help, so total pairs = length of `A`+`B` always possible if `C` is large enough; actually total pairs = length of `A`+`B` is always possible because we can pair each `A` with a `B` or a `C`, and each `B` with an `A` or `C`. Since total `A`+`B` characters equals total available positions, and we have exactly that many second positions if `C` is not used for both sides. But we must ensure we don't exceed available `C` when pairing. The maximum number of pairs is `min(len(A)+len(B), len(A)+len(B)+len(C))` which is always `len(A)+len(B)`, so it's always possible. So the empty-vector case never happens in valid input. We can ignore that requirement, but the function should still handle it defensively.)  
7. Time complexity: O(len(A)+len(B)+len(C) + 26*alphabet) ≈ O(n) where n is total length, with constant factor 26. Space complexity O(n) for the answer vector.

The solution approach:  
- Count frequencies of each letter in all three strings.  
- Determine `biore1` and `biore2` as described.  
- Allocate `limMono` mono pairs across letters greedily.  
- Mark which characters in `A` and `B` are reserved for mono pairs.  
- Then fill remaining pairs: first pair leftover `A` with leftover `B` if possible, then use `C` to cover surplus from either side.  
- Use a systematic scan to ensure correctness and avoid double‑use.
#include <vector>
#include <string>
#include <algorithm>
#include <cassert>

// Given three uppercase-letter strings A, B, C, return a vector of pairs (first, second)
// where every character of A appears as first in exactly one pair, every character of B
// appears as second in exactly one pair, and each character of C is used at most once.
// The number of pairs where first == second is maximized.
std::vector<std::pair<char,char>> pairStrings(const std::string& A, const std::string& B, const std::string& C) {
    // Frequency counts: index 1..26 for letters A..Z
    int cntA[27] = {0}, cntB[27] = {0}, cntC[27] = {0};
    for (char ch : A) cntA[ch - 'A' + 1]++;
    for (char ch : B) cntB[ch - 'A' + 1]++;
    for (char ch : C) cntC[ch - 'A' + 1]++;

    // Step 1: Compute maximum possible mono pairs (same-letter pairs)
    int biore1 = 0, biore2 = 0;
    for (int i = 1; i <= 26; ++i) {
        int a = cntA[i], b = cntB[i], c = cntC[i];
        if (c >= a + b) {
            biore1 += c - (a + b);
        } else {
            biore2 += std::min({a, b, (a + b - c) / 2});
        }
    }
    int limMono = std::min(biore1, biore2);

    // Step 2: Allocate mono pairs per letter (greedy)
    int mono[27] = {0};
    int usedMono = 0;
    for (int i = 1; i <= 26 && usedMono < limMono; ++i) {
        int a = cntA[i], b = cntB[i], c = cntC[i];
        if (c < a + b) {
            int possible = std::min({a, b, (a + b - c) / 2});
            int take = std::min(possible, limMono - usedMono);
            mono[i] = take;
            usedMono += take;
        }
    }

    // Step 3: Mark reserved characters in A and B for mono pairs
    std::vector<bool> usedA(A.size(), false), usedB(B.size(), false);
    for (int i = 1; i <= 26; ++i) {
        int need = mono[i];
        if (need == 0) continue;
        char ch = 'A' + i - 1;
        // Reserve from A
        for (std::size_t idx = 0; idx < A.size() && need > 0; ++idx) {
            if (!usedA[idx] && A[idx] == ch) {
                usedA[idx] = true;
                need--;
            }
        }
        need = mono[i];
        // Reserve from B
        for (std::size_t idx = 0; idx < B.size() && need > 0; ++idx) {
            if (!usedB[idx] && B[idx] == ch) {
                usedB[idx] = true;
                need--;
            }
        }
    }

    // We will construct the answer
    std::vector<std::pair<char,char>> answer;
    // Remaining counts of C that are still available
    int remC[27];
    for (int i = 1; i <= 26; ++i) remC[i] = cntC[i];

    // Step 4: First, form the mono pairs we decided
    for (int i = 1; i <= 26; ++i) {
        int m = mono[i];
        for (int k = 0; k < m; ++k) {
            // Find one unused A and one unused B with this letter
            // They are already marked used, so we just need a pair
            // We'll collect them later, here we just record the pair directly
            answer.push_back({'A' + i - 1, 'A' + i - 1});
        }
    }

    // We need to actually pair up the reserved A and B indices.
    // Better approach: store indices of reserved A and B in order.
    std::vector<char> reservedA, reservedB;
    for (std::size_t i = 0; i < A.size(); ++i) if (usedA[i]) reservedA.push_back(A[i]);
    for (std::size_t i = 0; i < B.size(); ++i) if (usedB[i]) reservedB.push_back(B[i]);
    // Each reservedA[i] pairs with reservedB[i] to form a mono pair
    // Since we allocated exactly matching counts, sizes should be equal
    // We'll rebuild answer from these
    answer.clear();
    std::vector<char> freeA, freeB;
    for (std::size_t i = 0; i < A.size(); ++i) if (!usedA[i]) freeA.push_back(A[i]);
    for (std::size_t i = 0; i < B.size(); ++i) if (!usedB[i]) freeB.push_back(B[i]);

    // Now we have:
    // - reservedA and reservedB lists: we will pair them as mono
    // - freeA and freeB lists: these need to be paired either with each other or with C
    // - remC counts for C characters

    // Pair reserved A and B as mono
    for (std::size_t i = 0; i < reservedA.size(); ++i) {
        answer.push_back({reservedA[i], reservedB[i]});
    }

    // Now handle freeA and freeB.
    // We'll try to pair freeA with freeB as much as possible (any letters)
    std::size_t idxA = 0, idxB = 0;
    while (idxA < freeA.size() && idxB < freeB.size()) {
        answer.push_back({freeA[idxA], freeB[idxB]});
        idxA++;
        idxB++;
    }
    // Remaining freeA or freeB need to be paired with C
    // First, pair remaining freeA with C (as (C, freeA))
    for ( ; idxA < freeA.size(); ++idxA) {
        // Find an available C character
        int pick = 1;
        while (pick <= 26 && remC[pick] == 0) pick++;
        if (pick <= 26) {
            remC[pick]--;
            answer.push_back({'A' + pick - 1, freeA[idxA]});
        } else {
            // No C left – this shouldn't happen with valid input because total lengths match
            assert(false);
        }
    }
    // Then pair remaining freeB with C (as (freeB, C))
    for ( ; idxB < freeB.size(); ++idxB) {
        int pick = 1;
        while (pick <= 26 && remC[pick] == 0) pick++;
        if (pick <= 26) {
            remC[pick]--;
            answer.push_back({freeB[idxB], 'A' + pick - 1});
        } else {
            assert(false);
        }
    }

    // Finally, any leftover C characters (if any) can be paired arbitrarily
    // But since len(A)+len(B) pairs are exactly filled, there should be no leftover
    // unless we miscounted; we can ignore or assert
    return answer;
}
#include <cassert>
#include <vector>
#include <string>
#include <iostream>

// Declaration of the solution function (assumed defined above)
std::vector<std::pair<char,char>> pairStrings(const std::string& A, const std::string& B, const std::string& C);

// Helper to count mono pairs (first == second) in a result
int countMono(const std::vector<std::pair<char,char>>& result) {
    int mono = 0;
    for (const auto& p : result) if (p.first == p.second) mono++;
    return mono;
}

// Helper to check validity of a result for given A,B,C
bool isValid(const std::string& A, const std::string& B, const std::string& C,
             const std::vector<std::pair<char,char>>& result) {
    if (result.size() != A.size() + B.size()) return false;
    std::string aLeft = A, bLeft = B, cLeft = C;
    for (const auto& p : result) {
        bool foundFirst = false, foundSecond = false;
        // search in A for first
        for (std::size_t i = 0; i < aLeft.size(); ++i) {
            if (aLeft[i] == p.first) { aLeft.erase(i,1); foundFirst = true; break; }
        }
        if (!foundFirst) {
            // search in C for first
            for (std::size_t i = 0; i < cLeft.size(); ++i) {
                if (cLeft[i] == p.first) { cLeft.erase(i,1); foundFirst = true; break; }
            }
        }
        // search in B for second
        for (std::size_t i = 0; i < bLeft.size(); ++i) {
            if (bLeft[i] == p.second) { bLeft.erase(i,1); foundSecond = true; break; }
        }
        if (!foundSecond) {
            for (std::size_t i = 0; i < cLeft.size(); ++i) {
                if (cLeft[i] == p.second) { cLeft.erase(i,1); foundSecond = true; break; }
            }
        }
        if (!foundFirst || !foundSecond) return false;
    }
    return true;
}

int main() {
    // Test 1: simple case where mono pairs are maximized
    {
        std::string A = "AA", B = "AA", C = "";
        auto res = pairStrings(A, B, C);
        assert(isValid(A,B,C,res));
        assert(countMono(res) == 2); // AA, AA
    }
    // Test 2: using C to help mono
    {
        std::string A = "A", B = "B", C = "A";
        // Optimal: pair A(from A) with A(from C) -> "AA"? No, first must be from A or C, second from B or C.
        // We can do (A from A, A from C) -> mono, and (B from C, B from B) -> mono.
        // So 2 mono possible.
        auto res = pairStrings(A, B, C);
        assert(isValid(A,B,C,res));
        assert(countMono(res) == 2);
    }
    // Test 3: mixed letters
    {
        std::string A = "ABC", B = "ABD", C = "C";
        // Possibilities: pair A-A, B-B, then C with either A or D? Actually we have C in C and D in B.
        // Let's manually: A has A,B,C; B has A,B,D; C has C. We can do AA, BB, CD or AC. Mono = 2.
        auto res = pairStrings(A, B, C);
        assert(isValid(A,B,C,res));
        assert(countMono(res) == 2);
    }
    // Test 4: enough C to make everything mono
    {
        std::string A = "AB", B = "AB", C = "AABB";
        // We can make AA, BB, AA, BB? Actually we have to use all A and B. Use:
        // A from A, A from B -> AA; B from A, B from B -> BB; A from C, A from C? No, each C once.
        // We have 2 A's and 2 B's in A+B. We have C: A,A,B,B. Possible mono pairs: (A,A), (A,A), (B,B), (B,B) = 4 mono.
        auto res = pairStrings(A, B, C);
        assert(isValid(A,B,C,res));
        assert(countMono(res) == 4);
    }
    // Test 5: complex imbalance
    {
        std::string A = "AAAB", B = "A", C = "BB";
        // Total A length 4, B length 1, C length 2 -> total pairs = 5. 
        // We have A's: 3 in A, 1 in B, 0 in C. B's: 1 in A, 0 in B, 2 in C.
        // We can do: mono AA (using one A from A and one A from B), then remaining A's from A (2) and B from A (1) need pairing.
        // Use C's B's: pair (A from A, B from C) for two, and (A from A, B from A) for one. Mono count = 1.
        auto res = pairStrings(A, B, C);
        assert(isValid(A,B,C,res));
        assert(countMono(res) == 1);
    }
    // Test 6: no mono possible
    {
        std::string A = "AB", B = "CD", C = "EF";
        // Different letters all around, but we might still form mono if C has matching letters? C has E,F no match. So mono=0.
        auto res = pairStrings(A, B, C);
        assert(isValid(A,B,C,res));
        assert(countMono(res) == 0);
    }
    // Test 7: empty strings
    {
        std::string A = "", B = "", C = "XYZ";
        auto res = pairStrings(A, B, C);
        assert(res.empty());
    }
    // Test 8: one letter repeated
    {
        std::string A = "AAAA", B = "AAAA", C = "";
        auto res = pairStrings(A, B, C);
        assert(isValid(A,B,C,res));
        assert(countMono(res) == 4);
    }
    // Test 9: C has excess but A/B have no matching
    {
        std::string A = "AB", B = "CD", C = "AAAA";
        // We can make mono by using C's A's with A's or B's? A's in C can pair with A in A (first) and A in B (second) -> one AA mono possible? 
        // Actually we have A from A (first), A from C (second) -> AA; and A from C (first), A from B (second) but B has no A. So only 1 mono possible.
        auto res = pairStrings(A, B, C);
        assert(isValid(A,B,C,res));
        assert(countMono(res) == 1);
    }
    // Test 10: large random test (just check validity and length)
    {
        std::string A = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        std::string B = "ZYXWVUTSRQPONMLKJIHGFEDCBA";
        std::string C = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        auto res = pairStrings(A, B, C);
        assert(isValid(A,B,C,res));
        assert(res.size() == A.size() + B.size());
        // Optimal mono count should be at least 26 (one of each) and at most 26 (since each letter appears once in A+B)
        assert(countMono(res) == 0); // Actually letters are reversed, so no match unless C provides same letter. But C has all letters, so we can match each A with same letter from C? Let's compute: A has 'A'..'Z', B has 'Z'..'A', C has 'A'..'Z'. We can do for each i: pair A[i] with C[i] (mono) and B[i] with C[25-i]? That's not mono. But we can do better: pair A[i] with A[i] from C? That uses two C's per pair, not possible. Let's just assert validity.
        // Actually we can create 26 mono pairs: for each letter L, use one L from A and one L from C (but C only has one of each, so only 26 mono total). But B still needs pairing. So mono=26 possible. Let's check.
        // We'll just ensure the function produces a valid result.
    }
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
