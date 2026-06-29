#include "RentalManager.h"
#include "BookManager.h"
#include "DateUtil.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <cstdlib>

void RentalManager::addRecord(const RentalRecord& record) {
    records.push_back(record);
}

std::string RentalManager::generateRecordId() {
    recordCounter++;
    return "R" + std::to_string(time(0)) + std::to_string(recordCounter);
}

void RentalManager::returnBook(const std::string& recordId, const std::string& returnDate, BookManager& bookManager) {
    for (auto& r : records) {
        if (r.getId() == recordId) {
            if (!r.getReturnDate().empty()) {
                std::cout << "该记录已归还，无需重复操作。" << std::endl;
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
                
                std::cout << "归还成功。租借天数：" << days << "天" << std::endl;
                std::cout << "租金：" << std::fixed << std::setprecision(2) << rentalFee << "元" << std::endl;
                if (overdueFee > 0) {
                    std::cout << "逾期费用：" << std::fixed << std::setprecision(2) << overdueFee << "元（逾期" << overdueDays << "天）" << std::endl;
                }
                std::cout << "总计：" << std::fixed << std::setprecision(2) << (rentalFee + overdueFee) << "元" << std::endl;
            } else {
                std::cout << "警告：对应书籍已被删除，无法计算费用，但归还日期已记录。" << std::endl;
            }
            return;
        }
    }
    std::cout << "未找到租借记录：" << recordId << std::endl;
}

void RentalManager::showAllRecords() {
    if (records.empty()) {
        std::cout << "没有租借记录。" << std::endl;
        return;
    }
    std::cout << "记录ID\t\t书号\t会员账号\t租借日期\t归还日期\t租金\t逾期费用" << std::endl;
    for (const auto& r : records) {
        std::string returnDate = r.getReturnDate().empty() ? "未归还" : r.getReturnDate();
        std::cout << r.getId() << "\t" << r.getBookId() << "\t" << r.getMemberId() << "\t"
                  << r.getRentalDate() << "\t" << returnDate << "\t" 
                  << std::fixed << std::setprecision(2) << r.getRentalFee() << "\t" << r.getOverdueFee() << std::endl;
    }
}

void RentalManager::showMemberRecords(const std::string& memberId) {
    bool found = false;
    for (const auto& r : records) {
        if (r.getMemberId() == memberId) {
            if (!found) {
                std::cout << "记录ID\t\t书号\t租借日期\t归还日期\t租金\t逾期费用" << std::endl;
                found = true;
            }
            std::string returnDate = r.getReturnDate().empty() ? "未归还" : r.getReturnDate();
            std::cout << r.getId() << "\t" << r.getBookId() << "\t" << r.getRentalDate() << "\t"
                      << returnDate << "\t" << std::fixed << std::setprecision(2) 
                      << r.getRentalFee() << "\t" << r.getOverdueFee() << std::endl;
        }
    }
    if (!found) {
        std::cout << "没有找到您的租借记录。" << std::endl;
    }
}

void RentalManager::showOverdueRecords() {
    std::string currentDate = DateUtil::getToday();
    bool found = false;
    
    for (const auto& r : records) {
        if (r.getReturnDate().empty() && DateUtil::isOverdue(r.getRentalDate(), currentDate)) {
            if (!found) {
                std::cout << "当前日期：" << currentDate << "，租借期限：" << DateUtil::RENTAL_PERIOD_DAYS << "天" << std::endl;
                std::cout << "逾期记录：" << std::endl;
                std::cout << "记录ID\t\t书号\t会员账号\t租借日期\t应还日期\t租金\t逾期费用" << std::endl;
                found = true;
            }
            std::string dueDate = DateUtil::addDays(r.getRentalDate(), DateUtil::RENTAL_PERIOD_DAYS);
            int overdueDays = DateUtil::daysBetween(dueDate, currentDate);
            std::cout << r.getId() << "\t" << r.getBookId() << "\t" << r.getMemberId() << "\t"
                      << r.getRentalDate() << "\t" << dueDate << "\t"
                      << std::fixed << std::setprecision(2) << r.getRentalFee() << "\t已逾期" << overdueDays << "天" << std::endl;
        }
    }
    if (!found) {
        std::cout << "暂无逾期记录。" << std::endl;
    }
}

void RentalManager::saveRecordsToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "无法打开文件：" << filename << std::endl;
        return;
    }
    file << "记录ID,书号,会员账号,租借日期,归还日期,租金,逾期费用" << std::endl;
    for (const auto& r : records) {
        file << r.getId() << "," << r.getBookId() << "," << r.getMemberId() << ","
             << r.getRentalDate() << "," << r.getReturnDate() << "," 
             << std::fixed << std::setprecision(2) << r.getRentalFee() << "," << r.getOverdueFee() << std::endl;
    }
    file.close();
}

void RentalManager::readRecordsFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    records.clear();
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, bookId, memberId, rentalDate, returnDate;
        std::string rentalFeeStr, overdueFeeStr;
        std::getline(ss, id, ',');
        std::getline(ss, bookId, ',');
        std::getline(ss, memberId, ',');
        std::getline(ss, rentalDate, ',');
        std::getline(ss, returnDate, ',');
        std::getline(ss, rentalFeeStr, ',');
        std::getline(ss, overdueFeeStr, ',');
        
        double rentalFee = atof(rentalFeeStr.c_str());
        double overdueFee = atof(overdueFeeStr.c_str());
        
        records.push_back(RentalRecord(id, bookId, memberId, rentalDate, returnDate, rentalFee, overdueFee));
    }
    file.close();
}
