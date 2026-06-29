#include "Admin.h"
#include <iostream>
using namespace std;

void Admin::showMenu() {
    cout << "\n=== 管理员菜单 ===" << endl;
    cout << "1. 书籍管理" << endl;
    cout << "2. 会员管理" << endl;
    cout << "3. 租借管理" << endl;
    cout << "4. 查看逾期记录" << endl;
    cout << "5. 分类管理" << endl;
    cout << "6. 保存数据" << endl;
    cout << "0. 退出登录" << endl;
    cout << "请选择：";
}

int Admin::getRoleType() {
    return 1;
}
