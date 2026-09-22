Write a C++ function `bool canGiveChange(const std::vector<int>& bills)` that simulates a lemonade stand where each customer pays with a $5, $10, or $20 bill and always buys one lemonade costing $5. The function should return `true` if the seller can provide exact change to every customer in the given order, and `false` otherwise. The seller starts with no money. Each customer must be served in sequence, and the seller may use bills received from previous customers to make change. For a $10 bill, the seller must return one $5 bill; for a $20 bill, the seller must return $15, either as one $10 + one $5 or as three $5 bills (prefer using the $10 if available). The function should handle empty input (returning `true`), large inputs efficiently, and any mix of bill denominations without assuming the input is sorted.

#include <cassert>
#include <vector>

int main() {
    // Empty input: no customers, always succeeds.
    assert(canGiveChange({}) == true);

    // Simple cases with only $5 bills.
    assert(canGiveChange({5}) == true);
    assert(canGiveChange({5, 5, 5}) == true);

    // First customer with $10 or $20 fails because no change available.
    assert(canGiveChange({10}) == false);
    assert(canGiveChange({20}) == false);

    // Classic sequence from LeetCode example.
    assert(canGiveChange({5, 5, 5, 10, 20}) == true);
    assert(canGiveChange({5, 5, 10, 10, 20}) == false);

    // Prefer using $10 for $20 change to save $5s.
    assert(canGiveChange({5, 10, 5, 20}) == true);
    assert(canGiveChange({5, 5, 5, 20}) == true); // three $5s for change
    assert(canGiveChange({5, 5, 10, 20}) == true); // $10 + $5 used

    // Failing case: insufficient $5s due to spending them on $10s.
    assert(canGiveChange({5, 10, 10, 20}) == false);

    // Large input with many $5s and $20s, still works.
    std::vector<int> manyFives(100, 5);
    assert(canGiveChange(manyFives) == true);
    std::vector<int> mixed;
    for (int i = 0; i < 100; ++i) {
        mixed.push_back(5);
        mixed.push_back(5);
        mixed.push_back(10);
        mixed.push_back(20);
    }
    assert(canGiveChange(mixed) == true);

    return 0;
}

#include <vector>

// Returns true if the seller can give exact change for every customer.
bool canGiveChange(const std::vector<int>& bills) {
    int fiveCount = 0;
    int tenCount = 0;

    for (int bill : bills) {
        if (bill == 5) {
            ++fiveCount;
        } else if (bill == 10) {
            ++tenCount;
            --fiveCount; // Must give a $5 as change
        } else if (bill == 20) {
            // Prefer using $10 + $5, else three $5 bills
            if (tenCount > 0) {
                --tenCount;
                --fiveCount;
            } else {
                fiveCount -= 3;
            }
        }
        // If we run out of $5 bills at any point, change is impossible.
        if (fiveCount < 0) {
            return false;
        }
    }
    return true;
}

// The core idea is to greedily track the count of $5 and $10 bills the seller has on hand, since $20 bills are never used for making change (no one pays with larger than $20, and $20 cannot be given as change). For each bill received, simulate the transaction:  
// - If it's a $5, increment the five-count.  
// - If it's a $10, we need to give a $5 as change, so increment ten-count and decrement five-count.  
// - If it's a $20, we need $15 change. To preserve smaller bills for future customers, we prefer to give one $10 and one $5 if a $10 is available; otherwise, we give three $5 bills. Then decrement the counts accordingly.  
// After each transaction, if the five-count becomes negative, it means we cannot make change, so return `false` immediately.  
// Edge cases: an empty list returns `true` (no customers). If the first customer pays with anything larger than $5, it will fail because we have no $5 bills. Also, we must never let the ten-count go negative (which is implicitly prevented because we only decrement ten when we have it, and five-count check catches failures).  
// Time complexity is O(n) where n is the number of bills, since we process each once. Space complexity is O(1) beyond the input vector, as we only store two integer counters.
