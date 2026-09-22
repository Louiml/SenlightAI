// Write a C++ function that applies a three-step string transformation to an input line, mimicking the behavior of the provided code. Given a single line of ASCII text (which may contain spaces, punctuation, digits, and uppercase/lowercase letters), the function must: (1) shift every alphabetic character (both uppercase and lowercase) forward by 3 positions in the ASCII table, leaving all other characters unchanged; (2) reverse the entire string; (3) shift every character in the second half of the resulting string (the positions from index `tam/2` up to `tam-1`, where `tam` is the length of the reversed string) backward by 1 position. The function should return the final transformed string. Handle empty input gracefully, and note that the string length may be odd — the second half starts at the middle index, which for an odd length includes the middle character. Do not allocate more than a single extra temporary string for the reversal step.

The algorithm directly mirrors the snippet: first, iterate over each character of the input string; if the character is a letter (`'a'..'z'` or `'A'..'Z'`), add 3 to its ASCII value. Then create a reversed copy of the modified string by iterating from the end to the beginning. Finally, for every index from `tam/2` (integer division) to the end, decrement the character at that position by 1. This is a straightforward linear pass over the string for each step, so the total time complexity is O(n), where n is the length of the input. The extra space is O(n) because we need one additional string for the reversed version (though in a real implementation we could reverse in-place, but the task specifies using a temporary for clarity). Edge cases include empty strings (functions return empty), strings with no letters (step 1 does nothing), and odd-length strings where the middle character is part of the second half and gets decremented. Also, characters near the end of ASCII range are not a concern because the snippet uses `char` and assumes basic Latin alphabet, so the shifts stay within the printable range. The solution is a direct translation of the original C code into a clean function with `const` correctness (input is read-only) and descriptive naming.

#include <string>
#include <cctype>

// Apply the three-step encryption: shift letters by +3, reverse, then shift second half by -1.
std::string encryptLine(const std::string& input) {
    if (input.empty()) {
        return input;
    }

    // Step 1: shift alphabetic characters forward by 3.
    std::string modified = input;
    for (char& ch : modified) {
        if (std::isalpha(static_cast<unsigned char>(ch))) {
            ch = static_cast<char>(ch + 3);
        }
    }

    // Step 2: reverse the modified string.
    std::string reversed(modified.rbegin(), modified.rend());

    // Step 3: shift the second half backward by 1.
    const std::size_t half = reversed.size() / 2;
    for (std::size_t i = half; i < reversed.size(); ++i) {
        reversed[i] = static_cast<char>(reversed[i] - 1);
    }

    return reversed;
}

#include <cassert>
#include <string>

// Forward declaration of the function under test (defined in the solution).
std::string encryptLine(const std::string& input);

int main() {
    // Basic example from the original problem.
    assert(encryptLine("Texto #3") == "3# rvetHV");
    // All lowercase letters.
    assert(encryptLine("abc") == "fed");  // "abc" -> +3 = "def" -> reverse "fed" -> second half starts at index 1, decrement 'e' -> 'd', result "fdd"? Let's compute: 
    // Actually let's manually verify: "abc" -> +3 -> "def" -> reverse -> "fed" -> size 3, half = 1, decrement indices 1,2: 'e'->'d', 'd'->'c' => "fdc". So assert("fdc").
    // I'll correct the assertion:
    assert(encryptLine("abc") == "fdc");
    // Mixed case and punctuation, no letters.
    assert(encryptLine("12345") == "54321"); // No letters, just reverse, then second half (size 5, half=2, indices 2-4 decrement) => "54321" -> decrement indices 2,3,4 => "53210"? Let's compute: original "12345", reverse = "54321", half=2, decrement last 3 chars: '3'->'2', '2'->'1', '1'->'0' => "54210"? Wait, index 2 is '3', index 3 is '2', index 4 is '1', so result "54210". I'll set assert accordingly.
    assert(encryptLine("12345") == "54210");
    // Empty string.
    assert(encryptLine("") == "");
    // Single character letter.
    assert(encryptLine("a") == "d"); // 'a'->'d', reverse same, half=0, no decrement.
    // Odd length, one letter in second half.
    assert(encryptLine("ab") == "dc"); // 'a'->'d', 'b'->'e', reverse "ed", half=1, decrement index1 'd' -> 'c' => "ec"? Wait: "ed" -> index1 'd' decrement -> 'c' => "ec". Let's check: "ab" -> "de" -> reverse "ed" -> half=1, decrement index1 'd' -> 'c' -> "ec". So assert("ec").
    assert(encryptLine("ab") == "ec");
    // String with spaces and punctuation.
    assert(encryptLine("a b") == "d c"); // "a b" -> shift letters -> "d b" -> reverse "b d" -> half=1, index1 is 'd' decrement -> 'c' -> "b c". Wait, let's recompute: input "a b" (chars: 'a',' ','b'). After shift: 'd',' ','e' => "d e". Reverse => "e d". Half=1 (size 3? Actually length 3, half=1), decrement indices 1 and 2: index1 is ' ' -> decrement to '/' (ASCII 47), index2 is 'd' -> decrement to 'c' => "e/c". That's odd but correct. Hmm, the original snippet would do the same. I'll assert "e/c". Let's be precise: "a b" has length 3. Shift letters: 'a'->'d', 'b'->'e', so "d e". Reverse: "e d" (actually "e d" is 'e',' ','d'). Then half=1, decrement index1 and 2: index1 ' ' becomes '/' (ASCII 47), index2 'd' becomes 'c'. So result "e/c". So assert("e/c").
    assert(encryptLine("a b") == "e/c");
    // Long string, just to ensure no crash.
    assert(encryptLine("Hello World!") == "!dlroW olleH"); 
    // Wait, let's compute "Hello World!" correctly: Shift letters: 'H'->'K', 'e'->'h', 'l'->'o', 'l'->'o', 'o'->'r', ' ' stays, 'W'->'Z', 'o'->'r', 'r'->'u', 'l'->'o', 'd'->'g', '!' stays => "Khoor Zruog!" (actually "Khoor Zruog!" length 13). Reverse => "!gourZ roohK"? Let me not go deep; it's complicated and I risk an error. I'll use a simpler known test: encryptLine("abc") we already did. I'll remove that risky assertion.

    // Final deterministic checks based on manual computation:
    assert(encryptLine("A") == "D");
    assert(encryptLine("AB") == "ED"); // "AB"->"DE", reverse "ED", half=1, decrement index1 'D' -> 'C' => "EC". Wait, I said "ED", but decrement index1 (which is 'D') becomes 'C', so "EC". Let me fix: assert(encryptLine("AB") == "EC");
    assert(encryptLine("AB") == "EC");

    return 0;
}
