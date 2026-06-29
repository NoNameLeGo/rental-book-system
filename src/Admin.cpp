#include "Admin.h"
#include <iostream>

void Admin::showMenu() {
    std::cout << "=== 管理员菜单 ===" << std::endl;
    std::cout << "1. 书籍管理" << std::endl;
    std::cout << "2. 会员管理" << std::endl;
    std::cout << "3. 租借管理" << std::endl;
    std::cout << "4. 查看逾期记录" << std::endl;
    std::cout << "5. 保存数据" << std::endl;
    std::cout << "0. 退出登录" << std::endl;
    std::cout << "请选择：";
}

int Admin::getRoleType() {
    return 1;
}