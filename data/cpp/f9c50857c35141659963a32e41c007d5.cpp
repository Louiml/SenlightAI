Write a C++ function `long long maxWordSum(const std::vector<std::string>& words)` that, given a list of uppercase English words (each between 1 and 8 characters long), assigns a distinct decimal digit (0–9) to each distinct letter appearing in the words, such that no word begins with the digit 0, and returns the maximum possible sum of the integers obtained by interpreting the words according to this assignment. The assignment is optimal globally; you may assume at most 10 distinct letters appear. The function should not modify the input, should use `const` references where possible, and must return a `long long` since sums may exceed 32-bit range.

The greedy algorithm sorts letters by their weighted contribution to the total sum. Each occurrence of a letter in a specific position contributes `position_value * digit` to the final sum. To maximize the total, we assign the largest digit (9) to the letter with the highest total weight, the next largest (8) to the next highest weight, and so on. However, we must ensure no word starts with 0. The standard approach is: compute for each letter its total weight as the sum of powers of 10 for each position it appears. Then sort letters by weight. If the letter with the smallest weight (or any letter) is forced to be 0 due to the leading-digit constraint, we need to handle that: specifically, we sort letters by weight, but ensure that the letter assigned digit 0 is not one that appears at the start of any word. The typical method is to first find a letter that never appears at the start of any word; if such a letter exists, assign it digit 0, and assign digits 1-9 to the remaining letters in descending order of weight. If all letters appear at the start of some word, then one letter must get 0 anyway, and the optimal choice is to give 0 to the letter with the smallest weight among those that can be zero (i.e., all letters, because in that case only one letter cannot be zero? Actually, if all letters appear as leading, it's impossible to assign 0 to any of them without violating the rule, but the problem statement probably guarantees a valid assignment exists; in that case, you must assign 0 to the least weighted letter anyway, because any other assignment would still violate the leading-zero rule? Wait: the rule says "no word begins with the digit 0", so if every letter appears as the first character of some word, then no letter can be assigned 0. But since we have at most 10 letters and 10 digits, if there are exactly 10 distinct letters and all appear as leading, then we must assign digits 1-9 to 9 of them and one letter gets 0? That would violate the rule. So the problem must guarantee that at least one letter never appears as a leading digit, or there are fewer than 10 letters, in which case we can assign 0 to a letter that doesn't appear as leading, or if all letters appear as leading, then we cannot assign 0 to any, meaning we must have at most 9 distinct letters? Actually if there are 9 distinct letters, we can assign them digits 1-9, leaving 0 unused. So the algorithm: collect all distinct letters, compute weights. Find the letter with the smallest weight among those that do NOT appear as the first character of any word. If such a letter exists, assign it digit 0, and assign digits 1-9 to the rest in descending weight order. If no such letter exists (i.e., every distinct letter appears as a leading character), then we cannot assign 0 to any letter, but we still have at most 10 letters; if there are ≤9 letters, we simply assign digits 1-9 to all (no zero used). If there are exactly 10 letters and all appear as leading, the problem is impossible, but assume the input guarantees a valid assignment exists, so we don't need to handle that. The time complexity is O(L * W) where W is total length of all words, plus O(10 log 10) for sorting. Space complexity O(1) for 10 letters.

#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdint>

// Returns the maximum possible sum of the integers encoded by the given words,
// assigning distinct digits to letters and respecting the no-leading-zero rule.
long long maxWordSum(const std::vector<std::string>& words) {
    // Arrays for each letter A-Z, but we only care about those that appear.
    long long weight[26] = {0};          // total positional weight for each letter
    bool isLeading[26] = {false};        // whether a letter appears as first character
    bool present[26] = {false};

    // Compute weights and leading flags.
    for (const std::string& word : words) {
        int len = word.size();
        for (int i = 0; i < len; ++i) {
            int c = word[i] - 'A';
            present[c] = true;
            weight[c] += static_cast<long long>(std::pow(10, len - 1 - i));
            if (i == 0) isLeading[c] = true;
        }
    }

    // Collect letters that actually appear.
    std::vector<int> letters;
    for (int i = 0; i < 26; ++i) {
        if (present[i]) letters.push_back(i);
    }

    // Sort letters by weight descending.
    std::sort(letters.begin(), letters.end(), [&](int a, int b) {
        return weight[a] > weight[b];
    });

    // Determine which letter gets digit 0.
    int zeroLetter = -1;
    // Find a letter that is not leading; if exists, the one with smallest weight
    // among those is the best candidate for zero.
    int bestNonLeading = -1;
    for (int idx : letters) {
        if (!isLeading[idx]) {
            if (bestNonLeading == -1 || weight[idx] < weight[bestNonLeading]) {
                bestNonLeading = idx;
            }
        }
    }
    if (bestNonLeading != -1) {
        zeroLetter = bestNonLeading;
    } else {
        // No non-leading letter exists, so we cannot assign zero to any letter.
        // If there are ≤9 letters, we skip zero entirely; else invalid input.
        zeroLetter = -1; // no zero used
    }

    // Assign digits 9,8,7,... to the remaining letters in descending weight order.
    long long digit[26] = {0};
    int nextDigit = 9;
    for (int idx : letters) {
        if (idx == zeroLetter) {
            digit[idx] = 0;
        } else {
            digit[idx] = nextDigit--;
        }
    }

    // Compute the sum.
    long long total = 0;
    for (const std::string& word : words) {
        long long value = 0;
        for (char ch : word) {
            value = value * 10 + digit[ch - 'A'];
        }
        total += value;
    }
    return total;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or include a header).

int main() {
    // Example 1: A + B = C? Actually simple: words "A", "B", "C" with distinct digits.
    // Max sum: 9+8+7=24, but leading zeros allowed? No, each single-letter word starts with its letter, so no letter can be zero. With 3 letters, assign 9,8,7 -> sum 24.
    assert(maxWordSum({"A", "B", "C"}) == 24);

    // Example 2: "ABC" + "DEF" -> each letter distinct, none leading zero? Both start with A and D, so A and D cannot be zero. Assign weights: A has 100, D has 100, B/E have 10, C/F have 1. Sorted: A/D 100, B/E 10, C/F 1. Zero goes to a non-leading letter: B, E, C, F all non-leading? Actually "ABC" start with A, "DEF" start with D, so B,C,E,F are non-leading. Smallest weight among non-leading is C or F (weight 1). Give C=0, then F=1? But then C contributes 0*1=0, F contributes 1*1=1. Assign digits: A=9, D=8, B=7, E=6, C=0, F=5? Wait we have 6 letters, digits 9,8,7,6,5,0. Max sum = 9*100 + 8*100 + 7*10 + 6*10 + 0*1 + 5*1 = 900+800+70+60+0+5=1835. Check: ABC=970, DEF=865? That gives 1835. But is there better? Let's compute: Weights: A=100, D=100, B=10, E=10, C=1, F=1. Assign 9 to A, 8 to D, 7 to B, 6 to E, 5 to C, 0 to F? But F is non-leading, and C also non-leading. Zero to F (weight 1) is fine, then C gets 5. Sum = 9*100 + 8*100 + 7*10 + 6*10 + 5*1 + 0*1 = 900+800+70+60+5+0=1835. Same. Our algorithm: non-leading letters: B,C,E,F. Among these, smallest weight is C and F (both 1). Choose C (first encountered? Order of letters vector is by weight descending: A,D,B,E,C,F (since A and D 100, B and E 10, C and F 1, order stable by original index? Actually sort is by weight, ties broken by original index? In C++ std::sort with lambda comparing only weight, ties are not stable, but our vector order is original indices 0..25, so A=0, B=1, C=2, D=3, E=4, F=5. After sorting by weight, A and D both 100, but A index 0 < D index 3 so A first, then D. Then B index 1 and E index 4 both 10, B first, then E. Then C index 2 and F index 5 both 1, C first, then F. So letters vector = {A,B,C,D,E,F}? Actually weights: A=100, B=10, C=1, D=100, E=10, F=1. Sorting descending: A(100), D(100), B(10), E(10), C(1), F(1). So letters = {0,3,1,4,2,5}. Non-leading: B(1), C(2), E(4), F(5). Among these, smallest weight is 1 for C and F. Since we iterate letters in sorted order: A, D, B, E, C, F. First non-leading with smallest weight? We check each idx in letters order: A leading, D leading, B non-leading weight 10, remember bestNonLeading=1 (B). E non-leading weight 10, weight 10 not <10 so keep B. C non-leading weight 1, weight 1<10 so bestNonLeading=2. F non-leading weight 1, weight 1<1 false, so keep C. So zeroLetter = C. Then assign digits: A=9, D=8, B=7, E=6, C=0, F=5. Sum = 9*100 + 8*100 + 7*10 + 6*10 + 0*1 + 5*1 = 1835. Correct.
    assert(maxWordSum({"ABC", "DEF"}) == 1835);

    // Example 3: "AA" + "BB" -> weights: A has 10+1=11? Actually "AA" gives A weight 10+1=11, "BB" gives B weight 11. Leading: both A and B are leading, so no non-leading letters. Then we cannot assign zero? But there are only 2 distinct letters, we can assign digits 9 and 8, sum = 99 + 88 = 187. Check: A=9, B=8 gives 99+88=187; better than A=8,B=9 gives 88+99=187 same. Our algorithm: no non-leading, zeroLetter=-1, assign digits 9 and 8 to A and B in descending weight (weights equal, order A then B because A index 0<B index 1), so A=9, B=8 -> sum 187.
    assert(maxWordSum({"AA", "BB"}) == 187);

    // Example 4: "ABCD" + "EFGH" -> 8 distinct letters, none leading? Actually each word starts with A and E, so only A and E are leading. Non-leading: B,C,D,F,G,H. All have weights: each letter appears once in position 3 (value 1) for B,C,D and F,G,H? Actually ABCD: A has 1000, B 100, C 10, D 1; EFGH: E 1000, F 100, G 10, H 1. So weights: A=1000, E=1000, B=100, F=100, C=10, G=10, D=1, H=1. Non-leading letters: B,C,D,F,G,H. Smallest weight among non-leading: D and H weight 1. Our algorithm picks D (since after sorting, order: A, E, B, F, C, G, D, H; first non-leading with weight 1 is D). Assign digits: A=9, E=8, B=7, F=6, C=5, G=4, D=0, H=3? Wait we assign digits 9,8,7,6,5,4,3,0 to the 8 letters in order: A=9, E=8, B=7, F=6, C=5, G=4, D=0, H=3. Sum = 9*1000+8*1000+7*100+6*100+5*10+4*10+0*1+3*1 = 17000+1300+90+3? Actually compute: 9000+8000=17000, +700+600=18300, +50+40=18390, +0+3=18393. Check: ABCD with digits A=9,B=7,C=5,D=0 gives 9750; EFGH with E=8,F=6,G=4,H=3 gives 8643; sum=18393. That seems correct.
    assert(maxWordSum({"ABCD", "EFGH"}) == 18393);

    // Edge case: single word "Z" -> only letter Z, assign 9, sum=9.
    assert(maxWordSum({"Z"}) == 9);

    // Edge case: "A"+"AA"+"AAA" -> weights: A appears in positions: for "A":1, "AA":10+1=11, "AAA":100+10+1=111, total weight 123. Only one letter, no leading issue, assign 9, sum = 9 + 99 + 999 = 1107.
    assert(maxWordSum({"A", "AA", "AAA"}) == 1107);

    // Case with 10 letters, one not leading: "ABCDEFGHIJ" and "K" but that's 11 letters? Not allowed. Use "ABCDEFGHIJ" and "ABCDEFGHI" etc. But simpler: words "ABCDEFGHIJ" and "K" has 11 distinct letters, invalid. So skip.

    return 0;
}
