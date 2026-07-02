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

static void clearInput() {
    if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); }
    cin.ignore();
}

#ifdef _WIN32
static void createDataDir() { _mkdir("datas"); }
#else
static void createDataDir() { mkdir("datas", 0777); }
#endif

static void initDefaultFiles() {
    ifstream uf("datas/user.csv");
    if (!uf.is_open()) {
        ofstream o("datas/user.csv");
        o << "账号,密码,角色,姓名,电话" << endl
          << "admin,123456,1,管理员,13800138000" << endl
          << "staff1,123456,2,工作人员,13800138001" << endl
          << "member1,123456,3,会员,13800138002" << endl;
        cout << "已创建默认用户文件。" << endl;
    }
    ifstream mf("datas/member.csv");
    if (!mf.is_open()) {
        ofstream o("datas/member.csv");
        o << "账号,密码,姓名,电话,注册日期" << endl
          << "member1,123456,会员,13800138002,2026-01-01" << endl;
        cout << "已创建默认会员文件。" << endl;
    }
}

static void rentBook(const string& memberId, BookManager& bm, RentalManager& rm, MemberManager& mm) {
    Member* member = mm.searchMember(memberId);
    if (!member) { cout << "会员不存在。" << endl; return; }
    cout << "请输入书号："; string bookId; getline(cin, bookId);
    Book* book = bm.searchBook(bookId);
    if (!book) { cout << "书籍不存在。" << endl; return; }
    if (book->getStock() <= 0) { cout << "库存不足。" << endl; return; }
    string rentalDate = DateUtil::getToday();
    string recordId = rm.generateRecordId();
    rm.addRecord(RentalRecord(recordId, bookId, memberId, rentalDate, "", 0.0, 0.0));
    book->setStock(book->getStock() - 1);
    string dueDate = DateUtil::addDays(rentalDate, DateUtil::RENTAL_PERIOD_DAYS);
    cout << "租借成功。记录ID：" << recordId << endl;
    cout << "应还日期：" << dueDate << "（" << DateUtil::RENTAL_PERIOD_DAYS << "天内）" << endl;
}

static void returnBook(BookManager& bm, RentalManager& rm) {
    cout << "请输入记录ID："; string rid; getline(cin, rid);
    rm.returnBook(rid, DateUtil::getToday(), bm);
}

static void handleBookManage(BookManager& bm) {
    cout << "--- 书籍管理 ---" << endl;
    cout << "1. 添加书籍 2. 修改书籍 3. 删除书籍 4. 查看所有书籍 0. 返回" << endl;
    cout << "请选择："; int ch; cin >> ch; clearInput();
    if (ch == 1) {
        string id, title, author, publisher, catId; double price; int stock;
        cout << "请输入书号："; getline(cin, id);
        if (bm.searchBook(id)) { cout << "该书号已存在。" << endl; return; }
        cout << "请输入书名："; getline(cin, title);
        cout << "请输入作者："; getline(cin, author);
        cout << "请输入出版社："; getline(cin, publisher);
        cout << "请输入价格："; cin >> price; clearInput();
        cout << "请输入库存："; cin >> stock; clearInput();
        cout << "请输入分类ID："; getline(cin, catId);
        bm.addBook(Book(id, title, author, publisher, price, stock, catId));
        cout << "添加成功。" << endl;
    } else if (ch == 2) {
        string id; cout << "请输入要修改的书号："; getline(cin, id);
        Book* book = bm.searchBook(id);
        if (!book) { cout << "未找到书籍。" << endl; return; }
        string title, author, publisher, catId; double price; int stock;
        cout << "请输入新书名："; getline(cin, title);
        cout << "请输入新作者："; getline(cin, author);
        cout << "请输入新出版社："; getline(cin, publisher);
        cout << "请输入新价格："; cin >> price; clearInput();
        cout << "请输入新库存："; cin >> stock; clearInput();
        cout << "请输入新分类ID："; getline(cin, catId);
        bm.modifyBook(id, Book(id, title, author, publisher, price, stock, catId));
        cout << "修改成功。" << endl;
    } else if (ch == 3) {
        string id; cout << "请输入要删除的书号："; getline(cin, id);
        cout << (bm.deleteBook(id) ? "删除成功。" : "未找到该书籍。") << endl;
    } else if (ch == 4) { bm.showAllBooks(); }
}

static void handleMemberManage(MemberManager& mm) {
    cout << "--- 会员管理 ---" << endl;
    cout << "1. 添加会员 2. 修改会员 3. 删除会员 4. 查看所有会员 0. 返回" << endl;
    cout << "请选择："; int ch; cin >> ch; clearInput();
    if (ch == 1) {
        string acc, pwd, name, phone;
        cout << "请输入账号："; getline(cin, acc);
        if (mm.searchMember(acc)) { cout << "该账号已存在。" << endl; return; }
        cout << "请输入密码："; getline(cin, pwd);
        cout << "请输入姓名："; getline(cin, name);
        cout << "请输入电话："; getline(cin, phone);
        string regDate = DateUtil::getToday();
        mm.addMember(Member(acc, pwd, name, phone, regDate));
        mm.addUserToCSV(acc, pwd, 3, name, phone);
        cout << "添加成功，注册日期：" << regDate << endl;
    } else if (ch == 2) {
        string acc; cout << "请输入要修改的账号："; getline(cin, acc);
        Member* m = mm.searchMember(acc);
        if (!m) { cout << "未找到会员。" << endl; return; }
        string pwd, name, phone;
        cout << "请输入新密码："; getline(cin, pwd);
        cout << "请输入新姓名："; getline(cin, name);
        cout << "请输入新电话："; getline(cin, phone);
        mm.modifyMember(acc, Member(acc, pwd, name, phone, m->getRegDate()));
        if (!mm.modifyUserInCSV(acc, pwd, name, phone))
            cout << "警告：用户登录文件更新失败。" << endl;
        else cout << "修改成功。" << endl;
    } else if (ch == 3) {
        string acc; cout << "请输入要删除的账号："; getline(cin, acc);
        if (mm.deleteMember(acc)) { mm.deleteUserFromCSV(acc); cout << "删除成功。" << endl; }
        else cout << "未找到该会员。" << endl;
    } else if (ch == 4) { mm.showAllMembers(); }
}

static void handleRentalManage(BookManager& bm, MemberManager& mm, RentalManager& rm, const string& opId = "") {
    cout << "--- 租借管理 ---" << endl;
    cout << "1. 租借书籍 2. 归还书籍 3. 查看所有记录 0. 返回" << endl;
    cout << "请选择："; int ch; cin >> ch; clearInput();
    if (ch == 1) {
        string mid = opId;
        if (mid.empty()) { cout << "请输入会员账号："; getline(cin, mid); }
        rentBook(mid, bm, rm, mm);
    } else if (ch == 2) { returnBook(bm, rm); }
    else if (ch == 3) { rm.showAllRecords(); }
}

int main() {
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
    createDataDir();
    initDefaultFiles();

    BookManager bookManager;
    MemberManager memberManager;
    RentalManager rentalManager;
    bookManager.readBooksFromCSV("datas/book.csv");
    bookManager.readCategoriesFromCSV("datas/category.csv");
    memberManager.readMembersFromCSV("datas/member.csv");
    rentalManager.readRecordsFromCSV("datas/rental.csv");

    while (true) {
        cout << "\n=== 租书店管理系统 ===" << endl;
        cout << "1. 管理员  2. 工作人员  3. 会员  0. 退出" << endl;
        cout << "请选择："; int roleChoice; cin >> roleChoice;
        if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
        cin.ignore();
        if (roleChoice == 0) { cout << "感谢使用，再见！" << endl; break; }
        if (roleChoice < 1 || roleChoice > 3) { cout << "无效选择。" << endl; continue; }

        string acc, pwd;
        cout << "请输入账号："; getline(cin, acc);
        cout << "请输入密码："; getline(cin, pwd);

        vector<string> userInfo = memberManager.readUserFromFile(acc);
        if (userInfo.empty()) { cout << "账号不存在。" << endl; continue; }
        if (userInfo[1] != pwd) { cout << "密码错误。" << endl; continue; }
        if (atoi(userInfo[2].c_str()) != roleChoice) { cout << "角色不匹配。" << endl; continue; }

        cout << "登录成功！欢迎，" << userInfo[3] << endl;
        UserBase* user = nullptr;
        if (roleChoice == 1) user = new Admin(acc, pwd, userInfo[3]);
        else if (roleChoice == 2) user = new Staff(acc, pwd, userInfo[3]);
        else user = new Member(acc, pwd, userInfo[3], userInfo[4], DateUtil::getToday());

        bool logout = false;
        while (!logout) {
            user->showMenu();
            int choice; cin >> choice;
            if (cin.fail()) { cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); continue; }
            cin.ignore();

            int role = user->getRoleType();
            if (role == 1) {
                switch (choice) {
                    case 1: handleBookManage(bookManager); break;
                    case 2: handleMemberManage(memberManager); break;
                    case 3: handleRentalManage(bookManager, memberManager, rentalManager); break;
                    case 4: rentalManager.showOverdueRecords(); break;
                    case 5: {
                        cout << "--- 分类管理 ---" << endl;
                        cout << "1. 查看所有分类 2. 添加分类 0. 返回" << endl;
                        cout << "请选择："; int c; cin >> c; cin.ignore();
                        if (c == 1) bookManager.showAllCategories();
                        else if (c == 2) {
                            string id, name;
                            cout << "请输入分类ID："; getline(cin, id);
                            cout << "请输入分类名称："; getline(cin, name);
                            bookManager.addCategory(BookCategory(id, name));
                            cout << "添加成功。" << endl;
                        }
                        break;
                    }
                    case 6:
                        bookManager.saveBooksToCSV("datas/book.csv");
                        bookManager.saveCategoriesToCSV("datas/category.csv");
                        memberManager.saveMembersToCSV("datas/member.csv");
                        rentalManager.saveRecordsToCSV("datas/rental.csv");
                        cout << "数据已保存。" << endl;
                        break;
                    case 0: logout = true; cout << "已退出登录。" << endl; break;
                    default: cout << "无效选择。" << endl; break;
                }
            } else if (role == 2) {
                switch (choice) {
                    case 1: handleBookManage(bookManager); break;
                    case 2: handleRentalManage(bookManager, memberManager, rentalManager); break;
                    case 3: rentalManager.showOverdueRecords(); break;
                    case 0: logout = true; cout << "已退出登录。" << endl; break;
                    default: cout << "无效选择。" << endl; break;
                }
            } else {
                switch (choice) {
                    case 1: bookManager.showAllBooks(); break;
                    case 2: rentBook(user->getAcc(), bookManager, rentalManager, memberManager); break;
                    case 3: returnBook(bookManager, rentalManager); break;
                    case 4: rentalManager.showMemberRecords(user->getAcc()); break;
                    case 0: logout = true; cout << "已退出登录。" << endl; break;
                    default: cout << "无效选择。" << endl; break;
                }
            }
        }
        delete user;
    }
    return 0;
}
