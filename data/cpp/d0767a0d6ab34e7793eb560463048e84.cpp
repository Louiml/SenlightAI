// Write a standalone C++ function `compute_first_sets` that, given a vector of productions (where each production pairs a nonterminal string on the left with a right-hand side vector of symbols), returns a `std::map<std::string, std::set<std::string>>` mapping each nonterminal symbol to its FIRST set. A symbol is a terminal if it is a lowercase letter, a digit, or any symbol that is not an uppercase letter. The special terminal string `"eta"` represents epsilon (empty string). The FIRST set of a nonterminal includes all terminals that can appear at the beginning of any string derivable from that nonterminal, plus `"eta"` if the nonterminal can derive the empty string. The grammar may be left-recursive, and the function must handle cycles correctly by iterating until no changes occur. Assume the input is non-empty and contains valid productions with well-formed left-hand sides. Return the FIRST sets for every nonterminal appearing on the left-hand side of any production.

The algorithm follows the classic fixed-point iteration for computing FIRST sets. First, we build a map from each left-hand side (LHS) to a vector of its right-hand side (RHS) symbol sequences. Then we initialize the result set for each nonterminal to empty. For each production `A → X1 X2 ... Xk`, we compute the FIRST of the RHS by scanning symbols from left to right: if the symbol is a terminal (and not `"eta"`), we add it to the result and stop; if it is a nonterminal, we add all elements from its current FIRST set except `"eta"`, and if `"eta"` is not present in that nonterminal's FIRST set, we stop; if `"eta"` is present, we continue to the next symbol. If we reach the end of the RHS without stopping, we add `"eta"`. We repeat this process for all productions until no set grows in an iteration. Edge cases include productions that are already terminal, empty RHS (which immediate adds `"eta"`), self-referential nonterminals, and indirect cycles where fixed point may need multiple passes. The function must not modify its input. Time complexity is O(number of productions × maximum RHS length × number of nonterminals × iterations), where iterations are bounded by the size of the terminal alphabet times the number of nonterminals (in practice small). Space complexity is O(total number of symbols across all productions plus size of all FIRST sets).

#include <map>
#include <set>
#include <vector>
#include <string>
#include <cctype>

struct Production {
    std::string left;
    std::vector<std::string> right;
};

// Helper: check if a symbol is a terminal (not an uppercase letter and not epsilon)
bool isTerminalSymbol(const std::string& symbol) {
    return !symbol.empty() && !std::isupper(static_cast<unsigned char>(symbol[0])) && symbol != "eta";
}

// Helper: compute FIRST of a sequence of symbols given current FIRST sets
std::set<std::string> firstOfSequence(const std::vector<std::string>& symbols,
                                      const std::map<std::string, std::set<std::string>>& firstSets) {
    std::set<std::string> result;
    for (const auto& sym : symbols) {
        if (isTerminalSymbol(sym)) {
            result.insert(sym);
            return result;
        }
        // sym is a nonterminal; must exist in firstSets
        const auto& symFirst = firstSets.at(sym);
        for (const auto& f : symFirst) {
            if (f != "eta") {
                result.insert(f);
            }
        }
        if (symFirst.count("eta") == 0) {
            return result;
        }
    }
    // All symbols can derive eta
    result.insert("eta");
    return result;
}

// Compute FIRST sets for all nonterminals in the grammar
std::map<std::string, std::set<std::string>> compute_first_sets(const std::vector<Production>& productions) {
    std::map<std::string, std::vector<std::vector<std::string>>> grammar;
    std::map<std::string, std::set<std::string>> firstSets;

    // Build grammar map and initialize all nonterminals
    for (const auto& prod : productions) {
        grammar[prod.left].push_back(prod.right);
        firstSets[prod.left]; // Ensure the key exists
    }

    bool updated;
    do {
        updated = false;
        for (const auto& [lhs, rhsList] : grammar) {
            for (const auto& rhs : rhsList) {
                auto seqFirst = firstOfSequence(rhs, firstSets);
                size_t before = firstSets[lhs].size();
                firstSets[lhs].insert(seqFirst.begin(), seqFirst.end());
                if (firstSets[lhs].size() > before) {
                    updated = true;
                }
            }
        }
    } while (updated);

    return firstSets;
}

#include <cassert>
#include <vector>
#include <string>
#include <set>
#include <map>

int main() {
    // Test 1: Simple grammar without epsilon
    std::vector<Production> prods1 = {
        {"S", {"A", "b"}},
        {"A", {"a"}},
        {"A", {"c"}}
    };
    auto first1 = compute_first_sets(prods1);
    assert(first1["S"] == std::set<std::string>({"a", "c"}));
    assert(first1["A"] == std::set<std::string>({"a", "c"}));

    // Test 2: Grammar with epsilon in one nonterminal
    std::vector<Production> prods2 = {
        {"S", {"A", "b"}},
        {"A", {"eta"}},
        {"A", {"a"}}
    };
    auto first2 = compute_first_sets(prods2);
    assert(first2["A"] == std::set<std::string>({"eta", "a"}));
    assert(first2["S"] == std::set<std::string>({"a", "b"})); // b comes from A→eta then b

    // Test 3: Left recursion
    std::vector<Production> prods3 = {
        {"E", {"E", "+", "T"}},
        {"E", {"T"}},
        {"T", {"x"}}
    };
    auto first3 = compute_first_sets(prods3);
    assert(first3["E"] == std::set<std::string>({"x"}));
    assert(first3["T"] == std::set<std::string>({"x"}));

    // Test 4: Indirect cycle with epsilon
    std::vector<Production> prods4 = {
        {"A", {"B"}},
        {"B", {"C"}},
        {"C", {"eta"}},
        {"C", {"d"}}
    };
    auto first4 = compute_first_sets(prods4);
    assert(first4["A"] == std::set<std::string>({"eta", "d"}));
    assert(first4["B"] == std::set<std::string>({"eta", "d"}));
    assert(first4["C"] == std::set<std::string>({"eta", "d"}));

    // Test 5: Terminal symbols that are multi-character
    std::vector<Production> prods5 = {
        {"S", {"if", "E"}},
        {"E", {"x"}}
    };
    auto first5 = compute_first_sets(prods5);
    assert(first5["S"] == std::set<std::string>({"if"}));
    assert(first5["E"] == std::set<std::string>({"x"}));

    // Test 6: Production with multiple nonterminals; second cannot derive eta
    std::vector<Production> prods6 = {
        {"S", {"A", "B"}},
        {"A", {"a"}},
        {"A", {"eta"}},
        {"B", {"b"}}
    };
    auto first6 = compute_first_sets(prods6);
    assert(first6["S"] == std::set<std::string>({"a", "b"})); // A→eta then B gives b

    // Test 7: Empty RHS production (epsilon production)
    std::vector<Production> prods7 = {
        {"S", {"eta"}},
        {"S", {"a", "S"}}
    };
    auto first7 = compute_first_sets(prods7);
    assert(first7["S"] == std::set<std::string>({"eta", "a"}));

    return 0;
}
