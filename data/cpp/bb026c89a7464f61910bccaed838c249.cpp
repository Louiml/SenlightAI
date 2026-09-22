Write a C++ function `mapOldAliasesToNew` that takes an integer `n` and reads `n` pairs of strings from standard input, where each pair represents an old handle and a new handle for a user (the new handle may itself later be used as an old handle in another pair). The function must simulate handle renaming: when a handle is renamed, all future references to the old handle must resolve to the final current handle, and later renames of an intermediate handle must be propagated backward to any handle that mapped to it. After processing all pairs, the function must print the total number of distinct current handles (i.e., handles that are not overwritten as old names but are final aliases) and then, for each such final handle, print the *original* first-seen handle (the handle that was first introduced) followed by the final current handle, sorted lexicographically by the original first-seen handle. The function must return nothing (void) and use only standard C++ libraries.
The core challenge is maintaining a mapping from each current handle to its *original* first-seen handle, while also tracking which handles have been renamed (so they are not counted as final). A natural approach is to use a map `currentToOriginal` that stores for each current handle the original handle it came from. When processing a pair `(old, new)`, if `old` exists in the map, then we retrieve its original handle, remove `old` from the map, and insert `new` mapped to that original handle. If `old` does not exist, it means `old` is a brand-new handle, so we insert `new` mapped to `old` (since `old` is the original). However, we also need to avoid counting handles that were used as old names as final. The key insight is that after processing all pairs, the keys of the map are exactly the final current handles, and their values are the original handles. The original handles may also appear as keys if they were never renamed, but if they were renamed, they won’t be keys. The map automatically excludes old names because they are erased when renamed. The count of map entries is the answer. To output sorted by original handle, we can copy the map’s pairs into a vector of `pair<string,string>` where the first element is the value (original) and second is the key (final), sort that vector by original, and print. Edge cases: if the input contains a pair where `old` equals `new` (self-rename), the map entry remains unchanged (or we could erase and reinsert, but it’s simpler to handle by checking if old exists and updating accordingly; if old==new, the map remains same; we should avoid erasing). Also, if a handle is used as `new` multiple times or as `old` multiple times, the map handles it correctly because each rename replaces the mapping. Time complexity is O(n log n) due to map operations and sorting; space is O(n) for the map and vector.
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

// Simulate handle renaming and print final original->current mappings sorted by originals.
void mapOldAliasesToNew(int n) {
    std::map<std::string, std::string> currentToOriginal;
    
    for (int i = 0; i < n; ++i) {
        std::string oldName, newName;
        std::cin >> oldName >> newName;
        
        if (oldName == newName) {
            // Self-rename: nothing changes effectively; if oldName existed, keep it.
            if (currentToOriginal.find(oldName) == currentToOriginal.end()) {
                currentToOriginal[oldName] = oldName;
            }
            continue;
        }
        
        if (currentToOriginal.find(oldName) != currentToOriginal.end()) {
            // oldName already existed, get its original and reassign to newName.
            std::string original = currentToOriginal[oldName];
            currentToOriginal.erase(oldName);
            currentToOriginal[newName] = original;
        } else {
            // oldName is new, so newName points to oldName as original.
            currentToOriginal[newName] = oldName;
        }
    }
    
    // Build vector of (original, current) pairs and sort by original.
    std::vector<std::pair<std::string, std::string>> result;
    for (const auto& entry : currentToOriginal) {
        result.push_back({entry.second, entry.first}); // original is value, current is key
    }
    std::sort(result.begin(), result.end());
    
    std::cout << result.size() << std::endl;
    for (const auto& p : result) {
        std::cout << p.first << " " << p.second << std::endl;
    }
}
#include <cassert>
#include <sstream>
#include <iostream>

// Declare the function (since we don't have main in solution, test includes it here)
// For testing, we'll redirect cin to a stringstream and capture cout.
void mapOldAliasesToNew(int n);

int main() {
    // Test case 1: simple rename chain
    {
        std::istringstream input("3\nAslan Nurbol\nNurbol HackMachine\nHackMachine Cyber\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        
        std::ostringstream output;
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());
        
        mapOldAliasesToNew(3);
        
        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);
        
        std::string expected = "1\nAslan Cyber\n";
        assert(output.str() == expected);
    }
    
    // Test case 2: multiple independent users
    {
        std::istringstream input("4\nAlice A\nBob B\nCarol C\nDave D\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        
        std::ostringstream output;
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());
        
        mapOldAliasesToNew(4);
        
        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);
        
        std::string expected = "4\nAlice A\nBob B\nCarol C\nDave D\n";
        assert(output.str() == expected);
    }
    
    // Test case 3: rename to an existing final handle (overwrite)
    {
        std::istringstream input("3\nX Y\nZ Y\nY W\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        
        std::ostringstream output;
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());
        
        mapOldAliasesToNew(3);
        
        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);
        
        // After first: Y->X. Second: Z->Z (since Y exists, but Y is old? Actually second pair: old=Z, new=Y; Z not in map, so Y->Z. Now map has Y->X and Y->Z? No, Y key already exists, so we overwrite: Y->Z. Third: old=Y exists, original=Z, erase Y, insert W->Z. Result: W->Z.
        std::string expected = "1\nZ W\n";
        assert(output.str() == expected);
    }
    
    // Test case 4: self-rename
    {
        std::istringstream input("2\nA A\nA B\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        
        std::ostringstream output;
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());
        
        mapOldAliasesToNew(2);
        
        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);
        
        // First pair: self, map has A->A. Second: old=A exists, original=A, erase A, insert B->A. Result: B->A.
        std::string expected = "1\nA B\n";
        assert(output.str() == expected);
    }
    
    // Test case 5: empty input (n=0)
    {
        std::istringstream input("0\n");
        std::streambuf* oldCin = std::cin.rdbuf(input.rdbuf());
        
        std::ostringstream output;
        std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());
        
        mapOldAliasesToNew(0);
        
        std::cin.rdbuf(oldCin);
        std::cout.rdbuf(oldCout);
        
        std::string expected = "0\n";
        assert(output.str() == expected);
    }
    
    std::cout << "All tests passed!" << std::endl;
    return 0;
}
