// Implement a C++ `StringSorter` class that manages an array of C-style strings and provides two sorting methods: `sortWithCopy()` and `sortWithMove()`. The class should store `m` strings of length `n` (up to 100 characters each, up to 10000 strings). The sorting methods must sort the strings lexicographically (using standard `strcmp` order) and return the time taken in seconds. The key distinction is that `sortWithCopy()` must perform sorting by copying string data during swaps (e.g., using `strcpy` or temporary buffers), while `sortWithMove()` must avoid copying the actual character data by swapping pointers or using move semantics (e.g., exchanging `char*` pointers). Both methods should sort the internal array in-place and be tested for correctness and performance. The class should include a constructor taking the array size, a copy constructor, and an `operator[]` for assignment of C-strings into the array. The task is to write the complete class definition and its method implementations, ensuring proper memory management (e.g., deep copy in copy constructor) and that the move-based sort is implemented without unnecessary character-by-character copies.

#include <cassert>
#include <cstring>
#include <cstdio>

// Include the StringSorter class definition here (or via header)

int main() {
    // Test 1: Basic sorting with copy
    StringSorter s1(3);
    s1[0] = const_cast<char*>("banana");
    s1[1] = const_cast<char*>("apple");
    s1[2] = const_cast<char*>("cherry");
    s1.sortWithCopy();
    assert(std::strcmp(s1[0], "apple") == 0);
    assert(std::strcmp(s1[1], "banana") == 0);
    assert(std::strcmp(s1[2], "cherry") == 0);

    // Test 2: Basic sorting with move
    StringSorter s2(3);
    s2[0] = const_cast<char*>("zebra");
    s2[1] = const_cast<char*>("alpha");
    s2[2] = const_cast<char*>("moon");
    s2.sortWithMove();
    assert(std::strcmp(s2[0], "alpha") == 0);
    assert(std::strcmp(s2[1], "moon") == 0);
    assert(std::strcmp(s2[2], "zebra") == 0);

    // Test 3: Copy constructor deep copy independence
    StringSorter original(2);
    original[0] = const_cast<char*>("first");
    original[1] = const_cast<char*>("second");
    StringSorter copy(original);
    copy[0] = const_cast<char*>("changed");
    assert(std::strcmp(original[0], "first") == 0);
    assert(std::strcmp(copy[0], "changed") == 0);

    // Test 4: Empty array sorting
    StringSorter s3(0);
    s3.sortWithCopy();
    s3.sortWithMove();

    // Test 5: Single element
    StringSorter s4(1);
    s4[0] = const_cast<char*>("only");
    s4.sortWithCopy();
    assert(std::strcmp(s4[0], "only") == 0);

    // Test 6: Equal strings
    StringSorter s5(3);
    s5[0] = const_cast<char*>("same");
    s5[1] = const_cast<char*>("same");
    s5[2] = const_cast<char*>("same");
    s5.sortWithMove();
    assert(std::strcmp(s5[0], "same") == 0);
    assert(std::strcmp(s5[1], "same") == 0);
    assert(std::strcmp(s5[2], "same") == 0);

    // Test 7: Mixed case and lexicographic order
    StringSorter s6(4);
    s6[0] = const_cast<char*>("Banana");
    s6[1] = const_cast<char*>("apple");
    s6[2] = const_cast<char*>("Cherry");
    s6[3] = const_cast<char*>("banana");
    s6.sortWithMove();
    // ASCII order: 'B' (66) < 'C' (67) < 'a' (97) < 'b' (98)
    assert(std::strcmp(s6[0], "Banana") == 0);
    assert(std::strcmp(s6[1], "Cherry") == 0);
    assert(std::strcmp(s6[2], "apple") == 0);
    assert(std::strcmp(s6[3], "banana") == 0);

    printf("All tests passed.\n");
    return 0;
}

#include <cstring>
#include <utility>

class StringSorter {
public:
    // Constructor: allocate array of char* pointers, initialized to nullptr
    explicit StringSorter(int size) : size_(size), data_(new char*[size_]) {
        for (int i = 0; i < size_; ++i) {
            data_[i] = nullptr;
        }
    }

    // Copy constructor: deep copy all strings
    StringSorter(const StringSorter& other) : size_(other.size_), data_(new char*[size_]) {
        for (int i = 0; i < size_; ++i) {
            if (other.data_[i] != nullptr) {
                int len = std::strlen(other.data_[i]);
                data_[i] = new char[len + 1];
                std::strcpy(data_[i], other.data_[i]);
            } else {
                data_[i] = nullptr;
            }
        }
    }

    // Destructor: free all memory
    ~StringSorter() {
        for (int i = 0; i < size_; ++i) {
            delete[] data_[i];
        }
        delete[] data_;
    }

    // Assignment operator for storing a C-string at index i
    char*& operator[](int index) {
        return data_[index];
    }

    // Sort using copying of string data during swaps (e.g., selection sort)
    void sortWithCopy() {
        for (int i = 0; i < size_ - 1; ++i) {
            int min_idx = i;
            for (int j = i + 1; j < size_; ++j) {
                if (data_[j] != nullptr && data_[min_idx] != nullptr 
                    && std::strcmp(data_[j], data_[min_idx]) < 0) {
                    min_idx = j;
                }
            }
            if (min_idx != i) {
                // Copy-swap: use temporary buffer
                size_t len_i = (data_[i] != nullptr) ? std::strlen(data_[i]) : 0;
                char* temp = new char[len_i + 1];
                if (data_[i] != nullptr) std::strcpy(temp, data_[i]);
                else temp[0] = '\0';

                // Move data_[min_idx] into data_[i] (copy)
                delete[] data_[i];
                size_t len_min = std::strlen(data_[min_idx]);
                data_[i] = new char[len_min + 1];
                std::strcpy(data_[i], data_[min_idx]);

                // Move temp into data_[min_idx] (copy)
                delete[] data_[min_idx];
                data_[min_idx] = new char[len_i + 1];
                std::strcpy(data_[min_idx], temp);

                delete[] temp;
            }
        }
    }

    // Sort using moving pointers (O(1) swap, no character copy)
    void sortWithMove() {
        for (int i = 0; i < size_ - 1; ++i) {
            int min_idx = i;
            for (int j = i + 1; j < size_; ++j) {
                if (data_[j] != nullptr && data_[min_idx] != nullptr 
                    && std::strcmp(data_[j], data_[min_idx]) < 0) {
                    min_idx = j;
                }
            }
            if (min_idx != i) {
                std::swap(data_[i], data_[min_idx]);
            }
        }
    }

private:
    int size_;
    char** data_;
};

// The solution requires managing a dynamic array of C-strings. Each string is stored as a `char*` pointer to a heap-allocated buffer of length `m+1` (including null terminator). The `StringSorter` class holds `char** data_` and `int size_`. The constructor allocates an array of `char*` and initializes each to `nullptr` (or empty strings). The copy constructor must deep-copy all strings: allocate new buffers and copy contents using `strcpy` or `memcpy`. The `operator[]` should return a reference to the `char*` so that assignment like `StrSort1[j] = tmp_str` works; this assignment should allocate a new buffer and copy the input C-string into it (or assign the pointer directly if ownership is transferred, but for safety we copy). For sorting: use a simple selection sort or bubble sort for clarity. In `sortWithCopy()`, during each swap, create a temporary buffer, copy string A into it, copy string B into A, then copy temporary into B (this copies data multiple times). In `sortWithMove()`, during swaps, exchange the `char*` pointers directly (e.g., `std::swap(data_[i], data_[j])`), which is O(1) per swap and copies no character data. Edge cases: empty array, single string, duplicate strings, strings of varying lengths (though task says fixed length, handle general). Time complexity: sorting n strings with O(n^2) comparisons, each comparison is O(m) for `strcmp`; copy-based sort adds O(m) per swap for data copy, while move-based sort is O(1) per swap. Space: O(n*m) for storing strings, O(1) extra for sorting. The copy constructor ensures independent memory so modifying one object doesn't affect the other.
