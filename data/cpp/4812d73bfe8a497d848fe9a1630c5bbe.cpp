Write a C++ function named `selectBlockTransactions` that simulates the transaction-selection logic from a block-creation algorithm. The function takes a vector of transactions (each with a size, fee, and priority score) and three size thresholds: `blockMaxSize`, `blockPrioritySize`, and `blockMinSize`. It must return a vector of selected transactions (by their indices in the input) that reproduces the key behavior of the original mining code: transactions are processed in a priority order (highest priority first, breaking ties by highest fee), but once the accumulated size exceeds `blockPrioritySize` or a transaction's priority drops below a fixed threshold (use `priorityThreshold = 1000.0` for simplicity), the selection switches to sorting primarily by fee (highest fee first, ties broken by priority). A transaction is skipped if adding it would exceed `blockMaxSize`, or if it is considered "free" (fee per byte below `minFeePerByte = 0.5`) and the accumulated size is already at least `blockMinSize`. Simulate the heap-based priority queue using a std::priority_queue (or equivalent) over a custom comparator that respects the current sorting mode, and return the indices in the order they were added to the block. The input transactions are assumed to be independent (no dependencies), and all values are integers for simplicity.

The core idea is to emulate the priority queue used in the original block assembler. The original algorithm uses a heap (`std::make_heap`/`pop_heap`/`push_heap`) with a comparator that can switch between two modes: initial mode sorts by priority descending (and if equal, by fee descending) until either the block priority size is reached or a low-priority transaction is encountered; after that, the comparator switches to fee-descending (and if equal, priority descending). Because the comparator changes in the middle of the algorithm, simply sorting once is insufficient. Instead, we process transactions in order of the priority-queue: repeatedly pop the highest-priority (or highest-fee) item, check if it fits the block size limit and passes the free-transaction rule, and if accepted, add it. When the switch condition triggers (accumulated size ≥ priority size, or current transaction's priority < threshold), we must rebuild the heap with the new comparator. Since the input set is small, we can avoid a complex heap by repeatedly scanning for the next best transaction based on the current mode, but a cleaner approach is to use a `std::priority_queue` that we recreate when the mode changes. However, the comparator mode is determined by the current state, which changes dynamically. A simpler implementation: maintain a `std::vector` of remaining transactions and each iteration find the best candidate using `std::max_element` with a comparator that uses the current mode. The mode is initially `byPriority`, and we update it to `byFee` permanently once the condition is met. For each selected transaction, we remove it from the remaining set (e.g., mark as used) and add its index to the result. Edge cases: transactions that don't fit are skipped, not retained; free transactions are skipped only when accumulated size ≥ blockMinSize; if multiple transactions tie in both priority and fee, any order is acceptable, but for determinism we can break ties by index. Time complexity: For `n` transactions, each selection scans up to `n` remaining transactions, and we select at most `n`, so O(n²) in the worst case. Space complexity is O(n) for the remaining set and result.

#include <vector>
#include <algorithm>
#include <cstdint>

struct TxData {
    int size;
    int64_t fee;
    double priority;
};

// Compare two transactions based on the current mode.
// Returns true if a should come before b in the selection order.
bool transactionLess(const TxData& a, const TxData& b, bool byFee) {
    if (byFee) {
        if (a.fee != b.fee) return a.fee < b.fee; // higher fee first → lower in order
        if (a.priority != b.priority) return a.priority < b.priority;
    } else {
        if (a.priority != b.priority) return a.priority < b.priority;
        if (a.fee != b.fee) return a.fee < b.fee;
    }
    return false; // equal, keep insertion order via index tie-break handled outside
}

// Simulate transaction selection for a block.
// Returns indices of selected transactions in the order they were added.
std::vector<int> selectBlockTransactions(
    const std::vector<TxData>& txs,
    int blockMaxSize,
    int blockPrioritySize,
    int blockMinSize,
    double priorityThreshold = 1000.0,
    double minFeePerByte = 0.5)
{
    std::vector<int> result;
    std::vector<bool> used(txs.size(), false);
    int blockSize = 0;
    bool byFee = (blockPrioritySize <= 0);

    for (;;) {
        // Find best candidate among unused transactions according to current mode.
        int bestIdx = -1;
        for (size_t i = 0; i < txs.size(); ++i) {
            if (used[i]) continue;
            const TxData& tx = txs[i];
            // Skip if doesn't fit or free rule
            if (byFee && (tx.fee / (double)tx.size < minFeePerByte) && (blockSize >= blockMinSize))
                continue;
            if (bestIdx == -1) {
                bestIdx = static_cast<int>(i);
            } else {
                bool better = false;
                if (byFee) {
                    if (tx.fee != txs[bestIdx].fee)
                        better = tx.fee > txs[bestIdx].fee;
                    else if (tx.priority != txs[bestIdx].priority)
                        better = tx.priority > txs[bestIdx].priority;
                    else
                        better = i < static_cast<size_t>(bestIdx);
                } else {
                    if (tx.priority != txs[bestIdx].priority)
                        better = tx.priority > txs[bestIdx].priority;
                    else if (tx.fee != txs[bestIdx].fee)
                        better = tx.fee > txs[bestIdx].fee;
                    else
                        better = i < static_cast<size_t>(bestIdx);
                }
                if (better) bestIdx = static_cast<int>(i);
            }
        }
        if (bestIdx == -1) break; // no more usable transactions

        const TxData& best = txs[bestIdx];
        if (blockSize + best.size >= blockMaxSize) {
            // Does not fit, so remove it from consideration (mark used to avoid retry)
            used[bestIdx] = true;
            continue;
        }

        // Add to block
        result.push_back(bestIdx);
        blockSize += best.size;
        used[bestIdx] = true;

        // Switch to fee mode if not already and threshold reached
        if (!byFee) {
            if (blockSize >= blockPrioritySize || best.priority < priorityThreshold) {
                byFee = true;
            }
        }
        // Note: after switching, the next loop will use fee-based selection
    }
    return result;
}

#include <cassert>
#include <vector>

// (The solution function is expected to be defined above; tests call it directly.)
int main() {
    using std::vector;
    using std::pair;

    // Test 1: Basic high-priority selection with enough room
    vector<TxData> txs1 = {
        {100, 1000, 5000.0},  // high priority
        {200, 2000, 3000.0},
        {50, 100, 900.0}      // low priority
    };
    vector<int> sel1 = selectBlockTransactions(txs1, 1000, 300, 0);
    // Priority mode: first take highest priority (idx0), then idx1 (size 300, still under 300? Actually after idx0 size=100, next best is idx1, size=300, total=400, switch condition? blockSize=100 < 300, priority 3000 >= 1000, so still byPriority, then idx2? But idx2 priority 900 < 1000, would trigger switch before adding? In loop, we check switch only after adding. Let's see: after idx1, blockSize=300, condition blockSize>=300 true, switch to byFee. Remaining: idx2. byFee: fee/50=2.0 >= 0.5, size 50, blockSize=350 < 1000, add idx2. So order: 0,1,2.
    assert(sel1 == vector<int>({0,1,2}));

    // Test 2: Block size limit skips a big transaction
    vector<TxData> txs2 = {
        {800, 5000, 10000.0}, // too big to fit with others?
        {100, 100, 9000.0},
        {100, 200, 8000.0}
    };
    vector<int> sel2 = selectBlockTransactions(txs2, 200, 200, 0);
    // Priority mode: first best is idx0 (size 800, blockSize 0, 0+800>=200? if >= blockMaxSize, skip before adding. Actually condition: if (blockSize + best.size >= blockMaxSize) continue; so idx0 skipped because 0+800>=200 is true. Then idx1 (size100), blockSize=100, then idx2 (size100), blockSize=200. Order: 1,2.
    assert(sel2 == vector<int>({1,2}));

    // Test 3: Free transaction skipped after min size reached
    vector<TxData> txs3 = {
        {100, 0, 1000.0},     // free, low priority
        {100, 1000, 100.0},   // low priority but pays fee
        {100, 2000, 2000.0}   // high priority
    };
    vector<int> sel3 = selectBlockTransactions(txs3, 1000, 200, 100, 1000.0, 0.5);
    // Priority mode: best idx2 (priority 2000), size100, blockSize=100. check switch: blockSize<200, priority 2000>=1000, still byPriority. Next best: idx0 (priority 1000) vs idx1 (priority 100), so idx0. size100, blockSize=200, now blockSize>=200 → switch to byFee. Remaining: idx1 (fee=1000, size100, fee/byte=10) and idx0? idx0 already used. So select idx1. Order: 2,0,1.
    assert(sel3 == vector<int>({2,0,1}));

    // Test 4: All free transactions, min size threshold blocks them
    vector<TxData> txs4 = {
        {50, 0, 100.0},
        {50, 0, 200.0}
    };
    vector<int> sel4 = selectBlockTransactions(txs4, 1000, 0, 200, 1000.0, 0.5);
    // blockPrioritySize=0 → byFee from start. Both have fee=0, so compare fee/byte=0 < 0.5, and blockSize=0 < minSize (200)? The skip condition: if byFee && (fee/size < minFeePerByte) && (blockSize >= blockMinSize) → blockSize=0 < 200, so not skipped. Both selected in fee/priority tie order by index: idx0 then idx1.
    assert(sel4 == vector<int>({0,1}));

    // Test 5: Switch to fee mode when a low-priority transaction is encountered
    vector<TxData> txs5 = {
        {100, 5000, 500.0},   // low priority but high fee
        {100, 100, 2000.0},   // high priority low fee
        {100, 200, 1500.0}
    };
    vector<int> sel5 = selectBlockTransactions(txs5, 1000, 1000, 0, 1000.0, 0.5);
    // priority mode: best idx1 (priority 2000), size100, blockSize=100. blockSize < 1000, priority 2000 >= 1000, still byPriority. Next best idx2 (priority 1500), size100, blockSize=200. Next best idx0 (priority 500 < 1000) → before adding, switch occurs? Actually switch check happens after adding idx2, blockSize=200, best.priority for idx0 is 500 < 1000, but we haven't added idx0 yet. In our loop, we find best among remaining using current mode. After adding idx1 and idx2, blockSize=200, still byPriority (blockSize<1000 and previous best priority >=1000). The next best in byPriority is idx0 (priority 500). After we pop (find best) it, we check if it fits (yes), then check switch condition: blockSize=200 < 1000 and best.priority=500 < 1000 → set byFee=true. Then we add idx0. Order: 1,2,0.
    assert(sel5 == vector<int>({1,2,0}));

    return 0;
}
