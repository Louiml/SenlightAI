// Write a standalone C++ function `string solution(const string& s)` that takes a lowercase string `s` consisting of letters `'a'`–`'z'` and determines whether it is possible to arrange all 26 letters of the alphabet in a sequence such that every pair of adjacent characters in the given string `s` appears as adjacent characters (in either order) in that arrangement. If such an arrangement exists, return the string `"YES"` followed by a newline then a valid arrangement of all 26 letters as a single string. If no arrangement exists, return just `"NO"`. The function should operate under the constraint that each letter can appear at most twice as a neighbor in the final arrangement (since it has two adjacent positions), and the graph formed by treating each letter as a vertex and each required adjacency as an undirected edge must be a simple path covering all vertices that appear in the input, with all other letters appended arbitrarily at the end. The input string `s` has length at least 1 and at most 10^5. The function must be self-contained and include all needed headers.
// Treat each distinct character in `s` as a node in an undirected graph. For every adjacent pair in `s`, add an undirected edge between those two characters. The key observation is that the final arrangement of the 26 letters is a sequence where each character (except the two endpoints) has exactly two neighbors in the arrangement. Thus, the required edges from `s` must form a set of paths (not cycles or branches) that can be concatenated into a single line. Specifically, the graph must have maximum degree ≤ 2 and must be a simple path (i.e., the number of vertices with degree 1 must be exactly 2, unless there are no edges, in which case any arrangement works). If the graph has a vertex with degree > 2, or the number of degree-1 vertices is not exactly 2 (and there is at least one edge), then no valid arrangement exists. When a valid path exists, we can find one endpoint (a degree-1 vertex), traverse the graph to collect the path, then append all other letters in alphabetical order. Edge cases: if `s` has length 1, it’s always valid; if a character appears only once and has no neighbors, it is placed at the end. The time complexity is O(26 + |s|) since we process each edge once and the graph has at most 26 nodes. Space complexity is O(26) for adjacency sets.
#include <bits/stdc++.h>
using namespace std;

// Determine if a valid arrangement exists and return "YES\n<arrangement>" or "NO".
string solution(const string& s) {
    const int ALPHA = 26;
    vector<set<int>> adj(ALPHA);
    int n = (int)s.size();

    // Build undirected graph from adjacent pairs.
    for (int i = 0; i + 1 < n; ++i) {
        int u = s[i] - 'a';
        int v = s[i + 1] - 'a';
        adj[u].insert(v);
        adj[v].insert(u);
    }

    // Check degree constraints: no vertex can have more than 2 neighbors.
    for (int i = 0; i < ALPHA; ++i) {
        if ((int)adj[i].size() > 2) {
            return "NO";
        }
    }

    // Count vertices with degree 1 (endpoints of paths).
    int degreeOne = 0;
    for (int i = 0; i < ALPHA; ++i) {
        if (adj[i].size() == 1) {
            ++degreeOne;
        }
    }

    // If there are edges, we need exactly 2 endpoints (a single path).
    bool hasEdge = false;
    for (int i = 0; i < ALPHA; ++i) {
        if (!adj[i].empty()) {
            hasEdge = true;
            break;
        }
    }
    if (hasEdge && degreeOne != 2) {
        return "NO";
    }

    // Build the arrangement.
    vector<int> arrangement;

    // If there are edges, find a starting endpoint with degree 1.
    int start = -1;
    for (int i = 0; i < ALPHA; ++i) {
        if (adj[i].size() == 1) {
            start = i;
            break;
        }
    }

    if (start == -1) {
        // No edges: any order works; place all letters alphabetically.
        for (int i = 0; i < ALPHA; ++i) {
            arrangement.push_back(i);
        }
    } else {
        // Traverse the path.
        int prev = -1;
        int cur = start;
        while (true) {
            arrangement.push_back(cur);
            int next = -1;
            for (int nb : adj[cur]) {
                if (nb != prev) {
                    next = nb;
                    break;
                }
            }
            if (next == -1) {
                break;
            }
            prev = cur;
            cur = next;
        }
        // Add all letters not in the path (they have degree 0).
        vector<bool> inPath(ALPHA, false);
        for (int v : arrangement) {
            inPath[v] = true;
        }
        for (int i = 0; i < ALPHA; ++i) {
            if (!inPath[i]) {
                arrangement.push_back(i);
            }
        }
    }

    // Build result string.
    string result = "YES\n";
    for (int v : arrangement) {
        result.push_back(char('a' + v));
    }
    return result;
}
#include <cassert>
#include <string>
using namespace std;

// Declaration of the solution function (should match the provided implementation).
string solution(const string& s);

int main() {
    // Single character: always valid, any arrangement works (check prefix "YES\n" and length 27).
    string res1 = solution("a");
    assert(res1.rfind("YES\n", 0) == 0);
    assert(res1.size() == 27); // "YES\n" + 26 letters

    // Two different letters: path exists, arrangement must contain those two adjacent.
    string res2 = solution("ab");
    assert(res2.substr(0, 3) == "YES");
    assert(res2.find('a') != string::npos);
    assert(res2.find('b') != string::npos);
    assert(abs((int)(res2.find('a') - res2.find('b'))) == 1);

    // Three letters forming a path: should be valid.
    string res3 = solution("abc");
    assert(res3.substr(0, 3) == "YES");

    // Three letters forming a star (center connected to two leaves): degree 3 for center -> NO.
    string res4 = solution("abac");
    assert(res4 == "NO");

    // Cycle of three (a-b, b-c, c-a): no degree-1 vertices, but cycle is invalid for a line -> NO.
    string res5 = solution("abcbca");
    assert(res5 == "NO");

    // Simple path with extra isolated letters: valid.
    string res6 = solution("zyx");
    assert(res6.substr(0, 3) == "YES");

    // All same letter: no distinct edges, degree 0 for all, valid.
    string res7 = solution("zzzz");
    assert(res7.substr(0, 3) == "YES");
    assert(res7.size() == 27);

    // Path of length 4: should be valid.
    string res8 = solution("dcba");
    assert(res8.substr(0, 3) == "YES");

    // Disconnected edges (two separate edges): degree-one count = 4, invalid.
    string res9 = solution("ab cd"); // But input has no spaces; use "abcd" gives path, so make two separate: "ab" and "cd" not in same string. Use "abdec"? Let's craft "abdc" gives path, but "ab" and "cd" as "ab"+"cd" = "abcd" is path. To get two edges with no connection, use "ab" and "cd" as "ab"+"cd" but that's a path. So we need a string like "ab" and "cd" not connected: "ab" then "cd" cannot be in one string without connection? Actually "abcd" is path. To get disconnected, we can have "a b" but no spaces, so use "ab" and "c d" but need a letter between? Not possible. So skip this test or use "axb" gives edges a-x, x-b -> path. Use "ab"+"cd" is "abcd" path. So instead use "ab" and "cd" as "ab"+"cd" is path, but "ab" + "cd" = "abcd" path. To get two separate edges, we need a string like "ab" and "cd" where there is no vertex connecting them, but in a single string they must be separated by something? Actually "ab" then "cd" is "abcd" which connects them. So to have disconnected we need four distinct letters with edges (a,b) and (c,d) but no edge between b and c. That would require a string like "ab" then "cd" but "abcd" gives edge b-c. So impossible in a single string? Actually we can have "ab" and then "cd" but we need a separator that is a letter, but that would create an edge. So the only way is if the string is "ab" and then "cd" is impossible because they'd be adjacent. So skip this case. Instead use a string with a vertex of degree 2 and another of degree 2 but disconnected? Hard. We'll just test a case with degree 2 but not a path: e.g., "aba" gives edges a-b, b-a (same), degree of a=1, b=1, so it's just an edge and is valid? Actually "aba" edges: a-b and b-a, same edge, degree both 1, valid. "abca" gives a-b, b-c, c-a -> triangle, degree all 2, no degree 1, invalid. We already have that.
    string res10 = solution("abca");
    assert(res10 == "NO");

    return 0;
}
