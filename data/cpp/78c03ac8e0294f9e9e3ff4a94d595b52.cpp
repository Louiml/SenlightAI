/*
Implement a C++ function that constructs a suffix tree for a given vector of unsigned integers and returns a vector of `(startIndex, length)` pairs representing all distinct substrings of the input sequence. The function should take a `const std::vector<unsigned>&` as input and return a `std::vector<std::pair<unsigned, unsigned>>`. A substring is identified by its starting index in the original sequence and its length. The output must contain exactly one entry for each distinct substring, ordered lexicographically by the substring's content (treating the integers as symbols). For example, given the input `{1,2,1}`, the distinct substrings are: `{1}`, `{1,2}`, `{1,2,1}`, `{2}`, `{2,1}`, and their corresponding pairs would be `(0,1),(0,2),(0,3),(1,1),(1,2)` but sorted by content lexicographically as `(0,1),(1,1),(0,2),(1,2),(0,3)`. You must implement the suffix tree construction yourself (no external libraries). The function should handle empty input (return empty vector) and sequences with repeated symbols correctly. Your solution should be efficient for sequences up to length 10,000.
*/

#include <vector>
#include <map>
#include <utility>
#include <climits>
#include <cassert>

namespace {

const unsigned EMPTY_IDX = UINT_MAX;

struct SuffixTreeNode {
    unsigned StartIdx;
    unsigned *EndIdx;
    SuffixTreeNode *Link;
    unsigned ConcatLen;
    std::map<unsigned, SuffixTreeNode*> Children;

    SuffixTreeNode(unsigned s, unsigned *e, SuffixTreeNode *l)
        : StartIdx(s), EndIdx(e), Link(l), ConcatLen(0) {}

    unsigned size() const { return *EndIdx - StartIdx + 1; }
    bool isRoot() const { return StartIdx == EMPTY_IDX; }
};

struct SuffixTree {
    const std::vector<unsigned> &Str;
    SuffixTreeNode *Root;
    unsigned LeafEndIdx;
    unsigned ActiveLen, ActiveIdx;
    SuffixTreeNode *ActiveNode;
    std::vector<SuffixTreeNode*> Nodes;

    SuffixTree(const std::vector<unsigned> &s) : Str(s), LeafEndIdx(0) {
        Root = insertInternalNode(nullptr, EMPTY_IDX, EMPTY_IDX, 0);
        ActiveNode = Root;
        ActiveLen = ActiveIdx = 0;
        unsigned suffixesToAdd = 0;
        for (unsigned pf = 0; pf < Str.size(); ++pf) {
            ++suffixesToAdd;
            LeafEndIdx = pf;
            suffixesToAdd = extend(pf, suffixesToAdd);
        }
        computeConcatLen();
    }

    ~SuffixTree() {
        for (auto *n : Nodes) {
            if (n->EndIdx != &LeafEndIdx) delete n->EndIdx;
            delete n;
        }
    }

    SuffixTreeNode* insertLeaf(SuffixTreeNode &parent, unsigned startIdx, unsigned edge) {
        auto *n = new SuffixTreeNode(startIdx, &LeafEndIdx, nullptr);
        parent.Children[edge] = n;
        Nodes.push_back(n);
        return n;
    }

    SuffixTreeNode* insertInternalNode(SuffixTreeNode *parent, unsigned startIdx,
                                       unsigned endIdx, unsigned edge) {
        auto *e = new unsigned(endIdx);
        auto *n = new SuffixTreeNode(startIdx, e, Root);
        if (parent) parent->Children[edge] = n;
        Nodes.push_back(n);
        return n;
    }

    void computeConcatLen() {
        std::vector<std::pair<SuffixTreeNode*, unsigned>> stack;
        stack.push_back({Root, 0});
        while (!stack.empty()) {
            auto [node, len] = stack.back();
            stack.pop_back();
            node->ConcatLen = len;
            for (auto &p : node->Children)
                stack.push_back({p.second, len + p.second->size()});
        }
    }

    unsigned extend(unsigned endIdx, unsigned suffixesToAdd) {
        SuffixTreeNode *needsLink = nullptr;
        while (suffixesToAdd > 0) {
            if (ActiveLen == 0) ActiveIdx = endIdx;
            unsigned firstChar = Str[ActiveIdx];

            if (ActiveNode->Children.count(firstChar) == 0) {
                insertLeaf(*ActiveNode, endIdx, firstChar);
                if (needsLink) {
                    needsLink->Link = ActiveNode;
                    needsLink = nullptr;
                }
            } else {
                auto *next = ActiveNode->Children[firstChar];
                unsigned nextLen = next->size();
                if (ActiveLen >= nextLen) {
                    ActiveIdx += nextLen;
                    ActiveLen -= nextLen;
                    ActiveNode = next;
                    continue;
                }
                unsigned lastChar = Str[endIdx];
                if (Str[next->StartIdx + ActiveLen] == lastChar) {
                    if (needsLink && !ActiveNode->isRoot()) {
                        needsLink->Link = ActiveNode;
                        needsLink = nullptr;
                    }
                    ++ActiveLen;
                    break;
                }
                auto *split = insertInternalNode(ActiveNode, next->StartIdx,
                                                 next->StartIdx + ActiveLen - 1, firstChar);
                insertLeaf(*split, endIdx, lastChar);
                next->StartIdx += ActiveLen;
                split->Children[Str[next->StartIdx]] = next;
                if (needsLink) needsLink->Link = split;
                needsLink = split;
            }
            --suffixesToAdd;
            if (ActiveNode->isRoot()) {
                if (ActiveLen > 0) {
                    --ActiveLen;
                    ActiveIdx = endIdx - suffixesToAdd + 1;
                }
            } else {
                ActiveNode = ActiveNode->Link;
            }
        }
        return suffixesToAdd;
    }
};

void enumerateSubstrings(SuffixTreeNode *node, unsigned pathLen, unsigned startIdx,
                         std::vector<std::pair<unsigned, unsigned>> &result) {
    for (auto &p : node->Children) {
        auto *child = p.second;
        unsigned childStart = node->isRoot() ? child->StartIdx : startIdx;
        unsigned edgeLen = child->size();
        for (unsigned k = 1; k <= edgeLen; ++k)
            result.emplace_back(childStart, pathLen + k);
        enumerateSubstrings(child, pathLen + edgeLen, childStart, result);
    }
}

} // anonymous namespace

std::vector<std::pair<unsigned, unsigned>> distinctSubstrings(const std::vector<unsigned>& str) {
    if (str.empty()) return {};
    SuffixTree tree(str);
    std::vector<std::pair<unsigned, unsigned>> result;
    enumerateSubstrings(tree.Root, 0, 0, result);
    return result;
}

#include <cassert>
#include <vector>
#include <utility>

int main() {
    // Empty input
    assert(distinctSubstrings({}) == std::vector<std::pair<unsigned, unsigned>>{});

    // Single element
    assert(distinctSubstrings({1}) == std::vector<std::pair<unsigned, unsigned>>{{0,1}});

    // Repeated elements
    assert(distinctSubstrings({1,1}) == std::vector<std::pair<unsigned, unsigned>>{{0,1},{0,2}});

    // Example from task
    std::vector<std::pair<unsigned, unsigned>> expected1 = {{0,1},{0,2},{0,3},{1,1},{1,2}};
    assert(distinctSubstrings({1,2,1}) == expected1);

    // All distinct
    std::vector<std::pair<unsigned, unsigned>> expected2 = {
        {0,1},{0,2},{0,3},{0,4},
        {1,1},{1,2},{1,3},
        {2,1},{2,2},
        {3,1}
    };
    assert(distinctSubstrings({1,2,3,4}) == expected2);

    // Mixed with duplicates
    std::vector<std::pair<unsigned, unsigned>> expected3 = {
        {0,1},{0,2},{1,1},{1,2},{2,1}
    }; // For {2,1,2}: substrings [2](0,1), [2,1](0,2), [1](1,1), [1,2](1,2), [2](2,1) but distinct [2] appears twice, only first occurrence (0,1) and also [2] at 2 is same content, so only one. Lexicographic: [1] (1,1) < [1,2] (1,2) < [2] (0,1) < [2,1] (0,2) < [2,1,2]? Wait, let's properly compute for {2,1,2}: distinct substrings: [2] (0,1), [2,1] (0,2), [2,1,2] (0,3), [1] (1,1), [1,2] (1,2). Sorted lexicographically: [1] (1,1), [1,2] (1,2), [2] (0,1), [2,1] (0,2), [2,1,2] (0,3). So expected3 = {{1,1},{1,2},{0,1},{0,2},{0,3}} but our function returns in DFS order ignoring lexicographic? Actually lexicographic order: compare [1] vs [1,2] vs [2] vs [2,1] vs [2,1,2]. Since 1<2, all starting with 1 come first: [1] and [1,2]; then starting with 2: [2] (prefix), [2,1], [2,1,2]. So order: (1,1),(1,2),(0,1),(0,2),(0,3). Let's verify our DFS: root children are 2 (since 2 appears first) and 1. Since map sorts, child 1 is visited first. Child 1 has start idx = child->StartIdx? Root child 1 StartIdx is the index of first 1, which is 1. So child 1 edge length? It leads to suffix [1,2]? Actually root child 1 edge label likely [1]? Let's not trust. But our algorithm should produce lexicographic order because DFS visits children in sorted order. So for {2,1,2}, root children: 1 (value 1) and 2 (value 2). So child 1 visited first, giving (1,1) and (1,2) for prefixes of its edge (if edge length 2). Then child 2 gives (0,1),(0,2),(0,3). So output would be {{1,1},{1,2},{0,1},{0,2},{0,3}} which is correct. We'll test that.
    assert(distinctSubstrings({2,1,2}) == std::vector<std::pair<unsigned, unsigned>>{{1,1},{1,2},{0,1},{0,2},{0,3}});

    return 0;
}

// The task requires building a suffix tree for an integer sequence and then enumerating all distinct substrings. The standard approach is Ukkonen's online suffix tree construction, which builds the tree in linear time. The provided code snippet shows a typical implementation: it processes the string character by character, maintaining an "active point" (node, edge position) and using suffix links to efficiently add new suffixes. After building the tree, a depth-first traversal collects all distinct substrings: each node (except the root) represents a set of substrings—those along the path from root to that node. Specifically, each node with path-label length `L` (sum of edge lengths from root) contributes all substrings of lengths from `parentLength+1` to `L`, each corresponding to a start index computed from the suffix index (for leaves) or inferred from the node's position. To collect distinct substrings, we can perform a DFS from the root, maintaining the current path (sequence of symbols) and the start position. At each node, for every extension of the path by one symbol, we add a pair `(startIdx, length)` where `startIdx` is the position of the first symbol of that extension in the original string. For internal nodes, the edges lead to children that represent longer substrings; the start index for the beginning of that edge is derived from the node's start index (for the first symbol of the edge). A simpler approach: after building the tree, traverse all nodes and for each edge (parent→child) with label length `edgeLen` and starting symbol position `startIdx` (which is `child->StartIdx`), we know that all prefixes of that edge label correspond to distinct substrings that are not represented higher up. However, the standard method is: for each node (except root), the number of distinct substrings contributed by that node's path is `node->ConcatLen - parentConcatLen`. But to get the actual pairs, we need the start positions. A robust method: perform a DFS from root, carrying a `startIdx` for the current path. When we traverse an edge (child), the edge label is `Str[child->StartIdx ... child->StartIdx+child->size()-1]`. For each position `k` from 1 to `child->size()`, the substring of length `k` starting at `pathStart + (k-1)` (where `pathStart` is the start index of the current path's first symbol) is a new distinct substring. However, we must ensure we don't duplicate substrings that are already represented by higher nodes. Since each distinct substring appears exactly once as a prefix of some path from root to a node, and each path corresponds to a unique node, we can collect all substrings by considering every node's path. But a node's path represents all substrings from length `parentLen+1` to `childLen`. To get their start indices, we can note that for a node representing a substring of length `L` starting at index `s`, the suffixes represented by its children start at `s + edgeOffset` etc. A simpler implementation: after building the tree, collect all `(startIdx, length)` pairs by traversing each edge. For each edge from parent to child, the edge label is `Str[child->StartIdx ... child->StartIdx+child->size()-1]`. For each `j` from 1 to `child->size()`, the substring of length `j` that ends at the end of this edge's prefix is actually a distinct substring, but its start index is `child->StartIdx + child->size() - j`? That is not correct. Better: we can collect all suffixes of all prefixes. Actually, a standard fact: each distinct substring corresponds to a path from root to a node (or a point on an edge). If we collect all suffixes of the suffix tree's path labels, we get all substrings. The start index for a substring that is a suffix of a node's path label can be derived from the node's suffix index (for leaves) or by combining. The cleanest approach: use the tree to generate all substrings explicitly by doing a DFS that builds the sequence along the path and at each step (at each node and also after each character along an edge) adds the current substring's start index. Since we need only start index and length, we can compute the start index as the position in the original string where the current path's first character came from. When we traverse an edge from node `N` to child `C`, the first character of that edge is at `Str[C->StartIdx]`. The start index of the substring that is the current path plus the first `k` characters of this edge is `currentStartIdx` for `k=0`? We need to be precise: The current path (from root to `N`) is a substring that starts at some index `S`. When we take the first character of the edge, the new substring of length `len(N)+1` starts at `S` (same start). The next character gives length `len(N)+2` still starting at `S`. So all extensions by the edge have the same start index `S`. However, each distinct substring that ends at any point along this edge is a new substring. For example, if the edge label is `[2,3]` and we are at node representing `[1]` starting at index 0, then substrings `[1,2]` (start 0, len 2) and `[1,2,3]` (start 0, len 3) are both new. But we also have substrings that start at later positions, like `[2]` and `[2,3]` which start at index 1. These are covered by other paths (from root via another edge). Indeed, each distinct substring has exactly one path from root. So if we do a DFS and for each edge, we add all prefixes of the edge label concatenated to the current path, those are all distinct. The start index for these is the start index of the current path. But we must also consider that the current path might be empty (root), then the start index for the first character of the edge is `C->StartIdx`. After that, all extensions of that edge have start index `C->StartIdx`. So the algorithm: DFS from root with `currentStartIdx` initially `EmptyIdx` (sentinel). For each child `C` of node `N`, the edge label length is `C->size()`. For `j` from 1 to `C->size()`, the substring consisting of the current path plus the first `j` characters of the edge has length `currentLen + j` and start index = (if `currentLen==0` then `C->StartIdx` else `currentStartIdx`). Actually if current path is empty (root), then the substring starting with the first char of edge starts at `C->StartIdx`; subsequent j>1 also start at `C->StartIdx`. If current path is non-empty, it has a start index `S`, and appending characters from the edge does not change the start index, so it remains `S`. However, this would generate duplicates because the substring that is exactly the edge label itself (when current path empty) is also generated as j-th prefix, but that's fine—it's distinct. But we must be careful: if we traverse node `N` which itself represents a substring, we should add that substring as well. However, the root represents empty string, not a substring. For non-root nodes, the node itself represents the entire path from root, which is a distinct substring. We can add that when we visit the node. But our DFS edge-based method already adds all prefixes of the edge, including the entire edge label. So at the child node, the path includes the entire edge, so adding the node's path is redundant. So we just iterate over all edges and add all prefixes of each edge. That gives all distinct substrings exactly once? Let's test with `{1,2,1}`. Tree: root → leaf for '1' (StartIdx=2, size=1) representing suffix [1]; root → internal node for '2'? Actually root → edge '1' leads to a node that represents [1] (leaf) and also has child '2'? Let's build manually: suffixes: [1,2,1], [2,1], [1]. Tree: root has children: 1 (edge label [1,2,1])? No, Ukkonen gives root→leaf for full string? Actually simpler: root has child for symbol 1, which leads to an internal node that splits into [1,2,1] and [1]? Let's not overcomplicate. The edge-based DFS: For each edge from root to child C, add all prefixes of C's edge label. For root, each child edge label is a suffix of the string? No, the root's children are the first character of each suffix. So edge label length can be >1. For example, if input is [1,2,3], the root has one child with edge label [1,2,3]? Actually in a proper suffix tree for a single string (not including sentinel), root has multiple children. For [1,2,3], suffixes: [1,2,3], [2,3], [3]. Root has child for 1 with edge [1,2,3]? That would be one edge, but then there is no branching, which is fine. So edge from root to leaf has label [1,2,3] length 3. Adding prefixes gives (0,1),(0,2),(0,3). Then other suffixes [2,3] and [3] are not represented as separate edges from root? Actually they are because root also has child for 2 and 3, but they'd be internal? In a proper suffix tree, each suffix corresponds to a path from root. So root has three children: for '1' (going to a leaf), for '2', and for '3'. The edge for '1' might have label [1] only, and then from that node an edge for '2' etc. So the edge-based DFS with adding all prefixes of each edge works. We must ensure we don't miss substrings that start at non-suffix positions; they are covered because every substring is a prefix of some suffix. Indeed, any substring is a prefix of the suffix that starts at its beginning. So it will be a path from root. So the algorithm: build suffix tree, then DFS from root. At each node, for each child, for each prefix length `k` from 1 to child->size(), add pair `(startIdx, currentLen + k)` where `startIdx` is the start index of the current path if current path non-empty, else `child->StartIdx`? Actually when current path is empty (root), the start index of the prefix of length k is `child->StartIdx` because the first character of the edge is at that index, and the prefix starts there. When current path is non-empty, the start index is `currentStartIdx`. But we need to track `currentStartIdx` in DFS. When we traverse an edge from node N to child C, we update the current path. The start index for the path that ends at C (the full edge included) is: if N is root, then `C->StartIdx`; else it's `currentStartIdx` (since we are only extending the path). So we pass to child the same `currentStartIdx`. For adding prefixes: for each k, we add `(currentStartIdxOfPath, currentLen + k)`, but if current path is empty we use `C->StartIdx`. However, we can simplify: we only need to add prefixes for edges when we traverse them. So in the DFS, for child C of node N, we have `pathLen` = length of path from root to N (node's ConcatLen). Then for each k=1..C->size(), the new substring has length `pathLen + k` and start index = (if N is root) `C->StartIdx` else `currentStartIdx`. But we need `currentStartIdx` for node N, which is the start index of the substring that ends at N. We can compute that during DFS: when we go from N to C, the start index for C is the same as N's start index (if N not root). For root's children, start index is `C->StartIdx`. So we can pass `startIdx` down. Then for each child, we add k from 1 to child->size() with start = `startIdx` (the start of the path to N) but careful: if N is root, the path is empty, so the substring that is just the first k chars of the edge starts at `child->StartIdx`. So start = (N==Root) ? child->StartIdx : startIdx. Since startIdx is undefined for root, we can set startIdx = 0 for root but not use it. Simpler: when traversing, we know the start index of the current path (the path from root to current node). We can store it in the DFS state. For root, startIdx = 0 but path length 0, we don't add anything for root itself. When going to child, the start index of the new node C is: if current node is root, then C->StartIdx; else currentStartIdx. Then for that edge, we add all prefixes of the edge using the start index of the path *before* the edge? Actually the prefixes of the edge have start index = (if root) C->StartIdx, else currentStartIdx. And after adding the full edge, the start index for child C remains that same value. So in DFS, we can do: when at node N with pathLen pLen and startIdx sIdx (for the path ending at N), for each child C: compute childStartIdx = (N==Root) ? C->StartIdx : sIdx. Then for k=1..C->size(), add pair (childStartIdx, pLen + k). Then recursively visit C with (pLen + C->size(), childStartIdx). This generates all distinct substrings. But we must handle the case where multiple edges have the same childStartIdx? That's fine. Also, we must ensure that we don't add duplicates when the same substring appears as a prefix of multiple edges? It shouldn't, because each distinct substring has exactly one path. The edge-based method is correct as long as the suffix tree has no redundant edges (which it doesn't). Complexity: tree construction O(n) with Ukkonen, DFS O(number of nodes + total edge length?) Actually the DFS visits each node once, but for each edge we loop k from 1 to edgeLength, which can be O(n^2) in worst case if we have a long path (e.g., string with all same characters, the suffix tree has a single leaf with edge length n, and we add O(n) pairs). That is acceptable because the output size is O(n^2) in worst case (all strings are distinct substrings, total O(n^2)). But for n=10,000 that would be 100 million pairs, which is too many. The task expects distinct substrings count can be up to O(n^2), so it's fine. However, the function returns all distinct substrings, which is inherently O(n^2) in worst case (e.g., all symbols distinct gives n(n+1)/2 substrings). So output size is O(n^2). The reference solution must be able to enumerate them. The tree construction is O(n) time, and enumeration is O(output size). Memory is O(n) for tree. Edge cases: empty input returns empty vector; single element returns one pair (0,1); all same symbols e.g., {1,1,1} has distinct substrings {1}, {1,1}, {1,1,1} so three pairs. We need to ensure sorting lexicographically by content. The tree's natural DFS order from children sorted by symbol value would give lexicographic order. So in DFS, iterate children in ascending order of symbol (edge). Standard implementation uses `std::map` for children, which sorts by symbol. That ensures output is sorted lexicographically. We'll use `std::map<unsigned, SuffixTreeNode*>`. In the reference solution, we'll re-implement a simplified suffix tree (not using LLVM). We'll create classes for nodes, implement Ukkonen's algorithm from scratch. The solution will include necessary headers, and the free function `distinctSubstrings`. We must ensure correctness for integer sequences including zeros? The input is unsigned, but can be any positive integers. Ukkonen typically works with a sentinel at end to avoid leaf issues, but the provided snippet doesn't use a sentinel; it iterates over the string and uses `LeafEndIdx` shared among leaves. We'll adopt that approach. We need to handle the case where `Str` is empty. We'll return empty vector. Also handle the case where string length >0. We'll define constants `EmptyIdx = UINT_MAX` or similar. Node structure: `StartIdx`, pointer to shared `EndIdx` (for leaves) or own `EndIdx` (internal), `Link`, `ConcatLen`, `SuffixIdx`, `Children` map. We'll implement insertLeaf, insertInternalNode, extend, setSuffixIndices similar to snippet. Then enumerate. For enumeration, we need `ConcatLen` computed via DFS as in snippet. Then we do a DFS from root to add pairs. Since we need sorted order, we'll use `std::map` for children, which sorts by `unsigned` key, giving lexicographic order (since symbols are compared as unsigned values). In the DFS, we'll collect pairs in a vector. Since `std::map` is sorted, the iteration order gives correct lexicographic order. We must be careful: lexicographic order of substrings compares symbols numerically. Since `std::map` sorts by key, and DFS visits children in that order, the generated pairs will be in sorted order. However, we generate pairs by iterating over edges and for each edge adding prefixes in increasing length. That is correct because within a path, longer substrings that share the same prefix are grouped after shorter ones? Lexicographic sort compares the sequences: for example, {1} vs {1,2}. {1} is a prefix of {1,2}, so {1} comes before {1,2}. Our method adds for edge (from root to child with symbol 1, edge label length L) the prefixes of length 1,2,...L, so they are in correct order. For two different first symbols, smaller symbol first. So it works.
//
// Now, we need to write the solution as a free function `std::vector<std::pair<unsigned, unsigned>> distinctSubstrings(const std::vector<unsigned>& str)`. We'll implement the tree inside the function or as helper classes. To keep self-contained, we can define local structs within the function or as private classes in an anonymous namespace. The solution code should be concise but correct. We'll re-implement Ukkonen's algorithm. Important details: - We need a shared `LeafEndIdx` that is updated each time we add a new character. - Each node has `Children` as `std::map<unsigned, Node*>`. - `Root` is internal node with StartIdx=EmptyIdx, EndIdx=EmptyIdx, Link=nullptr. - Active point: Node, Len, Idx. - Suffix links: internal nodes have Link to another internal node or root. - In `extend`, we follow the algorithm. We need to be careful with the `NeedsLink` handling. - After building, we compute `ConcatLen` via DFS. - Then we enumerate all distinct substrings. We must ensure we don't add empty string. The pair `(start, length)` where length>0. We'll add for each edge as described.
//
// Time complexity: O(n) for tree construction, O(number of substrings) for enumeration. Worst-case number of distinct substrings is O(n^2), but that is unavoidable if we return all. The task doesn't specify limits, but it's fine. Space complexity O(n) for tree, plus O(output) for result vector.
//
// Let's write the solution. We'll need to include `<vector>`, `<map>`, `<utility>`, `<cstdint>`. We'll use `unsigned` for indices. We'll define `EmptyIdx = UINT_MAX` (or a sentinel like -1 but unsigned). Use `std::map<unsigned, Node*>`.
//
// We'll implement helper methods inside a class `SuffixTree` but we can just define local structs and free functions. To keep it neat, we'll define a `struct Node` and a `SuffixTree` class within the function scope? C++ doesn't allow local classes to be used in templates? It's fine. But we can use `namespace { ... }` at file scope. The final answer must be a self-contained function in a single file. We'll output code only in the section.
//
// We'll also need to compute the `ConcatLen` for nodes, and for enumeration we need to know the start index of the path to each node. We'll perform a DFS that carries `pathLen` and `startIdx`. For the root, `pathLen=0`, `startIdx` not defined; we'll use a sentinel. For each child, we compute childStartIdx as described. Then for k from 1 to child->size(), we add `(childStartIdx, pathLen + k)`. Then recurse to child with `(pathLen + child->size(), childStartIdx)`. This ensures all substrings are enumerated in lexicographic order because we process children in map order and for each child we add prefixes in increasing length. However, we need to be careful: when we process a child, we add all prefixes of its edge. But note that for a node that is not root, the path to that node is already a distinct substring. But that substring is the full edge label from its parent, which we already added as the last prefix (k = child->size()) when processing that edge. So we don't need to add the node's path separately. Good.
//
// Let's test with a simple example. Input `{1,2,1}`. Build tree. Suffixes: [1,2,1], [2,1], [1]. Tree: root has child 1 (edge label [1]) leading to internal node A. A has child 2 (edge label [2]) leading to leaf for suffix [1,2,1]? Actually leaf at end with StartIdx=0. Also A has child 1 (edge label [1]) leading to leaf for suffix [1] ending at index 2. Root also has child 2 (edge label [2]?) leading to leaf for suffix [2,1]? Wait, root child 2 with edge label [2,1]? Let's not overthink; enumeration should work. DFS from root. For child 1 (symbol 1) with edge length maybe 1, add (0,1) because childStartIdx = child->StartIdx (say 0). Then recurse to node A. At node A, process child 2 (symbol 2) edge length 1, startIdx is 0 (path from root to A started at 0), add (0,2) then recurse to leaf with pathLen 2. At that leaf, no children. Then process child 1 (symbol 1) edge length 1, startIdx 0, add (0,3) but that would be substring [1,2,1] which is correct. Also root child 2 (symbol 2) edge length 2? Actually the suffix [2,1] starts at index 1, so root child 2 has edge label [2,1] length 2, childStartIdx = 1. For k=1 add (1,1); k=2 add (1,2). We get pairs: (0,1),(0,2),(0,3),(1,1),(1,2). Sorted lexicographically: (0,1),(1,1),(0,2),(1,2),(0,3) - but our order from DFS: root child 1 first gives (0,1),(0,2),(0,3) then root child 2 gives (1,1),(1,2). That order is (0,1),(0,2),(0,3),(1,1),(1,2) which is not lexicographic because (1,1) should come before (0,2) since [2] < [1,2]? Actually compare [1] vs [2]: [1] < [2] so (0,1) should be first, then [1,2] (0,2) vs [2] (1,1): compare first symbol: 1 vs 2, so [1,2] < [2], so (0,2) before (1,1). So our order is correct: (0,1),(0,2),(0,3) all start with 1, then (1,1),(1,2) start with 2. So it's lexicographic. Good.
//
// Now we must ensure that for root's child with edge length >1, we add prefixes in increasing length, which is correct because shorter prefix is lexicographically smaller (since it's a prefix). So solution is valid.
//
// Edge case: empty input, return empty. For input with one element, tree root has one child with edge length 1, we add (0,1).
//
// We also need to consider the case where the input vector contains zeros or any unsigned, fine.
//
// Now write the code. We'll implement `SuffixTree` struct with methods. We'll use `size_t` for indices but `unsigned` as per snippet. We'll include `<climits>` for UINT_MAX.
//
// Let's write the solution. We'll keep it self-contained.
