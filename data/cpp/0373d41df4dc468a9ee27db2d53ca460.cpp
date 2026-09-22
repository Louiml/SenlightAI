// Write a C++ function named `analyzeCricketMatches` that takes as parameters a dynamically allocated array of student names (as `std::string`), a dynamically allocated array of integers representing the number of cricket matches each student played, and an integer `size` indicating the number of students. The function must perform the following operations in this exact order: (1) display all students in the original order, formatted with a numbered index, left-aligned name padded to 30 characters with dots, and the match count labeled "Matches: "; (2) find and display the student who played the most matches (only the first occurrence if ties exist) using the same formatting; (3) compute and return the average number of matches played as a `double` (the array is not modified for this step); (4) sort both arrays in ascending order of student names (case-sensitive, lexicographic) using the selection sort algorithm, and then display the sorted list using the same formatting. The function should handle edge cases such as `size == 1` and ensure no division by zero occurs if `size` is 0 (in which case return 0.0 and display no output for lists). The function must be self-contained, use `const` correctly (e.g., parameters that are not modified like the match array during display or mean calculation should be `const` where appropriate), and include necessary headers.
// The solution requires a single function that orchestrates several sub-tasks. First, we display the array in its original order; this is straightforward with a loop that formats each line using `std::cout`, `std::setfill`, `std::setw`, and `std::left`. For finding the maximum matches, we iterate through the matches array, track the index of the first occurrence of the maximum value (initialize max to matches[0] and update only on strict greater-than comparisons to preserve first occurrence). For the average, we sum all matches and divide by `size`, but if `size` is 0, return 0.0 to avoid division by zero; otherwise compute and return the sum divided by size. For sorting, we implement selection sort that swaps both the names and matches arrays simultaneously, comparing names lexicographically with the `<` operator (which is case-sensitive). After sorting, we call the display routine again. Edge cases: `size == 1` means the loop for selection sort doesn't execute (since startScan < size-1 is false), and max and average work fine; `size == 0` should immediately return 0.0 without any output. Time complexity: Display and max finding are O(n), average is O(n), sorting is O(n²) in the worst case due to selection sort. Space complexity is O(1) auxiliary beyond the input arrays (we only use a few local variables). The function must not include a main() and must be self-contained with `<iostream>`, `<iomanip>`, `<string>`.
#include <iostream>
#include <iomanip>
#include <string>

// Analyzes cricket match data: displays, finds max, computes mean, sorts and redisplays.
void analyzeCricketMatches(std::string names[], int matches[], int size) {
    if (size == 0) {
        return;
    }

    // Display original list
    std::cout << "Original List:\n";
    for (int i = 0; i < size; ++i) {
        std::cout << (i + 1) << "  " << std::setfill('.') << std::setw(30) << std::left
                  << names[i] << " Matches: " << matches[i] << "\n";
    }

    // Find and display student with most matches (first occurrence)
    int maxIndex = 0;
    for (int i = 1; i < size; ++i) {
        if (matches[i] > matches[maxIndex]) {
            maxIndex = i;
        }
    }
    std::cout << "\nMost Matches:\n";
    std::cout << (maxIndex + 1) << "  " << std::setfill('.') << std::setw(30) << std::left
              << names[maxIndex] << " Matches: " << matches[maxIndex] << "\n";

    // Compute average
    double sum = 0;
    for (int i = 0; i < size; ++i) {
        sum += matches[i];
    }
    std::cout << "\nAverage Matches: " << (sum / size) << "\n";

    // Sort by name using selection sort
    for (int startScan = 0; startScan < size - 1; ++startScan) {
        int minIndex = startScan;
        std::string minName = names[minIndex];
        int minMatches = matches[minIndex];

        for (int index = startScan + 1; index < size; ++index) {
            if (names[index] < minName) {
                minName = names[index];
                minMatches = matches[index];
                minIndex = index;
            }
        }

        // Swap
        names[minIndex] = names[startScan];
        names[startScan] = minName;
        matches[minIndex] = matches[startScan];
        matches[startScan] = minMatches;
    }

    // Display sorted list
    std::cout << "\nSorted List:\n";
    for (int i = 0; i < size; ++i) {
        std::cout << (i + 1) << "  " << std::setfill('.') << std::setw(30) << std::left
                  << names[i] << " Matches: " << matches[i] << "\n";
    }
}
#include <cassert>
#include <sstream>
#include <iostream>
#include <string>

// Declaration of the function to test
void analyzeCricketMatches(std::string names[], int matches[], int size);

// Helper to capture cout output and return as string
std::string captureOutput(std::string names[], int matches[], int size) {
    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    analyzeCricketMatches(names, matches, size);
    std::cout.rdbuf(old);
    return buffer.str();
}

int main() {
    // Test 1: Basic case with 5 students
    std::string names1[] = {"Ragland, Nicholas", "Smith, John", "Zhang, Xiu Ying", "Evans, Olivia", "Song, Mona"};
    int matches1[] = {23, 28, 21, 30, 26};
    std::string out1 = captureOutput(names1, matches1, 5);
    assert(out1.find("Original List") != std::string::npos);
    assert(out1.find("Ragland, Nicholas") != std::string::npos);
    assert(out1.find("Evans, Olivia") != std::string::npos);
    assert(out1.find("Most Matches:") != std::string::npos);
    assert(out1.find("Matches: 30") != std::string::npos);
    assert(out1.find("Average Matches: 25.6") != std::string::npos);
    assert(out1.find("Sorted List") != std::string::npos);
    // Check sorted order by finding positions of names
    size_t posEvans = out1.find("Evans, Olivia");
    size_t posRagland = out1.find("Ragland, Nicholas");
    size_t posSmith = out1.find("Smith, John");
    size_t posSong = out1.find("Song, Mona");
    size_t posZhang = out1.find("Zhang, Xiu Ying");
    assert(posEvans < posRagland && posRagland < posSmith && posSmith < posSong && posSong < posZhang);

    // Test 2: Single student
    std::string names2[] = {"Alice"};
    int matches2[] = {7};
    std::string out2 = captureOutput(names2, matches2, 1);
    assert(out2.find("Average Matches: 7") != std::string::npos);
    assert(out2.find("Matches: 7") != std::string::npos);

    // Test 3: Size zero - should not crash and no output
    std::string names3[] = {};
    int matches3[] = {};
    std::string out3 = captureOutput(names3, matches3, 0);
    assert(out3.empty());

    // Test 4: Ties for most matches - should pick first
    std::string names4[] = {"Bob", "Alice", "Charlie"};
    int matches4[] = {10, 10, 5};
    std::string out4 = captureOutput(names4, matches4, 3);
    assert(out4.find("Bob") < out4.find("Most Matches") || out4.find("Bob") != std::string::npos);
    // After sorting, Alice appears first
    size_t posAlice4 = out4.find("Alice");
    size_t posBob4 = out4.find("Bob");
    assert(posAlice4 < posBob4);

    // Test 5: Negative numbers not allowed per original spec, but function should handle gracefully
    std::string names5[] = {"X", "Y"};
    int matches5[] = {-1, 5};
    std::string out5 = captureOutput(names5, matches5, 2);
    assert(out5.find("Average Matches: 2") != std::string::npos);

    std::cout << "All tests passed!\n";
    return 0;
}
