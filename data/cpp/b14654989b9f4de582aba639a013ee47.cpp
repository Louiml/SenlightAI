/*
Write a C++ function `std::vector<long long> assignTickets(const std::vector<long long>& ticketPrices, const std::vector<long long>& customerBudgets)` that processes ticket sales for a cinema. There are `n` tickets with given prices and `m` customers, each with a maximum budget. For each customer in the given order, find and return the most expensive ticket they can afford (price ≤ budget). After a ticket is sold, it is removed from availability. If no ticket is affordable, output `-1` for that customer. The function should return a vector of length `m` containing the sold ticket price (or `-1`) for each customer in order. All prices and budgets are positive integers up to 10^9, and `n` and `m` can be up to 2×10^5. Duplicate prices are allowed and represent distinct tickets.
*/

#include <vector>
#include <set>
#include <cstddef>

// Assigns the most expensive affordable ticket to each customer in order.
// ticketPrices: available ticket prices (may contain duplicates)
// customerBudgets: each customer's maximum price
// Returns: for each customer, the sold ticket price, or -1 if none affordable
std::vector<long long> assignTickets(const std::vector<long long>& ticketPrices,
                                     const std::vector<long long>& customerBudgets) {
    std::multiset<long long> available(ticketPrices.begin(), ticketPrices.end());
    std::vector<long long> result;
    result.reserve(customerBudgets.size());

    for (long long budget : customerBudgets) {
        // No ticket available or the cheapest ticket exceeds budget
        if (available.empty() || budget < *available.begin()) {
            result.push_back(-1);
            continue;
        }
        // Find first ticket price > budget, then step back to the last ≤ budget
        auto it = available.upper_bound(budget);
        // upper_bound returns an iterator; if it equals begin(), then all > budget,
        // but we already checked budget >= *begin(), so it is safe to decrement.
        --it;
        long long soldPrice = *it;
        available.erase(it);
        result.push_back(soldPrice);
    }
    return result;
}

#include <cassert>
#include <vector>

// The solution function is included above in the same translation unit.

int main() {
    // Basic case with distinct prices
    std::vector<long long> prices1 = {5, 3, 8, 1};
    std::vector<long long> budgets1 = {6, 2, 10, 0};
    std::vector<long long> expected1 = {5, 1, 8, -1};
    assert(assignTickets(prices1, budgets1) == expected1);

    // Duplicate prices: sell one occurrence at a time
    std::vector<long long> prices2 = {7, 7, 7};
    std::vector<long long> budgets2 = {7, 7, 7, 7};
    std::vector<long long> expected2 = {7, 7, 7, -1};
    assert(assignTickets(prices2, budgets2) == expected2);

    // Empty ticket list
    std::vector<long long> prices3;
    std::vector<long long> budgets3 = {1, 2, 3};
    std::vector<long long> expected3 = {-1, -1, -1};
    assert(assignTickets(prices3, budgets3) == expected3);

    // Empty customer list
    std::vector<long long> prices4 = {10, 20};
    std::vector<long long> budgets4;
    std::vector<long long> expected4;
    assert(assignTickets(prices4, budgets4) == expected4);

    // All tickets sold before some customers
    std::vector<long long> prices5 = {100};
    std::vector<long long> budgets5 = {50, 100, 200};
    std::vector<long long> expected5 = {-1, 100, -1};
    assert(assignTickets(prices5, budgets5) == expected5);

    // Large values and precise boundary
    std::vector<long long> prices6 = {1000000000LL, 1LL};
    std::vector<long long> budgets6 = {1000000000LL, 1LL, 1000000000LL};
    std::vector<long long> expected6 = {1000000000LL, 1LL, -1};
    assert(assignTickets(prices6, budgets6) == expected6);

    // Mixed order: some customers get -1, some get tickets
    std::vector<long long> prices7 = {4, 6, 9};
    std::vector<long long> budgets7 = {5, 7, 3, 8, 10};
    std::vector<long long> expected7 = {4, 6, -1, 9, -1};
    assert(assignTickets(prices7, budgets7) == expected7);

    return 0;
}

// The optimal approach is to maintain a sorted multiset of available ticket prices, allowing fast retrieval of the largest price not exceeding a given budget. In C++, `std::multiset` preserves duplicates and supports `lower_bound`/`upper_bound` in O(log n) time. For each customer budget `b`, first check if the multiset is empty or if the smallest available price is greater than `b`; if so, output `-1`. Otherwise, use `upper_bound(b)` to find the first element strictly greater than `b`, then decrement the iterator to get the largest ticket price ≤ `b`. Remove that price from the multiset (one occurrence). If no such price exists (i.e., `upper_bound` returns `begin()` and the first element is > b, which we already checked), handle gracefully. Edge cases include: empty ticket list, empty customer list, multiple customers with same budget, and when all tickets are sold before later customers. Time complexity is O((n+m) log n) for insertions and queries; space complexity is O(n).
