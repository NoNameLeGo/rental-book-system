#include "UserBase.h"

bool UserBase::checkLogin(std::string a, std::string p) {
    return (acc == a && pwd == p);
}