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

void initMemberFile() {
    ifstream file("datas/member.csv");
    if (!file.is_open()) {
        ofstream outFile("datas/member.csv");
        outFile << "账号,密码,姓名,电话,注册日期" << endl;
        outFile << "member1,123456,会员,13800138002,2026-01-01" << endl;
        outFile.close();
        cout << "已创建默认会员文件。" << endl;
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

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    createDataDir();
    initUserFile();
    initMemberFile();

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
        cout << "请选择：";
        int roleChoice;
        cin >> roleChoice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入无效，请输入数字。" << endl;
            continue;
        }
        cin.ignore();

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
        getline(cin, acc);
        cout << "请输入密码：";
        getline(cin, pwd);

        vector<string> userInfo = readUserFromFile(acc);
        if (userInfo.empty()) {
            cout << "账号不存在。" << endl;
            continue;
        }
        if (userInfo[1] != pwd) {
            cout << "密码错误。" << endl;
            continue;
        }
        int fileRole = atoi(userInfo[2].c_str());
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
            int choice;
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "输入无效，请输入数字。" << endl;
                continue;
            }
            cin.ignore();

            if (user->getRoleType() == 1) {
                switch (choice) {
                    case 1: {
                        cout << "--- 书籍管理 ---" << endl;
                        cout << "1. 添加书籍 2. 修改书籍 3. 删除书籍 4. 查看所有书籍 0. 返回" << endl;
                        int bookChoice;
                        cin >> bookChoice;
                        cin.ignore();
                        if (bookChoice == 1) {
                            string id, title, author, publisher, categoryId;
                            double price;
                            int stock;
                            cout << "请输入书号："; getline(cin, id);
                            if (bookManager.searchBook(id) != NULL) {
                                cout << "该书号已存在，请勿重复添加。" << endl;
                                break;
                            }
                            cout << "请输入书名："; getline(cin, title);
                            cout << "请输入作者："; getline(cin, author);
                            cout << "请输入出版社："; getline(cin, publisher);
                            cout << "请输入价格："; cin >> price; cin.ignore();
                            cout << "请输入库存："; cin >> stock; cin.ignore();
                            cout << "请输入分类ID："; getline(cin, categoryId);
                            bookManager.addBook(Book(id, title, author, publisher, price, stock, categoryId));
                            cout << "添加成功。" << endl;
                        } else if (bookChoice == 2) {
                            string id;
                            cout << "请输入要修改的书号："; getline(cin, id);
                            Book* book = bookManager.searchBook(id);
                            if (book) {
                                string title, author, publisher, categoryId;
                                double price;
                                int stock;
                                cout << "请输入新书名："; getline(cin, title);
                                cout << "请输入新作者："; getline(cin, author);
                                cout << "请输入新出版社："; getline(cin, publisher);
                                cout << "请输入新价格："; cin >> price; cin.ignore();
                                cout << "请输入新库存："; cin >> stock; cin.ignore();
                                cout << "请输入新分类ID："; getline(cin, categoryId);
                                bookManager.modifyBook(id, Book(id, title, author, publisher, price, stock, categoryId));
                                cout << "修改成功。" << endl;
                            } else {
                                cout << "未找到书籍。" << endl;
                            }
                        } else if (bookChoice == 3) {
                            string id;
                            cout << "请输入要删除的书号："; getline(cin, id);
                            if (bookManager.deleteBook(id)) {
                                cout << "删除成功。" << endl;
                            } else {
                                cout << "未找到该书籍。" << endl;
                            }
                        } else if (bookChoice == 4) {
                            bookManager.showAllBooks();
                        }
                        break;
                    }
                    case 2: {
                        cout << "--- 会员管理 ---" << endl;
                        cout << "1. 添加会员 2. 修改会员 3. 删除会员 4. 查看所有会员 0. 返回" << endl;
                        int memberChoice;
                        cin >> memberChoice;
                        cin.ignore();
                        if (memberChoice == 1) {
                            string acc2, pwd2, name, phone;
                            cout << "请输入账号："; getline(cin, acc2);
                            if (memberManager.searchMember(acc2) != NULL) {
                                cout << "该账号已存在，请勿重复添加。" << endl;
                                break;
                            }
                            cout << "请输入密码："; getline(cin, pwd2);
                            cout << "请输入姓名："; getline(cin, name);
                            cout << "请输入电话："; getline(cin, phone);
                            string regDate = DateUtil::getToday();
                            memberManager.addMember(Member(acc2, pwd2, name, phone, regDate));
                            if (addUserToCSV(acc2, pwd2, 3, name, phone)) {
                                cout << "添加成功，注册日期：" << regDate << endl;
                            } else {
                                cout << "会员已添加，但用户文件写入失败。" << endl;
                            }
                        } else if (memberChoice == 2) {
                            string acc2;
                            cout << "请输入要修改的账号："; getline(cin, acc2);
                            Member* member = memberManager.searchMember(acc2);
                            if (member) {
                                string pwd2, name, phone;
                                cout << "请输入新密码："; getline(cin, pwd2);
                                cout << "请输入新姓名："; getline(cin, name);
                                cout << "请输入新电话："; getline(cin, phone);
                                string regDate = member->getRegDate();
                                memberManager.modifyMember(acc2, Member(acc2, pwd2, name, phone, regDate));
                                if (!modifyUserInCSV(acc2, pwd2, name, phone)) {
                                    cout << "警告：会员信息已修改，但用户登录文件更新失败。" << endl;
                                } else {
                                    cout << "修改成功。" << endl;
                                }
                            } else {
                                cout << "未找到会员。" << endl;
                            }
                        } else if (memberChoice == 3) {
                            string acc2;
                            cout << "请输入要删除的账号："; getline(cin, acc2);
                            if (memberManager.deleteMember(acc2)) {
                                if (!deleteUserFromCSV(acc2)) {
                                    cout << "警告：会员已删除，但用户登录文件更新失败。" << endl;
                                } else {
                                    cout << "删除成功。" << endl;
                                }
                            } else {
                                cout << "未找到该会员。" << endl;
                            }
                        } else if (memberChoice == 4) {
                            memberManager.showAllMembers();
                        }
                        break;
                    }
                    case 3: {
                        cout << "--- 租借管理 ---" << endl;
                        cout << "1. 租借书籍 2. 归还书籍 3. 查看所有记录 0. 返回" << endl;
                        int rentalChoice;
                        cin >> rentalChoice;
                        cin.ignore();
                        if (rentalChoice == 1) {
                            string bookId, memberId;
                            cout << "请输入书号："; getline(cin, bookId);
                            cout << "请输入会员账号："; getline(cin, memberId);
                            Member* member = memberManager.searchMember(memberId);
                            if (!member) {
                                cout << "会员不存在。" << endl;
                                break;
                            }
                            Book* book = bookManager.searchBook(bookId);
                            if (book && book->getStock() > 0) {
                                string rentalDate = DateUtil::getToday();
                                string recordId = rentalManager.generateRecordId();
                                RentalRecord record(recordId, bookId, memberId, rentalDate, "", 0.0, 0.0);
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
                        } else if (rentalChoice == 2) {
                            string recordId;
                            cout << "请输入记录ID："; getline(cin, recordId);
                            string returnDate = DateUtil::getToday();
                            rentalManager.returnBook(recordId, returnDate, bookManager);
                        } else if (rentalChoice == 3) {
                            rentalManager.showAllRecords();
                        }
                        break;
                    }
                    case 4: {
                        rentalManager.showOverdueRecords();
                        break;
                    }
                    case 5: {
                        cout << "--- 分类管理 ---" << endl;
                        cout << "1. 查看所有分类 2. 添加分类 0. 返回" << endl;
                        int catChoice;
                        cin >> catChoice;
                        cin.ignore();
                        if (catChoice == 1) {
                            bookManager.showAllCategories();
                        } else if (catChoice == 2) {
                            string catId, catName;
                            cout << "请输入分类ID："; getline(cin, catId);
                            cout << "请输入分类名称："; getline(cin, catName);
                            bookManager.addCategory(BookCategory(catId, catName));
                            cout << "添加成功。" << endl;
                        }
                        break;
                    }
                    case 6: {
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
            } else if (user->getRoleType() == 2) {
                switch (choice) {
                    case 1: {
                        cout << "--- 书籍管理 ---" << endl;
                        cout << "1. 查看所有书籍 0. 返回" << endl;
                        int bookChoice;
                        cin >> bookChoice;
                        cin.ignore();
                        if (bookChoice == 1) {
                            bookManager.showAllBooks();
                        }
                        break;
                    }
                    case 2: {
                        cout << "--- 租借管理 ---" << endl;
                        cout << "1. 租借书籍 2. 归还书籍 3. 查看所有记录 0. 返回" << endl;
                        int rentalChoice;
                        cin >> rentalChoice;
                        cin.ignore();
                        if (rentalChoice == 1) {
                            string bookId, memberId;
                            cout << "请输入书号："; getline(cin, bookId);
                            cout << "请输入会员账号："; getline(cin, memberId);
                            Member* member = memberManager.searchMember(memberId);
                            if (!member) {
                                cout << "会员不存在。" << endl;
                                break;
                            }
                            Book* book = bookManager.searchBook(bookId);
                            if (book && book->getStock() > 0) {
                                string rentalDate = DateUtil::getToday();
                                string recordId = rentalManager.generateRecordId();
                                RentalRecord record(recordId, bookId, memberId, rentalDate, "", 0.0, 0.0);
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
                        } else if (rentalChoice == 2) {
                            string recordId;
                            cout << "请输入记录ID："; getline(cin, recordId);
                            string returnDate = DateUtil::getToday();
                            rentalManager.returnBook(recordId, returnDate, bookManager);
                        } else if (rentalChoice == 3) {
                            rentalManager.showAllRecords();
                        }
                        break;
                    }
                    case 3: {
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
            } else if (user->getRoleType() == 3) {
                switch (choice) {
                    case 1: {
                        bookManager.showAllBooks();
                        break;
                    }
                    case 2: {
                        string bookId;
                        cout << "请输入书号："; getline(cin, bookId);
                        Book* book = bookManager.searchBook(bookId);
                        if (book && book->getStock() > 0) {
                            string rentalDate = DateUtil::getToday();
                            string recordId = rentalManager.generateRecordId();
                            RentalRecord record(recordId, bookId, user->getAcc(), rentalDate, "", 0.0, 0.0);
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
                        break;
                    }
                    case 3: {
                        string recordId;
                        cout << "请输入记录ID："; getline(cin, recordId);
                        string returnDate = DateUtil::getToday();
                        rentalManager.returnBook(recordId, returnDate, bookManager);
                        break;
                    }
                    case 4: {
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
