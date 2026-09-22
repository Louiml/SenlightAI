// Write a C++ function `int cheapestRoute(int source, int destination, const std::vector<std::vector<int>>& routes, const std::vector<int>& costs)` that takes a starting city `source`, a target city `destination`, a list of `routes` (each route is a vector of city IDs in the order the route visits them), and a parallel list of integer costs (one per route). The function must return the minimum cost among all routes that contain both `source` and `destination` **and** where the index of `source` is strictly less than the index of `destination` (i.e., the route travels from `source` to `destination` in that order). If no such route exists, return `-1`. Assume all city IDs are positive integers, costs are positive, and each route has at least one city. Do not assume that the input is sorted.

// The core idea is to iterate over each route independently. For a given route, check whether it contains both the source and destination cities. If it contains both, find the index (position) of each within that route. The route is usable only if the index of the source is smaller than the index of the destination—meaning the route visits the source before the destination. If that condition holds, compare the route’s cost to the current minimum, and update the minimum if the route is cheaper (or if it is the first valid route found). Since each route is independent, we do not need to combine routes; the problem restricts us to a single route for the whole trip. Edge cases: a route may contain the source but not the destination, or vice versa—skip those. A route may contain both but with the destination appearing before the source—skip those too. If no route is valid, return `-1`. The time complexity is \(O(N \times L)\) where \(N\) is the number of routes and \(L\) is the maximum route length, since for each route we scan the entire vector to locate both cities. The space complexity is \(O(1)\) auxiliary, not counting the input storage.

#include <vector>
#include <algorithm>

// Returns the minimum cost of a single route that visits source before destination,
// or -1 if no such route exists.
int cheapestRoute(int source, int destination,
                  const std::vector<std::vector<int>>& routes,
                  const std::vector<int>& costs) {
    int minCost = -1;

    for (size_t i = 0; i < routes.size(); ++i) {
        const std::vector<int>& route = routes[i];

        // Find positions of source and destination in this route.
        auto itSource = std::find(route.begin(), route.end(), source);
        auto itDest = std::find(route.begin(), route.end(), destination);

        // Check if both cities are present and source appears before destination.
        bool sourceFound = (itSource != route.end());
        bool destFound = (itDest != route.end());
        bool sourceBeforeDest = sourceFound && destFound && (itSource < itDest);

        if (sourceBeforeDest) {
            if (minCost == -1 || costs[i] < minCost) {
                minCost = costs[i];
            }
        }
    }

    return minCost;
}

#include <cassert>
#include <vector>

// Function under test is declared above.

int main() {
    // Basic case: route 0 works, route 1 does not.
    std::vector<std::vector<int>> routes1 = {{1, 2, 3, 4}, {5, 6, 1, 4}};
    std::vector<int> costs1 = {100, 50};
    assert(cheapestRoute(1, 4, routes1, costs1) == 100);

    // No valid route: destination before source.
    std::vector<std::vector<int>> routes2 = {{4, 3, 1}, {1, 4}};
    std::vector<int> costs2 = {10, 20};
    assert(cheapestRoute(1, 4, routes2, costs2) == 20);

    // No route contains both cities.
    std::vector<std::vector<int>> routes3 = {{1, 2}, {3, 4}};
    std::vector<int> costs3 = {5, 6};
    assert(cheapestRoute(1, 4, routes3, costs3) == -1);

    // Choose cheapest among multiple valid routes.
    std::vector<std::vector<int>> routes4 = {{1, 5, 4}, {2, 1, 4}, {1, 4}};
    std::vector<int> costs4 = {30, 10, 20};
    assert(cheapestRoute(1, 4, routes4, costs4) == 10);

    // Same city for source and destination: route must contain it (index equal), but sourceBeforeDest requires less, so none valid.
    std::vector<std::vector<int>> routes5 = {{1, 2, 1}};
    std::vector<int> costs5 = {7};
    assert(cheapestRoute(1, 1, routes5, costs5) == -1);

    // Source at end, destination at start: invalid.
    std::vector<std::vector<int>> routes6 = {{4, 2, 1}};
    std::vector<int> costs6 = {3};
    assert(cheapestRoute(1, 4, routes6, costs6) == -1);

    // Single route valid with multiple occurrences.
    std::vector<std::vector<int>> routes7 = {{1, 2, 1, 4, 3}};
    std::vector<int> costs7 = {8};
    assert(cheapestRoute(1, 4, routes7, costs7) == 8);

    // Empty routes list.
    std::vector<std::vector<int>> routes8;
    std::vector<int> costs8;
    assert(cheapestRoute(1, 2, routes8, costs8) == -1);

    // Only one city in route: source==destination, but no progress.
    std::vector<std::vector<int>> routes9 = {{1}};
    std::vector<int> costs9 = {4};
    assert(cheapestRoute(1, 1, routes9, costs9) == -1);

    // Multiple valid routes, choose minimum.
    std::vector<std::vector<int>> routes10 = {{1, 3, 4}, {1, 9, 4}, {1, 4}};
    std::vector<int> costs10 = {5, 7, 6};
    assert(cheapestRoute(1, 4, routes10, costs10) == 5);
}
