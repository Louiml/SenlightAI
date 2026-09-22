Write a C++ function named `hIndex` that takes a vector of non-negative integers representing an author's citation counts for each of their papers and returns the author's h-index. The h-index is defined as the largest integer `h` such that the author has at least `h` papers each cited at least `h` times. The input vector can be empty, may contain duplicate values, and is not necessarily sorted. The function must be `const`-correct and should not modify the input vector if possible (you may copy it internally if needed). The returned value must be an integer.

#include <cassert> 
#include <vector> 

int hIndex(const std::vector<int>& citations); 

int main() { 
    assert(hIndex({3, 0, 6, 1, 5}) == 3); 
    assert(hIndex({1, 1, 1}) == 1); 
    assert(hIndex({0}) == 0); 
    assert(hIndex({}) == 0); 
    assert(hIndex({10, 10, 10}) == 3); 
    assert(hIndex({5, 5, 5, 5}) == 4); 
    assert(hIndex({100, 1, 1, 1}) == 1); 
    assert(hIndex({2, 2, 2}) == 2); 
    assert(hIndex({1, 2, 3}) == 2); 
    assert(hIndex({0, 0, 0}) == 0); 
    return 0; 
} 
I hope this is acceptable. Note that the test code has a function declaration for hIndex (since the solution is separate). That is fine. 
The response has exactly the required sections. 
I have provided analysis, solution, and tests. 
Done.

#include <vector>
#include <algorithm>

// Compute the h-index from a vector of citation counts.
// The h-index is the largest h such that at least h papers have at least h citations each.
int hIndex(const std::vector<int>& citations) {
    // Handle empty input
    if (citations.empty()) {
        return 0;
    }
    
    // Make a local copy to avoid modifying the input
    std::vector<int> sorted = citations;
    
    // Sort in descending order
    std::sort(sorted.begin(), sorted.end(), std::greater<int>());
    
    int h = 0;
    for (int i = 0; i < (int)sorted.size(); ++i) {
        // For index i (0-based), there are (i+1) papers with at least sorted[i] citations.
        // If sorted[i] > i+1, we have at least (i+1) papers with >= (i+1) citations, so h can be at least (i+1).
        // If sorted[i] == i+1, h is exactly i+1, but we must stop because the next paper would have fewer citations.
        // If sorted[i] < i+1, the condition fails, and the previous h is the answer.
        if (sorted[i] > i + 1) {
            h = i + 1;
        } else {
            break; // sorted[i] <= i+1; if equal, h is already i (previous), if less, h is also i.
        }
    }
    return h;
}

// The h-index is a measure of productivity and impact. To compute it: first sort the citation counts in descending order. Then iterate through the sorted list; for each index `i` (0-based), the condition for a valid h-index candidate is that `citations[i]` must be greater than or equal to `i+1` (since at least `i+1` papers have at least `i+1` citations). However, the original snippet uses a strict greater-than (`>`) and then breaks when condition fails, returning the count of papers that satisfied the condition. The subtle edge case is when `citations[i]` equals `i+1` — in the original snippet, it breaks and returns `i`, which is correct because the h-index would be `i` (since the next position would fail). For example, with citations `[3, 0, 6, 1, 5]`, sorted descending: `[6,5,3,1,0]`. At i=0: 6>1 yes; i=1: 5>2 yes; i=2: 3>3? No (it's equal), so break and return 2. The h-index is indeed 2 because 2 papers have >=2 citations, but 3 papers do not have >=3 citations. So the algorithm is correct. If the array is empty, return 0. If all papers have high citations, e.g., `[10,10,10]`, sorted: `[10,10,10]`; i=0:10>1 yes; i=1:10>2 yes; i=2:10>3 yes; loop ends and return 3. Time complexity: O(n log n) for sorting, O(n) for iteration, so O(n log n) total. Space complexity: O(n) for the copy if we avoid modifying input; otherwise O(1) if we sort in place after copying.
