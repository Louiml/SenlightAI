// Write a C++ function that, given a vector of transactions where each transaction has an ID (uint64_t), a size in bytes (size_t), a priority score (double), and a fee in satoshis (int64_t), selects a subset of transactions to include in a block subject to the following constraints: the total size of selected transactions must not exceed a given maximum block size, and the total number of selected transactions must not exceed a given maximum transaction count. The selection must maximize the total fee, but transactions with priority greater than or equal to a given priority threshold are always included regardless of fee (they are "priority" transactions and are forced in as long as they fit). After including all forced priority transactions that fit (in any order, but skip those that exceed remaining size or count), fill the remaining space with the highest fee-per-byte transactions among the non-forced ones. If two non-forced transactions have the same fee-per-byte, prefer the one with higher priority; if still tied, lower ID first. Return the list of selected transaction IDs in the order they were selected (priority transactions first in their original order, then the fee-selected ones sorted by their selection criteria). The function signature is `std::vector<uint64_t> selectTransactions(const std::vector<Transaction>& txs, size_t maxBlockSize, size_t maxTxCount, double priorityThreshold)`, where `Transaction` is a struct with fields `uint64_t id; size_t size; double priority; int64_t fee;`.

The problem breaks into two parts: forced inclusion of high-priority transactions, and then a knapsack-like greedy selection based on fee-per-byte. First, iterate through the input vector in its original order, and for each transaction with `priority >= priorityThreshold`, if adding it does not exceed the remaining size (initialized to `maxBlockSize`) and the remaining count (initialized to `maxTxCount`), then include it immediately in the result, subtract its size and decrement the count. This ensures forced priority transactions are processed in order, and if a later forced transaction doesn't fit, it is skipped (since order matters and we cannot reorder them). After this, collect all non-priority transactions into a separate list. Sort that list primarily by descending `(fee / size)` (computed as double to avoid integer truncation), then by descending priority, then by ascending ID. For each transaction in this sorted order, if it fits in remaining size and count, include it and update the remaining size and count. Since we are using a greedy fee-per-byte heuristic, the result is not guaranteed to be the optimal 0/1 knapsack solution, but it is a common and efficient approximation that matches the spirit of the original code's priority queue approach. Edge cases: zero-size transactions (should be handled gracefully; if size is 0, it fits any remaining size but still counts toward the transaction count), maxBlockSize or maxTxCount could be 0 (then no transactions are selected), and the input vector may be empty (return empty vector). Time complexity is O(n log n) due to the sort of non-priority transactions, where n is the number of non-priority transactions; space is O(n) for the list and the result.

#include <vector>
#include <algorithm>
#include <cstdint>

struct Transaction {
    uint64_t id;
    size_t size;
    double priority;
    int64_t fee;
};

// Select transactions for a block: forced priority first, then greedy by fee-per-byte.
std::vector<uint64_t> selectTransactions(const std::vector<Transaction>& txs,
                                         size_t maxBlockSize,
                                         size_t maxTxCount,
                                         double priorityThreshold) {
    std::vector<uint64_t> result;
    size_t remainingSize = maxBlockSize;
    size_t remainingCount = maxTxCount;

    // First pass: include all priority transactions in original order, if they fit.
    for (const auto& tx : txs) {
        if (remainingCount == 0 || remainingSize == 0) break;
        if (tx.priority >= priorityThreshold) {
            if (tx.size <= remainingSize) {
                result.push_back(tx.id);
                remainingSize -= tx.size;
                --remainingCount;
            }
        }
    }

    // Collect non-priority transactions.
    std::vector<const Transaction*> nonPriority;
    for (const auto& tx : txs) {
        if (tx.priority < priorityThreshold) {
            nonPriority.push_back(&tx);
        }
    }

    // Sort: fee-per-byte descending, then priority descending, then id ascending.
    std::sort(nonPriority.begin(), nonPriority.end(),
        [](const Transaction* a, const Transaction* b) {
            double rateA = static_cast<double>(a->fee) / (a->size == 0 ? 1.0 : a->size);
            double rateB = static_cast<double>(b->fee) / (b->size == 0 ? 1.0 : b->size);
            if (rateA != rateB) return rateA > rateB;
            if (a->priority != b->priority) return a->priority > b->priority;
            return a->id < b->id;
        });

    // Second pass: include non-priority transactions greedily.
    for (const auto* tx : nonPriority) {
        if (remainingCount == 0 || remainingSize == 0) break;
        if (tx->size <= remainingSize) {
            result.push_back(tx->id);
            remainingSize -= tx->size;
            --remainingCount;
        }
    }

    return result;
}

#include <cassert>
#include <vector>
#include <cstdint>
#include <iostream>

// Transaction struct and selectTransactions function are assumed available here.

int main() {
    // Test 1: Simple priority and fee selection.
    std::vector<Transaction> txs1 = {
        {1, 100, 5.0, 1000},
        {2, 50, 1.0, 500},
        {3, 200, 10.0, 2000},
        {4, 70, 0.5, 700}
    };
    auto result1 = selectTransactions(txs1, 220, 3, 4.0);
    assert((result1 == std::vector<uint64_t>{1, 3, 2})); // priority 1 (100) and 3 (200) don't fit both; only 1 fits, then fees: 2 (50) and 4 (70) but size left=120, so 2 then 4? Actually 50+70=120 fits, but count=1 left after priority, so only 2 (higher fee/byte) fits.

    // Test 2: No priority transactions, pure fee-per-byte.
    std::vector<Transaction> txs2 = {
        {1, 10, 0.0, 10},
        {2, 20, 0.0, 100},
        {3, 30, 0.0, 90}
    };
    auto result2 = selectTransactions(txs2, 50, 5, 100.0);
    // rates: 1:1.0, 2:5.0, 3:3.0 => order 2,3,1; sizes:20+30=50 fits, then 1 too big; result {2,3}
    assert((result2 == std::vector<uint64_t>{2, 3}));

    // Test 3: All priority, but limited size.
    std::vector<Transaction> txs3 = {
        {5, 100, 8.0, 0},
        {6, 200, 9.0, 0}
    };
    auto result3 = selectTransactions(txs3, 150, 2, 5.0);
    assert((result3 == std::vector<uint64_t>{5})); // only first fits.

    // Test 4: Tie in fee-per-byte, priority decides.
    std::vector<Transaction> txs4 = {
        {1, 10, 1.0, 10},
        {2, 10, 2.0, 10},
        {3, 10, 3.0, 10}
    };
    auto result4 = selectTransactions(txs4, 30, 3, 100.0); // no priorities
    // all same rate, sort by priority desc => 3,2,1; all fit
    assert((result4 == std::vector<uint64_t>{3, 2, 1}));

    // Test 5: Tie in rate and priority, lower ID first.
    std::vector<Transaction> txs5 = {
        {5, 10, 0.0, 10},
        {3, 10, 0.0, 10},
        {4, 10, 0.0, 10}
    };
    auto result5 = selectTransactions(txs5, 30, 3, 100.0);
    assert((result5 == std::vector<uint64_t>{3, 4, 5}));

    // Test 6: Zero max size or count.
    auto result6a = selectTransactions(txs1, 0, 10, 0.0);
    assert(result6a.empty());
    auto result6b = selectTransactions(txs1, 1000, 0, 0.0);
    assert(result6b.empty());

    // Test 7: Empty input.
    std::vector<Transaction> empty;
    auto result7 = selectTransactions(empty, 100, 100, 0.0);
    assert(result7.empty());

    // Test 8: Zero-size transactions.
    std::vector<Transaction> txs8 = {
        {1, 0, 0.0, 100},
        {2, 10, 0.0, 10}
    };
    auto result8 = selectTransactions(txs8, 10, 2, 100.0);
    // rate for 1 is treated as 100/1=100, rate for 2=1; so order 1,2; both fit (size 0+10<=10)
    assert((result8 == std::vector<uint64_t>{1, 2}));

    // Test 9: Priority transactions that don't fit are skipped, but later non-priority can fit.
    std::vector<Transaction> txs9 = {
        {1, 200, 5.0, 100}, // priority but too big
        {2, 50, 0.0, 500},
        {3, 50, 0.0, 400}
    };
    auto result9 = selectTransactions(txs9, 100, 2, 4.0);
    // priority 1 doesn't fit, skip; non-priority rates: 2=10,3=8 order 2,3; sizes 50+50=100 fits, count 2
    assert((result9 == std::vector<uint64_t>{2, 3}));

    std::cout << "All tests passed.\n";
    return 0;
}
