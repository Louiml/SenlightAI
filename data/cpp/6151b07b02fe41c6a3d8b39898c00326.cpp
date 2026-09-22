Write a C++ function `string smallestEquivalentString(string s1, string s2, string baseStr)` that takes two strings `s1` and `s2` of equal length (each containing only lowercase English letters) and a third string `baseStr` (also lowercase). The function must return a string of the same length as `baseStr` where each character is replaced with the smallest lexicographical character that is in its equivalence class. Two characters are considered equivalent if they appear at the same index in `s1` and `s2`, and equivalence is transitive (e.g., if `a`~`b` and `b`~`c`, then `a`~`c`). The output must replace each character in `baseStr` with the minimum character (by alphabetical order) within its equivalence class. Assume all strings are non-empty and contain only lowercase letters.

// The problem is a classic application of the Union-Find (Disjoint Set Union) data structure. We have 26 possible characters (mapped to indices 0–25). For each index `i` in `s1` and `s2`, we perform a union operation between the two characters at that position, so that all characters that are transitively linked become part of the same connected component. After processing all pairs, for each character in `baseStr`, we find its root (representative) in the union-find structure and output the character corresponding to that root. To ensure the output is the smallest character in each equivalence class, we need the union operation to always choose the smaller index as the root. The given code snippet uses a union-find where the root is always the smaller index (it checks `if (Px > Py)` and attaches the larger to the smaller). However, there is a subtle issue: the `findP` function returns the root with path compression, but if the root is not always the minimum (e.g., if we used a different union rule), we would need to track minima separately. Since the snippet’s union always sets the smaller index as the parent, the root after all unions is guaranteed to be the minimum index in the component. Therefore, the final replacement uses `findP` directly. Edge cases: when `s1` and `s2` have length 0 (not allowed per task), when `baseStr` is empty (not allowed), duplicate pairs are handled naturally (union of already-connected nodes does nothing). The algorithm runs in O((|s1| + |baseStr|) * α(26)) time, where α is the inverse Ackermann function (effectively constant for 26 elements), and O(26) space. In practice, we can treat this as O(|s1| + |baseStr|) time and O(1) extra space.

#include <string>
#include <vector>

class UnionFind {
public:
    explicit UnionFind(int n) : parent(n), size(n, 1) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        // Always attach larger root to smaller root so the smaller remains the root.
        if (a > b) std::swap(a, b);
        parent[b] = a;
        size[a] += size[b];
    }
private:
    std::vector<int> parent;
    std::vector<int> size;
};

// Returns the lexicographically smallest equivalent string for baseStr.
std::string smallestEquivalentString(const std::string& s1, const std::string& s2, const std::string& baseStr) {
    UnionFind uf(26);
    for (std::size_t i = 0; i < s1.size(); ++i) {
        uf.unite(s1[i] - 'a', s2[i] - 'a');
    }
    std::string result;
    result.reserve(baseStr.size());
    for (char c : baseStr) {
        int root = uf.find(c - 'a');
        result.push_back(static_cast<char>('a' + root));
    }
    return result;
}

#include <cassert>
#include <string>

// The solution function is declared above (assume it is included from the Solution section).
int main() {
    // Basic example from the snippet.
    assert(smallestEquivalentString("parker", "morris", "parser") == "makkek");
    // Another small test.
    assert(smallestEquivalentString("hello", "world", "hold") == "hdld");
    // All characters equivalent to 'a'.
    assert(smallestEquivalentString("abc", "bca", "xyz") == "aaa");
    // Single character strings.
    assert(smallestEquivalentString("a", "b", "c") == "a");
    // No equivalence pairs, base unchanged.
    assert(smallestEquivalentString("ab", "cd", "ef") == "ef");
    // Transitivity example.
    assert(smallestEquivalentString("ab", "bc", "ac") == "aa");
    // Repeated indices.
    assert(smallestEquivalentString("aa", "bb", "ab") == "aa");
    // Larger string.
    assert(smallestEquivalentString("leetcode", "programs", "sourcecode") == "aauaaaaada");
    // All same characters.
    assert(smallestEquivalentString("zzzz", "zzzz", "zz") == "zz");
    return 0;
}
