/*
Write a C++ function `int cheapestAffordableStay(int participants, int budget, const std::vector<int>& pricesPerPerson, const std::vector<std::vector<int>>& availability)` that, given a fixed number of participants, a maximum total budget, per-person weekly prices for each hotel, and a grid of available beds per weekend for each hotel, returns the minimum total cost for a stay if an affordable option exists, otherwise returns -1. Participants require exactly one weekend of accommodation, and the total cost for a chosen hotel is `pricePerPerson * participants`. A hotel is viable for a specific weekend only if its available beds on that weekend are at least the number of participants. The function must consider all hotels and all weekends, and pick the cheapest viable combination that does not exceed the budget. Input data may contain duplicate hotels or weekends, and any hotel may have zero availability for some weekends.
*/
#include <vector>
#include <algorithm>
#include <limits>

// Returns the minimum total cost to accommodate 'participants' people for one weekend
// using any hotel and weekend, provided the cost does not exceed 'budget'.
// If no such option exists, returns -1.
int cheapestAffordableStay(int participants, int budget,
                           const std::vector<int>& pricesPerPerson,
                           const std::vector<std::vector<int>>& availability) {
    // Validate basic constraints: negative participants or budget cannot be satisfied.
    if (participants < 0 || budget < 0) {
        return -1;
    }

    const int INF = std::numeric_limits<int>::max();
    int bestCost = INF;

    const std::size_t hotelCount = pricesPerPerson.size();

    for (std::size_t h = 0; h < hotelCount; ++h) {
        const int price = pricesPerPerson[h];
        if (price < 0) continue; // ignore nonsensical negative prices

        // Determine how many weekends this hotel has (if fewer weekends, skip).
        const std::size_t weekendCount = (h < availability.size()) ? availability[h].size() : 0;

        for (std::size_t w = 0; w < weekendCount; ++w) {
            const int availableBeds = availability[h][w];
            if (availableBeds >= participants) {
                const long long totalCost = static_cast<long long>(price) * participants;
                // Avoid overflow by clamping to int range.
                if (totalCost <= budget && totalCost < bestCost) {
                    bestCost = static_cast<int>(totalCost);
                }
            }
        }
    }

    return (bestCost == INF) ? -1 : bestCost;
}
#include <cassert>
#include <vector>

int main() {
    // Basic case: two hotels, two weekends each.
    {
        std::vector<int> prices = {100, 80};
        std::vector<std::vector<int>> avail = {{2, 10}, {5, 1}};
        assert(cheapestAffordableStay(2, 200, prices, avail) == 160); // hotel2 price80*2=160, within budget
    }

    // No affordable option because total cost exceeds budget.
    {
        std::vector<int> prices = {100, 90};
        std::vector<std::vector<int>> avail = {{2, 2}, {2, 2}};
        assert(cheapestAffordableStay(2, 150, prices, avail) == -1); // both cost >=180 >150
    }

    // Zero participants: cost is 0 for any viable hotel, but we treat as 0 only if budget >=0.
    {
        std::vector<int> prices = {50, 70};
        std::vector<std::vector<int>> avail = {{0, 1}, {0, 0}};
        assert(cheapestAffordableStay(0, 0, prices, avail) == 0); // 0*price = 0, within budget
    }

    // No weekend has enough beds.
    {
        std::vector<int> prices = {30};
        std::vector<std::vector<int>> avail = {{1, 2}};
        assert(cheapestAffordableStay(3, 100, prices, avail) == -1); // need 3, only max 2
    }

    // Multiple hotels, one weekend each, pick minimum viable.
    {
        std::vector<int> prices = {50, 40, 60};
        std::vector<std::vector<int>> avail = {{2}, {3}, {1}};
        assert(cheapestAffordableStay(2, 200, prices, avail) == 80); // hotel2 40*2=80
    }

    // Budget exactly matches the minimum viable cost.
    {
        std::vector<int> prices = {10, 20};
        std::vector<std::vector<int>> avail = {{5, 5}, {5, 5}};
        assert(cheapestAffordableStay(5, 50, prices, avail) == 50); // hotel1 10*5=50 <=50
    }

    // Negative budget handled.
    {
        std::vector<int> prices = {1};
        std::vector<std::vector<int>> avail = {{1}};
        assert(cheapestAffordableStay(1, -1, prices, avail) == -1);
    }

    // Empty hotel list.
    {
        std::vector<int> prices;
        std::vector<std::vector<int>> avail;
        assert(cheapestAffordableStay(10, 1000, prices, avail) == -1);
    }

    // Hotel with negative price is ignored.
    {
        std::vector<int> prices = {-5, 100};
        std::vector<std::vector<int>> avail = {{10, 10}, {10, 10}};
        assert(cheapestAffordableStay(2, 500, prices, avail) == 200); // only positive price considered
    }

    // Large numbers to ensure no overflow.
    {
        std::vector<int> prices = {1000000};
        std::vector<std::vector<int>> avail = {{1000000000}};
        // participants*price = 10^6 * 10^6 = 10^12 > int max, should be skipped.
        assert(cheapestAffordableStay(1000000, 2000000000, prices, avail) == -1); // overflow avoided
    }

    return 0;
}
// The solution is a straightforward exhaustive search over all hotels and weekends. For each hotel, iterate through every weekend’s availability count. If the available beds for that weekend are at least the number of participants, compute the total cost as `pricePerPerson * participants`. Keep track of the minimum cost found across all viable combinations. After checking all hotels, if the minimum cost is still the initial maximum sentinel (or if the minimum exceeds the budget), return -1; otherwise return that minimum. Edge cases: if participants is zero, any hotel is viable at cost 0, but we treat it normally; if there are no hotels or no weekends, the function returns -1; if budget is negative, return -1 because no positive cost can be affordable. The algorithm runs in O(H * W) time where H is the number of hotels and W is the maximum number of weekends per hotel (assuming the grid is rectangular). Auxiliary space is O(1) beyond the input vectors.
