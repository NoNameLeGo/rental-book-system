#ifndef RENTALMANAGER_H
#define RENTALMANAGER_H

#include "RentalRecord.h"
#include <vector>
#include <string>

class RentalManager {
private:
    std::vector<RentalRecord> records;

public:
    RentalManager() {}
    ~RentalManager() {}
    
    void addRecord(const RentalRecord& record);
    void returnBook(const std::string& recordId, const std::string& returnDate);
    void showAllRecords();
    void showMemberRecords(const std::string& memberId);
    void showOverdueRecords();
    double calculateFee(const std::string& bookId, int days);
    
    void saveRecordsToCSV(const std::string& filename);
    void readRecordsFromCSV(const std::string& filename);
};

#endif