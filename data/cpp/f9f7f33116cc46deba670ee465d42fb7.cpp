// Write a C++ function `countFileInstances` that takes a directory path as a `std::string` and returns a `std::map<std::string, int>` mapping each filename (without path) to the number of times its base name (the part before the first underscore, excluding the file extension) appears across all files in that directory. The function should read a directory using the Windows API (`FindFirstFile`/`FindNextFile`), and for each regular file (ignoring `.` and `..`), extract the base name up to the first underscore (if no underscore exists, use the entire filename without extension). Count occurrences of each base name case-insensitively (convert to uppercase). If the directory cannot be opened, return an empty map. Use only standard C++ and Windows API; do not use `dirent.h` or C++17 filesystem. Ensure the function is robust to filenames with multiple underscores (take only the first underscore) and to files with no extension (strip extension by removing after the last dot). The returned map keys should be the uppercase base names, and values are the counts.

The solution requires enumerating files in a directory using the Windows `FindFirstFile`/`FindNextFile` API. Build a search pattern by appending `\*` to the directory path. Use `WIN32_FIND_DATA` to inspect each entry: skip entries that are directories (attributes `FILE_ATTRIBUTE_DIRECTORY`) and skip `.` and `..` if they appear. For each valid file, convert the wide-character filename (`ffd.cFileName`) to a `std::string` using a helper that wraps `WideCharToMultiByte`. Then extract the base name: first, isolate the part before the first underscore (if any) using `find('_')`; otherwise, use the whole filename. Next, strip the extension by finding the last dot in that substring and removing it (unless there is no dot, then keep as‑is). Convert the resulting base name to uppercase using `toupper` on each character. Insert or increment the count in the map. Edge cases: empty directory, directories with subdirectories (skip them), filenames with no underscore, filenames with no dot, and filenames that differ only by case (they should be merged). The time complexity is \(O(n \cdot L)\) where \(n\) is the number of files and \(L\) is the average filename length; space complexity is \(O(n \cdot L)\) for the map keys and string conversions. The function should return an empty map early if `FindFirstFile` fails.

#include <windows.h>
#include <string>
#include <map>
#include <vector>
#include <cctype>

// Convert wide string to a narrow (UTF-8 / ANSI) string
std::string wideToNarrow(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, nullptr, nullptr);
    return strTo;
}

// Convert a string to uppercase in-place
std::string toUpper(std::string s) {
    for (char& c : s) c = std::toupper(static_cast<unsigned char>(c));
    return s;
}

// Count occurrences of base names (before first underscore) among all files in a directory.
std::map<std::string, int> countFileInstances(const std::string& dir) {
    std::map<std::string, int> result;
    WIN32_FIND_DATA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    DWORD dwError = 0;

    std::string searchPath = dir + "\\*";
    hFind = FindFirstFile(searchPath.c_str(), &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
        return result;  // empty map for inaccessible directory
    }

    do {
        // Skip directories (including "." and "..")
        if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            continue;
        }

        std::wstring wname(ffd.cFileName);
        std::string filename = wideToNarrow(wname);

        // Extract base name up to first underscore (excluding extension)
        std::string base = filename;
        size_t underscorePos = base.find('_');
        if (underscorePos != std::string::npos) {
            base = base.substr(0, underscorePos);
        }
        // Remove extension: find last dot in the base substring
        size_t dotPos = base.find_last_of('.');
        if (dotPos != std::string::npos) {
            base = base.substr(0, dotPos);
        }

        // Uppercase and count
        base = toUpper(base);
        if (!base.empty()) {
            result[base]++;
        }
    } while (FindNextFile(hFind, &ffd) != 0);

    dwError = GetLastError();
    FindClose(hFind);
    // If the loop ended because of an error (not ERROR_NO_MORE_FILES), we still return whatever we collected.
    if (dwError != ERROR_NO_MORE_FILES && dwError != 0) {
        // Optional: could log error; for simplicity keep partial results
    }
    return result;
}

#include <cassert>
#include <string>
#include <map>
#include <fstream>
#include <direct.h>
#include <windows.h>

// The solution function is expected to be defined above (countFileInstances).
// For tests, create a temporary directory with known files.
// Note: The test assumes a Windows environment and uses _mkdir and _chdir.

int main() {
    // Create a unique temp directory under current working directory
    std::string testDir = "test_count_instances";
    _mkdir(testDir.c_str());

    // Create files with known base names (case variations, underscores, extensions)
    std::ofstream(testDir + "\\Alpha_1.txt") << "data";
    std::ofstream(testDir + "\\alpha_2.log") << "data";
    std::ofstream(testDir + "\\ALPHA_test.dat") << "data";   // base: alpha
    std::ofstream(testDir + "\\Beta.txt") << "data";
    std::ofstream(testDir + "\\beta_noext") << "data";       // base: beta
    std::ofstream(testDir + "\\NoUnderscore.txt") << "data"; // base: nounderscore
    std::ofstream(testDir + "\\Gamma_extra_underscore.txt") << "data"; // base: gamma

    // Expected results:
    std::map<std::string, int> expected;
    expected["ALPHA"] = 3;   // Alpha_1.txt, alpha_2.log, ALPHA_test.dat
    expected["BETA"] = 2;    // Beta.txt, beta_noext
    expected["NOUNDERSCORE"] = 1;
    expected["GAMMA"] = 1;   // only first underscore

    std::map<std::string, int> actual = countFileInstances(testDir);

    // Compare maps correctly
    assert(actual.size() == expected.size());
    for (const auto& kv : expected) {
        auto it = actual.find(kv.first);
        assert(it != actual.end());
        assert(it->second == kv.second);
    }

    // Test with a nonexistent directory (should return empty map)
    std::map<std::string, int> empty = countFileInstances("nonexistent_dir_xyz");
    assert(empty.empty());

    // Cleanup: remove test files (simple approach: use system command, but for brevity skip full cleanup)
    // Note: In a real test, you would clean up files and remove directory.

    // Actually, let's clean up using _chdir and remove files to avoid leftover.
    std::remove((testDir + "\\Alpha_1.txt").c_str());
    std::remove((testDir + "\\alpha_2.log").c_str());
    std::remove((testDir + "\\ALPHA_test.dat").c_str());
    std::remove((testDir + "\\Beta.txt").c_str());
    std::remove((testDir + "\\beta_noext").c_str());
    std::remove((testDir + "\\NoUnderscore.txt").c_str());
    std::remove((testDir + "\\Gamma_extra_underscore.txt").c_str());
    _rmdir(testDir.c_str());

    return 0;
}
