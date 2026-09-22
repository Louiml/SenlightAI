// Given an integer `n` (1 ≤ n ≤ 5000), an integer `x` (1 ≤ x ≤ 5000), and a sequence of `n` probabilities `a[1..n]`, where each `a[i]` is an integer percentage between 0 and 100 inclusive, consider the following process: we have `x` independent trials, and in each trial, we perform a sequence of `n` independent experiments. In experiment `i`, a success occurs with probability `a[i]/100`, and a failure occurs otherwise. A trial is considered successful if at least one of the `n` experiments succeeds. Define `dp[i][j]` as the probability that after processing the first `i` experiments, exactly `j` successes have occurred in a single trial. Write a C++ function that returns the expected number of trials needed until we observe the first successful trial, where a trial is successful if at least one experiment in that trial succeeds. The function should take as input a vector of probabilities (as integers 0..100) and the integer `x` (which represents the number of trials we are willing to consider for expectation calculation? Actually, read carefully: In the given code, `x` is used to compute expected value up to that many trials, but the final answer is `ev[x]`, which is the expected number of trials given that we cap at `x`? Let's re-interpret: The code computes `ev[i]` for i from 1 to x, where `ev[i]` is the expected number of trials required to get the first success, given that we allow at most `i` trials? Actually, the recurrence is standard for expected value of a geometric-like distribution: if `p = P(at least one success in a trial) = 1 - dp[n][0]`, then `E[first success] = 1/p`. But the code computes something different: `ev[1] = 1/(1-dp[n][0])`, and for i≥2, `ev[i] = (1 + sum_{j=0}^{i-1} ev[j] * dp[n][i-j]) / (1 - dp[n][0])`. This is actually the expected number of trials until we get a total of `i` successes across all trials? No, that doesn't match. Actually, looking at the recurrence: `addOn = 1 + sum_{j=0}^{i-1} ev[j] * dp[n][i-j]`. This resembles the expected time to accumulate `i` successes in a sequence of trials, where each trial yields a certain number of successes (possibly zero) with distribution `dp[n][k]`. So `ev[i]` is the expected number of trials needed to accumulate at least `i` total successes across all experiments? But then `ev[x]` is the answer. Let's define clearly: We have an infinite sequence of independent trials; each trial consists of `n` independent Bernoulli experiments with success probabilities `a[i]/100`. Each trial produces a nonnegative integer `k` = number of successes in that trial, with probability `dp[n][k]`. We want the expected number of trials needed until the cumulative sum of successes reaches at least `x`. That is, we stop when total successes across all trials ≥ `x`. The given code computes that expected number via dynamic programming: `ev[i]` = expected number of trials needed to reach at least `i` total successes. Base case: `ev[0] = 0` (implicitly). For i≥1, we have: `ev[i] = 1 + sum_{j=0}^{i-1} ev[j] * P(exactly i-j successes in a trial)`? Actually, the recurrence is: `ev[i] = (1 + sum_{j=1}^{i} ev[i-j] * dp[n][j]?)`? Let's derive: To reach at least `i` successes, we take one trial. If that trial gives `k` successes, we need to reach at least `i-k` more successes. So `ev[i] = 1 + sum_{k=0}^{i} ev[max(0, i-k)] * dp[n][k]?` But that gives a self-referential term when k=0 (since i-k = i). So we isolate the k=0 term: `ev[i] = 1 + sum_{k=0}^{i} dp[n][k] * ev[i-k]` with `ev[0]=0`. For i≥1, the term k=0 gives `dp[n][0]*ev[i]`, so we bring it to left: `ev[i] - dp[n][0]*ev[i] = 1 + sum_{k=1}^{i} dp[n][k]*ev[i-k]`, so `ev[i] = (1 + sum_{k=1}^{i} dp[n][k]*ev[i-k]) / (1 - dp[n][0])`. That matches the code's recurrence with `ev[j]` for j from 0 to i-1 and `dp[n][i-j]`. So indeed, `ev[i]` is expected trials to accumulate at least `i` successes. Your task: Write a function `double expectedTrials(const vector<int>& a, int x)` that computes `ev[x]` as described. The function must handle n up to 5000 and x up to 5000, and output a double with high precision. The probabilities are given as integers 0-100. Note that `dp` matrix size is (n+1)*(n+1) but we only need `dp[n][*]` after computing, so you can compute a 1D DP for each trial. The time complexity is O(n^2 + x^2) but given constraints it's fine (25 million operations). Provide a solution.

#include <cassert>
#include <cmath>
#include <vector>
#include <iostream>

// Include the function definition here (or link against it)
// For testing, we place the solution function above.

int main() {
    // Test 1: Single experiment with probability 100% (always success). Each trial gives exactly 1 success.
    // Expected trials to get 5 successes = 5.
    std::vector<int> a1 = {100};
    double res1 = expectedTrials(a1, 5);
    assert(std::abs(res1 - 5.0) < 1e-9);

    // Test 2: Single experiment with probability 50%. Each trial gives 0 or 1 success (binomial(1,0.5)).
    // Expected trials to get 1 success = 1/(0.5) = 2. For 2 successes, it's sum of two geometric with mean 2 each => 4? Actually not independent but due to memoryless, it's 4.
    std::vector<int> a2 = {50};
    double res2 = expectedTrials(a2, 1);
    assert(std::abs(res2 - 2.0) < 1e-9);
    double res2b = expectedTrials(a2, 2);
    assert(std::abs(res2b - 4.0) < 1e-9);

    // Test 3: Two experiments each with 100% success => every trial gives exactly 2 successes.
    // To get 3 successes, we need 2 trials (first gives 2, second gives at least 1) but actually since each trial gives exactly 2, we need ceil(3/2)=2 trials.
    std::vector<int> a3 = {100, 100};
    double res3 = expectedTrials(a3, 3);
    assert(std::abs(res3 - 2.0) < 1e-9);

    // Test 4: Two experiments each 0% success => no success ever, should return large sentinel.
    std::vector<int> a4 = {0, 0};
    double res4 = expectedTrials(a4, 1);
    assert(res4 > 1e17);

    // Test 5: One experiment 0%, another 100% => each trial gives exactly 1 success (probability 1).
    assert(std::abs(expectedTrials({0, 100}, 7) - 7.0) < 1e-9);

    // Test 6: One experiment 100%, another 50%? That's not a clean scenario. Let's test with a small n and x where we can brute force simulate or use known result.
    // For n=1, p=0.25, expected trials to get 1 success = 1/0.25 = 4. To get 2 successes = 8 (since geometric sum).
    assert(std::abs(expectedTrials({25}, 1) - 4.0) < 1e-9);
    assert(std::abs(expectedTrials({25}, 2) - 8.0) < 1e-9);

    std::cout << "All tests passed.\n";
    return 0;
}

#include <vector>
#include <algorithm>

// Computes the expected number of independent trials needed to accumulate at least x successes,
// where each trial consists of n independent Bernoulli experiments with success probabilities a[i]/100.
double expectedTrials(const std::vector<int>& a, int x) {
    int n = (int)a.size();
    
    // Compute probability distribution of number of successes in one trial.
    std::vector<double> prob(n + 1, 0.0);
    prob[0] = 1.0;
    for (int i = 0; i < n; ++i) {
        double p = a[i] / 100.0;
        std::vector<double> next(n + 1, 0.0);
        for (int j = 0; j <= i; ++j) {
            if (prob[j] == 0.0) continue;
            // failure
            next[j] += prob[j] * (1.0 - p);
            // success
            next[j + 1] += prob[j] * p;
        }
        prob.swap(next);
    }
    
    double noSuccessProb = prob[0];
    if (noSuccessProb >= 1.0 - 1e-15) {
        // Probability of any success is exactly zero.
        return 1e18; // effectively infinity
    }
    
    // E[i] = expected trials to accumulate at least i successes.
    std::vector<double> E(x + 1, 0.0);
    E[0] = 0.0;
    for (int i = 1; i <= x; ++i) {
        double sum = 0.0;
        int upper = std::min(i, n);
        for (int j = 1; j <= upper; ++j) {
            sum += prob[j] * E[i - j];
        }
        E[i] = (1.0 + sum) / (1.0 - noSuccessProb);
    }
    
    return E[x];
}

// The problem reduces to computing the distribution of the number of successes in a single trial, where a trial consists of `n` independent Bernoulli experiments with probabilities `a[i]/100`. We first compute a probability mass function `prob[k]` for k=0..n, where `prob[k]` = probability that exactly k of the n experiments succeed. This is done via dynamic programming: let `cur[j]` = probability of exactly j successes after processing first i experiments. Initialize `cur[0]=1`. For each experiment with success probability p, update: `new[j] = cur[j]*(1-p) + (j>0? cur[j-1]*p : 0)`. After all n experiments, `prob[k] = cur[k]` (for k=0..n). Then we compute the expected number of trials to accumulate at least `x` total successes. Let `E[i]` be the expected number of trials needed to accumulate at least `i` successes. We have `E[0]=0`. For i≥1, the recurrence is `E[i] = (1 + sum_{j=1}^{i} prob[j] * E[i-j]) / (1 - prob[0])`. Here `j` is the number of successes in the current trial, and `E[i-j]` is the expected remaining trials (note if i-j ≤0 then E is 0 because we already reached the target). But careful: `i` can be up to x, but `j` can be up to n. For j > i, `E[i-j]` is zero because we already have at least i successes. So the sum is up to min(i,n). The recurrence is straightforward O(x * n) but n and x can be 5000, so O(25e6) is acceptable. Edge cases: if `prob[0] == 1` (i.e., all probabilities are 0), then no trial ever produces any success, so the expectation is infinite; but the problem likely guarantees at least one success possible. However, to be safe, if `prob[0] == 1`, return infinity (or maybe large number). Also if `prob[0] == 0`, then every trial produces at least one success, so the expected number of trials to get x successes is exactly x, because each trial gives at least 1 success, but could give more, so expectation is less than or equal to x? Actually if every trial gives at least 1 success, then to accumulate x successes, you need at most x trials, and the expectation is well-defined. The recurrence still works. Complexity: O(n^2 + x * min(n,x)) time and O(n + x) space if we store only the current DP for prob and the E array. The problem statement asks for a function returning a double, and we should use `long double` for precision, but output as double is fine. We'll implement with `vector<double>`.
