#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <limits>

#ifdef _WIN32
#define NOMINMAX
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
#include "DateUtil.h"

using namespace std;

void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string readLine(const string& prompt) {
    cout << prompt;
    string input;
    getline(cin >> ws, input);
    return input;
}

int readInt(const string& prompt) {
    while (true) {
        cout << prompt;
        int value;
        if (cin >> value) {
            clearInputBuffer();
            return value;
        }
        if (cin.eof()) return 0;
        cout << "输入无效，请输入一个整数。" << endl;
        clearInputBuffer();
    }
}

double readDouble(const string& prompt) {
    while (true) {
        cout << prompt;
        double value;
        if (cin >> value) {
            clearInputBuffer();
            return value;
        }
        if (cin.eof()) return 0.0;
        cout << "输入无效，请输入一个数字。" << endl;
        clearInputBuffer();
    }
}

void createDataDir() {
    #ifdef _WIN32
    _mkdir("datas");
    #else
    mkdir("datas", 0777);
    #endif
}

void initUserFile() {
    ifstream file("datas/user.csv");
    if (!file.is_open()) {
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

vector<string> readUserFromFile(const string& acc) {
    ifstream file("datas/user.csv");
    if (!file.is_open()) {
        return {};
    }
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
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

bool addUserToCSV(const string& acc, const string& pwd, int role, const string& name, const string& phone) {
    vector<string> existing = readUserFromFile(acc);
    if (!existing.empty()) {
        return false;
    }
    ofstream file("datas/user.csv", ios::app);
    if (!file.is_open()) return false;
    file << acc << "," << pwd << "," << role << "," << name << "," << phone << endl;
    file.close();
    return true;
}

bool modifyUserInCSV(const string& acc, const string& pwd, const string& name, const string& phone) {
    ifstream file("datas/user.csv");
    if (!file.is_open()) return false;
    vector<string> lines;
    string line;
    getline(file, line);
    lines.push_back(line);
    bool found = false;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string fileAcc, pwd2, role, oldName, oldPhone;
        getline(ss, fileAcc, ',');
        getline(ss, pwd2, ',');
        getline(ss, role, ',');
        getline(ss, oldName, ',');
        getline(ss, oldPhone, ',');
        if (fileAcc == acc) {
            lines.push_back(acc + "," + pwd + "," + role + "," + name + "," + phone);
            found = true;
        } else {
            lines.push_back(line);
        }
    }
    file.close();
    if (!found) return false;
    ofstream outFile("datas/user.csv");
    for (const auto& l : lines) {
        outFile << l << endl;
    }
    outFile.close();
    return true;
}

bool deleteUserFromCSV(const string& acc) {
    ifstream file("datas/user.csv");
    if (!file.is_open()) return false;
    vector<string> lines;
    string line;
    getline(file, line);
    lines.push_back(line);
    bool found = false;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string fileAcc;
        getline(ss, fileAcc, ',');
        if (fileAcc == acc) {
            found = true;
        } else {
            lines.push_back(line);
        }
    }
    file.close();
    if (!found) return false;
    ofstream outFile("datas/user.csv");
    for (const auto& l : lines) {
        outFile << l << endl;
    }
    outFile.close();
    return true;
}

void bookManageMenu(BookManager& bookManager) {
    cout << "\n--- 书籍管理 ---" << endl;
    cout << "1. 添加书籍" << endl;
    cout << "2. 修改书籍" << endl;
    cout << "3. 删除书籍" << endl;
    cout << "4. 查看所有书籍" << endl;
    cout << "5. 搜索书籍" << endl;
    cout << "0. 返回" << endl;
    int choice = readInt("请选择：");
    
    if (choice == 1) {
        string id = readLine("请输入书号：");
        string title = readLine("请输入书名：");
        string author = readLine("请输入作者：");
        string publisher = readLine("请输入出版社：");
        double price = readDouble("请输入价格：");
        int stock = readInt("请输入库存：");
        string categoryId = readLine("请输入分类ID：");
        bookManager.addBook(Book(id, title, author, publisher, price, stock, categoryId));
        cout << "添加成功。" << endl;
    } else if (choice == 2) {
        string id = readLine("请输入要修改的书号：");
        Book* book = bookManager.searchBook(id);
        if (book) {
            string title = readLine("请输入新书名：");
            string author = readLine("请输入新作者：");
            string publisher = readLine("请输入新出版社：");
            double price = readDouble("请输入新价格：");
            int stock = readInt("请输入新库存：");
            string categoryId = readLine("请输入新分类ID：");
            bookManager.modifyBook(id, Book(id, title, author, publisher, price, stock, categoryId));
            cout << "修改成功。" << endl;
        } else {
            cout << "未找到书籍。" << endl;
        }
    } else if (choice == 3) {
        string id = readLine("请输入要删除的书号：");
        Book* book = bookManager.searchBook(id);
        if (book) {
            cout << "找到书籍：《" << book->getTitle() << "》，确认删除？(y/n)：";
            char confirm;
            cin >> confirm;
            clearInputBuffer();
            if (confirm == 'y' || confirm == 'Y') {
                bookManager.deleteBook(id);
                cout << "删除成功。" << endl;
            } else {
                cout << "已取消删除。" << endl;
            }
        } else {
            cout << "未找到书籍。" << endl;
        }
    } else if (choice == 4) {
        bookManager.showAllBooks();
    } else if (choice == 5) {
        string id = readLine("请输入书号：");
        Book* book = bookManager.searchBook(id);
        if (book) {
            cout << "书号：" << book->getId() << endl;
            cout << "书名：" << book->getTitle() << endl;
            cout << "作者：" << book->getAuthor() << endl;
            cout << "出版社：" << book->getPublisher() << endl;
            cout << "价格：" << fixed << setprecision(2) << book->getPrice() << endl;
            cout << "库存：" << book->getStock() << endl;
            cout << "分类：" << book->getCategoryId() << endl;
        } else {
            cout << "未找到书籍。" << endl;
        }
    }
}

void memberManageMenu(MemberManager& memberManager) {
    cout << "\n--- 会员管理 ---" << endl;
    cout << "1. 添加会员" << endl;
    cout << "2. 修改会员" << endl;
    cout << "3. 删除会员" << endl;
    cout << "4. 查看所有会员" << endl;
    cout << "0. 返回" << endl;
    int choice = readInt("请选择：");
    
    if (choice == 1) {
        string acc = readLine("请输入账号：");
        if (!readUserFromFile(acc).empty()) {
            cout << "该账号已存在。" << endl;
            return;
        }
        string pwd = readLine("请输入密码：");
        string name = readLine("请输入姓名：");
        string phone = readLine("请输入电话：");
        string regDate = DateUtil::getToday();
        memberManager.addMember(Member(acc, pwd, name, phone, regDate));
        if (addUserToCSV(acc, pwd, 3, name, phone)) {
            cout << "添加成功，注册日期：" << regDate << endl;
        } else {
            cout << "会员已添加，但用户文件写入失败。" << endl;
        }
    } else if (choice == 2) {
        string acc = readLine("请输入要修改的账号：");
        Member* member = memberManager.searchMember(acc);
        if (member) {
            string pwd = readLine("请输入新密码：");
            string name = readLine("请输入新姓名：");
            string phone = readLine("请输入新电话：");
            string regDate = member->getRegDate();
            memberManager.modifyMember(acc, Member(acc, pwd, name, phone, regDate));
            modifyUserInCSV(acc, pwd, name, phone);
            cout << "修改成功。" << endl;
        } else {
            cout << "未找到会员。" << endl;
        }
    } else if (choice == 3) {
        string acc = readLine("请输入要删除的账号：");
        Member* member = memberManager.searchMember(acc);
        if (member) {
            cout << "确认删除会员「" << member->getName() << "」？(y/n)：";
            char confirm;
            cin >> confirm;
            clearInputBuffer();
            if (confirm == 'y' || confirm == 'Y') {
                memberManager.deleteMember(acc);
                deleteUserFromCSV(acc);
                cout << "删除成功。" << endl;
            } else {
                cout << "已取消删除。" << endl;
            }
        } else {
            cout << "未找到会员。" << endl;
        }
    } else if (choice == 4) {
        memberManager.showAllMembers();
    }
}

void categoryManageMenu(BookManager& bookManager) {
    cout << "\n--- 分类管理 ---" << endl;
    cout << "1. 添加分类" << endl;
    cout << "2. 查看所有分类" << endl;
    cout << "0. 返回" << endl;
    int choice = readInt("请选择：");
    
    if (choice == 1) {
        string id = readLine("请输入分类ID：");
        string name = readLine("请输入分类名称：");
        bookManager.addCategory(BookCategory(id, name));
        cout << "添加成功。" << endl;
    } else if (choice == 2) {
        bookManager.showAllCategories();
    }
}

void rentalManageMenu(RentalManager& rentalManager, BookManager& bookManager, const string& memberId, bool isMember) {
    cout << "\n--- 租借管理 ---" << endl;
    cout << "1. 租借书籍" << endl;
    cout << "2. 归还书籍" << endl;
    cout << "3. 查看所有记录" << endl;
    if (!isMember) {
        cout << "4. 查看逾期记录" << endl;
    }
    cout << "0. 返回" << endl;
    int choice = readInt("请选择：");
    
    if (choice == 1) {
        string bookId = readLine("请输入书号：");
        string actualMemberId = isMember ? memberId : readLine("请输入会员账号：");
        Book* book = bookManager.searchBook(bookId);
        if (book && book->getStock() > 0) {
            string rentalDate = DateUtil::getToday();
            cout << "租借日期默认为今天（" << rentalDate << "），按回车确认或输入其他日期（YYYY-MM-DD）：";
            string customDate;
            getline(cin, customDate);
            if (!customDate.empty()) {
                rentalDate = customDate;
            }
            string recordId = rentalManager.generateRecordId();
            RentalRecord record(recordId, bookId, actualMemberId, rentalDate, "", 0.0, 0.0);
            rentalManager.addRecord(record);
            book->setStock(book->getStock() - 1);
            string dueDate = DateUtil::addDays(rentalDate, DateUtil::RENTAL_PERIOD_DAYS);
            cout << "租借成功。记录ID：" << recordId << endl;
            cout << "应还日期：" << dueDate << "（" << DateUtil::RENTAL_PERIOD_DAYS << "天内）" << endl;
        } else if (book) {
            cout << "库存不足。" << endl;
        } else {
            cout << "书籍不存在。" << endl;
        }
    } else if (choice == 2) {
        string recordId = readLine("请输入记录ID：");
        string returnDate = DateUtil::getToday();
        cout << "归还日期默认为今天（" << returnDate << "），按回车确认或输入其他日期（YYYY-MM-DD）：";
        string customDate;
        getline(cin, customDate);
        if (!customDate.empty()) {
            returnDate = customDate;
        }
        rentalManager.returnBook(recordId, returnDate, bookManager);
    } else if (choice == 3) {
        if (isMember) {
            rentalManager.showMemberRecords(memberId);
        } else {
            rentalManager.showAllRecords();
        }
    } else if (choice == 4 && !isMember) {
        rentalManager.showOverdueRecords();
    }
}

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    #endif

    createDataDir();
    initUserFile();
    
    BookManager bookManager;
    MemberManager memberManager;
    RentalManager rentalManager;
    
    bookManager.readBooksFromCSV("datas/book.csv");
    bookManager.readCategoriesFromCSV("datas/category.csv");
    memberManager.readMembersFromCSV("datas/member.csv");
    rentalManager.readRecordsFromCSV("datas/rental.csv");
    
    while (true) {
        cout << "\n=== 租书店管理系统 ===" << endl;
        cout << "1. 管理员" << endl;
        cout << "2. 工作人员" << endl;
        cout << "3. 会员" << endl;
        cout << "0. 退出系统" << endl;
        int roleChoice = readInt("请选择角色：");
        
        if (roleChoice == 0) {
            cout << "感谢使用，再见！" << endl;
            break;
        }
        if (roleChoice < 1 || roleChoice > 3) {
            cout << "无效选择，请重新输入。" << endl;
            continue;
        }
        
        string acc = readLine("请输入账号：");
        string pwd = readLine("请输入密码：");
        
        vector<string> userInfo = readUserFromFile(acc);
        if (userInfo.empty()) {
            cout << "账号不存在。" << endl;
            continue;
        }
        if (userInfo[1] != pwd) {
            cout << "密码错误。" << endl;
            continue;
        }
        int fileRole = 0;
        try { fileRole = stoi(userInfo[2]); } catch (...) {}
        if (fileRole != roleChoice) {
            cout << "角色不匹配。" << endl;
            continue;
        }
        
        cout << "登录成功！欢迎，" << userInfo[3] << endl;
        
        UserBase* user = nullptr;
        if (roleChoice == 1) {
            user = new Admin(acc, pwd, userInfo[3]);
        } else if (roleChoice == 2) {
            user = new Staff(acc, pwd, userInfo[3]);
        } else if (roleChoice == 3) {
            user = new Member(acc, pwd, userInfo[3], userInfo[4], DateUtil::getToday());
        }
        
        bool logout = false;
        while (!logout) {
            user->showMenu();
            int choice = readInt("");
            
            if (user->getRoleType() == 1) {
                switch (choice) {
                    case 1: bookManageMenu(bookManager); break;
                    case 2: memberManageMenu(memberManager); break;
                    case 3: rentalManageMenu(rentalManager, bookManager, "", false); break;
                    case 4: rentalManager.showOverdueRecords(); break;
                    case 5: categoryManageMenu(bookManager); break;
                    case 6: {
                        bookManager.saveBooksToCSV("datas/book.csv");
                        bookManager.saveCategoriesToCSV("datas/category.csv");
                        memberManager.saveMembersToCSV("datas/member.csv");
                        rentalManager.saveRecordsToCSV("datas/rental.csv");
                        cout << "数据已保存。" << endl;
                        break;
                    }
                    case 0: logout = true; cout << "已退出登录。" << endl; break;
                    default: cout << "无效选择，请重新输入。" << endl; break;
                }
            } else if (user->getRoleType() == 2) {
                switch (choice) {
                    case 1: bookManageMenu(bookManager); break;
                    case 2: rentalManageMenu(rentalManager, bookManager, "", false); break;
                    case 3: rentalManager.showOverdueRecords(); break;
                    case 0: logout = true; cout << "已退出登录。" << endl; break;
                    default: cout << "无效选择，请重新输入。" << endl; break;
                }
            } else if (user->getRoleType() == 3) {
                switch (choice) {
                    case 1: bookManager.showAllBooks(); break;
                    case 2: rentalManageMenu(rentalManager, bookManager, user->getAcc(), true); break;
                    case 3: {
                        string recordId = readLine("请输入记录ID：");
                        string returnDate = DateUtil::getToday();
                        cout << "归还日期默认为今天（" << returnDate << "），按回车确认或输入其他日期（YYYY-MM-DD）：";
                        string customDate;
                        getline(cin, customDate);
                        if (!customDate.empty()) {
                            returnDate = customDate;
                        }
                        rentalManager.returnBook(recordId, returnDate, bookManager);
                        break;
                    }
                    case 4: rentalManager.showMemberRecords(user->getAcc()); break;
                    case 0: logout = true; cout << "已退出登录。" << endl; break;
                    default: cout << "无效选择，请重新输入。" << endl; break;
                }
            }
        }
        delete user;
    }
    return 0;
}
