// Write a C++ function `countNewChatMessages` that takes a vector of strings representing a chat log, where each string is either `"ENTER"` (indicating a new chat session starts) or a username (a non-empty string of lowercase letters), and returns the total number of messages that are the first message from a given username in the *entire* log after the most recent `"ENTER"`. In other words, when a new session begins, reset the "seen" status for all users; a message counts if its username has not appeared since the last `"ENTER"`. The function should handle any number of sessions and usernames, and must be case-sensitive. For example, for input `["ENTER", "alice", "bob", "alice", "ENTER", "alice", "bob"]`, the first session counts `alice` and `bob` (2 messages), and the second session counts only `alice` (1 message), for a total of 3.

#include <cassert>
#include <vector>
#include <string>

// declaration of the function from the solution
int countNewChatMessages(const std::vector<std::string>& log);

int main() {
    // Simple case: one session, unique names
    assert(countNewChatMessages({"ENTER", "alice", "bob"}) == 2);
    
    // Repeated name in the same session: only first counts
    assert(countNewChatMessages({"ENTER", "alice", "alice", "bob"}) == 2);
    
    // Multiple sessions reset the count
    assert(countNewChatMessages({"ENTER", "alice", "bob", "ENTER", "alice"}) == 3);
    assert(countNewChatMessages({"ENTER", "alice", "bob", "alice", "ENTER", "alice", "bob"}) == 3);
    
    // No initial ENTER: treats everything before first ENTER as session 0
    assert(countNewChatMessages({"alice", "bob", "alice", "ENTER", "carol"}) == 3);
    
    // Consecutive ENTERs should just reset, still count the first user after
    assert(countNewChatMessages({"ENTER", "ENTER", "alice"}) == 1);
    
    // Empty log
    assert(countNewChatMessages({}) == 0);
    
    // Only ENTERs
    assert(countNewChatMessages({"ENTER", "ENTER", "ENTER"}) == 0);
    
    // Case sensitivity
    assert(countNewChatMessages({"ENTER", "Alice", "alice"}) == 2);
    
    // Same user in different sessions counted each time
    assert(countNewChatMessages({"ENTER", "alice", "ENTER", "alice", "ENTER", "alice"}) == 3);
    
    return 0;
}

#include <string>
#include <vector>
#include <unordered_map>

// Count the number of unique-per-session chat messages.
// Each "ENTER" starts a new session; a message counts if that username
// has not appeared since the most recent "ENTER".
int countNewChatMessages(const std::vector<std::string>& log) {
    int session = 0;
    int answer = 0;
    std::unordered_map<std::string, int> lastSeenInSession;
    
    for (const std::string& entry : log) {
        if (entry == "ENTER") {
            ++session;
        } else {
            // If username never seen, or not seen in current session,
            // it counts as new.
            auto it = lastSeenInSession.find(entry);
            if (it == lastSeenInSession.end() || it->second < session) {
                ++answer;
                lastSeenInSession[entry] = session;
            }
        }
    }
    return answer;
}

// The core idea is to track the most recent session number when a username was last seen. We maintain an integer `session` counter initialized to 0, incrementing it by 1 whenever we encounter `"ENTER"`. We also maintain a `map` (or unordered_map) from username to the session number in which that username was last seen (or 0 if never seen). When a message (non-`"ENTER"`) arrives, we check if the stored session for that username is less than the current `session` number. If it is strictly less, that means the username has not been seen since the current session began, so we count it (increment answer) and update the stored session to the current session. If the stored session equals the current session, the username was already seen in this session, so we skip. This handles repeated usernames within the same session correctly. Edge cases: no `"ENTER"` at the start (implicitly session 0 begins, so all usernames before the first `"ENTER"` are in session 0 and count), multiple `"ENTER"` in a row (each resets the session, and the first username after an `"ENTER"` will always count because its stored session is less than the new session). Time complexity is O(M) where M is the number of strings in the input (each operation on a map is O(log U) for `std::map`, or O(1) average for `std::unordered_map`), and space complexity is O(U) where U is the number of distinct usernames that appear across all sessions.
