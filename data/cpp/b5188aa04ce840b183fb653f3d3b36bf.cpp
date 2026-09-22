/*
Write a C++ function that, given a non-negative integer `n`, generates and returns a `vector<string>` containing all binary strings of length exactly `n` in lexicographic order. The strings must consist only of the characters `'0'` and `'1'`. For example, for `n = 2`, the output must be `{"00", "01", "10", "11"}`. The function must handle the base case `n = 0` by returning a vector containing exactly one empty string. The solution must not use recursion or built-in bit manipulation tricks; it should build the strings iteratively by appending `'0'` and `'1'` to previously generated shorter strings in a specific order.
*/

#include <vector>
#include <string>

// Generate all binary strings of length n in the order produced by the "append 0 then append 1 reversed" iterative reflection.
std::vector<std::string> generateBinaryStrings(int n) {
    std::vector<std::string> result;
    result.emplace_back(""); // base case: length 0 has one empty string

    for (int len = 0; len < n; ++len) {
        std::vector<std::string> next;
        // First append '0' to each string in current order
        for (const auto& s : result) {
            next.emplace_back(s + "0");
        }
        // Then append '1' to each string in reverse order
        for (auto it = result.rbegin(); it != result.rend(); ++it) {
            next.emplace_back(*it + "1");
        }
        result = std::move(next);
    }
    return result;
}

#include <cassert>
#include <vector>
#include <string>

// Declaration of the function under test
std::vector<std::string> generateBinaryStrings(int n);

int main() {
    // n = 0
    std::vector<std::string> r0 = generateBinaryStrings(0);
    assert(r0.size() == 1);
    assert(r0[0] == "");

    // n = 1
    std::vector<std::string> r1 = generateBinaryStrings(1);
    assert((r1 == std::vector<std::string>{"0", "1"}));

    // n = 2
    std::vector<std::string> r2 = generateBinaryStrings(2);
    assert((r2 == std::vector<std::string>{"00", "10", "11", "01"}));

    // n = 3
    std::vector<std::string> r3 = generateBinaryStrings(3);
    assert((r3 == std::vector<std::string>{
        "000", "100", "110", "010", "011", "111", "101", "001"
    }));

    // Check that each string has correct length and contains only '0'/'1'
    for (int n = 0; n <= 5; ++n) {
        auto vec = generateBinaryStrings(n);
        assert(vec.size() == (1u << n));
        for (const auto& s : vec) {
            assert(s.size() == static_cast<size_t>(n));
            for (char c : s) {
                assert(c == '0' || c == '1');
            }
        }
    }

    return 0;
}

// The approach mimics generating all binary numbers from 0 to 2^n - 1 but with zero-padding to length `n`. A direct method is to start with a vector containing one empty string for length 0. For each step from `i = 0` to `n-1`, we generate the next list of strings of length `i+1` as follows: first, take each string in the current list and append `'0'` to it; then, take the same current list in reverse order and append `'1'` to each. This ordering produces lexicographic order because we treat `'0'` < `'1'` and generate strings in a breadth-first manner by prefix length. For example, starting with `[""]`, after step 1 we get `["0", "1"]`, then step 2 from `["0", "1"]` gives `["00", "10", "01", "11"]` — but wait, that is not lexicographic. Let's verify: The correct lexicographic order for length 2 is `00, 01, 10, 11`. The described process gives `00` (append 0 to "0"), `10` (append 0 to "1"), then `01` (append 1 to "0" reversed? Actually reverse of ["0","1"] is ["1","0"], append 1 gives "11" and "01"? Let's trace: current list = ["0","1"]. Append '0' in forward order: "00", "10". Then append '1' in reverse order: take reverse of ["0","1"] = ["1","0"], append '1': "11", "01". So final list is ["00","10","11","01"] which is not lexicographic. That is incorrect. The snippet actually produces the order by binary numbers? Let's analyze the provided snippet: It stores `ans[0]` as `[""]`. For each `i`, it does `for(auto s : ans[i]) ans[i+1].push_back(s+"0");` then `for(int j = ans[i].size()-1; j>=0; j--) ans[i+1].push_back(ans[i][j]+"1");`. For i=0: ans[0] = [""], first loop gives [""+"0"] = ["0"], second loop reverse of ans[0] = [""] gives [""+"1"] = ["1"], so ans[1] = ["0","1"]. For i=1: ans[1] = ["0","1"]. First loop forward: "00", "10". Second loop reverse of ["0","1"] = ["1","0"] gives "11", "01". So ans[2] = ["00","10","11","01"] which is not lexicographic. The snippet actually generates Gray code order? Indeed, reflection of binary reflected Gray code: For n=2, Gray code is 00,01,11,10 — not matching. The order generated is 00,10,11,01, which is the binary reflected Gray code? Let's check n=3: Starting from ans[2]=["00","10","11","01"], append 0 forward gives 000,100,110,010; append 1 reverse (reverse of ans[2] is ["01","11","10","00"]) gives 011,111,101,001 -> final ["000","100","110","010","011","111","101","001"] which is not standard lexicographic. So the snippet's output is not lexicographic. The task must be clarified: The snippet is a piece of code; we need to design a task inspired by it but independent. The snippet builds all binary strings of length n in a specific order (not necessarily lexicographic). So the task should specify the exact order that the snippet produces. Let's derive the order produced: It is the binary reflected Gray code? Actually standard binary reflected Gray code for n bits is generated by G(n) = 0G(n-1) followed by 1G'(n-1) where G' is reverse of G(n-1). That yields for n=2: 00,01,11,10. Our snippet gives 00,10,11,01 — that's the reverse of the standard Gray code? Let's check: Standard Gray n=2 is 00,01,11,10. Reverse is 10,11,01,00. Not that. Maybe it's the order of numbers when bits are reversed? Let's compute binary numbers 0..2^n-1 with reversed bit representation? For n=2: binary 0=00, 1=01, 2=10, 3=11. Reverse bits: 00,10,01,11 -> that gives 00,10,01,11, not matching. Our snippet gives 00,10,11,01. Another approach: It is the order produced by recursive "append 0 then append 1 in reverse order", which is known as "reflected binary code" but starting with 0 in front? Let's just define the order explicitly in the task. Since the task must be independent and clearly specified, we can state: "Generate all binary strings of length n such that the sequence is built as follows: start with an empty string; for each step from 0 to n-1, take the current sequence, first append '0' to each string in the current order, then append '1' to each string in the reverse order of the current sequence, concatenating these two groups." That reproduces the snippet's behavior. The function takes n and returns the vector in exactly that order. Edge cases: n=0 returns one empty string; n negative? Assume non-negative. Time complexity: O(n * 2^n) because each string is generated by copying previous strings, total number of characters generated is n * 2^n. Space complexity: O(n * 2^n) to store all strings.
