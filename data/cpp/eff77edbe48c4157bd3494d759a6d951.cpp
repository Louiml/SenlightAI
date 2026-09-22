/*
Write a standalone C++ function `computeCoverageAverages` that processes a collection of coverage problem instances. Each instance is described by a text file containing, on a single line, three integers separated by spaces: the number of base stations, the number of devices, and the coverage radius. The function must read a specified number of such instance files (named `instance0.txt`, `instance1.txt`, ...) from a given directory path, compute for each instance the average number of devices covered per base station (i.e., devices divided by stations, as a double), and then return the overall average across all instances. If any file cannot be opened or contains invalid data, skip that instance and do not include it in the average. If no valid instances are found, return `0.0`. The function signature must be: `double computeCoverageAverages(const std::string& directoryPath, int instanceCount);` where `directoryPath` is the folder containing the text files, and `instanceCount` is the number of instances to attempt (files are named `instance0.txt` through `instance{instanceCount-1}.txt`). Use `/` as the directory separator. Do not assume a trailing slash in `directoryPath`; add it if missing.
*/
#include <fstream>
#include <string>
#include <sstream>

// Compute the average ratio of devices to base stations across a set of instance files.
// Files are named instance0.txt, instance1.txt, ... in the given directory.
// Skips unreadable or invalid files; returns 0.0 if no valid instances exist.
double computeCoverageAverages(const std::string& directoryPath, int instanceCount) {
    std::string basePath = directoryPath;
    if (!basePath.empty() && basePath.back() != '/') {
        basePath += '/';
    }

    double sumRatios = 0.0;
    int validCount = 0;

    for (int i = 0; i < instanceCount; ++i) {
        std::string fileName = basePath + "instance" + std::to_string(i) + ".txt";
        std::ifstream inputFile(fileName);
        if (!inputFile.is_open()) {
            continue;
        }

        int stations = 0, devices = 0, radius = 0;
        if (!(inputFile >> stations >> devices >> radius)) {
            continue;
        }

        if (stations > 0) {
            sumRatios += static_cast<double>(devices) / static_cast<double>(stations);
            ++validCount;
        }
    }

    if (validCount == 0) {
        return 0.0;
    }
    return sumRatios / static_cast<double>(validCount);
}
#include <cassert>
#include <fstream>
#include <string>
#include <filesystem>

int main() {
    // Create a temporary directory for testing
    std::string testDir = "test_data_avg";
    std::filesystem::create_directory(testDir);

    // Write three valid files
    {
        std::ofstream f(testDir + "/instance0.txt");
        f << "10 20 100\n"; // ratio 2.0
    }
    {
        std::ofstream f(testDir + "/instance1.txt");
        f << "5 25 200\n"; // ratio 5.0
    }
    {
        std::ofstream f(testDir + "/instance2.txt");
        f << "8 12 50\n"; // ratio 1.5
    }

    // Average should be (2.0 + 5.0 + 1.5) / 3 = 8.5 / 3 = 2.83333...
    double result = computeCoverageAverages(testDir, 3);
    assert(result > 2.83333 && result < 2.83334);

    // Test with a missing file among valid ones
    {
        std::ofstream f(testDir + "/instance3.txt");
        f << "4 4 1\n"; // ratio 1.0
    }
    // instance4 does not exist, should be skipped; average = (2.0+5.0+1.5+1.0)/4 = 9.5/4 = 2.375
    result = computeCoverageAverages(testDir, 5);
    assert(result > 2.374 && result < 2.376);

    // Test all invalid (directory doesn't exist)
    result = computeCoverageAverages("nonexistent_dir_xyz", 3);
    assert(result == 0.0);

    // Test with a file containing zero stations (should be skipped)
    {
        std::ofstream f(testDir + "/instance5.txt");
        f << "0 10 10\n"; // invalid, skip
    }
    result = computeCoverageAverages(testDir, 6); // valid are 0-3 and 5? instance5 skipped, so 4 valid
    // valid files: 0,1,2,3 => ratios 2,5,1.5,1 => sum 9.5 count 4 => 2.375
    assert(result > 2.374 && result < 2.376);

    // Test with trailing slash in directory path
    result = computeCoverageAverages(testDir + "/", 3);
    assert(result > 2.83333 && result < 2.83334);

    // Clean up
    std::filesystem::remove_all(testDir);
    return 0;
}
// The solution iterates over indices from 0 to `instanceCount-1`, constructing the full file path by concatenating `directoryPath`, a `/` if the path does not end with one, and the filename. For each file, open an `ifstream` and attempt to read three integers. If reading succeeds and the number of stations is greater than zero, compute the ratio (devices / stations) as a double and accumulate it in a sum, incrementing a valid counter. If any read fails or the file cannot be opened, simply skip to the next index. After the loop, if the valid counter is zero, return `0.0`; otherwise return the accumulated sum divided by the valid counter. Edge cases include an empty directory path, missing files, malformed content, and division by zero if stations is zero. The time complexity is O(instanceCount) for file operations and simple arithmetic, with O(1) auxiliary space (excluding filesystem overhead). The algorithm is deterministic and robust to partial failures, ensuring no crashes and graceful handling of invalid data.
