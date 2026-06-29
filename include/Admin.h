#ifndef ADMIN_H
#define ADMIN_H

#include "UserBase.h"

class Admin : public UserBase {
public:
    Admin() {}
    Admin(std::string a, std::string p, std::string n) : UserBase(a, p, n) {}
    ~Admin() {}
    
    void showMenu() override;
    int getRoleType() override;
};

#endif