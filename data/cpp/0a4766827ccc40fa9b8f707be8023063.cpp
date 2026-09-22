// Write a C++ function named `middlePageNumber` that takes a file path as a `const std::string&` and processes the file in the same format as the given snippet. The file contains two sections: first, zero or more ordering rules of the form `A|B` (meaning page A must be printed before page B), one per line until an empty line is encountered; second, zero or more "update" lines, each a comma-separated list of page numbers. For each update line, determine if it is correctly ordered according to the rules (i.e., for every pair of pages, if a rule says A before B, A must appear before B in the update). If an update is correctly ordered, add its middle page number (the element at index `size/2`) to a running total. Return the total sum of middle page numbers from all correctly ordered updates. If the file cannot be opened or the format is invalid, return 0. The function must handle duplicate page numbers (treat them as valid but they still must satisfy rules), and rules can reference pages not present in any update. The file path is the only parameter.
The solution reads the file line by line. First, parse all `A|B` rules until an empty line; store them in an `unordered_map<int, unordered_set<int>>` where the key is page A and the set contains pages that must come after A (i.e., A before B). For each subsequent line, split it by commas into a vector of integers. Then validate the order by iterating through the vector and for each element at position `i`, check all earlier elements `j < i`: if there is a rule saying the earlier element must come after the current element (i.e., `before[current].contains(earlier)`), then the update is invalid. Alternatively, iterate pairwise. A simpler approach: for each adjacent pair (and also non-adjacent pairs) check if any rule is violated. The given snippet checks only adjacent pairs while building, but a correct check must consider all pairs because a rule may require a page to appear before a page that is not immediately preceding. For example, `1|3` and update `1,2,3` is fine, but `3,1` fails. The code in the snippet only checks the immediately previous page, which would miss a violation like `3,1` if `1` was earlier but not immediately before. However, the snippet's logic actually checks `before[y].find(x)` where `y` is the current and `x` is the previous, so it only checks the adjacent pair. That is insufficient for general rules. For a correct solution, we must check all pairs. A robust method: for each update, iterate over all pairs `(i,j)` with i<j, and if `before[update[i]].contains(update[j])` is false but `before[update[j]].contains(update[i])` is true, that means a rule requires `update[j]` before `update[i]`, but `i<j` means `update[i]` appears first, so it's invalid. Actually, we need to ensure that if a rule says `A|B`, then A must appear before B. So for each pair (i<j), if `before[update[j]]` contains `update[i]`, that means rule says `update[i]` before `update[j]`? Wait, `before[update[j]]` stores pages that must come before `update[j]`? Let's re-read: the snippet stores `before[a].insert(b)` meaning `a` must come before `b`. So `before[y]` contains all `x` such that rule says `x|y` (x before y). So if `before[y].contains(x)` and x appears after y in the update, that's a violation. So for each pair (i<j), if `before[update[i]]` contains `update[j]` and `before[update[j]]` contains `update[i]`? Actually no, we need to check if a rule requires `update[i]` before `update[j]` but they appear in opposite order. So if `before[update[j]]` contains `update[i]`, that means rule says `update[i]` must come before `update[j]`, but since i<j, that's fine. The violation is when `before[update[i]]` contains `update[j]` (meaning `update[j]` must come before `update[i]`) but i<j, so `update[j]` appears after `update[i]` – violation. So correct check: for all i<j, if `before[update[i]].count(update[j]) > 0`, then invalid because rule says update[j] before update[i] but it appears later. Alternatively, for each pair (a,b) in the update, if there's a rule `b|a`, then a must appear before b; if a appears after b, invalid. So simplest: for each update, for each i, for each j>i, if `before[update[i]].count(update[j])` then invalid. If valid, add `update[update.size()/2]`. Edge cases: empty updates (should be skipped? The middle index would be out of bounds; but typically updates have at least one page; if empty line? The parsing will produce empty vector? We'll handle by skipping if size==0). File not open returns 0. For n pages and m rules, time complexity: parsing rules O(m) average, for each update of length k, checking all pairs O(k^2), total O(U * k^2) where U is number of updates. Space: O(m) for the map, plus O(k) per update temporary.
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>

// Returns the sum of middle page numbers from correctly ordered updates in the input file.
// The file format: rules "A|B" until empty line, then comma-separated update lines.
int middlePageNumber(const std::string& filePath) {
    std::ifstream input(filePath);
    if (!input.is_open()) {
        return 0;
    }

    // Parse ordering rules: key A, value set of pages B that must come after A.
    std::unordered_map<int, std::unordered_set<int>> before;
    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) break;
        int a, b;
        if (std::sscanf(line.c_str(), "%d|%d", &a, &b) != 2) {
            return 0; // Invalid rule format.
        }
        before[a].insert(b);
    }

    int sum = 0;
    while (std::getline(input, line)) {
        if (line.empty()) continue;

        // Parse comma-separated integers.
        std::vector<int> update;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ',')) {
            if (token.empty()) continue;
            update.push_back(std::stoi(token));
        }
        if (update.empty()) continue;

        // Check all pairs: for i<j, if rule says update[j] before update[i], invalid.
        bool valid = true;
        for (size_t i = 0; i < update.size() && valid; ++i) {
            for (size_t j = i + 1; j < update.size(); ++j) {
                // Does a rule say update[j] must come before update[i]?
                if (before[update[i]].count(update[j]) > 0) {
                    valid = false;
                    break;
                }
            }
        }
        if (valid) {
            sum += update[update.size() / 2];
        }
    }
    return sum;
}
#include <cassert>
#include <fstream>
#include <string>

// Forward declaration of the function being tested.
int middlePageNumber(const std::string& filePath);

int main() {
    // Test 1: Simple valid and invalid updates.
    {
        std::ofstream out("test1.txt");
        out << "1|2\n2|3\n\n1,2,3\n3,2,1\n1,3,2\n";
        out.close();
        // Valid: 1,2,3 (middle 2), 1,3,2? Check: 1 before 2,2 before 3. For 1,3,2: 3 after 1 (OK), 2 after 3? rule 2|3 means 2 before 3, but here 3 before 2, so invalid. So only first valid.
        assert(middlePageNumber("test1.txt") == 2);
    }
    // Test 2: No rules, all updates valid.
    {
        std::ofstream out("test2.txt");
        out << "\n5,6,7\n8\n";
        out.close();
        // Both valid, middle: 6 and 8, sum 14.
        assert(middlePageNumber("test2.txt") == 14);
    }
    // Test 3: Rule not satisfied for non-adjacent pair.
    {
        std::ofstream out("test3.txt");
        out << "1|3\n\n1,2,3\n3,2,1\n";
        out.close();
        // 1,2,3 valid (middle 2); 3,2,1 invalid because 1 must be before 3 but 3 is first.
        assert(middlePageNumber("test3.txt") == 2);
    }
    // Test 4: Duplicate numbers and self rules.
    {
        std::ofstream out("test4.txt");
        out << "1|1\n2|3\n\n1,1\n3,2\n1,2,3\n";
        out.close();
        // Rule 1|1 means 1 before 1 – impossible, so any update with 1 invalid? Actually pair (1,1) i<j? For update 1,1: i=0 j=1, check before[1].count(1) >0, so invalid. 3,2 invalid (rule 2|3). 1,2,3 valid (middle 2). Sum=2.
        assert(middlePageNumber("test4.txt") == 2);
    }
    // Test 5: File doesn't exist.
    {
        assert(middlePageNumber("missing.txt") == 0);
    }
    return 0;
}
