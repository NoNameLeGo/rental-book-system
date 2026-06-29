#ifndef RENTALMANAGER_H
#define RENTALMANAGER_H

#include "RentalRecord.h"
#include <vector>
#include <string>

class BookManager;

class RentalManager {
private:
    std::vector<RentalRecord> records;
    int recordCounter;

public:
    RentalManager() : recordCounter(0) {}
    ~RentalManager() {}
    
    void addRecord(const RentalRecord& record);
    std::string generateRecordId();
    void returnBook(const std::string& recordId, const std::string& returnDate, BookManager& bookManager);
    void showAllRecords();
    void showMemberRecords(const std::string& memberId);
    void showOverdueRecords();
    
    void saveRecordsToCSV(const std::string& filename);
    void readRecordsFromCSV(const std::string& filename);
};

#endif
