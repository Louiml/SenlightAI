/*
Write a C++ function named `intersectionSorted` that takes two whitespace-separated strings of integers, `firstLine` and `secondLine`, and returns a `std::vector<int>` containing the intersection of the two sets of numbers (i.e., integers that appear in both lines), sorted in ascending order. Each integer in the result must appear exactly once, even if it occurs multiple times in either input. The input strings may contain leading/trailing whitespace, multiple spaces between numbers, negative numbers, and duplicate values. The function should handle empty inputs gracefully and return an empty vector if there is no common element.
*/

#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <algorithm>

// Return the sorted intersection of integers from two whitespace-separated strings.
std::vector<int> intersectionSorted(const std::string& firstLine, const std::string& secondLine) {
    std::set<int> firstSet;
    std::istringstream firstStream(firstLine);
    int value;
    while (firstStream >> value) {
        firstSet.insert(value);
    }

    std::vector<int> result;
    std::istringstream secondStream(secondLine);
    while (secondStream >> value) {
        if (firstSet.find(value) != firstSet.end()) {
            result.push_back(value);
        }
    }

    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Forward declaration from solution
std::vector<int> intersectionSorted(const std::string&, const std::string&);

int main() {
    assert(intersectionSorted("1 2 3", "2 3 4") == std::vector<int>({2, 3}));
    assert(intersectionSorted("1 2 2 3", "2 2 3 3") == std::vector<int>({2, 3}));
    assert(intersectionSorted("", "1 2") == std::vector<int>{});
    assert(intersectionSorted("  5   -1   10  ", "-1 10 10 5") == std::vector<int>({-1, 5, 10}));
    assert(intersectionSorted("7 8", "9 10") == std::vector<int>{});
    assert(intersectionSorted("100", "100") == std::vector<int>({100}));
    assert(intersectionSorted("-3 -1 -1 0", "-3 -3 0 0") == std::vector<int>({-3, 0}));
    assert(intersectionSorted("1 2 3 4 5", "5 4 3 2 1") == std::vector<int>({1, 2, 3, 4, 5}));
    assert(intersectionSorted("0 0 0", "0") == std::vector<int>({0}));
}

// The task mirrors the provided snippet’s behavior but wraps it in a reusable function. The main algorithm: parse all integers from the first line into a `std::set<int>` (which automatically sorts and deduplicates). Then parse the second line, and for each integer, check if it exists in the set; if so, push it into a result vector. After parsing, sort the result vector to ensure ascending order, then remove duplicates using `std::unique` (or rely on the fact that we only add once per distinct value—but duplicates in the second line could appear multiple times, so we need deduplication). A simpler approach: after collecting all candidates, sort and then erase unique duplicates via `std::unique`. Edge cases: empty strings, all duplicates, no intersection, negative numbers, extra whitespace. Time complexity: parsing each number is O(N + M) where N and M are counts of integers in the two strings; inserting into set is O(N log N); sorting candidates is O(K log K) where K is number of matches; overall O(N log N + M log M) worst-case, but typical O(N log N + M). Space complexity: O(N) for the set plus O(K) for the result vector.
