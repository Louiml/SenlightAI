// Given an initially empty multiset of real numbers and a sequence of operations, write a C++ function that processes the operations and returns a vector of strings containing the current average of the multiset, formatted with 20 decimal places, after every operation. The supported operations are: type 1 with parameters `a` (integer position index, 1 ≤ a ≤ top) and `x` (real value) means add `x` to the element at position `a` (i.e., increase that element's value by `x`); type 2 with parameter `x` means append a new element with value `x` to the end of the sequence; type 3 means remove the last element of the sequence. The multiset always contains at least one element (the first element starts at value 0). The function receives an integer `q` (number of operations, 1 ≤ q ≤ 2×10^5) and a vector of operations, each operation encoded as a vector of doubles (first element is type, then parameters). Output the average (sum / count) after each operation with 20 decimal digits.

// For each element in the sequence, we store its base value (as initially appended or as the initial 0) and an accumulated increment to that specific element. We maintain the total sum as a double. For type 1, we add `x` to the `extra` field of the element at index `a` (0‑based in storage) and add `x` to `sum`. For type 2, we push a new pair `{x, 0.0}` and add `x` to `sum`. For type 3, we pop the last element, subtract its current value (`first + second`) from `sum`, and remove it. Because the operation always refers to a fixed position from the bottom, removals from the top do not affect the indices of lower elements, so no shifting or merging is needed. After each operation, compute `sum / size` and format it with `std::fixed` and `std::setprecision(20)`. Edge cases: type 3 never makes the sequence empty; type 1 never references an index beyond current size. The algorithm runs in O(q) time and O(q) space for the sequence.

#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

// Process a sequence of operations and return the average after each operation.
// Each operation is a vector of doubles: {type, params...}
std::vector<std::string> processSequence(int q, const std::vector<std::vector<double>>& operations) {
    std::vector<std::pair<double, double>> seq; // {base_value, extra_additions}
    seq.push_back({0.0, 0.0});
    double sum = 0.0;
    std::vector<std::string> result;
    result.reserve(q);
    
    for (const auto& op : operations) {
        int type = static_cast<int>(op[0]);
        if (type == 1) {
            int a = static_cast<int>(op[1]); // 1-indexed from bottom
            double x = op[2];
            seq[a - 1].second += x;
            sum += x;
        } else if (type == 2) {
            double x = op[1];
            seq.push_back({x, 0.0});
            sum += x;
        } else if (type == 3) {
            const auto& back = seq.back();
            sum -= (back.first + back.second);
            seq.pop_back();
        }
        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(20) << (sum / static_cast<double>(seq.size()));
        result.push_back(oss.str());
    }
    return result;
}

#include <cassert>
#include <cmath>
#include <vector>
#include <string>

// The solution function is assumed to be defined above.

int main() {
    // Test 1: simple append and average
    {
        std::vector<std::vector<double>> ops = {
            {2, 10.0},
            {2, 20.0}
        };
        auto res = processSequence(2, ops);
        assert(res.size() == 2);
        // initial [0], after append 10 -> [0,10] avg 5.0
        // after append 20 -> [0,10,20] avg 10.0
        double v1 = stod(res[0]);
        double v2 = stod(res[1]);
        assert(std::abs(v1 - 5.0) < 1e-12);
        assert(std::abs(v2 - 10.0) < 1e-12);
    }
    
    // Test 2: add to a position and pop
    {
        std::vector<std::vector<double>> ops = {
            {2, 4.0},   // [0,4]
            {1, 1, 2.0}, // add 2 to position 1 -> [2,4] sum 6 avg 3.0
            {3},        // pop 4 -> [2] sum 2 avg 2.0
            {3}         // pop 2 -> [0] sum 0 avg 0.0
        };
        auto res = processSequence(4, ops);
        assert(std::abs(stod(res[0]) - 2.0) < 1e-12); // after append 4: [0,4] avg 2
        assert(std::abs(stod(res[1]) - 3.0) < 1e-12);
        assert(std::abs(stod(res[2]) - 2.0) < 1e-12);
        assert(std::abs(stod(res[3]) - 0.0) < 1e-12);
    }
    
    // Test 3: multiple additions to same position
    {
        std::vector<std::vector<double>> ops = {
            {1, 1, 5.0}, // [5]
            {1, 1, 3.0}, // [8]
            {2, 2.0}     // [8,2] sum 10 avg 5
        };
        auto res = processSequence(3, ops);
        assert(std::abs(stod(res[0]) - 5.0) < 1e-12);
        assert(std::abs(stod(res[1]) - 8.0) < 1e-12);
        assert(std::abs(stod(res[2]) - 5.0) < 1e-12);
    }
    
    // Test 4: precision check - 20 decimal places
    {
        std::vector<std::vector<double>> ops = {
            {2, 0.1},
            {2, 0.2}
        };
        auto res = processSequence(2, ops);
        // averages: first [0,0.1] avg 0.05, second [0,0.1,0.2] avg 0.1
        // Check that the string has exactly 20 digits after decimal point
        auto pos = res[0].find('.');
        assert(pos != std::string::npos);
        assert(res[0].size() - pos - 1 == 20);
        assert(std::abs(stod(res[0]) - 0.05) < 1e-12);
        assert(std::abs(stod(res[1]) - 0.1) < 1e-12);
    }
    
    // Test 5: only initial element
    {
        std::vector<std::vector<double>> ops = {};
        auto res = processSequence(0, ops);
        assert(res.empty());
    }
    
    return 0;
}
