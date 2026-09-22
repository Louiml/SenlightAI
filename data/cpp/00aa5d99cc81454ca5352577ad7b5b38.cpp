// Write a C++ function `int minimumInfections(int n, int m, const std::vector<std::vector<int>>& languages)` that, given `n` people (numbered 0 to n-1, each speaking a list of languages from 1 to m, possibly empty), and `m` total languages, returns the minimum number of initially infected people needed so that, if infection spreads between any two people sharing at least one language (transitively through a chain of such pairs), eventually every person who speaks at least one language becomes infected. People who speak no language can never be infected, and they are ignored for the count. If all `n` people speak no language, return `0` (no one can be infected). The function should build a graph where an edge connects any two people who share a language, then find the number of connected components among people with at least one language; the answer is the number of such components, because infecting one person per component suffices. Handle the edge case where there are zero people with languages by returning `0`.
#include <cassert>
#include <vector>

int main() {
    // No one speaks any language -> 0
    {
        std::vector<std::vector<int>> langs = {{}, {}, {}};
        assert(minimumInfections(3, 5, langs) == 0);
    }
    // One person with a language, others none -> 1
    {
        std::vector<std::vector<int>> langs = {{1}, {}, {}};
        assert(minimumInfections(3, 5, langs) == 1);
    }
    // Two separate groups sharing no languages -> 2
    {
        std::vector<std::vector<int>> langs = {{1,2}, {1}, {3}, {3}};
        assert(minimumInfections(4, 3, langs) == 2);
    }
    // All connected via transitive sharing -> 1
    {
        std::vector<std::vector<int>> langs = {{1}, {1,2}, {2,3}, {3}};
        assert(minimumInfections(4, 3, langs) == 1);
    }
    // One language shared by all, but some have no language -> still 1 component among speakers
    {
        std::vector<std::vector<int>> langs = {{1}, {1}, {}, {1}};
        assert(minimumInfections(4, 1, langs) == 1);
    }
    // Two people each speak different single languages -> 2
    {
        std::vector<std::vector<int>> langs = {{1}, {2}};
        assert(minimumInfections(2, 2, langs) == 2);
    }
    // All people speak the same two languages -> 1
    {
        std::vector<std::vector<int>> langs = {{1,2}, {1,2}, {1,2}};
        assert(minimumInfections(3, 2, langs) == 1);
    }
    // Language with one speaker, language with another, but they share a third -> 1
    {
        std::vector<std::vector<int>> langs = {{1,3}, {2,3}};
        assert(minimumInfections(2, 3, langs) == 1);
    }
    // Multiple isolated single-language speakers -> n
    {
        std::vector<std::vector<int>> langs = {{1}, {2}, {3}, {4}};
        assert(minimumInfections(4, 4, langs) == 4);
    }
    // Mixed: some share, some don't -> count components among speakers
    {
        std::vector<std::vector<int>> langs = {{1,2}, {1}, {3}, {4}};
        assert(minimumInfections(4, 4, langs) == 3);
    }
    return 0;
}
#include <vector>
#include <functional>

// Returns the minimum number of initially infected people needed so that,
// through transitive sharing of languages, all people who speak at least one language become infected.
int minimumInfections(int n, int m, const std::vector<std::vector<int>>& languages) {
    // hasLang[i] = true if person i speaks at least one language
    std::vector<bool> hasLang(n, false);
    for (int i = 0; i < n; ++i) {
        if (!languages[i].empty()) {
            hasLang[i] = true;
        }
    }

    // If nobody speaks any language, no one can be infected.
    bool anyHasLang = false;
    for (bool b : hasLang) if (b) { anyHasLang = true; break; }
    if (!anyHasLang) return 0;

    // Build adjacency list: for each language, all speakers form a clique.
    std::vector<std::vector<int>> speakers(m + 1); // languages 1..m
    for (int person = 0; person < n; ++person) {
        for (int lang : languages[person]) {
            speakers[lang].push_back(person);
        }
    }

    std::vector<std::vector<int>> adj(n);
    for (int lang = 1; lang <= m; ++lang) {
        const auto& people = speakers[lang];
        for (size_t i = 0; i < people.size(); ++i) {
            for (size_t j = i + 1; j < people.size(); ++j) {
                int u = people[i];
                int v = people[j];
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }
    }

    // Count connected components among people with at least one language.
    std::vector<bool> visited(n, false);
    int comps = 0;
    std::function<void(int)> dfs = [&](int u) {
        visited[u] = true;
        for (int v : adj[u]) {
            if (!visited[v]) {
                dfs(v);
            }
        }
    };

    for (int i = 0; i < n; ++i) {
        if (hasLang[i] && !visited[i]) {
            dfs(i);
            ++comps;
        }
    }
    return comps;
}
// The core idea: each person is a node. For each language, all people who speak that language form a clique (they can all directly infect each other). Adding all edges within each language's clique yields a graph where connected components correspond to groups of people that are mutually reachable via shared-language chains. The minimum initial infections equals the number of connected components among people who speak at least one language, because one infection per component can spread to the whole component. People with no languages are isolated and can never be infected, so they are excluded. If there are no such people, return `0`.  
// Algorithm:  
// 1. Track a boolean array `hasLanguage` for each person.  
// 2. Build adjacency list: for each language (1..m), collect all people who speak it. For each pair in that list, add undirected edges. To avoid duplicate edges, we can simply push both directions; duplicates are harmless for DFS.  
// 3. Perform DFS over all `n` nodes, but only visit nodes that have at least one language. Count components.  
// 4. Return component count (or 0 if no one has a language).  
// Time complexity: For each language with `k` speakers, adding all pairs takes O(k^2). Worst-case if all people speak all languages, O(n^2 * m) but typical constraints are small. Total edges O(sum over languages of k^2). DFS is O(n + E). Space: O(n + E) for adjacency list and visited.  
// Edge cases: Empty language lists for some people; all empty; multiple languages shared; large m but small n.
