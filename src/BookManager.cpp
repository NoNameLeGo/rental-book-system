#include "BookManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

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
    std::cout << "书号\t书名\t作者\t出版社\t价格\t库存\t分类" << std::endl;
    for (const auto& b : books) {
        std::cout << b.getId() << "\t" << b.getTitle() << "\t" << b.getAuthor() << "\t"
                  << b.getPublisher() << "\t" << b.getPrice() << "\t" << b.getStock() << "\t" << b.getCategoryId() << std::endl;
    }
}

void BookManager::addCategory(const BookCategory& category) {
    categories.push_back(category);
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
             << b.getPublisher() << "," << b.getPrice() << "," << b.getStock() << "," << b.getCategoryId() << std::endl;
    }
    file.close();
}

void BookManager::readBooksFromCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "无法打开文件：" << filename << std::endl;
        return;
    }
    books.clear();
    std::string line;
    std::getline(file, line); // 跳过标题行
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, title, author, publisher, categoryId;
        double price;
        int stock;
        std::getline(ss, id, ',');
        std::getline(ss, title, ',');
        std::getline(ss, author, ',');
        std::getline(ss, publisher, ',');
        ss >> price;
        ss.ignore();
        ss >> stock;
        ss.ignore();
        std::getline(ss, categoryId, ',');
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
        std::cout << "无法打开文件：" << filename << std::endl;
        return;
    }
    categories.clear();
    std::string line;
    std::getline(file, line); // 跳过标题行
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, name;
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        categories.push_back(BookCategory(id, name));
    }
    file.close();
}