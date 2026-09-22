// Write a C++ function `vector<vector<string>> mergeAccounts(vector<vector<string>>& accounts)` that takes a list of accounts, where each account is represented as a vector of strings with the first element being the account holder's name and the remaining elements being email addresses. Two accounts belong to the same person if they share at least one email address. The function should merge all accounts belonging to the same person, returning a list where each merged account contains the person's name (from any of the merged accounts) followed by all unique email addresses sorted lexicographically. The overall result can be in any order, but each merged account's emails must be sorted and deduplicated (the input may contain duplicates or empty email lists). The input list size and total number of emails are arbitrary, but the function must handle up to \(10^5\) distinct emails efficiently.
The problem reduces to grouping accounts into connected components where each email acts as an edge between accounts. Use a disjoint set (union-find) data structure with union by size and path compression. First, map each email to the first account index that contains it; if an email appears again in another account, union those two accounts. After processing all emails, for each distinct email, find the root of its associated account and push the email into a list for that root. Then, for each account index that has collected emails, sort those emails, prepend the account name (from the original `accounts[i][0]`), and add to the result. Edge cases: accounts with no emails (they remain isolated and should be included with just the name), duplicate emails within the same account (the map will just union it with itself, harmless), and accounts already merged through multiple shared emails. Time complexity is \(O(N \cdot M \cdot \alpha(N) + E \log E)\) where \(N\) is number of accounts, \(E\) is total unique emails, and \(\alpha\) is the inverse Ackermann function (near constant). Space complexity is \(O(N + E)\).
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

class DisjointSet {
private:
    std::vector<int> parent, size;
public:
    DisjointSet(int n) {
        parent.resize(n);
        size.assign(n, 1);
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x);
        int ry = find(y);
        if (rx == ry) return;
        if (size[ry] > size[rx]) std::swap(rx, ry);
        parent[ry] = rx;
        size[rx] += size[ry];
    }
};

std::vector<std::vector<std::string>> mergeAccounts(std::vector<std::vector<std::string>>& accounts) {
    int n = static_cast<int>(accounts.size());
    DisjointSet ds(n);
    std::unordered_map<std::string, int> emailToFirstAccount;

    for (int i = 0; i < n; ++i) {
        for (size_t j = 1; j < accounts[i].size(); ++j) {
            const std::string& email = accounts[i][j];
            auto it = emailToFirstAccount.find(email);
            if (it == emailToFirstAccount.end()) {
                emailToFirstAccount[email] = i;
            } else {
                ds.unite(i, it->second);
            }
        }
    }

    std::vector<std::vector<std::string>> mergedEmails(n);
    for (const auto& pair : emailToFirstAccount) {
        int root = ds.find(pair.second);
        mergedEmails[root].push_back(pair.first);
    }

    std::vector<std::vector<std::string>> result;
    for (int i = 0; i < n; ++i) {
        if (mergedEmails[i].empty() && accounts[i].size() == 1) {
            result.push_back({accounts[i][0]});
        } else if (!mergedEmails[i].empty()) {
            std::sort(mergedEmails[i].begin(), mergedEmails[i].end());
            mergedEmails[i].insert(mergedEmails[i].begin(), accounts[i][0]);
            result.push_back(mergedEmails[i]);
        }
    }
    return result;
}
#include <cassert>
#include <vector>
#include <string>
#include <algorithm>

int main() {
    // Test 1: Basic merging through a shared email
    std::vector<std::vector<std::string>> accounts1 = {
        {"John", "john@mail.com", "john2@mail.com"},
        {"John", "john@mail.com", "john_new@mail.com"},
        {"Mary", "mary@mail.com"}
    };
    auto res1 = mergeAccounts(accounts1);
    // Expected: One merged John account with 3 sorted emails, and Mary's account
    std::vector<std::vector<std::string>> expected1 = {
        {"John", "john2@mail.com", "john@mail.com", "john_new@mail.com"},
        {"Mary", "mary@mail.com"}
    };
    std::sort(res1.begin(), res1.end());
    std::sort(expected1.begin(), expected1.end());
    assert(res1 == expected1);

    // Test 2: Chained merging (Account 1 shares with 2, 2 shares with 3)
    std::vector<std::vector<std::string>> accounts2 = {
        {"A", "a@mail.com"},
        {"B", "b@mail.com", "a@mail.com"},
        {"C", "b@mail.com", "c@mail.com"}
    };
    auto res2 = mergeAccounts(accounts2);
    // All three accounts merge into one with emails a,b,c
    std::vector<std::vector<std::string>> expected2 = {
        {"A", "a@mail.com", "b@mail.com", "c@mail.com"}
    };
    assert(res2.size() == 1);
    assert(res2[0].size() == 4);
    assert(res2[0][0] == "A");
    assert(res2[0][1] == "a@mail.com");
    assert(res2[0][2] == "b@mail.com");
    assert(res2[0][3] == "c@mail.com");

    // Test 3: Duplicate emails within the same account
    std::vector<std::vector<std::string>> accounts3 = {
        {"D", "d@mail.com", "d@mail.com", "e@mail.com"}
    };
    auto res3 = mergeAccounts(accounts3);
    std::vector<std::vector<std::string>> expected3 = {
        {"D", "d@mail.com", "e@mail.com"}
    };
    assert(res3 == expected3);

    // Test 4: Account with no emails
    std::vector<std::vector<std::string>> accounts4 = {
        {"E", "e@mail.com"},
        {"F"}
    };
    auto res4 = mergeAccounts(accounts4);
    std::vector<std::vector<std::string>> expected4 = {
        {"E", "e@mail.com"},
        {"F"}
    };
    std::sort(res4.begin(), res4.end());
    std::sort(expected4.begin(), expected4.end());
    assert(res4 == expected4);

    // Test 5: Isolated accounts with no shared emails
    std::vector<std::vector<std::string>> accounts5 = {
        {"G", "g1@mail.com"},
        {"H", "h1@mail.com"}
    };
    auto res5 = mergeAccounts(accounts5);
    assert(res5.size() == 2);
    std::sort(res5.begin(), res5.end());
    assert(res5[0] == std::vector<std::string>({"G", "g1@mail.com"}));
    assert(res5[1] == std::vector<std::string>({"H", "h1@mail.com"}));

    return 0;
}
