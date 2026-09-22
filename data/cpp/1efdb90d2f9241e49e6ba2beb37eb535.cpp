// Write a C++ function that generates an array of 200 random uppercase letters (A-Z) and returns a string summarizing the frequency of each letter that appears at least once. The output string should contain lines in the format `"L: count"` separated by newline characters, where `L` is the uppercase letter and `count` is the number of times it occurs. The lines must be ordered alphabetically from A to Z. The function should use a fixed random seed or `rand()` with `srand(time(0))` internally for reproducibility in testing, but for testing purposes, it must accept a seed parameter to make deterministic. Specifically, the function signature should be `std::string letterFrequency(int seed)`, which sets the random seed, generates the array, computes frequencies, and returns the formatted result. Edge cases: if a letter appears zero times, it is omitted from the output; if all letters appear at least once, the output has 26 lines; the function must handle any seed value, including negative and zero.

#include <cassert>
#include <string>
#include <sstream>

// The solution function declaration (already defined above)
std::string letterFrequency(int seed);

int main() {
    // For a fixed seed, the output length equals the number of distinct letters.
    // We can verify by checking that the total of all counts equals 200.
    // Example for seed 42.
    std::string result1 = letterFrequency(42);
    assert(!result1.empty());  // At least one line present
    
    // Verify total counts sum to 200 using a simple parser.
    int total1 = 0;
    std::istringstream iss1(result1);
    std::string line;
    while (std::getline(iss1, line)) {
        // line format: "L: count"
        size_t colonPos = line.find(": ");
        assert(colonPos != std::string::npos);
        total1 += std::stoi(line.substr(colonPos + 2));
    }
    assert(total1 == 200);
    
    // Deterministic: same seed gives same output.
    assert(letterFrequency(42) == result1);
    
    // Different seeds may produce different outputs (with high probability).
    // Here we just ensure the function runs without error for seed 0.
    std::string result2 = letterFrequency(0);
    assert(!result2.empty());
    
    // Checks that each line is correctly formatted (valid letter and positive count)
    std::istringstream iss2(result2);
    while (std::getline(iss2, line)) {
        assert(line.size() >= 4); // "A: 1" at minimum
        assert(line[1] == ':');   // colon after the single uppercase letter
        assert(line[0] >= 'A' && line[0] <= 'Z');
        int count = std::stoi(line.substr(2));
        assert(count > 0);
    }
    
    // Verify that no letter appears more than 200 times in any single run.
    // Since the total is 200, each count is between 1 and 200.
    std::istringstream iss3(result2);
    while (std::getline(iss3, line)) {
        int count = std::stoi(line.substr(2));
        assert(count <= 200);
    }
    
    // Check that the output lines are in alphabetical order.
    char prev = 'A';
    std::istringstream iss4(result2);
    while (std::getline(iss4, line)) {
        char current = line[0];
        assert(current >= prev); // non-decreasing order
        prev = current;
    }
    
    // For a seed that likely generates all letters (theoretically possible but not guaranteed),
    // we just ensure that no negative counts or invalid format appear.

    // Additional edge: seed = -1 should also work.
    std::string resultNeg = letterFrequency(-1);
    assert(!resultNeg.empty());

    // All assertions passed.
    return 0;
}

#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sstream>

// Generate 200 random uppercase letters and return their frequency summary as a string.
std::string letterFrequency(int seed) {
    std::srand(seed);
    const int arraySize = 200;
    std::vector<char> arr(arraySize);
    std::vector<int> frequency(26, 0);

    for (int i = 0; i < arraySize; ++i) {
        arr[i] = 'A' + std::rand() % 26;
        ++frequency[arr[i] - 'A'];
    }

    std::ostringstream result;
    for (int i = 0; i < 26; ++i) {
        if (frequency[i] > 0) {
            result << static_cast<char>(i + 'A') << ": " << frequency[i] << "\n";
        }
    }

    std::string output = result.str();
    if (!output.empty() && output.back() == '\n') {
        output.pop_back(); // Remove trailing newline for cleaner output
    }
    return output;
}

// The solution uses a fixed-size array of 200 characters, seeded with `srand(seed)` to ensure deterministic output for testing. Generate each character as `'A' + rand() % 26` to cover all uppercase letters. Maintain a frequency array of size 26 initialized to zero. After generating all characters, increment the frequency for each letter using `arr[i] - 'A'` as an index. Then, iterate the frequency array from 0 to 25, and for each index where the count is greater than zero, append a line `char(i + 'A')` followed by `": "` and the count to a string, separated by newline characters. Edge cases include the seed producing the same letter multiple times or no occurrences of certain letters; the loop naturally skips zero counts. Time complexity is O(200 + 26) = O(1) since the array size is fixed, but if generalized to n, it would be O(n + 26) with O(26) auxiliary space for the frequency array. The use of a string stream or simple concatenation is acceptable; the reference solution uses `std::ostringstream` for efficiency. Since the function is deterministic with a seed, duplicate calls with the same seed produce identical output.
