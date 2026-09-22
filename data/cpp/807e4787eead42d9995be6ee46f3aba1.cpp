Write a C++ function `estimateTsQuerySelectivity` that, given a parsed boolean expression tree representing a tsquery (with leaf nodes being either exact lexeme matches or prefix matches, and internal nodes being AND, OR, or NOT operators) and optional statistics about the most common elements (MCELEM) in a tsvector column, estimates the selectivity (the fraction of rows likely to match) of that query. The function must take as input: a vector of `TextFreq` entries (each containing a lexeme string and its frequency, sorted lexicographically by length then byte order), a `minFreq` value (the lowest frequency among all MCELEM entries), a flag indicating whether statistics are available, and a pointer to the root of the query tree. For leaf nodes that are exact matches: if the lexeme is found in the MCELEM list via binary search, return its frequency; if not found, return `min(DEFAULT_TS_MATCH_SEL, minFreq / 2)`. For leaf nodes that are prefix matches (e.g., "word:*"): if statistics are available and there are at least 100 MCELEM entries, scan the entire list, compute the combined probability that any MCELEM matches the prefix (treating occurrences as independent events, combining with formula `p + q - p*q`), also compute the combined probability that any MCELEM appears at all, then estimate the selectivity as `matched + (1.0 - allmces) * (n_matched / length)`, and finally clamp this to be at least `min(DEFAULT_TS_MATCH_SEL, minFreq / 2)`. If statistics are insufficient or unavailable for prefix matches, return `DEFAULT_TS_MATCH_SEL * 4`. For internal nodes, combine child selectivities as: AND multiplies, OR sums then subtracts product, NOT subtracts from 1.0. The function must handle null/empty queries gracefully (return 0.0), and must use double precision for intermediate calculations, clamping every result to the range `[0.0, 1.0]`. Use the constant `DEFAULT_TS_MATCH_SEL = 0.005`. Ensure the solution is self-contained and does not rely on external PostgreSQL headers; define minimal structs for the query tree and text frequency.

The main algorithm is a recursive traversal of the query tree in pre-order. At each node, we compute the selectivity based on the operator type. The key challenges are: handling prefix matches correctly by scanning the entire MCELEM array (since a prefix can match multiple entries, binary search is not sufficient), while exact matches use binary search for efficiency. The prefix match estimate combines the actual observed frequencies of matching MCELEMs with an extrapolation to the non-MCELEM portion of the data, using the fraction of matching MCELEMs as a proxy. Edge cases include: empty query (return 0), no statistics available (use default for exact matches, and a larger default for prefix matches), insufficient MCELEM count (<100) for prefix statistics, and lexemes not present in MCELEM (use min of default and minFreq/2). The recursive function must guard against deep recursion by assuming the input tree is well-formed and acyclic. Time complexity: exact match leaf uses `O(log n)` binary search; prefix match leaf uses `O(n)` scan; internal nodes combine children. Overall, for a query with `m` nodes and `n` MCELEM entries, worst-case `O(m * n)` when many prefix leaves exist, otherwise `O(m log n)`. Space complexity is `O(depth)` for recursion stack, plus `O(n)` for the input structure.

#include <algorithm>
#include <cmath>
#include <cstring>
#include <string>
#include <vector>

// Default selectivity when no statistics are available or for fallback cases
constexpr double DEFAULT_TS_MATCH_SEL = 0.005;

// Represents a single most common element (MCELEM) with its frequency
struct TextFreq {
    std::string element;  // The lexeme
    double frequency;     // Its occurrence frequency in the column
};

// Query tree node types
enum class QueryNodeType {
    VAL,   // Leaf: exact or prefix lexeme match
    AND,   // Logical AND
    OR,    // Logical OR
    NOT    // Logical NOT
};

// A leaf node representing a lexeme match
struct QueryOperand {
    std::string lexeme;      // The lexeme string (without the prefix marker)
    bool isPrefix;           // true if this is a prefix match (e.g., "word:*")
};

// General query tree node
struct QueryNode {
    QueryNodeType type;
    // For VAL nodes:
    QueryOperand* operand = nullptr;
    // For operator nodes:
    QueryNode* left = nullptr;
    QueryNode* right = nullptr;
};

// Forward declaration for recursion
double tsquery_opr_selec(const QueryNode* node, const std::vector<TextFreq>& lookup,
                         int length, double minFreq, bool hasStats);

// Binary search comparator matching the PostgreSQL ordering: by length, then byte order
static bool compareLexemeToTextFreq(const std::string& lexeme, const TextFreq& tf) {
    if (lexeme.size() != tf.element.size()) {
        return lexeme.size() < tf.element.size();
    }
    return lexeme < tf.element;
}

// Estimate selectivity for a single leaf node (VAL)
static double tsquery_val_selec(const QueryOperand* oper, const std::vector<TextFreq>& lookup,
                                int length, double minFreq, bool hasStats) {
    if (!oper->isPrefix) {
        // Exact match
        if (!hasStats || lookup.empty()) {
            return DEFAULT_TS_MATCH_SEL;
        }
        // Binary search for the exact lexeme
        auto it = std::lower_bound(lookup.begin(), lookup.end(), oper->lexeme, compareLexemeToTextFreq);
        if (it != lookup.end() && it->element == oper->lexeme) {
            return it->frequency;
        }
        // Not found in MCELEM
        return std::min(DEFAULT_TS_MATCH_SEL, minFreq / 2.0);
    } else {
        // Prefix match (lexeme:*)
        if (!hasStats || lookup.empty() || length < 100) {
            return DEFAULT_TS_MATCH_SEL * 4;
        }
        double matched = 0.0;
        double allmces = 0.0;
        int n_matched = 0;
        for (const auto& tf : lookup) {
            bool matchesPrefix = false;
            if (tf.element.size() >= oper->lexeme.size()) {
                matchesPrefix = std::strncmp(oper->lexeme.c_str(), tf.element.c_str(), oper->lexeme.size()) == 0;
            }
            if (matchesPrefix) {
                matched = matched + tf.frequency - matched * tf.frequency;
                n_matched++;
            }
            allmces = allmces + tf.frequency - allmces * tf.frequency;
        }
        matched = std::max(0.0, std::min(1.0, matched));
        allmces = std::max(0.0, std::min(1.0, allmces));
        double selec = matched + (1.0 - allmces) * (static_cast<double>(n_matched) / length);
        // Ensure selectivity is at least as high as for a non-MCELEM exact match
        double floorVal = std::min(DEFAULT_TS_MATCH_SEL, minFreq / 2.0);
        return std::max(floorVal, selec);
    }
}

// Recursive selectivity estimation
double tsquery_opr_selec(const QueryNode* node, const std::vector<TextFreq>& lookup,
                         int length, double minFreq, bool hasStats) {
    if (node == nullptr) {
        return 0.0;
    }

    double selec = 0.0;
    switch (node->type) {
        case QueryNodeType::VAL:
            selec = tsquery_val_selec(node->operand, lookup, length, minFreq, hasStats);
            break;
        case QueryNodeType::NOT: {
            double child = tsquery_opr_selec(node->left, lookup, length, minFreq, hasStats);
            selec = 1.0 - child;
            break;
        }
        case QueryNodeType::AND: {
            double s1 = tsquery_opr_selec(node->left, lookup, length, minFreq, hasStats);
            double s2 = tsquery_opr_selec(node->right, lookup, length, minFreq, hasStats);
            selec = s1 * s2;
            break;
        }
        case QueryNodeType::OR: {
            double s1 = tsquery_opr_selec(node->left, lookup, length, minFreq, hasStats);
            double s2 = tsquery_opr_selec(node->right, lookup, length, minFreq, hasStats);
            selec = s1 + s2 - s1 * s2;
            break;
        }
    }
    // Clamp result to [0.0, 1.0]
    selec = std::max(0.0, std::min(1.0, selec));
    return selec;
}

// Main entry point: estimate selectivity of a tsquery with optional MCELEM statistics
// Returns 0.0 if query is null or empty (no valid nodes)
double estimateTsQuerySelectivity(const QueryNode* root,
                                  const std::vector<TextFreq>& mcelem,
                                  double minFreq,
                                  bool hasStats) {
    if (root == nullptr) {
        return 0.0;
    }
    return tsquery_opr_selec(root, mcelem, static_cast<int>(mcelem.size()), minFreq, hasStats);
}

#include <cassert>
#include <iostream>
#include <vector>
#include <string>

// Assume the solution header is included here; for standalone, paste the solution above.

int main() {
    using namespace std;

    // Helper to create a leaf node
    auto makeVal = [](const string& lex, bool prefix) {
        QueryNode* n = new QueryNode();
        n->type = QueryNodeType::VAL;
        n->operand = new QueryOperand{lex, prefix};
        return n;
    };
    // Helper to create an operator node
    auto makeOp = [](QueryNodeType t, QueryNode* l, QueryNode* r) {
        QueryNode* n = new QueryNode();
        n->type = t;
        n->left = l;
        n->right = r;
        return n;
    };

    // No stats, exact match
    {
        QueryNode* q = makeVal("hello", false);
        assert(abs(estimateTsQuerySelectivity(q, {}, 0.0, false) - 0.005) < 1e-9);
        delete q->operand; delete q;
    }

    // Empty query (null root)
    {
        assert(estimateTsQuerySelectivity(nullptr, {}, 0.0, false) == 0.0);
    }

    // Exact match found in MCELEM
    {
        vector<TextFreq> mcelem = {{"apple", 0.2}, {"banana", 0.1}, {"cherry", 0.05}};
        double minFreq = 0.05;
        QueryNode* q = makeVal("banana", false);
        double sel = estimateTsQuerySelectivity(q, mcelem, minFreq, true);
        assert(abs(sel - 0.1) < 1e-9);
        delete q->operand; delete q;
    }

    // Exact match not in MCELEM
    {
        vector<TextFreq> mcelem = {{"apple", 0.2}, {"banana", 0.1}, {"cherry", 0.05}};
        double minFreq = 0.05;
        QueryNode* q = makeVal("grape", false);
        double sel = estimateTsQuerySelectivity(q, mcelem, minFreq, true);
        assert(abs(sel - min(0.005, 0.05/2)) < 1e-9);  // min(0.005, 0.025) = 0.005
        delete q->operand; delete q;
    }

    // Prefix match with insufficient stats (<100) → default*4
    {
        vector<TextFreq> mcelem = {{"apple", 0.2}, {"banana", 0.1}};
        double minFreq = 0.1;
        QueryNode* q = makeVal("app", true);
        double sel = estimateTsQuerySelectivity(q, mcelem, minFreq, true);
        assert(abs(sel - 0.02) < 1e-9);  // 0.005*4 = 0.02
        delete q->operand; delete q;
    }

    // AND combination
    {
        vector<TextFreq> mcelem = {{"foo", 0.3}, {"bar", 0.2}, {"baz", 0.1}};
        double minFreq = 0.1;
        QueryNode* q = makeOp(QueryNodeType::AND, makeVal("foo", false), makeVal("bar", false));
        double sel = estimateTsQuerySelectivity(q, mcelem, minFreq, true);
        assert(abs(sel - 0.3*0.2) < 1e-9);
        delete q->left->operand; delete q->left;
        delete q->right->operand; delete q->right;
        delete q;
    }

    // OR combination with exact matches
    {
        vector<TextFreq> mcelem = {{"foo", 0.3}, {"bar", 0.2}};
        double minFreq = 0.2;
        QueryNode* q = makeOp(QueryNodeType::OR, makeVal("foo", false), makeVal("bar", false));
        double sel = estimateTsQuerySelectivity(q, mcelem, minFreq, true);
        assert(abs(sel - (0.3 + 0.2 - 0.3*0.2)) < 1e-9);
        delete q->left->operand; delete q->left;
        delete q->right->operand; delete q->right;
        delete q;
    }

    // NOT combination
    {
        QueryNode* q = makeOp(QueryNodeType::NOT, makeVal("foo", false), nullptr);
        double sel = estimateTsQuerySelectivity(q, {}, 0.0, false);
        assert(abs(sel - (1.0 - 0.005)) < 1e-9);
        delete q->left->operand; delete q->left;
        delete q;
    }

    // Complex: (foo AND NOT bar) OR (prefix "ch" with enough stats)
    {
        vector<TextFreq> mcelem;
        for (int i = 0; i < 100; ++i) {
            mcelem.push_back({"word" + to_string(i), 0.001});
        }
        // Make 10 entries start with "ch"
        for (int i = 0; i < 10; ++i) {
            mcelem[50 + i].element = "ch" + to_string(i);
        }
        double minFreq = 0.001;
        QueryNode* inner = makeOp(QueryNodeType::AND, makeVal("word1", false), makeOp(QueryNodeType::NOT, makeVal("word2", false), nullptr));
        QueryNode* prefix = makeVal("ch", true);
        QueryNode* q = makeOp(QueryNodeType::OR, inner, prefix);
        double sel = estimateTsQuerySelectivity(q, mcelem, minFreq, true);
        // Expect a value between 0 and 1
        assert(sel >= 0.0 && sel <= 1.0);
        // Cleanup (simplified; in real code need proper deletion, but here just to compile)
        // For brevity, skip deep cleanup or use smart pointers; tests pass with leak detection off.
    }

    cout << "All tests passed!" << endl;
    return 0;
}
