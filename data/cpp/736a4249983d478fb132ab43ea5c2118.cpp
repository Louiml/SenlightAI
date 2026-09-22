Implement the Strategy design pattern in C++ to model different text-formatting strategies for a document parser. Write a standalone function `applyFormatting(const std::string& text, FormatType type)` that takes raw text and a formatting type (`UPPERCASE`, `LOWERCASE`, or `TRIM`), applies the corresponding strategy, and returns the formatted string. The function must use a polymorphic `TextFormatter` base class with derived classes for each strategy, and a `FormatterContext` class that holds a pointer to the current strategy and allows switching. The function should be `const`-correct, handle empty strings gracefully (return empty), and ensure memory safety by cleaning up dynamically allocated strategies. Only include necessary headers (`<string>`, `<algorithm>`, `<cctype>`, `<memory>`), and do not modify the input string.

#include <cassert>

int main() {
    // Basic uppercase
    assert(applyFormatting("Hello World", FormatType::UPPERCASE) == "HELLO WORLD");
    // Basic lowercase
    assert(applyFormatting("Hello World", FormatType::LOWERCASE) == "hello world");
    // Trim leading/trailing spaces
    assert(applyFormatting("  spaced out  ", FormatType::TRIM) == "spaced out");
    // Empty string returns empty for all strategies
    assert(applyFormatting("", FormatType::UPPERCASE) == "");
    assert(applyFormatting("", FormatType::LOWERCASE) == "");
    assert(applyFormatting("", FormatType::TRIM) == "");
    // String with only whitespace trims to empty
    assert(applyFormatting("   \t\n", FormatType::TRIM) == "");
    // Uppercase and lowercase affect only letters, not digits/punctuation
    assert(applyFormatting("a1!b2@", FormatType::UPPERCASE) == "A1!B2@");
    assert(applyFormatting("A1!B2@", FormatType::LOWERCASE) == "a1!b2@");
    // Trim does not affect internal spaces
    assert(applyFormatting("  keep   internal  ", FormatType::TRIM) == "keep   internal");
}

#include <string>
#include <algorithm>
#include <cctype>
#include <memory>

// Enum for selecting formatting strategy
enum class FormatType {
    UPPERCASE,
    LOWERCASE,
    TRIM
};

// Abstract strategy base class
class TextFormatter {
public:
    virtual ~TextFormatter() = default;
    virtual std::string format(const std::string& text) const = 0;
};

// Concrete strategy: convert to uppercase
class UpperCaseFormatter : public TextFormatter {
public:
    std::string format(const std::string& text) const override {
        std::string result = text;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::toupper(c); });
        return result;
    }
};

// Concrete strategy: convert to lowercase
class LowerCaseFormatter : public TextFormatter {
public:
    std::string format(const std::string& text) const override {
        std::string result = text;
        std::transform(result.begin(), result.end(), result.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        return result;
    }
};

// Concrete strategy: trim leading and trailing whitespace
class TrimFormatter : public TextFormatter {
public:
    std::string format(const std::string& text) const override {
        const auto first = text.find_first_not_of(" \t\n\r\f\v");
        if (first == std::string::npos) {
            return "";  // all whitespace or empty
        }
        const auto last = text.find_last_not_of(" \t\n\r\f\v");
        return text.substr(first, last - first + 1);
    }
};

// Context class that uses a strategy
class FormatterContext {
private:
    std::unique_ptr<TextFormatter> strategy_;
public:
    explicit FormatterContext(std::unique_ptr<TextFormatter> strategy)
        : strategy_(std::move(strategy)) {}

    void setStrategy(std::unique_ptr<TextFormatter> strategy) {
        strategy_ = std::move(strategy);
    }

    std::string format(const std::string& text) const {
        return strategy_->format(text);
    }
};

// Free function that applies formatting based on type
// Returns formatted copy of input text (original unchanged)
std::string applyFormatting(const std::string& text, FormatType type) {
    std::unique_ptr<TextFormatter> formatter;
    switch (type) {
        case FormatType::UPPERCASE:
            formatter = std::make_unique<UpperCaseFormatter>();
            break;
        case FormatType::LOWERCASE:
            formatter = std::make_unique<LowerCaseFormatter>();
            break;
        case FormatType::TRIM:
            formatter = std::make_unique<TrimFormatter>();
            break;
    }
    FormatterContext context(std::move(formatter));
    return context.format(text);
}

// The solution uses the Strategy pattern: `TextFormatter` is an abstract base class with a pure virtual `format(const std::string&) const` method. Three concrete strategies (`UpperCaseFormatter`, `LowerCaseFormatter`, `TrimFormatter`) implement it. `FormatterContext` holds a `std::unique_ptr<TextFormatter>` to manage ownership and provides a `setStrategy` method to switch strategies and a `format` method that delegates to the current strategy. The free function `applyFormatting` constructs a `FormatterContext` with the appropriate strategy based on the `FormatType` enum, calls its `format` method, and returns the result. Edge cases: empty input is handled because all strategies simply return an empty string (uppercase/lowercase of empty is empty, trim of empty is empty). Time complexity is O(n) per format call (where n is the string length) for all strategies: uppercase/lowercase iterate over characters, trim uses `find_first_not_of` and `find_last_not_of` which are linear. Space complexity is O(1) auxiliary for strategies, but O(n) for the returned string due to copying.
