/*
Write a C++ function that, given a non-empty string `text` consisting only of lowercase English letters, returns the maximum number of complete instances of the word `"balloon"` that can be formed using the letters in `text`. Each letter in `text` can be used at most once per instance, and you may use only the letters `b`, `a`, `l`, `o`, `n` (ignore any other characters). For example, given `"balloonballoon"` the answer is 2, and given `"balon"` the answer is 0. The function should be named `maxNumberBalloons` and take the input string by const reference.
*/

#include <string>
#include <algorithm>

// Returns the maximum number of times "balloon" can be formed from the letters in text.
int maxNumberBalloons(const std::string& text) {
    int count_b = 0;
    int count_a = 0;
    int count_l = 0;
    int count_o = 0;
    int count_n = 0;

    for (char ch : text) {
        switch (ch) {
            case 'b': ++count_b; break;
            case 'a': ++count_a; break;
            case 'l': ++count_l; break;
            case 'o': ++count_o; break;
            case 'n': ++count_n; break;
            default: break; // ignore other letters
        }
    }

    // Each "balloon" needs: 1 b, 1 a, 2 l, 2 o, 1 n.
    int max_words = count_b;
    max_words = std::min(max_words, count_a);
    max_words = std::min(max_words, count_n);
    max_words = std::min(max_words, count_l / 2);
    max_words = std::min(max_words, count_o / 2);

    return max_words;
}

#include <cassert>
#include <string>

int main() {
    // Basic cases
    assert(maxNumberBalloons("balloon") == 1);
    assert(maxNumberBalloons("balloonballoon") == 2);
    assert(maxNumberBalloons("balon") == 0);

    // Repeated letters and leftovers
    assert(maxNumberBalloons("balloonballoonballoon") == 3);
    assert(maxNumberBalloons("llooabbn") == 1); // "balloon" possible with rearrangement
    assert(maxNumberBalloons("bbaallllooottnn") == 2); // 2 b,2 a,4 l,4 o,2 n

    // Empty or irrelevant letters
    assert(maxNumberBalloons("") == 0);
    assert(maxNumberBalloons("xyz") == 0);
    assert(maxNumberBalloons("aaaa") == 0);

    // Edge: only one 'l' or 'o'
    assert(maxNumberBalloons("balon") == 0); // only one l and one o
    assert(maxNumberBalloons("ballon") == 0); // only one o

    // Mixed case: extra letters should be ignored
    assert(maxNumberBalloons("ballooon") == 0); // only two o total, but need two pairs
    assert(maxNumberBalloons("balllooon") == 1); // enough l (3) and o (3) for 1

    // Large count
    assert(maxNumberBalloons("llloooabn") == 1); // 3 l, 3 o, 1 each of b,a,n
    assert(maxNumberBalloons("lllloooobbna") == 2); // 4 l, 4 o, 1 b,1 a,1 n -> only 1 from b/a/n

    return 0;
}

// The solution directly counts the occurrences of the five relevant letters in the input string. Since the word `"balloon"` requires: one `b`, one `a`, two `l`s, two `o`s, and one `n`, the maximum number of words is limited by the smallest ratio of available count to required count for each letter. Specifically, compute `count_b`, `count_a`, `count_l`, `count_o`, `count_n`. Then the answer is `min(count_b, count_a, count_n, count_l/2, count_o/2)` using integer division. Edge cases include strings that have no required letters (answer 0), strings with only one `'l'` or `'o'` (answer 0 because division truncates), and strings with extra irrelevant letters which are ignored. The algorithm runs in O(n) time where n is the length of the input string, and uses O(1) auxiliary space.
