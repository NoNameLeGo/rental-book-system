#ifndef BOOKCATEGORY_H
#define BOOKCATEGORY_H

#include <string>

class BookCategory {
private:
    std::string id;    // 分类ID
    std::string name;  // 分类名称

public:
    BookCategory() {}
    BookCategory(std::string i, std::string n) : id(i), name(n) {}
    ~BookCategory() {}
    
    std::string getId() const { return id; }
    std::string getName() const { return name; }
    
    void setId(std::string i) { id = i; }
    void setName(std::string n) { name = n; }
};

#endif