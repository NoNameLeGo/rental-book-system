#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#endif

#include "UserBase.h"
#include "Admin.h"
#include "Staff.h"
#include "Member.h"
#include "BookManager.h"
#include "MemberManager.h"
#include "RentalManager.h"

using namespace std;

// 创建数据目录
void createDataDir() {
    #ifdef _WIN32
    _mkdir("datas");
    #else
    mkdir("datas", 0777);
    #endif
}

// 初始化用户文件（如果不存在）
void initUserFile() {
    ifstream file("datas/user.csv");
    if (!file.is_open()) {
        // 创建默认用户
        ofstream outFile("datas/user.csv");
        outFile << "账号,密码,角色,姓名,电话" << endl;
        outFile << "admin,123456,1,管理员,13800138000" << endl;
        outFile << "staff1,123456,2,工作人员,13800138001" << endl;
        outFile << "member1,123456,3,会员,13800138002" << endl;
        outFile.close();
        cout << "已创建默认用户文件。" << endl;
    }
    file.close();
}

// 从用户文件读取用户信息
vector<string> readUserFromFile(const string& acc) {
    ifstream file("datas/user.csv");
    if (!file.is_open()) {
        return {};
    }
    string line;
    getline(file, line); // 跳过标题行
    while (getline(file, line)) {
        stringstream ss(line);
        string fileAcc, pwd, role, name, phone;
        getline(ss, fileAcc, ',');
        getline(ss, pwd, ',');
        getline(ss, role, ',');
        getline(ss, name, ',');
        getline(ss, phone, ',');
        if (fileAcc == acc) {
            return {fileAcc, pwd, role, name, phone};
        }
    }
    file.close();
    return {};
}

int main() {
    // 设置控制台编码
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    #endif

    // 创建数据目录
    createDataDir();
    
    // 初始化用户文件
    initUserFile();
    
    // 创建管理对象
    BookManager bookManager;
    MemberManager memberManager;
    RentalManager rentalManager;
    
    // 读取数据
    bookManager.readBooksFromCSV("datas/book.csv");
    bookManager.readCategoriesFromCSV("datas/category.csv");
    memberManager.readMembersFromCSV("datas/member.csv");
    rentalManager.readRecordsFromCSV("datas/rental.csv");
    
    // 登录循环
    while (true) {
        cout << "=== 租书店管理系统 ===" << endl;
        cout << "请选择角色：" << endl;
        cout << "1. 管理员" << endl;
        cout << "2. 工作人员" << endl;
        cout << "3. 会员" << endl;
        cout << "0. 退出系统" << endl;
        cout << "请选择：";
        
        int roleChoice;
        cin >> roleChoice;
        if (roleChoice == 0) {
            cout << "感谢使用，再见！" << endl;
            break;
        }
        if (roleChoice < 1 || roleChoice > 3) {
            cout << "无效选择，请重新输入。" << endl;
            continue;
        }
        
        string acc, pwd;
        cout << "请输入账号：";
        cin >> acc;
        cout << "请输入密码：";
        cin >> pwd;
        
        // 从文件读取用户信息
        vector<string> userInfo = readUserFromFile(acc);
        if (userInfo.empty()) {
            cout << "账号不存在。" << endl;
            continue;
        }
        
        // 验证密码和角色
        if (userInfo[1] != pwd) {
            cout << "密码错误。" << endl;
            continue;
        }
        
        int fileRole = stoi(userInfo[2]);
        if (fileRole != roleChoice) {
            cout << "角色不匹配。" << endl;
            continue;
        }
        
        cout << "登录成功！欢迎，" << userInfo[3] << endl;
        
        // 根据角色创建对象
        UserBase* user = nullptr;
        if (roleChoice == 1) {
            user = new Admin(acc, pwd, userInfo[3]);
        } else if (roleChoice == 2) {
            user = new Staff(acc, pwd, userInfo[3]);
        } else if (roleChoice == 3) {
            user = new Member(acc, pwd, userInfo[3], userInfo[4], "2026-01-01"); // 注册日期简化处理
        }
        
        // 进入角色菜单
        bool logout = false;
        while (!logout) {
            user->showMenu();
            int choice;
            cin >> choice;
            
            if (user->getRoleType() == 1) { // 管理员
                switch (choice) {
                    case 1: {
                        // 书籍管理
                        cout << "书籍管理功能" << endl;
                        cout << "1. 添加书籍 2. 修改书籍 3. 删除书籍 4. 查看所有书籍 0. 返回" << endl;
                        int bookChoice;
                        cin >> bookChoice;
                        if (bookChoice == 1) {
                            // 添加书籍
                            string id, title, author, publisher, categoryId;
                            double price;
                            int stock;
                            cout << "请输入书号：";
                            cin >> id;
                            cout << "请输入书名：";
                            cin >> title;
                            cout << "请输入作者：";
                            cin >> author;
                            cout << "请输入出版社：";
                            cin >> publisher;
                            cout << "请输入价格：";
                            cin >> price;
                            cout << "请输入库存：";
                            cin >> stock;
                            cout << "请输入分类ID：";
                            cin >> categoryId;
                            bookManager.addBook(Book(id, title, author, publisher, price, stock, categoryId));
                            cout << "添加成功。" << endl;
                        } else if (bookChoice == 2) {
                            // 修改书籍
                            string id;
                            cout << "请输入要修改的书号：";
                            cin >> id;
                            Book* book = bookManager.searchBook(id);
                            if (book) {
                                string title, author, publisher, categoryId;
                                double price;
                                int stock;
                                cout << "请输入新书名：";
                                cin >> title;
                                cout << "请输入新作者：";
                                cin >> author;
                                cout << "请输入新出版社：";
                                cin >> publisher;
                                cout << "请输入新价格：";
                                cin >> price;
                                cout << "请输入新库存：";
                                cin >> stock;
                                cout << "请输入新分类ID：";
                                cin >> categoryId;
                                bookManager.modifyBook(id, Book(id, title, author, publisher, price, stock, categoryId));
                                cout << "修改成功。" << endl;
                            } else {
                                cout << "未找到书籍。" << endl;
                            }
                        } else if (bookChoice == 3) {
                            // 删除书籍
                            string id;
                            cout << "请输入要删除的书号：";
                            cin >> id;
                            bookManager.deleteBook(id);
                            cout << "删除成功。" << endl;
                        } else if (bookChoice == 4) {
                            // 查看所有书籍
                            bookManager.showAllBooks();
                        }
                        break;
                    }
                    case 2: {
                        // 会员管理
                        cout << "会员管理功能" << endl;
                        cout << "1. 添加会员 2. 修改会员 3. 删除会员 4. 查看所有会员 0. 返回" << endl;
                        int memberChoice;
                        cin >> memberChoice;
                        if (memberChoice == 1) {
                            // 添加会员
                            string acc, pwd, name, phone, regDate;
                            cout << "请输入账号：";
                            cin >> acc;
                            cout << "请输入密码：";
                            cin >> pwd;
                            cout << "请输入姓名：";
                            cin >> name;
                            cout << "请输入电话：";
                            cin >> phone;
                            cout << "请输入注册日期（YYYY-MM-DD）：";
                            cin >> regDate;
                            memberManager.addMember(Member(acc, pwd, name, phone, regDate));
                            cout << "添加成功。" << endl;
                        } else if (memberChoice == 2) {
                            // 修改会员
                            string acc;
                            cout << "请输入要修改的账号：";
                            cin >> acc;
                            Member* member = memberManager.searchMember(acc);
                            if (member) {
                                string pwd, name, phone, regDate;
                                cout << "请输入新密码：";
                                cin >> pwd;
                                cout << "请输入新姓名：";
                                cin >> name;
                                cout << "请输入新电话：";
                                cin >> phone;
                                cout << "请输入新注册日期：";
                                cin >> regDate;
                                memberManager.modifyMember(acc, Member(acc, pwd, name, phone, regDate));
                                cout << "修改成功。" << endl;
                            } else {
                                cout << "未找到会员。" << endl;
                            }
                        } else if (memberChoice == 3) {
                            // 删除会员
                            string acc;
                            cout << "请输入要删除的账号：";
                            cin >> acc;
                            memberManager.deleteMember(acc);
                            cout << "删除成功。" << endl;
                        } else if (memberChoice == 4) {
                            // 查看所有会员
                            memberManager.showAllMembers();
                        }
                        break;
                    }
                    case 3: {
                        // 租借管理
                        cout << "租借管理功能" << endl;
                        cout << "1. 租借书籍 2. 归还书籍 3. 查看所有记录 0. 返回" << endl;
                        int rentalChoice;
                        cin >> rentalChoice;
                        if (rentalChoice == 1) {
                            // 租借书籍
                            string bookId, memberId, rentalDate;
                            cout << "请输入书号：";
                            cin >> bookId;
                            cout << "请输入会员账号：";
                            cin >> memberId;
                            cout << "请输入租借日期（YYYY-MM-DD）：";
                            cin >> rentalDate;
                            // 检查书籍是否存在且库存足够
                            Book* book = bookManager.searchBook(bookId);
                            if (book && book->getStock() > 0) {
                                // 生成记录ID
                                string recordId = "R" + to_string(time(0));
                                RentalRecord record(recordId, bookId, memberId, rentalDate, "", 0.0, 0.0);
                                rentalManager.addRecord(record);
                                // 减少库存
                                book->setStock(book->getStock() - 1);
                                cout << "租借成功。" << endl;
                            } else {
                                cout << "书籍不存在或库存不足。" << endl;
                            }
                        } else if (rentalChoice == 2) {
                            // 归还书籍
                            string recordId, returnDate;
                            cout << "请输入记录ID：";
                            cin >> recordId;
                            cout << "请输入归还日期（YYYY-MM-DD）：";
                            cin >> returnDate;
                            rentalManager.returnBook(recordId, returnDate);
                            cout << "归还成功。" << endl;
                        } else if (rentalChoice == 3) {
                            // 查看所有记录
                            rentalManager.showAllRecords();
                        }
                        break;
                    }
                    case 4: {
                        // 查看逾期记录
                        rentalManager.showOverdueRecords();
                        break;
                    }
                    case 5: {
                        // 保存数据
                        bookManager.saveBooksToCSV("datas/book.csv");
                        bookManager.saveCategoriesToCSV("datas/category.csv");
                        memberManager.saveMembersToCSV("datas/member.csv");
                        rentalManager.saveRecordsToCSV("datas/rental.csv");
                        cout << "数据已保存。" << endl;
                        break;
                    }
                    case 0: {
                        logout = true;
                        cout << "已退出登录。" << endl;
                        break;
                    }
                    default: {
                        cout << "无效选择，请重新输入。" << endl;
                        break;
                    }
                }
            } else if (user->getRoleType() == 2) { // 工作人员
                switch (choice) {
                    case 1: {
                        // 书籍管理（与管理员类似，但权限可能不同）
                        cout << "书籍管理功能" << endl;
                        cout << "1. 查看所有书籍 2. 搜索书籍 0. 返回" << endl;
                        int bookChoice;
                        cin >> bookChoice;
                        if (bookChoice == 1) {
                            bookManager.showAllBooks();
                        } else if (bookChoice == 2) {
                            string id;
                            cout << "请输入书号：";
                            cin >> id;
                            Book* book = bookManager.searchBook(id);
                            if (book) {
                                cout << "书号：" << book->getId() << endl;
                                cout << "书名：" << book->getTitle() << endl;
                                cout << "作者：" << book->getAuthor() << endl;
                                cout << "出版社：" << book->getPublisher() << endl;
                                cout << "价格：" << book->getPrice() << endl;
                                cout << "库存：" << book->getStock() << endl;
                                cout << "分类：" << book->getCategoryId() << endl;
                            } else {
                                cout << "未找到书籍。" << endl;
                            }
                        }
                        break;
                    }
                    case 2: {
                        // 租借管理
                        cout << "租借管理功能" << endl;
                        cout << "1. 租借书籍 2. 归还书籍 3. 查看所有记录 0. 返回" << endl;
                        int rentalChoice;
                        cin >> rentalChoice;
                        if (rentalChoice == 1) {
                            // 租借书籍（同管理员）
                            string bookId, memberId, rentalDate;
                            cout << "请输入书号：";
                            cin >> bookId;
                            cout << "请输入会员账号：";
                            cin >> memberId;
                            cout << "请输入租借日期（YYYY-MM-DD）：";
                            cin >> rentalDate;
                            Book* book = bookManager.searchBook(bookId);
                            if (book && book->getStock() > 0) {
                                string recordId = "R" + to_string(time(0));
                                RentalRecord record(recordId, bookId, memberId, rentalDate, "", 0.0, 0.0);
                                rentalManager.addRecord(record);
                                book->setStock(book->getStock() - 1);
                                cout << "租借成功。" << endl;
                            } else {
                                cout << "书籍不存在或库存不足。" << endl;
                            }
                        } else if (rentalChoice == 2) {
                            // 归还书籍（同管理员）
                            string recordId, returnDate;
                            cout << "请输入记录ID：";
                            cin >> recordId;
                            cout << "请输入归还日期（YYYY-MM-DD）：";
                            cin >> returnDate;
                            rentalManager.returnBook(recordId, returnDate);
                            cout << "归还成功。" << endl;
                        } else if (rentalChoice == 3) {
                            rentalManager.showAllRecords();
                        }
                        break;
                    }
                    case 3: {
                        // 归还管理（单独列出）
                        cout << "归还管理功能" << endl;
                        string recordId, returnDate;
                        cout << "请输入记录ID：";
                        cin >> recordId;
                        cout << "请输入归还日期（YYYY-MM-DD）：";
                        cin >> returnDate;
                        rentalManager.returnBook(recordId, returnDate);
                        cout << "归还成功。" << endl;
                        break;
                    }
                    case 4: {
                        // 查看逾期记录
                        rentalManager.showOverdueRecords();
                        break;
                    }
                    case 0: {
                        logout = true;
                        cout << "已退出登录。" << endl;
                        break;
                    }
                    default: {
                        cout << "无效选择，请重新输入。" << endl;
                        break;
                    }
                }
            } else if (user->getRoleType() == 3) { // 会员
                switch (choice) {
                    case 1: {
                        // 查看书籍
                        bookManager.showAllBooks();
                        break;
                    }
                    case 2: {
                        // 租借书籍
                        string bookId, rentalDate;
                        cout << "请输入书号：";
                        cin >> bookId;
                        cout << "请输入租借日期（YYYY-MM-DD）：";
                        cin >> rentalDate;
                        Book* book = bookManager.searchBook(bookId);
                        if (book && book->getStock() > 0) {
                            string recordId = "R" + to_string(time(0));
                            RentalRecord record(recordId, bookId, user->getAcc(), rentalDate, "", 0.0, 0.0);
                            rentalManager.addRecord(record);
                            book->setStock(book->getStock() - 1);
                            cout << "租借成功。" << endl;
                        } else {
                            cout << "书籍不存在或库存不足。" << endl;
                        }
                        break;
                    }
                    case 3: {
                        // 归还书籍
                        string recordId, returnDate;
                        cout << "请输入记录ID：";
                        cin >> recordId;
                        cout << "请输入归还日期（YYYY-MM-DD）：";
                        cin >> returnDate;
                        rentalManager.returnBook(recordId, returnDate);
                        cout << "归还成功。" << endl;
                        break;
                    }
                    case 4: {
                        // 查看我的租借记录
                        rentalManager.showMemberRecords(user->getAcc());
                        break;
                    }
                    case 0: {
                        logout = true;
                        cout << "已退出登录。" << endl;
                        break;
                    }
                    default: {
                        cout << "无效选择，请重新输入。" << endl;
                        break;
                    }
                }
            }
        }
        
        delete user;
    }
    
    return 0;
}