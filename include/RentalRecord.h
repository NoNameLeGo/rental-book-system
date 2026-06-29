#ifndef RENTALRECORD_H
#define RENTALRECORD_H

#include <string>

class RentalRecord {
private:
    std::string id;        // 记录ID
    std::string bookId;    // 书号
    std::string memberId;  // 会员账号
    std::string rentalDate; // 租借日期
    std::string returnDate; // 归还日期（空字符串表示未归还）
    double rentalFee;      // 租金
    double overdueFee;     // 逾期费用

public:
    RentalRecord() : rentalFee(0.0), overdueFee(0.0) {}
    RentalRecord(std::string i, std::string b, std::string m, std::string rd, std::string rtd, double rf, double of)
        : id(i), bookId(b), memberId(m), rentalDate(rd), returnDate(rtd), rentalFee(rf), overdueFee(of) {}
    ~RentalRecord() {}
    
    // getter和setter
    std::string getId() const { return id; }
    std::string getBookId() const { return bookId; }
    std::string getMemberId() const { return memberId; }
    std::string getRentalDate() const { return rentalDate; }
    std::string getReturnDate() const { return returnDate; }
    double getRentalFee() const { return rentalFee; }
    double getOverdueFee() const { return overdueFee; }
    
    void setId(std::string i) { id = i; }
    void setBookId(std::string b) { bookId = b; }
    void setMemberId(std::string m) { memberId = m; }
    void setRentalDate(std::string rd) { rentalDate = rd; }
    void setReturnDate(std::string rtd) { returnDate = rtd; }
    void setRentalFee(double rf) { rentalFee = rf; }
    void setOverdueFee(double of) { overdueFee = of; }
};

#endif