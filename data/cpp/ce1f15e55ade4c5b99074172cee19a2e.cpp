Given the global configuration variables from the provided `par` namespace snippet, write a C++ free function named `configureSimulation` that takes a single `std::string` parameter representing a path to a configuration file. The function must read the file line by line, where each non-empty line has the format `key = value` (with optional surrounding whitespace). The function must update the corresponding global variable in the `par` namespace if the key matches one of the following: `iMaxTime`, `iA`, `iB`, `iC`, `sParFile`, `sSeedFile`, `sOutDir`, `bPrintCyt`, `sM`, `iT`, `bP`, `iP_Start`, `iP_End`, `iniSDF1`, `inistiffness`, `basicSDF1`, `basicstiffness`, `maxstiffness`, `sw_mic`, `sw_mm`, `sw_cd8`, or `sw_treg`. For integer variables, parse as `int`; for double variables, parse as `double`; for boolean variables, accept `true` or `false` (case-insensitive); for string variables, take the part after `=` trimming leading/trailing whitespace. Invalid values for numeric or boolean types should cause the function to throw a `std::invalid_argument` with a message including the key and the line number (1-based). If a key is unknown, ignore the line. The function returns the total number of successfully updated parameters, and after reading all lines, it should ensure that `iP_Start <= iP_End`, otherwise set `iP_End = iP_Start` and count that as an additional successful update.

// The solution requires a robust line parser that extracts a key and a value from each line. The primary algorithm iterates over each line of the file, trims whitespace, skips empty lines, splits on the first `=`, and trims the key and value. A mapping from key names to an update operation is used to avoid long if-else chains; this can be implemented as a series of conditional checks or a `std::map` of `std::function` lambdas that capture references to the global variables. For each recognized key, the appropriate type conversion is attempted: integers via `std::stoi`, doubles via `std::stod`, booleans by comparing to "true"/"false" (case-insensitive) and throwing if invalid, and strings by simply taking the trimmed value. If conversion throws, catch and rethrow as `std::invalid_argument` with a descriptive message. Unknown keys are ignored. After processing all lines, check `iP_Start` and `iP_End`; if `iP_Start > iP_End`, set `iP_End = iP_Start` and increment the count. Edge cases include missing `=` (should be treated as unknown key and skipped), lines with extra spaces, empty values for strings (allowed), and very large numeric values that cause `std::stoi`/`std::stod` to throw `std::out_of_range` (should be caught and rethrown as `std::invalid_argument`). The time complexity is \(O(L \cdot K)\) where \(L\) is the number of lines and \(K\) is the number of keys (since each line is compared against up to 22 keys, but in practice we can do direct comparisons, so \(O(L)\) with constant factor 22). Space complexity is \(O(1)\) beyond the input buffer and the returned count. The solution must be self-contained, include necessary headers, and use `const` where appropriate for the input string.

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <map>
#include <functional>

namespace par {
    int iMaxTime = 10;
    double iA = 0;  //BTZ
    double iB = 0;    //LEN
    double iC = 0;    //Third drug
    std::string sParFile = "";
    std::string sSeedFile = "";
    std::string sOutDir = "Data";
    bool bPrintCyt = false;
    std::string sM = "M";
    int iT = -1;
    bool bP = false;
    int iP_Start = 100;
    int iP_End = 200;

    double iniSDF1 = 0.00535;
    double inistiffness = 400;

    double basicSDF1 = 0.0012;
    double basicstiffness = 250;
    double maxstiffness = 530;

    int sw_mic = 1;
    int sw_mm = 1;
    int sw_cd8 = 0;
    int sw_treg = 0;
}

namespace {
    std::string trim(const std::string& s) {
        auto start = s.find_first_not_of(" \t\r\n");
        if (start == std::string::npos) return "";
        auto end = s.find_last_not_of(" \t\r\n");
        return s.substr(start, end - start + 1);
    }

    bool parseBool(const std::string& value) {
        std::string lower = value;
        std::transform(lower.begin(), lower.end(), lower.begin(),
                       [](unsigned char c){ return std::tolower(c); });
        if (lower == "true") return true;
        if (lower == "false") return false;
        throw std::invalid_argument("Invalid boolean");
    }
}

// Reads a configuration file and updates global par:: variables.
// Returns number of successfully updated parameters.
// Throws std::invalid_argument on malformed numeric/boolean values.
int configureSimulation(const std::string& configPath) {
    std::ifstream file(configPath);
    if (!file.is_open()) {
        throw std::invalid_argument("Cannot open config file: " + configPath);
    }

    int updatedCount = 0;
    std::string line;
    int lineNumber = 0;

    auto parseError = [&](const std::string& key, const std::string& what) {
        return std::invalid_argument("Invalid value for key '" + key +
                                     "' at line " + std::to_string(lineNumber) +
                                     ": " + what);
    };

    while (std::getline(file, line)) {
        ++lineNumber;
        std::string trimmedLine = trim(line);
        if (trimmedLine.empty()) continue;

        auto eqPos = trimmedLine.find('=');
        if (eqPos == std::string::npos) continue; // no '=', treat as unknown/ignored

        std::string key = trim(trimmedLine.substr(0, eqPos));
        std::string value = trim(trimmedLine.substr(eqPos + 1));

        try {
            if (key == "iMaxTime") {
                par::iMaxTime = std::stoi(value);
                ++updatedCount;
            } else if (key == "iA") {
                par::iA = std::stod(value);
                ++updatedCount;
            } else if (key == "iB") {
                par::iB = std::stod(value);
                ++updatedCount;
            } else if (key == "iC") {
                par::iC = std::stod(value);
                ++updatedCount;
            } else if (key == "sParFile") {
                par::sParFile = value;
                ++updatedCount;
            } else if (key == "sSeedFile") {
                par::sSeedFile = value;
                ++updatedCount;
            } else if (key == "sOutDir") {
                par::sOutDir = value;
                ++updatedCount;
            } else if (key == "bPrintCyt") {
                par::bPrintCyt = parseBool(value);
                ++updatedCount;
            } else if (key == "sM") {
                par::sM = value;
                ++updatedCount;
            } else if (key == "iT") {
                par::iT = std::stoi(value);
                ++updatedCount;
            } else if (key == "bP") {
                par::bP = parseBool(value);
                ++updatedCount;
            } else if (key == "iP_Start") {
                par::iP_Start = std::stoi(value);
                ++updatedCount;
            } else if (key == "iP_End") {
                par::iP_End = std::stoi(value);
                ++updatedCount;
            } else if (key == "iniSDF1") {
                par::iniSDF1 = std::stod(value);
                ++updatedCount;
            } else if (key == "inistiffness") {
                par::inistiffness = std::stod(value);
                ++updatedCount;
            } else if (key == "basicSDF1") {
                par::basicSDF1 = std::stod(value);
                ++updatedCount;
            } else if (key == "basicstiffness") {
                par::basicstiffness = std::stod(value);
                ++updatedCount;
            } else if (key == "maxstiffness") {
                par::maxstiffness = std::stod(value);
                ++updatedCount;
            } else if (key == "sw_mic") {
                par::sw_mic = std::stoi(value);
                ++updatedCount;
            } else if (key == "sw_mm") {
                par::sw_mm = std::stoi(value);
                ++updatedCount;
            } else if (key == "sw_cd8") {
                par::sw_cd8 = std::stoi(value);
                ++updatedCount;
            } else if (key == "sw_treg") {
                par::sw_treg = std::stoi(value);
                ++updatedCount;
            }
            // Unknown keys are silently ignored
        } catch (const std::invalid_argument& e) {
            throw parseError(key, e.what());
        } catch (const std::out_of_range& e) {
            throw parseError(key, "value out of range");
        }
    }

    // Ensure iP_Start <= iP_End
    if (par::iP_Start > par::iP_End) {
        par::iP_End = par::iP_Start;
        ++updatedCount;
    }

    return updatedCount;
}

#include <cassert>
#include <fstream>
#include <iostream>

// The solution function and par namespace are assumed available above.

int main() {
    // Create a temporary config file
    const char* testFile = "test_config.txt";
    {
        std::ofstream out(testFile);
        out << "iMaxTime = 20\n";
        out << "iA = 1.5\n";
        out << "bPrintCyt = true\n";
        out << "sOutDir = Results\n";
        out << "unknownKey = 42\n";
        out << "iP_Start = 300\n";
        out << "iP_End = 100\n";
    }

    // Reset defaults before call
    par::iMaxTime = 10;
    par::iA = 0;
    par::bPrintCyt = false;
    par::sOutDir = "Data";
    par::iP_Start = 100;
    par::iP_End = 200;

    int count = configureSimulation(testFile);
    // 6 valid updates (iMaxTime, iA, bPrintCyt, sOutDir, iP_Start, iP_End) + 1 for fixing iP_End
    assert(count == 7);
    assert(par::iMaxTime == 20);
    assert(par::iA == 1.5);
    assert(par::bPrintCyt == true);
    assert(par::sOutDir == "Results");
    assert(par::iP_Start == 300);
    assert(par::iP_End == 300); // corrected

    // Test invalid double value
    {
        std::ofstream out("bad_config.txt");
        out << "iB = not_a_number\n";
    }
    par::iB = 0;
    bool threw = false;
    try {
        configureSimulation("bad_config.txt");
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
    assert(par::iB == 0); // unchanged

    // Test invalid boolean
    {
        std::ofstream out("bad_bool.txt");
        out << "bP = maybe\n";
    }
    par::bP = false;
    threw = false;
    try {
        configureSimulation("bad_bool.txt");
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Test missing file
    threw = false;
    try {
        configureSimulation("nonexistent_file_xyz.txt");
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Cleanup
    std::remove(testFile);
    std::remove("bad_config.txt");
    std::remove("bad_bool.txt");

    std::cout << "All tests passed." << std::endl;
    return 0;
}
