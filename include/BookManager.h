#ifndef BOOKMANAGER_H
#define BOOKMANAGER_H

#include "Book.h"
#include "BookCategory.h"
#include <vector>
#include <string>

class BookManager {
private:
    std::vector<Book> books;
    std::vector<BookCategory> categories;

public:
    BookManager() {}
    ~BookManager() {}
    
    // 书籍管理
    void addBook(const Book& book);
    void modifyBook(const std::string& id, const Book& book);
    void deleteBook(const std::string& id);
    Book* searchBook(const std::string& id);
    void showAllBooks();
    
    // 分类管理
    void addCategory(const BookCategory& category);
    void showAllCategories();
    
    // 文件操作
    void saveBooksToCSV(const std::string& filename);
    void readBooksFromCSV(const std::string& filename);
    void saveCategoriesToCSV(const std::string& filename);
    void readCategoriesFromCSV(const std::string& filename);
};

#endif