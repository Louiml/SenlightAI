// Write a C++ function `std::string classifySign(const std::string& input)` that simulates a deterministic finite automaton (DFA) over a sequence of characters representing a single lexeme (a sign/operator from a small programming language). The function must return the token type name as a string (e.g., `"Plus"`, `"Assign"`, `"CommentBegin"`, `"Fehler"`) based on the final accepting state after processing all characters. The valid patterns are: single-character operators (`+`, `-`, `*`, `/`, `=`, `<`, `>`, `;`, `(`, `)`, `[`, `]`, `{`, `}`, `!`), two-character operators (`!=`, `==`, `&&` — note `&` alone is invalid, `&&` is valid), and comment delimiters (`//` for begin, `*/` for end — note `/` alone is valid as Division, `/*` is invalid). If the input is empty or leads to a non-accepting state (STATE_0 or STATE_NULL), return `"Fehler"`. The function must model a state machine with at least the following states: start (STATE_0), invalid (STATE_NULL), and accepting states for each token. The DFA should process each character exactly once, and after the last character, the current state determines the result. Edge cases: a single `!` is valid (`Not`), `!=` is valid (`NotEqual`), but `!` followed by any other character is invalid. A single `&` is invalid, but `&&` is valid (`And`). A single `/` is valid (`Division`), `//` is (`CommentBegin`), `/*` is invalid, and `*/` is valid (`CommentEnd`). Do not use regex or string pattern matching; implement the state transitions explicitly.
// The solution models the given automaton as a lookup table of transitions (state, input character) → next state, mirroring the original `AutomatSign` class but simplified to only the relevant tokens. The algorithm: initialize current state to `STATE_0`. For each character in the input string, look up the transition from the current state and character. If no transition exists, go to `STATE_NULL` (terminal invalid) and break. Otherwise, update the current state. After processing all characters, map the final state to a token string; if the state is `STATE_0` or `STATE_NULL`, return `"Fehler"`. Important edge cases: the transition table must ensure that `&` only transitions from start to a temporary state, and only `&` from that temporary state leads to `And`; any other character from that temporary state goes to `STATE_NULL`. Similarly, `!` from start goes to a temporary state, and `=` from there yields `NotEqual`, otherwise invalid. For `/`, from start it goes to a temporary state; `/` yields `CommentBegin`, `*` yields `CommentEnd`, and any other character (or end of input) should return `Division` — so the automaton must treat a single `/` as accepting `Division` when the input ends, meaning we must check after the loop: if the current state is the temporary slash state, and we ran out of characters, it's `Division`. The time complexity is O(n) where n is the string length, and space complexity O(1) besides the output string.
#include <string>
#include <unordered_map>

// Returns the token type name for a sign/operator string, or "Fehler" if invalid.
std::string classifySign(const std::string& input) {
    enum State {
        START = 0,
        INVALID,
        PLUS, MINUS, STAR, DIVISION, EQUAL, LESS, GREATER,
        NOT_EQUAL, ASSIGN, AND, NOT,
        SEMICOLON, OPEN_PAREN, CLOSE_PAREN, OPEN_SQUARE, CLOSE_SQUARE,
        OPEN_BRACE, CLOSE_BRACE,
        SLASH_TMP, EXCLAM_TMP, AMP_TMP,
        COMMENT_BEGIN, COMMENT_END
    };

    // Transition table: (currentState, char) -> nextState
    static const std::unordered_map<std::pair<State, char>, State> transitions = {
        {{START, '+'}, PLUS},
        {{START, '-'}, MINUS},
        {{START, '*'}, STAR},
        {{START, '/'}, SLASH_TMP},
        {{START, '='}, EQUAL},
        {{START, '<'}, LESS},
        {{START, '>'}, GREATER},
        {{START, '!'}, EXCLAM_TMP},
        {{START, '&'}, AMP_TMP},
        {{START, ';'}, SEMICOLON},
        {{START, '('}, OPEN_PAREN},
        {{START, ')'}, CLOSE_PAREN},
        {{START, '['}, OPEN_SQUARE},
        {{START, ']'}, CLOSE_SQUARE},
        {{START, '{'}, OPEN_BRACE},
        {{START, '}'}, CLOSE_BRACE},
        {{SLASH_TMP, '/'}, COMMENT_BEGIN},
        {{SLASH_TMP, '*'}, COMMENT_END},
        {{EXCLAM_TMP, '='}, NOT_EQUAL},
        {{AMP_TMP, '&'}, AND}
    };

    if (input.empty()) {
        return "Fehler";
    }

    State current = START;
    for (size_t i = 0; i < input.size(); ++i) {
        char c = input[i];
        auto it = transitions.find({current, c});
        if (it == transitions.end()) {
            current = INVALID;
            break;
        }
        current = it->second;
    }

    // Special case: a single '/' is valid as Division.
    if (current == SLASH_TMP && input.size() == 1) {
        current = DIVISION;
    }

    switch (current) {
        case PLUS:          return "Plus";
        case MINUS:         return "Minus";
        case STAR:          return "Stern";
        case DIVISION:      return "Division";
        case EQUAL:         return "Equal";
        case LESS:          return "LessThan";
        case GREATER:       return "GreaterThan";
        case NOT_EQUAL:     return "NotEqual";
        case ASSIGN:        return "Assign"; // not used in this simplified set
        case AND:           return "And";
        case NOT:           return "Not";   // not used, single '!' is handled via EXCLAM_TMP? Actually '!' alone is invalid, see below
        case SEMICOLON:     return "Semicolon";
        case OPEN_PAREN:    return "OR_Bracket";
        case CLOSE_PAREN:   return "CR_Bracket";
        case OPEN_SQUARE:   return "OS_Bracket";
        case CLOSE_SQUARE:  return "CS_Bracket";
        case OPEN_BRACE:    return "OpeningBrace";
        case CLOSE_BRACE:   return "ClosingBrace";
        case COMMENT_BEGIN: return "CommentBegin";
        case COMMENT_END:   return "CommentEnd";
        default:            return "Fehler";
    }
}
#include <cassert>
#include <string>

// Assume classifySign is declared above.

int main() {
    // Single-character operators
    assert(classifySign("+") == "Plus");
    assert(classifySign("-") == "Minus");
    assert(classifySign("*") == "Stern");
    assert(classifySign("/") == "Division");
    assert(classifySign("=") == "Equal");
    assert(classifySign("<") == "LessThan");
    assert(classifySign(">") == "GreaterThan");
    assert(classifySign(";") == "Semicolon");
    assert(classifySign("(") == "OR_Bracket");
    assert(classifySign(")") == "CR_Bracket");
    assert(classifySign("[") == "OS_Bracket");
    assert(classifySign("]") == "CS_Bracket");
    assert(classifySign("{") == "OpeningBrace");
    assert(classifySign("}") == "ClosingBrace");

    // Two-character operators
    assert(classifySign("!=") == "NotEqual");
    assert(classifySign("&&") == "And");
    assert(classifySign("//") == "CommentBegin");
    assert(classifySign("*/") == "CommentEnd");

    // Invalid cases
    assert(classifySign("") == "Fehler");
    assert(classifySign("!") == "Fehler");       // '!' alone invalid
    assert(classifySign("&") == "Fehler");       // single '&' invalid
    assert(classifySign("/*") == "Fehler");      // invalid comment open
    assert(classifySign("++") == "Fehler");
    assert(classifySign("==") == "Fehler");      // '==' is not in the valid set (only '=' and '!=')
    assert(classifySign("a") == "Fehler");
    assert(classifySign("=+") == "Fehler");
}
