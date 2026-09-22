// Write a C++ function that reads a text file where each line contains exactly two integers separated by whitespace, stores all first-column values into a vector and all second-column values into another vector, then returns a `std::pair<long long, long long>` where the first element is the sum of absolute differences between the sorted first column and sorted second column (element-wise), and the second element is the sum of each value in the first column multiplied by the number of times that value appears in the second column. The function should take a filename as a `std::string` parameter and handle files with any number of lines (including zero) and any whitespace between the two integers on each line. Assume the integers fit in 64-bit signed range, and the file is well-formed (each line has exactly two integers). The function must not print anything.
// The solution involves reading the file line by line using a standard input file stream. For each line, we can parse the two integers using a `std::istringstream` to avoid issues with extra spaces or tabs. We store the first integer into vector `left` and the second into vector `right`. After reading all lines, we sort both vectors in ascending order using `std::sort`. For part 1, we iterate over indices `i` from 0 to size-1, compute `abs(left[i] - right[i])`, and accumulate the sum. For part 2, we iterate over each distinct value in `left` (can iterate over all elements, using `std::count` on `right` for each element, which is O(n^2) in worst case, but acceptable for moderate input; alternatively we could build a frequency map for `right` in O(n) and then sum `value * frequency[value]` for each `left` element). The frequency-map approach is more efficient and recommended. Edge cases: empty file (return {0,0}), unequal vector sizes (should not happen per problem statement, but we can handle by only iterating up to the smaller size for part1). Time complexity: file reading is O(n) where n is number of lines; sorting takes O(n log n); part1 is O(n); part2 with frequency map is O(n). Total O(n log n). Space complexity: O(n) for the two vectors and O(n) for the frequency map (worst case all values distinct).
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cstdlib>
#include <utility>

// Read a file where each line has two integers.
// Return: {sum of absolute differences after sorting, sum of left_value * count_in_right}
std::pair<long long, long long> solveAdjacencyFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<long long> left, right;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        long long a, b;
        iss >> a >> b;
        left.push_back(a);
        right.push_back(b);
    }

    // Part 1: sorted absolute differences
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    long long diffSum = 0;
    const size_t n = std::min(left.size(), right.size());
    for (size_t i = 0; i < n; ++i) {
        diffSum += std::llabs(left[i] - right[i]);
    }

    // Part 2: frequency map for right
    std::unordered_map<long long, long long> freq;
    for (long long v : right) {
        ++freq[v];
    }

    long long simScore = 0;
    for (long long v : left) {
        auto it = freq.find(v);
        if (it != freq.end()) {
            simScore += v * it->second;
        }
    }

    return {diffSum, simScore};
}
#include <cassert>
#include <fstream>
#include <cstdio>

int main() {
    // Create temporary test files
    std::ofstream f1("test1.txt");
    f1 << "3 4\n1 2\n5 6\n";
    f1.close();

    std::ofstream f2("test2.txt");
    f2 << "10 5\n10 5\n10 5\n";
    f2.close();

    std::ofstream f3("test3.txt");
    f3 << "7 7\n7 7\n";
    f3.close();

    std::ofstream f4("test4.txt");
    f4.close(); // empty file

    std::ofstream f5("test5.txt");
    f5 << "  -2   8\n-2 8\n0 0\n";
    f5.close();

    auto r1 = solveAdjacencyFile("test1.txt");
    // Sorted left: 1,3,5 ; right: 2,4,6 => diffs:1,1,1 => sum=3
    // similarity: left 1 appears 0, 3 appears 0, 5 appears 0 => 0
    assert(r1.first == 3LL);
    assert(r1.second == 0LL);

    auto r2 = solveAdjacencyFile("test2.txt");
    // Sorted left: 10,10,10 ; right: 5,5,5 => diffs:5,5,5 => sum=15
    // similarity: 10 appears 3 times => 10*3*3? Wait: each left 10 multiplied by count(10)=0? Actually right has 5 only, count(10)=0 => 0
    assert(r2.first == 15LL);
    assert(r2.second == 0LL);

    auto r3 = solveAdjacencyFile("test3.txt");
    // left:7,7 right:7,7 => diffs:0,0 sum=0
    // similarity: each 7 * count(7)=2 => 2* (7*2)=28? Actually there are two left 7s, each contributes 7*2=14 => total 28
    assert(r3.first == 0LL);
    assert(r3.second == 28LL);

    auto r4 = solveAdjacencyFile("test4.txt");
    assert(r4.first == 0LL);
    assert(r4.second == 0LL);

    auto r5 = solveAdjacencyFile("test5.txt");
    // left: -2,-2,0 ; right:8,8,0 => sorted: left -2,-2,0 ; right 0,8,8 => diffs:2,10,8 => sum=20
    // similarity: -2 appears 0 times, -2 appears 0, 0 appears once => 0*1=0 => total 0
    assert(r5.first == 20LL);
    assert(r5.second == 0LL);

    // Clean up
    std::remove("test1.txt");
    std::remove("test2.txt");
    std::remove("test3.txt");
    std::remove("test4.txt");
    std::remove("test5.txt");

    return 0;
}
