#include "BookManager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdlib>
using namespace std;

void BookManager::addBook(const Book& book) {
    books.push_back(book);
}

void BookManager::modifyBook(const string& id, const Book& book) {
    for (auto& b : books) {
        if (b.getId() == id) {
            b = book;
            return;
        }
    }
    cout << "未找到书籍：" << id << endl;
}

bool BookManager::deleteBook(const string& id) {
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->getId() == id) {
            books.erase(it);
            return true;
        }
    }
    return false;
}

Book* BookManager::searchBook(const string& id) {
    for (auto& b : books) {
        if (b.getId() == id) {
            return &b;
        }
    }
    return nullptr;
}

void BookManager::showAllBooks() {
    if (books.empty()) {
        cout << "没有书籍记录。" << endl;
        return;
    }
    cout << "书号\t书名\t\t作者\t\t出版社\t\t价格\t库存\t分类" << endl;
    cout << "------------------------------------------------------------------------" << endl;
    for (const auto& b : books) {
        cout << b.getId() << "\t" << b.getTitle() << "\t\t" << b.getAuthor() << "\t\t"
                  << b.getPublisher() << "\t" << fixed << setprecision(2) << b.getPrice() 
                  << "\t" << b.getStock() << "\t" << b.getCategoryId() << endl;
    }
}

void BookManager::addCategory(const BookCategory& category) {
    categories.push_back(category);
}

BookCategory* BookManager::searchCategory(const string& id) {
    for (auto& c : categories) {
        if (c.getId() == id) {
            return &c;
        }
    }
    return nullptr;
}

void BookManager::showAllCategories() {
    if (categories.empty()) {
        cout << "没有分类记录。" << endl;
        return;
    }
    cout << "分类ID\t分类名称" << endl;
    for (const auto& c : categories) {
        cout << c.getId() << "\t" << c.getName() << endl;
    }
}

void BookManager::saveBooksToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "无法打开文件：" << filename << endl;
        return;
    }
    file << "书号,书名,作者,出版社,价格,库存,分类ID" << endl;
    for (const auto& b : books) {
        file << b.getId() << "," << b.getTitle() << "," << b.getAuthor() << ","
             << b.getPublisher() << "," << fixed << setprecision(2) << b.getPrice() 
             << "," << b.getStock() << "," << b.getCategoryId() << endl;
    }
    file.close();
}

void BookManager::readBooksFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    books.clear();
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, title, author, publisher, categoryId;
        string priceStr, stockStr;
        getline(ss, id, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, publisher, ',');
        getline(ss, priceStr, ',');
        getline(ss, stockStr, ',');
        getline(ss, categoryId, ',');
        
        double price = atof(priceStr.c_str());
        int stock = atoi(stockStr.c_str());
        
        books.push_back(Book(id, title, author, publisher, price, stock, categoryId));
    }
    file.close();
}

void BookManager::saveCategoriesToCSV(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "无法打开文件：" << filename << endl;
        return;
    }
    file << "分类ID,分类名称" << endl;
    for (const auto& c : categories) {
        file << c.getId() << "," << c.getName() << endl;
    }
    file.close();
}

void BookManager::readCategoriesFromCSV(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    categories.clear();
    string line;
    getline(file, line);
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string id, name;
        getline(ss, id, ',');
        getline(ss, name, ',');
        categories.push_back(BookCategory(id, name));
    }
    file.close();
}
