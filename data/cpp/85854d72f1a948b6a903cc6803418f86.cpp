/*
Write a C++ function that simulates a filtering system for a database of records, where each record is a row of exactly 3 string fields. The function takes an initial vector of records (each represented as `std::vector<std::string>` of size 3) and a series of query steps. Each query step consists of 3 strings, where `"-"` acts as a wildcard matching any value in that field, and any other string must exactly match the field value at that column index. The filtering is sequential: after applying a query step, only records that matched all fields of that step (based on the wildcard rules) survive, and the next step applies only to those surviving records. The function should return a vector of integers, where the i-th integer is the number of records remaining after applying the i-th query step. Assume the initial record list is non-empty, and all input strings are non-empty (except the wildcard `"-"`). For example, if the initial records are `[["a","b","c"], ["a","x","c"], ["d","b","c"]]` and the first query is `["a","-","c"]`, the remaining count is 2 (records 1 and 2). If the second query is `["-","b","-"]`, applied to those 2 survivors, only record 1 remains, so the result is `[2, 1]`.
*/

#include <vector>
#include <string>

// Count the number of records surviving each sequential filtering step.
// Each record is a vector of 3 strings. Each query step is also a vector of 3 strings,
// where "-" acts as a wildcard matching any value in that column.
std::vector<int> countSurvivors(const std::vector<std::vector<std::string>>& records,
                                const std::vector<std::vector<std::string>>& queries) {
    std::vector<int> result;
    result.reserve(queries.size());

    for (const auto& query : queries) {
        // Start with a copy of all records for this query step.
        std::vector<std::vector<std::string>> current = records;

        // Apply filters column by column.
        for (int col = 0; col < 3; ++col) {
            const std::string& filter = query[col];
            std::vector<std::vector<std::string>> next;
            next.reserve(current.size());

            for (const auto& record : current) {
                if (filter == "-" || record[col] == filter) {
                    next.push_back(record);
                }
            }

            current = std::move(next);
        }

        result.push_back(static_cast<int>(current.size()));
    }

    return result;
}

#include <cassert>
#include <vector>
#include <string>

// The solution function is declared here (or included from a header).
std::vector<int> countSurvivors(const std::vector<std::vector<std::string>>& records,
                                const std::vector<std::vector<std::string>>& queries);

int main() {
    // Example from the snippet: 3 records, 2 queries.
    std::vector<std::vector<std::string>> records = {
        {"a", "b", "c"},
        {"a", "x", "c"},
        {"d", "b", "c"}
    };
    std::vector<std::vector<std::string>> queries = {
        {"a", "-", "c"},
        {"-", "b", "-"}
    };
    std::vector<int> expected = {2, 1};
    assert(countSurvivors(records, queries) == expected);

    // Single record and no wildcards.
    records = {{"x", "y", "z"}};
    queries = {{"x", "y", "z"}};
    expected = {1};
    assert(countSurvivors(records, queries) == expected);

    // Single record, query forces zero survivors.
    queries = {{"x", "y", "w"}};
    expected = {0};
    assert(countSurvivors(records, queries) == expected);

    // All wildcards match everything.
    records = {{"1", "2", "3"}, {"4", "5", "6"}};
    queries = {{"-", "-", "-"}};
    expected = {2};
    assert(countSurvivors(records, queries) == expected);

    // Sequential filtering with empty intermediate list.
    records = {{"a", "b", "c"}, {"d", "e", "f"}};
    queries = {
        {"-", "-", "-"},   // 2 survivors
        {"a", "-", "-"},   // 1 survivor
        {"-", "x", "-"}    // 0 survivors
    };
    expected = {2, 1, 0};
    assert(countSurvivors(records, queries) == expected);

    // Check that the original records are not modified (const reference).
    records = {{"a", "b", "c"}};
    queries = {{"a", "b", "c"}, {"-", "-", "-"}};
    expected = {1, 1};
    assert(countSurvivors(records, queries) == expected);
    assert(records[0][0] == "a"); // unchanged

    return 0;
}

// The problem is a faithful re-implementation of the logic in the provided snippet, but generalized as a free function. The core algorithm: for each query step, maintain a candidate list (initially the full original record list). For each of the 3 fields (columns) in the query step, filter the current candidate list by keeping only records where the query field equals `"-"` or equals the record's field value at that index. This is done iteratively: start with `tmp1` = current candidates, then for each column index `j` from 0 to 2, iterate over the query’s field string `s` (which is read one at a time), build a new temporary vector `tmp2` of records that match at column `j`, then replace `tmp1` with `tmp2`. After processing all 3 fields, the size of `tmp1` is the number of survivors for that query step. Important edge cases: if at any point the candidate list becomes empty, subsequent filters will still run but remain empty (size 0); the wildcard `"-"` must match every record regardless of field value; the initial record list must not be modified, so we copy it at the start of each query step. Time complexity: for each query step, we process up to 3 fields, and for each field we scan the current candidate list (which at most is the original `n` records). So each query step is O(3 * n) = O(n), and for `m` steps the total is O(n*m). Space complexity is O(n) for the temporary vectors per step, plus the input storage. The function should be `const`-correct: accept the record list as `const std::vector<std::vector<std::string>>&` and each query as `const std::vector<std::vector<std::string>>&` (where each inner vector is the 3 query strings). Return `std::vector<int>`.
