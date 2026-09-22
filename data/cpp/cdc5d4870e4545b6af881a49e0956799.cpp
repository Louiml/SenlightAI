// Write a C++ function named `printSubsetSums` that takes an array of integers and its size, and returns a vector of strings, where each string represents one non-empty subset of the original array, with the elements listed in reverse order of their appearance in the original array, separated by spaces. The subsets must be generated in the order of integers from 1 to 2^n - 1, where n is the array size, using the binary representation of each integer to decide which elements to include (1 means include, 0 means exclude). For example, given `{1,3,9,2}`, the first subset (i=1, binary 0001) should include only the last element, so output string "2". The second (i=2, binary 0010) should include only the third element, output "9". The third (i=3, binary 0011) should include the third and fourth elements, output "9 2", and so on. The function must handle arrays of size at least 1, and must not include the empty subset.
// The core idea is to iterate over all integers from 1 to 2^n - 1. For each integer `mask`, its binary representation of length n (padded with leading zeros) determines which elements to include: the least significant bit corresponds to the last array element, the next bit to the second-last, etc. To generate each subset string, we scan the bits from least significant (rightmost) to most significant (leftmost), and for each bit that is 1, we append the corresponding array element (starting from the last index downward). This naturally produces the elements in reverse order of the original array. We can avoid recursion by simply dividing the mask by 2 in a loop, since the bit positions map directly to indices n-1 down to 0. Edge cases: when the array size is 1, only one mask (1) exists, and the output is that single element. For any n, the number of strings is 2^n - 1. Time complexity is O(2^n * n) because for each mask we process n bits. Space complexity is O(2^n * n) for the output vector of strings (excluding the input array itself). We must ensure the function is `const`-correct by taking the array as `const std::vector<int>&` or a pointer + size with const.
#include <vector>
#include <string>

// Returns all non-empty subsets of the input array, each as a string,
// with elements listed in reverse order of the original array.
// Subsets are produced in the order of binary counts from 1 to 2^n - 1.
std::vector<std::string> printSubsetSums(const std::vector<int>& arr) {
    std::vector<std::string> result;
    int n = static_cast<int>(arr.size());
    int total = 1 << n;  // 2^n

    for (int mask = 1; mask < total; ++mask) {
        std::string subset;
        int temp = mask;
        int index = n - 1;
        while (temp > 0) {
            if (temp % 2 == 1) {
                if (!subset.empty()) {
                    subset += " ";
                }
                subset += std::to_string(arr[index]);
            }
            temp /= 2;
            --index;
        }
        result.push_back(subset);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

// (The function printSubsetSums is assumed to be defined above.)

int main() {
    std::vector<int> arr1 = {1, 3, 9, 2};
    std::vector<std::string> res1 = printSubsetSums(arr1);
    assert(res1.size() == 15);
    assert(res1[0] == "2");
    assert(res1[1] == "9");
    assert(res1[2] == "9 2");
    assert(res1[3] == "3");
    assert(res1[4] == "3 2");
    assert(res1[5] == "3 9");
    assert(res1[6] == "3 9 2");
    assert(res1[7] == "1");
    assert(res1[8] == "1 2");
    assert(res1[9] == "1 9");
    assert(res1[10] == "1 9 2");
    assert(res1[11] == "1 3");
    assert(res1[12] == "1 3 2");
    assert(res1[13] == "1 3 9");
    assert(res1[14] == "1 3 9 2");

    std::vector<int> arr2 = {5};
    std::vector<std::string> res2 = printSubsetSums(arr2);
    assert(res2.size() == 1);
    assert(res2[0] == "5");

    std::vector<int> arr3 = {10, 20};
    std::vector<std::string> res3 = printSubsetSums(arr3);
    assert(res3.size() == 3);
    assert(res3[0] == "20");
    assert(res3[1] == "10");
    assert(res3[2] == "10 20");

    return 0;
}
