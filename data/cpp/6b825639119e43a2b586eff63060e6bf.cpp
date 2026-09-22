Write a C++ function that takes a list of participant entries, where each entry consists of a name (a non-empty string without spaces) and a category string (one of `"rat"`, `"woman"`, `"child"`, `"man"`, or `"captain"`). The function must output (or return) the names sorted by the following priority rules: `"rat"` entries come first (in their original input order), then `"woman"` and `"child"` entries together (in their original input order relative to each other, regardless of whether the label is "woman" or "child"), then `"man"` entries (in original order), and finally `"captain"` entries (in original order). Within each of the five groups, the relative order of entries must match their input order. The function should accept a vector of pairs (name, category) and return a vector of names in the sorted order.

// The problem is a stable grouping by priority category. The original snippet uses a `map` to assign a numeric priority to each category, then stores each entry as a pair `(priority, input_index)` along with the name, and finally sorts by `(priority, input_index)` using the default `std::sort` on pairs, which ensures stability by input order within each priority group. The same approach can be implemented more cleanly using a custom comparator or by using `std::stable_sort` with a comparator that only compares category priority (since `stable_sort` preserves original order for equal elements). The priority mapping is: `"rat"` → 0, `"woman"` → 1, `"child"` → 1, `"man"` → 2, `"captain"` → 3. Edge cases: duplicate names are allowed and must remain in their input order; the input could be empty; category strings are guaranteed valid per specification; names may be any non-space string. Time complexity is O(N log N) due to sorting, and space complexity is O(N) for the output vector and internal copies.

#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

// Return names sorted by category priority, preserving input order within each group.
std::vector<std::string> sortByCategory(const std::vector<std::pair<std::string, std::string>>& entries) {
    static const std::unordered_map<std::string, int> priority = {
        {"rat", 0},
        {"woman", 1},
        {"child", 1},
        {"man", 2},
        {"captain", 3}
    };
    
    std::vector<std::string> result;
    result.reserve(entries.size());
    
    // Create a vector of (priority, original_index, name) for stable sorting
    std::vector<std::tuple<int, int, std::string>> indexed;
    indexed.reserve(entries.size());
    for (std::size_t i = 0; i < entries.size(); ++i) {
        indexed.emplace_back(priority.at(entries[i].second), static_cast<int>(i), entries[i].first);
    }
    
    std::sort(indexed.begin(), indexed.end());
    
    for (const auto& item : indexed) {
        result.push_back(std::get<2>(item));
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

int main() {
    std::vector<std::pair<std::string, std::string>> entries1 = {
        {"Alice", "woman"},
        {"Bob", "man"},
        {"Charlie", "rat"},
        {"Diana", "captain"},
        {"Eve", "child"}
    };
    std::vector<std::string> expected1 = {"Charlie", "Alice", "Eve", "Bob", "Diana"};
    assert(sortByCategory(entries1) == expected1);

    std::vector<std::pair<std::string, std::string>> entries2 = {
        {"A", "rat"},
        {"B", "rat"},
        {"C", "woman"},
        {"D", "man"},
        {"E", "captain"},
        {"F", "child"},
        {"G", "woman"}
    };
    // rats first (A, B), then women/children in original order (C, F, G), then man (D), captain (E)
    std::vector<std::string> expected2 = {"A", "B", "C", "F", "G", "D", "E"};
    assert(sortByCategory(entries2) == expected2);

    std::vector<std::pair<std::string, std::string>> entries3; // empty input
    assert(sortByCategory(entries3).empty());

    std::vector<std::pair<std::string, std::string>> entries4 = {
        {"Z", "captain"},
        {"Y", "captain"},
        {"X", "man"},
        {"W", "child"}
    };
    std::vector<std::string> expected4 = {"W", "X", "Z", "Y"}; // child first, then man, then captains in input order
    assert(sortByCategory(entries4) == expected4);

    // Test stability when same category and same priority
    std::vector<std::pair<std::string, std::string>> entries5 = {
        {"1", "woman"},
        {"2", "child"},
        {"3", "woman"},
        {"4", "child"}
    };
    std::vector<std::string> expected5 = {"1", "2", "3", "4"};
    assert(sortByCategory(entries5) == expected5);
    return 0;
}
