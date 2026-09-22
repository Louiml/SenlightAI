// The provided code implements a wine recommendation system using an adjacency list where each wine node points to the previously inserted wine, and search functions match exact characteristics or suggest which fields to adjust. Your task is to create a standalone, self-contained C++ function that solves a simplified version of this problem: Given a vector of `Wine` objects (each with `name`, `province`, `country`, `region`, `winery`, `variety`, `price`, `points`, `description`), write a function `findMatchingWines` that takes the vector and a target set of characteristics (price as `double`, points as `double`, winery, variety, region, province, country as strings) and returns a vector of `Wine` objects that exactly match all provided criteria (an empty string or a negative price/points means “don’t care” for that field). If no wine matches, the function should return an empty vector. The function must be `const`-correct, not modify the input vector, and handle edge cases such as empty input vector, partial criteria (only some fields specified), and exact floating-point comparison using `==` (since inputs are derived from stored data, no tolerance is needed). The solution should be standalone without any external dependencies beyond standard headers, and must include the `Wine` struct definition.

// The solution iterates through the vector of wines once. For each wine, we check if it matches all non-negative constraints: for numeric fields (price, points), we only compare if the target value is non-negative (since negative values indicate “don’t care”); for string fields, we only compare if the target string is non-empty. If all specified fields match, we add the wine to the result vector. Edge cases include: an empty input vector (return empty), an empty target (all fields “don’t care” → return all wines), and exact equality for doubles (using `==` is safe because inputs come from the stored data). Time complexity is O(n * m), where n is the number of wines and m is the number of fields checked (constant, so effectively O(n)). Space complexity is O(k) for the result vector, where k is the number of matching wines. The function is `const` and takes the vector by `const reference` to avoid copying.

#include <vector>
#include <string>

struct Wine {
    std::string name;
    std::string province;
    std::string country;
    std::string region;
    std::string winery;
    std::string variety;
    double price;
    double points;
    std::string description;
};

// Returns all wines that exactly match the specified criteria.
// Empty string fields and negative numeric values are treated as "don't care".
std::vector<Wine> findMatchingWines(const std::vector<Wine>& wines,
                                    double price, double points,
                                    const std::string& winery,
                                    const std::string& variety,
                                    const std::string& region,
                                    const std::string& province,
                                    const std::string& country) {
    std::vector<Wine> result;
    for (const Wine& w : wines) {
        bool matches = true;
        if (price >= 0 && w.price != price) matches = false;
        if (points >= 0 && w.points != points) matches = false;
        if (!winery.empty() && w.winery != winery) matches = false;
        if (!variety.empty() && w.variety != variety) matches = false;
        if (!region.empty() && w.region != region) matches = false;
        if (!province.empty() && w.province != province) matches = false;
        if (!country.empty() && w.country != country) matches = false;
        if (matches) result.push_back(w);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    Wine w1{"A", "CA", "US", "Napa", "Winery1", "Cabernet", 50.0, 90.0, "Rich"};
    Wine w2{"B", "OR", "US", "Willamette", "Winery2", "Pinot", 40.0, 88.0, "Light"};
    Wine w3{"C", "CA", "US", "Napa", "Winery1", "Chardonnay", 30.0, 85.0, "Crisp"};
    std::vector<Wine> all = {w1, w2, w3};

    // Match all fields exactly
    auto res1 = findMatchingWines(all, 50.0, 90.0, "Winery1", "Cabernet", "Napa", "CA", "US");
    assert(res1.size() == 1 && res1[0].name == "A");

    // Only match by country
    auto res2 = findMatchingWines(all, -1, -1, "", "", "", "", "US");
    assert(res2.size() == 3);

    // Match by price and country, other fields don't care
    auto res3 = findMatchingWines(all, 30.0, -1, "", "", "", "", "US");
    assert(res3.size() == 1 && res3[0].name == "C");

    // No match
    auto res4 = findMatchingWines(all, 100.0, -1, "", "", "", "", "");
    assert(res4.empty());

    // Empty input vector
    std::vector<Wine> none;
    auto res5 = findMatchingWines(none, -1, -1, "", "", "", "", "");
    assert(res5.empty());

    // All criteria "don't care" returns all
    auto res6 = findMatchingWines(all, -1, -1, "", "", "", "", "");
    assert(res6.size() == 3);

    // Match only winery
    auto res7 = findMatchingWines(all, -1, -1, "Winery1", "", "", "", "");
    assert(res7.size() == 2 && res7[0].name == "A" && res7[1].name == "C");

    return 0;
}
