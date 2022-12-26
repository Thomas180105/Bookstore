#ifndef ERROR
#define ERROR
#include <iostream>
//引用了来自期末模拟机考T1的异常类
class error : public std::exception {
private:
    std::string msg;

public:
    explicit error(const char *_msg_) : msg(_msg_) {}

    const char *toString() {
        return msg.c_str();
    }
};
#endif