Given integers \( n \) and \( q \) followed by \( q \) lines each containing two integers \( x, y \) and a word `"same"` or `"different"`, write a C++ function `int solveRelation(int n, const std::vector<std::tuple<int,int,std::string>>& queries)` that determines whether there exists an assignment of two distinct colors (say color 0 and color 1) to each of \( n \) items such that for every query:
- if the word is `"same"`, items \( x \) and \( y \) must have the same color;
- if the word is `"different"`, items \( x \) and \( y \) must have different colors.
If no valid assignment exists, return `-1`. Otherwise, return the maximum possible size of one color class (i.e., the maximum number of items that can all be assigned the same color) over all valid assignments. The constraints: \( 1 \le n \le 10^5 \), \( 0 \le q \le 10^5 \), and \( 1 \le x, y \le n \). Note that each pair may appear multiple times, and words are given in lowercase.
This is a 2-SAT or bipartite consistency problem. Model each item \( i \) by two DSU nodes: \( i \) (meaning “item \( i \) is in class A”) and \( i+n \) (meaning “item \( i \) is in class B”). For a `same` query, unite \((x, y)\) and \((x+n, y+n)\) because if \( x \) is in class A then \( y \) must be in class A, and if \( x \) is in class B then \( y \) must be in class B. For a `different` query, unite \((x, y+n)\) and \((x+n, y)\). After processing all queries, for each item \( i \), if find(i) == find(i+n), then forcing \( i \) to be A also forces it to be B — a contradiction, so return -1. Otherwise, each connected component in the DSU represents a group of item–class assignments that must all be true together. For each item \( i \), consider the component containing \( i \) and the component containing \( i+n\): these two are complementary classes for item \( i \). In any consistent assignment, we must choose exactly one of these two complementary components to be the “A” side and the other to be the “B” side. To maximize the size of the larger color class, for each pair of complementary components we add the larger of their sizes. We iterate over all items, but only consider a component once by checking if find(i) == i (i.e., the component root). The size of a component is maintained in `sc` during union. Time complexity is \( O((n+q) \alpha(n)) \) due to DSU operations; space complexity is \( O(n) \).
#include <bits/stdc++.h>
using namespace std;

struct RelationDSU {
    vector<int> parent, rank, size;
    RelationDSU(int n) : parent(2*n+1), rank(2*n+1, 1), size(2*n+1, 0) {
        for (int i = 1; i <= n; ++i) {
            parent[i] = i;
            parent[i+n] = i+n;
            size[i+n] = 1;
        }
    }
    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }
    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (rank[a] < rank[b]) swap(a, b);
        parent[b] = a;
        rank[a] += rank[b];
        size[a] += size[b];
    }
};

int solveRelation(int n, const vector<tuple<int,int,string>>& queries) {
    RelationDSU dsu(n);
    for (const auto& [x, y, word] : queries) {
        if (word == "same") {
            dsu.unite(x, y);
            dsu.unite(x+n, y+n);
        } else {
            dsu.unite(x, y+n);
            dsu.unite(x+n, y);
        }
    }
    for (int i = 1; i <= n; ++i) {
        if (dsu.find(i) == dsu.find(i+n)) return -1;
    }
    int answer = 0;
    for (int i = 1; i <= n; ++i) {
        if (dsu.find(i) == i) {
            int compA = dsu.find(i);
            int compB = dsu.find(i+n);
            answer += max(dsu.size[compA], dsu.size[compB]);
        }
    }
    return answer;
}
#include <bits/stdc++.h>
#include <cassert>
using namespace std;

int solveRelation(int n, const vector<tuple<int,int,string>>& queries);

int main() {
    // Example 1: two items, must be different, so one color has 1, other has 1 => max size = 1
    vector<tuple<int,int,string>> q1 = {{1,2,"different"}};
    assert(solveRelation(2, q1) == 1);

    // Example 2: three items all same, so all 3 can be one color
    vector<tuple<int,int,string>> q2 = {{1,2,"same"},{2,3,"same"}};
    assert(solveRelation(3, q2) == 3);

    // Example 3: contradiction: 1 same as 2 but different from 3 and 2 same as 3
    vector<tuple<int,int,string>> q3 = {{1,2,"same"},{2,3,"same"},{1,3,"different"}};
    assert(solveRelation(3, q3) == -1);

    // Example 4: no queries, 4 items, we can put all 4 together
    vector<tuple<int,int,string>> q4 = {};
    assert(solveRelation(4, q4) == 4);

    // Example 5: chain of different: 1-2, 2-3 => colors: 1=0,2=1,3=0 => max class size = 2
    vector<tuple<int,int,string>> q5 = {{1,2,"different"},{2,3,"different"}};
    assert(solveRelation(3, q5) == 2);

    // Example 6: duplicate same and different
    vector<tuple<int,int,string>> q6 = {{1,2,"same"},{1,2,"same"},{2,3,"different"},{1,3,"different"}};
    assert(solveRelation(3, q6) == 2);

    // Example 7: self same (absorbed)
    vector<tuple<int,int,string>> q7 = {{1,1,"same"}};
    assert(solveRelation(1, q7) == 1);

    // Example 8: self different is contradiction
    vector<tuple<int,int,string>> q8 = {{1,1,"different"}};
    assert(solveRelation(1, q8) == -1);

    // Example 9: two disconnected groups, each group internally different
    // group1: 1-2 different, group2: 3-4 different => max total = 1+1 = 2
    vector<tuple<int,int,string>> q9 = {{1,2,"different"},{3,4,"different"}};
    assert(solveRelation(4, q9) == 2);

    // Example 10: complex mixed, 2n up to 100000 correctness check for small random
    // here we just test a known small case
    vector<tuple<int,int,string>> q10 = {{1,2,"different"},{2,3,"same"},{3,4,"different"}};
    // colors: 1=0,2=1,3=1,4=0 => class sizes 2 and 2 => max = 2
    assert(solveRelation(4, q10) == 2);

    cout << "All tests passed!\n";
    return 0;
}
