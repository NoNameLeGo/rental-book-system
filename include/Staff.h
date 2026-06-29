#ifndef STAFF_H
#define STAFF_H

#include "UserBase.h"

class Staff : public UserBase {
public:
    Staff() {}
    Staff(std::string a, std::string p, std::string n) : UserBase(a, p, n) {}
    ~Staff() {}
    
    void showMenu() override;
    int getRoleType() override;
};

#endif