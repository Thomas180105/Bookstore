#include "book.h"
#include "user.h"
#include <iomanip>
ostream &operator<<(ostream &os, const book &rhs)
{
    os<<rhs.ISBN<<'\t'<<rhs.BookName<<'\t'<<rhs.Author<<'\t'<<rhs.Keyword<<'\t';
    os<<std::fixed<<std::setprecision(2)<<rhs.Price<<'\t'<<rhs.Book_cnt<<'\n';
    return os;
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