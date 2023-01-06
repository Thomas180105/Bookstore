#include <iostream>
#include "src/strScanner.h"
#include "src/error.cpp"
#include "src/book.h"
#include "src/user.h"
#include "src/diary.h"
#include "src/linkedList.h"
/*
# 基础指令
quit
exit

# 帐户系统指令
su [UserID] ([Password])?
logout
register [UserID] [Password] [Username]
passwd [UserID] ([CurrentPassword])? [NewPassword]
useradd [UserID] [Password] [Privilege] [Username]
delete [UserID]

# 图书系统指令
show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?
buy [ISBN] [Quantity]
select [ISBN]
modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
import [Quantity] [TotalCost]

# 日志系统指令
show finance ([Count])?
log*/
bool exitFlag = false;
int main()
{
    std::string working;
    init();
    initDiary();
    while (!exitFlag && getline(std::cin, working))
    {
        try{
//            std::cout<<working<<'\n';
            strScanner scanner(working);
            std::string op = scanner.nextStr();
            if (op.empty()) continue;
            if (op == "quit" || op == "exit") exitFlag = true;
            else if (op == "su") {su(scanner); bookIn();}
            else if (op == "logout") {user_logout(); bookOut();}
            else if (op == "register") registerUser(scanner);
            else if (op == "passwd") passwd(scanner);
            else if (op == "useradd") useradd(scanner);
            else if (op == "delete") deleteUser(scanner);
            else if (op == "show")
            {
                std::string tmp = scanner.nextStr_preview();
                if (tmp == "finance") showFinance(scanner);
                else show(scanner);
            }
            else if (op == "buy") buy(scanner);
            else if (op == "select") select(scanner);
            else if (op == "modify") modify(scanner);
            else if (op == "import") import(scanner);
            else throw error("Invalid");
        }
        catch (error &obj){
            std::cout<<obj.toString()<<'\n';
        }
    }
    writeCount();
    return 0;
}
//rm Book_Author_storage Book_ISBN_storage Book_BookName_storage Book_Keyword_storage User_storage
