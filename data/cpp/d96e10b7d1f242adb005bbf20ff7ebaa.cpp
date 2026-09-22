// Write a C++ function that takes two vectors of strings: `articles` (each string formatted as a book code followed by a space and an integer quantity, e.g., `"ABC 20"`) and `categories` (each string is a single uppercase letter representing a category code, e.g., `"A"`, `"B"`). The function must return a string summarizing the total quantity of books for each category listed in the `categories` vector, in the same order as they appear. The output format must be: `"(X : total)"` for each category, separated by `" - "` (e.g., `"(A : 20) - (B : 0)"`). If a category has no books in `articles`, its total is `0`. If `articles` is empty, return an empty string. The input strings in `articles` are guaranteed to have exactly one space separating the code and quantity, and the quantity is a valid non-negative integer. Categories are guaranteed to be non-empty, and each category string is a single uppercase letter. You may assume no duplicate category codes appear in `categories`, but `articles` may contain multiple entries for the same category code—quantities for the same code must be summed.

#include <cassert>
#include <string>
#include <vector>

int main() {
    // Basic example with two categories, one missing.
    std::vector<std::string> art1 = {"ABAR 200", "CDXE 500", "BKWR 250", "BTSQ 890", "DRTY 600"};
    std::vector<std::string> cat1 = {"A", "B"};
    assert(stockSummary(art1, cat1) == "(A : 200) - (B : 1140)");

    // Empty articles returns empty string.
    std::vector<std::string> art2;
    std::vector<std::string> cat2 = {"A", "B"};
    assert(stockSummary(art2, cat2) == "");

    // All categories absent, all totals zero.
    std::vector<std::string> art3 = {"X 10", "Y 20"};
    std::vector<std::string> cat3 = {"A", "B", "C"};
    assert(stockSummary(art3, cat3) == "(A : 0) - (B : 0) - (C : 0)");

    // Single category with multiple entries.
    std::vector<std::string> art4 = {"A 5", "A 10", "B 7"};
    std::vector<std::string> cat4 = {"A", "B"};
    assert(stockSummary(art4, cat4) == "(A : 15) - (B : 7)");

    // Only one category in list.
    std::vector<std::string> art5 = {"Z 100"};
    std::vector<std::string> cat5 = {"Z"};
    assert(stockSummary(art5, cat5) == "(Z : 100)");

    // Large quantities.
    std::vector<std::string> art6 = {"A 123456", "B 789012"};
    std::vector<std::string> cat6 = {"B", "A"};
    assert(stockSummary(art6, cat6) == "(B : 789012) - (A : 123456)");

    // Articles with same code but different spacing? (assume exactly one space)
    std::vector<std::string> art7 = {"A 1", "A 2", "A 3"};
    std::vector<std::string> cat7 = {"A"};
    assert(stockSummary(art7, cat7) == "(A : 6)");

    return 0;
}

#include <string>
#include <vector>
#include <unordered_map>
#include <cctype>

// Returns a summary string of total quantities per category.
std::string stockSummary(const std::vector<std::string>& articles,
                         const std::vector<std::string>& categories) {
    if (articles.empty()) {
        return "";
    }

    // Accumulate total quantity per category code (first character).
    std::unordered_map<char, int> totals;
    for (const auto& article : articles) {
        char code = article[0];
        // Find space and parse the number after it.
        size_t spacePos = article.find(' ');
        int quantity = std::stoi(article.substr(spacePos + 1));
        totals[code] += quantity;
    }

    // Build result string in the order of categories.
    std::string result;
    for (size_t i = 0; i < categories.size(); ++i) {
        char code = categories[i][0];
        int total = 0;
        auto it = totals.find(code);
        if (it != totals.end()) {
            total = it->second;
        }
        // Append formatted entry with separator.
        result += "(" + categories[i] + " : " + std::to_string(total) + ")";
        if (i != categories.size() - 1) {
            result += " - ";
        }
    }
    return result;
}

// The solution processes the `articles` list once to accumulate total quantities per category code. Use an `std::unordered_map<char, int>` to store the running sum for each distinct first character (which represents the category code). For each article string, extract the first character (index 0) and the integer quantity after the space (use `std::stoi` on the substring after the first space). Then increment the map entry for that character. After building the map, iterate through the `categories` vector in order. For each category code, look up the value in the map; if absent, treat as 0, otherwise use the accumulated value. Build the output string by appending `"(X : total)"` and adding `" - "` between entries (but not after the last one). If `articles` is empty, return `""` immediately. Complexity: building the map is O(A·k) where A is number of articles and k is the average length of the strings (since we use `substr` and `stoi`); iterating over categories is O(C) where C is the number of categories. Space complexity is O(U) where U is the number of unique category codes in articles, plus the output string size.
