#include <algorithm>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

vector<uint32_t> BuildZFunc(vector<uint32_t>& pattern) {
    uint32_t n = pattern.size();
    vector<uint32_t> Z(n);
    uint32_t l = 0;
    uint32_t r = 0;

    for (uint32_t i = 1; i < n; i++) {
        if (i <= r) {
            Z[i] = min(r - i + 1, Z[i - l]);
        }
        while (i + Z[i] < n && pattern[Z[i]] == pattern[i + Z[i]]) {
            Z[i]++;
        }
        if (i + Z[i] - 1 > r) {
            l = i;
            r = i + Z[i] - 1;
        }
    }

    return Z;
}

vector<uint32_t> BuildNFunc(vector<uint32_t>& pattern) {
    reverse(pattern.begin(), pattern.end());
    vector<uint32_t> Z = BuildZFunc(pattern);
    vector<uint32_t> N(Z.rbegin(), Z.rend());
    reverse(pattern.begin(), pattern.end());
    return N;
}

void BoyerMoore(vector<uint32_t>& pattern, vector<uint32_t>& text,
    vector<pair<uint32_t, uint32_t>>& counters, map<uint32_t, vector<uint32_t>>& occurrences) {
    vector<uint32_t> N = BuildNFunc(pattern);
    vector<uint32_t> l(pattern.size() + 1);
    vector<uint32_t> L(pattern.size() + 1);
    uint32_t j;

    for (int i = 0; i < pattern.size() - 1; i++) {
        if (N[i] != 0) {
            j = pattern.size() - N[i];
            l[j] = i;
        }

        if (N[i] == i + 1) {
            L[(pattern.size() - 1) - i] = i + 1;
        } else {
            L[(pattern.size() - 1) - i] = L[(pattern.size() - 1) - i + 1];
        }
    }

    int offset = pattern.size() - 1;

    while (offset < text.size()) {
        int i = pattern.size() - 1;
        int j = offset;

        while (i >= 0 && pattern[i] == text[j]) {
            i--;
            j--;
        }

        if (i == -1) {
            int index = offset - pattern.size() + 1;

            cout << counters[index].first << ", " << counters[index].second << "\n";

            if (pattern.size() > 2) {
                offset += pattern.size() - L[1];
            } else {
                offset++;
            }

        } else {
            int goodSuffix = 1;
            int badChar = 1;

            if (i < pattern.size() - 1) {
                if (l[i + 1] > 0) {
                    goodSuffix = pattern.size() - l[i + 1] - 1;
                } else {
                    goodSuffix = pattern.size() - L[i + 1];
                }
            }

            for (uint32_t& rightIndex : occurrences[text[j]]) {
                if (rightIndex < i && rightIndex != 0) {
                    badChar = i - rightIndex;
                    break;
                }
            }

            offset += max(goodSuffix, badChar);
        }
    }
}

int main() {
    cin.tie(0);
    cout.tie(0);
    ios::sync_with_stdio(false);

    vector<uint32_t> pattern;
    vector<uint32_t> text;
    vector<pair<uint32_t, uint32_t>> counters;
    map<uint32_t, vector<uint32_t>> occurrences;

    string temp;
    char s;

    while ((s = getchar()) != EOF) {
        if (s == ' ') {
            if (!temp.empty()) {
                pattern.push_back(static_cast<uint32_t>(stoul(temp)));
                temp.clear();
            }
        } else if (s == '\n') {
            if (!temp.empty()) {
                pattern.push_back(static_cast<uint32_t>(stoul(temp)));
                temp.clear();
            }
            break;
        } else {
            temp.push_back(s);
        }
    }

    if (pattern.size() == 0) {
        return 0;
    }

    uint32_t row = 1;
    uint32_t word = 1;
    uint32_t space = 0;

    while ((s = getchar()) != EOF) {
        if (s == ' ') {
            if (!temp.empty()) {
                text.push_back(static_cast<uint32_t>(stoul(temp)));
                temp.clear();
                counters.push_back({ row, word });
            }
            if (!space) {
                space = 1;
                word++;
            }
        } else if (s == '\n') {
            if (!temp.empty()) {
                text.push_back(static_cast<uint32_t>(stoul(temp)));
                temp.clear();
                counters.push_back({ row, word });
            }
            space = 0;
            word = 1;
            row++;
        } else {
            space = 0;
            temp.push_back(s);
        }
    }

    if (text.size() < pattern.size()) {
        return 0;
    }

    for (long i = pattern.size() - 1; i >= 0; i--) {
        occurrences[pattern[i]].push_back(i);
    }

    BoyerMoore(pattern, text, counters, occurrences);

    return 0;
}