#include "user.h"
void init()
{
    char tmpId[31] = "root";
    account boss(tmpId, "sjtu", "crm", 7);
    auto res = userBlock.find(tmpId);
    if (res.first) return;
    userBlock.insert(tmpId, boss);
}

void su(strScanner &scanner)
{
    std::string input_id = scanner.nextStr();
    if (!scanner.check(input_id, 30, 0)) throw error("Invalid");
    std::string input_passwd = scanner.nextStr();
    if (!scanner.is_end()) throw error("Invalid");

    auto query_res = userBlock.find(input_id.c_str());
    if (!query_res.first) throw error("Invalid");
    account &tmp = query_res.second.back();
    if (input_passwd.empty())
    {
        if (userStack.empty()) throw error("Invalid");
        if (userStack.top().Privilege <= tmp.Privilege) throw error("Invalid");
    }
    else
    {
        if (strcmp(input_passwd.c_str(), tmp.Password) != 0) throw error("Invalid");
//        if (!scanner.check(input_passwd, 30, 0)) throw error("Invalid");
    }
    userStack.push(tmp);
    std::cout<<"userStack.empty() is "<<userStack.empty()<<'\n';
    std::cout<<"the top is "<<tmp.UserID<<"  "<<tmp.Username<<'\n';
}
void user_logout()
{
    if (userStack.empty()) throw error("Invalid");
    if (userStack.top().Privilege == 0) throw error("Invalid");
    userStack.pop();
}
void registerUser(strScanner &scanner)
{
    std::string input_id = scanner.nextStr();
    if (!scanner.check(input_id, 30, 0)) throw error("Invalid");
    std::string input_passwd = scanner.nextStr();
    if (!scanner.check(input_passwd, 30, 0)) throw error("Invalid");
    std::string input_name = scanner.nextStr();
    if (!scanner.check(input_name, 30, 1)) throw error("Invalid");
    if (!scanner.is_end()) throw error("Invalid");

    if (userBlock.find(input_id.c_str()).first) throw error("Invalid");
    account cur(input_id.c_str(), input_passwd.c_str(), input_name.c_str(), 1);
    userBlock.insert(input_id.c_str(), cur);
}
void passwd(strScanner &scanner)
{
    //firstly, the privilege restriction
    if (userStack.empty()) throw error("Invalid");
    if (userStack.top().Privilege < 1) throw error("Invalid");
    //secondly, the input format restriction
    std::string input_id = scanner.nextStr();
    if (!scanner.check(input_id, 30, 0)) throw error("Invalid");
    std::string input_passwd1 = scanner.nextStr();
    if (!scanner.check(input_passwd1, 30, 0)) throw error("Invalid");
    std::string input_passwd2 = scanner.nextStr();
    if (!input_passwd2.empty() && !scanner.check(input_passwd2, 30, 0)) throw error("Invalid");
    if (!scanner.is_end()) throw error("Invalid");
    //thirdly, the logic restriction
    auto query_res = userBlock.find(input_id.c_str());
    if (!query_res.first) throw error("Invalid");
    account cur = query_res.second.back();
    if (input_passwd2.empty())
    {
        if (userStack.empty()) throw error("Invalid");
        if (userStack.top().Privilege != 7) throw error("Invalid");
        account change_to = cur;
        strcpy(change_to.Password, input_passwd1.c_str());
        userBlock.modify(input_id.c_str(), cur, change_to);
    }
    else
    {
        if (strcmp(input_passwd1.c_str(), cur.Password) != 0) throw error("Invalid");
        account change_to = cur;
        strcpy(change_to.Password, input_passwd2.c_str());
        userBlock.modify(input_id.c_str(), cur, change_to);
    }
}
void useradd(strScanner &scanner)
{
    if (userStack.empty()) throw error("Invalid_useradd_1");
    if (userStack.top().Privilege < 3) throw error("Invalid_useradd_2");
    std::string input_id = scanner.nextStr();
    if (!scanner.check(input_id, 30, 0)) throw error("Invalid");
    auto query_res = userBlock.find(input_id.c_str());
    if (query_res.first) throw error("Invalid");
    std::string input_passwd = scanner.nextStr();
    if (!scanner.check(input_passwd, 30, 0)) throw error("Invalid");
    std::string str_num = scanner.nextStr();
    if (!scanner.check(str_num, 1, 5)) throw error("Invalid");
    int input_num = str_num[0] - '0';
    if (input_num >= userStack.top().Privilege) throw error("Invalid");
    std::string input_name = scanner.nextStr();
    if (!scanner.check(input_name, 30, 1)) throw error("Invalid");

    account cur(input_id.c_str(), input_passwd.c_str(), input_name.c_str(), input_num);
    userBlock.insert(input_id.c_str(), cur);
}
void deleteUser(strScanner &scanner)
{
    if (userStack.empty()) throw error("Invalid");
    if (userStack.top().Privilege < 7) throw error("Invalid");
    std::string input_id = scanner.nextStr();
    if (!scanner.check(input_id, 30, 0)) throw error("Invalid");
    auto query_res = userBlock.find(input_id.c_str());
    if (!query_res.first) throw error("Invalid");
    std::stack<account> tmp;
    bool login_flag = false;
    while(!userStack.empty() && !login_flag)
    {
        if (strcmp(userStack.top().UserID, input_id.c_str()) == 0) login_flag = true;
        tmp.push(userStack.top());
        userStack.pop();
    }
    while(!tmp.empty())
    {
        userStack.push(tmp.top());
        tmp.pop();
    }
    if (login_flag) throw error("Invalid");
    userBlock.del(input_id.c_str(), query_res.second.back());
}
