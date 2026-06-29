#ifndef MEMBER_H
#define MEMBER_H

#include "UserBase.h"

class Member : public UserBase {
private:
    std::string phone;  // 电话
    std::string regDate; // 注册日期

public:
    Member() {}
    Member(std::string a, std::string p, std::string n, std::string ph, std::string rd) 
        : UserBase(a, p, n), phone(ph), regDate(rd) {}
    ~Member() {}
    
    void showMenu() override;
    int getRoleType() override;
    
    std::string getPhone() const { return phone; }
    std::string getRegDate() const { return regDate; }
};

#endif