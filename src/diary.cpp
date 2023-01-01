#include <iomanip>
#include "diary.h"
void initDiary()
{
    auto query_res = diaryBlock.find(std::to_string(0).c_str());
    if (query_res.first) Count = (int)query_res.second.back();
    else Count = 0;
}
void writeCount()
{
    diaryBlock.insert(std::to_string(0).c_str(), Count);
}
void addDiary(const double &num)
{
    ++Count;
    diaryBlock.insert(std::to_string(Count).c_str(), num);
}
void showFinance(strScanner &scanner)
{
    if (isUserEmpty()) throw error("Invalid");
    if (getTopUser() < 7) throw error("Invalid");
    scanner.nextStr();
    if (!scanner.is_end())
    {
        std::string input_count = scanner.nextStr();
        if (!scanner.is_end()) throw error("Invalid");
        int digit_count = scanner.strToInt_quantityJudge(input_count);
        if (digit_count < 0 || digit_count > Count) throw error("Invalid");
        if (digit_count == 0)
        {
            putchar('\n');
            return;
        }
        double in = 0, out = 0;
        for (int i = 0; i < digit_count; ++i)
        {
            auto query_res = diaryBlock.find(std::to_string(Count - i).c_str());
            double tmp = query_res.second.back();
            if (tmp > 0) in += tmp;
            else out -= tmp;
        }
        std::cout<<std::fixed<<std::setprecision(2)<<"+ "<<in<<" - "<<out<<'\n';
    }
    else
    {
        double in = 0, out = 0;
        for (int i = 0; i < Count; ++i)
        {
            auto query_res = diaryBlock.find(std::to_string(Count - i).c_str());
            double tmp = query_res.second.back();
            if (tmp > 0) in += tmp;
            else out -= tmp;
        }
        std::cout<<std::fixed<<std::setprecision(2)<<"+ "<<in<<" - "<<out<<'\n';
    }
}