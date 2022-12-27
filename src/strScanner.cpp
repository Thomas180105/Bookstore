#include "strScanner.h"

std::string strScanner::nextStr()
{
    int st = pos;
    //鲁棒性警告！
    if (line[st] == '\0') return "";
    while (line[pos] == ' ') {++st, ++pos;}
    while (line[pos] != ' ' && line[pos] != '\0') {++pos;}
    return line.substr(st, pos - st);
}
std::string strScanner::nextStr_half()
{
    int st = pos;
    if (line[st] == '\0') return "";
    while (line[pos] == ' ') {++st, ++pos;}
    if (line[pos] != '-') return "";
    ++st, ++pos;
    while (line[pos] != ' ' && line[pos] != '\0' && line[pos] != '=') {++pos;}
    if (line[pos] == ' ' || line[pos] == '\0') return "";
    ++pos;
    return line.substr(st, (pos - 1) - st);
}
std::string strScanner::nextStr_specialJudge()
{
    int st = pos;
    if (line[pos] != '\"') return "";
    ++st, ++pos;
    while (line[pos] != '\0' && line[pos] != '\"' && line[pos] != ' ') {++pos;}
    if (line[pos] != '\"') return "";
    ++pos;
    return line.substr(st, (pos - 1) -st);
}
bool strScanner::is_end()
{
    while (line[pos] == ' ') ++pos;
    if (line[pos] == '\0') return true;
    return false;
}
bool strScanner::check(const std::string &s, int max_len, int mode)
{
    if (s.empty() || s.size() > max_len) return false;
    int num;
    switch (mode){
        case 0:
            for (auto ch : s)
                if (!(ch >= '0' && ch <= '9' || ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z' || ch == '_')) return false;
            break;
        case 1:
            for (auto ch : s)
                if (!(ch >= 33 && ch <= 126)) return false;
            break;
        case 2:
            for (auto ch : s)
                if (!(ch >= '0' && ch <= '9')) return false;
            break;
        case 3:
            for (auto ch : s)
                if (ch == '\"' || !(ch >= 33 && ch <= 126)) return false;
            break;
        case 4:
            for (auto ch : s)
                if ((ch != '.' ) && !(ch >= '0' && ch <= '9')) return false;
            break;
        case 5:
            num = s[0] - '0';
            if (!(num == 1 || num == 3 || num == 7)) return false;
        default:
            break;
    }
    return true;
}
