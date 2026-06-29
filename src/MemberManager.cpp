#include "MemberManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

void MemberManager::addMember(const Member& member) {
    members.push_back(member);
}

void MemberManager::modifyMember(const std::string& acc, const Member& member) {
    for (auto& m : members) {
        if (m.getAcc() == acc) {
            m = member;
            return;
        }
    }
    std::cout << "未找到会员：" << acc << std::endl;
}

void MemberManager::deleteMember(const std::string& acc) {
    for (auto it = members.begin(); it != members.end(); ++it) {
        if (it->getAcc() == acc) {
            members.erase(it);
            return;
        }
    }
    std::cout << "未找到会员：" << acc << std::endl;
}

Member* MemberManager::searchMember(const std::string& acc) {
    for (auto& m : members) {
        if (m.getAcc() == acc) {
            return &m;
        }
    }
    return nullptr;
}

void MemberManager::showAllMembers() {
    if (members.empty()) {
        std::cout << "没有会员记录。" << std::endl;
        return;
    }
    std::cout << "账号\t\t姓名\t\t电话\t\t注册日期" << std::endl;
    std::cout << "--------------------------------------------------------" << std::endl;
    for (const auto& m : members) {
        std::cout << m.getAcc() << "\t\t" << m.getName() << "\t\t" << m.getPhone() << "\t" << m.getRegDate() << std::endl;
    }
}

void MemberManager::saveMembersToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "无法打开文件：" << filename << std::endl;
        return;
    }
    file << "账号,密码,姓名,电话,注册日期" << std::endl;
    for (const auto& m : members) {
        file << m.getAcc() << "," << m.getPwd() << "," << m.getName() << ","
             << m.getPhone() << "," << m.getRegDate() << std::endl;
    }
    file.close();
}

void MemberManager::readMembersFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    members.clear();
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string acc, pwd, name, phone, regDate;
        std::getline(ss, acc, ',');
        std::getline(ss, pwd, ',');
        std::getline(ss, name, ',');
        std::getline(ss, phone, ',');
        std::getline(ss, regDate, ',');
        members.push_back(Member(acc, pwd, name, phone, regDate));
    }
    file.close();
}
