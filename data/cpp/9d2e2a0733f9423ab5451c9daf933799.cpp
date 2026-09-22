/*
Write a C++ function `countMaxZeroEndingRuns` that processes a sequence of integers stored in a text file (one integer per line or separated by whitespace) and returns a `std::pair<int, int>` where the first element is the maximum length of a contiguous subsequence (run) in which every integer ends with digit `0`, and the second element is the number of such runs that achieve that maximum length. If the file is empty or contains no zero-ending integers, return `{0, 0}`. The function must read from a given filename, handle files with arbitrary whitespace, and treat each zero-ending integer as part of a run; non-zero-ending integers break the current run. Ignore leading/trailing whitespace and empty lines.
*/

#include <fstream>
#include <utility>

// Reads integers from the file named 'filename' and returns a pair:
// first = maximum length of a contiguous run of numbers ending in 0,
// second = number of such runs achieving that maximum.
// Returns {0,0} for empty file or no zero-ending numbers.
std::pair<int, int> countMaxZeroEndingRuns(const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        return {0, 0};
    }

    int x;
    int current_run = 0;
    int max_run = 0;
    int count = 0;

    while (fin >> x) {
        if (x % 10 == 0) {
            ++current_run;
        } else {
            current_run = 0;
        }

        if (current_run > max_run) {
            max_run = current_run;
            count = 1;
        } else if (current_run == max_run && current_run > 0) {
            ++count;
        }
    }

    return {max_run, count};
}

#include <cassert>
#include <fstream>
#include <utility>

// The solution function is assumed to be included above.
// For testing, create temp files.

int main() {
    // Test 1: Basic sequence with two max runs
    { std::ofstream f("test1.txt"); f << "10 20 30 5 40 50 60 7 80 90\n"; }
    auto r1 = countMaxZeroEndingRuns("test1.txt");
    assert(r1.first == 3 && r1.second == 2);

    // Test 2: All numbers end in zero -> one run
    { std::ofstream f("test2.txt"); f << "10 20 30 40\n"; }
    auto r2 = countMaxZeroEndingRuns("test2.txt");
    assert(r2.first == 4 && r2.second == 1);

    // Test 3: No zero-ending numbers
    { std::ofstream f("test3.txt"); f << "1 2 3 4 5\n"; }
    auto r3 = countMaxZeroEndingRuns("test3.txt");
    assert(r3.first == 0 && r3.second == 0);

    // Test 4: Empty file
    { std::ofstream f("test4.txt"); }
    auto r4 = countMaxZeroEndingRuns("test4.txt");
    assert(r4.first == 0 && r4.second == 0);

    // Test 5: Negative zero-ending numbers
    { std::ofstream f("test5.txt"); f << "-10 -20 3 -30 -40 -50 7\n"; }
    auto r5 = countMaxZeroEndingRuns("test5.txt");
    assert(r5.first == 3 && r5.second == 2);

    // Test 6: Single zero-ending number
    { std::ofstream f("test6.txt"); f << "100\n"; }
    auto r6 = countMaxZeroEndingRuns("test6.txt");
    assert(r6.first == 1 && r6.second == 1);

    // Test 7: Whitespace variations
    { std::ofstream f("test7.txt"); f << "  10 \n 20\t30\n 5 40\n"; }
    auto r7 = countMaxZeroEndingRuns("test7.txt");
    assert(r7.first == 3 && r7.second == 1);

    // Test 8: Multiple max runs with same length at end
    { std::ofstream f("test8.txt"); f << "10 1 20 2 30\n"; }
    auto r8 = countMaxZeroEndingRuns("test8.txt");
    assert(r8.first == 1 && r8.second == 3);

    // Test 9: File not found
    auto r9 = countMaxZeroEndingRuns("nonexistent_file_xyz.txt");
    assert(r9.first == 0 && r9.second == 0);

    return 0;
}

// The solution scans the file integer by integer using an input file stream. Maintain three variables: `current_run` (length of the current consecutive zero-ending run), `max_run` (longest run seen so far), and `count` (number of runs equal to `max_run`). For each integer `x`, check if `x % 10 == 0` (handling negatives: `x % 10` may be `0` or `-0`, both equal `0`, so it works). If yes, increment `current_run`; otherwise reset `current_run` to `0`. After updating `current_run`, compare with `max_run`. If `current_run > max_run`, update `max_run` and set `count = 1`. Else if `current_run == max_run` and `current_run > 0`, increment `count` (only when current run length is positive to avoid counting empty runs). Important edge cases: empty file → return `{0,0}`; file with only non-zero-ending numbers → `max_run` remains `0`, `count` should remain `0` (do not increment for zero-length runs); multiple runs of the same maximum length are counted correctly. Time complexity is O(n) where n is the number of integers in the file; space complexity O(1) auxiliary.
