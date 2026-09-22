// You are given a program that reads an integer `n` from the command line, prints `n` on the first line, then generates and prints `n` random integers in the range `[1, 2*n]` (inclusive), one per line. Write a standalone C++ function `generateRandomSequence` that takes an integer `n` and returns a `std::string` containing exactly that output format: a first line with `n`, followed by `n` lines each holding a random integer in `[1, 2*n]`. The function must use `std::mt19937` seeded with `std::random_device`. The output must be deterministic in structure (always `n` lines after the first) but values are random. Handle the edge case `n <= 0` by returning an empty string. Do not include any `main` function in the solution; only the free function. The function must be `const`-correct, meaning it should not modify any passed parameters and should use `const` where appropriate.
// The solution must generate a properly formatted string. We start by checking if `n` is positive; if not, return an empty string since there is no valid sequence. For positive `n`, we use `std::mt19937` seeded with `std::random_device` to obtain a high-quality random generator. We then create a uniform integer distribution over `[1, 2*n]`. We construct the result by first appending `std::to_string(n)` followed by a newline. Then, for each of the `n` iterations, we generate a random number using the distribution and the engine, convert it to a string, append it, and add a newline (except after the very last number, to match typical output, but since we must exactly match the snippet, we include a newline after every line, including the last; the snippet prints each number with `<< "\n"`, so we do the same). Edges cases: `n = 0` should return an empty string because there are no lines to print. Negative `n` also returns empty. Time complexity is O(n) because we generate n numbers and perform constant-time string appends (amortized). Space complexity is O(n) for the returned string, which holds approximately n numbers plus newlines, each number up to O(log n) digits, so total O(n log n) characters, but we can state O(n) in terms of number of values. The use of `const` in the function signature is unnecessary for the parameter because we only pass by value, but we ensure the function does not modify any external state. The function itself is not `const` because it returns a new string and uses local random generator.
#include <string>
#include <random>

// Generate a sequence in the format: first line n, then n random integers in [1, 2*n], each on its own line.
// Returns an empty string if n <= 0.
std::string generateRandomSequence(const int n) {
    if (n <= 0) {
        return std::string();
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, n * 2);

    std::string result = std::to_string(n);
    result += "\n";

    for (int i = 0; i < n; ++i) {
        result += std::to_string(dist(mt));
        result += "\n";
    }

    return result;
}
#include <cassert>
#include <string>
#include <sstream>
#include <vector>

// The solution function is declared here (normally in a header, but for testing we include the code inline).
std::string generateRandomSequence(const int n) {
    if (n <= 0) {
        return std::string();
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, n * 2);

    std::string result = std::to_string(n);
    result += "\n";

    for (int i = 0; i < n; ++i) {
        result += std::to_string(dist(mt));
        result += "\n";
    }

    return result;
}

int main() {
    // n <= 0 returns empty string
    assert(generateRandomSequence(0).empty());
    assert(generateRandomSequence(-3).empty());

    // n = 1: output has 2 lines, first "1", second a number in [1,2]
    std::string s1 = generateRandomSequence(1);
    std::istringstream iss1(s1);
    std::string line;
    std::getline(iss1, line);
    assert(line == "1");
    std::getline(iss1, line);
    int val1 = std::stoi(line);
    assert(val1 >= 1 && val1 <= 2);
    assert(iss1.eof());

    // n = 5: output has 6 lines, first "5", all other numbers in [1,10]
    std::string s5 = generateRandomSequence(5);
    std::istringstream iss5(s5);
    std::getline(iss5, line);
    assert(line == "5");
    std::vector<int> vals;
    while (std::getline(iss5, line)) {
        int v = std::stoi(line);
        assert(v >= 1 && v <= 10);
        vals.push_back(v);
    }
    assert(vals.size() == 5);

    // n = 10: output has 11 lines, first "10", all other numbers in [1,20]
    std::string s10 = generateRandomSequence(10);
    std::istringstream iss10(s10);
    std::getline(iss10, line);
    assert(line == "10");
    int count = 0;
    while (std::getline(iss10, line)) {
        int v = std::stoi(line);
        assert(v >= 1 && v <= 20);
        ++count;
    }
    assert(count == 10);

    // Check that the output ends with a newline (the last getline returns empty and leaves the stream at eof)
    std::string sen = generateRandomSequence(3);
    assert(!sen.empty() && sen.back() == '\n');

    // Continuously generate larger n to ensure no crash and legal bounds
    for (int n : {100, 1000, 5000}) {
        std::string s = generateRandomSequence(n);
        std::istringstream iss(s);
        std::getline(iss, line);
        assert(std::stoi(line) == n);
        int cnt = 0;
        while (std::getline(iss, line)) {
            int v = std::stoi(line);
            assert(v >= 1 && v <= 2 * n);
            ++cnt;
        }
        assert(cnt == n);
    }

    return 0;
}
