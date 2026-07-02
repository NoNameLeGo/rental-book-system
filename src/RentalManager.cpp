#include "RentalManager.h"
#include "BookManager.h"
#include "DateUtil.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

void RentalManager::addRecord(const RentalRecord& record) { records.push_back(record); }

string RentalManager::generateRecordId() {
    recordCounter++;
    return "R" + to_string(time(0)) + to_string(recordCounter);
}

void RentalManager::returnBook(const string& recordId, const string& returnDate, BookManager& bookManager) {
    for (auto& r : records) {
        if (r.getId() == recordId) {
            if (!r.getReturnDate().empty()) {
                cout << "该记录已归还，无需重复操作。" << endl;
                return;
            }
            r.setReturnDate(returnDate);
            Book* book = bookManager.searchBook(r.getBookId());
            if (book) {
                book->setStock(book->getStock() + 1);
                int days = DateUtil::daysBetween(r.getRentalDate(), returnDate);
                if (days < 0) days = 0;
                double rentalFee = DateUtil::calculateRentalFee(book->getPrice(), days);
                int overdueDays = days - DateUtil::RENTAL_PERIOD_DAYS;
                double overdueFee = DateUtil::calculateOverdueFee(book->getPrice(), overdueDays);
                r.setRentalFee(rentalFee);
                r.setOverdueFee(overdueFee);
                cout << "归还成功。租借天数：" << days << "天" << endl;
                cout << "租金：" << fixed << setprecision(2) << rentalFee << "元" << endl;
                if (overdueFee > 0) {
                    cout << "逾期费用：" << fixed << setprecision(2) << overdueFee
                         << "元（逾期" << overdueDays << "天）" << endl;
                }
                cout << "总计：" << fixed << setprecision(2) << (rentalFee + overdueFee) << "元" << endl;
            } else {
                cout << "警告：对应书籍已被删除，无法计算费用，但归还日期已记录。" << endl;
            }
            return;
        }
    }
    cout << "未找到租借记录：" << recordId << endl;
}

void RentalManager::showAllRecords() {
    if (records.empty()) { cout << "没有租借记录。" << endl; return; }
    cout << "记录ID\t\t书号\t会员账号\t租借日期\t归还日期\t租金\t逾期费用" << endl;
    for (const auto& r : records) {
        string returnDate = r.getReturnDate().empty() ? "未归还" : r.getReturnDate();
        cout << r.getId() << "\t" << r.getBookId() << "\t" << r.getMemberId() << "\t"
             << r.getRentalDate() << "\t" << returnDate << "\t"
             << fixed << setprecision(2) << r.getRentalFee() << "\t" << r.getOverdueFee() << endl;
    }
}

void RentalManager::showMemberRecords(const string& memberId) {
    bool found = false;
    for (const auto& r : records) {
        if (r.getMemberId() == memberId) {
            if (!found) {
                cout << "记录ID\t\t书号\t租借日期\t归还日期\t租金\t逾期费用" << endl;
                found = true;
            }
            string returnDate = r.getReturnDate().empty() ? "未归还" : r.getReturnDate();
            cout << r.getId() << "\t" << r.getBookId() << "\t" << r.getRentalDate() << "\t"
                 << returnDate << "\t" << fixed << setprecision(2)
                 << r.getRentalFee() << "\t" << r.getOverdueFee() << endl;
        }
    }
    if (!found) cout << "没有找到您的租借记录。" << endl;
}

void RentalManager::showOverdueRecords() {
    string currentDate = DateUtil::getToday();
    bool found = false;
    for (const auto& r : records) {
        if (r.getReturnDate().empty() && DateUtil::isOverdue(r.getRentalDate(), currentDate)) {
            if (!found) {
                cout << "当前日期：" << currentDate << "，租借期限：" << DateUtil::RENTAL_PERIOD_DAYS << "天" << endl;
                cout << "逾期记录：" << endl;
                cout << "记录ID\t\t书号\t会员账号\t租借日期\t应还日期\t租金\t逾期费用" << endl;
                found = true;
            }
            string dueDate = DateUtil::addDays(r.getRentalDate(), DateUtil::RENTAL_PERIOD_DAYS);
            int overdueDays = DateUtil::daysBetween(dueDate, currentDate);
            cout << r.getId() << "\t" << r.getBookId() << "\t" << r.getMemberId() << "\t"
                 << r.getRentalDate() << "\t" << dueDate << "\t"
                 << fixed << setprecision(2) << r.getRentalFee() << "\t已逾期" << overdueDays << "天" << endl;
        }
    }
    if (!found) cout << "暂无逾期记录。" << endl;
}

void RentalManager::saveRecordsToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) { cout << "无法打开文件：" << filename << endl; return; }
    file << "记录ID,书号,会员账号,租借日期,归还日期,租金,逾期费用" << endl;
    for (const auto& r : records) {
        file << r.getId() << "," << r.getBookId() << "," << r.getMemberId() << ","
             << r.getRentalDate() << "," << r.getReturnDate() << ","
             << fixed << setprecision(2) << r.getRentalFee() << "," << r.getOverdueFee() << endl;
    }
    file.close();
}

void RentalManager::readRecordsFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return;
    records.clear();
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string fields[7];
        for (int i = 0; i < 7; ++i) getline(ss, fields[i], ',');
        records.push_back(RentalRecord(fields[0], fields[1], fields[2], fields[3], fields[4],
                                       stod(fields[5]), stod(fields[6])));
    }
    file.close();
}
