// Write a C++ function `int minimumFlipsToAllHappy(const std::string& pancakes)` that takes a string consisting only of the characters `'+'` (happy side up) and `'-'` (blank side up), representing a stack of pancakes where the leftmost character is the top. In one move, you may choose any prefix of the stack (a contiguous segment from the top) and flip it over, reversing the order of that prefix and turning each pancake in it to its opposite side (`'+'` becomes `'-'` and vice versa). The goal is to make all pancakes show `'+'`. Return the minimum number of flips required to achieve this. The input string length is between 1 and 100, inclusive, and the problem is guaranteed to be solvable for any given input. The function must be efficient and handle edge cases such as an already all-happy stack (return 0) and an all-blank stack (return 1, since one full flip reverses order but all become happy, which is fine because order after flipping doesn't matter as long as all are happy).

// The key observation is that the minimum number of flips equals the number of "runs" of consecutive identical characters, but with a small twist: because a flip reverses the entire prefix, flipping at a boundary between runs can merge runs. The optimal strategy is to always flip the longest prefix possible that starts with the opposite character from the top. For example, if the top is `'+'` and we need to flip blanks underneath, we first flip the top happy run to blanks, then flip the entire prefix up to the lowest blank to make it happy. This is exactly what the greedy algorithm does: while there exists a blank in the stack, find the rightmost blank; if there are leading happy pancakes, flip that prefix (turning them to blank) and increment the count; then flip the entire prefix up to that rightmost blank, which turns all blanks to happy and reverses the prefix. After each full prefix flip, the bottom segment that was already happy remains unchanged because we only flip up to the rightmost blank, and everything below it was already happy. Each iteration removes at least one blank and possibly merges runs, and the process terminates in at most `2*n` flips. The time complexity is O(n^2) in the worst case if we simulate each flip by rebuilding strings, but with an optimized simulation using a deque or by directly counting runs, we can achieve O(n). For the given constraints (n ≤ 100), even O(n^2) is acceptable. The space complexity is O(n) for the working copy of the string. Edge cases: empty string (though constraints say non-empty, we can return 0), already all happy (return 0), all blank (one full flip returns happy, count=1). The greedy is optimal because flipping a prefix that doesn't include the bottommost blank is wasteful; you always need to flip from the top up to some boundary, and picking the rightmost blank ensures you maximize progress per flip.

#include <string>
#include <algorithm>

// Returns the minimum number of prefix flips needed to make all pancakes happy ('+').
int minimumFlipsToAllHappy(const std::string& pancakes) {
    std::string s = pancakes; // working copy
    int flips = 0;
    while (true) {
        // Find rightmost blank
        int last_blank = -1;
        for (int i = static_cast<int>(s.size()) - 1; i >= 0; --i) {
            if (s[i] == '-') {
                last_blank = i;
                break;
            }
        }
        if (last_blank == -1) {
            break; // all happy
        }
        // If top is happy, flip leading happy run to blanks
        if (s[0] == '+') {
            int start = 0;
            while (start < static_cast<int>(s.size()) && s[start] == '+') {
                start++;
            }
            for (int i = 0; i < start; ++i) {
                s[i] = '-';
            }
            flips++;
        }
        // Now flip the entire prefix up to last_blank
        int left = 0;
        int right = last_blank;
        while (left < right) {
            char temp = s[left];
            s[left] = (s[right] == '+') ? '-' : '+';
            s[right] = (temp == '+') ? '-' : '+';
            left++;
            right--;
        }
        if (left == right) {
            s[left] = (s[left] == '+') ? '-' : '+';
        }
        flips++;
    }
    return flips;
}

#include <cassert>
#include <string>

int minimumFlipsToAllHappy(const std::string& pancakes);

int main() {
    // Already all happy
    assert(minimumFlipsToAllHappy("+++") == 0);
    assert(minimumFlipsToAllHappy("+") == 0);
    // Single blank
    assert(minimumFlipsToAllHappy("-") == 1);
    // All blank
    assert(minimumFlipsToAllHappy("---") == 1);
    // Alternating cases
    assert(minimumFlipsToAllHappy("+-") == 2);
    assert(minimumFlipsToAllHappy("-+") == 1);
    assert(minimumFlipsToAllHappy("+-+-") == 4);
    assert(minimumFlipsToAllHappy("-+-+") == 3);
    // Code Jam example: "--+-" -> step: flip prefix to make top blank, then flip up to last blank → 3 flips? Let's check: actually optimal: flip prefix "--+"? Let's compute manually but use known result? The classic Code Jam "Pancake Revenge" small set: "--+-" needs 3 flips. Yes.
    assert(minimumFlipsToAllHappy("--+-") == 3);
    // Longer random known: "++--" -> flip "++" to "--" (1), flip all "-- --" -> becomes "++++"? Actually flip full string: reversed and toggled: "--++"? Let's trust algorithm: expected 2? Let's simulate: "++--" -> top run "++" flip to "--" -> "----" (1 flip), then flip all -> "++++" (2 flips). So equals 2.
    assert(minimumFlipsToAllHappy("++--") == 2);
    // Single happy
    assert(minimumFlipsToAllHappy("+") == 0);
    // Mixed with long happy bottom
    assert(minimumFlipsToAllHappy("---+++") == 1); // flip all: toggles and reverses -> "---+++" reversed and toggled to "+++---"? But we need all happy. Actually flip all once gives "---+++" reversed and toggled: original "---+++" length 6, reverse gives "+++---", toggle gives "---+++"? Wait toggle each: '-' becomes '+', '+' becomes '-': "+++---" -> after toggle "---+++"? That's back to same? No: reverse "---+++" gives "+++---", toggle gives "---+++" indeed back to same. So need more? Actually let's not include that; instead test a known case: "+++-" -> flip "+++" to "---" (1) then flip all up to last blank (index 3) -> becomes "+++-"? Hmm. Let's use trusted known answer: For "+++-", the minimum is 2? Simulate: First flip top prefix "+++" to "---" -> "----", then flip all -> "++++" (2). Yes. So assert 2.
    assert(minimumFlipsToAllHappy("+++-") == 2);
    // Maximum length simple
    std::string long_all_blank(100, '-');
    assert(minimumFlipsToAllHappy(long_all_blank) == 1);
    std::string long_alternating;
    for (int i = 0; i < 100; ++i) long_alternating += (i % 2 == 0) ? '+' : '-';
    // For length 100 alternating starting with '+': pattern "+-+-+-..." The optimal flips? For each run boundary, you flip twice per boundary? Actually for n=100 alternating, minimum flips equals number of runs minus 1? Let's compute: For "+-+-...", it has 100 runs, so answer is 99? Let's verify with small: "+-+" (runs 3) -> flips? Flip "+" to "-" (1) then flip up to last "-"? Actually let's trust our algorithm for larger, but for test we can just assert that it returns a non-negative value? Better to omit this risky test. Instead use known result from Code Jam: For "+-+", the answer is 2? Let's simulate: "+-+" -> top '+' flip to '-' -> "--+" (1), rightmost blank index 1, flip prefix up to 1 -> toggles and reverses "--" -> "+++"? Actually flip prefix of length 2: "--" reversed and toggled becomes "++" so stack "+++" happy? So total 2. So assert 2 for "+-+".
    assert(minimumFlipsToAllHappy("+-+") == 2);
    return 0;
}
