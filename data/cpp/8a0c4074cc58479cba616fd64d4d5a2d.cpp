Write a C++ function that solves the "Password Problem" variant from Google Code Jam: given the number of typed characters `n` and the password length `m` (where `n ≤ m`), and an array of probabilities `p[1..n]` that each typed character is correct (independent), compute the minimum expected number of keystrokes needed to guarantee access to the account. You may either: keep typing (press Enter immediately), press Backspace `k` times (for any `k` from 1 to `n`) then retype the remaining characters and press Enter, or press Enter right away to give up (which costs a fixed penalty). The expected cost formulas are:  
- **Keep typing**: `p_all * (m - n + 1) + (1 - p_all) * (2*m - n + 2)` where `p_all` is the product of all `p[i]`.  
- **Backspace `j` times**: cost = `cur * (2*(1+n-j) + m - n + 1) + (1 - cur) * (2*(1+n-j) + m - n + 1 + m + 1)` where `cur` is product of probabilities for first `j` characters (i.e., the probability that the first `j` typed characters are correct). For `j = 0`, the cost is the keep‑typing formula (but you must also consider `j = n` meaning press Enter immediately after deleting all, which is the give‑up case).  
- **Give up immediately**: cost = `m + 2`.  
Return the minimum of these three strategies as a `double`. The input probabilities are given as `std::vector<double>` of size `n` in the order typed. The function signature: `double expectedKeystrokes(int n, int m, const std::vector<double>& p)`. Ensure the solution is self‑contained, robust for `n = 0`? (though the problem states `n ≥ 1`, handle `n = 0` gracefully by returning `m + 1`? Actually standard problem has `n ≥ 1`; but for safety, if `n == 0`, return `m + 1` because you type all characters from scratch plus enter).
#include <cassert>
#include <vector>
#include <cmath>

// Solution function declared above (assumed in same translation unit)
double expectedKeystrokes(int n, int m, const std::vector<double>& p);

int main() {
    // Test 1: n=1, m=2, p={0.5}
    // keepTyping = 0.5*(2-1+1) + 0.5*(4-1+2)=0.5*2+0.5*5=1+2.5=3.5
    // giveUp = 4
    // backspace j=1: cur=0.5, cost=0.5*(2*(1+0)+2-1+1)+0.5*(2*(1+0)+2-1+1+3)=0.5*4+0.5*7=2+3.5=5.5
    // min = 3.5
    assert(std::abs(expectedKeystrokes(1, 2, {0.5}) - 3.5) < 1e-9);

    // Test 2: n=2, m=5, p={0.9, 0.9}
    // allCorrect=0.81, keepTyping=0.81*(5-2+1)+0.19*(10-2+2)=0.81*4+0.19*10=3.24+1.9=5.14
    // giveUp=7
    // j=1: cur=0.9, cost=0.9*(2*(1+1)+5-2+1)+0.1*(2*(1+1)+5-2+1+6)=0.9*8+0.1*14=7.2+1.4=8.6
    // j=2: cur=0.81, cost=0.81*(2*(1+0)+5-2+1)+0.19*(2*(1+0)+5-2+1+6)=0.81*6+0.19*12=4.86+2.28=7.14
    // min = 5.14
    assert(std::abs(expectedKeystrokes(2, 5, {0.9, 0.9}) - 5.14) < 1e-9);

    // Test 3: n=1, m=1, p={1.0}
    // allCorrect=1, keepTyping=1*(1-1+1)+0=1
    // giveUp=3
    // j=1: cur=1, cost=1*(2*(1)+1-1+1)+0=1*4=4? Actually 2*(1+0)+1-1+1=2+1=3? Wait compute: 2*(1+n-j)+m-n+1=2*(1+0)+1-1+1=2+0+1=3. So cost=1*3=3. min=1
    assert(std::abs(expectedKeystrokes(1, 1, {1.0}) - 1.0) < 1e-9);

    // Test 4: n=3, m=4, p={0.5, 0.5, 0.5}
    // allCorrect=0.125, keepTyping=0.125*(4-3+1)+0.875*(8-3+2)=0.125*2+0.875*7=0.25+6.125=6.375
    // giveUp=6
    // j=1: cur=0.5, cost=0.5*(2*(1+2)+4-3+1)+0.5*(2*(1+2)+4-3+1+5)=0.5*9+0.5*14=4.5+7=11.5
    // j=2: cur=0.25, cost=0.25*(2*(1+1)+4-3+1)+0.75*(2*(1+1)+4-3+1+5)=0.25*7+0.75*12=1.75+9=10.75
    // j=3: cur=0.125, cost=0.125*(2*(1+0)+4-3+1)+0.875*(2*(1+0)+4-3+1+5)=0.125*5+0.875*10=0.625+8.75=9.375
    // min = 6 (giveUp)
    assert(std::abs(expectedKeystrokes(3, 4, {0.5, 0.5, 0.5}) - 6.0) < 1e-9);

    // Test 5: n=0 edge case (not typical but safe)
    assert(std::abs(expectedKeystrokes(0, 5, {}) - 6.0) < 1e-9);

    // Test 6: all probabilities 1.0, n=2, m=3
    // allCorrect=1, keepTyping=1*(3-2+1)+0=2
    // giveUp=5
    // j=1: cur=1, cost=1*(2*(1+1)+3-2+1)+0=2*3? Actually 2*(1+1)=4, +3-2+1=2 => total 6? Wait 2*(1+n-j)=2*(1+2-1)=2*2=4, +m-n+1=3-2+1=2 => 6
    // j=2: cur=1, cost=1*(2*(1+0)+3-2+1)=2*1? 2*(1)=2, +2=4
    // min = 2
    assert(std::abs(expectedKeystrokes(2, 3, {1.0, 1.0}) - 2.0) < 1e-9);

    return 0;
}
#include <vector>
#include <algorithm>

// Compute the minimum expected number of keystrokes for the Google Code Jam "Password Problem".
// n: number of characters already typed, m: password length (n <= m)
// p: probabilities that each typed character is correct (size n)
double expectedKeystrokes(int n, int m, const std::vector<double>& p) {
    if (n == 0) {
        // Nothing typed, just type the full password and press Enter.
        return static_cast<double>(m + 1);
    }

    double allCorrect = 1.0;
    for (int i = 0; i < n; ++i) {
        allCorrect *= p[i];
    }

    // Strategy 1: keep typing (press Enter now)
    double keepTyping = allCorrect * (m - n + 1) + (1.0 - allCorrect) * (2 * m - n + 2);

    // Strategy 2: give up immediately (press Enter, then retype whole password and Enter again)
    double giveUp = static_cast<double>(m + 2);

    // Strategy 3: backspace j times for j = 1..n (j = 0 is keepTyping)
    double bestBackspace = keepTyping; // start with j=0 (same as keepTyping)
    double curProb = 1.0; // prob first j characters are correct
    for (int j = 1; j <= n; ++j) {
        // 'j' backspaces means we keep first j characters (index 0..j-1)
        // Update probability of first j being correct
        curProb *= p[j - 1];
        double cost = curProb * (2 * (1 + n - j) + m - n + 1) +
                      (1.0 - curProb) * (2 * (1 + n - j) + m - n + 1 + m + 1);
        bestBackspace = std::min(bestBackspace, cost);
    }

    return std::min(keepTyping, std::min(giveUp, bestBackspace));
}
// The solution iterates over all possible backspace counts `j` from 0 to `n`. For each `j`, we compute the probability that the first `j` typed characters are correct (the product of `p[0]..p[j-1]`). The expected cost if we backspace `j` times is:  
// - If we are correct so far (probability `cur`), we then retype the remaining `n - j` characters (cost `2*(1+n-j)` includes backspaces and retyping) and press Enter once (cost `m - n + 1` to fill the rest of the password and enter).  
// - If we are wrong (probability `1 - cur`), we must retype the entire password from scratch, costing `2*(1+n-j)` plus `m` more keystrokes for the full length, plus the Enter press (the `+1` in the formula).  
// The keep‑typing option is exactly `j = 0`. The give‑up option is `m + 2` (press Enter immediately after typing nothing more, but you already typed `n` characters? Actually in the problem, pressing Enter right away costs `m + 2` because you have to press Backspace `n` times to clear all, then type the full password `m` times and press Enter once, total `n + m + 2`? Wait the standard formula for give up is `m + 2` because you assume you press Backspace `n` times (cost `n`) then type `m` characters and press Enter (`m+1`), total `n + m + 1`? That doesn't match. The given code uses `res3 = m + 2`, which is the cost if you press Enter right away without backspacing – you lose the opportunity to correct, so you have to start over completely: type `m` characters (cost `m`) and press Enter (cost 1), plus you already typed `n` characters but they are wrong? Actually the standard solution for the problem defines "give up" as pressing Enter immediately, which costs `m + 2` because you have to press Enter, then type the correct password of length `m` and press Enter again? No, the original Code Jam problem defines "press Enter right away" as an option that costs `m + 2` (that is, you press Enter, then you must retype the whole password of length `m` and press Enter again, total `m + 2`? Let's trust the given code: `res3 = m + 2`. So we use that directly.  
// The algorithm computes all three candidates and takes the minimum. Edge cases: `n = m` (all characters typed) – backspace options still valid. `n = 0`? Not in problem, but we handle by returning `m + 1` (just type full password and enter). Time complexity: O(n) for the loop, O(1) space. We must be careful with double precision – output to 6 decimal places in original but function returns double.
