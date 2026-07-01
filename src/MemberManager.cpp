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
        string fileAcc, pwd, role, name, phone;
        getline(ss, fileAcc, ',');
        getline(ss, pwd, ',');
        getline(ss, role, ',');
        getline(ss, name, ',');
        getline(ss, phone, ',');
        if (fileAcc == acc) return {fileAcc, pwd, role, name, phone};
    }
    return {};
}

bool MemberManager::addUserToCSV(const string& acc, const string& pwd, int role, const string& name, const string& phone) {
    if (!readUserFromFile(acc).empty()) return false;
    ofstream file("datas/user.csv", ios::app);
    if (!file.is_open()) return false;
    file << acc << "," << pwd << "," << role << "," << name << "," << phone << endl;
    return true;
}

bool MemberManager::modifyUserInCSV(const string& acc, const string& pwd, const string& name, const string& phone) {
    ifstream file("datas/user.csv");
    if (!file.is_open()) return false;
    vector<string> lines;
    string line;
    getline(file, line);
    lines.push_back(line);
    bool found = false;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string fileAcc, pwd2, role, oldName, oldPhone;
        getline(ss, fileAcc, ',');
        getline(ss, pwd2, ',');
        getline(ss, role, ',');
        getline(ss, oldName, ',');
        getline(ss, oldPhone, ',');
        if (fileAcc == acc) {
            lines.push_back(acc + "," + pwd + "," + role + "," + name + "," + phone);
            found = true;
        } else {
            lines.push_back(line);
        }
    }
    file.close();
    if (!found) return false;
    ofstream outFile("datas/user.csv");
    for (const auto& l : lines) outFile << l << endl;
    return true;
}

bool MemberManager::deleteUserFromCSV(const string& acc) {
    ifstream file("datas/user.csv");
    if (!file.is_open()) return false;
    vector<string> lines;
    string line;
    getline(file, line);
    lines.push_back(line);
    bool found = false;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string fileAcc;
        getline(ss, fileAcc, ',');
        if (fileAcc == acc) found = true;
        else lines.push_back(line);
    }
    file.close();
    if (!found) return false;
    ofstream outFile("datas/user.csv");
    for (const auto& l : lines) outFile << l << endl;
    return true;
}

void MemberManager::addMember(const Member& member) {
    members.push_back(member);
}

void MemberManager::modifyMember(const string& acc, const Member& member) {
    for (auto& m : members) {
        if (m.getAcc() == acc) {
            m = member;
            return;
        }
    }
    cout << "未找到会员：" << acc << endl;
}

bool MemberManager::deleteMember(const string& acc) {
    for (auto it = members.begin(); it != members.end(); ++it) {
        if (it->getAcc() == acc) {
            members.erase(it);
            return true;
        }
    }
    return false;
}

Member* MemberManager::searchMember(const string& acc) {
    for (auto& m : members) {
        if (m.getAcc() == acc) {
            return &m;
        }
    }
    return nullptr;
}

void MemberManager::showAllMembers() {
    if (members.empty()) {
        cout << "没有会员记录。" << endl;
        return;
    }
    cout << "账号\t\t姓名\t\t电话\t\t注册日期" << endl;
    cout << "--------------------------------------------------------" << endl;
    for (const auto& m : members) {
        cout << m.getAcc() << "\t\t" << m.getName() << "\t\t" << m.getPhone() << "\t" << m.getRegDate() << endl;
    }
}

void MemberManager::saveMembersToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "无法打开文件：" << filename << endl;
        return;
    }
    file << "账号,密码,姓名,电话,注册日期" << endl;
    for (const auto& m : members) {
        file << m.getAcc() << "," << m.getPwd() << "," << m.getName() << ","
             << m.getPhone() << "," << m.getRegDate() << endl;
    }
    file.close();
}

void MemberManager::readMembersFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
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
