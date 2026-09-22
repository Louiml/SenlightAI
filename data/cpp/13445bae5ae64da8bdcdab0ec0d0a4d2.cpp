// Write a standalone C++ function `vector<int> getIntersectionFromFile(const string& filename)` that reads two lines of integers from a text file. The first line contains an integer `n` followed by `n` integers, and the second line contains an integer `m` followed by `m` integers. The function must return a sorted vector containing the distinct integers that appear in both lists (i.e., the set intersection). If there is no common integer, return an empty vector. Assume the file exists and is correctly formatted. The integers may be negative, zero, or positive, and duplicates within each list should be ignored.

#include <cassert>
#include <fstream>
#include <vector>

// Function to write a test file
void writeTestFile(const std::string& filename, const std::string& content) {
    std::ofstream out(filename);
    out << content;
}

int main() {
    // Test 1: Basic intersection
    writeTestFile("test1.txt", "4 1 2 3 4\n3 3 4 5");
    assert((getIntersectionFromFile("test1.txt") == std::vector<int>{3, 4}));

    // Test 2: No intersection
    writeTestFile("test2.txt", "3 10 20 30\n2 5 6");
    assert((getIntersectionFromFile("test2.txt") == std::vector<int>{}));

    // Test 3: Duplicates in input, negative numbers
    writeTestFile("test3.txt", "5 -1 -1 0 2 2\n4 -2 -1 0 4");
    assert((getIntersectionFromFile("test3.txt") == std::vector<int>{-1, 0}));

    // Test 4: One list empty (n=0)
    writeTestFile("test4.txt", "0\n3 7 8 9");
    assert((getIntersectionFromFile("test4.txt") == std::vector<int>{}));

    // Test 5: Both lists identical
    writeTestFile("test5.txt", "3 1 2 3\n3 1 2 3");
    assert((getIntersectionFromFile("test5.txt") == std::vector<int>{1, 2, 3}));

    // Test 6: Single common element
    writeTestFile("test6.txt", "2 100 200\n3 200 300 400");
    assert((getIntersectionFromFile("test6.txt") == std::vector<int>{200}));

    // Clean up test files (optional but good practice)
    std::remove("test1.txt");
    std::remove("test2.txt");
    std::remove("test3.txt");
    std::remove("test4.txt");
    std::remove("test5.txt");
    std::remove("test6.txt");

    return 0;
}

#include <vector>
#include <set>
#include <fstream>

// Reads two lists of integers from a file and returns their sorted set intersection.
// File format: first line contains n followed by n integers, second line contains m followed by m integers.
std::vector<int> getIntersectionFromFile(const std::string& filename) {
    std::ifstream file(filename);
    int n, m;

    // Read first list
    file >> n;
    std::set<int> firstSet;
    int value;
    for (int i = 0; i < n; ++i) {
        file >> value;
        firstSet.insert(value);
    }

    // Read second list
    file >> m;
    std::set<int> secondSet;
    for (int i = 0; i < m; ++i) {
        file >> value;
        secondSet.insert(value);
    }

    // Iterate over smaller set to reduce number of lookups
    const std::set<int>& smaller = (firstSet.size() <= secondSet.size()) ? firstSet : secondSet;
    const std::set<int>& larger = (firstSet.size() <= secondSet.size()) ? secondSet : firstSet;

    std::vector<int> result;
    for (int x : smaller) {
        if (larger.find(x) != larger.end()) {
            result.push_back(x);
        }
    }
    return result;
}

// The main algorithm is straightforward: read all integers from each line into two separate `std::set<int>` containers, which automatically remove duplicates and keep elements sorted. Then iterate through the smaller set (or just one set) and check membership in the other set using `find()`. If an element is found in both, append it to the result vector. Because `std::set` stores elements in sorted order, the result vector will naturally be sorted if we traverse one set in order. Edge cases include: one list empty (if `n` or `m` is 0), no intersection (return empty vector), and negative numbers (handled naturally by the comparison operators). Time complexity is O(n log n + m log m) for building the sets, and O(min(n, m) * log(max(n, m))) for the intersection check. Space complexity is O(n + m) for storing the sets and the result.
