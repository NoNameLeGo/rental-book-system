# 租书店管理系统

C++ 面向对象程序设计课程设计项目。基于控制台的租书店管理系统，支持多角色登录、书籍管理、会员管理、租借归还等功能。

## 功能特性

- **多角色权限控制**：管理员、工作人员、会员三种角色，各有不同功能菜单
- **书籍管理**：书籍的添加、修改、删除、查询（管理员/工作人员）
- **会员管理**：会员的添加、修改、删除、查询（管理员）
- **租借归还**：会员租借书籍、归还书籍、自动计算租金和逾期费用
- **逾期查询**：查询当前未归还的租借记录
- **数据持久化**：所有数据通过 CSV 文件保存，重启程序不丢失

## 项目结构

```
rental-book-system/
├── include/              # 头文件
│   ├── UserBase.h        # 用户基类（抽象类）
│   ├── Admin.h           # 管理员类
│   ├── Staff.h           # 工作人员类
│   ├── Member.h          # 会员类
│   ├── Book.h            # 书籍实体类
│   ├── BookCategory.h    # 书籍分类实体类
│   ├── RentalRecord.h    # 租借记录实体类
│   ├── BookManager.h     # 书籍管理类
│   ├── MemberManager.h   # 会员管理类
│   └── RentalManager.h   # 租借管理类
├── src/                  # 源文件
│   ├── UserBase.cpp
│   ├── Admin.cpp
│   ├── Staff.cpp
│   ├── Member.cpp
│   ├── BookManager.cpp
│   ├── MemberManager.cpp
│   └── RentalManager.cpp
├── datas/                # 数据文件
│   ├── user.csv          # 用户账号数据
│   ├── book.csv          # 书籍数据
│   ├── category.csv      # 书籍分类数据
│   ├── member.csv        # 会员数据
│   └── rental.csv        # 租借记录数据
├── docs/                 # 文档
│   └── 课程设计报告.md
└── main.cpp              # 主程序入口
```

## 编译与运行

### 环境要求

- C++ 编译器（GCC / MinGW / MSVC）
- Windows 系统（使用了 Windows API 设置控制台编码）

### 编译命令

```bash
g++ -o main.exe main.cpp src/*.cpp -Iinclude
```

### 运行

```bash
./main.exe
```

### 默认账号

| 角色 | 账号 | 密码 |
|------|------|------|
| 管理员 | admin | 123456 |
| 工作人员 | staff1 | 123456 |
| 会员 | member1 | 123456 |

## 技术要点

- **抽象类与多态**：UserBase 作为抽象父类，定义纯虚函数 showMenu() 和 getRoleType()，三个角色类继承并重写，通过父类指针实现运行时多态
- **分文件编程**：头文件与源文件分离，类声明放在 include 目录，类实现放在 src 目录
- **CSV 文件读写**：使用 ifstream/ofstream 进行文件读写，getline + stringstream 进行字符串分割
- **向量容器**：使用 vector 动态管理书籍、会员和租借记录数据
