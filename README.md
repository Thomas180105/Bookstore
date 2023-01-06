# 开发文档

（阅读以下文本前请先阅读“requirements.md”）

## 助教提示：

~~~
（0）string的实现是存储一个指针以及一个长度，所以不能储存在外存，要实现外部存储请使用char[]

（1）维护用户栈不需要可持续，直接放在内存即可

（2）注意选择图书也需要维护一个栈，因为当前用户pop之后，新的栈顶的用户所选择的图书保持该用户之前的选择

（3）图书维护的信息不包括[totalPrice]
~~~

## 项目名称与文档作者

Bookstore 陈瑞茗

## 程序功能概述

整体来说，涉及到的三方关系为：账户、图书、书店（日志系统）

### 关于账户

* 维护的信息：[UserID], [Password], [Username] ，[Privilege]

* 支持的操作如下：注册、创建、登入、登出、删除、修改密码

### 关于图书

* 维护的信息：[ISBN] [BookName], [Author] [Keyword] [Quantity] [Price]

* 支持的操作：检索、购买、选择、修改信息（需先选择）、进货

### 关于书店

* 维护的信息：[Count]、[each finance]、[each operation]

* 支持的操作:财务记录查询、输出日志（所有[each finance][each operation]）

## 主体逻辑说明

### 账户对象的创建与储存

虽然存在shopkeeper、employee、customer、visitor这四种权限不同的类别，但为了块状链表统一化储存、仅实现一个类：User，类中储存的Privilege作为不同指令的准入条件，这样也可以实现分权限提供不同扩展指令的要求，由User构造的对象以UserID为关键字储存在块状链表中。

### 账户系统指令
首先维护一个栈（栈中元素为账户的[UserID]），初始时用户栈中有且仅有一个对应于初始游客的User对象，之后的账户系统指令可以分为三类：一类是对于用户栈的维护（用户的登入、登出），这需要用到块状链表的查找；一类是对于块状链表的维护（用户的注册、创建、删除），这需要用到块状链表的插入与删除；最后一类是对用户对象的维护（修改密码），这需要用到块状链表的查找。

### 图书对象的创建与储存
实现一个类Book，由Book构造的对象分别以[ISBN]、[Bookname]、[Author]、[keyword]为关键字储存在不同的块状链表中（以便后续检索）。

### 图书系统指令
首先创建一个全局的用户栈存储当前选中的图书。之后的操作分为三类：一类是检索图书，这需要对相对应的块状链表进行查找；一类是修改now_selected；一类是以ISBN为关键字对于图书对象进行维护，包括购买、修改信息、图书进货，注意维护对象的同时也需要对块状链表进行维护。

### 日志相关信息的存储
用一个元素类型为string的数组保存财务记录、用一个文件存储日志（即财务记录）。注意引起数组更新的操作包括图书进货与图书购买

### 日志系统指令
财务记录查询直接访问上述数组即可、生成日志直接输出日志文件

## 代码文件结构

~~~
* strScanner.cpp与strScanner.h用于处理字符串的拆分以及非法性判断

* Error.cpp用于报错

* linkedList.h实现了块状链表类；

* User.cpp与User.h在使用块状链表类的基础上实现了用户类；

* Book.cpp与Book.h在使用块状链表类的基础上实现了图书类；

* diary.cpp与diary.h在使用块状链表类的基础上实现了日志系统

* main.cpp使用了块状链表类、用户类、图书类，实现了程序的功能
~~~
## 类的接口与成员说明：

### strScanner类：
~~~
* 维护的成员 std::string line; int pos;

* std::string nextStr() 读取下一个词（以空格和‘\0’为分隔符），有非法性判断

* std::string nextStr_preview() 预读取(不修改pos值)下一个词（以空格和‘\0’为分隔符），有非法性判断

* std::string nextStr_half() 读取下一个词（以‘=’为终止符），有非法性判断

* std::string nextStr_withoutQuotes() 读取下一个词，去掉首位的引号，有非法性判断

* std::string nextStr_specialJudge(); 对于首位有引号的字符串的特殊非法性检验

* std::string keyword_specialJudge();对于关键词的特殊非法性判断

* int strScanner::strToInt_quantityJudge(const std::string &obj)为int值读取，且有特殊非法性判断

* double strScanner::strToDouble_quantityJudge(const std::string &obj)为double值读取，且有特殊非法性判断

* bool is_end();判断是否当前字符串已经结束

* bool check（）； 按照类型进行初步非法性判断
~~~
### Error类
继承自std：：exception,实现了简易的构造函数和向std::string的转换函数

### linkedList类：
~~~
linkedList类基于文件实现了char[] -> T的键值对存储，允许同一key对应多个值T

std::pair<bool, std::vector<T>> find(char index_[]) 支持同一char[]对应多个T
void insert(char index_[], T value_)
void del(char index_[], T value_)
std::vector<T> findAll()
void modify(char index_[], T his_value, T new_value) 使用条件：保证当前模式是one key - one value

~~~
### User类：
~~~
* 成员：char UserID[31], char Password[31], char Username[31]，int Privilege;

* void su(strScanner &scanner);登录账户

* void user_logout();注销账户（登出）

* void registerUser(strScanner &scanner)；注册账户

* void passwd(strScanner &scanner); 修改密码

* void useradd（strScanner &scanner）;创建账户

* void deleteUser(strScanner &scanner); 删除用户
~~~
### Book类

工具函数如下：
~~~
* void addBook(const book &obj);在多个存储图书相关的文件中添加当前图书

* void delBook(const book &obj);在多个存储图书相关的文件中删除当前图书

* void bookIn（）； void bookOut（）；为对于图书栈的处理
~~~
对外接口如下：
~~~
* void show(strScanner &scanner); 为按照不同的检录关键词来展示对应图书

* void buy(strScanner &scanner);用户购买书

* void select(strScanner &scanner);选中图书

* void modify(strScanner &scanner);不同情况下的图书信息修改

* void import(strScanner &scanner);书店购买书
~~~
### diary类：
~~~
* void initDiary();读取当前收支数量count,每次启动系统时调用

* void writeCount();将当前收支数量count写入文件,每次关闭系统时调用

* void addDiary(const double &num);写一条收支记录

* void showFinance(strScanner &scanner);展示收支情况
~~~
## 文件存储说明：
~~~
文件User_storage:存储一个key为UserID，value为User对象的键值数据库

文件Book_ISBN_storage:存储一个key为[ISBN],value为Book对象的键值数据库

文件Book_Bookname_storage：存储一个key为[bookname],value为Book对象的键值数据库

文件Book_Author_storage:存储一个key为[Author],value为Book对象的键值数据库

文件Book_keyword_storage:存储一个key为[keyword],value为Book对象的键值数据库

文件User_storage:存储一个key为编号,value为double的键值数据库
~~~
## 其他说明
项目未AC，在complex2和complex4中出现了WA的情况，错误的情况大致是7000行输出中有10行以内的错误，可能是linkedList的bug