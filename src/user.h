#ifndef BOOKSTORE_USER_H
#define BOOKSTORE_USER_H

#include "linkedList.h"
#include "strScanner.h"
#include "error.cpp"
#include <stack>

class account {
public:
    char UserID[31];
    char Password[31];
    char Username[31];
    int Privilege;

public:
    account() {}
    account(const char *id_, const char *password_, const char *name_, const int num_)
    {
        strcpy(UserID, id_);
        strcpy(Password, password_);
        strcpy(Username, name_);
        Privilege = num_;
    }
    account(const account &rhs)
    {
        strcpy(UserID, rhs.UserID);
        strcpy(Password, rhs.Password);
        strcpy(Username, rhs.Username);
        Privilege = rhs.Privilege;
    }
    account &operator=(const account &rhs)
    {
        if (this == &rhs) return *this;
        strcpy(UserID, rhs.UserID);
        strcpy(Password, rhs.Password);
        strcpy(Username, rhs.Username);
        Privilege = rhs.Privilege;
        return *this;
    }
    bool operator<(const account &rhs) const {return strcmp(UserID, rhs.UserID) < 0;}
    bool operator>(const account &rhs) const {return strcmp(UserID, rhs.UserID) > 0;}
};
linkedList<account> userBlock("users");
std::stack<account> userStack;

void init();
void su(strScanner &scanner);
void logout();
void registerUser(strScanner &scanner);
void passwd(strScanner &scanner);
void useradd(strScanner &scanner);
void deleteUser(strScanner &scanner);
#endif //BOOKSTORE_USER_H
