#ifndef MEMBERMANAGER_H
#define MEMBERMANAGER_H

#include "Member.h"
#include <vector>
#include <string>

class MemberManager {
private:
    std::vector<Member> members;

public:
    MemberManager() {}
    ~MemberManager() {}
    
    void addMember(const Member& member);
    void modifyMember(const std::string& acc, const Member& member);
    bool deleteMember(const std::string& acc);
    Member* searchMember(const std::string& acc);
    void showAllMembers();
    
    void saveMembersToCSV(const std::string& filename);
    void readMembersFromCSV(const std::string& filename);
    
    // 用户登录文件操作
    std::vector<std::string> readUserFromFile(const std::string& acc);
    bool addUserToCSV(const std::string& acc, const std::string& pwd, int role, const std::string& name, const std::string& phone);
    bool modifyUserInCSV(const std::string& acc, const std::string& pwd, const std::string& name, const std::string& phone);
    bool deleteUserFromCSV(const std::string& acc);
};

#endif