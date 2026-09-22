/*
Write a C++ function `double expectedValue(const std::vector<int>& squares)` that, given a vector of `n` non-negative integers representing values on squares numbered 1 to `n`, computes the expected sum of values collected when starting at square 1 and repeatedly rolling a fair 6-sided die. From square `i`, you move to square `i + k` where `k` is the die roll (1 to 6), but if the roll would take you beyond square `n`, you move to square `n` (the game ends when you reach square `n`). The value of each square is collected when you land on it, including the starting square (square 1) and the final square (square n). The function should return the expected total collected value. For example, if `squares = {1, 2, 3}`, the expected value starting from square 1 with values 1,2,3 and die rolls only possible to move to 2 or 3 (since from 1, roll 1 goes to 2, roll 2 goes to 3, rolls 3-6 would overshoot to 3) is: from 1, you get 1, then with probability 1/2 go to 2, get 2, then from 2 only roll 1 goes to 3 (rolls 2-6 overshoot to 3), so you get 3 with probability 1; with probability 1/2 go directly to 3, get 3. So total = 1 + (1/2*(2+3) + 1/2*3) = 1 + 2.5 + 1.5 = 5.0. The function should handle any `n >= 1`.
*/

#include <vector>
#include <algorithm>

// Compute expected total value starting from square 1 with a fair 6-sided die.
// squares[i] is the value on square i+1 for i=0..n-1.
double expectedValue(const std::vector<int>& squares) {
    int n = static_cast<int>(squares.size());
    std::vector<double> dp(n, 0.0);
    dp[n-1] = squares[n-1];
    for (int i = n - 2; i >= 0; --i) {
        double sum = 0.0;
        // Roll outcomes 1..6
        for (int roll = 1; roll <= 6; ++roll) {
            int dest = std::min(i + roll, n - 1);
            sum += dp[dest];
        }
        dp[i] = squares[i] + sum / 6.0;
    }
    return dp[0];
}

#include <cassert>
#include <vector>

double expectedValue(const std::vector<int>& squares);

int main() {
    // n=1: only start and end at same square
    assert(expectedValue({5}) == 5.0);
    // n=2: from 1, roll1->2, roll2-6 overshoot to 2 => always get square2
    assert(expectedValue({1, 10}) == 11.0);
    // n=3: example from description
    assert(expectedValue({1, 2, 3}) == 5.0);
    // n=6: all rolls valid from each position? from 1, dest 2..6, but from 6 end. 
    // Let's compute manually: dp[6]=1, dp[5]=1+(1/6)*(dp[6]) = 1+1/6=1.166666..., etc. 
    // We'll just check a simple case: all zeros -> 0.
    assert(expectedValue({0, 0, 0, 0, 0}) == 0.0);
    // n=7: from 6, rolls 1 ->7, 2..6 overshoot to 7 => dp[6]=value6 + (1/6)*dp[7]*6 = value6+dp[7].
    // from 5: dest 6,7,7,7,7,7 => dp[5]=value5 + (dp[6]+5*dp[7])/6.
    // Let's test with simple values 1,1,1,1,1,1,1.
    // compute: dp[7]=1; dp[6]=1+1=2; dp[5]=1+(2+5)/6=1+7/6=2.1666667; 
    // dp[4]=1+(dp[5]+dp[6]+4)/6 = 1+(2.1667+2+4)/6 =1+8.1667/6=1+1.3611=2.3611; 
    // dp[3]=1+(dp[4]+dp[5]+dp[6]+3)/6 =1+(2.3611+2.1667+2+3)/6 =1+9.5278/6=1+1.58796=2.58796;
    // dp[2]=1+(dp[3]+dp[4]+dp[5]+dp[6]+2)/6 =1+(2.58796+2.3611+2.1667+2+2)/6 =1+11.1158/6=1+1.8526=2.8526;
    // dp[1]=1+(dp[2]+dp[3]+dp[4]+dp[5]+dp[6]+1)/6 =1+(2.8526+2.58796+2.3611+2.1667+2+1)/6 =1+12.9684/6=1+2.1614=3.1614.
    double res = expectedValue({1,1,1,1,1,1,1});
    assert(res > 3.16 && res < 3.17);
    // Test overshoot to n for n=2: from 1, rolls 1->2, 2-6->2, so always 2.
    assert(expectedValue({0, 100}) == 100.0);
    // Mixed test with exact rational: n=3, squares {1,0,0}: 
    // dp[3]=0; dp[2]=0+(1/6)*dp[3]*6 =0; dp[1]=1+(1/6)*(dp[2]*5 + dp[3])? 
    // from 1: dest 2 (roll1) and 3 (rolls2-6) => dp[1]=1+(dp[2]+5*dp[3])/6 =1+0=1.
    assert(expectedValue({1,0,0}) == 1.0);
    // Test n=4: {1,2,3,4} compute quickly: dp4=4; dp3=3+(1/6)*(dp4*6)=3+4=7; dp2=2+(1/6)*(dp3+dp4*5)=2+(7+20)/6=2+27/6=2+4.5=6.5; dp1=1+(1/6)*(dp2+dp3+dp4*4)=1+(6.5+7+16)/6=1+29.5/6=1+4.9167=5.9167.
    double r2 = expectedValue({1,2,3,4});
    assert(r2 > 5.9 && r2 < 5.93);
    return 0;
}

// We use dynamic programming from the end to the beginning. Define `dp[i]` as the expected total value collected from square `i` up to and including square `n`, assuming we are currently at square `i` and will roll the die. Then `dp[n] = squares[n-1]` (since square n is the final, no further rolls). For `i < n`, let `m = min(6, n - i)` be the number of possible distinct destinations from `i` (since rolls beyond `n-i` overshoot to n). For each roll `k` from 1 to `m`, destination is `i + k`. But if `k > m`, destination is `n`. However, note that the die has 6 equally likely outcomes, but if `n-i < 6`, some rolls map to `n`. So the expected value is: `dp[i] = squares[i-1] + (1/6) * sum_{roll=1}^{6} dp[ dest(i, roll) ]`, where `dest(i, roll) = min(i+roll, n)`. This is equivalent to: `dp[i] = squares[i-1] + ( sum_{k=1}^{min(6, n-i)} dp[i+k] + (6 - min(6, n-i)) * dp[n] ) / 6`. But note that in the snippet they used `dp[i]/=min(n-i,6);` which is an approximation that assumes when overshooting you stay at current? Actually the snippet's code is slightly different: it sums over `k=1..min(n-i,6)` and divides by `min(n-i,6)`, effectively assuming a die with only `min(n-i,6)` faces (i.e., equal probability among valid destinations, ignoring overshoots). That is a common simplification but not exact for a 6-sided die. For the task, we must follow the exact interpretation: use a fair 6-sided die. However, the problem statement says "rolls beyond square n, you move to square n". So we implement exact. Edge cases: n=1 -> dp[1]=squares[0]. n small (1-6) -> many rolls map to n. Time O(n * 6) = O(n), space O(n) for dp (or O(1) if we only keep up to 6 next values). We'll use an array of size n.
