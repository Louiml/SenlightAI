Write a C++ function that models a simple order book and simulates limit order execution for a single asset. The function should accept a vector of integer prices representing sequential market ticks, a buy-limit price, a sell-limit price, and a quantity. It must return a struct (or `std::pair`) containing the total executed buy quantity, total executed sell quantity, and final cash balance, assuming that at each tick, the function attempts to match the best available limit orders first (i.e., if the market price is at or below the buy limit, a buy executes for the full remaining quantity; if at or above the sell limit, a sell executes), and that unfilled orders remain active until filled or the ticks end. Handle edge cases such as empty ticks, zero quantity, and price bounds (negative or zero prices). The solution must be self-contained with no external dependencies beyond the C++ standard library.

#include <cassert>
#include <vector>

int main() {
    // Basic execution: both orders fill completely.
    std::vector<int> ticks1 = {90, 95, 100, 105, 110};
    auto r1 = simulateLimitOrders(ticks1, 100, 100, 10);
    assert(r1.boughtQty == 10);
    assert(r1.soldQty == 10);
    assert(r1.cashBalance == 0.0); // bought at 100, sold at 100

    // Only buys execute because sell limit never reached.
    std::vector<int> ticks2 = {80, 85, 90, 95};
    auto r2 = simulateLimitOrders(ticks2, 90, 120, 5);
    assert(r2.boughtQty == 5);
    assert(r2.soldQty == 0);
    assert(r2.cashBalance == -450.0); // 5 * -90

    // Only sells execute because buy limit never reached.
    std::vector<int> ticks3 = {110, 115, 120, 125};
    auto r3 = simulateLimitOrders(ticks3, 80, 120, 7);
    assert(r3.boughtQty == 0);
    assert(r3.soldQty == 7);
    assert(r3.cashBalance == 840.0); // 7 * 120

    // Empty ticks: no execution.
    auto r4 = simulateLimitOrders({}, 100, 100, 10);
    assert(r4.boughtQty == 0);
    assert(r4.soldQty == 0);
    assert(r4.cashBalance == 0.0);

    // Zero quantity: nothing happens.
    auto r5 = simulateLimitOrders({95, 96, 97}, 100, 100, 0);
    assert(r5.boughtQty == 0);
    assert(r5.soldQty == 0);
    assert(r5.cashBalance == 0.0);

    // Tick satisfies both buy and sell conditions simultaneously (price = buyLimit = sellLimit).
    std::vector<int> ticks6 = {100, 100, 100};
    auto r6 = simulateLimitOrders(ticks6, 100, 100, 3);
    assert(r6.boughtQty == 3);
    assert(r6.soldQty == 3);
    assert(r6.cashBalance == 0.0);

    // Negative prices are allowed; cash reflects signed values.
    std::vector<int> ticks7 = {-5, -6, -7};
    auto r7 = simulateLimitOrders(ticks7, -5, -2, 4);
    assert(r7.boughtQty == 4);
    assert(r7.soldQty == 0);
    assert(r7.cashBalance == 20.0); // 4 * -5 = -20, so cash -20? Wait: bought at -5 => cash = - (4 * -5) = +20. Yes.

    return 0;
}

#include <vector>
#include <utility>   // for std::pair

// Result structure: boughtQty, soldQty, cashBalance (positive means net cash inflow)
struct OrderExecutionResult {
    int boughtQty;
    int soldQty;
    double cashBalance;
};

// Simulate limit order execution given a series of market prices.
// buyLimitPrice and sellLimitPrice are the resting limit order prices.
// quantityPerTick is the maximum shares that can be executed per tick per side.
// Returns the total bought, sold, and final cash balance.
OrderExecutionResult simulateLimitOrders(
    const std::vector<int>& ticks,
    int buyLimitPrice,
    int sellLimitPrice,
    int quantityPerTick
) {
    // Initialize result
    OrderExecutionResult result{0, 0, 0.0};
    if (ticks.empty() || quantityPerTick <= 0) {
        return result;
    }

    // Remaining quantities for the buy and sell orders (assume equal initial quantities)
    int remainingBuy = quantityPerTick;   // total we intend to buy
    int remainingSell = quantityPerTick;  // total we intend to sell

    // Process each tick in order
    for (int price : ticks) {
        // Buy execution: if market price is at or below buy limit
        if (remainingBuy > 0 && price <= buyLimitPrice) {
            // Execute min(remainingBuy, quantityPerTick) shares at buyLimitPrice
            int executed = std::min(remainingBuy, quantityPerTick);
            result.boughtQty += executed;
            result.cashBalance -= executed * static_cast<double>(buyLimitPrice); // cash decreases
            remainingBuy -= executed;
        }

        // Sell execution: if market price is at or above sell limit
        if (remainingSell > 0 && price >= sellLimitPrice) {
            int executed = std::min(remainingSell, quantityPerTick);
            result.soldQty += executed;
            result.cashBalance += executed * static_cast<double>(sellLimitPrice); // cash increases
            remainingSell -= executed;
        }

        // Early exit if both orders are fully filled
        if (remainingBuy == 0 && remainingSell == 0) {
            break;
        }
    }

    return result;
}

// We need to simulate a basic two-sided market with one resting buy limit order and one resting sell limit order. For each tick price, we check if the price is ≤ buyLimit (then we buy min(remainingBuyQty, quantity) shares at buyLimit) and if price ≥ sellLimit (then we sell min(remainingSellQty, quantity) shares at sellLimit). The quantity parameter is the maximum shares that can be executed per tick on each side. We accumulate bought and sold quantities and compute cash as soldQty*sellLimit – boughtQty*buyLimit (cash inflow from sells, outflow from buys). We stop when both orders are fully filled or we run out of ticks. Important edge cases: empty ticks vector (return zeros), quantity=0 (do nothing, cash=0), limits may be zero or negative (still valid prices, but we use them as thresholds), if a tick satisfies both buy and sell conditions, we execute both in the same tick (buy first, then sell, order doesn’t matter since each is independent). Time complexity O(T) where T is number of ticks; space O(1) beyond the input copy.
