// Given a 2D vector of strings representing a database relation (each row is a tuple, each column is an attribute), write a C++ function that returns the number of candidate keys. A candidate key is a minimal set of columns such that no two rows have identical values across all columns in the set (i.e., the set uniquely identifies each row), and no proper subset of that set also uniquely identifies the rows. The function must consider all subsets of columns of size 1 up to the total number of columns and count each distinct minimal unique column combination exactly once, even if multiple subsets have the same uniqueness property. The input relation is non-empty, has at least one row, and each row has the same number of columns (at least one). Column values are arbitrary non-empty strings; duplicates are possible within a column. The output is an integer count of candidate keys.
#include <cassert>
#include <vector>
#include <string>

int main() {
    // Test 1: Classic example - columns 0 and 1 together are unique, but column 0 alone is unique.
    {
        vector<vector<string>> rel = {
            {"a", "1", "x"},
            {"b", "1", "y"},
            {"c", "2", "z"}
        };
        // Column 0 alone is unique (a,b,c), so it's the only candidate key.
        assert(countCandidateKeys(rel) == 1);
    }

    // Test 2: No single column is unique, but columns 0 and 1 together are unique.
    {
        vector<vector<string>> rel = {
            {"a", "1", "x"},
            {"a", "2", "y"},
            {"b", "1", "z"}
        };
        // Column 0 alone: a,a,b -> not unique. Column 1 alone: 1,2,1 -> not unique. Column 2 alone: x,y,z -> unique? no, needs all columns? Let's check: col2 = x,y,z all distinct => unique, size 1! So actually col2 is a key. But minimal? Yes. So ans=1.
        // Correct: col2 is unique, no smaller subset (size 1) is unique? col0 and col1 are not unique. So 1.
        assert(countCandidateKeys(rel) == 1);
    }

    // Test 3: No combination is unique until all columns are used.
    {
        vector<vector<string>> rel = {
            {"a", "1"},
            {"a", "1"},
            {"b", "2"}
        };
        // Single column: col0 has a,a,b -> not unique; col1 has 1,1,2 -> not unique.
        // Two columns: (a,1), (a,1), (b,2) -> first two rows identical -> still not unique. So no key, ans=0.
        assert(countCandidateKeys(rel) == 0);
    }

    // Test 4: Multiple candidate keys - column 0 unique and column 1 unique.
    {
        vector<vector<string>> rel = {
            {"a", "1", "x"},
            {"b", "2", "y"},
            {"c", "3", "z"}
        };
        // Both col0 and col1 are unique individually, both size 1, so both are candidate keys. ans=2.
        assert(countCandidateKeys(rel) == 2);
    }

    // Test 5: Candidate key of size 2, but a subset of size 1 is not unique.
    {
        vector<vector<string>> rel = {
            {"a", "1", "x"},
            {"a", "2", "y"},
            {"b", "1", "x"}
        };
        // col0: a,a,b -> not unique; col1: 1,2,1 -> not unique; col2: x,y,x -> not unique.
        // pairs: (0,1): (a,1),(a,2),(b,1) -> unique, size 2; (0,2): (a,x),(a,y),(b,x) -> unique, size 2; (1,2): (1,x),(2,y),(1,x) -> duplicate (1,x) appears twice? rows 0 and 2 have (1,x) and (1,x) -> not unique.
        // So keys: {0,1} and {0,2}? But {0,1} minimal? Yes, no subset of size 1 is unique. So both are candidate keys. ans=2.
        assert(countCandidateKeys(rel) == 2);
    }

    // Test 6: Single row - any non-empty set is unique, but minimal size is 1.
    {
        vector<vector<string>> rel = {
            {"only", "row", "data"}
        };
        // Columns 0,1,2 each alone are unique because there is only one row. Since we process size 1 first, we count each single column? Actually each single column is a candidate key. So ans = C = 3.
        assert(countCandidateKeys(rel) == 3);
    }

    // Test 7: Relation with all identical rows - no key unless all columns? Actually no combination is unique because all rows identical. So ans=0.
    {
        vector<vector<string>> rel = {
            {"same", "same"},
            {"same", "same"},
            {"same", "same"}
        };
        assert(countCandidateKeys(rel) == 0);
    }

    // Test 8: Larger relation with a key of size 2 and no smaller key.
    {
        vector<vector<string>> rel = {
            {"1", "a", "p"},
            {"2", "a", "q"},
            {"3", "b", "p"},
            {"4", "b", "q"}
        };
        // col0 unique (1,2,3,4) => size 1 key exists, so ans=1 (col0). Actually col0 alone is unique, so ans=1.
        assert(countCandidateKeys(rel) == 1);
    }

    // Test 9: Key of size 2 where one column is not unique but the other is unique? Actually if any single column is unique, it's a key.
    {
        vector<vector<string>> rel = {
            {"x", "1"},
            {"y", "1"},
            {"z", "2"}
        };
        // col0 unique, col1 not unique. So ans=1.
        assert(countCandidateKeys(rel) == 1);
    }

    // Test 10: Ensure that superset of a found key is not counted.
    {
        vector<vector<string>> rel = {
            {"a", "1", "x"},
            {"b", "1", "y"},
            {"c", "2", "z"}
        };
        // As before col0 unique, so {0} counts. {0,1} and {0,2} and {0,1,2} are supersets and must not count. So ans=1.
        assert(countCandidateKeys(rel) == 1);
    }

    return 0;
}
#include <string>
#include <vector>
#include <set>
#include <utility>

using namespace std;

// Count the number of candidate keys in a relation.
// A candidate key is a minimal set of columns with unique combinations across all rows.
int countCandidateKeys(const vector<vector<string>>& relation) {
    const int R = relation.size();
    const int C = relation[0].size();
    int ans = 0;

    vector<int> picked;
    vector<pair<int, int>> already; // (bitmask, number of columns)

    // Check if the currently picked columns form a unique combination.
    auto isUnique = [&]() -> bool {
        set<vector<string>> seen;
        for (int i = 0; i < R; ++i) {
            vector<string> tmp;
            for (int j = 0; j < (int)picked.size(); ++j) {
                tmp.push_back(relation[i][picked[j]]);
            }
            if (!seen.insert(tmp).second) {
                return false;
            }
        }
        return true;
    };

    // Depth-first search to generate subsets of a given size.
    // start: first column index to consider
    // cnt: target subset size
    function<void(int, int)> dfs = [&](int start, int cnt) {
        if ((int)picked.size() == cnt) {
            // Build bitmask for current subset.
            int now = 0;
            for (int idx : picked) {
                now |= (1 << idx);
            }
            // Skip if this subset is a superset of an existing candidate key.
            for (const auto& key : already) {
                if ((key.first & now) == key.first && key.second < cnt) {
                    return;
                }
            }
            if (isUnique()) {
                ++ans;
                already.push_back({now, cnt});
            }
            return;
        }
        for (int col = start; col < C; ++col) {
            picked.push_back(col);
            dfs(col + 1, cnt);
            picked.pop_back();
        }
    };

    // Consider subsets of increasing size.
    for (int size = 1; size <= C; ++size) {
        picked.clear();
        dfs(0, size);
    }

    return ans;
}
// The solution enumerates all subsets of columns in increasing order of size (from 1 to C, the number of columns). For each subset, we first check if it contains any previously identified candidate key as a proper subset. This is done by bitmask representation: each column corresponds to a bit; a subset `now` is skipped if there exists a previously found candidate key with bitmask `already[i].first` such that `(already[i].first & now) == already[i].first` and `already[i].second < picked.size()`. This ensures minimality—we only process subsets that are not supersets of a smaller candidate key. Then we test uniqueness by building a temporary vector of strings for each row consisting of values at the selected columns and inserting them into a `set`. If the set size equals the number of rows, the subset is unique; we increment the answer and record it in the `already` list. The enumeration uses depth-first search with backtracking over column indices. Edge cases include a relation with a single row (any non-empty column set is a key, but only the smallest such set counts, which is size 1), and columns with duplicate values across all rows (no key may exist if no combination is unique; in that case the answer is 0). Time complexity is \(O(2^C \cdot R \cdot C)\) in the worst case because we generate all subsets and for each check rows with up to C values; space complexity is \(O(2^C \cdot C + R \cdot C)\) for storing candidate keys and the temporary row vectors, but practically the set stores at most R entries per check.
