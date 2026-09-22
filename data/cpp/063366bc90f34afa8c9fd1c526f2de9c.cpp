Write a C++ function named `hateLovePhrase` that takes an integer `n` (`1 <= n <= 100`) representing the number of alternating emotional statements and returns a string following the pattern: for odd positions (1st, 3rd, 5th, ...) the phrase uses "I hate", and for even positions it uses "I love". Consecutive statements are joined by " that ", except the very last statement which ends with " it". The function must handle the general case for any `n`, not just 1, 2, or 3. Return the complete formatted string, with no trailing spaces, and ensure the first letter of the output is capitalized (assume "I" always capital). Example: `n=1` returns `"I hate it"`, `n=2` returns `"I hate that I love it"`, `n=3` returns `"I hate that I love that I hate it"`.

// The solution builds the phrase iteratively. For each index from 1 to `n`, we append either `"I hate"` (if index is odd) or `"I love"` (if index is even). After each segment except the last one, we append `" that "`. After the final segment, we append `" it"` and return the full string. The only edge case is `n=1`, where no " that " appears and the output is just `"I hate it"`. The algorithm runs in O(n) time because it constructs a string of length proportional to n, and uses O(n) auxiliary space for the result string. It is straightforward, with no special overflow or input validation needed for the given constraints.

#include <string>

// Build a phrase of alternating "I hate" / "I love" segments joined by " that ".
std::string hateLovePhrase(int n) {
    std::string result;
    for (int i = 1; i <= n; ++i) {
        if (i % 2 == 1) {
            result += "I hate";
        } else {
            result += "I love";
        }
        if (i != n) {
            result += " that ";
        }
    }
    result += " it";
    return result;
}

#include <cassert>
#include <string>

// Function declaration (could be in a header)
std::string hateLovePhrase(int n);

int main() {
    assert(hateLovePhrase(1) == "I hate it");
    assert(hateLovePhrase(2) == "I hate that I love it");
    assert(hateLovePhrase(3) == "I hate that I love that I hate it");
    assert(hateLovePhrase(4) == "I hate that I love that I hate that I love it");
    assert(hateLovePhrase(5) == "I hate that I love that I hate that I love that I hate it");
    assert(hateLovePhrase(10) == "I hate that I love that I hate that I love that I hate that I love that I hate that I love that I hate that I love it");
    return 0;
}
