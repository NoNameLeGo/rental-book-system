#include "Member.h"
#include <iostream>

void Member::showMenu() {
    std::cout << "=== 会员菜单 ===" << std::endl;
    std::cout << "1. 查看书籍" << std::endl;
    std::cout << "2. 租借书籍" << std::endl;
    std::cout << "3. 归还书籍" << std::endl;
    std::cout << "4. 查看我的租借记录" << std::endl;
    std::cout << "0. 退出登录" << std::endl;
    std::cout << "请选择：";
}

int Member::getRoleType() {
    return 3;
}