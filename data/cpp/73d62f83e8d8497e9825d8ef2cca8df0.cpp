// Given two integers `T` (number of true statements) and `F` (number of false statements), write a C++ function `maxStatements` that returns the maximum number of statements that can appear in a valid sequence, or -1 if no valid sequence exists. A sequence is valid if it consists of all `T` true statements and all `F` false statements arranged in some order, and every true statement in the sequence must be immediately followed by at least one false statement (except possibly the last true statement if it is the final element). In other words, no true statement can appear consecutively with another true statement unless it is the last element; each true statement (except the last one overall) must have a false statement directly after it. If `F >= T`, no such arrangement exists because there aren't enough false statements to separate the true statements properly. Otherwise, return the maximum possible total length of the sequence (which is always `T + F` for any valid arrangement, but if no arrangement exists return -1). However, the original problem has a specific formula: if `F >= T`, return -1; otherwise return `min(2*F + 1, T + F)`. This formula gives the maximum length of a valid subsequence that can be formed using at most all available statements, not necessarily using all of them. So your function should compute exactly this: if F >= T, return -1; else return min(2*F + 1, T + F). Input values are non-negative integers up to 10^18, so use 64-bit integers.

The core observation is that true statements cannot be adjacent unless the second true is the very last element of the sequence. Since each true (except possibly the last) needs a false immediately after it, each false can separate at most two true statements (one before and one after), and a true can be placed at the end without a following false. Thus, with `F` false statements, you can have at most `2*F + 1` true statements in a valid sequence (arranged as T F T F ... T), but you are limited by the available `T` true statements, so the maximum number of total statements possible in a valid sequence is `min(2*F + 1, T + F)`. This formula is only valid when `F < T`; if `F >= T`, you cannot even arrange all `T` true statements because each true needs a false after it (except possibly the last), and with `F >= T` you would run out of false statements before placing all trues. For example, T=3, F=1: 2*1+1=3, T+F=4, min=3, but can we place 3 trues and 0 falses? Yes, sequence T T T is invalid because the first two trues are consecutive without a false between. Actually with one false, you can place at most 2 trues (T F T), so the formula gives min(3,4)=3, but the actual max is 3? Wait, let's test T=3,F=1: formula gives 3, but can we make 3 statements? For example T F T uses 2 trues and 1 false, length 3, leaving one true unused. So indeed max length is 3. Another example T=4,F=2: formula min(5,6)=5, but can we make length 5? Yes, T F T F T uses 3 trues and 2 falses. Can we use 4 trues? Need at least 3 falses to separate them (T F T F T F T). So 2 falses can support at most 3 trues, giving length 5. That matches 2*F+1=5. Also note that using all T + F is only possible if T <= 2F+1, i.e., T <= 2F+1. So the condition F >= T ensures failure. Edge cases: T=0, F=0? The problem likely expects if F>=T then -1, so for T=0,F=0, F>=T true, return -1. For T=1,F=0, F<T, min(0+1? 2*0+1=1, T+F=1, min=1, valid sequence "T"). For large values, use long long. Time complexity O(1), space O(1).

#include <algorithm>

// Return the maximum number of statements in a valid sequence, or -1 if none.
long long maxStatements(long long T, long long F) {
    if (F >= T) {
        return -1;
    }
    return std::min(2LL * F + 1LL, T + F);
}

#include <cassert>

// Solution function declared here (for completeness, not needed in test)
long long maxStatements(long long, long long);

int main() {
    // Basic cases
    assert(maxStatements(1, 0) == 1);
    assert(maxStatements(2, 1) == 3); // 2*1+1=3, T+F=3 -> 3
    assert(maxStatements(3, 1) == 3); // 2*1+1=3, T+F=4 -> 3
    assert(maxStatements(4, 2) == 5); // 2*2+1=5, T+F=6 -> 5
    assert(maxStatements(3, 3) == -1); // F >= T
    assert(maxStatements(0, 0) == -1); // F >= T
    assert(maxStatements(5, 2) == 5); // 2*2+1=5, T+F=7 -> 5
    assert(maxStatements(100, 49) == 99); // 2*49+1=99, T+F=149 -> 99
    assert(maxStatements(100, 50) == 100); // 2*50+1=101, T+F=150 -> 100
    assert(maxStatements(1000000000000000000LL, 1LL) == 3); // large values, 2*1+1=3, T+F huge -> 3
    return 0;
}
