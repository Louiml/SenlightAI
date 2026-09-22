// Write a C++ function `collectAndSum` that simulates a parallel MPI-style message collection pattern *without* actually using MPI. The function should take a vector of integers representing process IDs (where each ID is ≥ 1, and the list may contain duplicates or be empty) and return a double precision sum computed as follows: for each process ID `p` in the input vector, the "contribution" is `-p` (i.e., negative of the ID). The function must ignore any ID equal to `0` (treated as the root process that does not send). The function returns the total sum of all contributions. For example, given IDs `{1, 2, 3, 4}`, the sum is `-1 + -2 + -3 + -4 = -10`. If the input is empty, return `0.0`. Ensure the function is const-correct and uses `double` for the sum to match the original snippet’s use of `MPI_DOUBLE`.

#include <cassert>
#include <vector>

// Include the solution function declaration here (or paste the implementation above).
double collectAndSum(const std::vector<int>& processIDs);

int main() {
    // Empty vector
    assert(collectAndSum({}) == 0.0);

    // Single non-zero ID
    assert(collectAndSum({1}) == -1.0);

    // Multiple non-zero IDs
    assert(collectAndSum({1, 2, 3, 4}) == -10.0);

    // Duplicate IDs
    assert(collectAndSum({2, 2, 2}) == -6.0);

    // Including zeros (should be ignored)
    assert(collectAndSum({0, 5, 0, 6}) == -11.0);

    // All zeros
    assert(collectAndSum({0, 0, 0}) == 0.0);

    // Mixed with a larger sequence (simulates numprocs-1 senders)
    std::vector<int> manyIDs = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    assert(collectAndSum(manyIDs) == -55.0);

    return 0;
}

#include <vector>

// Given a vector of process IDs (non-zero IDs are "senders"),
// return the sum of -ID for all non-zero IDs.
double collectAndSum(const std::vector<int>& processIDs) {
    double sum = 0.0;
    for (int id : processIDs) {
        if (id != 0) {
            sum += -static_cast<double>(id);
        }
    }
    return sum;
}

// The solution iterates through each element in the input vector. For each element, we check if it is not equal to `0` (since the root process never sends). If non-zero, we subtract its value from the accumulating sum (which is a `double` to match the original MPI double precision type). Edge cases include: an empty vector (returns 0.0), a vector containing only zeros (returns 0.0, since zeros are ignored), duplicates (each duplicate contributes its negative value), and negative process IDs (though the problem states IDs are ≥1, the function would still work—it just subtracts the value). The algorithm is a simple linear scan, so time complexity is O(n) where n is the size of the vector, and space complexity is O(1) excluding the input storage. No special data structures are needed.
