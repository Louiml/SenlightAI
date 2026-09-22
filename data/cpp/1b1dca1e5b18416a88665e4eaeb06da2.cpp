// Write a C++ function that, for each pair of 32-bit unsigned integers from a file named "input.txt" (which starts with an integer line count, followed by that many pairs of unsigned integers), reads the pair and counts three statistics for their 32-bit binary representations (considering all 32 bits, including leading zeros): the number of set bits (1s) in the first number, the number of set bits in the second number, and the Hamming distance between the two numbers (the number of bit positions where they differ). The function should not print anything; instead, it should return a `std::vector<std::tuple<int,int,int>>` containing one tuple per pair in the input order: `(count1, count2, distance)`. Ensure the function reads from the exact filename and handles the file format precisely; if the file is missing or malformed, the behavior is undefined, but you may assume it is well-formed for the test cases.

#include <cassert>
#include <vector>
#include <tuple>
#include <fstream>

// Forward declaration of the solution function (as if included from the solution file)
std::vector<std::tuple<int, int, int>> binaryStatsFromFile();

int main() {
    // Prepare a test input file with known contents
    {
        std::ofstream fout("input.txt");
        fout << "5\n";
        fout << "0 0\n";          // 0 ones, 0 ones, distance 0
        fout << "1 2\n";          // 1 (000...001) has 1 set bit, 2 (000...010) has 1 set bit, distance 2
        fout << "4294967295 0\n"; // all 32 ones, 0 ones, distance 32
        fout << "15 15\n";        // both have 4 ones, distance 0
        fout << "7 11\n";         // 7=000...0111 (3 ones), 11=000...1011 (3 ones), distance 1 (bit 3 differs)
        fout.close();
    }

    auto result = binaryStatsFromFile();

    assert(result.size() == 5);
    auto [c1, c2, d] = result[0];
    assert(c1 == 0 && c2 == 0 && d == 0);

    std::tie(c1, c2, d) = result[1];
    assert(c1 == 1 && c2 == 1 && d == 2);

    std::tie(c1, c2, d) = result[2];
    assert(c1 == 32 && c2 == 0 && d == 32);

    std::tie(c1, c2, d) = result[3];
    assert(c1 == 4 && c2 == 4 && d == 0);

    std::tie(c1, c2, d) = result[4];
    assert(c1 == 3 && c2 == 3 && d == 1);

    return 0;
}

#include <fstream>
#include <vector>
#include <tuple>

// Reads pairs of unsigned integers from "input.txt" and returns
// (popcount1, popcount2, hamming_distance) for each pair.
std::vector<std::tuple<int, int, int>> binaryStatsFromFile() {
    std::ifstream fin("input.txt");
    int lineCount = 0;
    fin >> lineCount;

    std::vector<std::tuple<int, int, int>> results;
    results.reserve(lineCount);

    for (int i = 0; i < lineCount; ++i) {
        unsigned int num1 = 0, num2 = 0;
        fin >> num1 >> num2;

        int count1 = 0;
        int count2 = 0;
        int distance = 0;

        for (int bit = 0; bit < 32; ++bit) {
            const int bit1 = static_cast<int>((num1 >> bit) & 1U);
            const int bit2 = static_cast<int>((num2 >> bit) & 1U);
            count1 += bit1;
            count2 += bit2;
            distance += (bit1 != bit2) ? 1 : 0;
        }

        results.emplace_back(count1, count2, distance);
    }

    return results;
}

// The solution reads the line count from the file, then for each pair of unsigned integers, we compute the binary representations on the fly without needing explicit arrays. For each number, we iterate over all 32 bit positions (from 0 to 31) and check if the bit is set using bitwise AND with a mask. To count set bits in a number `x`, for each bit position `i`, check `(x >> i) & 1`. To compute the Hamming distance, for each bit position, compare the bits of both numbers: if they differ, increment the distance. We can combine the counting of set bits for both numbers and the distance in a single loop over 32 positions. Edge cases include numbers with all bits zero (counts are zero, distance is zero if both zero) and all bits one (counts are 32, distance is zero if both identical). Time complexity is O(32 * n) = O(n) per pair, which is constant per pair. Space complexity is O(n) for the result vector, plus O(1) auxiliary.
