#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

struct Student {
    string name;
    string gender_full;
    int class_id;
    int scores[9];
    int avg_score;
    int rank;

    void update_avg() {
        int sum = 0;
        for (int i = 0; i < 9; ++i) sum += scores[i];
        avg_score = sum / 9;
    }
};

vector<Student> students;

bool compareStudents(int a_idx, int b_idx) {
    const Student& a = students[a_idx];
    const Student& b = students[b_idx];
    if (a.avg_score != b.avg_score) {
        return a.avg_score > b.avg_score;
    }
    for (int i = 0; i < 9; ++i) {
        if (a.scores[i] != b.scores[i]) {
            return a.scores[i] > b.scores[i];
        }
    }
    return a.name < b.name;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    unordered_map<string, int> name_to_idx;
    vector<int> ranking_list;
    bool started = false;

    string cmd;
    while (cin >> cmd) {
        if (cmd == "ADD") {
            string name, gender;
            int class_id;
            int s[9];
            cin >> name >> gender >> class_id;
            for (int i = 0; i < 9; ++i) cin >> s[i];

            if (started) {
                cout << "[Error]Cannot add student now.\n";
                continue;
            }
            if (name_to_idx.find(name) != name_to_idx.end()) {
                cout << "[Error]Add failed.\n";
                continue;
            }

            int idx = students.size();
            name_to_idx[name] = idx;
            Student st;
            st.name = name;
            st.gender_full = (gender == "M" ? "male" : "female");
            st.class_id = class_id;
            for (int i = 0; i < 9; ++i) st.scores[i] = s[i];
            st.update_avg();
            st.rank = 0;
            students.push_back(st);
        } else if (cmd == "START") {
            started = true;
            ranking_list.reserve(students.size());
            for (int i = 0; i < (int)students.size(); ++i) {
                ranking_list.push_back(i);
            }
            sort(ranking_list.begin(), ranking_list.end(), compareStudents);
            for (int i = 0; i < (int)ranking_list.size(); ++i) {
                students[ranking_list[i]].rank = i + 1;
            }
        } else if (cmd == "UPDATE") {
            string name;
            int code, score;
            cin >> name >> code >> score;
            auto it = name_to_idx.find(name);
            if (it == name_to_idx.end()) {
                cout << "[Error]Update failed.\n";
            } else {
                students[it->second].scores[code] = score;
                students[it->second].update_avg();
            }
        } else if (cmd == "FLUSH") {
            sort(ranking_list.begin(), ranking_list.end(), compareStudents);
            for (int i = 0; i < (int)ranking_list.size(); ++i) {
                students[ranking_list[i]].rank = i + 1;
            }
        } else if (cmd == "PRINTLIST") {
            for (int i = 0; i < (int)ranking_list.size(); ++i) {
                const Student& st = students[ranking_list[i]];
                cout << i + 1 << " " << st.name << " " << st.gender_full << " " << st.class_id << " " << st.avg_score << "\n";
            }
        } else if (cmd == "QUERY") {
            string name;
            cin >> name;
            auto it = name_to_idx.find(name);
            if (it == name_to_idx.end()) {
                cout << "[Error]Query failed.\n";
            } else {
                cout << "STUDENT " << name << " NOW AT RANKING " << students[it->second].rank << "\n";
            }
        } else if (cmd == "END") {
            break;
        }
    }

    return 0;
}