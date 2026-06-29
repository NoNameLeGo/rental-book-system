#ifndef USERBASE_H
#define USERBASE_H

#include <string>

class UserBase {
protected:
    std::string acc;  // 账号
    std::string pwd;  // 密码
    std::string name; // 姓名

public:
    UserBase() {}
    UserBase(std::string a, std::string p, std::string n) : acc(a), pwd(p), name(n) {}
    virtual ~UserBase() {}
    
    virtual void showMenu() = 0;      // 纯虚函数：显示菜单
    virtual int getRoleType() = 0;    // 纯虚函数：获取角色类型
    
    bool checkLogin(std::string a, std::string p); // 检查登录
    
    // 获取属性
    std::string getAcc() const { return acc; }
    std::string getPwd() const { return pwd; }
    std::string getName() const { return name; }
};

#endif