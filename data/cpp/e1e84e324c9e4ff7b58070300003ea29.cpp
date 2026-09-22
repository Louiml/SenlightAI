Given a binary string `s` consisting only of characters `'0'` and `'1'`, write a C++ function `int maxGroupOperations(const std::string& s)` that returns the maximum number of operations that can be performed according to the following rule: In one operation, you may choose a group of consecutive `'0'` characters and move all the `'1'` characters that appear anywhere before that group (i.e., to its left) one position to the right (shifting them into the zeros), which effectively reduces the number of leading zeros before each moved `'1'`. To maximize the total number of such moves, you must repeatedly perform moves on any zero-group that has at least one `'1'` to its left, but each zero-group can only be used once per operation. The process ends when no zero-group has a `'1'` to its left; return the total number of moves executed. For example, for `s = "1001101"`, the answer is `4`. The function must handle empty strings (returning `0`) and strings with no zeros or no ones.

#include <cassert>

int main() {
    // Basic examples from the problem description.
    assert(maxGroupOperations("1001101") == 4);
    
    // All ones: no zeros, so no moves.
    assert(maxGroupOperations("1111") == 0);
    
    // All zeros: no ones to move, so no moves.
    assert(maxGroupOperations("0000") == 0);
    
    // Single zero after one: one move.
    assert(maxGroupOperations("10") == 1);
    
    // Zeros at the beginning: no ones before them.
    assert(maxGroupOperations("0011") == 0);
    
    // Alternating pattern: each zero group has prior ones.
    assert(maxGroupOperations("101010") == 3); // groups: after 1, after 11, after 111? Actually: "1 0 1 0 1 0": zero groups at pos1 (1 one), pos3 (2 ones), pos5 (3 ones) → 1+2+3=6? Let's recompute: s="101010": process: i=0 '1' ones=1; i=1 '0' run length1, ans+=1 → ans=1; i=2 '1' ones=2; i=3 '0' ans+=2 → ans=3; i=4 '1' ones=3; i=5 '0' ans+=3 → ans=6. So correct assert is 6.
    assert(maxGroupOperations("101010") == 6);
    
    // Multiple zeros in a group: count once.
    assert(maxGroupOperations("10001") == 1); // one group of zeros with one '1' before
    
    // Zeros after a large number of ones.
    assert(maxGroupOperations("11000") == 2); // one zero group, two ones before
    
    // Zeros interspersed with ones but ending with zeros.
    assert(maxGroupOperations("101100") == 5); // zero groups: after first '1' (1 one), after "11" (2 ones) → 1+2=3? Actually: s="101100": indices 0:'1' ones=1; 1:'0' group ans+=1; 2:'1' ones=2; 3:'1' ones=3; 4:'0' group ans+=3 (total 4); 5:'0' part of same group? The loop will skip both zeros in one group: so total ans=1+3=4. So assert 4.
    assert(maxGroupOperations("101100") == 4);
    
    // Empty string.
    assert(maxGroupOperations("") == 0);
    
    // Single character.
    assert(maxGroupOperations("0") == 0);
    assert(maxGroupOperations("1") == 0);
    
    return 0;
}

#include <string>

// Returns the maximum number of operations possible on a binary string.
// Each operation moves all '1's to the left of a consecutive zero-group
// one step to the right into that group.
int maxGroupOperations(const std::string& s) {
    int answer = 0;
    int ones_seen = 0;
    const int n = static_cast<int>(s.size());

    for (int i = 0; i < n; ++i) {
        if (s[i] == '1') {
            ++ones_seen;
        } else { // s[i] == '0'
            // Skip the entire run of consecutive zeros.
            int j = i;
            while (j < n && s[j] == '0') {
                ++j;
            }
            // Every '1' before this zero-run can be moved into it once.
            answer += ones_seen;
            i = j - 1; // outer loop will increment to j
        }
    }

    return answer;
}

// The key observation is that each move corresponds to a zero-group that lies to the right of at least one `'1'`. Since a zero-group can be used in exactly one move (after which those zeros are effectively "filled" by shifting ones), the total number of moves equals the sum over all zero-groups of the number of `'1'`s that are positioned before that group. To compute this efficiently, scan the string left to right while maintaining `count` = number of `'1'`s seen so far. Whenever a run of consecutive `'0'`s is encountered (skip the whole run using an inner loop), add `count` to the answer, because every `'1'` before that run can be moved into this zero-group once. Then continue scanning after the run. Edge cases: if the string ends with zeros, the final zero-group is also counted (since there may be ones before it), but if there are no ones before a zero-group, it contributes zero. If there are no zeros, the answer is 0. Time complexity is O(n) because each character is visited at most twice (once in outer loop, once in inner zero-skip); space complexity is O(1) aside from input.
