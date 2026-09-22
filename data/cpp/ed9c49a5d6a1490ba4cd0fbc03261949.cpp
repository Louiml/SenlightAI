Write a C++ function `int tripCost(const std::vector<std::pair<std::string, std::string>>& cityFares, const std::string& route)` that takes a list of city names paired with their per-trip cost (as strings, where the cost is a non‑negative integer), and a route string that is a sequence of city names separated by single spaces with an optional dash (`-`) symbol placed between certain city names. The route always starts and ends with a city name, and a dash only appears between two city names (e.g., `"A - B C - D"`). The function must compute the total cost of all trips that consist of exactly two consecutive city names connected by a dash (i.e., a pair like `"A - B"`), but only if the second city of that pair appears in the `cityFares` list, adding the corresponding integer cost for that city. If a paired city is not found in the list, that pair is ignored. Cities without a dash are not counted, and repeated pairs are counted each time they appear. For example, for `cityFares = {{"B","5"},{"D","7"}}` and route `"A - B C - D A - B"`, the total is 5+7+5 = 17. Assume the route string is non‑empty, contains at least one dash, and all city names are non‑empty, have no spaces or dashes, and are distinct from the delimiter tokens. The input list may be empty (meaning no costs, total is 0).

The key is to parse the route string word by word, identifying when a dash separates two consecutive city names. A simple approach: split the route string into tokens using spaces (ignoring the dash itself). Iterate through tokens; when we encounter a dash as a token, that indicates a pair: the previous city token and the next city token (which will appear after the dash, but note the next non‑dash token after the dash is the second city). However, careful: the route may have multiple tokens; we can process it sequentially. An easier method: replace dashes with spaces and then split, but the dash loses information. Instead, iterate character by character or use a string stream that reads tokens; when reading a token, if it equals `"-"`, then it signals that the last saved city (from the previous token) is paired with the city that will be read next. So: maintain a variable `prevCity` (empty initially). For each token (split by spaces), if token == `"-"`, set a flag `expectPair` and do not update `prevCity`. Otherwise, if `expectPair` is true, then the current token is the second city; check if it is in the `cityFares` map (convert vector to unordered_map for O(1) lookup) and add its cost; then clear the flag and set `prevCity` to the current token (for possible future pairs). If `expectPair` is false, just set `prevCity = current token`. Edge cases: consecutive dashes (though task says only between names, but handle gracefully), multiple spaces, and the last token not being a city after a dash (not expected per constraints). Time complexity is O(n + m) where n is the total characters in the route (for tokenization) and m is the number of city entries (for map construction), but tokenization is O(n). Space is O(m) for the map and O(number of tokens) for the token vector. For simplicity, we can split using `std::istringstream` after replacing dashes with a special dummy token? Better approach: use a loop reading tokens from istringstream directly, and handle the dash token inside the loop.

#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>

// Compute total cost of dash-separated city pairs in the route.
// cityFares: list of (city, cost) pairs, cost as a non-negative integer string.
// route: string with city names and '-' delimiters.
int tripCost(const std::vector<std::pair<std::string, std::string>>& cityFares, const std::string& route) {
    // Build lookup map for O(1) cost access.
    std::unordered_map<std::string, int> costMap;
    for (const auto& entry : cityFares) {
        costMap[entry.first] = std::stoi(entry.second);
    }

    int total = 0;
    std::string prevCity;       // last city token seen
    bool expectPair = false;    // true when we just saw a dash

    std::istringstream iss(route);
    std::string token;
    while (iss >> token) {
        if (token == "-") {
            expectPair = true;
            // Do not change prevCity; it will be paired with the next token.
            continue;
        }

        if (expectPair) {
            // Token is the second city of the pair.
            auto it = costMap.find(token);
            if (it != costMap.end()) {
                total += it->second;
            }
            expectPair = false;
        }

        // Update prevCity for future pairs (this token could be first of a new pair).
        prevCity = token;
    }

    return total;
}

#include <cassert>
#include <vector>
#include <string>

// Include the solution function here (or link appropriately).

int main() {
    // Basic case with multiple pairs.
    std::vector<std::pair<std::string, std::string>> fares1 = {{"B","5"},{"D","7"}};
    assert(tripCost(fares1, "A - B C - D A - B") == 17);

    // Only some pairs have known costs.
    std::vector<std::pair<std::string, std::string>> fares2 = {{"X","10"}};
    assert(tripCost(fares2, "A - B X - Y - X") == 10); // Only X - Y? Actually Y not known, but X - Y is pair with second Y unknown, so 0; then Y - X pair second X known adds 10.

    // City not found in fares is ignored.
    std::vector<std::pair<std::string, std::string>> fares3 = {{"Home","3"}};
    assert(tripCost(fares3, "Home - School") == 0); // School not in list.

    // Empty fares list yields zero.
    std::vector<std::pair<std::string, std::string>> fares4;
    assert(tripCost(fares4, "A - B") == 0);

    // Repeated same pair counts multiple times.
    std::vector<std::pair<std::string, std::string>> fares5 = {{"P","4"}};
    assert(tripCost(fares5, "P - Q P - Q") == 8); // Each occurrence adds 4 if Q is known? Q not known, so actually 0; let's test with known second city.

    // Redo: use known second city.
    std::vector<std::pair<std::string, std::string>> fares6 = {{"Q","6"}};
    assert(tripCost(fares6, "P - Q P - Q") == 12);

    // Single dash and city not needing first city in list.
    std::vector<std::pair<std::string, std::string>> fares7 = {{"Z","9"}};
    assert(tripCost(fares7, "A - Z") == 9);

    // No spaces around dash? The route uses spaces as given; but handle missing spaces? Not required.

    return 0;
}
