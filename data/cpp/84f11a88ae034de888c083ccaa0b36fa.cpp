// Write a C++ function that takes a word (a string containing only lowercase English letters) and an array of 26 positive integers, where the integer at index `i` represents the height of the corresponding letter `'a' + i` (e.g., index 0 for 'a', index 1 for 'b', ..., index 25 for 'z'). The function must return the area of the rectangular highlight of the word, which is calculated as the maximum height among all letters in the word multiplied by the word's length. For example, if the word is "abc" and the heights for 'a','b','c' are 1,3,2 respectively, the max height is 3 and the length is 3, so the area is 9. Assume the word is non-empty and heights are strictly positive.
To compute the area, first determine the length of the input word and the maximum height among its letters. Since the input word is guaranteed to be lowercase English letters, we can map each character to its corresponding height by subtracting the ASCII value of `'a'` (97) to get a zero-based index into the heights array. Iterate through each character of the string, access the height via `heights[ch - 'a']`, and track the maximum height encountered. The area then is `max_height * word_length`. Edge cases: a single-letter word yields an area equal to its own height; duplicate letters do not change the max; all heights are positive so no zero or negative values need handling. Time complexity is O(n) where n is the word length, and space complexity is O(1) excluding the input storage.
#include <string>
#include <algorithm>

// Compute the area of a rectangular highlight for a word given letter heights.
int highlightArea(const std::string& word, const int heights[26]) {
    int maxHeight = 0;
    for (char ch : word) {
        maxHeight = std::max(maxHeight, heights[ch - 'a']);
    }
    return maxHeight * static_cast<int>(word.size());
}
#include <cassert>

int main() {
    // Sample: word "abc", heights: a=1, b=3, c=2 -> max=3, len=3 -> area=9
    int h1[26] = {1,3,2};
    assert(highlightArea("abc", h1) == 9);

    // Single letter: "z" with height 5 -> max=5, len=1 -> area=5
    int h2[26] = {5};
    assert(highlightArea("z", h2) == 5);

    // Duplicate letters: "aa" with height 7 -> max=7, len=2 -> area=14
    int h3[26] = {7};
    assert(highlightArea("aa", h3) == 14);

    // All same letter: "bbbb" with height 10 -> max=10, len=4 -> area=40
    int h4[26] = {10};
    assert(highlightArea("bbbb", h4) == 40);

    // Mixed: "hello" with heights: h=1 e=2 l=3 o=4 -> max=4, len=5 -> area=20
    int h5[26] = {1,2,3,4}; // a=1,b=2,c=3,d=4, e=2, h=1, l=3, o=4
    assert(highlightArea("hello", h5) == 20);

    // Higher letters: "az" with a=1, z=9 -> max=9, len=2 -> area=18
    int h6[26] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9};
    assert(highlightArea("az", h6) == 18);
}
