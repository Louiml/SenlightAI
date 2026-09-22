// Write a C++ function `std::string complementDNA(const std::string& dna)` that takes a non-empty string consisting only of uppercase letters 'A', 'T', 'C', and 'G' (representing a DNA sequence) and returns a new string where each nucleotide is replaced by its complement: 'A' ↔ 'T' and 'C' ↔ 'G'. The function must not modify the input string (pass by const reference) and should preserve the original order. The returned string should be the DNA complement of the input. You may assume the input contains only valid characters ('A','T','C','G'), but your function must still handle any unexpected character by mapping it to 'C' (as in the snippet's else branch), though this is not expected to occur.

#include <cassert>
#include <string>

// Function prototype (should match the solution)
std::string complementDNA(const std::string& dna);

int main() {
    // Basic cases
    assert(complementDNA("A") == "T");
    assert(complementDNA("T") == "A");
    assert(complementDNA("C") == "G");
    assert(complementDNA("G") == "C");

    // Longer sequence
    assert(complementDNA("ATCG") == "TAGC");

    // Full complement (self-inverse)
    assert(complementDNA("TAGC") == "ATCG");

    // Repeated characters
    assert(complementDNA("AAAA") == "TTTT");
    assert(complementDNA("GGGG") == "CCCC");

    // Mixed sequence
    assert(complementDNA("ACGTACGT") == "TGCATGCA");

    // Edge case: almost empty (but non-empty per spec)
    assert(complementDNA("G") == "C");

    // Ensure input is not modified: use a constant reference test via string literal
    const char* original = "ATGC";
    std::string input(original);
    std::string output = complementDNA(input);
    assert(input == original); // input unchanged

    // All good
    return 0;
}

#include <string>

// Return the DNA complement of a given sequence.
// Maps 'A'<->'T' and 'C'<->'G'. Any other character maps to 'C'.
std::string complementDNA(const std::string& dna) {
    std::string result;
    result.reserve(dna.size()); // Pre-allocate for efficiency

    for (char c : dna) {
        if (c == 'A') {
            result += 'T';
        } else if (c == 'T') {
            result += 'A';
        } else if (c == 'C') {
            result += 'G';
        } else if (c == 'G') {
            result += 'C';
        } else {
            // Fallback for unexpected characters (not expected in valid input)
            result += 'C';
        }
    }

    return result;
}

// The solution iterates over each character in the input string `dna` and creates a new string `result` of the same length. For each character, we apply a direct mapping using an if-else chain or a switch statement: 'A' → 'T', 'T' → 'A', 'C' → 'G', 'G' → 'C', and any other character defaults to 'C' (as per the snippet). The main edge case is an empty string (though the task says non-empty, it’s trivial to handle) — if the input is empty, we return an empty string. Also, since the function takes a const reference, no mutation of the original occurs. Time complexity is O(n) where n is the length of the string, because we process each character exactly once. Space complexity is O(n) for the returned string (which is required to store the result), but no extra auxiliary space beyond that (the result string is the output). The algorithm is straightforward and linear.
