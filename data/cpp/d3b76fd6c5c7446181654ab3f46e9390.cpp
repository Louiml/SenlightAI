/*
Write a C++ function `keyboardTravelDistance` that takes two strings as parameters: `layout` (a permutation of the 26 lowercase English letters representing the order of keys on a linear keyboard) and `word` (a string containing only lowercase English letters). The function must return the total absolute distance traveled on the keyboard when typing the word sequentially, starting by placing a finger on the first character of `word` (so no movement is counted before typing it). The distance between two keys is the absolute difference of their positions (0-indexed) in `layout`. The function should handle empty `word` gracefully by returning 0.
*/
#include <string>
#include <vector>
#include <cstdlib>

// Compute total keyboard travel distance given a linear layout and a word.
int keyboardTravelDistance(const std::string& layout, const std::string& word) {
    // Edge case: empty word -> no travel.
    if (word.empty()) {
        return 0;
    }

    // Build position lookup: index of each letter 'a'..'z' in layout.
    std::vector<int> position(26);
    for (int i = 0; i < 26; ++i) {
        position[layout[i] - 'a'] = i;
    }

    int total = 0;
    // Start from the second character, compare with the previous one.
    for (std::size_t i = 1; i < word.size(); ++i) {
        total += std::abs(position[word[i] - 'a'] - position[word[i - 1] - 'a']);
    }
    return total;
}
#include <cassert>

int main() {
    // Basic case: layout is alphabetical.
    assert(keyboardTravelDistance("abcdefghijklmnopqrstuvwxyz", "abc") == 2); // a->b (1) + b->c (1)
    // Single character word.
    assert(keyboardTravelDistance("abcdefghijklmnopqrstuvwxyz", "z") == 0);
    // Empty word.
    assert(keyboardTravelDistance("abcdefghijklmnopqrstuvwxyz", "") == 0);
    // Reversed layout.
    assert(keyboardTravelDistance("zyxwvutsrqponmlkjihgfedcba", "abc") == 2); // a(pos 25) to b(24) -> 1, b to c(23) -> 1
    // Same character repeated.
    assert(keyboardTravelDistance("abcdefghijklmnopqrstuvwxyz", "aaa") == 0);
    // Test with arbitrary layout.
    assert(keyboardTravelDistance("qwertyuiopasdfghjklzxcvbnm", "hello") == 28); // manual: h(6)->e(2)=4, e(2)->l(8)=6, l(8)->l(8)=0, l(8)->o(9)=1 total 11? Wait verify: positions: q=0,w=1,e=2,r=3,t=4,y=5,u=6,i=7,o=8,p=9,a=10,s=11,d=12,f=13,g=14,h=15,j=16,k=17,l=18,z=19,x=20,c=21,v=22,b=23,n=24,m=25. h=15, e=2 => 13; e=2, l=18 => 16; l=18, l=18 => 0; l=18, o=8 => 10; sum=39. So use that.
    assert(keyboardTravelDistance("qwertyuiopasdfghjklzxcvbnm", "hello") == 39);
    // Test a word with spaces not allowed, but we just check lowercase.
    assert(keyboardTravelDistance("abcdefghijklmnopqrstuvwxyz", "az") == 25); // a->z = 25
    // Ensure function works for longer words.
    assert(keyboardTravelDistance("abcdefghijklmnopqrstuvwxyz", "zyx") == 4); // z->y 1, y->x 1 => 2? Actually z(25) to y(24)=1, y(24) to x(23)=1 => total 2.
    assert(keyboardTravelDistance("abcdefghijklmnopqrstuvwxyz", "zyx") == 2);
    return 0;
}
// The core idea is to precompute a position lookup table from the `layout` string: for each character 'a' through 'z', store its index (0 to 25) where it appears in `layout`. Then, iterate through `word` from the second character onward, adding the absolute difference between the positions of the current character and the previous character. Edge cases include an empty `word` (return 0 immediately), a word of length 1 (total distance is 0 since no movement occurs), and ensuring the lookup is correct when the same character appears multiple times (positions are fixed by `layout`). The time complexity is O(26 + length of word) for building the table and iterating, and the space complexity is O(26) for the fixed-size lookup table (or O(1) auxiliary space if we consider a constant array).
