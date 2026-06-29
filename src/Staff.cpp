#include "Staff.h"
#include <iostream>

void Staff::showMenu() {
    std::cout << "\n=== 工作人员菜单 ===" << std::endl;
    std::cout << "1. 书籍管理" << std::endl;
    std::cout << "2. 租借管理" << std::endl;
    std::cout << "3. 查看逾期记录" << std::endl;
    std::cout << "0. 退出登录" << std::endl;
    std::cout << "请选择：";
}

int Staff::getRoleType() {
    return 2;
}
