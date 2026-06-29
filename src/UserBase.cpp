#include "UserBase.h"
using namespace std;

bool UserBase::checkLogin(string a, string p) {
    return (acc == a && pwd == p);
}
