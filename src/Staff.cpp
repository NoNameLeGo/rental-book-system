#include "Staff.h"
#include <iostream>
using namespace std;

void Staff::showMenu() {
    cout << "\n=== 工作人员菜单 ===" << endl;
    cout << "1. 书籍管理" << endl;
    cout << "2. 租借管理" << endl;
    cout << "3. 查看逾期记录" << endl;
    cout << "0. 退出登录" << endl;
    cout << "请选择：";
}

int Staff::getRoleType() {
    return 2;
}
