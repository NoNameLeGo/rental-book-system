#include "Member.h"
#include <iostream>
using namespace std;

void Member::showMenu() {
    cout << "=== 会员菜单 ===" << endl;
    cout << "1. 查看书籍" << endl;
    cout << "2. 租借书籍" << endl;
    cout << "3. 归还书籍" << endl;
    cout << "4. 查看我的租借记录" << endl;
    cout << "0. 退出登录" << endl;
    cout << "请选择：";
}

int Member::getRoleType() {
    return 3;
}
