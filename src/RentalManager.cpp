#include "RentalManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

void RentalManager::addRecord(const RentalRecord& record) {
    records.push_back(record);
}

void RentalManager::returnBook(const std::string& recordId, const std::string& returnDate) {
    for (auto& r : records) {
        if (r.getId() == recordId) {
            r.setReturnDate(returnDate);
            // 这里可以计算逾期费用，简化处理
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
    std::cout << "记录ID\t书号\t会员账号\t租借日期\t归还日期\t租金\t逾期费用" << std::endl;
    for (const auto& r : records) {
        std::cout << r.getId() << "\t" << r.getBookId() << "\t" << r.getMemberId() << "\t"
                  << r.getRentalDate() << "\t" << r.getReturnDate() << "\t" << r.getRentalFee() << "\t" << r.getOverdueFee() << std::endl;
    }
}

void RentalManager::showMemberRecords(const std::string& memberId) {
    if (records.empty()) {
        std::cout << "没有租借记录。" << std::endl;
        return;
    }
    std::cout << "记录ID\t书号\t租借日期\t归还日期\t租金\t逾期费用" << std::endl;
    for (const auto& r : records) {
        if (r.getMemberId() == memberId) {
            std::cout << r.getId() << "\t" << r.getBookId() << "\t" << r.getRentalDate() << "\t"
                      << r.getReturnDate() << "\t" << r.getRentalFee() << "\t" << r.getOverdueFee() << std::endl;
        }
    }
}

void RentalManager::showOverdueRecords() {
    // 获取当前日期
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char currentDate[20];
    sprintf(currentDate, "%04d-%02d-%02d", ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday);
    
    if (records.empty()) {
        std::cout << "没有租借记录。" << std::endl;
        return;
    }
    std::cout << "逾期记录：" << std::endl;
    std::cout << "记录ID\t书号\t会员账号\t租借日期\t归还日期\t租金\t逾期费用" << std::endl;
    for (const auto& r : records) {
        if (r.getReturnDate().empty() && r.getRentalDate() < currentDate) {
            // 简化处理：如果归还日期为空且租借日期早于当前日期，则视为逾期
            std::cout << r.getId() << "\t" << r.getBookId() << "\t" << r.getMemberId() << "\t"
                      << r.getRentalDate() << "\t" << r.getReturnDate() << "\t" << r.getRentalFee() << "\t" << r.getOverdueFee() << std::endl;
        }
    }
}

double RentalManager::calculateFee(const std::string& bookId, int days) {
    // 简化处理：每天租金为书籍价格的1%
    // 这里需要访问BookManager，但为了简化，假设书籍价格已知
    // 实际中应该传入BookManager或书籍价格
    // 这里返回一个默认值
    return 10.0 * days; // 假设每天10元
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
             << r.getRentalDate() << "," << r.getReturnDate() << "," << r.getRentalFee() << "," << r.getOverdueFee() << std::endl;
    }
    file.close();
}

void RentalManager::readRecordsFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "无法打开文件：" << filename << std::endl;
        return;
    }
    records.clear();
    std::string line;
    std::getline(file, line); // 跳过标题行
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, bookId, memberId, rentalDate, returnDate;
        double rentalFee, overdueFee;
        std::getline(ss, id, ',');
        std::getline(ss, bookId, ',');
        std::getline(ss, memberId, ',');
        std::getline(ss, rentalDate, ',');
        std::getline(ss, returnDate, ',');
        ss >> rentalFee;
        ss.ignore();
        ss >> overdueFee;
        records.push_back(RentalRecord(id, bookId, memberId, rentalDate, returnDate, rentalFee, overdueFee));
    }
    file.close();
}