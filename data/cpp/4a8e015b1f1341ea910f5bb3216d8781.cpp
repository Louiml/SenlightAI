Write a C++ function that models a simplified line-breaking algorithm inspired by the BudouX word segmentation engine. The function `findBreakPositions(const std::vector<std::string>& words, const std::map<std::string, int>& uwModel, const std::map<std::string, int>& bwModel, const std::map<std::string, int>& twModel, int negativeSum) → std::vector<size_t>` returns the zero-based indices after which a line break should be inserted. The algorithm processes a sliding window of 6 consecutive words (similar to how the original processes Unicode characters), and for each position (except the first and last), it computes a score by summing contributions from: (1) single-word features `UW1`–`UW6` for each of the 6 words in the window, where the key is `"UW" + to_string(i) + ":" + word`; (2) bigram features `BW1`–`BW3` for three adjacent word pairs, with keys `"BW" + to_string(i) + ":" + pair.first + ":" + pair.second`; and (3) trigram features `TW1`–`TW4` for four adjacent word triples, with keys `"TW" + to_string(i) + ":" + w1 + ":" + w2 + ":" + w3`. Each feature contributes its stored integer score if present, otherwise 0. The base score is `negativeSum` (which represents a negative bias). If the total score is strictly greater than 0, that position is a breakpoint. The function must handle edge cases: windows that extend beyond the beginning or end of the word list (treat missing words as not contributing any features), sequences with fewer than 6 words (still correctly evaluate available features), and duplicate words in the list. The function must return breakpoints in increasing order, excluding index 0 and index `n-1` (if the total score at those boundary positions would be evaluated, they should not be reported because no break can occur before the first word or after the last word in this simplified model). Provide a Standalone implementation with only standard library headers.
The core idea is to mimic the sliding-window approach from the original code, but simplified to operate on words rather than Unicode code points. We are given vectors/maps of pre-computed feature scores. The `negativeSum` parameter represents the initial bias added to every score calculation; the original code computes `fNegativeSum` as half the total of all negative values in the model, and we treat it as a supplied constant. For each candidate break position `p` (where `p` is the index of the last word before the break, ranging from 0 to n-2), we need to extract features from words at positions `p-2, p-1, p, p+1, p+2, p+3` (the original uses indices `[startIdx .. startIdx+5]` where `startIdx = p-2`). However, to match the original code’s indexing, note that the original evaluates the breakpoint after the character at index `startIdx+3` (the 4th of the 6 windows). So for a break after word index `k` (0-based), we set `startIdx = k - 3`, and then the 6-word window is `[startIdx .. startIdx+5]` = `[k-3 .. k+2]`. But wait, this seems off because the original processes characters 0-based and evaluates break after the 4th character in a 6-character window: window chars [i, i+1, i+2, i+3, i+4, i+5], break after i+3. Translating to words, we want break after word at index `j` (0-based). So we need window words at indices `j-3, j-2, j-1, j, j+1, j+2`? That gives 6 words: positions `[j-3, j-2, j-1, j, j+1, j+2]` — yes, because the break is after the 4th word (index j). Let’s verify: if break after index 3 (4th word), window should be indices 0,1,2,3,4,5 — so `j-3=0` works. So for break candidate `j` (where `0 <= j <= n-2`), window start = `j-3`. But then for `j=0` (break after first word), window start = -3, which means we need to handle out-of-bounds by ignoring those features. The original code uses an index list initialized with -1 for out-of-bounds positions and skips features that reference invalid indices. In our simplified version, we can iterate over the window positions from `windowStart = j-3` to `windowStart+5`, and for each valid word index `idx` within [0, n-1], we build feature keys and look them up. For single-word features: `UW1` corresponds to window position 0, `UW2` to position 1, ..., `UW6` to position 5. So key = `"UW" + to_string(winPos+1) + ":" + word`. For bigrams: `BW1` uses words at window positions 1 and 2 (i.e., indices `windowStart+1` and `windowStart+2`), `BW2` uses positions 2 and 3, `BW3` uses positions 3 and 4. But in the original, `BW` loop uses `start = startIdx + i + 1` where `i` from 0 to 2, so BW1 uses indices `startIdx+1` and `startIdx+2`? Actually let’s read original: for BW loop: `start = startIdx + i + 1;` and then checks `indexList[start]` and `indexList[start+1]` and then uses substring from indexList[start] to end where end = indexList[start+2]? Wait that’s confusing. In original, `indexList` holds code unit offsets, not word indices. Let’s reinterpret for our simplified model: In the original, the window covers 6 characters (code points) at positions `startIdx` to `startIdx+5`. It evaluates break after position `startIdx+3`. For UW features, it uses each individual character at positions `startIdx+0` through `startIdx+5`? Actually the code does `for i=0..5`: `start = startIdx + i;` and then uses substring from indexList[start] to indexList[start+1] (or numCodeUnits if next is -1). So UW1 uses character at position `startIdx`, UW2 at `startIdx+1`, etc. For BW features, it does `for i=0..2`: `start = startIdx + i + 1;` so BW1 uses character at `startIdx+1` and then substring from that character to the next character (`indexList[start+1]`)? Actually it uses `indexList[start]` as start and `end` as `indexList[start+1]` or next valid? Let’s re-read: 
```
for (int i = 0; i < 3; i++) { // BW1 ~ BW3
    start = startIdx + i + 1;
    if (indexList[start] != -1 && indexList[start+1] != -1) {
        end = (indexList[start+2] != -1) ? indexList[start+2] : numCodeUnits;
        score += fModel[...].geti(inString.tempSubString(indexList[start], end - indexList[start]));
    }
}
```
Hmm, that uses `indexList[start]` as the beginning and `end` as `indexList[start+2]`? That looks like it’s taking the substring from character at `startIdx+i+1` to character at `startIdx+i+3`, which is actually a trigram? Wait, maybe it’s a sliding window of two characters? Let’s interpret: `indexList[start]` points to the code unit offset of character at position `startIdx+i+1` (call it c1). `indexList[start+1]` points to c2 (next character). `indexList[start+2]` points to c3 (next). The substring from `indexList[start]` to `end` (where `end = indexList[start+2]` if valid) covers characters c1 and c2 (two characters). So that is indeed a bigram of two consecutive characters. So BW1 is bigram of characters at positions `startIdx+1` and `startIdx+2`; BW2 is positions `startIdx+2` and `startIdx+3`; BW3 is positions `startIdx+3` and `startIdx+4`. Similarly for TW features: `start = startIdx + i` for i=0..3, and it accesses `indexList[start]`, `indexList[start+1]`, `indexList[start+2]` and uses substring from `indexList[start]` to `end` where `end = indexList[start+3]` (if valid). So TW1 uses characters at positions `startIdx+0, startIdx+1, startIdx+2` (a trigram), TW2 uses positions `1,2,3`, TW3 uses `2,3,4`, TW4 uses `3,4,5`. So the mapping is: for a break after word index `j` (where `0 <= j <= n-2`), set `startIdx = j-3` (so that the break is after the 4th element of the window). The window positions 0..5 correspond to word indices `startIdx + p` (where p=0..5). Then features are:
- UW(p) for p=0..5: word at index `startIdx+p`.
- BW(i) for i=0..2: bigram of words at `(startIdx+i+1, startIdx+i+2)`.
- TW(i) for i=0..3: trigram of words at `(startIdx+i, startIdx+i+1, startIdx+i+2)`.
When any required word index is out of bounds (less than 0 or >= n), treat that feature as absent (skip it). We compute total score = `negativeSum` + sum of all contributing feature values (only if the key exists in the corresponding map). If total > 0, then break after word index `j` (i.e., push `j+1` as the break index? The task says "indices after which a line break should be inserted". So if break after word at index `j`, we return `j+1` as the break position. But we should exclude `j=0` and `j=n-1`? Actually break after the first word is `j=0` → return 1; break after last word is `j=n-1` → return n. But the original always adds a break at start (0) and end (length). Since the task says "excluding index 0 and index n-1", that likely means we should not consider inserting a break before the first word (position 0) or after the last word (position n). But a break after the first word is position 1, which is allowed. The instruction says "excluding index 0 and index n-1" – maybe it means the break positions themselves? The example output in the task description? Let’s interpret: The function returns breakpoints as positions in the word list (1-based, i.e., number of words before the break). So possible breakpoints are 1 through n-1 (since a break after the last word is n, but that is not a meaningful break in this context; but the task says exclude n-1? That would exclude the break after the second-to-last word? That seems odd. Let’s re-read: "The function must return breakpoints in increasing order, excluding index 0 and index n-1 (if the total score at those boundary positions would be evaluated, they should not be reported because no break can occur before the first word or after the last word in this simplified model)." So index 0 means break before first word (position 0), and index n-1 means break before last word? Actually break before last word would be position n-1 (if words are 0-indexed, break after word at index n-2 gives position n-1). The instruction says exclude index 0 and index n-1. So we evaluate break candidates for `j` from 0 to n-1, compute score, but only report break positions that are strictly greater than 0 and strictly less than n-1? Wait, position values are 1..n? Let’s define: The `return vector<size_t>` contains positions where a break occurs, where position `p` means "there is a break between word `p-1` and word `p`" (i.e., after `p` words). So for n words, valid break positions are 1..n-1 (a break after the last word, position n, is not allowed; a break before the first, position 0, is not allowed). The instruction says "excluding index 0 and index n-1" – but n-1 is actually a valid position (break after the second-to-last word). Hmm, maybe they mean excluding the candidates where `j=0` (break after first word?) That would be position 1. Not clear. Let’s interpret the instruction literally: "excluding index 0 and index n-1" as break positions. So we should filter out position 0 and position n-1 from the result. But position n-1 is a valid break in normal line breaking (after the second-to-last word, before the last word). However, the text says "because no break can occur before the first word or after the last word". Aaah, they mean: if we consider break candidates at word indices, the candidate for break before first word (index -1) and after last word (index n-1?) No, let's clarify: In the original algorithm, it adds a break at the start (position 0) and end (position length). We are not adding those. The instruction: "The function must ... excluding index 0 and index n-1" – Probably they mean: For a list of n words, we evaluate breakpoints at positions 1,2,...,n-1 (i.e., after word 0, after word 1, ..., after word n-2). But they also want to exclude the trivial boundaries: position 0 (before first word) and position n (after last word). However they wrote "index n-1" which is ambiguous. I’ll re-read: "The function must return breakpoints in increasing order, excluding index 0 and index n-1 (if the total score at those boundary positions would be evaluated, they should not be reported because no break can occur before the first word or after the last word in this simplified model)." In a 0-based index list of words, index 0 is the first word, index n-1 is the last word. A "breakpoint" between words is typically denoted by the index of the word after the break. So a break between word at index i and i+1 is denoted as position i+1. So index 0 would mean break before first word (i=-1), index n-1 would mean break between word n-2 and n-1? That’s a valid location. But they say "no break can occur before the first word or after the last word" – so maybe they think: break at index 0 means before first word (i.e., position 0), and break at index n-1 means after last word (position n-1)?? That is confusing. Let’s look at the example from the original code: They add a break for start (0) and end (length). So we should not add those. The instruction says "excluding index 0 and index n-1" – since we are iterating over break candidates where the break is after word at index j (j from 0 to n-2), the resulting break position is j+1 (ranges 1..n-1). Excluding index 0 means we skip the break after word at index -1 (not possible). Excluding index n-1 means we skip the break after word at index n-2? That would give position n-1, but position n-1 is a valid break (after second-to-last word). It seems they want to exclude the break at the very end (after last word) which is position n, but they wrote n-1. I’ll assume the intended meaning is: We should only consider break positions strictly between 0 and n (i.e., 1..n-1), and also that we do not report a break at position n (after last word) even if score is positive. But since we never evaluate break after last word (j=n-1 would require window beyond end, but we could evaluate it and get score; but it’s not a break we want). The simplest interpretation: Evaluate for j = 0 to n-1 (where j is the index after which break would go, so position j+1). But we only report those where j+1 is between 1 and n-1 inclusive, but not 0 and not n? Actually n is position after last word, exclude that. So we exclude j = n-1 (which gives position n). Also exclude j = -1 (not possible). So we iterate j from 0 to n-1 inclusive, compute score, and if score>0 and j < n-1, then push j+1. But j=0 gives position 1, which is allowed. So the only exclusion is j = n-1 (break after last word) and j = -1 (not iterated). That matches "before the first word or after the last word". So the condition: only report if score>0 and (j+1 < n). Because j+1 == n means after last word. So we report all positions from 1 to n-1 (inclusive) where score>0. That seems correct.

So the algorithm: 
- Input: vector<string> words (size n ≥ 1)
- Maps: uw, bw, tw (string→int), negativeSum (int)
- For each j from 0 to n-1 (but we will restrict later) compute score.
- We can compute for j from 0 to n-2 (since j=n-1 gives position n, which we skip). Actually we can compute up to n-2, because position n is never valid. But we also need to skip if n <= 1? If n==1, no breaks possible. If n==0, return empty.
- For each j from 0 to n-2 (inclusive), compute startIdx = j-3.
- Then for p=0..5, word index = startIdx+p; if valid, key = "UW"+to_string(p+1)+":"+words[wordIdx]; add if found.
- For i=0..2: w1 = startIdx+i+1, w2 = startIdx+i+2; if both valid, key = "BW"+to_string(i+1)+":"+words[w1]+":"+words[w2]; add if found.
- For i=0..3: w1=startIdx+i, w2=startIdx+i+1, w3=startIdx+i+2; if all valid, key = "TW"+to_string(i+1)+":"+words[w1]+":"+words[w2]+":"+words[w3]; add if found.
- score = negativeSum + sum
- If score > 0 and (j+1) < n (always true since j <= n-2), push j+1.
Return vector.

Edge cases: n=0, n=1, n=2 (window smaller than 6, some features missing). Duplicate words are fine because keys are based on content. The maps may not contain all keys; missing keys contribute 0. There is no restriction on the number of words; we iterate all break candidates.

Time complexity: For n words, we process O(n) break candidates, each with constant number of feature lookups (up to 6+3+4=13 lookups). Each lookup is O(1) average for std::map (O(log M) where M is map size) but we can say O(1) if we consider constant. So O(n) time. Space: O(1) extra besides the returned vector (which can have up to n elements). Actually for worst case, up to n breakpoints, so O(n) space for output.

Now write the solution function. We'll name it `findBreakPositions`. Use `std::vector<size_t>` returns. Use `std::map` for feature maps. The function signature:
```cpp
std::vector<size_t> findBreakPositions(
    const std::vector<std::string>& words,
    const std::map<std::string, int>& uwModel,
    const std::map<std::string, int>& bwModel,
    const std::map<std::string, int>& twModel,
    int negativeSum
);
```
We'll include `<vector>`, `<string>`, `<map>`, `<cstddef>`. We'll write a helper lambda to find value from map with fallback 0.

Careful: negativeSum is an int, score can overflow? Probably not a concern for tests.

Now produce the solution code as per instruction: only output code in section, no main. Then provide test code in section with a main that uses assert.

Let's create a small test scenario. For simplicity, we'll create feature maps with a few entries and verify break positions. Example: words = {"A","B","C","D","E","F"}. We want break after "C" (position 3). Set negativeSum = -5, and add features such that score for j=2 (break after index 2, position 3) is positive. For window startIdx = j-3 = -1, so window positions: -1 (invalid),0,1,2,3,4 for UW1..UW6? Actually startIdx=-1, so UW1 invalid, UW2 = words[0]="A", UW3="B", UW4="C", UW5="D", UW6="E". Also BW1 uses indices startIdx+1=0 and startIdx+2=1 → "A","B"; BW2 uses 1,2 → "B","C"; BW3 uses 2,3 → "C","D". TW1 uses -1,0,1 invalid; TW2 uses 0,1,2 → "A","B","C"; TW3 uses 1,2,3 → "B","C","D"; TW4 uses 2,3,4 → "C","D","E". So we can set negativeSum = -10, and add feature "BW2:B:C" = 11 (score becomes -10+11=1, positive), and no other features. Then j=2 gives score 1, push 3. For other j, score <=0, so no breaks. That would work. But also need to test edge with n<6. Let's write a comprehensive test.

We'll write test with 3 asserts: one with n=6, one with n=2 (break after first word? For n=2, j=0 gives position 1, check if we can make it positive), one with n=1 (no breaks). Also test with all words identical? Not necessary.

code:
```cpp
#include <cassert>
#include <map>
#include <string>
#include <vector>

// Assume solution function is defined above.

int main() {
    // Test 1: n=6, break after "C" (position 3)
    std::vector<std::string> words1 = {"A","B","C","D","E","F"};
    std::map<std::string,int> uw1; // empty
    std::map<std::string,int> bw1;
    bw1["BW2:B:C"] = 11;
    std::map<std::string,int> tw1;
    auto breaks1 = findBreakPositions(words1, uw1, bw1, tw1, -10);
    assert((breaks1 == std::vector<size_t>{3}));

    // Test 2: n=2, break after first word (position 1) using UW2 feature
    std::vector<std::string> words2 = {"hello","world"};
    std::map<std::string,int> uw2;
    uw2["UW2:hello"] = 5;
    std::map<std::string,int> bw2;
    std::map<std::string,int> tw2;
    auto breaks2 = findBreakPositions(words2, uw2, bw2, tw2, -4);
    assert((breaks2 == std::vector<size_t>{1}));

    // Test 3: n=1, no possible breaks
    std::vector<std::string> words3 = {"only"};
    auto breaks3 = findBreakPositions(words3, uw2, bw2, tw2, 100);
    assert(breaks3.empty());

    // Test 4: n=3, no break when score not positive
    std::vector<std::string> words4 = {"a","b","c"};
    auto breaks4 = findBreakPositions(words4, uw2, bw2, tw2, 10); // all features 0, score=10>0 for every j? Wait negativeSum=10, no features, so score=10 always >0, thus breaks at 1 and 2? But j=1 gives position 2 (n-1=2, allowed). So breaks {1,2}
    assert((breaks4 == std::vector<size_t>{1,2}));
}
```
But wait in test 4, negativeSum=10, no features, so score=10>0 for all j=0 and j=1 (n=3 so j from 0 to 1). So breaks at 1 and 2. That tests negativeSum positive.

We also need to test that we do not include break at position 0 or position n. Since we only iterate j from 0 to n-2, j=0 gives position 1, which is fine. j=n-1 is not iterated, so no break at n. That's covered.

Also test that features outside map contribute 0. Could add a test where a feature key missing causes no break. But enough.

Now produce the final response with exact sections.

Make sure the solution function is self-contained, uses `const` references, and includes all necessary headers. We'll use `std::map` but could also use `std::unordered_map` for efficiency; but the task says maps, we'll use `std::map`.

We'll write helper lambda inside the function:
```cpp
auto getScore = [](const std::map<std::string,int>& m, const std::string& key) -> int {
    auto it = m.find(key);
    return (it != m.end()) ? it->second : 0;
};
```

Then implement.

Make sure to handle n==0 or n==1 early return.

Write solution code now.
#include <cstddef>
#include <map>
#include <string>
#include <vector>

// Simplified BudouX-style word break engine.
// Returns a vector of break positions (1-based: position p means a break after the p-th word)
// for which the computed score is positive, excluding position 0 (before first word)
// and position n (after last word) by construction.
std::vector<size_t> findBreakPositions(
    const std::vector<std::string>& words,
    const std::map<std::string, int>& uwModel,
    const std::map<std::string, int>& bwModel,
    const std::map<std::string, int>& twModel,
    int negativeSum) {

    std::vector<size_t> result;
    const size_t n = words.size();
    if (n <= 1) return result;  // No internal breaks possible.

    // Helper to look up a feature value; missing features contribute 0.
    auto getScore = [](const std::map<std::string, int>& model, const std::string& key) -> int {
        auto it = model.find(key);
        return (it != model.end()) ? it->second : 0;
    };

    // j is the index of the last word before the candidate break.
    // The break position (number of words before the break) is j+1.
    // We only evaluate j from 0 to n-2 (position 1 through n-1).
    for (size_t j = 0; j + 1 < n; ++j) {
        // The sliding window starts at startIdx = j - 3 (int to allow negative values).
        int startIdx = static_cast<int>(j) - 3;
        int score = negativeSum;

        // Single-word features UW1..UW6.
        for (int p = 0; p < 6; ++p) {
            int wordIdx = startIdx + p;
            if (wordIdx >= 0 && static_cast<size_t>(wordIdx) < n) {
                std::string key = "UW" + std::to_string(p + 1) + ":" + words[wordIdx];
                score += getScore(uwModel, key);
            }
        }

        // Bigram features BW1..BW3.
        for (int i = 0; i < 3; ++i) {
            int idx1 = startIdx + i + 1;
            int idx2 = startIdx + i + 2;
            if (idx1 >= 0 && static_cast<size_t>(idx1) < n &&
                idx2 >= 0 && static_cast<size_t>(idx2) < n) {
                std::string key = "BW" + std::to_string(i + 1) + ":" +
                                  words[idx1] + ":" + words[idx2];
                score += getScore(bwModel, key);
            }
        }

        // Trigram features TW1..TW4.
        for (int i = 0; i < 4; ++i) {
            int idx1 = startIdx + i;
            int idx2 = startIdx + i + 1;
            int idx3 = startIdx + i + 2;
            if (idx1 >= 0 && static_cast<size_t>(idx1) < n &&
                idx2 >= 0 && static_cast<size_t>(idx2) < n &&
                idx3 >= 0 && static_cast<size_t>(idx3) < n) {
                std::string key = "TW" + std::to_string(i + 1) + ":" +
                                  words[idx1] + ":" + words[idx2] + ":" + words[idx3];
                score += getScore(twModel, key);
            }
        }

        // If the total score is strictly positive, report break position j+1.
        if (score > 0) {
            result.push_back(j + 1);
        }
    }

    return result;
}
#include <cassert>
#include <map>
#include <string>
#include <vector>

// (Assume findBreakPositions from the solution above is available.)

int main() {
    // Test 1: Six words, break after "C" (position 3) using a bigram feature.
    {
        std::vector<std::string> words = {"A","B","C","D","E","F"};
        std::map<std::string,int> uw;
        std::map<std::string,int> bw;
        bw["BW2:B:C"] = 11;
        std::map<std::string,int> tw;
        auto breaks = findBreakPositions(words, uw, bw, tw, -10);
        assert((breaks == std::vector<size_t>{3}));
    }

    // Test 2: Two words, break after first (position 1) using a unigram feature.
    {
        std::vector<std::string> words = {"hello","world"};
        std::map<std::string,int> uw;
        uw["UW2:hello"] = 5;
        std::map<std::string,int> bw;
        std::map<std::string,int> tw;
        auto breaks = findBreakPositions(words, uw, bw, tw, -4);
        assert((breaks == std::vector<size_t>{1}));
    }

    // Test 3: Single word, no possible breaks.
    {
        std::vector<std::string> words = {"only"};
        std::map<std::string,int> uw;
        std::map<std::string,int> bw;
        std::map<std::string,int> tw;
        auto breaks = findBreakPositions(words, uw, bw, tw, 100);
        assert(breaks.empty());
    }

    // Test 4: Three words, positive bias produces breaks after every word (positions 1 and 2).
    {
        std::vector<std::string> words = {"a","b","c"};
        std::map<std::string,int> uw;
        std::map<std::string,int> bw;
        std::map<std::string,int> tw;
        auto breaks = findBreakPositions(words, uw, bw, tw, 10);
        assert((breaks == std::vector<size_t>{1,2}));
    }

    // Test 5: Five words, negative bias prevents any break even with features.
    {
        std::vector<std::string> words = {"a","b","c","d","e"};
        std::map<std::string,int> uw;
        uw["UW3:c"] = 5;
        std::map<std::string,int> bw;
        std::map<std::string,int> tw;
        auto breaks = findBreakPositions(words, uw, bw, tw, -100);
        assert(breaks.empty());
    }

    // Test 6: Ensure no break is reported at position 0 or at position n.
    // For 4 words, with very high bias, we should get {1,2,3} but not {0} or {4}.
    {
        std::vector<std::string> words = {"x","y","z","w"};
        std::map<std::string,int> uw;
        std::map<std::string,int> bw;
        std::map<std::string,int> tw;
        auto breaks = findBreakPositions(words, uw, bw, tw, 1000);
        assert((breaks == std::vector<size_t>{1,2,3}));
    }
}
