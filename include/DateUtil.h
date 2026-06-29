#ifndef DATEUTIL_H
#define DATEUTIL_H

#include <string>
#include <ctime>

class DateUtil {
public:
    static const int RENTAL_PERIOD_DAYS = 30;
    static const double DAILY_RATE_RATIO;
    static const double OVERDUE_RATE_RATIO;

    static std::string getToday() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buf[20];
        sprintf(buf, "%04d-%02d-%02d", ltm->tm_year + 1900, ltm->tm_mon + 1, ltm->tm_mday);
        return std::string(buf);
    }

    static tm parseDate(const std::string& date) {
        tm t = {};
        if (date.empty()) return t;
        int y, m, d;
        if (sscanf(date.c_str(), "%d-%d-%d", &y, &m, &d) == 3) {
            t.tm_year = y - 1900;
            t.tm_mon = m - 1;
            t.tm_mday = d;
            t.tm_hour = 0;
            t.tm_min = 0;
            t.tm_sec = 0;
            t.tm_isdst = -1;
        }
        return t;
    }

    static int daysBetween(const std::string& startDate, const std::string& endDate) {
        tm t1 = parseDate(startDate);
        tm t2 = parseDate(endDate);
        time_t time1 = mktime(&t1);
        time_t time2 = mktime(&t2);
        if (time1 == -1 || time2 == -1) return 0;
        double diff = difftime(time2, time1) / (60 * 60 * 24);
        return static_cast<int>(diff >= 0 ? diff + 0.5 : diff - 0.5);
    }

    static std::string addDays(const std::string& date, int days) {
        tm t = parseDate(date);
        time_t tt = mktime(&t);
        if (tt == -1) return date;
        tt += days * 24 * 60 * 60;
        tm* result = localtime(&tt);
        char buf[20];
        sprintf(buf, "%04d-%02d-%02d", result->tm_year + 1900, result->tm_mon + 1, result->tm_mday);
        return std::string(buf);
    }

    static bool isOverdue(const std::string& rentalDate, const std::string& currentDate) {
        std::string dueDate = addDays(rentalDate, RENTAL_PERIOD_DAYS);
        return currentDate > dueDate;
    }

    static double calculateRentalFee(double bookPrice, int days) {
        if (days < 0) days = 0;
        return bookPrice * DAILY_RATE_RATIO * days;
    }

    static double calculateOverdueFee(double bookPrice, int overdueDays) {
        if (overdueDays <= 0) return 0.0;
        return bookPrice * OVERDUE_RATE_RATIO * overdueDays;
    }
};

#endif
