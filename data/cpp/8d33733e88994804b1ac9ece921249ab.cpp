/*
Write a C++ function `suggestedProducts` that, given a vector of product names (lowercase alphabetic strings only) and a search word (also lowercase alphabetic), returns a vector of vectors of strings. For each prefix length from 1 to the length of the search word, the result must contain up to the first three product names (in lexicographical order) that have that prefix. If fewer than three products have the prefix, return all of them; if none, return an empty vector for that prefix. Assume the input products vector may contain duplicates, and duplicates should be counted separately. The function must handle an empty search word by returning an empty vector of vectors.
*/
#include <vector>
#include <string>
#include <algorithm>

// Trie node storing up to three product indices for each prefix.
class TrieNode {
public:
    std::vector<TrieNode*> children;
    std::vector<int> indices;
    TrieNode() : children(26, nullptr) {}
};

// Return suggested products for each prefix of searchWord.
std::vector<std::vector<std::string>> suggestedProducts(
    std::vector<std::string>& products, const std::string& searchWord) {
    std::sort(products.begin(), products.end());
    TrieNode* root = new TrieNode();
    // Insert each product with its sorted index.
    for (int i = 0; i < static_cast<int>(products.size()); ++i) {
        TrieNode* node = root;
        const std::string& w = products[i];
        for (char c : w) {
            int idx = c - 'a';
            if (!node->children[idx]) {
                node->children[idx] = new TrieNode();
            }
            node = node->children[idx];
            if (node->indices.size() < 3) {
                node->indices.push_back(i);
            }
        }
    }
    // Search for prefixes of searchWord.
    std::vector<std::vector<std::string>> result;
    TrieNode* node = root;
    bool match = true;
    for (char c : searchWord) {
        if (!match || !node->children[c - 'a']) {
            match = false;
            result.emplace_back();
            continue;
        }
        node = node->children[c - 'a'];
        std::vector<std::string> suggestions;
        for (int idx : node->indices) {
            suggestions.push_back(products[idx]);
        }
        result.push_back(std::move(suggestions));
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// Function declaration from the solution.
std::vector<std::vector<std::string>> suggestedProducts(std::vector<std::string>& products, const std::string& searchWord);

int main() {
    // Basic case with exactly three suggestions.
    std::vector<std::string> p1 = {"mobile","mouse","moneypot","monitor","mousepad"};
    auto r1 = suggestedProducts(p1, "mouse");
    assert(r1.size() == 5);
    assert(r1[0] == std::vector<std::string>({"mobile","moneypot","monitor"}));
    assert(r1[1] == std::vector<std::string>({"mobile","moneypot","monitor"}));
    assert(r1[2] == std::vector<std::string>({"mouse","mousepad"}));
    assert(r1[3] == std::vector<std::string>({"mouse","mousepad"}));
    assert(r1[4] == std::vector<std::string>({"mouse","mousepad"}));

    // Products with no common prefix with search word.
    std::vector<std::string> p2 = {"abc","def"};
    auto r2 = suggestedProducts(p2, "xyz");
    assert(r2.size() == 3);
    for (const auto& v : r2) assert(v.empty());

    // Duplicate products counted separately.
    std::vector<std::string> p3 = {"a","a","b"};
    auto r3 = suggestedProducts(p3, "a");
    assert(r3.size() == 1);
    assert(r3[0] == std::vector<std::string>({"a","a"}));

    // Empty search word.
    std::vector<std::string> p4 = {"x"};
    auto r4 = suggestedProducts(p4, "");
    assert(r4.empty());

    // Single product multiple prefixes.
    std::vector<std::string> p5 = {"ab"};
    auto r5 = suggestedProducts(p5, "ab");
    assert(r5.size() == 2);
    assert(r5[0] == std::vector<std::string>({"ab"}));
    assert(r5[1] == std::vector<std::string>({"ab"}));

    // More than three products with same prefix.
    std::vector<std::string> p6 = {"apple","app","ap","a","apply","appetite"};
    auto r6 = suggestedProducts(p6, "app");
    assert(r6.size() == 3);
    assert(r6[0] == std::vector<std::string>({"a","ap","app"}));
    assert(r6[1] == std::vector<std::string>({"ap","app","apple"}));
    assert(r6[2] == std::vector<std::string>({"app","apple","appetite"}));

    return 0;
}
// The solution builds a trie over the sorted list of products. Sorting the products lexicographically ensures that as we insert each product in order, the indices stored at each trie node are naturally in lexicographical order for that prefix. For each product index `i`, we traverse its characters, creating nodes as needed, and at every visited node we append `i` to that node’s stored index list only if the list size is already less than 3 (so we keep at most the first three product indices for that prefix). Then, for the search word, we walk down the trie character by character. For each prefix length from 0 (we store results for prefixes of lengths 1..n) but note the code returns a vector of size equal to search word length; for each step we store the move of the node’s index list into the answer. If a child node is missing, we break and leave later positions empty. Edge cases: empty search word (return empty), products with no matching prefix (empty inner vectors), and duplicates (each duplicate is inserted separately, and the trie stores its own index). Time complexity: building the trie takes O(total characters in all products + number of products) because each product is inserted character by character; searching takes O(search word length). Space complexity: O(total characters in all products) for the trie nodes, plus O(number of products) for the index lists (but each product index appears at most at each node along its path, so worst-case O(total characters in all products) for index lists too, since each product length contributes to each node).
