// Given a string `s` that contains only lowercase English letters, write a C++ function `countHeavyMetalPairs` that returns the number of pairs `(i, j)` such that `i < j`, the substring `s[i..i+4]` equals `"heavy"`, and the substring `s[j-4..j]` equals `"metal"`. Substrings may overlap within the string, but each occurrence of `"heavy"` and `"metal"` is a distinct position. The function must handle strings of length up to 1,000,000 efficiently, and ensure that the total count fits in a 64‑bit integer (use `long long`). The input string is guaranteed to contain only lowercase letters.
The key observation is to count how many `"heavy"` occurrences appear before each `"metal"` occurrence. A direct double loop would be O(n^2) and too slow for large strings. Instead, we can scan the string from left to right, counting the number of `"heavy"` occurrences seen so far. Whenever we encounter a `"metal"` substring starting at index `j-4` (so the last character is at `j`), we add the current count of `"heavy"` occurrences to the answer, because every previously seen `"heavy"` occurs before this `"metal"` (and all distinct positions satisfy `i < j`). This works because scanning left to right guarantees all `"heavy"` occurrences counted so far start at indices strictly less than the current `"metal"` start index. Edge cases: if the string length is less than 5, no valid substrings exist; also substrings can overlap, e.g., `"heavyheavy"` contains two distinct `"heavy"` occurrences with start indices 0 and 5, which is fine. Time complexity is O(n) with O(1) auxiliary space (not counting input/output). We must use `long long` for the answer because the maximum number of pairs is roughly (n/5)^2, which for n=10^6 is about 4×10^10, exceeding 32-bit range.
#include <string>

// Returns the number of pairs (heavy, metal) where heavy occurs before metal.
// heavy = substring "heavy" starting at index i, metal = substring ending at j (j-4 is start).
long long countHeavyMetalPairs(const std::string& s) {
    const std::string heavy = "heavy";
    const std::string metal = "metal";
    long long heavyCount = 0;
    long long result = 0;
    const int n = static_cast<int>(s.size());

    // Scan left to right. For each position, check if a "heavy" starts here.
    // If so, increment heavyCount. If a "metal" ends here (starts at i-4), add heavyCount to result.
    for (int i = 0; i + 5 <= n; ++i) {
        // Check for heavy starting at i
        if (s.compare(i, heavy.size(), heavy) == 0) {
            ++heavyCount;
        }
        // Check for metal ending at i+4 (i.e., starting at i)
        if (s.compare(i, metal.size(), metal) == 0) {
            result += heavyCount;
        }
    }

    return result;
}
#include <cassert>
#include <string>

// forward declaration of the tested function
long long countHeavyMetalPairs(const std::string& s);

int main() {
    // Basic case with one of each
    assert(countHeavyMetalPairs("heavy metal") == 1);
    // Reverse order gives zero because heavy must occur before metal
    assert(countHeavyMetalPairs("metal heavy") == 0);
    // Two heavies after one metal: only the metal before both heavies? Actually metal comes first, so zero pairs
    assert(countHeavyMetalPairs("metal heavy heavy") == 0);
    // One heavy then two metals -> two pairs
    assert(countHeavyMetalPairs("heavy metal metal") == 2);
    // Two heavies then one metal -> two pairs
    assert(countHeavyMetalPairs("heavy heavy metal") == 2);
    // Overlapping substrings: "heavyheavy" contains two heavies, no metals -> zero
    assert(countHeavyMetalPairs("heavyheavy") == 0);
    // "metalmetal" contains two metals, no heavies -> zero
    assert(countHeavyMetalPairs("metalmetal") == 0);
    // Empty and short strings
    assert(countHeavyMetalPairs("") == 0);
    assert(countHeavyMetalPairs("abc") == 0);
    // "heavy" exactly then "metal" exactly
    assert(countHeavyMetalPairs("heavymetal") == 1);
    // Heavy and metal mixed non-contiguously
    assert(countHeavyMetalPairs("hheavyymetall") == 1); // the substring "heavy" appears at index 1, "metal" appears at index 7? Actually "h"+"heavy"+"my"+"metal"+"l" → one heavy at idx1, one metal at idx? Let's check: string = "hheavyymetall" length 13, "heavy" at indices 1-5? s[1]='h', s[2]='e', s[3]='a', s[4]='v', s[5]='y'? Actually char positions: 0:'h',1:'h',2:'e',3:'a',4:'v',5:'y',6:'y',7:'m',8:'e',9:'t',10:'a',11:'l',12:'l'. So "heavy" at idx1 (1-5) works, "metal" at idx7 (7-11) works → 1 pair.
    assert(countHeavyMetalPairs("hheavyymetall") == 1);
    // Large count test: small synthetic: "heavy" * 3 then "metal" * 3 → each of 3 heavies pairs with each of 3 metals = 9
    assert(countHeavyMetalPairs("heavyheavyheavymetalmetalmetal") == 9);
    return 0;
}
