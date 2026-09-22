Write a C++ function `std::string soundexCode(const std::string& word)` that takes a non-empty string containing only uppercase English letters (A–Z) and returns its simplified Soundex-like code as a string of digits. The mapping is: B,F,P,V → 1; C,G,J,K,Q,S,X,Z → 2; D,T → 3; L → 4; M,N → 5; R → 6; all other letters (A,E,I,O,U,H,W,Y) → 0 (these are ignored). The output should contain only the non-zero digits after applying this rule: a digit is printed only if it is not zero and it is not the same as the immediately preceding digit (before ignoring zeros). For example, for the input "BFP", the raw mapping gives 1,1,1; since the second and third are duplicates of the first, the output is "1". For "PBP", the mapping gives 1,0,1; since the zeros are ignored, the two 1's are separated by a zero, so both are printed → "11". For "ABC", mapping gives 0,0,0 → no digits printed, so return an empty string. The function must not modify the input and must work for words of any length (including length 1, where it returns the single non-zero digit if any, or empty if the letter maps to 0). Do not include any main function or I/O in the solution function.

The core algorithm processes the input string left to right. For each character, compute its Soundex digit using a helper mapping (a simple switch or a static array of size 26 indexed by `c - 'A'`). Maintain a variable `previous` that stores the last printed (non-zero, non-duplicate) digit. Also maintain a boolean `justPrinted` to track whether we have printed a digit yet, because we must not suppress a digit that repeats after a zero: for example, in "PBP", the first P prints '1', the B maps to 0 and is skipped, then the second P maps to 1, and since it is not equal to the last printed digit (which is still '1' from before the zero), but we need to print it because the zero broke the adjacency. Actually, careful: the rule says "a digit is printed only if it is not zero and it is not the same as the immediately preceding digit". "Immediately preceding digit" refers to the digit that would have been printed if we had printed all digits in order, including zeros? The intended rule (as in the snippet) is: when iterating over the raw mapped digits (including zeros), skip any zero, and also skip any digit equal to the previous digit in the filtered sequence. However, in the snippet, they push all digits to a vector including zeros, then iterate and skip if `vec[i]==vec[i-1]`. That means zeros are considered as previous digits too? Let's analyze the snippet: for input "PBP", vector = [1,0,1]. Then loop: i=0 prints 1; i=1 (0) is skipped because vec[i]==0; i=2 (1), check i>0 && vec[2]==vec[1]? vec[2]=1, vec[1]=0, not equal, so prints 1 → "11". Good. For input "BPB" → vector = [1,0,1] as well, output "11". For "BB" → vector=[1,1], i=1: vec[1]==vec[0] (1==1) → skip → "1". For "B B" (but no spaces), "BAB" -> vector=[1,0,1] -> "11". So the rule is: when we have consecutive identical non-zero digits with no zero in between, we suppress the later one. If a zero appears between identical digits, we do not suppress. So we need to track the previous *original* digit (including zeros) to know if the current digit is equal to the last digit in the raw vector. But zeros are never printed anyway. So the correct logic: maintain `lastDigit` as the previous character's mapped digit (including 0). Then for each new digit `d`, if `d != 0` and `d != lastDigit` (where `lastDigit` is the previous mapped digit, which could be 0), then print `d`. After processing, update `lastDigit = d` for every position. That matches the snippet exactly: because in the snippet they compare `vec[i]` with `vec[i-1]` (the immediate previous in the vector). So we just need to keep the previous digit from the previous character. So the algorithm: start with `last = -1` (sentinel). For each char `c` in word, compute `d = map[c-'A']`. If `d != 0 && d != last` then append char `'0'+d` to result. Then set `last = d`. That handles all cases. Time complexity O(n), space O(1) auxiliary (plus output string length). Edge cases: length 1, all zeros, consecutive zeros, zeros between same digits, repeated digits without zeros.

#include <string>

// Returns the simplified Soundex code for a word of uppercase letters.
std::string soundexCode(const std::string& word) {
    // Mapping for letters A-Z to digits 0-6 as per rules.
    static const char map[26] = {
        '0', // A
        '1', // B
        '2', // C
        '3', // D
        '0', // E
        '1', // F
        '2', // G
        '0', // H
        '0', // I
        '2', // J
        '2', // K
        '4', // L
        '5', // M
        '5', // N
        '0', // O
        '1', // P
        '2', // Q
        '6', // R
        '2', // S
        '3', // T
        '0', // U
        '1', // V
        '0', // W
        '2', // X
        '0', // Y
        '2'  // Z
    };

    std::string result;
    char lastDigit = '\0';  // sentinel; not a digit

    for (char c : word) {
        char d = map[c - 'A'];
        if (d != '0' && d != lastDigit) {
            result.push_back(d);
        }
        lastDigit = d;
    }

    return result;
}

#include <cassert>
#include <string>

std::string soundexCode(const std::string& word);

int main() {
    // Basic cases from the rules
    assert(soundexCode("BFP") == "1");
    assert(soundexCode("PBP") == "11");
    assert(soundexCode("ABC") == "");
    assert(soundexCode("A") == "");
    assert(soundexCode("B") == "1");
    // Consecutive zeros
    assert(soundexCode("AEIOU") == "");
    // Duplicate after zero
    assert(soundexCode("BAB") == "1");
    // Mixed with duplicates
    assert(soundexCode("CC") == "2");
    assert(soundexCode("CXC") == "22"); // C->2, X->2, C->2, but X is 2, so second C matches previous? Let's compute: C(2) -> result "2", last=2; X(2) -> d=2, d==last skip, last=2; C(2) -> d==last skip -> result "2"? Wait careful: The raw vector for "CXC" is [2,2,2]. According to snippet: i=0 print 2; i=1: vec[1]==vec[0] skip; i=2: vec[2]==vec[1]? yes equal -> skip -> output "2". So yes it's "2". The test should be "2". Let me compute manually: C=2, X=2, C=2 -> all same and consecutive -> only first printed -> "2". So assert(soundexCode("CXC") == "2") is correct. I'll change that.
    assert(soundexCode("CXC") == "2");
    // More complex
    assert(soundexCode("DTR") == "36"); // D=3, T=3 (skip), R=6 -> "36"
    assert(soundexCode("LL") == "4");
    assert(soundexCode("LRL") == "464"); // L=4, R=6, L=4 -> all distinct -> "464"
    assert(soundexCode("MNN") == "5"); // M=5, N=5 (skip), N=5 (skip) -> "5"
    assert(soundexCode("MNMN") == "55"); // M=5,N=5(skip),M=5 (not same as previous N? previous digit is 5, but N was 5 so equal? Wait: M=5, N=5 skip, M=5 -> previous digit is 5 (from N), but N's digit was 5 and was skipped but lastDigit is still 5. So third M's d=5 equals lastDigit=5 -> skip. So output "5". Let's check: raw [5,5,5,5] all same -> output "5". So assert should be "5". I'll fix.
    assert(soundexCode("MNMN") == "5");
    // Long word with varied mapping
    assert(soundexCode("Z") == "2");
    assert(soundexCode("ZZ") == "2");
    return 0;
}
