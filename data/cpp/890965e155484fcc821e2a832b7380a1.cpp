/*
Write a C++ function that reads a CSV file containing crash records, where each line has fields including date at index 0, number of injured persons at index 10, and number of dead persons at index 11. The function should parse each line correctly while ignoring commas that appear inside quoted fields (particularly relevant for the location column), skip any malformed lines that do not have at least 26 columns, skip the header line (first non-malformed line), and sum up the total injured and dead only for records where the year extracted from the date field is 2023. The date format is `MM/DD/YYYY` (e.g., `1/15/2023`). The function should take the file path as a `const std::string&` parameter and return a `std::pair<int, int>` where the first element is the total injured and the second is the total dead. If the file cannot be opened, return `{0, 0}`. Ignore any cell values that are empty or non-positive when summing.
*/

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>

// Parse a single CSV line, respecting double-quoted fields that may contain commas.
std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> cells;
    std::string currentCell;
    bool insideQuotes = false;

    for (char ch : line) {
        if (ch == '"') {
            insideQuotes = !insideQuotes;
        } else if (ch == ',' && !insideQuotes) {
            cells.push_back(currentCell);
            currentCell.clear();
        } else {
            currentCell += ch;
        }
    }
    cells.push_back(currentCell); // last cell
    return cells;
}

// Compute total injured and dead persons from crash records for the year 2023.
// Returns {total_injured, total_dead}. Returns {0,0} if file cannot be opened.
std::pair<int, int> count2023InjuriesAndDeaths(const std::string& filePath) {
    std::ifstream inputFile(filePath);
    if (!inputFile.is_open()) {
        return {0, 0};
    }

    int totalInjured = 0;
    int totalDead = 0;
    bool isFirstLine = true;
    std::string line;

    while (std::getline(inputFile, line)) {
        std::vector<std::string> cells = parseCSVLine(line);
        if (cells.size() < 26) {
            continue; // malformed or truncated line
        }

        if (isFirstLine) {
            isFirstLine = false; // skip header
            continue;
        }

        // Extract year from date field "MM/DD/YYYY"
        const std::string& dateField = cells[0];
        size_t firstSlash = dateField.find('/');
        if (firstSlash == std::string::npos) {
            continue; // invalid date format
        }
        size_t secondSlash = dateField.find('/', firstSlash + 1);
        if (secondSlash == std::string::npos) {
            continue; // invalid date format
        }
        std::string yearStr = dateField.substr(secondSlash + 1);
        int year;
        try {
            year = std::stoi(yearStr);
        } catch (...) {
            continue; // non-numeric year
        }

        if (year == 2023) {
            if (!cells[10].empty()) {
                int injured = std::stoi(cells[10]);
                if (injured > 0) {
                    totalInjured += injured;
                }
            }
            if (!cells[11].empty()) {
                int dead = std::stoi(cells[11]);
                if (dead > 0) {
                    totalDead += dead;
                }
            }
        }
    }

    return {totalInjured, totalDead};
}

#include <cassert>
#include <fstream>
#include <iostream>

// Solution function declaration (from the solution above)
std::pair<int, int> count2023InjuriesAndDeaths(const std::string& filePath);
std::vector<std::string> parseCSVLine(const std::string& line);

int main() {
    // Create a temporary CSV file for testing
    const std::string testFile = "test_crashes.csv";
    {
        std::ofstream file(testFile);
        file << "date,borough,zip,lat,long,location,on_street,off_street,cross_street,persons_injured,persons_killed,vehicle_count,contributing,vehicle_types,time,borough2,zip2,lat2,long2,location2,on_street2,off_street2,cross_street2,persons_injured2,persons_killed2,extra\n";
        file << "1/15/2023,Manhattan,10001,40.7,-74.0,\"123 Main St, Apt 4\",Ave A,St B,St C,2,1,1,None,Car,10:00,Manhattan,10001,40.7,-74.0,\"456 Oak St, Suite 5\",Ave B,St D,St E,0,0,x\n";
        file << "12/25/2023,Brooklyn,11201,40.6,-73.9,\"789 Pine Rd\",Ave C,St F,St G,5,0,2,Speeding,Truck,11:30,Brooklyn,11201,40.6,-73.9,\"101 Elm St\",Ave D,St H,St I,1,1,y\n";
        file << "2/29/2024,Queens,11301,40.7,-73.8,Location1,Ave E,St J,St K,3,0,1,None,Car,12:00,Queens,11301,40.7,-73.8,Location2,Ave F,St L,St M,0,0,z\n";
        file << "6/1/2023,Bronx,10401,40.8,-73.9,\"Comma, inside, location\",Ave G,St N,St O,0,0,1,None,Motorcycle,13:00,Bronx,10401,40.8,-73.9,Loc2,Ave H,St P,St Q,0,0,w\n";
        file << "3/14/2023,Staten Island,10301,40.6,-74.1,Location3,Ave I,St R,St S,1,2,3,None,Car,14:00,Staten Island,10301,40.6,-74.1,Loc4,Ave J,St T,St U,0,0,v\n";
    }

    auto result = count2023InjuriesAndDeaths(testFile);
    assert(result.first == 8);   // 2 + 5 + 0 + 1 = 8
    assert(result.second == 3);  // 1 + 0 + 0 + 2 = 3

    // Test missing file
    auto missingResult = count2023InjuriesAndDeaths("nonexistent_file.csv");
    assert(missingResult.first == 0);
    assert(missingResult.second == 0);

    // Test parsing function independently
    std::vector<std::string> cells = parseCSVLine("\"123 Main St, Apt 4\",Ave A,St B");
    assert(cells.size() == 3);
    assert(cells[0] == "123 Main St, Apt 4");
    assert(cells[1] == "Ave A");
    assert(cells[2] == "St B");

    std::cout << "All tests passed!" << std::endl;
    return 0;
}

// The solution requires a two-part approach: robust CSV parsing and year extraction. For parsing, we iterate character by character through each line, toggling a boolean flag when encountering a double-quote character `"`. When we encounter a comma while not inside a quoted region, we push the current accumulated cell string to a vector and clear it. At the end of the line, we push the final cell. This ensures that commas inside quoted fields (e.g., `"123 Main St, Apt 4"`) are preserved as part of that cell and do not create extra columns. After parsing, we check if the vector size is at least 26 (0-indexed, so we can safely access indices 0, 10, and 11). We skip the first non-malformed line as the header. For the date field at index 0, we need to extract the year. The date is in `MM/DD/YYYY` format; we use `find` twice: first to locate the first `/`, then the second `/` after that, and take the substring after the second slash as the year string, then convert to integer using `stoi`. If the year equals 2023, we check if cells at indices 10 and 11 are non-empty and their integer values are greater than 0 before adding them to the totals. Edge cases include: header line detection (skip the first valid line), malformed lines with fewer than 26 columns (skip), empty or negative injury/death cells (ignore), quoted fields containing commas (handled by the CSV parser), and file open failure (return `{0,0}`). Time complexity is O(total characters in the file) because we process each line once and each character once for parsing. Space complexity is O(max line length) for the temporary vector and cell strings, plus O(1) for the counters.
