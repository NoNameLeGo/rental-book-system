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
    void deleteMember(const std::string& acc);
    Member* searchMember(const std::string& acc);
    void showAllMembers();
    
    void saveMembersToCSV(const std::string& filename);
    void readMembersFromCSV(const std::string& filename);
};

#endif