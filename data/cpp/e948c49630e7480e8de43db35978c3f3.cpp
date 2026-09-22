You are given a list of transactions, where each transaction is represented as a vector `[cost, cashback]`. You must complete these transactions in any order, but before starting you can choose an initial amount of money. For each transaction, you pay `cost` first, then immediately receive `cashback` back. You may only perform a transaction if your current money is at least `cost` before paying it; if not, you cannot proceed. Write a C++ function `long long minimumMoney(vector<vector<int>>& transactions)` that returns the minimum initial money needed to ensure that all transactions can be completed in at least one valid order (i.e., there exists some permutation of transactions that can be executed successfully starting with that initial money). The input contains `n` transactions (1 ≤ n ≤ 1e5), each with `cost` and `cashback` being integers from 0 to 1e9. You may reorder the transactions arbitrarily. The function must compute the answer efficiently, handling large totals within 64-bit signed integers.

The key insight is to separate transactions into two groups: those with `cost > cashback` (net loss) and those with `cost ≤ cashback` (net non-loss). For a net-loss transaction, the money spent is `cost`, but you recover `cashback`, so the worst-case dip in money during that transaction is `cost - cashback` (the net outflow). To minimize initial money, it is optimal to perform all net-loss transactions first in an order that minimizes the peak requirement, then handle the non-loss ones. For any valid ordering, the total sum of net losses from all net-loss transactions, `sumLoss = Σ max(0, cost - cashback)`, must be covered by the initial money plus whatever cashback is received before the last transaction. More formally, the optimal strategy is: place a net-loss transaction with the highest `cashback` last among all net-loss ones, and place a non-loss transaction with the highest `cost` last among all transactions. The minimum initial money is the maximum over all transactions of `sumLoss + (cashback if net-loss, or cost if non-loss)`. Explanation: For each transaction, consider it as the final transaction to be executed. Before executing it, you must have already paid all other net losses, requiring `sumLoss - (cost - cashback)` if the final one is a net-loss, or `sumLoss` if it is non-loss. Then, before paying the final transaction's `cost`, you need enough to cover that `cost`, so total needed is `sumLoss + cashback` for net-loss (since you receive cashback after paying, but you need to pay `cost` first; the required amount before this transaction is `sumLoss + cost - (cost - cashback) = sumLoss + cashback`, but actually check: you need `sumLoss - (cost - cashback)` from previous net losses, plus `cost` for this transaction, totaling `sumLoss + cashback`). For non-loss final transaction, you need `sumLoss` from previous net losses plus `cost` for this, totaling `sumLoss + cost`. Taking the maximum over all candidates gives the minimal sufficient initial money, and it is also necessary because you can order transactions to achieve this bound. Time complexity is O(n) for two passes, space O(1) besides input storage. Edge cases: all transactions are net-loss, all are non-loss, or mixed; also cost and cashback can be zero, and sums may exceed 32-bit ints so use `long long`.

#include <vector>
#include <algorithm>

// Computes the minimum initial money needed to complete all transactions in some order.
long long minimumMoney(std::vector<std::vector<int>>& transactions) {
    long long totalNetLoss = 0;
    for (const auto& t : transactions) {
        // Sum of positive differences (cost - cashback) for unprofitable transactions.
        totalNetLoss += std::max(0, t[0] - t[1]);
    }

    long long answer = 0;
    for (const auto& t : transactions) {
        if (t[0] > t[1]) {
            // If this unprofitable transaction is last, we need totalNetLoss + cashback.
            answer = std::max(answer, totalNetLoss + static_cast<long long>(t[1]));
        } else {
            // If this profitable/even transaction is last, we need totalNetLoss + cost.
            answer = std::max(answer, totalNetLoss + static_cast<long long>(t[0]));
        }
    }
    return answer;
}

#include <cassert>
#include <vector>

// The solution function is declared above; here is the test harness.
long long minimumMoney(std::vector<std::vector<int>>& transactions);

int main() {
    // Example 1: mixed transactions
    std::vector<std::vector<int>> t1 = {{2, 1}, {5, 0}, {4, 2}};
    assert(minimumMoney(t1) == 10); // Needed: sumLoss=1+5+2=8, max over candidates: 8+1=9,8+0=8,8+2=10 => 10

    // Example 2: all profitable (cost <= cashback)
    std::vector<std::vector<int>> t2 = {{1, 2}, {3, 3}, {0, 5}};
    assert(minimumMoney(t2) == 3); // sumLoss=0, max(max(cost, cashback?) ) Actually max(cost)=3, max(cashback)=5 but only cost used for non-loss => max(0+1,0+3,0+0)=3

    // Example 3: single unprofitable transaction
    std::vector<std::vector<int>> t3 = {{10, 3}};
    assert(minimumMoney(t3) == 10); // sumLoss=7, candidate: 7+3=10

    // Example 4: single profitable transaction
    std::vector<std::vector<int>> t4 = {{5, 10}};
    assert(minimumMoney(t4) == 5); // sumLoss=0, candidate: 0+5=5

    // Example 5: all unprofitable with varying cashback
    std::vector<std::vector<int>> t5 = {{4, 0}, {6, 2}, {3, 1}};
    // sumLoss = 4+4+2 = 10; candidates: 10+0=10, 10+2=12, 10+1=11 -> 12
    assert(minimumMoney(t5) == 12);

    // Example 6: edge case with zeros
    std::vector<std::vector<int>> t6 = {{0, 0}, {0, 0}};
    assert(minimumMoney(t6) == 0);

    // Example 7: large values to check long long
    std::vector<std::vector<int>> t7 = {{1000000000, 0}, {1000000000, 1000000000}};
    // sumLoss = 1000000000, candidates: 1e9+0=1e9, 0? For non-loss: cost=1e9, so 1e9+1e9=2e9
    assert(minimumMoney(t7) == 2000000000LL);

    return 0;
}
