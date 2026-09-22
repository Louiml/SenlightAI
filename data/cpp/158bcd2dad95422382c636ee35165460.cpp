// Write a C++ function named `printAddresses` that takes a constant reference to a `std::array<int, 4>` and an output stream reference (defaulting to `std::cout`), and prints for each element its index, the memory address of the element (using `&arr[i]`), and the element's value, in exactly the format: `"Address of element <i> is: <address> (value <value>)\n"` for `i` from 0 to 3. The function must not modify the array or the stream's state beyond normal output. The task is to demonstrate understanding of pointer/address printing in C++ with const-correctness and stream handling. The function should return `void`.

The main algorithm is straightforward: iterate over the array using a `for` loop from `i = 0` to `i < arr.size()`. For each iteration, output the index, the address of `arr[i]` using the `&` operator (which yields a `const int*` since the array is constant), and the value. Use the output stream passed as a parameter to allow flexibility (e.g., printing to `std::cerr` or a file stream). Edge cases: the array size is fixed at 4, so no bounds checking is needed; the array is constant, so we must use `const int*` for addresses, but the formatting works the same. Time complexity is O(4) = O(1) since the size is fixed; space complexity is O(1). No special edge cases other than ensuring the stream is valid, but we assume it is.

#include <array>
#include <iostream>

// Prints the address and value of each element in a fixed-size array of 4 ints.
// Uses the provided output stream (default std::cout) for output.
void printAddresses(const std::array<int, 4>& arr, std::ostream& os = std::cout) {
    for (std::size_t i = 0; i < arr.size(); ++i) {
        os << "Address of element " << i << " is: " << &arr[i]
           << " (value " << arr[i] << ")" << std::endl;
    }
}

#include <array>
#include <sstream>
#include <cassert>

int main() {
    std::array<int, 4> test = {10, 20, 30, 40};
    std::ostringstream oss;
    printAddresses(test, oss);
    std::string output = oss.str();

    // Since addresses are runtime-dependent, verify the structure by checking substrings.
    assert(output.find("Address of element 0 is: 0x") != std::string::npos);
    assert(output.find("(value 10)") != std::string::npos);
    assert(output.find("Address of element 1 is: 0x") != std::string::npos);
    assert(output.find("(value 20)") != std::string::npos);
    assert(output.find("Address of element 2 is: 0x") != std::string::npos);
    assert(output.find("(value 30)") != std::string::npos);
    assert(output.find("Address of element 3 is: 0x") != std::string::npos);
    assert(output.find("(value 40)") != std::string::npos);

    // Verify that addresses are increasing (typical for contiguous array elements).
    std::size_t pos0 = output.find("is: ") + 4;
    std::size_t pos1 = output.find("is: ", pos0) + 4;
    std::size_t pos2 = output.find("is: ", pos1) + 4;
    std::size_t pos3 = output.find("is: ", pos2) + 4;
    std::string addr0 = output.substr(pos0, output.find(" ", pos0) - pos0);
    std::string addr1 = output.substr(pos1, output.find(" ", pos1) - pos1);
    std::string addr2 = output.substr(pos2, output.find(" ", pos2) - pos2);
    std::string addr3 = output.substr(pos3, output.find(" ", pos3) - pos3);
    // (Note: This simple substring test may need adjustment if the platform prints addresses with prefixes, but for typical hex output it works.)
    assert(addr0 < addr1 && addr1 < addr2 && addr2 < addr3);

    // Test with a different stream (e.g., cerr-like) but here we just use another ostringstream.
    std::ostringstream oss2;
    printAddresses(test, oss2);
    // Reuse a simple equality: each line ends with newline, count lines.
    int newlines = 0;
    for (char c : oss2.str()) if (c == '\n') ++newlines;
    assert(newlines == 4);
}
