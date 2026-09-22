// Write a C++ function `modeAlongDimension` that takes a 2D matrix (represented as `std::vector<std::vector<T>>`) and an integer `d` (1 for column-wise mode, 2 for row-wise mode), and returns a 1D vector containing the mode (most frequent value) of each column (if `d==1`) or each row (if `d==2`). The input matrix is guaranteed to be non-empty and rectangular. If multiple values tie for the mode, choose the one that appears first (i.e., the one with the smallest index) when scanning the dimension. The function must be templated on the value type `T` and handle types comparable with `==`. The result should be stored in a `std::vector<T>` and returned by value. Include full template implementation and ensure it compiles independently.

#include <cassert>
#include <vector>

int main() {
    // Test 1: Column-wise mode of a simple matrix
    std::vector<std::vector<int>> mat1 = {{1, 2, 3}, {1, 2, 4}, {5, 2, 3}};
    std::vector<int> colMode = modeAlongDimension(mat1, 1);
    assert(colMode.size() == 3);
    assert(colMode[0] == 1); // column 0: 1,1,5 -> mode 1
    assert(colMode[1] == 2); // column 1: 2,2,2 -> mode 2
    assert(colMode[2] == 3); // column 2: 3,4,3 -> mode 3 (3 appears first)

    // Test 2: Row-wise mode with ties broken by first occurrence
    std::vector<std::vector<int>> mat2 = {{1, 2, 1, 2}, {3, 3, 4, 4}};
    std::vector<int> rowMode = modeAlongDimension(mat2, 2);
    assert(rowMode.size() == 2);
    assert(rowMode[0] == 1); // row 0: 1 appears at index 0, 2 at index 1 -> tie, choose 1
    assert(rowMode[1] == 3); // row 1: 3 appears at index 0, 4 at index 2 -> tie, choose 3

    // Test 3: Single row/column matrix
    std::vector<std::vector<double>> mat3 = {{2.5, 2.5, 3.5}};
    std::vector<double> mode3 = modeAlongDimension(mat3, 2);
    assert(mode3.size() == 1);
    assert(mode3[0] == 2.5);

    // Test 4: All values distinct, mode is first element
    std::vector<std::vector<char>> mat4 = {{'a', 'b', 'c'}, {'d', 'e', 'f'}};
    std::vector<char> mode4c = modeAlongDimension(mat4, 1);
    assert(mode4c.size() == 3);
    assert(mode4c[0] == 'a');
    assert(mode4c[1] == 'b');
    assert(mode4c[2] == 'c');

    // Test 5: Negative and repeated values
    std::vector<std::vector<int>> mat5 = {{-1, -1, 3}, {-1, 2, 3}, {4, 2, 3}};
    std::vector<int> mode5 = modeAlongDimension(mat5, 1);
    assert(mode5.size() == 3);
    assert(mode5[0] == -1); // column 0: -1 appears twice
    assert(mode5[1] == 2);  // column 1: 2 appears twice (tie with -1? no, -1 appears once)
    assert(mode5[2] == 3);  // column 2: 3 appears three times

    // Test 6: Rectangular matrix with varying row lengths? Not allowed, but we assume rectangular.
    // Test with 1x1 matrix
    std::vector<std::vector<int>> mat6 = {{42}};
    std::vector<int> mode6 = modeAlongDimension(mat6, 1);
    assert(mode6.size() == 1 && mode6[0] == 42);
    mode6 = modeAlongDimension(mat6, 2);
    assert(mode6.size() == 1 && mode6[0] == 42);

    // Test 7: Large matrix with duplicates and tie-breaking (vector of strings)
    std::vector<std::vector<std::string>> mat7 = {{"apple", "banana", "apple"}, {"cherry", "banana", "cherry"}};
    std::vector<std::string> mode7c = modeAlongDimension(mat7, 1);
    assert(mode7c.size() == 3);
    assert(mode7c[0] == "apple"); // column 0: apple, cherry -> tie, apple first
    assert(mode7c[1] == "banana");
    assert(mode7c[2] == "apple"); // column 2: apple, cherry -> tie, apple first

    return 0;
}

#include <vector>
#include <map>
#include <cassert>

// Compute the mode (most frequent value) along each column (d==1) or each row (d==2) of a 2D matrix.
// Ties are broken by choosing the value that appears first when scanning the column/row.
// The input matrix must be non-empty and rectangular (all rows have same length).
template <typename T>
std::vector<T> modeAlongDimension(const std::vector<std::vector<T>>& X, int d) {
    assert(d == 1 || d == 2);
    const int m = static_cast<int>(X.size());
    assert(m > 0);
    const int n = static_cast<int>(X[0].size());
    assert(n > 0);

    std::vector<T> result;
    if (d == 1) {
        // Column-wise mode: for each column j, find mode over rows.
        result.resize(n);
        for (int j = 0; j < n; ++j) {
            std::map<T, int> counts;
            std::vector<T> order;  // first-appearance order
            for (int i = 0; i < m; ++i) {
                const T& val = X[i][j];
                if (counts.find(val) == counts.end()) {
                    counts[val] = 1;
                    order.push_back(val);
                } else {
                    counts[val]++;
                }
            }
            // Find maximum count, breaking ties by earliest appearance in order.
            int best_count = -1;
            T best_val = T{};
            for (const T& val : order) {
                if (counts[val] > best_count) {
                    best_count = counts[val];
                    best_val = val;
                }
            }
            result[j] = best_val;
        }
    } else {  // d == 2
        // Row-wise mode: for each row i, find mode over columns.
        result.resize(m);
        for (int i = 0; i < m; ++i) {
            std::map<T, int> counts;
            std::vector<T> order;
            for (int j = 0; j < n; ++j) {
                const T& val = X[i][j];
                if (counts.find(val) == counts.end()) {
                    counts[val] = 1;
                    order.push_back(val);
                } else {
                    counts[val]++;
                }
            }
            int best_count = -1;
            T best_val = T{};
            for (const T& val : order) {
                if (counts[val] > best_count) {
                    best_count = counts[val];
                    best_val = val;
                }
            }
            result[i] = best_val;
        }
    }
    return result;
}

// The algorithm processes each target column or row independently. For `d==1`, we iterate over each column index `j` (0 to n-1), and for each column, we count the occurrences of each distinct value by scanning all rows. To find the mode, we create a frequency count for each unique value encountered, but since values are typed `T`, we cannot use an array. Instead, we use a `std::map<T, int>` to count occurrences in the order first encountered, then pick the key with the highest count; ties are broken by choosing the earliest encountered value because we iterate the map in insertion order (if we insert only on first occurrence). However, a simpler approach is to use a `std::unordered_map<T, int>` but then tie-breaking requires extra bookkeeping. Since the problem statement requires the first-appearing value on ties, we can use a `std::map<T, int>` because it stores keys in sorted order, which is not the same as first-appearance order. Therefore, we must manually track first occurrence order: use a vector of pairs (value, count), and when a new value appears, append it; when an existing value appears, increment its count. After scanning the column/row, iterate over this vector in insertion order and pick the value with the maximum count, updating on strict greater only. For `d==2`, iterate over each row index `i` and apply the same logic to the elements in that row. Time complexity is O(n * m * log(min(n,m))) if using map per iteration, but with the vector-of-pairs approach it's O(n * m * min(n,m)) in the worst case because finding an existing value is linear. However, we can optimize by using `std::map` for counting while also tracking first occurrence order via a separate vector, yielding O(n * m * log(min(n,m))). To keep the solution simple, we'll use a `std::map<T, int>` plus a vector of unique keys to maintain first-appearance order, achieving O(n * m * log(min(n,m))) time. Space complexity is O(min(n,m)) for the counts plus O(n) or O(m) for the output vector.
