#ifndef BOOKSTORE_BOOK_H
#define BOOKSTORE_BOOK_H
#include <iostream>
#include <stack>
#include <string>
#include "linkedList.h"
#include "strScanner.h"
#include "error.cpp"
using std::ostream;

class book {
public:
    char ISBN[21] = {};
    char BookName[61] = {};
    char Author[61] = {};
    char Keyword[61] = {};
    long long Book_cnt = 0;
    double Price = 0;

    book() = default;
    book(const char *ISBN_, const char *name_, const char *author_, const char *keyword_, long long cnt_, double price_)
    {
        strcpy(ISBN, ISBN_);
        strcpy(BookName, name_);
        strcpy(Author, author_);
        strcpy(Keyword, keyword_);
        Book_cnt = cnt_;
        Price = price_;
    }
    book(const book &rhs)
    {
        strcpy(ISBN, rhs.ISBN);
        strcpy(BookName, rhs.BookName);
        strcpy(Author, rhs.Author);
        strcpy(Keyword, rhs.Keyword);
        Book_cnt = rhs.Book_cnt;
        Price = rhs.Price;
    }
    book &operator=(const book &rhs)
    {
        if (this == &rhs) return *this;
        strcpy(ISBN, rhs.ISBN);
        strcpy(BookName, rhs.BookName);
        strcpy(Author, rhs.Author);
        strcpy(Keyword, rhs.Keyword);
        Book_cnt = rhs.Book_cnt;
        Price = rhs.Price;
        return *this;
    }
    bool operator<(const book &rhs) const {return strcmp(ISBN, rhs.ISBN) < 0;}
    bool operator>(const book &rhs) const {return strcmp(ISBN, rhs.ISBN) > 0;}
    friend ostream &operator<<(ostream &os, const book &rhs);
};

linkedList<book> bookBlock_ISBN("Book_ISBN_storage");
linkedList<book> bookBlock_BookName("Book_BookName_storage");
linkedList<book> bookBlock_Author("Book_Author_storage");
linkedList<book> bookBlock_Keyword("Book_Keyword_storage");
std::stack<book> bookStack;
void addBook(const book &obj);
void delBook(const book &obj);
void bookIn();
void bookOut();
void show(strScanner &scanner);
void buy(strScanner &scanner);
void select(strScanner &scanner);
void modify(strScanner &scanner);
void import(strScanner &scanner);
ostream &operator<<(ostream &os, const book &rhs);
#endif //BOOKSTORE_BOOK_H
