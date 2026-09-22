Write a C++ function named `replaceEgyptWithSpace` that takes a non-empty string as input and returns a new string where every occurrence of the exact substring `"EGYPT"` (case-sensitive, consecutive characters) is replaced by a single space character `' '`. All other characters remain unchanged and in their original order. If multiple occurrences overlap or are adjacent, each occurrence should be replaced independently. For example, `"EEGYPT"` becomes `"E "` (the first `E` is kept, then `"EGYPT"` at positions 2–6 becomes a space). The input may contain uppercase letters, lowercase letters, digits, punctuation, and spaces; but the function should only match the uppercase `"EGYPT"` exactly, and it must not match partial overlaps like `"EGYP"` or `"GYPT"`. The function should be efficient and not modify the input string.

#include <cassert>
#include <string>

// Declare the function (already defined above, but for completeness)
std::string replaceEgyptWithSpace(const std::string& input);

int main() {
    assert(replaceEgyptWithSpace("EGYPT") == " ");
    assert(replaceEgyptWithSpace("HELLOEGYPTWORLD") == "HELLO WORLD");
    assert(replaceEgyptWithSpace("EEGYPT") == "E ");
    assert(replaceEgyptWithSpace("EGYPTEGYPT") == "  ");
    assert(replaceEgyptWithSpace("EGYP") == "EGYP");
    assert(replaceEgyptWithSpace("1 EGYPT 2") == "1  2");
    assert(replaceEgyptWithSpace("egypt") == "egypt"); // case-sensitive
    assert(replaceEgyptWithSpace("") == ""); // edge case
    assert(replaceEgyptWithSpace("XEGYPTY") == "X Y");
    assert(replaceEgyptWithSpace("EGYPT")) == " ";
    return 0;
}

#include <string>

// Replace every occurrence of the substring "EGYPT" with a single space.
std::string replaceEgyptWithSpace(const std::string& input) {
    std::string result;
    result.reserve(input.size());

    const std::string pattern = "EGYPT";
    const std::size_t pattern_len = pattern.size();

    std::size_t i = 0;
    while (i < input.size()) {
        if (i + pattern_len <= input.size() && input.compare(i, pattern_len, pattern) == 0) {
            result.push_back(' ');
            i += pattern_len;
        } else {
            result.push_back(input[i]);
            ++i;
        }
    }

    return result;
}

// The core problem is to scan the input string from left to right and, whenever we encounter a position where the substring starting at that position equals exactly `"EGYPT"`, we append a space to the output and advance the index past those 5 characters. Otherwise, we copy the current character and advance by one. The simplest and most robust way is to use a loop with an index `i` from 0 to `n-1`. For each `i`, check if `input.substr(i, 5) == "EGYPT"`. If yes, append `' '` to the result and set `i += 5` (then the loop's increment will move to `i+5+1`? Actually, careful: we should manually advance `i` by 5 before the loop increments, or use a while loop. In the reference solution, we use a while loop: while `i < n`, if the substring matches, append space and `i += 5`; else append `input[i]` and `i++`. This avoids any off-by-one errors. Edge cases: empty string (but task says non-empty, though function can handle it), input shorter than 5 characters (no match), overlapping occurrences like `"EEGYPT"` – only the second `E` starts a match, the first `E` is copied, then space. For `"EGYPTEGYPT"`, we get two spaces. For `"EGYPT"` exactly, we get one space. For `"EG"` only, we copy both. Time complexity is O(n * m) where m=5, effectively O(n) since m is constant. Space complexity is O(n) for the output string, plus O(1) auxiliary.
