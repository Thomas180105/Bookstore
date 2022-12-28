#include "book.h"
#include "user.h"
#include <iomanip>
ostream &operator<<(ostream &os, const book &rhs)
{
    os<<rhs.ISBN<<'\t'<<rhs.BookName<<'\t'<<rhs.Author<<'\t'<<rhs.Keyword<<'\t';
    os<<std::fixed<<std::setprecision(2)<<rhs.Price<<'\t'<<rhs.Book_cnt<<'\n';
    return os;
}
void addBook(const book &obj)
{
    bookBlock_ISBN.insert(obj.ISBN, obj);
    bookBlock_BookName.insert(obj.BookName, obj);
    bookBlock_Author.insert(obj.Author, obj);
    std::string cur;
    for (const auto &ch : obj.Keyword)
    {
        if (ch == '|')
        {
            bookBlock_Keyword.insert(cur.c_str(), obj);
            cur = "";
        }
        else cur += ch;
    }
    if (!cur.empty()) bookBlock_Keyword.insert(cur.c_str(), obj);
}
void delBook(const book &obj)
{
    bookBlock_ISBN.del(obj.ISBN, obj);
    bookBlock_BookName.del(obj.BookName, obj);
    bookBlock_Author.del(obj.Author, obj);
    std::string cur;
    for (const auto &ch : obj.Keyword)
    {
        if (ch == '|')
        {
            bookBlock_Keyword.del(cur.c_str(), obj);
            cur = "";
        }
        else cur += ch;
    }
    if (!cur.empty()) bookBlock_Keyword.del(cur.c_str(), obj);
}
//如果一本书obj修改前后没有修改ISBN、BookName、Author、keyWord，那么直接调用modifyBook(obj, obj)即可
//否则需要先将obj修改前的值进行拷贝，然后对于obj修改后调用modifyBook(his_, cur_);
void modifyBook(const book &his_book, const book &new_book)
{
    delBook(his_book);
    addBook(new_book);
}
void bookIn(const book &cur)
{
    bookStack.push(cur);
}
void bookOut()
{
    if (bookStack.empty()) throw error("Invalid");
    bookStack.pop();
}
void show(strScanner &scanner)
{
    if (userStack.empty()) throw error("Invalid");
    if (userStack.top().Privilege < 1) throw error("Invalid");
    bool notFound = true;

    if (scanner.is_end())
    {
        auto ans = bookBlock_ISBN.findAll();
        if (!ans.empty()) notFound = false;
        for (const auto& i : ans) std::cout<<i;
    }
    else
    {
        std::string type = scanner.nextStr_half();
        if (type.empty()) throw error("Invalid");
        if (type == "ISBN")
        {
            std::string input_ISBN = scanner.nextStr();
            if (!scanner.check(input_ISBN, 20, 1)) throw error("Invalid");
            if (!scanner.is_end()) throw error("Invalid");
            auto query_res = bookBlock_ISBN.find(input_ISBN.c_str());
            if (query_res.first)
            {
                notFound = false;
                std::cout<<query_res.second.back();
            }
        }
        else if (type == "name")
        {
            std::string input_name = scanner.nextStr_specialJudge();
            if (!scanner.check(input_name, 60, 3)) throw error("Invalid");
            if (!scanner.is_end()) throw error("Invalid");
            auto query_res = bookBlock_BookName.find(input_name.c_str());
            if (query_res.first)
            {
                notFound = false;
                for (const auto& i : query_res.second) std::cout<<i;
            }
        }
        else if (type == "author")
        {
            std::string input_author = scanner.nextStr_specialJudge();
            if (!scanner.check(input_author, 60, 3)) throw error("Invalid");
            if (!scanner.is_end()) throw error("Invalid");
            auto query_res = bookBlock_Author.find(input_author.c_str());
            if (query_res.first)
            {
                notFound = false;
                for (const auto& i : query_res.second) std::cout<<i;
            }
        }
        else if (type == "keyword")
        {
            std::string input_keyword = scanner.nextStr_specialJudge();
            if (!scanner.check(input_keyword, 60, 3)) throw error("Invalid");
            if (!scanner.is_end()) throw error("Invalid");
            for (char i : input_keyword) if (i == '|') throw error("Invalid");
            auto query_res = bookBlock_Keyword.find(input_keyword.c_str());
            if (query_res.first)
            {
                notFound = false;
                for (const auto& i : query_res.second) std::cout<<i;
            }
        }
        else throw error("Invalid");
    }

    if (notFound) putchar('\n');
}
void buy(strScanner &scanner)
{
    if (userStack.empty()) throw error("Invalid");
    if (userStack.top().Privilege < 1) throw error("Invalid");
    std::string input_ISBN = scanner.nextStr();
    if (!scanner.check(input_ISBN, 20, 1)) throw error("Invalid");
    std::string str_quantity = scanner.nextStr();
    if (!scanner.is_end()) throw error("Invalid");
    int digit_quantity = scanner.strToInt_quantityJudge(str_quantity);
    if (digit_quantity == -1) throw error("Invalid");
    auto query_res = bookBlock_ISBN.find(input_ISBN.c_str());
    if (!query_res.first) throw error("Invalid");
    book cur = query_res.second.back();
    if (cur.Book_cnt < digit_quantity) throw error("Invalid");

    cur.Book_cnt -= digit_quantity;
    modifyBook(cur, cur);
    std::cout<<std::fixed<<std::setprecision(2)<<cur.Price * digit_quantity<<'\n';
}