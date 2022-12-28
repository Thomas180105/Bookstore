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
bool strScanner::keyword_specialJudge(const std::string &obj)
{
    //必须互不相同,关键词不可以为空
    std::string element[61];
    int cnt = 0;
    for (const auto &ch : obj)
    {
        if (ch == '|')
        {
            if (element[cnt].empty()) return false;
            ++cnt;
        }
        else element[cnt] += ch;
    }
    for (int i = 0; i <= cnt; ++i)
    {
        for (int j = i + 1; j <= cnt; ++j)
            if (element[i] == element[j]) return false;
    }
    return true;
}
int strScanner::strToInt_quantityJudge(const std::string &obj)
{
    int res = 0;
    if (obj.empty()) return -1;
    if (obj.size() > 10) return -1;
    for (auto ch : obj)
    {
        //如果是负数，此处会被排除
        if (!(ch >= '0' && ch <= '9')) return -1;
        res = res * 10 + ch - '0';
    }
    if (res <= 0) return -1;
    return res;
}
//obj保证为仅由数字和'.'组成（相当于顺带限制了不可以为负数），否则在check(mode为4)时就已经抛出异常了，根本不会调用此函数
double strScanner::strToDouble_doubleJudge(const std::string &obj)
{
    if (obj.empty()) return -1;
    if (obj.size() > 13) return -1;
    //"本系统中浮点数输入输出精度固定为小数点后两位",所以'.'的出现次数必为1且位置固定
    bool dotFlag = false;
    for (const auto &ch : obj)
    {
        if (ch == '.')
        {
            if (dotFlag) return -1;
            else dotFlag = true;
        }
    }
    if (obj[obj.size() - 3]) return -1;

    return stod(obj);
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
