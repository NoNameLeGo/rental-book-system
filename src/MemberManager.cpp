#include "MemberManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

vector<string> MemberManager::readUserFromFile(const string& acc) {
    ifstream file("datas/user.csv");
    if (!file.is_open()) return {};
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string fa, pwd, role, name, phone;
        getline(ss, fa, ',');
        getline(ss, pwd, ',');
        getline(ss, role, ',');
        getline(ss, name, ',');
        getline(ss, phone, ',');
        if (fa == acc) return {fa, pwd, role, name, phone};
    }
    return {};
}

static bool writeUsersCSV(const string& filename, const vector<vector<string>>& rows) {
    ofstream file(filename);
    if (!file.is_open()) return false;
    file << "账号,密码,角色,姓名,电话" << endl;
    for (const auto& r : rows) {
        for (size_t i = 0; i < r.size(); ++i) {
            file << r[i];
            if (i + 1 < r.size()) file << ",";
        }
        file << endl;
    }
    return true;
}

static vector<vector<string>> readAllUsers() {
    ifstream file("datas/user.csv");
    vector<vector<string>> rows;
    if (!file.is_open()) return rows;
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        vector<string> fields;
        string f;
        while (getline(ss, f, ',')) fields.push_back(f);
        rows.push_back(fields);
    }
    return rows;
}

bool MemberManager::addUserToCSV(const string& acc, const string& pwd, int role, const string& name, const string& phone) {
    if (!readUserFromFile(acc).empty()) return false;
    ofstream file("datas/user.csv", ios::app);
    if (!file.is_open()) return false;
    file << acc << "," << pwd << "," << role << "," << name << "," << phone << endl;
    return true;
}

bool MemberManager::modifyUserInCSV(const string& acc, const string& pwd, const string& name, const string& phone) {
    auto rows = readAllUsers();
    bool found = false;
    for (auto& r : rows) {
        if (r[0] == acc) { r[1] = pwd; r[3] = name; r[4] = phone; found = true; }
    }
    if (!found) return false;
    return writeUsersCSV("datas/user.csv", rows);
}

bool MemberManager::deleteUserFromCSV(const string& acc) {
    auto rows = readAllUsers();
    bool found = false;
    vector<vector<string>> filtered;
    filtered.push_back({}); // header placeholder
    for (const auto& r : rows) {
        if (r[0] == acc) { found = true; }
        else filtered.push_back(r);
    }
    if (!found) return false;
    return writeUsersCSV("datas/user.csv", filtered);
}

void MemberManager::addMember(const Member& member) { members.push_back(member); }

void MemberManager::modifyMember(const string& acc, const Member& member) {
    for (auto& m : members) {
        if (m.getAcc() == acc) { m = member; return; }
    }
    cout << "未找到会员：" << acc << endl;
}

bool MemberManager::deleteMember(const string& acc) {
    for (auto it = members.begin(); it != members.end(); ++it) {
        if (it->getAcc() == acc) { members.erase(it); return true; }
    }
    return false;
}

Member* MemberManager::searchMember(const string& acc) {
    for (auto& m : members) { if (m.getAcc() == acc) return &m; }
    return nullptr;
}

void MemberManager::showAllMembers() {
    if (members.empty()) { cout << "没有会员记录。" << endl; return; }
    cout << "账号\t\t姓名\t\t电话\t\t注册日期" << endl;
    cout << "--------------------------------------------------------" << endl;
    for (const auto& m : members) {
        cout << m.getAcc() << "\t\t" << m.getName() << "\t\t" << m.getPhone() << "\t" << m.getRegDate() << endl;
    }
}

void MemberManager::saveMembersToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) { cout << "无法打开文件：" << filename << endl; return; }
    file << "账号,密码,姓名,电话,注册日期" << endl;
    for (const auto& m : members) {
        file << m.getAcc() << "," << m.getPwd() << "," << m.getName() << ","
             << m.getPhone() << "," << m.getRegDate() << endl;
    }
    file.close();
}

void MemberManager::readMembersFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;
    members.clear();
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string acc, pwd, name, phone, regDate;
        getline(ss, acc, ',');
        getline(ss, pwd, ',');
        getline(ss, name, ',');
        getline(ss, phone, ',');
        getline(ss, regDate, ',');
        members.push_back(Member(acc, pwd, name, phone, regDate));
    }
    file.close();
}
