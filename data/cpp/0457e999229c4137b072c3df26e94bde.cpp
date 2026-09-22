/*
You are given a string `S` containing only uppercase and lowercase English letters. A common problem in games is matching a lowercase key to an uppercase lock: each lowercase letter represents a key, and each uppercase letter represents a lock. Your task is to write a C++ function `int unmatchedLocks(const std::string& S)` that processes the string from left to right. When you see a lowercase letter, you acquire a key of that letter (you can have multiple keys of the same letter). When you see an uppercase letter, it is a lock of that letter; if you currently have at least one key of the corresponding lowercase letter, you use one such key to open the lock (remove that key from your inventory). If you do not have a key for that lock, the lock remains unmatched, and you count it as a failure. At the end, return the total number of unmatched locks (i.e., uppercase letters that appeared when no matching lowercase key was available). The input may be empty. For example, `"aA"` returns 0 because the key 'a' opens lock 'A'; `"Aa"` returns 1 because when 'A' appears there is no 'a' key yet; `"aAbB"` returns 0. Note that lowercase keys are never consumed except when used for a matching uppercase lock, and uppercase letters never become keys.
*/

#include <string>
#include <vector>

// Count how many uppercase letters appear without a matching available lowercase key.
int unmatchedLocks(const std::string& S) {
    // counts of available keys for each lowercase letter (a=0, b=1, ..., z=25)
    std::vector<int> keyCount(26, 0);
    int result = 0;

    for (char ch : S) {
        if (ch >= 'a' && ch <= 'z') {
            // acquire a key
            ++keyCount[ch - 'a'];
        } else {
            // ch is uppercase, corresponding lowercase index is ch - 'A'
            int idx = ch - 'A';
            if (keyCount[idx] > 0) {
                // use one key to open the lock
                --keyCount[idx];
            } else {
                // no key available -> unmatched lock
                ++result;
            }
        }
    }
    return result;
}

#include <cassert>

int main() {
    // Basic cases
    assert(unmatchedLocks("") == 0);
    assert(unmatchedLocks("aA") == 0);
    assert(unmatchedLocks("Aa") == 1);
    assert(unmatchedLocks("aAbB") == 0);
    assert(unmatchedLocks("aAbBcC") == 0);
    
    // Multiple keys and locks of same letter
    assert(unmatchedLocks("aaAA") == 0); // two keys open two locks
    assert(unmatchedLocks("aAA") == 1);  // one key, second A has no key
    assert(unmatchedLocks("AAa") == 2);  // both A locks appear before key, then key unused

    // Mixed order and different letters
    assert(unmatchedLocks("bBaA") == 0); // b key, B lock, a key, A lock
    assert(unmatchedLocks("cCaA") == 0); // c key, C lock, a key, A lock
    assert(unmatchedLocks("aAabB") == 0); // a key, A lock, a key, b key, B lock
    assert(unmatchedLocks("aBbA") == 1); // a key, B no key, b key, A no key (since b key used for B? Actually B appears before b, so B unmatched, then b key, then A unmatched)
    // More explicit: "aBbA" → a key, B lock (no b key) → unmatched, b key, A lock (no a key? a key is still there) Actually a key still available, so A is opened. Wait process: 'a' → keyCount[a]=1; 'B' → keyCount[b]==0, result=1; 'b' → keyCount[b]=1; 'A' → keyCount[a]>0, decrement to 0. So result=1. correct.
    assert(unmatchedLocks("aBbA") == 1);
    
    // Only uppercase or only lowercase
    assert(unmatchedLocks("ABC") == 3);
    assert(unmatchedLocks("abc") == 0);
    
    // Sentinel: large input pattern
    std::string pattern = "aA";
    for (int i = 0; i < 100; ++i) pattern += "aA";
    assert(unmatchedLocks(pattern) == 0);
}

// The solution requires maintaining a count of available keys for each lowercase letter. Since there are only 26 letters, use an array of size 26 to store counts. Iterate through the string character by character: if the character is lowercase (`'a'`–`'z'`), increment its count. If it is uppercase (`'A'`–`'Z'`), compute the index of its corresponding lowercase letter (`c - 'A' + 'a'` or equivalently `c - 'A'` for the count array), then: if the count is positive, decrement it (the lock is opened); otherwise, increment the answer (an unmatched lock). Edge cases: the string can be empty, returning 0; a lock may appear after matching keys are used by earlier locks, so counts must be decremented correctly; multiple keys and locks of the same letter are handled correctly by counts. Time complexity is O(n) where n is the length of the string, and space complexity is O(1) because the count array has fixed size 26.
