Write a C++ function `applyDiscountToPrices` that takes a string `s` containing words separated by single spaces and an integer discount percentage `d` (where 0 ≤ d ≤ 100). For each word in the string that is a valid price (defined as starting with a `$` followed immediately by one or more decimal digits, with no other characters), replace that word with the discounted price format: `$` followed by exactly two decimal places (rounded to the nearest cent, with halves rounded up). All other words must remain unchanged. Preserve the original spacing pattern: the output should have the same number of space-separated tokens as the input, with a single space between consecutive tokens and no leading or trailing spaces. The input string is guaranteed to be non-empty and to contain only printable ASCII characters. The function must handle potentially large price values (up to 10^12) without overflow. Example: input `"price is $100.50"`? Note: the price token must be exactly `$` + digits only (no decimal point or other characters). For instance, `"$100"` is valid, but `"$100.50"` is not. If the input is `"$100 $20 $3.5"`, with discount 10, output should be `"$90.00 $18.00 $3.5"` (since `$3.5` is invalid because it contains a decimal point).

#include <cassert>
#include <string>

int main() {
    // Basic discount
    assert(applyDiscountToPrices("$100 $20 $3", 10) == "$90.00 $18.00 $2.70");
    // Invalid tokens remain unchanged
    assert(applyDiscountToPrices("$100 $3.5 price", 10) == "$90.00 $3.5 price");
    // Single token
    assert(applyDiscountToPrices("$50", 0) == "$50.00");
    assert(applyDiscountToPrices("$50", 100) == "$0.00");
    // Discount 25, price 1 -> $0.75
    assert(applyDiscountToPrices("$1", 25) == "$0.75");
    // No valid prices
    assert(applyDiscountToPrices("hello world", 10) == "hello world");
    // Large value
    assert(applyDiscountToPrices("$1000000000000", 1) == "$990000000000.00");
    // Ensure no extra spaces
    assert(applyDiscountToPrices("a b c", 50) == "a b c");
    // Multiple spaces? Input guarantee single spaces, but test with single
    assert(applyDiscountToPrices("$10 $20", 50) == "$5.00 $10.00");
    return 0;
}

#include <string>
#include <cctype>

// Apply a percentage discount to all valid "$digits" tokens in a space-separated string.
// Returns a new string with discounted prices formatted to two decimal places.
std::string applyDiscountToPrices(const std::string& s, int discount) {
    std::string result;
    size_t i = 0;
    const size_t n = s.size();
    while (i < n) {
        // Extract one token (up to the next space or end).
        size_t j = i;
        while (j < n && s[j] != ' ') {
            ++j;
        }
        std::string token = s.substr(i, j - i);
        
        // Check if token is a valid price: starts with '$', length > 1, rest are digits.
        bool valid = false;
        if (token.size() >= 2 && token[0] == '$') {
            valid = true;
            for (size_t k = 1; k < token.size(); ++k) {
                if (!std::isdigit(static_cast<unsigned char>(token[k]))) {
                    valid = false;
                    break;
                }
            }
        }
        
        if (valid) {
            // Parse the numeric part (guaranteed to fit in long long by task spec).
            long long value = std::stoll(token.substr(1));
            // Compute discounted amount in cents (rounded to nearest cent, halves up).
            long long discountedCents = (value * (100 - discount) * 100 + 5000) / 10000;
            // Alternatively: (value * (100 - discount) + 50) / 100 for cents? Let's compute precisely:
            // price = value * (100 - discount) / 100.0 dollars
            // cents = floor( price * 100 + 0.5 ) = floor( value * (100 - discount) + 0.5 * 100 )
            // = floor( (value * (100 - discount) * 100 + 50) / 100? Wait, careful:
            // price_cents = value * (100 - discount) * 100 / 100 = value * (100 - discount)
            // Actually price in cents = (value * (100 - discount) / 100) * 100 = value * (100 - discount) / 100 * 100 = value * (100 - discount) / 1? No, let's recompute:
            // price_dollars = value * (100 - discount) / 100.0
            // price_cents_raw = price_dollars * 100 = value * (100 - discount) / 100.0 * 100 = value * (100 - discount) / 1.0 = value * (100 - discount) exactly? Wait, that's integer? 
            // Actually value is integer, (100 - discount) is integer, so price_cents = value * (100 - discount) / 1? That's wrong. Let's do: 
            // price = value * (100 - discount) / 100.0
            // price_cents = round( price * 100 ) = round( value * (100 - discount) ) because (price*100) = value * (100 - discount) / 100.0 * 100 = value * (100 - discount) / 1.0? 
            // No: price = value * (100 - discount) / 100.0, so price*100 = value * (100 - discount) / 1.0 = value * (100 - discount) exactly. Wait that means price_cents is exactly value*(100-discount)? That's not right for discount 10 and value 100: price=90, price_cents=9000, value*(100-10)=100*90=9000, yes that is correct. So rounded cents = value*(100-discount) without any rounding? No, but the division by 100.0 is exact because we multiply by 100, which is exactly the denominator. So actually price in cents is exactly integer value*(100-discount) when value is integer? Yes, because (value * (100-discount) / 100.0) * 100 = value*(100-discount) exactly. So no rounding needed, but wait: why does the original snippet use rounding? Because the original may have had decimal inputs? In our task, we only have integer dollar amounts, but the discount can produce fractional cents if we don't multiply by 100. Let's double-check: if value=1, discount=1, then price=0.99, cents=99, value*(100-1)=99, correct. So indeed cents = value*(100-discount) exactly. But hold on: if discount is 10, value=1, price=0.90, cents=90, value*(100-10)=90, yes. So we don't need rounding because the product is exact integer cents. However, to be safe and follow the original pattern, we can still compute with long long and add 50 for rounding? But that's unnecessary. Let's just do cents = value * (100 - discount). But then the output should be "$0.90" etc. So we can directly format. But the problem says round to nearest cent with halves up, which is automatic because the product is integer? Actually if the input is always integer dollars, the discounted price in cents is always integer. So no rounding needed. But for safety, we can do (value * (100 - discount) + 50) / 100? That would give dollars? No, let's just compute dollars and cents directly:
            // dollars_part = (value * (100 - discount)) / 100
            // cents_part = (value * (100 - discount)) % 100
            long long discounted = value * (100 - discount); // this is in cents
            long long dollars = discounted / 100;
            long long cents = discounted % 100;
            // Format to two decimal places.
            std::string formatted = "$" + std::to_string(dollars) + ".";
            if (cents < 10) formatted += "0";
            formatted += std::to_string(cents);
            token = formatted;
        }
        
        // Append the processed token to the result, adding a space if not first token.
        if (!result.empty()) result += ' ';
        result += token;
        
        // Move index past the token and any following space(s) – but input guarantees single spaces.
        i = j + 1; // skip the space
    }
    return result;
}

// The solution processes the input string token by token, splitting on spaces. For each token, we validate whether it is a proper price: it must start with `$`, have length at least 2, and all characters after the `$` must be digits (`'0'`–`'9'`). If valid, we parse the numeric part using `std::stoll` (after taking the substring from index 1) to a `long long`. The discounted value is computed as `value * (100 - d) / 100.0`. To round to the nearest cent with halves up, we compute `roundedCents = floor(valueAfterDiscount * 100 + 0.5)`. Then we format the result as `$` + integer part + `.` + two-digit cents (ensuring leading zero for cents under 10). We do not use floating-point `to_string` directly because it may produce scientific notation or too many decimals. Instead, we convert the rounded cents integer to a fixed two-decimal string manually. The main edge cases include: a token like `"$"` (invalid), a token with non-digit characters (invalid), tokens that are not price-like (unchanged), and discount of 0 or 100 (still works). Time complexity is O(n * L) where n is the number of tokens and L is the average token length (due to string operations), but overall linear in the input length. Space complexity is O(m) for the output string, where m is the length of the result (same order as input).
