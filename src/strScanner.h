#ifndef BOOKSTORE_STRSCANNER_H
#define BOOKSTORE_STRSCANNER_H

#include <string>

class strScanner{
private:
    std::string line;
    int pos;

public:
    strScanner() {pos = 0;}

    explicit strScanner(const std::string &s) {line = s, pos = 0;}

    void init(const std::string &s) {line = s, pos = 0;}

    std::string nextStr();
    std::string nextStr_half();
    std::string nextStr_withoutQuotes();
    //处理"str"的情况
    std::string nextStr_specialJudge();
    bool keyword_specialJudge(const std::string &obj);
    int strToInt_quantityJudge(const std::string &obj);
    double strToDouble_doubleJudge(const std::string &obj);
    bool is_end();
    //0 : 数字，字母，下划线
    //1 : 除不可见字符以外 ASCII 字符(33 - 126)
    //2 : 数字
    //3 : 除不可见字符和英文双引号以外 ASCII 字符
    //4 : 数字和.
    //5 : 1和3和7
    bool check (const std::string &s, int max_len, int mode);
};
#endif //BOOKSTORE_STRSCANNER_H
