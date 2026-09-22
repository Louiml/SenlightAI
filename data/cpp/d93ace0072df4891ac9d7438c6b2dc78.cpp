/*
Given a sequence of operations for a disjoint-set data structure with union by rank and path compression, write a C++ function `bool verifyUnionFind(const std::vector<std::string>& operations)` that processes the operations and returns `true` if and only if the sequence is internally consistent. Each operation is one of: `"MAKE x"`, `"UNION x y"`, or `"FIND x y"`. `MAKE` creates a new singleton set containing integer `x`. `UNION x y` merges the sets containing `x` and `y`. `FIND x y` checks whether `x` and `y` are in the same set; if they are not both currently valid elements (i.e., never appeared in a `MAKE` or `UNION` operation before the `FIND`), the operation is invalid and the function must return `false`. The function should simulate the operations using the exact Union-by-Rank with rank-as-size (as in the original snippet) and path compression, and return `false` if any `FIND` reports different set membership than the actual state of the data structure (i.e., if the operation claims they are in the same set when they are not, or claims they are in different sets when they are the same). Also return `false` if a `UNION` references an element not yet created, or if a `MAKE` attempts to create an element that already exists. You may assume all integers are non-negative and small enough to fit in `long`. The function must be self-contained, not relying on any external disjoint-set library.
*/
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

struct DSUNode {
    long data;
    long rank; // size of set
    DSUNode* parent;
    DSUNode(long d, long r) : data(d), rank(r), parent(this) {}
};

class DisjointSetValidator {
private:
    std::unordered_map<long, DSUNode*> nodes;

    DSUNode* findRoot(DSUNode* node) {
        if (node->parent != node) {
            node->parent = findRoot(node->parent);
        }
        return node->parent;
    }

public:
    bool make(long x) {
        if (nodes.find(x) != nodes.end()) return false;
        nodes[x] = new DSUNode(x, 1);
        return true;
    }

    bool unite(long x, long y) {
        if (nodes.find(x) == nodes.end() || nodes.find(y) == nodes.end()) return false;
        DSUNode* rootX = findRoot(nodes[x]);
        DSUNode* rootY = findRoot(nodes[y]);
        if (rootX == rootY) return true; // already same, nothing to do
        if (rootX->rank >= rootY->rank) {
            rootY->parent = rootX;
            rootX->rank += rootY->rank;
        } else {
            rootX->parent = rootY;
            rootY->rank += rootX->rank;
        }
        return true;
    }

    bool find(long x, long y) {
        if (nodes.find(x) == nodes.end() || nodes.find(y) == nodes.end()) return false;
        DSUNode* rootX = findRoot(nodes[x]);
        DSUNode* rootY = findRoot(nodes[y]);
        return rootX == rootY;
    }

    ~DisjointSetValidator() {
        for (auto& pair : nodes) delete pair.second;
    }
};

bool verifyUnionFind(const std::vector<std::string>& operations) {
    DisjointSetValidator dsu;
    for (const std::string& op : operations) {
        std::istringstream iss(op);
        std::string cmd;
        iss >> cmd;
        if (cmd == "MAKE") {
            long x;
            if (!(iss >> x)) return false;
            if (!dsu.make(x)) return false;
        } else if (cmd == "UNION") {
            long x, y;
            if (!(iss >> x >> y)) return false;
            if (!dsu.unite(x, y)) return false;
        } else if (cmd == "FIND") {
            long x, y;
            if (!(iss >> x >> y)) return false;
            bool claimed = (x == y); // in FIND, they say whether same? Wait parse: "FIND x y" – we need to know claimed value from input.
            // Actually the task says: `FIND x y` checks whether x and y are in the same set; but how does it encode the claim? The operation is just "FIND x y" – it doesn't have a claimed boolean. The task says: "if the operation claims they are in the same set when they are not, or claims they are in different sets when they are the same." So the operation itself is a query that carries no expected answer – it just asks "are they in the same set?" But then consistency check would be meaningless. Probably the operation format is "FIND x y expected" where expected is 1 or 0. Let me reinterpret: The task says "`FIND x y` checks whether `x` and `y` are in the same set; if they are not both currently valid elements ... the operation is invalid and the function must return `false`." Then later: "return `false` if a `FIND` reports different set membership than the actual state" – so the operation must include an expected result. I'll assume the input format is "FIND x y expected" where expected is 1 (same) or 0 (different). Since the example does not show, I'll design the solution to parse three tokens after FIND: x, y, expected. I'll modify the solution accordingly.
        } else {
            return false;
        }
    }
    return true;
}

Wait, the above solution is incomplete because I misread the FIND format. The task statement says: "`FIND x y` checks whether `x` and `y` are in the same set" but then "return `false` if a `FIND` reports different set membership than the actual state". That implies the operation itself includes a claimed answer, likely "FIND x y same" or "FIND x y diff". To fully satisfy the task, I'll define the operation as "FIND x y expected" where expected is 1 for same, 0 for different. I'll rewrite the solution cleanly.

[Solution (corrected)]
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

struct Node {
    long data;
    long size; // rank as size
    Node* parent;
    Node(long d) : data(d), size(1), parent(this) {}
};

class DisjointSet {
private:
    std::unordered_map<long, Node*> map;

    Node* findRoot(Node* n) {
        if (n->parent != n) n->parent = findRoot(n->parent);
        return n->parent;
    }

public:
    bool make(long x) {
        if (map.find(x) != map.end()) return false;
        map[x] = new Node(x);
        return true;
    }

    bool unite(long x, long y) {
        if (map.find(x) == map.end() || map.find(y) == map.end()) return false;
        Node* rx = findRoot(map[x]);
        Node* ry = findRoot(map[y]);
        if (rx == ry) return true;
        // Union by size (rank as size)
        if (rx->size >= ry->size) {
            ry->parent = rx;
            rx->size += ry->size;
        } else {
            rx->parent = ry;
            ry->size += rx->size;
        }
        return true;
    }

    bool same(long x, long y) {
        if (map.find(x) == map.end() || map.find(y) == map.end()) return false;
        return findRoot(map[x]) == findRoot(map[y]);
    }

    ~DisjointSet() {
        for (auto& p : map) delete p.second;
    }
};

bool verifyUnionFind(const std::vector<std::string>& operations) {
    DisjointSet dsu;
    for (const auto& op : operations) {
        std::istringstream iss(op);
        std::string cmd;
        iss >> cmd;
        if (cmd == "MAKE") {
            long x;
            if (!(iss >> x)) return false;
            if (!dsu.make(x)) return false;
        } else if (cmd == "UNION") {
            long x, y;
            if (!(iss >> x >> y)) return false;
            if (!dsu.unite(x, y)) return false;
        } else if (cmd == "FIND") {
            long x, y;
            int expected;
            if (!(iss >> x >> y >> expected)) return false;
            bool actual = dsu.same(x, y);
            // dsu.same returns false if either element not present; but we need to distinguish "not present" vs "different set".
            // So we must check existence separately.
            // Let's modify same to return a tri-state? Simpler: we call a method that returns -1 for not present, 0 for different, 1 for same.
            // But easier: check existence first.
            // I'll add a method contains.
            if (!dsu.same(x, y) && !(dsu.same(x,x))) { // hacky, not good
                // Actually we need a contains method.
            }
            // Instead, I'll reimplement inside verify: use a separate function that returns 0,1, or -1.
        } else {
            return false;
        }
    }
    return true;
}

This is getting messy. Let me write a clean, final solution.

[Solution final]
#include <vector>
#include <string>
#include <sstream>
#include <unordered_map>

class DisjointSetValidator {
    struct Node {
        long data;
        long rank;
        Node* parent;
        Node(long d) : data(d), rank(1), parent(this) {}
    };

    std::unordered_map<long, Node*> nodes;

    Node* findRoot(Node* n) {
        if (n->parent != n) n->parent = findRoot(n->parent);
        return n->parent;
    }

public:
    bool contains(long x) const { return nodes.find(x) != nodes.end(); }

    bool make(long x) {
        if (contains(x)) return false;
        nodes[x] = new Node(x);
        return true;
    }

    bool unite(long x, long y) {
        if (!contains(x) || !contains(y)) return false;
        Node* rx = findRoot(nodes[x]);
        Node* ry = findRoot(nodes[y]);
        if (rx == ry) return true;
        if (rx->rank >= ry->rank) {
            ry->parent = rx;
            rx->rank += ry->rank;
        } else {
            rx->parent = ry;
            ry->rank += rx->rank;
        }
        return true;
    }

    int findRelation(long x, long y) {
        // returns -1 if either not present, 0 if different set, 1 if same set
        if (!contains(x) || !contains(y)) return -1;
        return (findRoot(nodes[x]) == findRoot(nodes[y])) ? 1 : 0;
    }

    ~DisjointSetValidator() {
        for (auto& p : nodes) delete p.second;
    }
};

bool verifyUnionFind(const std::vector<std::string>& operations) {
    DisjointSetValidator dsu;
    for (const auto& op : operations) {
        std::istringstream iss(op);
        std::string cmd;
        iss >> cmd;
        if (cmd == "MAKE") {
            long x;
            if (!(iss >> x)) return false;
            if (!dsu.make(x)) return false;
        } else if (cmd == "UNION") {
            long x, y;
            if (!(iss >> x >> y)) return false;
            if (!dsu.unite(x, y)) return false;
        } else if (cmd == "FIND") {
            long x, y;
            int expected;
            if (!(iss >> x >> y >> expected)) return false;
            int actual = dsu.findRelation(x, y);
            if (actual == -1 || actual != expected) return false;
        } else {
            return false;
        }
    }
    return true;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Basic valid sequence
    std::vector<std::string> ops1 = {"MAKE 1", "MAKE 2", "UNION 1 2", "FIND 1 2 1"};
    assert(verifyUnionFind(ops1) == true);

    // Invalid FIND: claims different but same set
    std::vector<std::string> ops2 = {"MAKE 1", "MAKE 2", "UNION 1 2", "FIND 1 2 0"};
    assert(verifyUnionFind(ops2) == false);

    // Invalid FIND: claims same but different sets
    std::vector<std::string> ops3 = {"MAKE 1", "MAKE 2", "FIND 1 2 1"};
    assert(verifyUnionFind(ops3) == false);

    // Invalid MAKE (duplicate)
    std::vector<std::string> ops4 = {"MAKE 5", "MAKE 5"};
    assert(verifyUnionFind(ops4) == false);

    // Invalid UNION (element not created)
    std::vector<std::string> ops5 = {"MAKE 1", "UNION 1 2"};
    assert(verifyUnionFind(ops5) == false);

    // Invalid FIND (element not created)
    std::vector<std::string> ops6 = {"MAKE 1", "FIND 1 2 0"};
    assert(verifyUnionFind(ops6) == false);

    // Union by rank with equal sizes
    std::vector<std::string> ops7 = {"MAKE 1", "MAKE 2", "UNION 1 2", "MAKE 3", "UNION 2 3", "FIND 1 3 1"};
    assert(verifyUnionFind(ops7) == true);

    // Path compression test with multiple finds
    std::vector<std::string> ops8 = {"MAKE 1", "MAKE 2", "MAKE 3", "UNION 1 2", "UNION 2 3", "FIND 1 3 1", "FIND 1 2 1", "FIND 2 3 1"};
    assert(verifyUnionFind(ops8) == true);

    // Chain of unions, then check different set
    std::vector<std::string> ops9 = {"MAKE 10", "MAKE 20", "UNION 10 20", "MAKE 30", "FIND 10 30 0"};
    assert(verifyUnionFind(ops9) == true);

    return 0;
}
// The task requires implementing a disjoint-set forest with three operations: `MAKE` (create new singleton), `UNION` (merge by rank where rank is the size of the set, as the original code uses `rank` as a sum of sizes), and `FIND` (which also serves as a validation check). The key is that `FIND` operations must match the actual set membership at that point. The original snippet has a potential bug in `Union` because after the first `if` block, the second `if` condition `it2->second->rank > it1->second->rank` might be evaluated with updated ranks from the first block, but since the first block only executes when `rank1 >= rank2`, the second block is effectively for the case where `rank2 > rank1` before any modification. However, to be consistent with the original behavior, we need to replicate exactly: if `rank1 >= rank2`, point `node2` parent to `node1` and set `rank1 = rank1 + rank2`. Else (i.e., `rank2 > rank1`), point `node1` parent to `node2` and set `rank2 = rank1 + rank2`. But careful: after the first `if` modifies `rank1`, the second `if` checks `rank2 > rank1` which may now be false even if originally `rank2 > rank1`? Actually for simplicity, we can implement a cleaner version: determine which root has larger rank (size); if equal, attach second to first and increase first's rank by second's rank; if second larger, attach first to second and increase second's rank by first's rank. But the original snippet has a subtle bug: it uses `>=` in first condition and then `>` in second, but after first condition executes, it modifies ranks, so the second condition could accidentally trigger incorrectly if the first condition updated ranks such that the second condition becomes true. For example, if initial ranks are `r1=2, r2=1`, first condition true, we set `parent2=node1`, `r1=3`, then second condition checks `r2 > r1`? `r2` is still 1, 1 > 3 false, so fine. If `r1=1, r2=2`, first condition false (1>=2 false), then second condition true, we set `parent1=node2`, `r2=1+2=3`. That's fine. Since the conditions are mutually exclusive in the original order (as long as we don't recompute the iterators), it works. However, there's a subtle issue: after the first `if` block, we modify `it1->second->rank` and `it2->second->parent`, but `it2->second->rank` remains unchanged. Then the second `if` checks `it2->second->rank > it1->second->rank`. If originally `r1 >= r2`, then after updating `r1 = r1 + r2`, now `r2 > r1` is impossible because `r2 <= r1 < r1+r2`. So it's safe. If originally `r1 < r2`, first if false, second if true, and we update. So the logic is actually correct. For the task, we will replicate this behavior.
//
// Important edge cases: `FIND` on a never-created element must return `false` (invalid operation). `UNION` on a never-created element must return `false`. `MAKE` on an existing element must return `false`. Also, `FIND x y` must compare the actual roots of the two elements; if the operation claims `true` (same) but actual roots differ, return `false`; if operation claims `false` but roots same, return `false`. Also, we must handle path compression in `find` to mimic the original's recursive/iterative path compression (the original prints during find, but we won't print). For complexity: `MAKE` is O(1), `UNION` and `FIND` are O(alpha(n)) amortized due to path compression and union by rank. Overall time O(total operations * alpha(n)). Space O(n) for the forest.
