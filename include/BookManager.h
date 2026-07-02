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
    
    void addBook(const Book& book);
    void modifyBook(const std::string& id, const Book& book);
    bool deleteBook(const std::string& id);
    Book* searchBook(const std::string& id);
    void showAllBooks();
    
    void addCategory(const BookCategory& category);
    BookCategory* searchCategory(const std::string& id);
    void showAllCategories();
    
    void saveBooksToCSV(const std::string& filename);
    void readBooksFromCSV(const std::string& filename);
    void saveCategoriesToCSV(const std::string& filename);
    void readCategoriesFromCSV(const std::string& filename);
};

#endif
