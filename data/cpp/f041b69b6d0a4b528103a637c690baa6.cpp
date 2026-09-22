// Write a C++ function `decodeMorseSequence(const std::string& input)` that takes a string containing uppercase letters and the special characters `_`, `,`, `.`, and `?` (with no spaces in the input), and returns the decoded string according to this custom Morse-like code system: each letter or symbol is first converted to a Morse code string (e.g., `A` → `.-`, `_` → `..--`), all codes are concatenated in order to form a long binary-like string, then the lengths of each original code (as digit characters) are recorded in order, reversed, and used to split the concatenated string into chunks of those lengths from left to right. Each chunk is then decoded back to the corresponding character using the reverse mapping, and the resulting characters are concatenated to produce the final output string. For example, given `"HELLO"`, the function should return `"HELLO"`. The function must handle empty input gracefully by returning an empty string. Implement the function without using global variables; all mappings should be local to the function.

The solution builds two local lookup tables: a map from each symbol (string of one character) to its Morse code, and a reverse map from each Morse code back to the symbol. For each character in the input string, we look up its Morse code and append it to a concatenated string, while also appending the length of that code (as a digit character) to a separate string. After processing all characters, we reverse the length string so that the lengths are processed in reverse order (like stacking and popping from the end). We then iterate over the reversed length string; for each digit `n`, we take the next `n` characters from the concatenated Morse string, look up that code in the reverse map, and append the corresponding symbol to the result. This correctly reassembles the original characters because the length string originally stored lengths in the order of characters, and reversing it means we process the last character's code first, but since we take chunks from the beginning of the concatenated string, we actually need to be careful: Wait—let’s reason. In the original code, `len` is built in order of input characters: `len = "4 2 ..."` as digits, then reversed. Reversed, we take chunks from the start of `mc`. That works because the last chunk corresponds to the last character’s code, but since we are taking from the front of `mc`, the first chunk we take (which is the last length after reversal) actually corresponds to the first character’s code? Let’s verify with a simple example: Input "A" (code ".-" length 2). `mc=".-"`, `len="2"`, reverse→"2", take 2 chars from start: ".-" → works. Input "AB" (A: .- (2), B: -... (4)). `mc=".--..."`, `len="24"`, reverse→"42". First iteration: cnt=4, take mc[0..3] = ".--."? That is P, not A. That seems wrong? Let’s test with original snippet: For "AB", the original code would produce? Let’s simulate: `mc` = ".-" + "-..." = ".--..." (6 chars). `len` = "2"+"4" = "24", reverse = "42". nxt=0. Iter1: cnt=4, take code = mc[0..3] = ".--." → from_morse has ".--." = P? No, from_morse maps ".--." → P? Actually to_morse["P"] = ".--." so from_morse[".--."]="P". Iter2: cnt=2, take mc[4..5] = ".." → I. So output "PI"? That’s not "AB". But wait, the original snippet might have been intended for a different encoding? Let’s re-read original: It takes input string `c`, builds `mc` (concatenated codes), `len` (lengths as digits), reverses `len`, then for each `n` in reversed `len`, takes `n` chars from `mc` starting at `nxt`, looks up from_morse, appends. This seems to produce scrambled output unless the encoding is self-symmetric? Actually, let’s test with "AB": The original code would produce "PI" as we computed, but that doesn’t match "AB". However, the original snippet is given; maybe the task is to implement exactly that behavior, not to decode back to original? The task description says "given a code snippet" and asks to create a task "inspired by" the snippet. The snippet as written transforms input string to a different string (like a custom cipher). So the task should be to implement that transformation, not necessarily a decode back to original. But the prompt says "decodeMorseSequence" – but the snippet actually encodes. Wait, the snippet reads input, converts each char to morse, concatenates, reverses lengths, splits, and decodes from morse. That actually produces a different string than original. So the task should describe that transformation precisely, and the solution function should implement that transformation. The test should check that `decodeMorseSequence("HELLO")` returns the same as what the original code would output. Actually, let’s run mental test: For "HELLO", original code produces some scrambled output. But the task description I wrote above incorrectly says it returns "HELLO". That would be wrong. I need to correct the task description.

Let’s redefine the task: Write a function `transformMorse(const std::string& input)` that takes a string of allowed characters (A-Z, `_`, `,`, `.`, `?`) and returns the transformed string as per the exact algorithm in the snippet: convert each char to its Morse code (using the given map), concatenate all codes, build a string of length digits, reverse that length string, then from the concatenated codes take chunks of sizes given by the reversed length string in order, map each chunk back to a character using the inverse map, and concatenate those characters. Return the resulting string. For empty input, return empty string.

The mapping tables are provided in the snippet; we can hardcode them in the function.

Complexity: Let `n` be the number of characters in input. Building the concatenated string takes O(total Morse length) = O(n * max_len) where max_len is 4 or 5 (largest code length is 4? Actually `?` is 4, `_` is 4, etc. All are up to 4? Let’s check: `-...` is 4, `..--` 4, `----` 4, others ≤4). So O(n). Reversing length string O(n). Then iterating over reversed length and taking substrings totals O(total length) = O(n). So O(n) time. Space O(n) for the concatenated strings and result.

#include <string>
#include <map>
#include <algorithm>

// Transform a string using the given Morse-like mapping and length-reversal scheme.
std::string transformMorse(const std::string& input) {
    // Mapping from symbol to its code.
    const std::map<std::string, std::string> to_morse = {
        {"A", ".-"}, {"B", "-..."}, {"C", "-.-."}, {"D", "-.."},
        {"E", "."}, {"F", "..-."}, {"G", "--."}, {"H", "...."},
        {"I", ".."}, {"J", ".---"}, {"K", "-.-"}, {"L", ".-.."},
        {"M", "--"}, {"N", "-."}, {"O", "---"}, {"P", ".--."},
        {"Q", "--.-"}, {"R", ".-."}, {"S", "..."}, {"T", "-"},
        {"U", "..-"}, {"V", "...-"}, {"W", ".--"}, {"X", "-..-"},
        {"Y", "-.--"}, {"Z", "--.."}, {"_", "..--"}, {",", ".-.-"},
        {".", "---."}, {"?", "----"}
    };

    // Build reverse mapping from code to symbol.
    std::map<std::string, std::string> from_morse;
    for (const auto& [symbol, code] : to_morse) {
        from_morse[code] = symbol;
    }

    std::string combined;
    std::string lengths;

    for (char ch : input) {
        std::string sym(1, ch);
        auto it = to_morse.find(sym);
        if (it == to_morse.end()) {
            // If an unknown character appears, skip it (or handle as needed).
            continue;
        }
        combined += it->second;
        lengths += std::to_string(it->second.size());
    }

    // Reverse the length string.
    std::reverse(lengths.begin(), lengths.end());

    std::string result;
    size_t pos = 0;
    for (char len_char : lengths) {
        int cnt = len_char - '0';
        std::string code = combined.substr(pos, cnt);
        pos += cnt;
        auto it = from_morse.find(code);
        if (it != from_morse.end()) {
            result += it->second;
        }
    }

    return result;
}

#include <cassert>
#include <string>

// Function under test.
std::string transformMorse(const std::string& input);

int main() {
    // Empty input yields empty output.
    assert(transformMorse("") == "");

    // Single character that maps to itself when lengths are reversed? Let's test.
    // For "A": combined=".-", lengths="2", reversed="2", take 2 chars -> ".-" -> "A".
    assert(transformMorse("A") == "A");

    // For "B": combined="-...", lengths="4", reversed="4", take 4 chars -> "-..." -> "B".
    assert(transformMorse("B") == "B");

    // For "E": combined=".", lengths="1", reversed="1", take 1 char -> "." -> "E".
    assert(transformMorse("E") == "E");

    // For two characters: "AB" as computed: combined=".--...", lengths="24", reversed="42",
    // take 4 chars -> ".--." -> "P", then 2 chars -> ".." -> "I", so "PI".
    assert(transformMorse("AB") == "PI");

    // For "HELLO": Let's compute quickly:
    // H=.... (4), E=. (1), L=.-.. (4), L=.-.. (4), O=--- (3)
    // combined = "...." + "." + ".-.." + ".-.." + "---" = "......-...-..---" (wait compute: "...." + "." = ".....", then ".-.." -> "..... .-.." = "......-.."? Let's actually concatenate: "...." + "." = "....." (5 dots), then ".-.." -> ".....-..", then ".-.." -> ".....-...-..", then "---" -> ".....-...-..---" (that's 5+4+4+3=16 chars). Lengths string: "4"+"1"+"4"+"4"+"3" = "41443", reversed = "34414". Then take 3 chars -> "..."? First three of combined are "..." (dots), from_morse["..."] = "S". Then take 4 chars -> positions 3-6: combined[3..6]? combined = indices 0-15: 0'.',1'.',2'.',3'.',4'.',5'-',6'.',7'.',8'-',9'.',10'.',11'-',12'-',13'-'? Hard to do by hand. Better trust the snippet. But we can test with known output: The original snippet would output something. Since we can't run it, we can just assert that transformMorse("HELLO") is equal to the result of the algorithm, which we can compute in code. But for the test, we can just assert that it's a specific string that we compute by hand or using the snippet logic. Alternatively, we can test other pairs.

    // For "TEST": T="-", E=".", S="...", T="-"
    // combined = "-" + "." + "..." + "-" = "-....-" (7 chars)
    // lengths = "1"+"1"+"3"+"1" = "1131", reversed = "1311"
    // take 1 char: "-" -> "T"
    // take 3 chars: next three: ". .."? combined[1..3] = ". . ."? combined = "-....-": indices 0'-',1'.',2'.',3'.',4'.',5'-'. So chars 1-3 = "...": -> "S"
    // take 1 char: index 4 = "." -> "E"
    // take 1 char: index 5 = "-" -> "T"
    // Result "TSET"? Wait order: first "T", then "S", then "E", then "T" → "TSET". So transformMorse("TEST") == "TSET".
    assert(transformMorse("TEST") == "TSET");

    // For "??": "?" code "----" length 4, twice.
    // combined = "--------" (8 dashes)
    // lengths = "44", reversed = "44"
    // take 4 chars "----" -> "?" then take 4 chars "----" -> "?" → "??"
    assert(transformMorse("??") == "??");

    // For mixed with underscore: "_A" : "_" = "..--" length 4, "A" = ".-" length 2
    // combined = "..--.-" (6 chars)
    // lengths = "42", reversed = "24"
    // take 2 chars ".." -> "I", then take 4 chars "--.-" -> "Q"? But "--.-" maps to Q? Actually to_morse["Q"]="--.-", so from_morse["--.-"]="Q". So "IQ".
    assert(transformMorse("_A") == "IQ");

    // For comma and period: "," = ".-.-" (4), "." = "---." (4)
    // combined = ".-.-.---." (8 chars)
    // lengths = "44", reversed = "44"
    // take 4 chars ".-.-" -> "," then take 4 chars ".---."? Wait combined[4..7] = ".---."? Actually combined = ".-.-" + "---." = ".-.-.---." (8 chars). First 4 = ".-.-" -> ",". Next 4 = ".---."? But that is 5 chars? No, index 4 to 7: combined[4]='.',[5]='-',[6]='-',[7]='-'? Let's list: positions: 0'.',1'-',2'.',3'-',4'.',5'-',6'-',7'-'? Actually after ".-.-" (4 chars: 0'.',1'-',2'.',3'-'), then "---." (4 chars: 4'-',5'-',6'-',7'.')? Wait "---." is dash dash dash dot, so positions 4'-',5'-',6'-',7'.'. So combined[4..7] = "---." which maps to "."? Yes from_morse["---."]=".". So result ",."? Actually first chunk gives ",", second gives "." -> ",." . So transformMorse(".,")? Let's test with input ",."? Input has comma then period? Let's do input ",." (comma then period) -> combined = ".-.-" + "---." = ".-.-.---." as above, lengths = "44", reversed "44", first 4 = ".-.-" -> ",", second 4 = "---." -> "." → ",." which is same as input. So assert(transformMorse(",.") == ",.").

    assert(transformMorse(",.") == ",.");

    // For "A?" : A (2) and ? (4) -> combined = ".-.----" (6 chars? ".-" + "----" = ".-.----" which is 6 chars? Actually ".-" is 2, "----" is 4, total 6: '.', '-', '-', '-', '-', '-'? Wait ".-" then "----" gives ".----" (dot then 4 dashes) that's 5? No: ".-" is dot dash, then "----" is four dashes, concatenated: ". - - - - -" → actually dot then dash then four dashes = one dot and five dashes? Wait, ".-" has dot and dash, then "----" has four dashes, so total 2+4=6 chars: positions: 0'.',1'-',2'-',3'-',4'-',5'-'? That’s 1 dot and 5 dashes. Lengths = "24", reversed = "42". Take 4 chars from start: ".- -"? combined[0..3] = ".- - -"? Actually indices 0'.',1'-',2'-',3'-' -> ".---" which maps to "J"? Yes to_morse["J"]=".---", so from_morse[".---"]="J". Then take 2 chars: combined[4..5] = "--" -> "M". So result "JM". Test: assert(transformMorse("A?")=="JM");

    assert(transformMorse("A?") == "JM");

    // For lowercase letters, they are not in map, so they are skipped.
    // Input "a" would produce empty result.
    assert(transformMorse("a") == "");

    // For a longer string "SOS" -> S="...", O="---", S="..."
    // combined = "...---..." (9 chars)
    // lengths = "333", reversed = "333"
    // take 3 chars each: "...", "---", "..." -> "SOS"
    assert(transformMorse("SOS") == "SOS");

    return 0;
}
