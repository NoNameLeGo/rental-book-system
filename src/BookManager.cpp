#include "BookManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

void BookManager::addBook(const Book& book) {
    books.push_back(book);
}

void BookManager::modifyBook(const std::string& id, const Book& book) {
    for (auto& b : books) {
        if (b.getId() == id) {
            b = book;
            return;
        }
    }
    std::cout << "未找到书籍：" << id << std::endl;
}

void BookManager::deleteBook(const std::string& id) {
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->getId() == id) {
            books.erase(it);
            return;
        }
    }
    std::cout << "未找到书籍：" << id << std::endl;
}

Book* BookManager::searchBook(const std::string& id) {
    for (auto& b : books) {
        if (b.getId() == id) {
            return &b;
        }
    }
    return nullptr;
}

void BookManager::showAllBooks() {
    if (books.empty()) {
        std::cout << "没有书籍记录。" << std::endl;
        return;
    }
    std::cout << "书号\t书名\t\t作者\t\t出版社\t\t价格\t库存\t分类" << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    for (const auto& b : books) {
        std::cout << b.getId() << "\t" << b.getTitle() << "\t\t" << b.getAuthor() << "\t\t"
                  << b.getPublisher() << "\t" << std::fixed << std::setprecision(2) << b.getPrice() 
                  << "\t" << b.getStock() << "\t" << b.getCategoryId() << std::endl;
    }
}

void BookManager::addCategory(const BookCategory& category) {
    categories.push_back(category);
}

BookCategory* BookManager::searchCategory(const std::string& id) {
    for (auto& c : categories) {
        if (c.getId() == id) {
            return &c;
        }
    }
    return nullptr;
}

void BookManager::showAllCategories() {
    if (categories.empty()) {
        std::cout << "没有分类记录。" << std::endl;
        return;
    }
    std::cout << "分类ID\t分类名称" << std::endl;
    for (const auto& c : categories) {
        std::cout << c.getId() << "\t" << c.getName() << std::endl;
    }
}

void BookManager::saveBooksToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "无法打开文件：" << filename << std::endl;
        return;
    }
    file << "书号,书名,作者,出版社,价格,库存,分类ID" << std::endl;
    for (const auto& b : books) {
        file << b.getId() << "," << b.getTitle() << "," << b.getAuthor() << ","
             << b.getPublisher() << "," << std::fixed << std::setprecision(2) << b.getPrice() 
             << "," << b.getStock() << "," << b.getCategoryId() << std::endl;
    }
    file.close();
}

void BookManager::readBooksFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    books.clear();
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, title, author, publisher, categoryId;
        std::string priceStr, stockStr;
        std::getline(ss, id, ',');
        std::getline(ss, title, ',');
        std::getline(ss, author, ',');
        std::getline(ss, publisher, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, stockStr, ',');
        std::getline(ss, categoryId, ',');
        
        double price = 0.0;
        int stock = 0;
        try { price = std::stod(priceStr); } catch (...) {}
        try { stock = std::stoi(stockStr); } catch (...) {}
        
        books.push_back(Book(id, title, author, publisher, price, stock, categoryId));
    }
    file.close();
}

void BookManager::saveCategoriesToCSV(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "无法打开文件：" << filename << std::endl;
        return;
    }
    file << "分类ID,分类名称" << std::endl;
    for (const auto& c : categories) {
        file << c.getId() << "," << c.getName() << std::endl;
    }
    file.close();
}

void BookManager::readCategoriesFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    categories.clear();
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, name;
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        categories.push_back(BookCategory(id, name));
    }
    file.close();
}
