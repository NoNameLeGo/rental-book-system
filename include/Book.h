#ifndef BOOK_H
#define BOOK_H

#include <string>

class Book {
private:
    std::string id;        // 书号
    std::string title;     // 书名
    std::string author;    // 作者
    std::string publisher; // 出版社
    double price;          // 价格
    int stock;             // 库存
    std::string categoryId; // 分类ID

public:
    Book() : price(0.0), stock(0) {}
    Book(std::string i, std::string t, std::string a, std::string p, double pr, int s, std::string c)
        : id(i), title(t), author(a), publisher(p), price(pr), stock(s), categoryId(c) {}
    ~Book() {}
    
    // getter和setter
    std::string getId() const { return id; }
    std::string getTitle() const { return title; }
    std::string getAuthor() const { return author; }
    std::string getPublisher() const { return publisher; }
    double getPrice() const { return price; }
    int getStock() const { return stock; }
    std::string getCategoryId() const { return categoryId; }
    
    void setId(std::string i) { id = i; }
    void setTitle(std::string t) { title = t; }
    void setAuthor(std::string a) { author = a; }
    void setPublisher(std::string p) { publisher = p; }
    void setPrice(double pr) { price = pr; }
    void setStock(int s) { stock = s; }
    void setCategoryId(std::string c) { categoryId = c; }
};

#endif