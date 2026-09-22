/*
Write a C++ function named `isTransactionReplaceable` that determines whether a given transaction (represented as a struct with a unique ID, a flag `signalsRBF` indicating whether it explicitly signals Replace-By-Fee, a set of input indices, and a parent transaction ID for unconfirmed predecessors) should be considered replaceable under BIP125 rules. The function must consider the transaction itself and all of its unconfirmed ancestors in the mempool (a simplified container storing transactions). If the transaction itself signals RBF, it is replaceable. If the transaction is not present in the mempool, the result is `UNKNOWN` (since we cannot know all inputs). If the transaction is present, check all unconfirmed ancestor transactions (following parent pointers recursively, including self and all ancestors); if any ancestor signals RBF, the result is `REPLACEABLE`. Otherwise, the result is `FINAL`. Also provide a helper that checks only the transaction itself when the mempool is empty. Use an enum `RBFState` with values `REPLACEABLE`, `UNKNOWN`, `FINAL`. The mempool will be a `std::unordered_map<TxID, Transaction>`, and transactions have `TxID`, `bool signalsRBF`, `TxID parentID` (0 if none). Ancestor collection must avoid cycles. The function signature: `RBFState isTransactionReplaceable(const Transaction& tx, const std::unordered_map<TxID, Transaction>& mempool)`.
*/
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <cstdint>

// Enumeration for replaceability state.
enum class RBFState {
    REPLACEABLE, // Signals BIP125 replaceability
    UNKNOWN,     // Cannot determine because transaction not in mempool
    FINAL        // Not replaceable
};

// Simple transaction struct.
struct Transaction {
    using TxID = uint64_t;
    TxID id;
    bool signalsRBF;
    TxID parentID; // 0 if none
};

// Helper to collect all ancestors (including self) from mempool.
void collectAncestors(const Transaction& tx,
                      const std::unordered_map<Transaction::TxID, Transaction>& mempool,
                      std::unordered_set<Transaction::TxID>& visited,
                      std::vector<const Transaction*>& ancestors) {
    if (visited.count(tx.id)) return;
    visited.insert(tx.id);
    ancestors.push_back(&tx);
    if (tx.parentID != 0) {
        auto it = mempool.find(tx.parentID);
        if (it != mempool.end()) {
            collectAncestors(it->second, mempool, visited, ancestors);
        }
    }
}

// Main solution function.
RBFState isTransactionReplaceable(const Transaction& tx,
                                  const std::unordered_map<Transaction::TxID, Transaction>& mempool) {
    // First check the transaction itself.
    if (tx.signalsRBF) {
        return RBFState::REPLACEABLE;
    }

    // If transaction not in mempool, we cannot know all inputs.
    if (mempool.find(tx.id) == mempool.end()) {
        return RBFState::UNKNOWN;
    }

    // Collect all unconfirmed ancestors, including self.
    std::unordered_set<Transaction::TxID> visited;
    std::vector<const Transaction*> ancestors;
    collectAncestors(tx, mempool, visited, ancestors);

    // Check all ancestors for RBF signal.
    for (const auto* anc : ancestors) {
        if (anc->signalsRBF) {
            return RBFState::REPLACEABLE;
        }
    }

    return RBFState::FINAL;
}

// Helper for empty mempool case.
RBFState isTransactionReplaceableEmptyMempool(const Transaction& tx) {
    return tx.signalsRBF ? RBFState::REPLACEABLE : RBFState::UNKNOWN;
}
#include <cassert>
#include <unordered_map>

int main() {
    using TxID = Transaction::TxID;

    // Helper to create transactions.
    auto makeTx = [](TxID id, bool rbf, TxID parent) {
        return Transaction{id, rbf, parent};
    };

    // Build a mempool: tx1 (id=1) no RBF, parent=0; tx2 (id=2) no RBF, parent=1; tx3 (id=3) RBF, parent=2.
    std::unordered_map<TxID, Transaction> mempool;
    mempool[1] = makeTx(1, false, 0);
    mempool[2] = makeTx(2, false, 1);
    mempool[3] = makeTx(3, true, 2);

    // Transaction itself signals RBF -> replaceable.
    Transaction txSelf = makeTx(4, true, 0);
    assert(isTransactionReplaceable(txSelf, mempool) == RBFState::REPLACEABLE);

    // Transaction not in mempool, no RBF -> unknown.
    Transaction txNotInPool = makeTx(99, false, 0);
    assert(isTransactionReplaceable(txNotInPool, mempool) == RBFState::UNKNOWN);

    // Transaction in mempool, no RBF signal on itself or ancestors -> final.
    assert(isTransactionReplaceable(mempool[1], mempool) == RBFState::FINAL);

    // Transaction in mempool, has ancestor with RBF -> replaceable.
    assert(isTransactionReplaceable(mempool[2], mempool) == RBFState::REPLACEABLE);
    assert(isTransactionReplaceable(mempool[3], mempool) == RBFState::REPLACEABLE);

    // Cycle handling: tx5 parent=6, tx6 parent=5, neither signals RBF.
    std::unordered_map<TxID, Transaction> cyclicMempool;
    cyclicMempool[5] = makeTx(5, false, 6);
    cyclicMempool[6] = makeTx(6, false, 5);
    assert(isTransactionReplaceable(cyclicMempool[5], cyclicMempool) == RBFState::FINAL);

    // Empty mempool helper.
    Transaction txRbf = makeTx(10, true, 0);
    Transaction txNoRbf = makeTx(11, false, 0);
    assert(isTransactionReplaceableEmptyMempool(txRbf) == RBFState::REPLACEABLE);
    assert(isTransactionReplaceableEmptyMempool(txNoRbf) == RBFState::UNKNOWN);

    return 0;
}
// The algorithm first checks the transaction's own `signalsRBF` flag; if true, return `REPLACEABLE`. Next, verify that the transaction's ID exists in the mempool; if not, return `UNKNOWN`. Then perform a traversal over ancestors starting from the given transaction. Use an iterative stack or recursion with a visited set to avoid cycles (since parent chains could theoretically loop, though in practice they shouldn’t; we handle it defensively). For each visited transaction, if its `signalsRBF` flag is true, return `REPLACEABLE`. If no ancestor signals, return `FINAL`. The helper `isTransactionReplaceableEmptyMempool` simply checks the transaction’s own flag and returns `UNKNOWN` otherwise, matching the logic that without a mempool we can only know the transaction itself. Time complexity is O(A) where A is the number of ancestors (in worst case number of transactions in mempool reachable), and space complexity O(A) for the visited set and call stack. Edge cases: transaction not in mempool → UNKNOWN even if it signals RBF? Actually, if it signals RBF itself, we return REPLACEABLE before checking mempool, so that’s fine. If it doesn’t signal and isn’t in mempool → UNKNOWN. If it is in mempool, we check all ancestors including itself; self is included in traversal, but its own flag was already checked, so duplication is harmless. Cycles are handled by visited set.
