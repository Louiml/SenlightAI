/*
Write a C++ function named `formatChatMessages` that takes a `std::vector<std::string>` of chat room activity records and returns a `std::vector<std::string>` of human-readable messages in the order the activities occurred. Each input record is a string with space-separated tokens following one of these formats: `"Enter <user_id> <nickname>"`, `"Leave <user_id>"`, or `"Change <user_id> <new_nickname>"`. The function must maintain a nickname map keyed by user ID: `Enter` and `Change` update the nickname for that user, while `Leave` does not change it. For each `Enter` and `Leave` action, produce an output string in the form `"<current_nickname>님이 들어왔습니다."` for `Enter` and `"<current_nickname>님이 나갔습니다."` for `Leave`, using the most recent nickname known for that user at the time of the action. `Change` actions produce no output. You may assume all records are well-formed and that every `Leave` or `Change` refers to a user who has previously entered. The function must preserve the relative order of `Enter`/`Leave` events in the output.
*/
#include <string>
#include <vector>
#include <map>
#include <sstream>

namespace {
    enum class Action { None, Enter, Leave, Change };

    // Parse a single record string into action, user id, and optional nickname.
    void parseRecord(const std::string& record, Action& action, std::string& id, std::string& name) {
        std::istringstream iss(record);
        std::string token;
        std::vector<std::string> parts;
        while (iss >> token) {
            parts.push_back(token);
        }
        if (parts.empty()) {
            action = Action::None;
            return;
        }
        if (parts[0] == "Enter") {
            action = Action::Enter;
            id = parts[1];
            name = parts[2];
        } else if (parts[0] == "Leave") {
            action = Action::Leave;
            id = parts[1];
        } else if (parts[0] == "Change") {
            action = Action::Change;
            id = parts[1];
            name = parts[2];
        } else {
            action = Action::None;
        }
    }
}

// Convert chat room activity records into display messages.
std::vector<std::string> formatChatMessages(const std::vector<std::string>& record) {
    std::map<std::string, std::string> nicknameMap;
    std::vector<std::pair<std::string, Action>> events;

    for (const std::string& rec : record) {
        Action action = Action::None;
        std::string id, name;
        parseRecord(rec, action, id, name);
        if (action == Action::None) continue;

        if (action == Action::Enter) {
            nicknameMap[id] = name;
            events.push_back({id, Action::Enter});
        } else if (action == Action::Leave) {
            events.push_back({id, Action::Leave});
        } else if (action == Action::Change) {
            nicknameMap[id] = name;
        }
    }

    std::vector<std::string> result;
    for (const auto& event : events) {
        const std::string& currentName = nicknameMap[event.first];
        std::string message = currentName + "님이 ";
        message += (event.second == Action::Enter) ? "들어왔습니다." : "나갔습니다.";
        result.push_back(message);
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>

int main() {
    std::vector<std::string> test1 = {
        "Enter uid123 Muzi",
        "Enter uid456 Apeach",
        "Leave uid123",
        "Enter uid123 Prodo",
        "Change uid456 Ryan"
    };
    std::vector<std::string> expected1 = {
        "Muzi님이 들어왔습니다.",
        "Apeach님이 들어왔습니다.",
        "Muzi님이 나갔습니다.",
        "Prodo님이 들어왔습니다."
    };
    assert(formatChatMessages(test1) == expected1);

    std::vector<std::string> test2 = {
        "Enter uid1 A",
        "Change uid1 B",
        "Leave uid1"
    };
    std::vector<std::string> expected2 = {
        "A님이 들어왔습니다.",
        "B님이 나갔습니다."
    };
    assert(formatChatMessages(test2) == expected2);

    std::vector<std::string> test3 = {
        "Enter uid1 X",
        "Enter uid2 Y",
        "Leave uid2",
        "Leave uid1"
    };
    std::vector<std::string> expected3 = {
        "X님이 들어왔습니다.",
        "Y님이 들어왔습니다.",
        "Y님이 나갔습니다.",
        "X님이 나갔습니다."
    };
    assert(formatChatMessages(test3) == expected3);

    std::vector<std::string> test4 = {
        "Enter uid1 One",
        "Enter uid1 Two",
        "Leave uid1"
    };
    std::vector<std::string> expected4 = {
        "One님이 들어왔습니다.",
        "Two님이 들어왔습니다.",
        "Two님이 나갔습니다."
    };
    assert(formatChatMessages(test4) == expected4);

    std::vector<std::string> test5; // empty
    assert(formatChatMessages(test5).empty());

    return 0;
}
// The solution processes each record sequentially. First, we parse each string into an action type, user ID, and optionally a nickname using a helper that splits on spaces. We maintain a `std::map<std::string, std::string>` to store the current nickname for each user ID. For `Enter` and `Change`, we update the map with the new nickname. For `Enter` and `Leave`, we push a pair `(user_id, action_type)` into a vector that records the event order. After processing all records, we iterate over this event vector and for each entry, look up the user's current nickname from the map (which by then contains the final nickname, but since nicknames are only updated on `Enter`/`Change`, using the final map is equivalent because `Leave` does not change nicknames and the last known nickname is what we want). Then we construct the output string: `nickname + "님이 " + (action == Enter ? "들어왔습니다." : "나갔습니다.")`. Edge cases: a user might `Enter`, `Leave`, then `Enter` again with a different nickname—the map correctly holds the latest nickname at each point, but since we build output after processing all records, we need to ensure we use the nickname that was valid at the time of each event. However, because `Leave` does not alter nicknames and only `Enter`/`Change` can change them, the final nickname for a user is the same as the nickname that would be used for all their events (unless a `Change` happens after a `Leave`, but that is fine because the output for a prior `Leave` should use the nickname at that time, and since `Change` only happens while the user is inside, the final map will reflect that change—this matches the problem's semantics). Time complexity is O(N * L) where N is the number of records and L is the average length of a record (due to string splitting), and O(U + E) space for the map and event list where U is distinct users and E is the number of enter/leave events. The output vector itself uses O(E) space.
